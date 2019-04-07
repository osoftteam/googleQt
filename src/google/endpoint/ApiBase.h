#pragma once

#include <list>
#include <set>
#include <memory>
#include <functional>
#include <cctype>
#include <QString>
#include <QDateTime>
#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QNetworkReply>
#include <QStringList>
#include <QUrlQuery>
#include <QDomDocument>
#include <QDomNodeList>
#include <QDebug>

#include "ApiException.h"
#include "GoogleTask.h"


namespace googleQt {

    bool loadJsonFromFile(QString path, QJsonObject& js);
    bool storeJsonToFile(QString path, const QJsonObject js);
    bool isConnectedToNetwork();

    template<class T>
    QJsonArray struct_list2jsonarray(const std::list<T>& lst)
    {
        QJsonArray rv;
        for (typename std::list<T>::const_iterator i = lst.cbegin(); i != lst.end(); i++) {
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
        for (typename std::list<std::unique_ptr<T>>::const_iterator i = lst.cbegin(); i != lst.end(); i++) {
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
        for (typename std::list <std::list<T> >::const_iterator i = lst.cbegin(); i != lst.end(); i++) {
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
        for (typename std::list<T>::const_iterator i = lst.cbegin(); i != lst.end(); i++) {
            const T o = *i;
            rv.append(o);
        }
        return rv;
    };

    template<class T>
    void jsonarray2struct_list(QJsonArray ar, std::list<T>& lst)
    {
        int Max = ar.size();
        for (int i = 0; i < Max; ++i) {
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
        for (int i = 0; i < Max; ++i) {
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
        for (int i = 0; i < Max; ++i) {
            int v = arr[i].toInt();
            lst.push_back(v);
        }
#else
        int Max = arr.size();
        for (int i = 0; i < Max; ++i) {
            QString tmp = arr[i].toString();
            lst.push_back(tmp.toInt());
        }

#endif
    }

    template<class T>
    void jsonarray2list(const QJsonArray& arr, std::list<T>& lst, std::false_type)
    {
        int Max = arr.size();
        for (int i = 0; i < Max; ++i) {
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
        for (int i = 0; i < Max; ++i) {
            QJsonArray arr2 = arr[i].toArray();
            std::list<T> lst2;
            jsonarray2struct_list(arr2, lst2);
            lst.push_back(lst2);
        }
    };

    template<class T>
    bool chunk_list_execution(const std::list<T>& inputList,
        std::function<bool(const std::list<T>&)> chunk_processor,
        size_t chunk_size = 40)
    {
        typedef typename std::list<T>::const_iterator ITR;
        std::list<T> subList;
        for (ITR i = inputList.cbegin(); i != inputList.cend(); i++)
        {
            T o = *i;
            subList.push_back(o);
            if (subList.size() == chunk_size)
            {
                if (!chunk_processor(subList))
                    return false;
                subList.clear();
            }
        }
        if (!subList.empty())
        {
            if (!chunk_processor(subList))
                return false;
        }
        return true;
    }

    /// converts std::list of strings -> comma separated list
    QString slist2commalist(const std::list<QString>& lst);
    QString slist2commalist_decorated(const std::list<QString>& lst, char deco = '\'');
    /// converts space separated strings -> std::list of strings
    std::list<QString> split_string(QString s);
    /// converts size to string with KB, MB or GB suffix
    QString size_human(qreal num);
    /// trims string to alpha-num characters, makes is lowercase for typeLabels in GContacts
    QString trim_alpha_label(QString lbl);

    class VoidType
    {
    public:
        static std::unique_ptr<VoidType> create(const QByteArray&)
        {
            return std::unique_ptr<VoidType>();
        };

        static VoidType& instance();
        operator QJsonObject ()const;

        void build(const QString& link, QUrl& url)const
        {
            url.setUrl(link);
        }

        QString userId()const { return ""; }

#ifdef API_QT_AUTOTEST
        static std::unique_ptr<VoidType> EXAMPLE(int, int) { return std::unique_ptr<VoidType>(new VoidType()); };
#endif //API_QT_AUTOTEST

    };

    class NotAnException
    {
    public:
        static void raise(const QByteArray&, int, const std::string&) {};
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
            foreach(const QJsonValue & val, arr)
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
        void setFields(QString fields) { m_Fields = fields; };
        void clearFields() { m_Fields = ""; };
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
        static std::unique_ptr<D> EXAMPLE(int, int) { std::unique_ptr<D> rv(new D); return rv; };
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
		UrlBuilder& add(QString name, uint64_t value);
    protected:
        QUrlQuery m_q;
        QUrl&     m_url;
    };

    template <class T>
    class JsonFactory
    {
    public:
        std::unique_ptr<T>  create(const QByteArray& data)
        {
            QJsonDocument doc = QJsonDocument::fromJson(data);
            QJsonObject js = doc.object();
            return create(js);
        }


        std::unique_ptr<T>  create(const QJsonObject& js)
        {
            std::unique_ptr<T> rv(new T);
            rv->fromJson(js);
            return rv;
        }
    };

    template<class T>
    class QParamArgWithBody : public QParamArg
    {
    public:
        class factory : public JsonFactory<T>
        {

        };
        virtual void toJson(QJsonObject& js)const = 0;
        operator QJsonObject()const {
            QJsonObject js;
            toJson(js);
            return js;
        };
    };

    struct CaseInsensitiveLess : std::binary_function<std::string, std::string, bool>
    {
        bool operator() (const QString & lhs, const QString & rhs) const {
            return std::lexicographical_compare(lhs.begin(),
                lhs.end(),
                rhs.begin(),
                rhs.end(),
                case_insensitive_char_cmp);
        }

        static bool case_insensitive_char_cmp(const QChar& lhs, const QChar& rhs)
        {
            return (lhs.toUpper() < rhs.toUpper());
        }
    };


    enum class EBatchId
    {
        none,
        retrieve,
        create,
        update,
        delete_operation
    };

    namespace xml_util
    {
        ///create or edit node
        void    updateNode(QDomDocument& doc, QDomNode& parent_node, QString child_name, QString child_value);
        ///add new node
        QDomElement addNode(QDomDocument& doc, QDomNode& parent_node, QString name);
        ///find node if none - create a new one
        QDomElement ensureNode(QDomDocument& doc, QDomNode& parent_node, QString name);
        ///remve all child nodes with the name in parent
        void removeNodes(QDomNode& parent_node, QString name);
        ///add text to existing node
        QDomText addText(QDomDocument& doc, QDomElement parent_elem, QString text);
    };

#define DECLARE_PATH_CLASS(P) struct path_##P{QString path()const{return #P;}}
#define EXPECT(E, M) if(!E)qWarning() << M;

}

#define DECL_STD_BOUND_TASK_CB(ENDP_FUNC)                               \
    template <class RES,                                                \
              class RESULT_FACTORY,                                     \
              class BODY>                                               \
    void ENDP_FUNC(QUrl url,                                            \
                   const BODY& body,                                    \
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
        ENDP_FUNC<RES, RESULT_FACTORY, BODY>                            \
            (url, body, completed_CB, failed_CB);                       \
    }                                                                   \


#define DECL_BODYLESS_BOUND_TASK_CB(ENDP_FUNC)                          \
    template <class RES,                                                \
              class RESULT_FACTORY>                                     \
    void ENDP_FUNC(QUrl url,                                            \
                       GoogleTask<RES>* t)                              \
    {                                                                   \
     std::function<void(std::unique_ptr<RES>)> completed_CB =           \
         [=](std::unique_ptr<RES> r)                                    \
     {                                                                  \
      t->completed_callback(std::move(r));                              \
      };                                                                \
                                                                        \
     std::function<void(std::unique_ptr<GoogleException>)> failed_CB =  \
         [=](std::unique_ptr<GoogleException> ex)                       \
     {                                                                  \
      t->failed_callback(std::move(ex));                                \
      };                                                                \
                                                                        \
     ENDP_FUNC<RES, RESULT_FACTORY>                                     \
     (url, completed_CB, failed_CB);                                    \
     }                                                                  \

#define DECL_VOID_BOUND_TASK_CB(ENDP_FUNC)                              \
    void ENDP_FUNC(QUrl url,                                            \
                       GoogleVoidTask* t)                               \
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


#define DECL_BODY_VOID_RES_BOUND_TASK_CB(ENDP_FUNC)                      \
    template <class BODY>                                               \
    void ENDP_FUNC(QUrl url,                                            \
                   const BODY& body,                                    \
                   GoogleVoidTask* t)                                  \
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
        ENDP_FUNC<BODY>                                                 \
            (url, body, completed_CB, failed_CB);                       \
    }                                                                   \


