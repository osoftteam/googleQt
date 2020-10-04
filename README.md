google QR is a C++11/Qt adaptation of Google GDrive v3, Contacts API v3, GMail v1, GTask v1 API. Underneath is a simple web API built on HTTP, OAuth 2.0, and JSON.

The API is based on asynchronous and blocking functions. Blocking functions return the unique_ptr of typed objects using move semantics or raise exceptions in case of error. The asynchronous functions are organized around the GoogleTask object, similar to QNetworkReply.

google QR is about concurrent processing, not parallel. That means the library offers a set of classes with asynchronous functions that are executed in free time slots of the same thread where they are initiated. No parallel threads are created and to the thread, synchronizations are needed. The library encourages recursive calls when chaining of asynchronous operations are needed - it simplifies code and allows cleaner implementations. The asynchronous chaining can be demonstrated in the following sample:
```
Problem: download the last N message with attachments.

The task gets split into subtask:
1.list N messages to get N Ids 
2.download N messages using Ids
3.download attachment for each message
```
All steps can be organized to run asynchronously so when one task gets finished the next would be scheduled. In real application step #, 1 can be implemented as a series of group requests with 'next' token (so really there are 4 steps and #1 is a recursive one). To simplify the implementation of such algorithms we introduced GoogleTask class with the following methods:

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
  GoogleException* error()
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

Notice blocking calls are wrapped in try/catch block, that is because blocking functions (without Async suffix) can throw an exception - when the server is unhappy about a request and responds with 400, network issues, etc. Not-blocking functions have the suffix 'Async' and don't throw exceptions but return GoogleTask objects that can be connected to, chained, and used to retrieve result data and error description if any. If objects are not chained via 'then' and not turned into blocking calls with 'waitForResultAndRelease' they should be scheduled to delete using 'delete later'.


### More examples
Please visit the examples folder, there is one console-based project for each kind of API. We call them bash-projects because they offer a small menu with options to explore and test API and demonstrate code snippets. You will find drivebys, Gmail bash, gas bash, etc.

Rename GDrive file
```
    try
        {    
            RenameFileArg arg(fileId);
            arg.setName(fileName);
            arg.setFields("id,name,size,mimeType,webContentLink");
            m_gd->getFiles()->rename(arg);
        }    
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
```

Download GDrive file
```
    QFile out(fileName);
    if (!out.open(QFile::WriteOnly | QIODevice::Truncate)) {
        std::cout << "Error opening file: " << fileName.toStdString() << std::endl;
        return;
    }
    try
        {
            DownloadFileArg arg(fileId);
            m_gd->getFiles()->downloadFile(arg, &out);
            out.flush();
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
```

List Gmail labels
```
    try
        {
            auto labels_list = m_gm->getLabels()->list();
            for (auto lbl : labels_list->labels())
                {
                    std::cout << "id=" << lbl.id() 
                              << " name=" << lbl.name() 
                              << " type=" << lbl.type() 
                              << " messagestotal=" << lbl.messagestotal()
                              << " unread=" << lbl.messagesunread()
                              << " threadstotal=" << lbl.threadstotal()
                              << std::endl;
                }

        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
```

List Gmail messages
```
    try
        {
            gmail::ListArg listArg;
            listArg.setMaxResults(10);
            auto mlist = m_gm->getMessages()->list(listArg);
            for(auto m : mlist->messages())
                {
                    std::cout << = m.id() << std::endl;
                }
            auto nextToken = mlist->nextpagetoken();
            if(!nextToken.isEmpty())
                {
                    std::cout << "next-token: " << nextToken << std::endl;
                }
        }
    catch(GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
```

Send an email message with attachments
```
    QString msg_to = arg_list[0];
    QString msg_subject = arg_list[1];
    QString msg_text = arg_list[2];
    QString msg_html = QString("<div dir=\"ltr\">%1</div>").arg(msg_text);

    QDir dir("upload");
    if (!dir.exists()) {
        std::cout << "Upload-directory not found." << std::endl;
        return;
    }

    QFileInfoList flist = dir.entryInfoList(QDir::Files);
    if (flist.size() == 0) {
        std::cout << "Upload-directory is empty, nothing to attach" << std::endl;
        return;
    }

    std::list<QString> attachments;
    for (auto& fi : flist) {
        attachments.push_back(fi.absoluteFilePath());
    }

    try
        {
            gmail::SendMimeMessageArg arg(m_c.userId(), msg_to, msg_subject, msg_text, msg_html);
            arg.addAttachments(attachments);
            auto r = m_gm->getMessages()->send(arg);
            printMessage(r.get());
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
```



### Requirements.
C++11 compiler and Qt 5.xx. We used VS2015, GCC 5.xx, XCode 8.xx. Anything newer/better will do.

### Installation
Run make in 'project' folder to generate Makefile, compile, see generated static library google QR. To create a sample project, cd in 'examples' and run make for each subfolder. There is a shell script to open IDE on windows and generate project files - see folder 'bin' and win-configure-open-ide.bat or Cygwin based Cygwin-build.csh. You have to add google QR to your LIB list (as shown in sample) and add a path to project/src to the INCLUDE list.

### Current status.
contacts namespace is getting into shape but still under development, expect surprises, others should be OK.


### Design and development.
At the core of the library are basic interfaces that provide access to Google services using documented HTTP/JSON protocols. Those classes are generated from doc-specifications called STONE. STONE was designed by Dropbox as the declarative language to maintain their APIs. We adapted STONE python machinery for C++11/Qt/Dropbox interfaces first and then for selected Google APIs. Our main goal was Gmail and GDrive as future engines for Ardi-application (see prokarpaty.net). At one point we decided to abstract caching classes from Ardi into google QR as well. We estimated the development of stable GMail cache and core interfaces for GDrive, GTask in 1 year. It includes also autotests and sample projects. 7 months already passed and we have a beta of rich GUI hosting application using all this code (except GTasks).


### Features
The library covers GDrive, Gmail, GTalk but not completely, particularly resumable downloads are not implemented. Also sending email functions is not fully tested for various destination clients (for encoding etc). 

- connect to Gdrive, Gmail, GTalk Google API.
- request and autorefresh OAuth2 token
- asynchronous and blocking interfaces
- specialized exceptions for blocking calls
- concurrent batch requests for Gmail (up to 4 requests simultaneously)
- caching cloud data for selected services

