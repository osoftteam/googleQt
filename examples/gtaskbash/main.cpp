#include <iostream>
#include <functional>

#include <QtCore/QCoreApplication>
#include <QFile>
#include <QStringList>

#include "google/endpoint/ApiAppInfo.h"
#include "google/endpoint/ApiAuthInfo.h"
#include "google/demo/ApiTerminal.h"
#include "google/demo/ApiListener.h"
#include "GtaskCommands.h"

using namespace googleQt;


int main(int argc, char *argv[]) 
{
    QCoreApplication app(argc, argv);

    if (argc != 3) {
        std::string  s = QString("\nUsage: %1 <app-file> <auth-file>\n"
                                 "Example: %1 ../app.info ../token.info\n"
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

    std::cout << std::endl << std::endl;
    std::cout << "Task is member of a TaskList." << std::endl;
    std::cout << "First you get TaskList and then collection of Tasks in particular TaskList." << std::endl;
    std::cout << "Your need TaskListId to get information about TaskList" << std::endl;
    std::cout << "Your need TaskListId & TaskId to get information about Task." << std::endl;
    
    demo::ApiListener lsn;
    //GoogleClient c(appInfo.release(), authInfo.release());
	auto c = googleQt::createClient(appInfo.release(), authInfo.release());
    QObject::connect(c.get(), &GoogleClient::downloadProgress, &lsn, &demo::ApiListener::transferProgress);

    GtaskCommands cmd(*c);
    demo::Terminal t("gtask");
    t.addAction("ls_tlist",     "List TaskLists", [&](QString arg) {cmd.ls_tlist(arg); });
    t.addAction("get_tlist",    "Get TaskList by tasklistID", [&](QString arg) {cmd.get_tlist(arg); });
    t.addAction("insert_tlist", "Insert new tasklist", [&](QString arg) {cmd.insert_tlist(arg); });
    t.addAction("update_tlist", "Update a tasklist", [&](QString arg) {cmd.update_tlist(arg); });
    t.addAction("delete_tlist", "Delete a tasklist", [&](QString arg) {cmd.delete_tlist(arg); });
    t.addAction("------------", "-------------", [&](QString) {});
    t.addAction("ls",           "List Tasks", [&](QString arg) {cmd.ls(arg); });
    t.addAction("get",          "Get Task by tasklistID & taskID", [&](QString arg) {cmd.get(arg); });
    t.addAction("insert",       "Insert new task", [&](QString arg) {cmd.insert(arg); });
    t.addAction("update",       "Update a task", [&](QString arg) {cmd.update(arg); });
    t.addAction("update_note",  "Update a task note", [&](QString arg) {cmd.update_note(arg); });
    t.addAction("move",       "Move task", [&](QString arg) {cmd.move(arg); });
    t.addAction("delete",       "Delete a task", [&](QString arg) {cmd.delete_task(arg); });
    t.addSeparator();
    t.addAction("reload_cache", "Reload task list", [&](QString arg) {cmd.reload_cache(arg); });
    t.start();
    return 0;
}
