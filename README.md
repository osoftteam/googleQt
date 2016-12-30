googleQt is C++11/Qt adaptation of Google GDrive v3, GMail v1, GTask v1 API. Underneath is simple web API built on HTTP, OAuth 2.0, and JSON.

The API is based on asynchronous and blocking functions. Blocking functions return unique_ptr of typed objects using move semantics or raise exceptions in case of error. The asynchronous functions are organized around GoogleTask<T> object, similiar to QNetworkReply

The object has signal to connect:
```
void finished();
```
two functions to query state:
```
bool isCompleted()
bool isFailed()
```

and two function to get access to Result class or Exception object in case of failure:
```
GoogleException* error()
T* get()
```
The object of GoogleTask<T> type should be deleted from 'finished' slots via 'deleteLater', similiar to QNetworkRepy. It is easy to wait for asynchronous task to complete using
```
std::unique_ptr<RESULT> waitForResultAndRelease();
```
The function makes call blocking, auto releases Task object and moves result to caller, if available. In case error function will capture failed callback and raise GoogleException (or derived).

###Requirement.
Qt 5.xx and C++11 compiler - VS2015, GCC 5.xx, clang from XCode 6.xx or newer versions.

###Installation
Run qmake in 'prj' folder to generate Makefile, compile, see generated static library googleQt.
To create sample project, cd in 'examples' and run qmake for each subfolder. There is shell script to open IDE
on windows and generate project files - see folder 'bin' and win-configure-open-ide.bat
You have to add googleQt to your LIB list (as shown in sample) and add path to cprj/src to the INCLUDE list.


###Current status
Work in progress, getting ready for beta.
Resumable uploads will be implemeted later also handling slow connections, concurrent requests partially implemented for Gmail. Caching of GMail messages under development.

###Features
- connect to Gdrive, Gmail, GTask Google API.
- request and autorefresh OAuth2 token
- asynchronous and blocking interfaces
- specialized exceptions for blocking calls
- parallel requests for selected services
- caching cloud data for selected services


# License
LGPL v3