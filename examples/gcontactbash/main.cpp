#include <iostream>
#include <functional>

#include <QtCore/QCoreApplication>
#include <QFile>
#include <QStringList>

#include "google/endpoint/ApiAppInfo.h"
#include "google/endpoint/ApiAuthInfo.h"
#include "google/demo/ApiTerminal.h"
#include "google/demo/ApiListener.h"
#include "GcontactCommands.h"
#include "gmail/GmailRoutes.h"
#include "google/AUTOTEST/GoogleAutotest.h"

using namespace googleQt;

int main(int argc, char *argv[]) 
{
    QCoreApplication app(argc, argv);

    if (argc != 4) {
        std::string  s = QString("\nUsage: %1 <app-file> <auth-file>\n"
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
    
    std::cout << std::endl << std::endl;
    std::cout << "GContacts" << std::endl;
    
    demo::ApiListener lsn;
	auto c = googleQt::createClient(appInfo.release(), authInfo.release());
    DECLARE_AUTOTEST_INSTANCE(c.get(), "autotest-res.txt");

    /// setup DB-cache ///
    const QString dbPath = "contacts-cache/gm-cache.sqlite";
    auto mr = c->gmail();
    if (!mr->setupCache(dbPath, "downloads", "contacts-cache")) {
        std::cout << "Failed to initialize SQLite cache database: " << dbPath.toStdString() << std::endl;
        return 0;
    };

    GcontactCommands cmd(*c);
    demo::Terminal t("gcontact");
    t.addAction("ls_contacts",      "List Contacts", [&](QString ) {cmd.ls_contacts(); });
    t.addAction("get_contact",      "get single contact entry", [&](QString arg) {cmd.get_contact(arg); });
    t.addAction("create_contact",   "create new contact", [&](QString arg) {cmd.create_contact(arg); });
    t.addAction("delete_contact",   "delete contact entry", [&](QString arg) {cmd.delete_contact(arg); });
    t.addAction("update_contact_name", "change name of a contact entry", [&](QString arg) {cmd.update_contact_name(arg); });    
    t.addAction("ls_contacts_date", "List contacts modified after date/time", [&](QString arg) {cmd.ls_contacts_date(arg); });
    t.addAction("update_contact_user_field", "Update/add user field", [&](QString arg) {cmd.update_contact_user_field(arg); });
    t.addSeparator();
    t.addAction("ls_groups", "List Contact Groups", [&](QString) {cmd.ls_groups(); });
    t.addAction("get_group", "get single group entry", [&](QString arg) {cmd.get_group(arg); });
    t.addAction("create_group", "create new contact group", [&](QString arg) {cmd.create_group(arg); });
    t.addAction("delete_group", "delete group entry", [&](QString arg) {cmd.delete_group(arg); });
    t.addAction("update_group_title", "change title of a contacts group", [&](QString arg) {cmd.update_group_title(arg); });
    t.addAction("ls_groups_date", "List groups modified after date/time", [&](QString arg) {cmd.ls_groups_date(arg); });
    t.addAction("ls_group_contacts", "list group contacts", [&](QString arg) {cmd.ls_group_contacts(arg); });
    t.addAction("set_contact_groups", "set contact groups", [&](QString arg) {cmd.set_contact_groups(arg); });
    t.addSeparator();
    t.addAction("download_photo", "download contact photo", [&](QString arg) {cmd.download_photo(arg); });
    t.addAction("upload_photo", "upload contact photo", [&](QString arg) {cmd.upload_photo(arg); });
    t.addAction("delete_photo", "delete contact photo", [&](QString arg) {cmd.delete_photo(arg); });
    t.addSeparator();

    t.addAction("batch_list_contacts", "get contact entries in batch", [&](QString arg) {cmd.batch_list_contacts(arg); });
    t.addAction("batch_create_contact", "create contact entries in batch", [&](QString arg) {cmd.batch_create_contact(arg); });
    t.addAction("batch_update_contact", "update contact entries in batch", [&](QString arg) {cmd.batch_update_contact(arg); });
    t.addAction("batch_delete_contact", "delete contact entries in batch", [&](QString arg) {cmd.batch_delete_contact(arg); });
    t.addSeparator();
    t.addAction("batch_list_groups", "get contact groups in batch", [&](QString arg) {cmd.batch_list_groups(arg); });
    t.addAction("batch_create_group", "create contact groups in batch", [&](QString arg) {cmd.batch_create_group(arg); });
    t.addAction("batch_update_group", "update contact groups in batch", [&](QString arg) {cmd.batch_update_group(arg); });
    t.addAction("batch_delete_group", "delete groups in batch", [&](QString arg) {cmd.batch_delete_group(arg); });

    t.addSeparator();
    t.addAction("cache_sync", "sync contacts cache", [&](QString) {cmd.cache_sync(); });
    t.addAction("cache_ls", "print contacts cache", [&](QString) {cmd.cache_ls(); });
    t.addAction("cache_get_contact", "find contacts cache entry", [&](QString arg) {cmd.cache_get_contact(arg); });
    t.addAction("cache_update", "modify contact names locally", [&](QString arg) {cmd.cache_update(arg); });
    t.addAction("cache_delete", "delete contact locally", [&](QString arg) {cmd.cache_delete(arg); });
    t.addAction("resolve_cache_photo", "resolve cache photo", [&](QString arg) {cmd.resolve_cache_photo(arg); });    
    t.addAction("sync_cache_photos", "sync photos with cache", [&](QString) {cmd.sync_cache_photos(); });
    t.addAction("add_cache_photo", "add(replace) contact photo", [&](QString arg) {cmd.add_cache_photo(arg); });
    t.addSeparator();
    
    t.addAction("ls_as_json",       "List Contacts as json", [&](QString ) {cmd.ls_as_json(); });
    t.addAction("export_last_result",     "Export last response to a file", [&](QString ) {cmd.export_last_result(); });
    t.addAction("print_last_result",     "Print last response", [&](QString ) {cmd.print_last_result(); });
    t.addAction("parse_file", "Read and parse xml contacts file", [&](QString arg) {cmd.parse_file(arg); });
    t.addAction("parse_group_file", "Read and parse xml contacts group file", [&](QString arg) {cmd.parse_group_file(arg); });
    t.addAction("test_contact_xml", "Test xml-serialization of a contact entry", [&](QString) {cmd.test_contact_xml(); });
    t.addAction("test_merge", "Read xml contacts file, merge with custom data, print result", [&](QString arg) {cmd.test_merge(arg); });
    t.addAction("test_concurrent_req", "Test concurrent (time-sharing) execution more that 1 task", [&](QString) {cmd.test_concurrent_req(); });
    t.start();
	googleQt::releaseClient(c);
    return 0;
};
