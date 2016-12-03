#pragma once
#include "google/endpoint/ApiUtil.h"

namespace googleQt {
    namespace gdrive {
        DECLARE_PATH_CLASS(about);
        DECLARE_PATH_CLASS(trash);

        class AboutArg : public PathArg<path_about, AboutArg>
        {
        public:
            AboutArg();
            void setFields(QString val) { m_Fields = val; };
            void clearFields() { m_Fields = ""; };
            void build(const QString& link_path, QUrl& url)const override;
        protected:
            QString m_Fields;
        };

        class FileListArg : public QParamArg
        {
        public:
            FileListArg(QString pageToken = "");
            
            void build(const QString& link_path, QUrl& url)const override;

            /**
            The source of files to list. 
            Acceptable values are:
            "domain": Files shared to the user's domain.
            "user": Files owned by or shared to the user. (default)
            */
            QString getCorpus()const { return m_corpus; }
            void    setCorpus(QString val) { m_corpus = val; }

            /**
                A comma-separated list of sort keys. Valid keys are 'createdTime', 'folder', 
                'modifiedByMeTime', 'modifiedTime', 'name', 'quotaBytesUsed', 'recency', 
                'sharedWithMeTime', 'starred', and 'viewedByMeTime'. Each key sorts ascending by 
                default, but may be reversed with the 'desc' modifier. 
                Example usage: ?orderBy=folder,modifiedTime desc,name. 
                Please note that there is a current limitation for users with approximately 
                one million files in which the requested sort order is ignored.
            */
            QString getOrderBy()const { return m_orderBy; }
            void    setOrderBy(QString val) { m_orderBy = val; }

            /**
                The maximum number of files to return per page. Acceptable values are 1 to 1000, 
                inclusive. (Default: 100)
            */
            int     getPageSize()const { return m_pageSize; }
            void    setPageSize(int val) { m_pageSize = val; }

            /**
                The token for continuing a previous list request on the next page. This should 
                be set to the value of 'nextPageToken' from the previous response.
            */
            QString getPageToken()const { return m_pageToken; }
            void    setPageToken(QString val) { m_pageToken = val; }

            /**
                A query for filtering the file results. See the "Search for Files" guide 
                for supported syntax.
            */
            QString getQ()const { return m_q; }
            void    setQ(QString val) { m_q = val; }

            /**
                A comma-separated list of spaces to query within the corpus. 
                Supported values are 'drive', 'appDataFolder' and 'photos'.
            */
            QString getSpaces()const { return m_spaces; }
            void    setSpaces(QString val) { m_spaces = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<FileListArg> EXAMPLE();
#endif //API_QT_AUTOTEST

        protected:
            QString m_corpus;
            QString m_orderBy;
            int     m_pageSize;
            QString m_pageToken;
            QString m_q;
            QString m_spaces;
        };

        class GetFileArg : public QParamArg
        {
        public:
            GetFileArg(QString fileId = "");
            void build(const QString& link_path, QUrl& url)const override;

            /**
                The ID of the file.
            */
            QString getFileId()const { return m_fileId; }
            void    setFileId(QString val) { m_fileId = val; }

            /**
                Whether the user is acknowledging the risk of downloading known malware or 
                other abusive files. This is only applicable when alt=media. (Default: false)
            */
            bool    getAcknowledgeAbuse()const { return m_acknowledgeAbuse; }
            void    setAcknowledgeAbuse(bool val) { m_acknowledgeAbuse = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<GetFileArg> EXAMPLE();
#endif //API_QT_AUTOTEST

        protected:
            QString m_fileId;
            bool    m_acknowledgeAbuse;
        };//FileIdArg
        
        class DownloadFileArg : public QParamArg
        {
        public:
            DownloadFileArg(QString fileId = "");
            void build(const QString& link_path, QUrl& url)const override;

            /**
            The ID of the file.
            */
            QString getFileId()const { return m_fileId; }
            void    setFileId(QString val) { m_fileId = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<DownloadFileArg> EXAMPLE();
#endif //API_QT_AUTOTEST

        protected:
            QString m_fileId;
        };      

        class CopyFileArg : public QParamArg
        {
        public:
            CopyFileArg();
            //virtual QString arg()const override;
            void build(const QString& link_path, QUrl& url)const override;

            /**
            The ID of the file.
            */
            QString getFileId()const { return m_fileId; }
            void    setFileId(QString val) { m_fileId = val; }

            /**
            Whether to ignore the domain's default visibility settings for the created file. 
            Domain administrators can choose to make all uploaded files visible to the domain 
            by default; this parameter bypasses that behavior for the request. Permissions 
            are still inherited from parent folders. (Default: false)
            */
            bool    getIgnoreDefaultVisibility()const { return m_ignoreDefaultVisibility; }
            void    setIgnoreDefaultVisibility(bool val) { m_ignoreDefaultVisibility = val; }

