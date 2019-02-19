#pragma once

#include "GoogleClient.h"

using namespace googleQt;

namespace googleQt{
    namespace messages
    {
        class MessageResource;
    };
    namespace labels
    {
        class LabelResource;
    };    
    namespace mail_cache
    {
        class GmailCacheRoutes;
    }
    namespace threads 
    {
        class ThreadResource;
    }
}

class GmailCommands
{
public:
    GmailCommands(GoogleClient& c);

    /**
       ls - list messages
    */
    void ls     (QString nextToken);

    /**
       ls_by_labels - list messages that belong to labels provided
    */
    void ls_by_labels(QString labelIds);

    /**
       search - search for messages 
    */
    void search(QString q);

    
    /**
       get - get message by id using default (full) format
    */
    void get    (QString message_id);

    /**
       get_snippet - get snippet by id using default (full) format
    */
    void get_snippet    (QString message_id);
    
    /**
       get_raw - get message by id using raw format
    */
    void get_raw    (QString message_id);

    /**
       save_raw - save message by id using raw format
    */
    void save_raw    (QString message_id);
    
    /**
       export_html_body - export message body as html file
    */
    void get_html    (QString message_id_space_fileName);

    
    /**
       send plain text message
    */
    void send_prepared_rfc822(QString messageFileName);

    /**
       send html text message
    */
    void send_as_html(QString to_subject_text);

    /**
       send html text message with attachments
    */
    void send_att(QString to_subject_text);

    /**
       trash - trash message by id
    */
    void trash(QString message_id);
    
    /**
       untrash - untrash message by id
    */
    void untrash(QString message_id);

    /**
       delete_msg - delete message by id
    */
    void delete_msg(QString message_id);

    void add_label(QString message_id_label);
    void remove_label(QString message_id_label);

    /**
       ls_labels - list labels
    */
    void ls_labels(QString);

    /**
       get_label - get label
    */
    void get_label(QString label_id);

    /**
       create_label - create label
    */
    void create_label(QString name);

    /**
       delete_label - delete label
    */
    void delete_label(QString label_id);

    /**
       update_label - update label
    */
    void update_label(QString labelid_space_name);
    
    /**
       ls_threads - list threads
    */
    void ls_threads(QString);

    /**
       get_thread - get thread info
    */
    void get_thread(QString thread_id);

    /**
        add_thread_label - modify thread, add labels
    */
    void add_thread_label(QString threadid_labelids);

    /**
    remove_thread_label - modify thread, remove labels
    */
    void remove_thread_label(QString threadid_labelids);

    /**
       ls_drafts - list drafts
    */
    void ls_drafts(QString);

    /**
    ls_threads_by_labels - list threads that belong to labels provided
    */
    void ls_threads_by_labels(QString labelIds);

    /**
    q_threads - list threads using query parameter
    */
    void q_threads(QString qstr);


    /**
       get_draft - get draft info
    */
    void get_draft(QString draft_id);

    /**
       history - get history
    */
    void history(QString startHistoryIdStr);

    /**
       get_batch_snippets - get snippents for list of messages
    */
    void get_batch_snippets(QString id_list);

    /**
       get_batch_details - get email body & snippens for list of messages
    */
    void get_batch_details(QString id_list);
    
    /**
       get_cache_snippets - get snippents for list of messages
    */
    void get_cache_snippets(QString id_list);

    /**
       get_cache_details - get email body & snippens for list of messages
    */
    void get_cache_details(QString id_list);

    /**
       cache_check_email - list latest messages snippets, first
       query for new messages then lookup snippets and update local cache
    */
    void check_email_cache(QString nextToken);

    /**
       download_attachments - download all attachments to local directory
       The directory named download/<msg-id> will be created
    */
    void download_attachments(QString msgId);
    /**
       down_att_async - same download attachments async way
    */
    void down_att_async(QString msgId);
    
    void get_cache_threads(QString );

    void export_last_result(QString );
    void print_last_result(QString );
    void print_user_id(QString );

    void base64url_encode(QString fileName);
    void base64url_decode(QString data);
    
protected:
    void listMessages(QString nextToken, QString labelIds, QString query = "");
    void listThreads(QString nextToken, QString labelIds, QString qstr);
    void listDrafts(QString nextToken);
    void printSnippet(messages::MessageResource*);
    void printMessage(messages::MessageResource*);
    void printThread(threads::ThreadResource*);
    void exportMessageBody(messages::MessageResource* r, QString fileName);
    void printLabel(labels::LabelResource*);
    bool loadMessageFile(QString fileName, messages::MessageResource*);
    
protected:
    GoogleClient& m_c;
    GmailRoutes*  m_gm;
    int           m_batch_counter{0};
};

