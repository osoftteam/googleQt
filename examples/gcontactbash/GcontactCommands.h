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
    * list contacts
    */
    void ls_contacts();

    /**
    * get single contact entry
    */
    void get_contact(QString contactid);

    /**
    * create single contact entry
    * requires email adddress, first name, last name - space separated
    */
    void create_contact(QString email_first_last);

    /**
    * delete contact entry
    * requires contactid
    */
    void delete_contact(QString contactid);

    /**
        modify contact name
    */
    void update_contact_name(QString contactId_name);

    /**
    * list contacts modified after date
    */
    void ls_contacts_date(QString updatedMin);

    /**
        add/modify contact custom field
    */
    void update_contact_user_field(QString contactId_key_value);

    /**
    * get contact photo
    */
    void download_photo(QString contactid);

    /**
    * add/update contact photo
    */
    void upload_photo(QString contactid_space_file_name);

    /**
    * delete contact photo
    */
    void delete_photo(QString contactid);

    /**
     * update local cache photo file
     */
    void resolve_cache_photo(QString contactid);

    /**
    * list contact group
    */
    void ls_groups();

    /**
    * get single group entry
    */
    void get_group(QString groupid);

    /**
    * create contact group
    * requires title, content
    */
    void create_group(QString title_content);

    /**
    * delete contact group
    * requires groupId
    */
    void delete_group(QString groupId);

    /**
    modify group title
    */
    void update_group_title(QString groupId_title);

    /**
    * list group modified after date
    */
    void ls_groups_date(QString updatedMin);

    /**
    * list contacts in a group
    * requires groupId
    */
    void ls_group_contacts(QString groupId);

    /**
    * set contact groups
    */
    void set_contact_groups(QString contactId_space_groupId);

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
    * delete contact in batch by id
    */
    void batch_delete_contact(QString id_space_id);

    /**
    * list groups in batch mode
    */
    void batch_list_groups(QString id_space_id);

    /**
    * create contact groups in singe batch
    * space separated list of names will be used as titles
    */
    void batch_create_group(QString name_space_name);


    /**
    *   modify group title in batch - append '-b'
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
    void cache_get_contact(QString cid);

    
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
     * synchronize contacts cache
     */
    void cache_sync();
    
    /**
    * synchronize photos
    */
    void sync_cache_photos();

    /**
    * add photos
    */
    void add_cache_photo(QString id_space_file_name);


    /**
    * list contacts using json
    */
    void ls_as_json();

    /**
    * create contact obj, export to XML and import back
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
