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

using namespace googleQt;


int main(int argc, char *argv[]) 
{
    QCoreApplication app(argc, argv);

    if (argc != 4) {
        std::string  s = QString("\nUsage: %1 <app-file> <auth-file> <user-id>\n"
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
    QString argUserId = argv[3];

    std::unique_ptr<ApiAppInfo> appInfo(new ApiAppInfo);
    if(!appInfo->readFromFile(argAppFile)){
        std::cerr << "Error reading <app-info-file>" << std::endl;
        return 0;
    };    
    
    std::unique_ptr<ApiAuthInfo> authInfo(new ApiAuthInfo(argAuthFile));
    if(!authInfo->reload()){
        std::cout << "Error reading <auth-file>" << std::endl;
        std::cin.ignore();
        return 0;        
    }

    demo::ApiListener lsn;
    GoogleClient c(appInfo.release(), authInfo.release(), argUserId);
    QObject::connect(&c, &GoogleClient::downloadProgress, &lsn, &demo::ApiListener::transferProgress);

    GmailCommands cmd(c);
    demo::Terminal t("gmail");
    t.addAction("ls",               "List Mesages", [&](QString arg) {cmd.ls(arg);} );
    t.addAction("ls_by_labels",     "List Mesages by label ID", [&](QString arg) {cmd.ls_by_labels(arg);} );
    t.addAction("get",              "Get Mesage by ID", [&](QString arg) {cmd.get(arg);} );
    t.addAction("get_raw",          "Get Mesage by ID using raw format", [&](QString arg) {cmd.get(arg);} );
    t.addAction("get_html",         "Export body of Mesage to html file", [&](QString arg) {cmd.get_html(arg);} );
    t.addAction("trash",            "Trash Mesage by ID", [&](QString arg) {cmd.trash(arg); });
    t.addAction("untrash",          "Untrash Mesage by ID", [&](QString arg) {cmd.untrash(arg); });
    t.addAction("send",             "Send Mesage", [&](QString arg) {cmd.send(arg); });
    t.addAction("import",           "Import Mesage", [&](QString arg) {cmd.importMessage(arg); });
    t.addAction("insert",           "Insert Mesage", [&](QString arg) {cmd.insertMessage(arg); });
    t.addAction("           ",      "            ", [&](QString ) {});
    t.addAction("ls_labels",        "List Labels", [&](QString arg) {cmd.ls_labels(arg); });
    t.addAction("get_label",        "Get Label Info", [&](QString arg) {cmd.get_label(arg); });
    t.addAction("create_label",     "Create Label", [&](QString arg) {cmd.create_label(arg); });
    t.addAction("delete_label",     "Delete Label", [&](QString arg) {cmd.delete_label(arg); });
    t.addAction("           ",      "            ", [&](QString ) {});
    t.addAction("ls_threads",       "List Threads", [&](QString arg) {cmd.ls_threads(arg); });
    t.addAction("get_thread",       "Get Thread Info", [&](QString arg) {cmd.get_thread(arg); });
    t.addAction("ls_drafts",        "List Drafts", [&](QString arg) {cmd.ls_drafts(arg); });
    t.addAction("get_draft",        "Get Draft Info", [&](QString arg) {cmd.get_draft(arg); });
    t.addAction("history",          "Get Mailbox History", [&](QString arg) {cmd.history(arg); });
    t.addAction("         ",        "            ", [&](QString ) {});
    t.addAction("export_last_result",   "Export last response to a file", [&](QString arg) {cmd.export_last_result(arg); });
    t.addAction("print_last_result",   "Print last response", [&](QString arg) {cmd.print_last_result(arg); });
    
    t.start();
    return 0;
}
