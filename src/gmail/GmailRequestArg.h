#pragma once

#include "google/endpoint/ApiUtil.h"

namespace googleQt{
    namespace gmail{
        DECLARE_PATH_CLASS(modify);
        DECLARE_PATH_CLASS(send);
        DECLARE_PATH_CLASS(trash);
        DECLARE_PATH_CLASS(untrash);
        DECLARE_PATH_CLASS(import);
        DECLARE_PATH_CLASS(insert);
        DECLARE_PATH_CLASS(batchDelete);

        class ListArg: public QParamArg
        {
        public:
            ListArg();
            ListArg(QString user_id);
            
            QString userId()const { return m_userId; }
            void    setUserId(QString id) { m_userId = id; };

            /**
             * Include messages from SPAM and TRASH in the results. (Default: false)
             */
            bool          getIncludeSpamTrash()const{return m_includeSpamTrash;};
            void          setIncludeSpamTrash(bool v){m_includeSpamTrash = v;}

            /**
             *  Only return messages with labels that match all of the specified label IDs.
             */
            QStringList&  labels(){return m_labelIds;}

            /**
             *  Maximum number of messages to return.
             */
            int           getMaxResults()const{return m_maxResults;};
            void          setMaxResults(int val){m_maxResults = val;}

            /**
             * Page token to retrieve a specific page of results in the list.
             */
            QString       getPageToken()const{return m_pageToken;}
            void          setPageToken(QString v){m_pageToken = v;}

            /**
             * Only return messages matching the specified query. Supports the same query format as the Gmail search box. 
             * For example, "from:someuser@example.com rfc822msgid: is:unread".
             */
            QString       getQ()const{return m_q;}
            void          setQ(QString v){m_q = v;}

            void build(const QString& link_path, QUrl& url)const override;
            
#ifdef API_QT_AUTOTEST
            static std::unique_ptr<ListArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST


        protected:
            QString     m_userId;
            bool        m_includeSpamTrash{false};
            QStringList m_labelIds;
            int         m_maxResults{50};
            QString     m_pageToken;
            QString     m_q;
        };
        
        class HistoryListArg : public QParamArg
        {
        public:
            HistoryListArg(QString userId, int startHistoryId = 0);

            QString userId()const { return m_userId; }
            void    setUserId(QString id) { m_userId = id; };

            /**
            *   Only return messages with labels that match all of the specified label IDs.
            */
            QStringList&  labels() { return m_labelIds; }

            /**
            *  The maximum number of history records to return. 
            */
            int           getMaxResults()const { return m_maxResults; };
            void          setMaxResults(int val) { m_maxResults = val; }

            /**
            * Page token to retrieve a specific page of results in the list.
            */
            QString       getPageToken()const { return m_pageToken; }
            void          setPageToken(QString v) { m_pageToken = v; }

            /**
            * Required. Returns history records after the specified startHistoryId. 
            * The supplied startHistoryId should be obtained from the historyId of a message, 
            * thread, or previous list response. History IDs increase chronologically but are not 
            * contiguous with random gaps in between valid IDs. Supplying an invalid or out of date 
            * startHistoryId typically returns an HTTP 404 error code. A historyId is typically valid 
            * for at least a week, but in some rare circumstances may be valid for only a few hours. 
            * If you receive an HTTP 404 error response, your application should perform a full sync. 
            * If you receive no nextPageToken in the response, there are no updates to retrieve and you 
            * can store the returned historyId for a future request. 
            */
            int           getStartHistoryId()const { return m_startHistoryId; }
            void          setStartHistoryId(int v) { m_startHistoryId = v; }

            void build(const QString& link_path, QUrl& url)const override;
#ifdef API_QT_AUTOTEST
            static std::unique_ptr<HistoryListArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            QString     m_userId;
            QStringList m_labelIds;
            int         m_maxResults;
            QString     m_pageToken;
            uint16_t    m_startHistoryId;
        };
        
        class DraftListArg : public QParamArg 
        {
        public:
            DraftListArg();
            DraftListArg(QString user_id);

            QString userId()const { return m_userId; }
            void    setUserId(QString id) { m_userId = id; };

