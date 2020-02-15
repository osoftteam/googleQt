#include <iostream>
#include <functional>

#include <QtCore/QCoreApplication>
#include <QFile>
#include <QStringList>

#include "google/endpoint/ApiAppInfo.h"
#include "google/endpoint/ApiAuthInfo.h"
#include "google/demo/ApiTerminal.h"
#include "google/demo/ApiListener.h"
#include "GdriveCommands.h"

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
        std::cout << "Error reading <auth-file> - token file" << std::endl;
        std::cout << "Use 'authorize' sample to generate token file." << std::endl;
        std::cin.ignore();
        return 0;        
    }

    demo::ApiListener lsn;
	auto c = googleQt::createClient(appInfo.release(), authInfo.release());
    QObject::connect(c.get(), &GoogleClient::downloadProgress, &lsn, &demo::ApiListener::transferProgress);

    GdriveCommands cmd(*c);
    demo::Terminal t("gdrive");
    t.addAction("about",            "About (Information about account - limits etc.)",          [&](QString arg) {cmd.about(arg); });
    t.addAction("ls",               "List",           [&](QString arg) {cmd.ls(arg); });
    t.addAction("ls_folders",       "Show folders",   [&](QString arg) {cmd.ls_folders(arg); });
    t.addAction("ls_dir_content",   "Show directory content",   [&](QString arg) {cmd.ls_dir_content(arg); });
    t.addAction("clean_dir_content","Delete directory content (with confirmation)",   [&](QString arg) {cmd.clean_dir_content(arg); });    
    t.addAction("get",              "Get File Info",  [&](QString arg) {cmd.get(arg); });
    t.addAction("find_by_name",     "Find by File Name",  [&](QString arg) {cmd.find_by_name(arg); });
    t.addAction("mkdir",            "Create New Folder",[&](QString arg) {cmd.mkdir(arg); });
    t.addAction("rename",           "Rename File",  [&](QString arg) {cmd.rename(arg); });
    t.addAction("move",             "Move File (Add/Remove parents)",  [&](QString arg) {cmd.move_file(arg); });
    t.addAction("download",         "Download file",  [&](QString arg) {cmd.download(arg); });
    t.addAction("cat",              "Print file content on screen", [&](QString arg) {cmd.cat(arg); });
    t.addAction("hash",             "Calc file content hash", [&](QString arg) {cmd.hash_file(arg); });    
    t.addAction("create",           "Create a new file using 'multipart' method", [&](QString arg) {cmd.create(arg); });
    t.addAction("create_using_id",  "Create a new file using 'multipart' method, using ID. see generate_ids", [&](QString arg) {cmd.create_using_id(arg); });
    t.addAction("upload_simple",    "Upload file using 'simple' method", [&](QString arg) {cmd.upload_simple(arg); });
    t.addAction("upgrade_file",    "Replace remote file with local using 'upgrade' utility", [&](QString arg) {cmd.upgrade_file(arg); });    
    t.addAction("rm",               "Delete file or folder", [&](QString arg) {cmd.rm(arg); });
    t.addAction("generate_ids", "generate(reserve) some ids for future create call.", [&](QString arg) {cmd.generate_ids(arg); });
    t.addSeparator();
    t.addAction("create_in_appdata","Create file in appDataFolder", [&](QString arg) {cmd.create_in_appdata(arg); });
    t.addAction("ls_space_content", "Show space content (appDataFolder, drive, photos)",   [&](QString arg) {cmd.ls_space_content(arg); });    
    t.addAction("rm_appdata_files","Remove files in appDataFolder", [&](QString arg) {cmd.rm_appdata_files(arg); });
    t.addAction("clean_space_content", "Delete space content with confirmation (appDataFolder, drive, photos)",   [&](QString arg) {cmd.clean_space_content(arg); });    
    t.addSeparator();
    t.addAction("ls_comments",      "List comments",  [&](QString arg) {cmd.ls_comments(arg); });
    t.addAction("get_comment",      "Get comment",    [&](QString arg) {cmd.get_comment(arg); });
    t.addAction("new_comment",      "Create comment", [&](QString arg) {cmd.new_comment(arg); });
    t.addAction("rm_comment",       "Remove comment", [&](QString arg) {cmd.rm_comment(arg); });
    t.addSeparator();
    t.addAction("ls_permissions",   "List permissions",  [&](QString arg) {cmd.ls_permissions(arg); });
    t.addAction("get_permission",   "Get permission",    [&](QString arg) {cmd.get_permission(arg); });
    t.addAction("create_permissions", "Create permission", [&](QString arg) {cmd.create_permissions(arg); });
    t.addAction("delete_permissions", "Delete permission", [&](QString arg) {cmd.delete_permissions(arg); });

    t.addSeparator();
    t.addAction("ls_revisions", "List revisions", [&](QString arg) {cmd.ls_revisions(arg); });
    t.addAction("get_revision", "Get revision", [&](QString arg) {cmd.get_revision(arg); });
    t.addSeparator();
    t.addAction("print_last_result", "Print last response",    [&](QString arg) {cmd.print_last_result(arg); });
    t.addAction("set_local_proxy",  "Setup Local Proxy connection",    [&](QString arg) {cmd.set_local_proxy(arg); });
    t.start();
	googleQt::releaseClient(c);
    return 0;
}
