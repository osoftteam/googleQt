#pragma once

#include <list>
#include <set>
#include <memory>
#include <functional>
#include <QString>
#include <QDebug>
#include <QDateTime>
#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QNetworkReply>
#include <QStringList>
#include <QUrlQuery>
#include "ApiException.h"
#include "GoogleTask.h"
#ifdef API_QT_AUTOTEST
#include "ApiAutotest.h"
#endif

namespace googleQt{
    
    bool loadJsonFromFile(QString path, QJsonObject& js);
    bool storeJsonToFile(QString path, const QJsonObject js);

    template<class T> 
    QJsonArray struct_list2jsonarray(const std::list<T>& lst)
    {
        QJsonArray rv;
        for(typename std::list<T>::const_iterator i = lst.cbegin(); i != lst.end(); i++){
            T o = *i;
            QJsonObject js(o);
            rv.append(js);
        }
        return rv;
    };    

    template<class T> 
    QJsonArray struct_list2jsonarray_uptr(const std::list<std::unique_ptr<T>>& lst)
    {
        QJsonArray rv;
        for(typename std::list<std::unique_ptr<T>>::const_iterator i = lst.cbegin(); i != lst.end(); i++){
            const std::unique_ptr<T>& o = *i;
            QJsonObject js;
            o->toJson(js);
            rv.append(js);
        }
        return rv;
    };    
    

    template<class T> 
    QJsonArray list_of_struct_list2jsonarray(const std::list <std::list<T>> & lst)
    {
        QJsonArray rv;
        for(typename std::list <std::list<T> >::const_iterator i = lst.cbegin(); i != lst.end(); i++){
            const std::list<T>& IL = *i;
            QJsonArray arr2 = struct_list2jsonarray(IL);
            rv.append(arr2);
        }
        return rv;
    };

    template<class T> 
    QJsonArray ingrl_list2jsonarray(const std::list<T>& lst)
    {
        QJsonArray rv;
        for(typename std::list<T>::const_iterator i = lst.cbegin(); i != lst.end(); i++){
            const T o = *i;
            rv.append(o);
        }
        return rv;
    };
    
    template<class T> 
    void jsonarray2struct_list(QJsonArray ar, std::list<T>& lst)
    {
        int Max = ar.size();
        for (int i = 0; i < Max; ++i){
            QJsonObject js = ar[i].toObject();
            T o;
            o.fromJson(js);
            lst.push_back(o);
        }
    };

    template<class T> 
    void jsonarray2struct_list_uptr(QJsonArray ar, std::list<std::unique_ptr<T>>& lst)
    {
        int Max = ar.size();
        for (int i = 0; i < Max; ++i){
            QJsonObject js = ar[i].toObject();
            std::unique_ptr<T> o = T::factory::create(js);
            o->fromJson(js);
            lst.emplace_back(std::move(o));
        }
    };

    
    template<class T> 
    void jsonarray2list(QJsonArray arr, std::list<T>& lst, std::true_type)
    {
# if QT_VERSION > QT_VERSION_CHECK(5, 6, 0)
        int Max = arr.size();
        for (int i = 0; i < Max; ++i){
            int v = arr[i].toInt();
            lst.push_back(v);
        }
#else
        int Max = arr.size();
        for (int i = 0; i < Max; ++i){
            QString tmp = arr[i].toString();
            lst.push_back(tmp.toInt());
        }

#endif
    }

    template<class T> 
    void jsonarray2list(const QJsonArray& arr, std::list<T>& lst, std::false_type)
    {
        int Max = arr.size();
        for (int i = 0; i < Max; ++i){
            T v = arr[i].toString();
            lst.push_back(v);
        }
    }

    template<class T> 
    void jsonarray2ingrl_list(const QJsonArray& arr, std::list<T >& lst)
    {
        jsonarray2list(arr, lst, std::is_integral<T>());
    };


    template<class T> 
    void jsonarray2list_of_struct_list(QJsonArray arr, std::list <std::list<T>>& lst)
    {
        int Max = arr.size();
        for (int i = 0; i < Max; ++i){
            QJsonArray arr2 = arr[i].toArray();
            std::list<T> lst2;
            jsonarray2struct_list(arr2, lst2);
            lst.push_back(lst2);
        }
    };

    class VoidType
    {
    public:
        static std::unique_ptr<VoidType> create(const QByteArray& )
        {
            return std::unique_ptr<VoidType>();
        };

        static VoidType& instance();
        operator QJsonObject ()const;

        void build(const QString& link, QUrl& url)const 
        {
            url.setUrl(link);
        }
    };

    class NotAnException
    {
    public:
        static void raise(const QByteArray&, int, const std::string& ){};
    };

    template <class T, class D>
    class ListFromJsonLoader
    {
    public:
        static T create(const QByteArray& data)
        {
            T res;
            QJsonDocument doc = QJsonDocument::fromJson(data);
            QJsonArray arr = doc.array();
            foreach (const QJsonValue & val, arr)
                {
                    QJsonObject js = val.toObject();
                    D o;
                    o.fromJson(js);
                    res.push_back(o);
                }
            return std::move(res);
        }        
    };