            /**
            * Include messages from SPAM and TRASH in the results. (Default: false)
            */
            bool          getIncludeSpamTrash()const { return m_includeSpamTrash; };
            void          setIncludeSpamTrash(bool v) { m_includeSpamTrash = v; }

            /**
            *  The maximum number of history records to return.
            */
            int           getMaxResults()const { return m_maxResults; };
            void          setMaxResults(int val) { m_maxResults = val; }

            /**
            * Page token to retrieve a specific page of results in the list.
            */
            QString       getPageToken()const { return m_pageToken; }
            void          setPageToken(QString v) { m_pageToken = v; }

            /**
            * Only return messages matching the specified query. Supports the same query format as the Gmail search box.
            * For example, "from:someuser@example.com rfc822msgid: is:unread".
            */
            QString       getQ()const { return m_q; }
            void          setQ(QString v) { m_q = v; }

            void build(const QString& link_path, QUrl& url)const override;

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<DraftListArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            QString     m_userId;
            bool        m_includeSpamTrash;
            int         m_maxResults{50};
            QString     m_pageToken;
            QString     m_q;
        };


        class IdArg : public QParamArg
        {
        public:
            IdArg(){};
            IdArg(QString userIdValue, QString idValue, QString format = "full");

            void build(const QString& link_path, QUrl& url)const override;

            QString userId()const { return m_userId; }
            void    setUserId(QString id) { m_userId = id; };

            QString getId()const { return m_id; }
            void    setId(QString id) { m_id = id; };

            /**
            !!!Warning!!! this class can be used to retrieve email or thread data,
            see gmail reference and comments below

            For email messages routes format has following meaning
                   "full": Returns the full email message data with body content 
                   parsed in the payload field; the raw field is not used. (default)
                   "metadata": Returns only email message ID, labels, and email 
                   headers.
                   "minimal": Returns only email message ID and labels; does not
                   return the email headers, body, or payload.
                   "raw": Returns the full email message data with body content 
                   in the raw field as a base64url encoded string; the payload 
                   field is not used.

            For email thread routes format has following meaning
                "full": Returns the parsed email message content in the payload field 
                and the raw field is not used. (default)
                "metadata": Returns email headers with message metadata such as identifiers 
                and labels.
                "minimal": Only returns email message metadata such as identifiers and 
                labels, it does not return the email headers, body, or payload.

             */
            
            QString      getFormat()const{return m_format;}
            void         setFormat(QString f){m_format = f;}
            QStringList& headers(){return m_headers;}

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<IdArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            QString     m_userId;
            QString     m_id;
            QString     m_format;
            QStringList m_headers;
        };       

        class AttachmentIdArg : public QParamArg
        {
        public:
            AttachmentIdArg() {};
            AttachmentIdArg(QString user_id, QString message_id, QString attachment_id);

            void build(const QString& link_path, QUrl& url)const override;

            QString userId()const { return m_userId; }
            void    setUserId(QString id) { m_userId = id; };

            QString getMessageId()const { return m_message_id; }
            void    setMessageId(QString id) { m_message_id = id; };

            QString getAttachmentId()const { return m_attachment_id; }
            void    setAttachmentId(QString id) { m_attachment_id = id; };


#ifdef API_QT_AUTOTEST
            static std::unique_ptr<AttachmentIdArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            QString     m_userId;
            QString     m_message_id;
            QString     m_attachment_id;
        };


        class ModifyMessageArg: public QParamArgWithBody<ModifyMessageArg>
        {
        public:
            ModifyMessageArg() {};
            ModifyMessageArg(QString user_id,
                    QString message_id);

            QString userId()const { return m_userId; }
            void    setUserId(QString id) { m_userId = id; };

            QString messageId()const { return m_message_id; }
            void setMessageId(QString mid) { m_message_id = mid; }

            const std::list <QString>& getAddlabels()const { return m_addLabels; };
            void setAddlabels(const std::list <QString>& arg) { m_addLabels = arg;};
            const std::list <QString>& getRemovelabels()const { return m_removeLabels; };
            void setRemovelabels(const std::list <QString>& arg) { m_removeLabels = arg;};

