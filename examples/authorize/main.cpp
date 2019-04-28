#include <iostream>
#include <memory>
#include <QUrl>
#include <QCoreApplication>
#include <QFile>
#include "google/endpoint/ApiAppInfo.h"
#include "google/endpoint/GoogleWebAuth.h"
#include "google/endpoint/ApiException.h"

using namespace googleQt;

void saveScopeUrl(QUrl url)
{
    QString UrlFile = "scope-auth-url.info";
    QFile out(UrlFile);
    if (!out.open(QFile::WriteOnly | QIODevice::Truncate)) {
        std::cout << "Error opening file: " << UrlFile.toStdString() << std::endl;
        return;
    }

    out.write(url.toString().toStdString().c_str());
    out.write("\n");
    out.close();

    std::cout << "Link saved to file: " << UrlFile.toStdString() << std::endl;
};

int main(int argc, char *argv[]) 
{
    QCoreApplication app(argc, argv);

    if (argc < 3) {
        std::string  s = QString("Usage:   %1 <app-info-file> <auth-file-output>\n"
                                 "Usage: %1 %1 <app-info-file> <auth-file-output> refresh\n"
                                 "Example: %1 ../app.info ../token.info\n"
                                 "Example: %1 ../app.info ../token.info refresh\n"
                                 "\n"
                                 "<app-info-file>: a JSON file with information about your API app.  Example:\n"
                                 "\n"
                                 "  {\n"
                                 "    \"key\": \"Your Google API app key...\",\n"
                                 "    \"secret\": \"Your Google API app secret...\"\n"
                                 "  }\n"
                                 "\n"
                                 "  Get an API app key by registering with Google:\n"
                                 "    https://console.developers.google.com \n"
                                 "\n"
                                 "<auth-file-output>: If authorization is successful, the resulting API \n"
                                 "  access token will be saved to this file, which can then be used with\n"
                                 "  other example programs, such as the one in \"examples/account-info\". \n"
                                 "\n"
                                 "refresh: refresh existing token file\n"
                                 " Press ENTER to proceed.").arg(argv[0]).toStdString();

        std::cout << s << std::endl;
        std::cout << std::endl;
        std::cin.ignore();
        return 0;
    }

    bool refreshToken = false;
    QString argAppInfoFile = argv[1];
    QString argAuthFileOutput = argv[2];
    if(argc == 4)
        {
            QString argRefresh = argv[3];
            if(argRefresh.compare("refresh") == 0){
                refreshToken = true;
            }
        }
    
    std::unique_ptr<ApiAppInfo> appInfo(new ApiAppInfo);
    if(!appInfo->readFromFile(argAppInfoFile)){
        std::cerr << "Error reading <app-info-file>" << std::endl;
        return 0;
    };

    try
        {
            std::unique_ptr<ApiAuthInfo> authInfo( new ApiAuthInfo(argAuthFileOutput));
            if(refreshToken){
                if(!authInfo->reload()){
                    std::cout << "Failed to load token: " << argAuthFileOutput.toStdString() << std::endl;
                    return 0;
                }

                if(!GoogleWebAuth::refreshToken(appInfo.get(), authInfo.get()))
                    {
                        std::cout << "Error, failed to store access token to file: " << argAuthFileOutput.toStdString() << std::endl;
                    }
                else
                    {
                        std::cout << "token file refreshed: " << argAuthFileOutput.toStdString() << std::endl;
                    }                
            }
            else
                {
                    STRING_LIST scopes;
                    scopes.push_back(GoogleWebAuth::authScope_gmail_modify());
                    scopes.push_back(GoogleWebAuth::authScope_tasks());
                    scopes.push_back(GoogleWebAuth::authScope_gdrive());
                    scopes.push_back(GoogleWebAuth::authScope_gdrive_appdata());
                    scopes.push_back(GoogleWebAuth::authScope_contacts_modify());
                    QUrl url = GoogleWebAuth::getCodeAuthorizeUrl(appInfo.get(), scopes);
                    saveScopeUrl(url);

                    std::cout << "1. Go to " << url.toString().toStdString() << std::endl;
                    std::cout << "2. Click \"Allow\" (you might have to log in first)." << std::endl;
                    std::cout << "3. Copy the authorization code." << std::endl;
                    std::cout << "Enter the authorization code here: " << std::endl;
                    std::cout << ">";
    
                    std::string auth_code;
                    std::cin >> auth_code;

                    
                    if(!GoogleWebAuth::getTokenFromCode(appInfo.get(), QString::fromStdString(auth_code), authInfo.get()))
                        {
                            std::cout << "Error, failed to store access token to file: " << argAuthFileOutput.toStdString() << std::endl;
                        }
                    else
                        {
                            std::cout << "finished, token stored in file " << argAuthFileOutput.toStdString() << std::endl;
                        }
                }
        }
    catch(GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }

    std::cin.ignore();
    return 0;
}