    class UrlBuilder;
    class QParamArg
    {
    public:
        virtual void build(const QString& link_path, QUrl& url)const = 0;
        void setFields(QString fields){ m_Fields = fields;};
        void clearFields(){ m_Fields = "";};
    protected:
        void ResponseFields2Builder(UrlBuilder& b)const;
    protected:
        QString m_Fields;
    };

    template <class P, class D>
    class PathArg : public P
    {
    public:
        virtual void build(const QString& link_path, QUrl& url)const 
        {
            url.setUrl(link_path + QString("/%1").arg(P::path()));
        }

#ifdef API_QT_AUTOTEST
        static std::unique_ptr<D> EXAMPLE(){std::unique_ptr<D> rv(new D);return rv;};
#endif //API_QT_AUTOTEST        
    };

    template <class P, class D>
    class PathWithIdArg : public P
    {
    public:
        PathWithIdArg() {}
        PathWithIdArg(QString idValue) :m_id(idValue) {}
        virtual void build(const QString& link_path, QUrl& url)const
        {
            url.setUrl(link_path + QString("/%1/%2").arg(m_id).arg(P::path()));
        }

#ifdef API_QT_AUTOTEST
        static std::unique_ptr<D> EXAMPLE() { std::unique_ptr<D> rv(new D("100")); return rv; };
#endif //API_QT_AUTOTEST
    protected:
        QString m_id;
    };

    class UrlBuilder 
    {
    public:
        UrlBuilder(const QString& link_path, QUrl& url);
        virtual ~UrlBuilder();

        UrlBuilder& add(QString name, QString value);
        UrlBuilder& add(QString name, const char* value);
        UrlBuilder& add(QString name, bool value);
        UrlBuilder& add(QString name, int value);
        UrlBuilder& add(QString name, const QDateTime& value);

    protected:
        QUrlQuery m_q;
        QUrl&     m_url;
    };


#define DECLARE_PATH_CLASS(P) struct path_##P{QString path()const{return #P;}}
#define EXPECT(E, M) if(!E)qWarning() << M;

    //........
    /**
       GOOGLE_BLOCKING_CALL or GBC - macross converts 2 async callbacks into one blocking call
       that returns value of the first callback and throws exception in case of second callback.
    */

#define GOOGLE_BLOCKING_CALL(AFUNC, REST, ARGV)     \
    std::unique_ptr<GoogleException> ex;            \
    std::unique_ptr<REST> result;                   \
    AFUNC(ARGV,                                     \
          [this, &result](std::unique_ptr<REST> r)  \
          {                                         \
              result = std::move(r);                \
              m_end_point->exitEventsLoop();        \
          },                                        \
          [&](std::unique_ptr<GoogleException> e)   \
          {                                         \
              ex = std::move(e);                    \
              m_end_point->exitEventsLoop();        \
          }                                         \
          );                                        \
    if(!ex && !result)                              \
        m_end_point->runEventsLoop();               \
    if (ex)                                         \
        ex->raise();                                \
    return result;                                  \

#define BODY_ARG_GBC(AFUNC, REST, ARGV, BODY_VAL)   \
    std::unique_ptr<GoogleException> ex;            \
    std::unique_ptr<REST> result;                   \
    AFUNC(ARGV, BODY_VAL,                           \
          [this, &result](std::unique_ptr<REST> r)  \
          {                                         \
              result = std::move(r);                \
              m_end_point->exitEventsLoop();        \
          },                                        \
          [&](std::unique_ptr<GoogleException> e)   \
          {                                         \
              ex = std::move(e);                    \
              m_end_point->exitEventsLoop();        \
          }                                         \
          );                                        \
    if(!ex && !result)                              \
        m_end_point->runEventsLoop();               \
    if (ex)                                         \
        ex->raise();                                \
    return result;                                  \

#define BODY_NO_ARG_ARG_GBC(AFUNC, REST, BODY_VAL)  \
    std::unique_ptr<GoogleException> ex;            \
    std::unique_ptr<REST> result;                   \
    AFUNC(BODY_VAL,                                 \
        [this, &result](std::unique_ptr<REST> r)    \
    {                                               \
    result = std::move(r);                          \
    m_end_point->exitEventsLoop();                  \
},                                                  \
        [&](std::unique_ptr<GoogleException> e)     \
    {                                               \
    ex = std::move(e);                              \
    m_end_point->exitEventsLoop();                  \
}                                                   \
        );                                          \
    if(!ex && !result)                              \
        m_end_point->runEventsLoop();               \
    if (ex)                                         \
        ex->raise();                                \
    return result;                                  \


#define DATA_GBC(AFUNC, REST, ARGV, DATA)           \
    std::unique_ptr<GoogleException> ex;            \
    std::unique_ptr<REST> result;                   \
    AFUNC(ARGV,                                     \
          DATA,                                     \
          [this, &result](std::unique_ptr<REST> r)  \
          {                                         \
              result = std::move(r);                \
              m_end_point->exitEventsLoop();        \
          },                                        \
          [&](std::unique_ptr<GoogleException> e)   \
          {                                         \
              ex = std::move(e);                    \
              m_end_point->exitEventsLoop();        \
          }                                         \
          );                                        \
    if(!ex && !result)                              \
        m_end_point->runEventsLoop();               \
    if (ex)                                         \
        ex->raise();                                \
    return result;                                  \


#define VOID_RESULT_ARG_WITH_DATA_GBC(AFUNC, ARGV, DATA)    \
    std::unique_ptr<GoogleException> ex;                    \
    bool completed = false;                                 \
    AFUNC(ARGV,                                             \
        DATA,                                               \
        [this, &completed](void)                            \
    {                                                       \
    completed = true;;                                      \
    m_end_point->exitEventsLoop();                          \
},                                                          \
        [&](std::unique_ptr<GoogleException> e)             \
    {                                                       \
    ex = std::move(e);                                      \
    m_end_point->exitEventsLoop();                          \
}                                                           \
        );                                                  \
    if(!ex && !completed)                                   \
        m_end_point->runEventsLoop();                       \
    if (ex)                                                 \
        ex->raise();                                        \


#define VOID_ARG_GBC(AFUNC, REST)                   \
    std::unique_ptr<GoogleException> ex;            \
    std::unique_ptr<REST> result;                   \
    AFUNC([this, &result](std::unique_ptr<REST> r)  \
    {                                               \
    result = std::move(r);                          \
    m_end_point->exitEventsLoop();                  \
},                                                  \
        [&](std::unique_ptr<GoogleException> e)     \
    {                                               \
    ex = std::move(e);                              \
    m_end_point->exitEventsLoop();                  \
}                                                   \
        );                                          \
    if(!ex && !result)                              \
        m_end_point->runEventsLoop();               \
    if (ex)                                         \
        ex->raise();                                \
    return result;                                  \


#define VOID_RESULT_GBC(AFUNC, ARGV)                \
    std::unique_ptr<GoogleException> ex;            \
    bool completed = false;                         \
    AFUNC(ARGV,                                     \
          [this, &completed](void)                  \
          {                                         \
              completed = true;                     \
              m_end_point->exitEventsLoop();        \
          },                                        \
          [&](std::unique_ptr<GoogleException> e)   \
          {                                         \
              ex = std::move(e);                    \
              m_end_point->exitEventsLoop();        \
          }                                         \
          );                                        \
          if(!ex && !completed)                     \
              m_end_point->runEventsLoop();         \
          if (ex)                                   \
              ex->raise();                          \

}


