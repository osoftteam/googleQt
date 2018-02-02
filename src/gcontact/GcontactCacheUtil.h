#pragma once
#include <QSqlQuery>
#include "google/endpoint/ApiUtil.h"

/**

DEPRECATED !

GOOGLE_QT_CONTACT_DB_STRUCT_AS_RECORD - lib will store structures as records in detail table, one row per structure
One table will be used to store all kind of record parts, mutliple groups supported as well.
Not used by default compilation, instead contact info is stored as two xml strings - original and modified
On server update modified xml is merged with original xml.
*/
// #define GOOGLE_QT_CONTACT_DB_STRUCT_AS_RECORD

namespace googleQt {
    namespace gcontact {
        /**
        basic xml parsing product, can be invalid
        */
        class NullablePart
        {
        public:
            NullablePart() {}
            virtual ~NullablePart() {}
            bool isNull()const { return m_is_null; }
            virtual bool isEmpty()const {return isNull(); }
        protected:
            bool    m_is_null{ false };
        };

        /**
        object can be stored as one record and dbID is primary key
        */
        class DbPersistant : public NullablePart
        {
        public:
            bool    isDbIdNull()const { return (m_db_id == -1); }
            int     dbID()const { return m_db_id; }
            void    setDbID(int v) { m_db_id = v; }
        protected:
            int     m_db_id{ -1 };
        };

        class ContactXmlPersistant : public DbPersistant
        {
        public:
            /// EStatus has magic numbers stored in DB, don't modify them
            enum EStatus
            {
                localCopy       = 1,
                localModified   = 2,
                localRemoved    = 3,
                localRetired    = 4
            };

            ContactXmlPersistant();

            QString etag()const { return m_etag; }
            QString id()const { return m_id; }
            QString title()const { return m_title; }
            QString content()const { return m_content; }
            const QDateTime& updated()const { return m_updated; }
            googleQt::EBatchId batchId()const { return m_batch_id; }

            EStatus status()const { return m_status; }
            void markAsClean() { m_status = localCopy; };
            void markAsModified() { m_status = localModified; };
            void markAsDeleted() { m_status = localRemoved; };
            void markAsRetired() { m_status = localRetired; };
            bool isModified()const { return (m_status == localModified); }
            void setBatchid(googleQt::EBatchId bid) { m_batch_id = bid; }

            QString  originalXml()const { return m_original_xml_string; }

            bool parseXml(const QByteArray & data);
            bool parseXml(const QString & xml);
            QString mergedXml(QString mergeOrigin)const;
            virtual bool parseEntryNode(QDomNode n) = 0;
            virtual void mergeEntryNode(QDomDocument& doc, QDomNode& entry_node) const = 0;

            ///assign everything except dbid
            void assignContent(const ContactXmlPersistant& src);

            /// int -> status enum, does some validation
            static EStatus validatedStatus(int val, bool* ok = nullptr);
            static QString batch2xml(googleQt::EBatchId bid);

        protected:
            QString             m_etag, m_id, m_title, m_content;
            QDateTime           m_updated;
            QString             m_original_xml_string;
            EStatus             m_status;

            googleQt::EBatchId  m_batch_id{ googleQt::EBatchId::none };
        };//ContactXmlPersistant


#ifdef GOOGLE_QT_CONTACT_DB_STRUCT_AS_RECORD
          /**
          object can be stored as series os records, each with it's own ID and label
          */
        class MRecordDbPersistant : public NullablePart
        {
        public:
            using   ID2NAME = std::map<int, QString>;
            using   NAME2ID = std::map<QString, int>;
            /// one obj can have multiple records, they all will have same kind id
            virtual int  objKind()const = 0;
            virtual bool insertDb(QSqlQuery* q, std::function<void(QSqlQuery*)> header_binder, int group_idx) = 0;
        protected:
            void clearDbMaps();
            bool insertDbRecord(QSqlQuery* q, std::function<void(QSqlQuery*)> header_binder, int group_idx, QString recordName, QString recordValue);
            bool insertDbRecord(QSqlQuery* q, std::function<void(QSqlQuery*)> header_binder, int group_idx, QString recordName, bool recordValue);
        protected:
            ID2NAME m_id2name;
            NAME2ID m_name2id;
        };

        enum ContactPartKind
        {
            pkindEmail = 1,
            pkindPhone = 2,
            pkindAddress = 3
        };
#else
        using MRecordDbPersistant = NullablePart;
#endif //GOOGLE_QT_CONTACT_DB_STRUCT_AS_RECORD


        template <class P>
        class PartList
        {
        public:
            QString toString()const
            {
                QString s = "";
                for (auto& p : m_parts) {
                    s += QString(";") + p.toString();
                }
                return s;
            };

