#pragma once
#include <QSqlQuery>
#include "google/endpoint/ApiUtil.h"

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
            DbPersistant();
            bool    isDbIdNull()const { return (m_db_id == -1); }
            int     dbID()const { return m_db_id; }
            void    setDbID(int v) { m_db_id = v; }
            /// modified in memory and need persistance
            bool    isDirty()const{return (m_flags.is_dirty == 1);}
            void    setDirty(bool val = true);
            void    setRegisterModifications(bool val = true);
        protected:
            int     m_db_id{ -1 };

            union Flags{
                unsigned char flags;
                struct{
                    unsigned is_dirty    : 1;
                    unsigned register_mods : 1;
                };
            } m_flags;
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

            EStatus status()const { return m_status; }
            void markAsNormalCopy() { m_status = localCopy;setDirty(true); };
            void markAsModified() { m_status = localModified;setDirty(true); };
            void markAsDeleted() { m_status = localRemoved;setDirty(true); };
            void markAsRetired() { m_status = localRetired;setDirty(true); };
            bool isModified()const { return (m_status == localModified); }
            bool isRemoved()const { return (m_status == localRemoved); }
            bool isRetired()const { return (m_status == localRetired); }
            bool isCreatedLocally()const { return m_id.isEmpty(); }

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

            /// userPtr - custom user data pointer
            void* userPtr()const { return m_user_ptr; }
            void  setUserPtr(void* p)const { m_user_ptr = p; }

            static void printXmlParseError(QString contextMsg,
                                            const QByteArray & data,
                                            QString errorMsg,
                                            int errorLine,
                                            int errorColumn);
            static bool verifyXml(const QString & xml);
        protected:

            QString             m_etag, m_id, m_title, m_content;
            QDateTime           m_updated;
            QString             m_original_xml_string;
            EStatus             m_status;
            mutable void*       m_user_ptr{ nullptr };            
        };//ContactXmlPersistant


        using MRecordDbPersistant = NullablePart;

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
                    ContactXmlPersistant::printXmlParseError("InfoList - Failed to parse contacts XML document",
                                                            data,
                                                            errorMsg,
                                                            errorLine,
                                                            errorColumn);
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
                            sc->markAsNormalCopy();
                            add(sc);
                        }
                        else {
                            c->assignContent(*(sc.get()));
                            c->markAsNormalCopy();
                        }
                    }
                }
            }

            const std::vector<std::shared_ptr<T>>& items()const { return m_items; }
            std::vector<std::shared_ptr<T>>& items() { return m_items; }
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

        template <class T, class B>
        class BatchBuilderInfoList: public InfoList<T>
        {
        public:
            B buildBatchRequestList() const
            {
                B lst;
                for (auto& o : InfoList<T>::m_items) {
                    if (!o->isRetired())
                    {
                        if (o->isCreatedLocally())
                        {
                            lst.add(o->buildBatchRequest(googleQt::EBatchId::update));
                        }
                        else {
                            if (o->isModified()) {
                                lst.add(o->buildBatchRequest(googleQt::EBatchId::update));
                            }
                            else if (o->isRemoved()) {
                                lst.add(o->buildBatchRequest(googleQt::EBatchId::delete_operation));
                            }
                        }
                    }
                }
                return lst;
            }
        };

        class BatchRequest
        {
        public:
            googleQt::EBatchId batchId()const { return m_batch_id; }
            void setBatchid(googleQt::EBatchId bid) { m_batch_id = bid; }

            static QString batch2xml(googleQt::EBatchId bid);
        protected:
            QString toBatchXmlEntryBegin()const;
        protected:
            googleQt::EBatchId  m_batch_id{ googleQt::EBatchId::none };
        };

        class BatchResult 
        {
        public:
            EBatchId batchResultId()const { return m_id; }
            QString  batchResultOperationType()const { return m_operation_type; }
            int      batchResultStatusCode()const { return m_status_code; }
            QString  batchResultStatusReason()const { return m_status_reason; }
            bool     parseBatchResult(QDomNode n);
            bool     succeded()const;
#ifdef API_QT_AUTOTEST
            static QString EXAMPLE(googleQt::EBatchId bid);
#endif
        protected:
            EBatchId m_id;
            QString  m_operation_type;
            int      m_status_code;
            QString  m_status_reason;
            bool     m_is_null{ false };
        };

    };//gcontact

};

#define COMPARE_NO_CASE(N) if (N.compare(o.N, Qt::CaseInsensitive)){return false;}
