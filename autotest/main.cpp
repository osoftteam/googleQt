#include <iostream>
#include <QtCore/QCoreApplication>
#include "GoogleClient.h"
#include "google/AUTOTEST/GoogleAutotest.h"

using namespace googleQt;


int main(int argc, char *argv[]) {

    QCoreApplication app(argc, argv);

    if (argc != 2) {
        std::string  s = "\nUsage: COMMAND <destination-file>\n"
        "\n"
        "<destination-file>: The file where results of autotest procedure will be generated"
        " Press ENTER to proceed.";

        std::cout << s << std::endl;
        std::cin.ignore();
        return 0;
    }

    QString argResFile = argv[1];
    std::unique_ptr<ApiAppInfo>appInfo(new ApiAppInfo);
    appInfo->setKeySecret("my-key", "my-secret");
    std::unique_ptr<ApiAuthInfo> authInfo(new ApiAuthInfo());
    authInfo->setEmail("me@gmail.com");
    GoogleClient c(appInfo.release(), authInfo.release());
    {
        GoogleAutotest autotest(c);
        if (!autotest.init(argResFile)) {
            std::cout << "Error opening destination file " << argResFile.toStdString() << std::endl;
            std::cin.ignore();
            return 0;
        }
        std::cout << "Press ENTER to start autotest. The result will be generated: " << argResFile.toStdString() << std::endl;
        std::cin.ignore();
        autotest.generateCalls();
        std::cout << "finished" << std::endl;
    }
    std::cin.ignore();

    return 0;
}
