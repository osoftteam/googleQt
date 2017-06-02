googleQt is C++11/Qt adaptation of Google GDrive v3, GMail v1, GTask v1 API. Underneath is simple web API built on HTTP, OAuth 2.0, and JSON.

The API is based on asynchronous and blocking functions. Blocking functions return unique_ptr of typed objects using move semantics or raise exceptions in case of error. The asynchronous functions are organized around GoogleTask object, similar to QNetworkReply.

googleQt is about concurrent processing, not parallel. That means library offers set of classes with asynchronous functions that are executed in free time slots of same thread where they are initiated. No parallel threads are created and to thread synchronizations are needed. The library encourages recursive calls when chaining of asynchronous operations are needed - it simplifies code and allows cleaner implementations. Sample of asynchronous chaining can be demonstrated in following sample:
```
Problem: download last N message with attachments.

The task gets splitted into subtask:
1.list N messages to get N Ids 
2.download N messages using Ids
3.download attachment for each message
```
All steps can be organized to run asynchronously so when one task gets finished the next would be scheduled. In real application step #1 can be implemented as series of group requests with 'next' token. To simplify implementation of such algorithms we introduced GoogleTask class with following methods:

```
GoogleTask<T>

signals:
   void finished();
```
functions to query state:
```
  bool isCompleted()
  bool isFailed()
```
function to get access to Result class or Exception object in case of failure:
```
  DropboxException* error()
  T* get()
```
function to turn async operation into blocking:
```
  std::unique_ptr<T> waitForResultAndRelease();
```

function to chain one task to another:
```
  then(std::function... after_completed, std::function... on_error)
```

Here is sample of chaining tasks when downloading attachments of one email:
```
    auto t = m_gm->getMessages()->get_Async(gmail::IdArg(m_c.userId(), msgId));
    t->then([=](std::unique_ptr<messages::MessageResource> r) 
    {
        auto p = r->payload();
        auto parts = p.parts();
        m_batch_counter = parts.size();
        for (auto pt : parts) {
            auto b = pt.body();
            if (b.size() > 0 && !b.attachmentid().isEmpty()) {
                gmail::AttachmentIdArg arg(m_c.userId(), msgId, b.attachmentid());
                auto a = m_gm->getAttachments()->get_Async(arg);
                a->then([=](std::unique_ptr<attachments::MessageAttachment> att)
                {
                    store_attachment(att.get(), pt.filename());
                });
            }
        }//for parts
    });
```
The sample implementation using blocking calls:
```
    try
        {
            gmail::IdArg arg(m_c.userId(), msgId);
            auto r = m_gm->getMessages()->get(arg);
            auto p = r->payload();
            auto parts = p.parts();
            for (auto pt : parts){
                    auto b = pt.body();
                    if(b.size() > 0 && !b.attachmentid().isEmpty()){
                        gmail::AttachmentIdArg arg(m_c.userId(), msgId, b.attachmentid());
                        auto r = m_gm->getAttachments()->get(arg);
                        store_attachment(r, pt.filename());
                    }
                }            
        }
    catch(GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
```

Notice blocking calls are wrapped in try/catch block, that is because blocking functions (without Async suffix) can throw exception - when data are wrong, network issues etc. Not-blocking functions have suffix 'Async' and don't throw exceptions but return GoogleTask objects that can be connected to, chained and used to retrieve result data and error description if any. If object are not chained via 'then' and not turned into blocking calls with 'waitForResultAndRelease' they should be scheduled to delete using 'deleteLater'.

### Requirements. C++11 compiler and Qt 5.xx. We used VS2015, GCC 5.xx, XCode 8.xx. Anything newer/better will do.

### Installation Run qmake in 'prj' folder to generate Makefile, compile, see generated static library googleQt. To create sample project, cd in 'examples' and run qmake for each subfolder. There is shell script to open IDE on windows and generate project files - see folder 'bin' and win-configure-open-ide.bat or Cygwin based cygwin-build.csh. You have to add googleQt to your LIB list (as shown in sample) and add path to prj/src to the INCLUDE list.

### Current status - work in progress, getting ready for beta. Resumable uploads will be implemented later also handling slow connections, concurrent requests partially implemented for Gmail. Caching of GMail messages under development.

### Features
The library covers GDrive, GMail, GTask but not completely, particularly resumable downloads are not implemented. Also sending emails functions are not fully tested for various destination client (for encoding etc). 

- connect to Gdrive, Gmail, GTask Google API.
- request and autorefresh OAuth2 token
- asynchronous and blocking interfaces
- specialized exceptions for blocking calls
- parallel requests for selected services
- caching cloud data for selected services

# License

LGPL v3