#define DECL_STD_BOUND_TASK_CB(ENDP_FUNC)                               \
    template <class RES,                                                \
              class RESULT_FACTORY,                                     \
              class BODY>                                               \
    void ENDP_FUNC(QUrl url,                                            \
        const BODY& body,                                               \
        GoogleTask<RES>* t)                                             \
    {                                                                   \
    std::function<void(std::unique_ptr<RES>)> completed_CB =            \
        [=](std::unique_ptr<RES> r)                                     \
    {                                                                   \
    t->completed_callback(std::move(r));                                \
};                                                                      \
                                                                        \
    std::function<void(std::unique_ptr<GoogleException>)> failed_CB =   \
        [=](std::unique_ptr<GoogleException> ex)                        \
    {                                                                   \
    t->failed_callback(std::move(ex));                                  \
};                                                                      \
                                                                        \
    ENDP_FUNC<RES, RESULT_FACTORY, BODY>                                \
    (url, body, completed_CB, failed_CB);                               \
}                                                                       \


#define DECL_BODYLESS_BOUND_TASK_CB(ENDP_FUNC)                          \
    template <class RES,                                                \
              class RESULT_FACTORY>                                     \
    void ENDP_FUNC(QUrl url,                                            \
                   GoogleTask<RES>* t)                                  \
    {                                                                   \
        std::function<void(std::unique_ptr<RES>)> completed_CB =        \
            [=](std::unique_ptr<RES> r)                                 \
            {                                                           \
                t->completed_callback(std::move(r));                    \
            };                                                          \
                                                                        \
        std::function<void(std::unique_ptr<GoogleException>)> failed_CB = \
            [=](std::unique_ptr<GoogleException> ex)                    \
            {                                                           \
                t->failed_callback(std::move(ex));                      \
            };                                                          \
                                                                        \
        ENDP_FUNC<RES, RESULT_FACTORY>                                  \
            (url, completed_CB, failed_CB);                             \
    }                                                                   \

#define DECL_VOID_BOUND_TASK_CB(ENDP_FUNC)                              \
    void ENDP_FUNC(QUrl url,                                            \
                   GoogleVoidTask* t)                                   \
    {                                                                   \
        std::function<void(void)> completed_CB =                        \
            [=](void)                                                   \
            {                                                           \
                t->completed_callback();                                \
            };                                                          \
                                                                        \
        std::function<void(std::unique_ptr<GoogleException>)> failed_CB = \
            [=](std::unique_ptr<GoogleException> ex)                    \
            {                                                           \
                t->failed_callback(std::move(ex));                      \
            };                                                          \
                                                                        \
        ENDP_FUNC(url, completed_CB, failed_CB);                        \
    }                                                                   \


