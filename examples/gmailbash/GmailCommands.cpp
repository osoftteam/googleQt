#include <iostream>
#include <functional>
#include <QBuffer>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <iomanip>
#include "GmailCommands.h"
#include "google/demo/ApiTerminal.h"
#include "gmail/GmailRoutes.h"

GmailCommands::GmailCommands(GoogleClient& c):m_c(c)
{
    m_gm = m_c.gmail();
};

void GmailCommands::listMessages(QString nextToken, QString labelIds, QString query)
{
    try
        {
            gmail::ListArg listArg;
            listArg.setMaxResults(30);
            listArg.setPageToken(nextToken);
            if(!labelIds.isEmpty()){
                listArg.labels() = labelIds.split(" ");
            }
            if(!query.isEmpty()){
                listArg.setQ(query);
            }

            auto mlist = m_gm->getMessages()->list(listArg);
            QString id_list;
            for(auto m : mlist->messages())
                {
                    id_list += m.id();
                    id_list += " ";
                }

            get_batch_snippets(id_list);
            nextToken = mlist->nextpagetoken();
            if(!nextToken.isEmpty())
                {
                    std::cout << "next-token: " << nextToken << std::endl;
                }
        }
    catch(GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GmailCommands::listThreads(QString nextToken, QString labelIds, QString qstr)
{
    try
        {
            gmail::ListArg listArg;
            listArg.setMaxResults(20);
            listArg.setPageToken(nextToken);
            if (!labelIds.isEmpty()) {
                listArg.labels() = labelIds.split(" ");
            }
            if (!qstr.isEmpty()) {
                listArg.setQ(qstr);
            }

            int idx = 1;
            auto threads_list = m_gm->getThreads()->list(listArg);
            for (auto t1 : threads_list->threads())
                {
                    std::cout << idx++ << ". "
                        << "tid=" << t1.id()
                        << " historyid=" << t1.historyid();
                    std::cout << std::endl;
                }

            nextToken = threads_list->nextpagetoken();
            std::cout << "next-token: " << nextToken << std::endl;
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
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GmailCommands::base64url_encode(QString fileName)
{
    if (fileName.isEmpty())
        {
            std::cout << "Error, prepared message file expected: " << fileName << std::endl;
            return;
        }

    QFile mf(fileName);
    if (!mf.open(QFile::ReadOnly)) {
        std::cout << "Error, failed to open prepared message file: " << fileName << std::endl;
        return;
    }

    QByteArray ba = mf.readAll().toBase64(QByteArray::Base64UrlEncoding);
    std::cout << ba.constData() << std::endl;
};

void GmailCommands::base64url_decode(QString data)
{
    std::string str = data.toStdString();
    QByteArray ba = QByteArray::fromBase64(str.c_str(), QByteArray::Base64UrlEncoding);
    std::cout << ba.constData() << std::endl;
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

void GmailCommands::search(QString q)
{
    if(q.isEmpty()){
        std::cout << "Please provide search query" << std::endl;
        std::cout << "example ------------------" << std::endl;
        std::cout << "\"november rain\"" << std::endl;
        std::cout << "from:user@yahoo.com" << std::endl;
        std::cout << "from:fred OR from:anna" << std::endl;
        std::cout << "subject:toyota prius" << std::endl;
        std::cout << "has:drive has:document" << std::endl;
        std::cout << "is:unread" << std::endl;
        return;
    }
    
    listMessages("", "", q);
};

void GmailCommands::send_prepared_rfc822(QString messageFileName)
{
    if (messageFileName.isEmpty()){
        std::cout << "Invalid parameters, expected <file-name>" << std::endl;
        return; 
    }

    if (!QFileInfo::exists(messageFileName)) {
        std::cout << "File not found: " << messageFileName << std::endl;
        return;
    }

    try
        {
            gmail::SendMimeMessageArg arg;
            arg.setRawRfc822MessageFile(messageFileName);
            auto r = m_gm->getMessages()->send(arg);
            printMessage(r.get());
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GmailCommands::send_as_html(QString to_subject_text)
{
    QStringList arg_list = to_subject_text.split(" ",
                                                 Qt::SkipEmptyParts);

    if (arg_list.size() < 3)
        {
            std::cout << "Invalid parameters, expected <To> <Subject> <Text>" << std::endl;
            return;
        }

    QString msg_to = arg_list[0];
    QString msg_subject = arg_list[1];
    QString msg_text = arg_list[2];
    QString msg_html = QString("<div dir=\"ltr\">%1</div>").arg(msg_text);
    
    try
        {
            gmail::SendMimeMessageArg arg(m_c.userId(), msg_to, msg_subject, msg_text, msg_html);
            auto r = m_gm->getMessages()->send(arg);
            printMessage(r.get());
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GmailCommands::send_att(QString to_subject_text)
{
    QStringList arg_list = to_subject_text.split(" ",
                                                 Qt::SkipEmptyParts);

    if (arg_list.size() < 3)
        {
            std::cout << "Invalid parameters, expected <To> <Subject> <Text>" << std::endl;
            return;
        }

    QString msg_to = arg_list[0];
    QString msg_subject = arg_list[1];
    QString msg_text = arg_list[2];
    QString msg_html = QString("<div dir=\"ltr\">%1</div>").arg(msg_text);

    QDir dir("upload");
    if (!dir.exists()) {
        std::cout << "Upload-directory not found." << std::endl;
        return;
    }

    QFileInfoList flist = dir.entryInfoList(QDir::Files);
    if (flist.size() == 0) {
        std::cout << "Upload-directory is empty, nothing to attach" << std::endl;
        return;
    }

    STRING_LIST attachments;
    for (auto& fi : flist) {
        attachments.push_back(fi.absoluteFilePath());
    }

    try
        {
            gmail::SendMimeMessageArg arg(m_c.userId(), msg_to, msg_subject, msg_text, msg_html);
            arg.addAttachments(attachments);
            auto r = m_gm->getMessages()->send(arg);
            printMessage(r.get());
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GmailCommands::trash(QString message_id) 
{   
    if (message_id.isEmpty()) {
        std::cout << "Invalid parameters, expected <msg-id>" << std::endl;
        return;
    }

    try
        {
            gmail::TrashMessageArg arg(m_c.userId(), message_id);
            m_gm->getMessages()->trash(arg);
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GmailCommands::untrash(QString message_id)
{
    if (message_id.isEmpty()) {
        std::cout << "Invalid parameters, expected <msg-id>" << std::endl;
        return;
    }

    try
        {
            gmail::UntrashMessageArg arg(m_c.userId(), message_id);
            m_gm->getMessages()->untrash(arg);
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GmailCommands::delete_msg(QString message_id)
{
    if (message_id.isEmpty()) {
        std::cout << "Invalid parameters, expected <msg-id>" << std::endl;
        return;
    }

    try
        {
            gmail::IdArg arg(m_c.userId(), message_id);
            m_gm->getMessages()->deleteOperation(arg);
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};


void GmailCommands::add_label(QString message_id_label)
{
    QStringList arg_list = message_id_label.split(" ",
                                                  Qt::SkipEmptyParts);

    if (arg_list.size() < 2)
        {
            std::cout << "Invalid parameters, expected <message-ID> <label>" << std::endl;
            return;
        }

    QString msg_id = arg_list[0];
    QString msg_label = arg_list[1];

    
    try
        {
            gmail::ModifyMessageArg arg(m_c.userId(), msg_id);
            arg.addAddLabel(msg_label);
            std::unique_ptr<messages::MessageResource> m = m_gm->getMessages()->modify(arg);
            std::cout << "Label added: " << msg_label << std::endl;
            printMessage(m.get());
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GmailCommands::remove_label(QString message_id_label)
{
    QStringList arg_list = message_id_label.split(" ",
                                                  Qt::SkipEmptyParts);

    if (arg_list.size() < 2)
        {
            std::cout << "Invalid parameters, expected <message-ID> <label>" << std::endl;
            return;
        }

    QString msg_id = arg_list[0];
    QString msg_label = arg_list[1];
    
    try
        {
            gmail::ModifyMessageArg arg(m_c.userId(), msg_id);
            arg.addRemoveLabel(msg_label);
            std::unique_ptr<messages::MessageResource> m = m_gm->getMessages()->modify(arg);
            std::cout << "Label removed: " << msg_label << std::endl;
            printMessage(m.get());        
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GmailCommands::printSnippet(messages::MessageResource* r)
{
    //    std::set<QString> headers_to_print = {"From", "To", "Subject", "CC", "BCC"};
    
    std::cout << "id="<< r->id() << std::endl
              << "tid=" << r->threadid() << std::endl
              << "snippet=" << r->snippet() << std::endl;
    auto& labels = r->labelids();
    if(labels.size() > 0){
        std::cout << "labels=";
        for (auto lb : labels) {
            std::cout << lb << " ";
        }
        std::cout << std::endl;
    }
}

static std::set<QString> headers_to_print = { "From", "To", "Subject", "CC", "BCC", "References" };

void GmailCommands::printMessage(messages::MessageResource* r)
{    
    std::cout << "id="<< r->id() << std::endl
              << "tid=" << r->threadid() << std::endl
              << "snippet=" << r->snippet() << std::endl;
    auto& labels = r->labelids();
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

    const messages::MessageMimeBody& payload_body = p.body();
    if(!payload_body.attachmentid().isEmpty() || payload_body.size() > 0)
        {
            std::cout << "payload_body "
                      << " attId:" << payload_body.attachmentid()
                      << " size:" << payload_body.size()
                      << " data:" << payload_body.data().constData()
                      << std::endl;
        }
    
    if (p.mimetype().compare("text/html") == 0)
        {
            QByteArray payload_body = QByteArray::fromBase64(p.body().data(), QByteArray::Base64UrlEncoding);
            std::cout << payload_body.constData() << std::endl;
        }
    else
        {
            auto parts = p.parts();
            std::cout << "parts count: " << parts.size() << std::endl;
            for (auto pt : parts)
                {
                    printMessagePart(pt, "", "part");
                    auto subparts = pt.parts();
                    if (subparts.size() > 0) {
                        std::cout << "subparts count: " << subparts.size() << std::endl;
                        for (auto spt : subparts)
                        {
                            printMessagePart(spt, "SSS", "subpart");
                        }
                    }
                }
        }
};


void GmailCommands::printMessagePart(const messages::MessagePart& pt, QString prefix, QString title)
{
    const messages::MessagePartBody& pt_body = pt.body();
    QString partID = pt.partid();
    std::cout << prefix << "------------------------------------------------"
        << title << partID << "---------------------------------"
        << std::endl;
    std::cout << pt.mimetype() << " body-size:" << pt_body.size() << std::endl;
    if (!pt_body.attachmentid().isEmpty()) {
        std::cout << "attachmentid:" << pt_body.attachmentid() << std::endl;
    }

    bool is_plain_text = false;
    bool is_html_text = false;
    auto pt_headers = pt.headers();
    for (auto h : pt_headers)
    {
        if (h.name() == "Content-Type") {
            is_plain_text = (h.value().indexOf("text/plain") == 0);
            is_html_text = (h.value().indexOf("text/html") == 0);
        }
        std::cout << "" << h.name().leftJustified(20, ' ') << " " << h.value() << std::endl;
    }
    if (is_plain_text || is_html_text)
    {
        std::cout << QByteArray::fromBase64(pt_body.data(), QByteArray::Base64UrlEncoding).constData() << std::endl;
    }
};

void GmailCommands::exportMessageBody(messages::MessageResource* r, QString fileName)
{
    QByteArray body_part;
    
    auto p = r->payload();
    if (p.mimetype().compare("text/html") == 0)
        {
            body_part = QByteArray::fromBase64(p.body().data(), QByteArray::Base64UrlEncoding);
        }
    else
        {
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
                                        body_part = QByteArray::fromBase64(pt_body.data(), QByteArray::Base64UrlEncoding);
                                        break;;
                                    }
                                }//headers
                        }//parts
                }//for
        }

    if(body_part.size() > 0)
        {
            QFile file_in(fileName);
            if (!file_in.open(QFile::WriteOnly)) {
                qWarning() << "Error opening file: " << fileName;
                return;
            }
            file_in.write(body_part);
            file_in.close();
            std::cout << "exported: " << fileName << " " << size_human(body_part.size()) << std::endl;
        }
    else
        {
            std::cout << "HTML body not found" << std::endl;
        }
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
            gmail::IdArg arg(m_c.userId(), msg_id);
            for (auto& h : headers_to_print) {
                arg.headers().push_back(h);
            }
            auto r = m_gm->getMessages()->get(arg);
            printMessage(r.get());
        }
    catch(GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GmailCommands::get_snippet(QString msg_id)
{
    if(msg_id.isEmpty())
        {
            std::cout << "Missing parameters, expected <message_id>" << std::endl;
            return;
        }
    
    try
        {
            gmail::IdArg mid(m_c.userId(), msg_id);
            mid.setFormat("minimal");
            auto r = m_gm->getMessages()->get(mid);
            printSnippet(r.get());
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

void GmailCommands::save_raw(QString msg_id)
{
    if(msg_id.isEmpty())
        {
            std::cout << "Missing parameters, expected <message_id>" << std::endl;
            return;
        }

    QString dest_dir = QString("download/%1").arg(msg_id);
    QDir att_dir;    
    if(!att_dir.mkpath(dest_dir)){
        std::cout << "Failed to create directory: " << dest_dir << std::endl;
        return;
    };
    
    try
        {
            auto r = m_gm->getMessages()->get(gmail::IdArg(msg_id, "raw"));
            QFile file_in(dest_dir + "/raw_msg.txt");
            if (file_in.open(QFile::WriteOnly)) {
                file_in.write(QByteArray::fromBase64(r->raw(), QByteArray::Base64UrlEncoding));
                file_in.close();
                std::cout << r->id()
                          << " " << r->raw().size()
                          << std::endl;
            }
        }
    catch(GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GmailCommands::get_html(QString message_id_space_fileName)
{
    QStringList arg_list = message_id_space_fileName.split(" ",
                                                           Qt::SkipEmptyParts);
    if(arg_list.size() < 2)
        {
            std::cout << "Invalid parameters, expected <message_id> <file_name>" << std::endl;
            return;
        }

    QString msg_id = arg_list[0];
    QString fileName = arg_list[1];
    
    try
        {
            auto r = m_gm->getMessages()->get(gmail::IdArg(m_c.userId(), msg_id));
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
            int idx = 1;
            std::cout << "------ system labels ----" << std::endl;
            auto labels_list = m_gm->getLabels()->list();
            for (auto lbl : labels_list->labels()){
                if (lbl.type() == "system") {
                    std::cout << idx << "."
                        << "id=" << lbl.id()
                        << " name=" << lbl.name()
                        << " type=" << lbl.type()
                        << " messagestotal=" << lbl.messagestotal()
                        << " unread=" << lbl.messagesunread()
                        << " threadstotal=" << lbl.threadstotal()
                        << std::endl;
                    idx++;
                }
            }

            std::cout << "--------------------" << std::endl;

            for (auto lbl : labels_list->labels()) {
                if (lbl.type() != "system") {
                    std::cout << idx << "."
                        << "id=" << lbl.id()
                        << " name=" << lbl.name()
                        << " type=" << lbl.type()
                        << " messagestotal=" << lbl.messagestotal()
                        << " unread=" << lbl.messagesunread()
                        << " threadstotal=" << lbl.threadstotal()
                        << std::endl;
                    idx++;
                }
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
            auto lbl = m_gm->getLabels()->get(gmail::IdArg(m_c.userId(), label_id));
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
            gmail::IdArg l_id(m_c.userId(), label_id);
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
    QStringList arg_list = labelid_space_name.split(" ", Qt::SkipEmptyParts);
    if(arg_list.size() < 2)
        {
            std::cout << "Invalid parameters, expected <label_id> <new_name>" << std::endl;
            return;
        }

    QString labelid = arg_list[0];
    QString name = arg_list[1];
    
    try
        {                
            gmail::IdArg l_id(m_c.userId(), labelid);
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

void GmailCommands::printThread(threads::ThreadResource* t)
{
    std::cout << "tid=" << t->id()
        << " snipped=" << t->snipped()
        << " historyid=" << t->historyid()
        << " messagescount=" << t->messages().size()
        << std::endl;
    for (auto& m : t->messages()) {
        std::cout << "    id=" << m.id()
            << " ";
        auto p = m.payload();
        auto header_list = p.headers();
        for (auto h : header_list)
        {
            if (headers_to_print.find(h.name())
                != headers_to_print.end())
                std::cout << h.name() << ":"
                << h.value();
        }
        std::cout << " ";
        std::cout << m.snippet();
        std::cout << std::endl;

        auto& labels = m.labelids();
        if (labels.size() > 0) {
            std::cout << "    labels=";
            for (auto lb : labels) {
                std::cout << lb << " ";
            }
            std::cout << std::endl;
        }
    }
};

void GmailCommands::ls_threads(QString nextToken)
{
    listThreads(nextToken, "", "");
};

void GmailCommands::ls_threads_by_labels(QString labelIds) 
{
    listThreads("", labelIds, "");
};

void GmailCommands::q_threads(QString qstr) 
{
    listThreads("", "", qstr);
};

void GmailCommands::get_thread(QString id_list)
{
    static std::set<QString> headers_to_print = {"From", "To"};
    STRING_LIST arg_list = split_string(id_list);
    if (arg_list.empty()) 
        {
            std::cout << "Space separated thread ID list required" << std::endl;
            return;
        }
    
    try
        {
            for(auto s : arg_list){
                auto t = m_gm->getThreads()->get(gmail::IdArg(m_c.userId(), s));
                printThread(t.get());
            }
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GmailCommands::add_thread_label(QString threadid_labelids)
{
    STRING_LIST arg_list = split_string(threadid_labelids);
    if (arg_list.size() < 2)
    {
        std::cout << "Space separated threadId label ID list required" << std::endl;
        return;
    }

    auto it = arg_list.begin();
    QString threadid = *it;
    it++;

    try
    {
        gmail::ModifyMessageArg arg(m_c.userId(), threadid);
        for (; it != arg_list.end(); it++) {
            arg.addAddLabel(*it);
        }
        std::unique_ptr<threads::ThreadResource> t = m_gm->getThreads()->modify(arg);
        printThread(t.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

///..
void GmailCommands::remove_thread_label(QString threadid_labelids)
{
    STRING_LIST arg_list = split_string(threadid_labelids);
    if (arg_list.size() < 2)
    {
        std::cout << "Space separated threadId label ID list required" << std::endl;
        return;
    }

    auto it = arg_list.begin();
    QString threadid = *it;
    it++;

    try
    {
        gmail::ModifyMessageArg arg(m_c.userId(), threadid);
        for (; it != arg_list.end(); it++) {
            arg.addRemoveLabel(*it);
        }
        std::unique_ptr<threads::ThreadResource> t = m_gm->getThreads()->modify(arg);
        printThread(t.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

///...

void GmailCommands::history(QString hist_id_res_num)
{
    QString startHistoryIdStr;
    int MaxResults = 20;
    STRING_LIST arg_list = split_string(hist_id_res_num);
    if (arg_list.size() == 1)
    {
        startHistoryIdStr = arg_list[0];
    }
    else if (arg_list.size() > 1)
    {
        startHistoryIdStr = arg_list[0];
        MaxResults = arg_list[1].toInt();
    }

    bool int_ok;
    int startHistoryId = startHistoryIdStr.toInt(&int_ok);
    if(!int_ok){
        std::cout << "Please enter valid history ID" << std::endl;
        return;
    }
    
    try
        {
            gmail::HistoryListArg histArg(m_c.userId(), startHistoryId);
            histArg.setMaxResults(MaxResults);

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

            std::cout << "historyid=" << history->historyid() << " nextpagetoken=" << history->nextpagetoken()
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
            auto d = m_gm->getDrafts()->get(gmail::IdArg(m_c.userId(), draft_id));
            auto m = d->message();
            std::cout << "id=" << d->id() << std::endl;
            printMessage(&m);
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GmailCommands::download_attachments(QString msgId)
{
    if(msgId.isEmpty())
        {
            std::cout << "Missing parameters, expected <message_id>" << std::endl;
            return;
        }

    QString dest_dir = QString("download/%1").arg(msgId);
    QDir att_dir;    
    if(!att_dir.mkpath(dest_dir)){
        std::cout << "Failed to create directory: " << dest_dir << std::endl;
        return;
    };
    
    try
        {
            gmail::IdArg arg(m_c.userId(), msgId);
            auto r = m_gm->getMessages()->get(arg);
            auto p = r->payload();
            auto parts = p.parts();
            for (auto pt : parts){
                auto b = pt.body();
                if(b.size() > 0 && !b.attachmentid().isEmpty()){
                    gmail::AttachmentIdArg arg(m_c.userId(), msgId, b.attachmentid());
                    auto r = m_gm->getAttachments()->get(arg);

                    QFile file_in(dest_dir + "/" + pt.filename());
                    if (file_in.open(QFile::WriteOnly|QIODevice::Truncate)) {
                        file_in.write(QByteArray::fromBase64(r->data(), QByteArray::Base64UrlEncoding));
                        file_in.close();
                        std::cout << pt.filename()
                                  << " " << r->size()
                                  << std::endl;
                    }
                }
            }            
        }
    catch(GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
}

void GmailCommands::down_att_async(QString msgId)
{
    if(msgId.isEmpty())
        {
            std::cout << "Missing parameters, expected <message_id>" << std::endl;
            return;
        }

    QString dest_dir = QString("download/%1").arg(msgId);
    QDir att_dir;    
    if(!att_dir.mkpath(dest_dir)){
        std::cout << "Failed to create directory: " << dest_dir << std::endl;
        return;
    };

    std::function<void(attachments::MessageAttachment* , QString )> store_attachment = [=](attachments::MessageAttachment* a, QString filename)
        {
            QFile file_in(dest_dir + "/" + filename);
            if (file_in.open(QFile::WriteOnly|QIODevice::Truncate)) {
                file_in.write(QByteArray::fromBase64(a->data(), QByteArray::Base64UrlEncoding));
                file_in.close();
                std::cout << filename
                          << " " << a->size()
                          << std::endl;
            }            
        };

    auto t = m_gm->getMessages()->get_Async(gmail::IdArg(m_c.userId(), msgId));
    t->then([=](std::unique_ptr<messages::MessageResource> r) 
            {
                auto p = r->payload();
                auto parts = p.parts();
                m_batch_counter = parts.size();
                for (auto pt : parts) {
                    auto b = pt.body();
                    if (b.size() > 0 && !b.attachmentid().isEmpty()) {
                        gmail::AttachmentIdArg arg(m_c.userId(), msgId, b.attachmentid());
                        auto a = m_gm->getAttachments()->get_Async(arg);
                        a->then([=](std::unique_ptr<attachments::MessageAttachment> att)
                                {
                                    store_attachment(att.get(), pt.filename());
                                    m_batch_counter--;
                                    if (m_batch_counter == 0) {
                                        m_c.exitEventsLoop();
                                    }
                                });
                    }
                    else {
                        m_batch_counter--;
                    }
                }//for parts
            });

    m_c.runEventsLoop();
};

void GmailCommands::export_last_result(QString)
{
    QString dest_dir = QString("download");
    QDir att_dir;    
    if(!att_dir.mkpath(dest_dir)){
        std::cout << "Failed to create directory: " << dest_dir << std::endl;
        return;
    };

    QString fileName = dest_dir + "/result_export.txt";    
    m_c.exportLastResponse(fileName);
    std::cout << "saved:" << fileName << std::endl;
};

void GmailCommands::print_last_result(QString )
{
    m_c.printLastResponse();
};

void GmailCommands::print_user_id(QString )
{
    std::cout << m_c.userId();
};


void GmailCommands::get_batch_snippets(QString id_list) 
{
    STRING_LIST arg_list = split_string(id_list);
    if (arg_list.empty()) 
        {
            std::cout << "Space separated messages ID list required" << std::endl;
            return;
        }
    RESULT_LIST<messages::MessageResource> res = m_gm->cacheRoutes()->getUserBatchMessages(EDataState::snippet, arg_list);
    std::sort(res.begin(), res.end(), [](std::unique_ptr<messages::MessageResource>& f, std::unique_ptr<messages::MessageResource>& s) {return (f->internaldate() > s->internaldate()); });
    //res.sort([](std::unique_ptr<messages::MessageResource>& f, std::unique_ptr<messages::MessageResource>& s) {return (f->internaldate() > s->internaldate()); });
    std::cout << "batch size: " << res.size() << std::endl;
    
    int n = 1;
    for (auto& m : res)
        {
            auto& payload = m->payload();
            auto& header_list = payload.headers();

            std::cout << n << ". " << m->id() << "|" << m->internaldate() << "|";
            QDateTime dt = QDateTime::fromMSecsSinceEpoch(m->internaldate());
            std::cout << dt.toString(Qt::ISODate).toStdString() << "|";
            for (auto h : header_list)
                {
                    std::cout << h.value() << "|";
                }
            std::cout << std::endl;
            n++;
        }
};

void GmailCommands::get_batch_details(QString id_list) 
{
    STRING_LIST arg_list = split_string(id_list);
    if (arg_list.empty())
        {
            std::cout << "Space separated messages ID list required" << std::endl;
            return;
        }

    RESULT_LIST<messages::MessageResource> res = m_gm->cacheRoutes()->getUserBatchMessages(EDataState::body, arg_list);
    int n = 1;
    for (auto& m : res)
        {
            auto& payload = m->payload();
            auto& header_list = payload.headers();

            std::cout << n << ". " << m->id() << "|";
            for (auto h : header_list)
                {
                    std::cout << h.value() << "|";
                }
            std::cout << std::endl;
            auto& parts = payload.parts();
            for (auto pt : parts)
                {
                    bool is_plain_text = false;
                    bool is_html_text = false;
                    auto& pt_headers = pt.headers();
                    for (auto h : pt_headers)
                        {
                            if (h.name() == "Content-Type") {
                                is_plain_text = (h.value().indexOf("text/plain") == 0);
                                is_html_text = (h.value().indexOf("text/html") == 0);
                            }
                            std::cout << "" << h.name().leftJustified(20, ' ') << " " << h.value() << std::endl;
                        }
                    if (is_plain_text || is_html_text)
                        {
                            const messages::MessagePartBody& pt_body = pt.body();
                            std::cout << QByteArray::fromBase64(pt_body.data(), QByteArray::Base64UrlEncoding).constData() << std::endl;
                        }
                }
            std::cout << "----------------------------------------------------------" << std::endl;
            n++;
        }

};

void GmailCommands::get_cache_snippets(QString id_list)
{    
    STRING_LIST arg_list = split_string(id_list);
    if (arg_list.empty())
        {
            std::cout << "Space separated messages ID list required" << std::endl;
            return;
        }

    auto lst = m_gm->cacheRoutes()->getCacheMessages(EDataState::snippet, arg_list);
    std::cout << "loaded from cache: " << lst->result_list.size() << std::endl;

    int n = 1;
    for (auto& i : lst->result_list)
        {
            mail_cache::MessageData* m = i.get();
            std::cout << n << ". " << m->id() << "|";
            std::cout << m->from() << "|";
            std::cout << m->subject() << "|";
            std::cout << m->snippet() << "|" << std::endl;
            n++;
        }
};

void GmailCommands::get_cache_details(QString id_list) 
{    
    STRING_LIST arg_list = split_string(id_list);
    if (arg_list.empty())
        {
            std::cout << "Space separated messages ID list required" << std::endl;
            return;
        }

    auto lst = m_gm->cacheRoutes()->getCacheMessages(EDataState::body, arg_list);
    std::cout << "loaded from cache: " << lst->result_list.size() << std::endl;

    int n = 1;
    for (auto& i : lst->result_list)
        {
            mail_cache::MessageData* m = i.get();
            std::cout << n << ". " << m->id() << "|";
            std::cout << m->from() << "|";
            std::cout << m->subject() << "|";
            std::cout << m->snippet() << "|" << std::endl;
            std::cout << "------------- plain text --------------" << std::endl;
            std::cout << m->plain() << std::endl;
            std::cout << "------------- html text --------------" << std::endl;
            std::cout << m->html() << std::endl;
            n++;
        }
};

void GmailCommands::check_email_cache(QString nextToken) 
{
    try
        {    
            auto lst = m_gm->cacheRoutes()->getNextCacheMessages(20, nextToken);
            std::cout << "loaded from cache: " << lst->result_list.size() << std::endl;

            int n = 1;
            for (auto& i : lst->result_list)
                {
                    mail_cache::MessageData* m = i.get();
                    std::cout << n << ". " << m->id() << "|";
                    std::cout << m->from() << "|";
                    std::cout << m->subject() << "|";
                    std::cout << m->snippet() << "|" << std::endl;
                    n++;
                }
        }
    catch(GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }    
};


void GmailCommands::get_cache_threads(QString) 
{
    try
    {
        auto lst = m_gm->cacheRoutes()->getNextCacheThreads(20);
        std::cout << "loaded thread from cache: " << lst->result_list.size() << std::endl;

        int n = 1;
        for (auto& i : lst->result_list)
        {
            mail_cache::ThreadData* t = i.get();
            std::cout << n << ". " << t->id() << "|";
            std::cout << t->historyId() << "|";
            std::cout << t->messagesCount() << "|";
            std::cout << t->snippet() << "|" << std::endl;
            n++;
        }
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};