            bool operator==(const PartList<P>& o) const
            {
                if (m_parts.size() != o.m_parts.size())
                    return false;

                size_t Max = m_parts.size();
                for (size_t i = 0; i < Max; i++) {
                    if (m_parts[i] != o.m_parts[i])
                        return false;
                }

                return true;
            };

            bool operator!=(const PartList<P>& o) const
            {
                return !(*this == o);
            };

#ifdef GOOGLE_QT_CONTACT_DB_STRUCT_AS_RECORD
            bool insertDb(QSqlQuery* q, std::function<void(QSqlQuery*)> header_binder)
            {
                int idx = 1;
                for (auto& p : m_parts) {
                    if (!p.insertDb(q, header_binder, idx)) {
                        return false;
                    }
                    idx++;
                }
                return true;
            }
#endif

            size_t size()const { return m_parts.size(); }
            const P& operator[](size_t idx)const { return m_parts[idx]; }

        protected:
            std::vector<P> m_parts;
        };//PartList


        template <class T>
        class InfoList
        {
        public:
            void add(std::shared_ptr<T> c) {
                m_items.push_back(c);
                if (!c->id().isEmpty()) {
                    m_id2item[c->id()] = c;
                }
            };

            void add(std::unique_ptr<T> c) {
                std::shared_ptr<T> c2(c.release());
                add(c2);
            };

            void clear() { m_items.clear(); m_id2item.clear(); };

            QString toString()const
            {
                QString rv;
                for (auto& c : items()) {
                    rv += c->toString();
                    rv += "\n";
                }
                return rv;
            };

            bool parseXml(const QByteArray & data)
            {
                QDomDocument doc;
                QString errorMsg = 0;
                int errorLine;
                int errorColumn;
                if (!doc.setContent(data, &errorMsg, &errorLine, &errorColumn)) {
                    qWarning() << "Failed to parse contacts XML document: " << errorMsg << "line=" << errorLine << "column=" << errorColumn;
                    qWarning() << "-- begin data";
                    qWarning() << data;
                    qWarning() << "-- end data";
                    return false;
                }

                QDomNodeList entries = doc.elementsByTagName("entry");
                for (int i = 0; i < entries.size(); i++) {
                    QDomNode n = entries.item(i);
                    std::shared_ptr<T> ci(new T);
                    if (ci->parseEntryNode(n)) {
                        add(ci);
                    }
                }

                return true;
            };

            QString toXmlFeed(QString userEmail)const
            {
                QString rv = "<?xml version='1.0' encoding='UTF-8'?>\n"
                    "<feed xmlns = 'http://www.w3.org/2005/Atom'\n"
                    "xmlns:gContact = 'http://schemas.google.com/contact/2008'\n"
                    "xmlns:gd = 'http://schemas.google.com/g/2005'\n"
                    "xmlns:batch = 'http://schemas.google.com/gdata/batch'>\n";

                for (auto& c : m_items) {
                    rv += c->toXml(userEmail);
                    rv += "\n";
                }
                rv += "</feed>";

                return rv;
            }


            bool operator==(const InfoList<T>& o) const
            {
                if (m_items.size() != o.m_items.size())
                    return false;

                if (m_id2item.size() != o.m_id2item.size())
                    return false;


                size_t Max = m_items.size();
                for (size_t i = 0; i < Max; i++) {
                    if (*(m_items[i]) != *(o.m_items[i]))
                        return false;
                }

                return true;
            };

            bool operator!=(const InfoList<T>& o) const
            {
                return !(*this == o);
            };

            QDateTime recalcUpdatedTime(const QDateTime& t1) {
                QDateTime rv = t1;

                if (!rv.isValid()) {
                    if (m_items.size() > 0) {
                        auto o = *(m_items.begin());
                        rv = o->updated();
                    }
                }

                for (auto& o : m_items) {
                    if (o->updated().isValid() && o->updated() > rv) {
                        rv = o->updated();
                    }
                }

                return rv;
            }

            void mergeList(InfoList<T>& source) {
                for (auto sc : source.items()) {
                    auto c = findById(sc->id());
                    if (!c) {
                        add(sc);
                    }
                    else {
                        if (c->isModified()) 
                        {
                            c->markAsRetired();
                            sc->markAsClean();
                            add(sc);
                        }
                        else {
                            c->assignContent(*(sc.get()));
                            c->markAsClean();
                        }
                    }
                }
            }

            const std::vector<std::shared_ptr<T>>& items()const { return m_items; }
            const std::shared_ptr<T> findById(QString sid) {
                std::shared_ptr<T> rv;
                auto it = m_id2item.find(sid);
                if (it != m_id2item.end()) {
                    rv = it->second;
                }
                return rv;
            }


        protected:
            std::vector<std::shared_ptr<T>> m_items;
            std::map<QString, std::shared_ptr<T>> m_id2item;
        };//InfoList



    };//gcontact

};
