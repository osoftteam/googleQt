#include <iostream>
#include <functional>
#include <QBuffer>
#include <QFile>
#include <QFileInfo>
#include <iomanip>
#include "GmailCommands.h"
#include "google/demo/ApiTerminal.h"
#include "gmail/GmailRoutes.h"

static QString size_human(qreal num)
{
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while(num >= 1024.0 && i.hasNext())
        {
            unit = i.next();
            num /= 1024.0;
        }
    return QString().setNum(num, 'f', 2) + " " + unit;
}

GmailCommands::GmailCommands(GoogleClient& c):m_c(c)
{
    m_gm = m_c.gmail();
};

void GmailCommands::listMessages(QString nextToken, QString labelIds)
{
    try
        {
            gmail::ListArg listArg;
            listArg.setMaxResults(10);
            listArg.setPageToken(nextToken);
            if(!labelIds.isEmpty()){
                listArg.labels() = labelIds.split(" ");
            }
            
            gmail::IdArg msgID;
            msgID.setFormat("metadata");
            msgID.headers().push_back("Subject");
            msgID.headers().push_back("From");
            
            int n = 1;
            auto mlist = m_gm->getMessages()->list(listArg);
            for(auto m : mlist->messages())
                {
                    msgID.setId(m.id());
                    //std::cout << n << ". " << m.id() << std::endl;
                    
                    auto msg_resource = m_gm->getMessages()->get(msgID);
                    auto payload = msg_resource->payload();
                    auto header_list = payload.headers();
                    std::cout << n << ". " << msg_resource->id() << " ";
                    for(auto h : header_list)
                        {
                            std::cout << h.value()
                                      << " ";
                        }
                    std::cout << std::endl;                    
                    
                    n++;
                }
            nextToken = mlist->nextpagetoken();
            std::cout << "next-token: " << nextToken << std::endl;
            //            m_c.printLastApiCall();   
        }
    catch(GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GmailCommands::listThreads(QString nextToken, QString labelIds)
{
    try
    {
        gmail::ListArg listArg;
        listArg.setMaxResults(40);
        listArg.setPageToken(nextToken);
        if (!labelIds.isEmpty()) {
            listArg.labels() = labelIds.split(" ");
        }

        int idx = 1;
        auto threads_list = m_gm->getThreads()->list(listArg);
        for (auto t : threads_list->threads())
        {
            std::cout << idx++ << ". " << t.id() << std::endl;
        }

        nextToken = threads_list->nextpagetoken();
        std::cout << "next-token: " << nextToken << std::endl;
        //        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GmailCommands::listDrafts(QString nextToken)
{
    try
    {
        gmail::DraftListArg listArg;
        listArg.setMaxResults(40);
        listArg.setPageToken(nextToken);

        int idx = 1;
        auto drafts_list = m_gm->getDrafts()->list(listArg);
        for (auto d : drafts_list->drafts())
        {
            std::cout << idx++ << ". " << "id="<< d.id() << std::endl;
        }

        nextToken = drafts_list->nextpagetoken();
        if(!nextToken.isEmpty()){
            std::cout << "next-token: " << nextToken << std::endl;
        }
        //        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

bool GmailCommands::loadMessageFile(QString fileName, messages::MessageResource* msg)
{
    if (fileName.isEmpty())
    {
        std::cout << "Error, prepared message file expected: " << fileName << std::endl;
        return false;
    }

    QFile mf(fileName);
    if (!mf.open(QFile::ReadOnly)) {
        std::cout << "Error, failed to open prepared message file: " << fileName << std::endl;
        return false;
    }

    QByteArray ba = mf.readAll().toBase64();
    msg->setRaw(ba);
    return true;
};

void GmailCommands::ls(QString nextToken)
{
    listMessages(nextToken, "");
};

void GmailCommands::ls_by_labels(QString labelIds)
{
    listMessages("", labelIds);
};

void GmailCommands::send(QString message_file_name) 
{
    try
    {
        gmail::SendMessageArg arg;
        messages::MessageResource body;
        if (!loadMessageFile(message_file_name, &body)) 
            return;
        printMessage(&body);
        return;
        auto r = m_gm->getMessages()->send(arg, body);
        printMessage(r.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GmailCommands::importMessage(QString message_file_name)
{
    try
    {
        gmail::ImportMessageArg arg;
        messages::MessageResource body;
        if (!loadMessageFile(message_file_name, &body))
            return;
        auto r = m_gm->getMessages()->importMessage(arg, body);
        printMessage(r.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GmailCommands::insertMessage(QString message_file_name)
{
    try
    {
        gmail::InsertMessageArg arg;
        messages::MessageResource body;
        if (!loadMessageFile(message_file_name, &body))
            return;
        auto r = m_gm->getMessages()->insert(arg, body);
        printMessage(r.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};


void GmailCommands::trash(QString message_id) 
{   
    try
    {
        gmail::TrashMessageArg arg(message_id);
        m_gm->getMessages()->trash(arg);
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GmailCommands::untrash(QString message_id) 
{
    try
    {
        gmail::UntrashMessageArg arg(message_id);
        m_gm->getMessages()->untrash(arg);
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GmailCommands::printMessage(messages::MessageResource* r)
{
    std::set<QString> headers_to_print = {"From", "To", "Subject"};
    
    std::cout << "rid="<< r->id() << std::endl
              << "tid=" << r->threadid() << std::endl
              << "snippet=" << r->snippet() << std::endl;
    const std::list <QString>& labels = r->labelids();
    if(labels.size() > 0){
        std::cout << "labels=";
        for (auto lb : labels) {
            std::cout << lb << " ";
        }
        std::cout << std::endl;
    }
    auto p = r->payload();
    std::cout << "type: " << p.mimetype() << std::endl;
    auto header_list = p.headers();
    std::cout << "headers count: " << header_list.size() << std::endl;
    for(auto h : header_list)
        {
            if(headers_to_print.find(h.name()) != headers_to_print.end())
                std::cout << h.name().leftJustified(20, ' ') << h.value() << std::endl;
        }

    auto parts = p.parts();
    std::cout << "parts count: " << parts.size() << std::endl;
    for(auto pt : parts)
        {
            const messages::MessagePartBody& pt_body = pt.body();
            QString partID = pt.partid();
            std::cout << "------------------------------------------------"
                      << "part" << partID << "---------------------------------"
                      << std::endl;            
            std::cout << pt.mimetype() << " body-size:" << pt_body.size() << std::endl;
            bool is_plain_text = false;
            bool is_html_text = false;
            auto pt_headers = pt.headers();
            for(auto h : pt_headers)
                {
                    if(h.name() == "Content-Type"){
                        is_plain_text = (h.value().indexOf("text/plain") == 0);
                        is_html_text = (h.value().indexOf("text/html") == 0);
                    }
                    std::cout << ""<< h.name().leftJustified(20, ' ') << " " << h.value() << std::endl;
                }
            if(is_plain_text || is_html_text)
                {
                    std::cout << QByteArray::fromBase64(pt_body.data(), QByteArray::Base64UrlEncoding).constData() << std::endl;
                }
        }
};

void GmailCommands::exportMessageBody(messages::MessageResource* r, QString fileName)
{
    auto p = r->payload();
    auto parts = p.parts();
    for(auto pt : parts)
        {
            for(auto pt : parts)
                {
                    const messages::MessagePartBody& pt_body = pt.body();            
                    auto pt_headers = pt.headers();
                    for(auto h : pt_headers)
                        {
                            bool is_html_text = false;
                            if(h.name() == "Content-Type"){
                                is_html_text = (h.value().indexOf("text/html") == 0);
                            }
                            if(is_html_text){
                                QByteArray body_part = QByteArray::fromBase64(pt_body.data(), QByteArray::Base64UrlEncoding);
                                QFile file_in(fileName);
                                if (!file_in.open(QFile::WriteOnly)) {
                                    qWarning() << "Error opening file: " << fileName;
                                    return;
                                }
                                file_in.write(body_part);
                                file_in.close();
                                std::cout << "exported: " << fileName << " " << size_human(body_part.size()) << std::endl;
                                return;
                            }
                        }//headers
                }//parts
        }

    std::cout << "HTML body not found" << std::endl;
};

void GmailCommands::get(QString msg_id)
{
    if(msg_id.isEmpty())
        {
            std::cout << "Missing parameters, expected <message_id>" << std::endl;
            return;
        }
    
    try
        {
            auto r = m_gm->getMessages()->get(gmail::IdArg(msg_id));
            printMessage(r.get());
        }
    catch(GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }            
};

void GmailCommands::get_raw(QString msg_id)
{
    if(msg_id.isEmpty())
        {
            std::cout << "Missing parameters, expected <message_id>" << std::endl;
            return;
        }
    
    try
        {
            auto r = m_gm->getMessages()->get(gmail::IdArg(msg_id, "raw"));
            printMessage(r.get());
        }
    catch(GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }            
};

void GmailCommands::get_html(QString message_id_space_fileName)
{
    QStringList arg_list = message_id_space_fileName.split(" ",
                                                           QString::SkipEmptyParts);
    if(arg_list.size() < 2)
        {
            std::cout << "Invalid parameters, expected <message_id> <file_name>" << std::endl;
            return;
        }

    QString msg_id = arg_list[0];
    QString fileName = arg_list[1];
    
    try
        {
            auto r = m_gm->getMessages()->get(gmail::IdArg(msg_id));
            exportMessageBody(r.get(), fileName);
        }
    catch(GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GmailCommands::ls_labels(QString )
{
    try
    {
        auto labels_list = m_gm->getLabels()->list();
        for (auto lbl : labels_list->labels())
        {
            std::cout << "id=" << lbl.id() << " name=" << lbl.name() << " type=" << lbl.type() << std::endl;
        }

    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GmailCommands::printLabel(labels::LabelResource* lbl)
{
    std::cout << "id=" << lbl->id()
              << " name=" << lbl->name() 
              << " type=" << lbl->type() 
              << " total=" << lbl->messagestotal()
              << " unread=" << lbl->messagesunread()
              << " threads_total=" << lbl->threadstotal()
              << std::endl;
};

void GmailCommands::get_label(QString label_id)
{
    try
    {
        auto lbl = m_gm->getLabels()->get(label_id);
        printLabel(lbl.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GmailCommands::create_label(QString name)
{
    try
    {
        labels::LabelResource lbl;
        lbl.setName(name).setMessagelistvisibility("show").setLabellistvisibility("labelShow");
        auto new_label = m_gm->getLabels()->create(lbl);
        printLabel(new_label.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GmailCommands::delete_label(QString label_id)
{
    try
    {
        gmail::IdArg l_id(label_id);
        m_gm->getLabels()->deleteOperation(l_id);
        std::cout << "label deleted" << std::endl;
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GmailCommands::update_label(QString labelid_space_name)
{
    QStringList arg_list = labelid_space_name.split(" ", QString::SkipEmptyParts);
    if(arg_list.size() < 2)
        {
            std::cout << "Invalid parameters, expected <label_id> <new_name>" << std::endl;
            return;
        }

    QString labelid = arg_list[0];
    QString name = arg_list[1];
    
    try
    {                
        gmail::IdArg l_id(labelid);
        labels::LabelResource lbl;
        lbl.setName(name).setMessagelistvisibility("show").setLabellistvisibility("labelShow");
        auto new_label = m_gm->getLabels()->update(l_id, lbl);
        printLabel(new_label.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};


void GmailCommands::ls_threads(QString nextToken)
{
    listThreads(nextToken, "");
};

void GmailCommands::get_thread(QString thread_id)
{
    try
    {
        auto t = m_gm->getThreads()->get(thread_id);
        std::cout << "tid=" << t->id() << " snipped=" << t->snipped() << " historyid=" << t->historyid() << " messagescount=" << t->messages().size() << std::endl;
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GmailCommands::history(QString startHistoryIdStr)
{
    bool int_ok;
    int startHistoryId = startHistoryIdStr.toInt(&int_ok);
    if(!int_ok){
        std::cout << "Please enter valid history ID" << std::endl;
        return;
    }
    
    try
    {
        gmail::HistoryListArg histArg(startHistoryId);

        auto history = m_gm->getHistory()->list(histArg);
        for (auto h : history->history())
        {
            std::cout << "id= " << h.id()
                << " messages=" << h.messages().size()
                << " added=" << h.messagesadded().size()
                << " deleted=" << h.messagesdeleted().size()
                << " labelsadded=" << h.labelsadded().size()
                << " labelsremoved=" << h.labelsremoved().size()
                << std::endl;
        }

        std::cout << "historyid=" << history->historyid() << "nextpagetoken=" << history->nextpagetoken()
            << std::endl;
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GmailCommands::ls_drafts(QString page_token)
{
    listDrafts(page_token);
};

void GmailCommands::get_draft(QString draft_id)
{
    try
    {
        auto d = m_gm->getDrafts()->get(draft_id);
        auto m = d->message();
        std::cout << "id=" << d->id() << std::endl;
        printMessage(&m);
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};


void GmailCommands::export_last_result(QString fileName)
{
    m_c.exportLast200Response(fileName);
};

void GmailCommands::print_last_result(QString )
{
    m_c.printLast200Response();
};
