#pragma once

#include "GoogleClient.h"

using namespace googleQt;

namespace googleQt {
    class GcontactRoutes;
    
    namespace gcontact {
        class ContactList;
        class GroupList;
        class ContactInfo;
        class GroupInfo;
        class BatchContactList;
        class BatchGroupList;
    }
};


class GcontactCommands
{
public:
    GcontactCommands(GoogleClient& c);
    
    /**
    * ls_contacts - list contacts
    */
    void ls_contacts();

    /**
    * get_contact - get single contact entry
    */
    void get_contact(QString contactid);

    /**
    * create_contact - create single contact entry
    * requires email adddress, first name, last name - space separated
    */
    void create_contact(QString email_first_last);

    /**
    * delete_contact - delete contact entry
    * requires contactid
    */
    void delete_contact(QString contactid);

    /**
        modify contact name
    */
    void update_contact_name(QString contactId_name);

    /**
    * ls_contacts_date - list contacts modified after date
    */
    void ls_contacts_date(QString updatedMin);


    /**
    * download_photo - get contact photo
    */
    void download_photo(QString contactid);

    /**
    * upload_photo - add/update contact photo
    */
    void upload_photo(QString contactid_space_file_name);

    /**
    * delete_photo - delete contact photo
    */
    void delete_photo(QString contactid);


    /**
    * ls_groups - list contact group
    */
    void ls_groups();

    /**
    * get_group - get single group entry
    */
    void get_group(QString groupid);

    /**
    * create_group - create contact group
    * requires title, content
    */
    void create_group(QString title_content);

    /**
    * delete_group - delete contact group
    * requires groupId
    */
    void delete_group(QString groupId);

    /**
    modify group title
    */
    void update_group_title(QString groupId_title);

    /**
    * ls_group_date - list group modified after date
    */
    void ls_groups_date(QString updatedMin);

    /**
        list contacts in batch mode
    */
    void batch_list_contacts(QString id_space_id);

    /**
    * batch_create_contact - create contact entries in singe batch
    * space separated list of names will be used as first names with last name appended suffix '4batch'
    */
    void batch_create_contact(QString name_space_name);

    /**
        modify contact names in batch - append '-b'
    */
    void batch_update_contact(QString id_space_id);

    /**
    delete contact in batch by id
    */
    void batch_delete_contact(QString id_space_id);

    /**
    list groups in batch mode
    */
    void batch_list_groups(QString id_space_id);

    /**
    * create contact groups in singe batch
    * space separated list of names will be used as titles
    */
    void batch_create_group(QString name_space_name);


    /**
    modify group title in batch - append '-b'
    */
    void batch_update_group(QString id_space_id);

    /**
    delete groups in batch by id
    */
    void batch_delete_group(QString id_space_id);

    /**
       print cache tables
     */
    void cache_ls();

    /**
       cache find contact by id
     */
    void cache_find_contact_by_id(QString cid);

    
    /**
        modify contact names locally - append '-c'
    */
    void cache_update(QString id_space_id);

    /**
        mark contact as 'deleted' in cache so in next sync
        it will be deleted on server as well
    */
    void cache_delete(QString id_space_id);
    
    
    /**
     * sync_contacts - synchronize contacts cache
     */
    void sync_contacts();
    
    /**
    * ls_as_json - list contacts using json
    */
    void ls_as_json();

    /**
    * test_contact_xml - create contact obj, export to XML and import back
    */
    void test_contact_xml();

    void test_concurrent_req();

    /**
    * test_merge_xml - import XML file, create contact with custom data and merge into the file
    */
    void test_merge(QString xmlFileName);

    ///parse xml contacts file
    void parse_file(QString xmlFileName);

    ///parse xml group contacts file
    void parse_group_file(QString xmlFileName);


    void export_last_result();
    void print_last_result();

    void print_contact_list(gcontact::ContactList* lst);
    void print_group_list(gcontact::GroupList* lst);
    void print_batch_contact_result(const gcontact::BatchContactList* lst);
    void print_batch_group_result(const gcontact::BatchGroupList* lst);
    void print_cache_contact_list(const gcontact::ContactList* lst);
    void print_cache_group_list(const gcontact::GroupList* lst);
    
    std::unique_ptr<gcontact::ContactInfo> generateContactInfo(QString first, QString last, QString email);    
    std::unique_ptr<gcontact::GroupInfo> generateGroupInfo(QString title, QString content);

protected:
    GoogleClient& m_c;
    GcontactRoutes*  m_gt;
};