            void addAddLabel(QString name);
            void addRemoveLabel(QString name);
            
            void build(const QString& link_path, QUrl& url)const override;
            void toJson(QJsonObject& js)const override;
            operator QJsonObject ()const;

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<ModifyMessageArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            QString m_userId;
            QString m_message_id;
            std::list <QString> m_addLabels;
            std::list <QString> m_removeLabels;

        };
    
        class MimeBodyPart 
        {
            friend class SendMimeMessageArg;
        public:
            enum EType
            {
                ptypeNone,
                ptypePlain,
                ptypeHtml,
                ptypeFile
            };
            static MimeBodyPart makePlainPart(QString text);
            static MimeBodyPart makeHtmlPart(QString text);
            static MimeBodyPart makeFilePart(QString file_name);

            QByteArray toRfc822()const;

        protected:
            void setContent(QString content, QString _type);            
        protected:
            QString m_content_type;
            QString m_content;
            EType   m_part_type{ ptypeNone };
        };

        class SendMimeMessageArg : public QParamArgWithBody<SendMimeMessageArg>
        {
        public:
            SendMimeMessageArg();

            ///plain text message constructor
            SendMimeMessageArg(QString from, 
                QString to, 
                QString subject, 
                QString text);

            ///plain+html text message constructor
            SendMimeMessageArg(QString from,
                QString to,
                QString subject,
                QString text_plain,
                QString text_html);

            QString userId()const { return m_userId; }
            void    setUserId(QString id) { m_userId = id; };

            QString getSubject()const { return m_Subject; }
            void setSubject(QString subject) { m_Subject = subject; }

            QString getFrom()const { return m_From; }
            void setFrom(QString from_val) { m_From = from_val; }

            QString getTo()const { return m_To; }
            void setTo(QString to_val) { m_To = to_val; }

            QString getCC()const { return m_CC; }
            void setCC(QString cc_val) { m_CC = cc_val; }

            QString getBCC()const { return m_BCC; }
            void setBCC(QString bcc_val) { m_BCC = bcc_val; }

			QString getThreadId()const { return m_threadId; }
			void    setThreadId(QString thread_id) { m_threadId = m_threadId; }

			QString getInReplyToMsgId()const { return m_InReplyToMsgId; }
			void	setInReplyToMsgId(QString msgId) { m_InReplyToMsgId = msgId; }

			QString	getReferences()const { return m_references; }
			void	setReferences(QString sval) { m_references = sval; }

            void addBodyPart(const MimeBodyPart& pt) { m_body_parts.push_back(pt); };
            void addAttachments(const std::list<QString>& attachments);

            /**
                if rawRfc822MessageFile is set the file content as whole will be loaded
                and sent instead of object members
            */
            QString rawRfc822MessageFile()const { return m_rawRfc822MessageFile; }
            void    setRawRfc822MessageFile(QString fileName) {m_rawRfc822MessageFile = fileName;}

            void build(const QString& link_path, QUrl& url)const override;
            void toJson(QJsonObject& js)const override;
            QByteArray toRfc822()const;
            operator QJsonObject ()const;
            bool saveAsRfc822(QString filePathName)const;

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<SendMimeMessageArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            QString m_userId;
            QString m_From;
            QString m_To;
            QString m_CC;
            QString m_BCC;
            QString m_Subject;
			QString m_threadId;
			QString m_InReplyToMsgId;
			QString m_references;
            QString m_rawRfc822MessageFile;
            std::list<MimeBodyPart> m_body_parts;
        };

        class InsertMessageArg : public PathArg<path_insert, InsertMessageArg>
        {
        public:
            InsertMessageArg();
            void build(const QString& link_path, QUrl& url)const override;

            /**
                The type of upload request to the /upload URI. Acceptable values are:
                media - Simple upload. Upload the media only, without any metadata.
                multipart - Multipart upload. Upload both the media and its metadata, in a single request.
                resumable - Resumable upload. Upload the file in a resumable fashion, using a series of
                at least two requests where the first request includes the metadata.
            */
            QString getUploadType()const { return m_uploadType; }
            void    setUploadType(QString val) { m_uploadType = val; }