            /**
            Whether to set the 'keepForever' field in the new head revision. This is only applicable 
            to files with binary content in Drive. (Default: false)
            */
            bool    getKeepRevisionForever()const { return m_keepRevisionForever; }
            void    setKeepRevisionForever(bool val) { m_keepRevisionForever = val; }

            /**
            A language hint for OCR processing during image import (ISO 639-1 code).
            */
            QString getOcrLanguage()const { return m_ocrLanguage; }
            void    setOcrLanguage(QString val) { m_ocrLanguage = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<CopyFileArg> EXAMPLE();
#endif //API_QT_AUTOTEST

        protected:
            QString m_fileId;
            bool    m_ignoreDefaultVisibility;
            bool    m_keepRevisionForever;
            QString m_ocrLanguage;
        };//CopyFileArg

        class DeleteFileArg: public QParamArg
        {
        public:
            DeleteFileArg(QString fileId = "");
            void build(const QString& link_path, QUrl& url)const override;

            /**
            The ID of the file.
            */
            QString getFileId()const { return m_fileId; }
            void    setFileId(QString val) { m_fileId = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<DeleteFileArg> EXAMPLE();
#endif //API_QT_AUTOTEST

        protected:
            QString m_fileId;
        };

        class CreateFileArg : public QParamArg
        {
        public:
            CreateFileArg();
            void build(const QString& link_path, QUrl& url)const override;

            /**
                The type of upload request to the /upload URI. Acceptable values are:
                media - Simple upload. Upload the media only, without any metadata.
            */
            //            QString getUploadType()const { return m_uploadType; }
            //void    setUploadType(QString val) { m_uploadType = val; }

            /**
                Whether to ignore the domain's default visibility settings for the created file. 
                Domain administrators can choose to make all uploaded files visible to the domain by default; 
                this parameter bypasses that behavior for the request. Permissions are still inherited from 
                parent folders. (Default: false)
            */
            bool    getIgnoreDefaultVisibility()const { return m_ignoreDefaultVisibility; }
            void    setIgnoreDefaultVisibility(bool val) { m_ignoreDefaultVisibility = val; }

            /**
                Whether to set the 'keepForever' field in the new head revision. 
                This is only applicable to files with binary content in Drive. (Default: false)
            */
            bool    getKeepRevisionForever()const { return m_keepRevisionForever; }
            void    setKeepRevisionForever(bool val) { m_keepRevisionForever = val; }

            /**
                A language hint for OCR processing during image import (ISO 639-1 code).
            */
            QString getOcrLanguage()const { return m_ocrLanguage; }
            void    setOcrLanguage(QString val) { m_ocrLanguage = val; }

            /**
                    Whether to use the uploaded content as indexable text. (Default: false)
            */
            bool    getUseContentAsIndexableText()const { return m_useContentAsIndexableText; }
            void    setUseContentAsIndexableText(bool val) { m_useContentAsIndexableText = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<CreateFileArg> EXAMPLE();
#endif //API_QT_AUTOTEST

        protected:
            //QString m_uploadType;
            bool    m_ignoreDefaultVisibility;
            bool    m_keepRevisionForever;
            QString m_ocrLanguage;
            bool    m_useContentAsIndexableText;
        };
        
        class EmptyTrashArg: public PathArg<path_trash, EmptyTrashArg>
        {
        public:

        };//EmptyTrashArg
        
        class PermissionArg : public QParamArg
        {
        public:
            PermissionArg();
            PermissionArg(QString fileId, QString permissionId);
            void build(const QString& link_path, QUrl& url)const override;

            /**
            The ID of the file.
            */
            QString getFileId()const { return m_fileId; }
            void    setFileId(QString val) { m_fileId = val; }

            /**
            The ID of the permission.
            */
            QString getPermissionId()const { return m_permissionId; }
            void    setPermissionId(QString val) { m_permissionId = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<PermissionArg> EXAMPLE();
#endif //API_QT_AUTOTEST

        protected:
            QString m_fileId;
            QString m_permissionId;
        };

        class CreatePermissionArg : public QParamArg
        {
        public:
            CreatePermissionArg();
            void build(const QString& link_path, QUrl& url)const override;

            /**
            The ID of the file.
            */
            QString getFileId()const { return m_fileId; }
            void    setFileId(QString val) { m_fileId = val; }

            /**
                A custom message to include in the notification email.
            */
            QString getEmailMessage()const { return m_emailMessage; }
            void    setEmailMessage(QString val) { m_emailMessage = val; }

            /**
                Whether to send a notification email when sharing to users or groups. 
                This defaults to true for users and groups, and is not allowed for other 
                requests. It must not be disabled for ownership transfers.
            */
            bool    getSendNotificationEmail()const { return m_sendNotificationEmail; }
            void    setSendNotificationEmail(bool val) { m_sendNotificationEmail = val; }

