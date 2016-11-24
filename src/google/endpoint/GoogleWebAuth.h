#pragma once

#include <QString>
#include "ApiAuthInfo.h"
#include "ApiAppInfo.h"

namespace googleQt{
    class GoogleAppInfo;

    class GoogleWebAuth
    {
    public:
        /**
            getCodeAuthorizeUrl - format string that should be opened to enable
            Dropbox access and request for access token, which will be used in all
            API interactions.
        */
        static QUrl getCodeAuthorizeUrl(const ApiAppInfo* appInfo, QString scope);

        static QUrl getCodeAuthorizeUrl(const ApiAppInfo* appInfo, const std::list<QString>& scopes);

        /**
           getTokenFromCode - makes http call to Dropbox to retrive
           access token by providing authorize code
         */
        static bool getTokenFromCode(const ApiAppInfo* appInfo, QString code, ApiAuthInfo* auth);

        /**
           refreshToken - makes http call to Dropbox to retrive
           access token by providing refresh token
         */
        static bool refreshToken(const ApiAppInfo* appInfo, ApiAuthInfo* auth);
        
        
        /**
         *  Read all resources and their metadata—no write operations.
         */
        static QString authScope_gmail_readonly();

        /**
         * Create, read, update, and delete drafts. Send messages and drafts.
         */
        static QString authScope_gmail_compose();

        /**
         * Send messages only. No read or modify privileges on mailbox.
         */
        static QString authScope_gmail_send();

        /**
         * All read/write operations except immediate, permanent deletion of threads and messages, bypassing Trash.
         */
        static QString authScope_gmail_modify();

        /**
         * Full access to the account, including permanent deletion of threads and messages.
         */
        static QString authScope_full_access();

        /**
        * read/write access to Tasks
        */
        static QString authScope_tasks();

        /**
        * read-only access to Tasks
        */
        static QString authScope_tasks_readonly();

        /**
        * GDrive access
        */
        static QString authScope_gdrive();

        /**
        * GDrive readonly access
        */
        static QString authScope_gdrive_readonly();

    protected:
        static bool updateToken(const QUrl& url, ApiAuthInfo* auth, const QString& str);
    };
};
