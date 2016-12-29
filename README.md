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
The object of GoogleTask<T> type should be deleted from 'finished' slots via 'deleteLater', similiar to QNetworkRepy.


###Current status
Work in progress, getting ready for beta.
Resumable gdrive upload will be implemeted later, concurrent parallel requests implemented for Gmail. Caching for GMail not completed.

###Features
- connect to Gdrive, Gmail, GTask Google API.
- request OAuth2 token
- autorefresh access tokem
- functions & classes to work with API
- methods for concurrent parralel requests
- caching cloud data for selected services


# License
LGPL v3