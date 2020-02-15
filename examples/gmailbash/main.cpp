#include <iostream>
#include <functional>

#include <QtCore/QCoreApplication>
#include <QFile>
#include <QStringList>

#include "google/endpoint/ApiAppInfo.h"
#include "google/endpoint/ApiAuthInfo.h"
#include "google/demo/ApiTerminal.h"
#include "google/demo/ApiListener.h"
#include "GmailCommands.h"
#include "gmail/GmailRoutes.h"
#include "google/AUTOTEST/GoogleAutotest.h"


using namespace googleQt;


int main(int argc, char *argv[]) 
{
    QCoreApplication app(argc, argv);

    if (argc != 4) {
        std::string  s = QString("\nUsage: %1 <app-file> <auth-file> <email>\n"
                                 "Example: %1 ../app.info ../token.info me@gmail.com\n"
                                 "\n"
                                 "<app-info-file>: a JSON file with information about your API app.\n"
                                 "<auth-file>: An \"auth file\" that contains the information necessary to make\n"
                                 "an authorized Google API request.  Generate this file using the \"authorize\"\n"
                                 "example program.\n"
                                 "\n"
                                 " Press ENTER to proceed.").arg(argv[0]).toStdString();

        std::cout << s << std::endl;
        std::cout << std::endl;
        std::cin.ignore();
        return 0;
    }

    QString argAppFile = argv[1];
    QString argAuthFile = argv[2];
    QString argEmail = argv[3];

    std::unique_ptr<ApiAppInfo> appInfo(new ApiAppInfo);
    if(!appInfo->readFromFile(argAppFile)){
        std::cerr << "Error reading <app-info-file>" << std::endl;
        return 0;
    };    
    
    std::unique_ptr<ApiAuthInfo> authInfo(new ApiAuthInfo(argAuthFile));
    if(!authInfo->reload()){
        std::cout << "Error reading <auth-file>" << std::endl;
        std::cout << "Use 'authorize' sample to generate token file." << std::endl;
        std::cin.ignore();
        return 0;        
    }

    authInfo->setEmail(argEmail);

    demo::ApiListener lsn;
	auto c = googleQt::createClient(appInfo.release(), authInfo.release());
    QObject::connect(c.get(), &GoogleClient::downloadProgress, &lsn, &demo::ApiListener::transferProgress);   
    DECLARE_AUTOTEST_INSTANCE(c.get(), "autotest-res.txt");

    /// setup DB-cache ///
    const QString dbPath = "gm-cache.sqlite";
    auto mr = c->gmail();
    if (!mr->setupCache(dbPath, "downloads", "contacts-cache")) {
        std::cout << "Failed to initialize SQLite cache database: " << dbPath.toStdString() << std::endl;
        return 0;
    };
    
    GmailCommands cmd(*c);
    demo::Terminal t("gmail");
    t.addAction("ls",               "List Messages", [&](QString arg) {cmd.ls(arg);} );
    t.addAction("ls_by_labels",     "List Messages by label ID", [&](QString arg) {cmd.ls_by_labels(arg);} );
    t.addAction("search",     "Search for Messages", [&](QString arg) {cmd.search(arg);} );
    t.addAction("get",              "Get Message by ID", [&](QString arg) {cmd.get(arg);} );
    t.addAction("get_snippet",      "Get Message snippet by ID", [&](QString arg) {cmd.get_snippet(arg);} );    
    t.addAction("get_raw",          "Get Message by ID using raw format", [&](QString arg) {cmd.get(arg);} );
    t.addAction("save_raw",         "Save Message by ID using raw format", [&](QString arg) {cmd.save_raw(arg);} );    
    t.addAction("get_html",         "Export body of Message to html file", [&](QString arg) {cmd.get_html(arg);} );
    t.addAction("trash",            "Trash Message by ID", [&](QString arg) {cmd.trash(arg); });
    t.addAction("untrash",          "Untrash Message by ID", [&](QString arg) {cmd.untrash(arg); });
    t.addAction("delete",           "Delete Message by ID", [&](QString arg) {cmd.delete_msg(arg); });
    t.addAction("send_prepared_rfc822",  "Send Message stored in a file", [&](QString arg) {cmd.send_prepared_rfc822(arg); });
    t.addAction("send_as_html",     "Send Html Message", [&](QString arg) {cmd.send_as_html(arg); });
    t.addAction("send_att",         "Send Email with attachments", [&](QString arg) {cmd.send_att(arg); });
    t.addAction("add_label",        "Add label", [&](QString arg) {cmd.add_label(arg); });
    t.addAction("remove_label",     "Remove label", [&](QString arg) {cmd.remove_label(arg); });
    t.addSeparator();
    t.addAction("ls_labels",        "List Labels", [&](QString arg) {cmd.ls_labels(arg); });
    t.addAction("get_label",        "Get Label Info", [&](QString arg) {cmd.get_label(arg); });
    t.addAction("create_label",     "Create Label", [&](QString arg) {cmd.create_label(arg); });
    t.addAction("delete_label",     "Delete Label", [&](QString arg) {cmd.delete_label(arg); });
    t.addSeparator();
    t.addAction("ls_threads",       "List Threads", [&](QString arg) {cmd.ls_threads(arg); });
    t.addAction("get_thread",       "Get Thread Info", [&](QString arg) {cmd.get_thread(arg); });
    t.addAction("ls_drafts",        "List Drafts", [&](QString arg) {cmd.ls_drafts(arg); });
    t.addAction("ls_threads_by_labels", "List Threads by label", [&](QString arg) {cmd.ls_threads_by_labels(arg); });
    t.addAction("q_threads", "Search Threads", [&](QString arg) {cmd.q_threads(arg); });
    t.addAction("add_thread_label", "Add Thread Label", [&](QString arg) {cmd.add_thread_label(arg); });
    t.addAction("remove_thread_label", "Remove Thread Label", [&](QString arg) {cmd.remove_thread_label(arg); });
    t.addAction("get_draft",        "Get Draft Info", [&](QString arg) {cmd.get_draft(arg); });
    t.addAction("history",          "Get Mailbox History", [&](QString arg) {cmd.history(arg); });
    t.addSeparator();
    t.addAction("get_batch_snippets", "Get Messages snippets in batch by ID list", [&](QString arg) {cmd.get_batch_snippets(arg); });
    t.addAction("get_batch_details", "Get Messages email body in batch by ID list", [&](QString arg) {cmd.get_batch_details(arg); });
    t.addSeparator();
    t.addAction("get_cache_snippets", "Get Messages snippets by ID list from cache if available, if not query cloud", [&](QString arg) {cmd.get_cache_snippets(arg); });
    t.addAction("get_cache_details", "Get Messages email body by ID list from cache if available, if not query cloud", [&](QString arg) {cmd.get_cache_details(arg); });    
    t.addAction("check_email_cache", "Check for new email on cloud, if found return and update cache, otherwise return cache messages", [&](QString arg) {cmd.check_email_cache(arg); });
    t.addSeparator();
    t.addAction("download_attachments", "Download all attachments of message", [&](QString arg) {cmd.download_attachments(arg); });
    t.addAction("down_att_async", "Download all attachments of message (async way)", [&](QString arg) {cmd.down_att_async(arg); });
    t.addSeparator();
    t.addAction("get_cache_threads", "Check for new threads & emails on cloud, if found return and update cache, otherwise return cache messages", [&](QString arg) {cmd.get_cache_threads(arg); });
    t.addSeparator();
    t.addAction("export_last_result",   "Export last response to a file", [&](QString arg) {cmd.export_last_result(arg); });
    t.addAction("print_last_result",   "Print last response", [&](QString arg) {cmd.print_last_result(arg); });
    t.addAction("print_user_id",   "Print user ID", [&](QString arg) {cmd.print_user_id(arg); });
    t.addAction("base64url_encode",   "Base64Url encode file", [&](QString arg) {cmd.base64url_encode(arg); });
    t.addAction("base64url_decode",   "Base64Url decode data", [&](QString arg) {cmd.base64url_decode(arg); });
    
    t.start();
	googleQt::releaseClient(c);
    std::cout << "press ENTER to exit" << std::endl;
    std::cin.ignore();
    return 0;
}