            /*
                Mark the email as permanently deleted (not TRASH) and only visible in Google Apps Vault 
                to a Vault administrator. Only used for Google Apps for Work accounts. (Default: false)
            */
            bool    getDeleted()const { return m_deleted; }
            void    setDeleted(bool v) { m_deleted = v; }

            /*
                Source for Gmail's internal date of the message. 
                Acceptable values are:
                "dateHeader": The internal message time is based on the Date header in the email, when valid.
                "receivedTime": The internal message date is set to the time the message is received by Gmail. (default)
            */
            QString getInternalDateSource()const { return m_internalDateSource; }
            void setInternalDateSource(QString s) { m_internalDateSource = s; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<InsertMessageArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            QString m_uploadType;
            bool    m_deleted;
            QString m_internalDateSource;
        };

        class TrashMessageArg: public PathWithIdArg<path_trash, TrashMessageArg>
        {
        public:
            TrashMessageArg(QString user_id, QString idValue) { m_userId = user_id; m_id = idValue; }
            QString userId()const { return m_userId; }
            void    setUserId(QString id) { m_userId = id; };

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<TrashMessageArg> EXAMPLE(int, int);
#endif //API_QT_AUTOTEST

        protected:
            QString m_userId;
        };

        class UntrashMessageArg: public PathWithIdArg<path_untrash, UntrashMessageArg>
        {
        public:
            UntrashMessageArg(QString user_id, QString idValue){ m_userId = user_id; m_id = idValue;}
            QString userId()const { return m_userId; }
            void    setUserId(QString id) { m_userId = id; };

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<UntrashMessageArg> EXAMPLE(int, int);
#endif //API_QT_AUTOTEST

        protected:
            QString m_userId;
        };

        class ImportMessageArg: public PathArg<path_import, ImportMessageArg>
        {
        public:
            ImportMessageArg();
            void build(const QString& link_path, QUrl& url)const override;
            //virtual QString arg()const override;
            /**
                The type of upload request to the /upload URI. Acceptable values are:
                media - Simple upload. Upload the media only, without any metadata.
                multipart - Multipart upload. Upload both the media and its metadata, in a single request.
                resumable - Resumable upload. Upload the file in a resumable fashion, using a series of
                at least two requests where the first request includes the metadata.
            */
            QString getUploadType()const { return m_uploadType; }
            void    setUploadType(QString val) { m_uploadType = val; }

            /*
                Mark the email as permanently deleted (not TRASH) and only visible in Google Apps Vault
                to a Vault administrator. Only used for Google Apps for Work accounts. (Default: false)
            */
            bool    getDeleted()const { return m_deleted; }
            void    setDeleted(bool v) { m_deleted = v; }

            /*
                Source for Gmail's internal date of the message.
                Acceptable values are:
                "dateHeader": The internal message time is based on the Date header in the email, when valid.
                "receivedTime": The internal message date is set to the time the message is received by Gmail. (default)
            */
            QString getInternalDateSource()const { return m_internalDateSource; }
            void setInternalDateSource(QString s) { m_internalDateSource = s; }

            /*
                Ignore the Gmail spam classifier decision and never mark this email as SPAM in the mailbox. 
                (Default: false)
            */
            bool    getNeverMarkSpam()const { return m_neverMarkSpam; }
            void    setNeverMarkSpam(bool v) { m_neverMarkSpam = v; }

            /*
                Process calendar invites in the email and add any extracted meetings to the Google Calendar 
                for this user. (Default: false)
            */
            bool    getProcessForCalendar()const { return m_processForCalendar; }
            void    setProcessForCalendar(bool v) { m_processForCalendar = v; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<ImportMessageArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            QString m_uploadType;
            bool    m_deleted;
            QString m_internalDateSource;
            bool    m_neverMarkSpam;
            bool    m_processForCalendar;
        };

        class BatchDeleteMessageArg: public PathArg<path_batchDelete, BatchDeleteMessageArg>
        {
        };

    };//common
};