            /**
                Whether to transfer ownership to the specified user and downgrade the current 
                owner to a writer. This parameter is required as an acknowledgement of the 
                side effect. (Default: false)
            */
            bool    getTransferOwnership()const { return m_transferOwnership; }
            void    setTransferOwnership(bool val) { m_transferOwnership = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<CreatePermissionArg> EXAMPLE();
#endif //API_QT_AUTOTEST

        protected:
            QString m_fileId;
            QString m_emailMessage;
            bool    m_sendNotificationEmail;
            bool    m_transferOwnership;
        };

        class PermissionListArg : public QParamArg
        {
        public:
            PermissionListArg(QString fileId = "");
            void build(const QString& link_path, QUrl& url)const override;

            /**
            The ID of the file.
            */
            QString getFileId()const { return m_fileId; }
            void    setFileId(QString val) { m_fileId = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<PermissionListArg> EXAMPLE();
#endif //API_QT_AUTOTEST

        protected:
            QString m_fileId;
        };

        
        class CreateCommentArg : public QParamArg
        {
        public:
            CreateCommentArg();
            CreateCommentArg(QString fileId);
            void build(const QString& link_path, QUrl& url)const override;

            /**
            The ID of the file.
            */
            QString getFileId()const { return m_fileId; }
            void    setFileId(QString val) { m_fileId = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<CreateCommentArg> EXAMPLE();
#endif //API_QT_AUTOTEST

        protected:
            QString m_fileId;
        };
        
        class DeleteCommentArg : public QParamArg
        {
        public:
            DeleteCommentArg();
            DeleteCommentArg(QString fileId, QString commentId);
            void build(const QString& link_path, QUrl& url)const override;

            /**
            The ID of the file.
            */
            QString getFileId()const { return m_fileId; }
            void    setFileId(QString val) { m_fileId = val; }

            /**
            The ID of the comment.
            */
            QString getCommentId()const { return m_commentId; }
            void    setCommentId(QString val) { m_commentId = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<DeleteCommentArg> EXAMPLE();
#endif //API_QT_AUTOTEST

        protected:
            QString m_fileId;
            QString m_commentId;
        };

        
        class GetCommentArg : public QParamArg
        {
        public:
            GetCommentArg();
            GetCommentArg(QString fileId, QString commentId);
            void build(const QString& link_path, QUrl& url)const override;

            /**
            The ID of the file.
            */
            QString getFileId()const { return m_fileId; }
            void    setFileId(QString val) { m_fileId = val; }

            /**
            The ID of the comment.
            */
            QString getCommentId()const { return m_commentId; }
            void    setCommentId(QString val) { m_commentId = val; }

            /**
            Whether to return deleted comments. Deleted comments will not include their original content. (Default: false)
            */
            bool getIncludeDeleted()const { return m_includeDeleted; }
            void setIncludeDeleted(bool val) { m_includeDeleted = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<GetCommentArg> EXAMPLE();
#endif //API_QT_AUTOTEST

        protected:
            QString m_fileId;
            QString m_commentId;
            bool m_includeDeleted;
        };
        
        class CommentListArg : public QParamArg
        {
        public:
            CommentListArg(QString fileId = "");
            void build(const QString& link_path, QUrl& url)const override;

            /**
            The ID of the file.
            */
            QString getFileId()const { return m_fileId; }
            void    setFileId(QString val) { m_fileId = val; }

            /**
                Whether to include deleted comments. Deleted comments will not include their 
                original content. (Default: false)
            */
            bool    getIncludeDeleted()const { return m_includeDeleted; }
            void    setIncludeDeleted(bool val) { m_includeDeleted = val; }

            /**
                The maximum number of comments to return per page. Acceptable values 
                are 1 to 100, inclusive. (Default: 20)
            */
            int     getPageSize()const { return m_pageSize; }
            void    setPageSize(int val) { m_pageSize = val; }

            /**The token for continuing a previous list request on the next page. 
            This should be set to the value of 'nextPageToken' from the previous response.              
            */
            QString getPageToken()const { return m_pageToken; }
            void    setPageToken(QString val) { m_pageToken = val; }

            /**
            The minimum value of 'modifiedTime' for the result comments (RFC 3339 date-time).
            */
            const QDateTime& getStartModifiedTime()const { return m_startModifiedTime; }
            void      setStartModifiedTime(const QDateTime& val) { m_startModifiedTime = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<CommentListArg> EXAMPLE();
#endif //API_QT_AUTOTEST

        protected:
            QString m_fileId;
            bool    m_includeDeleted;
            int     m_pageSize;
            QString m_pageToken;
            QDateTime m_startModifiedTime;
        };

    };//gdrive
};//googleQt
