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
            bool    registerMods()const{return (m_flags.register_mods == 1);}
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
                localCopy       = 1,    ///<-- regular local copy
                localModified   = 2,    ///<-- modified locally, need to sync
                localRemoved    = 3,    ///<-- removed locally, need to sync
                localRetired    = 4,    ///<-- retired/backuped local copy, can happen after delete
                localIdLimbo    = 5     ///<-- local copy that need cloudId from server, 
                                        ///--- objects with this state should be short lived during sync
                                        ///--- basically localIdLimbo means we got clouldId in memory and want to store it in DB
                                        ///--- the SQL-update in this case will be different from regular data update
            };

            ContactXmlPersistant();

            QString etag()const { return m_etag; }
            QString id()const { return m_id; }
            QString title()const { return m_title; }
            QString content()const { return m_content; }
            const QDateTime& updated()const { return m_updated; }            

            EStatus status()const { return m_status; }
            void markAsNormalCopy();
            void markAsModified();
            void markAsDeleted();
            void markAsRetired();
            void markAsIdLimbo();
            bool isModified()const { return (m_status == localModified); }
            bool isRemoved()const { return (m_status == localRemoved); }
            bool isRetired()const { return (m_status == localRetired); }
            bool isIdLimbo()const { return (m_status == localIdLimbo); }
            bool isCreatedLocally()const { return m_id.isEmpty(); }

			bool isRepoSyncCompleted()const;
			void setRepoSyncCompleted(bool val);

            QString  parsedXml()const { return m_parsed_xml; }

            bool parseXml(const QByteArray & data);
            bool parseXml(const QString & xml);
            QString mergedXml(QString mergeOrigin)const;
            virtual bool parseEntryNode(QDomNode n) = 0;
            virtual void mergeEntryNode(QDomDocument& doc, QDomNode& entry_node) const = 0;

            ///assign everything except dbid
            void assignContent(const ContactXmlPersistant& src);

            /// int -> status enum, does some validation
            static EStatus validatedStatus(int val, bool* ok = nullptr);            
            static QString status2string(EStatus status);

            static void printXmlParseError(QString contextMsg,
                                            const QByteArray & data,
                                            QString errorMsg,
                                            int errorLine,
                                            int errorColumn);
            static bool verifyXml(const QString & xml);
        protected:

            QString             m_etag, m_id, m_title, m_content;
            QDateTime           m_updated;
            QString             m_parsed_xml;
            EStatus             m_status;
			bool				m_repo_sync_completed{ false };
        };//ContactXmlPersistant

		struct ReplicaSyncResult 
		{
			int updated_local{ 0 };
			int updated_remote{ 0 };
		};

		struct ContactsReplicaSyncResult 
		{
			ReplicaSyncResult groups;
			ReplicaSyncResult contacts;
		};

        /**
            collection of 'parts' - emails, phones, addresses, etc.
        */
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

            const std::vector<P>& items()const{return m_parts;}
            std::vector<P>& items(){return m_parts;}            
            
        protected:
            std::vector<P> m_parts;
        };//PartList

          /**
            collection of labeled 'parts' - has cache to lookup part by label
            if anything changes rebuildLabelsMap should be called.
          */
        template <class T>
        class LabeledPartList : public PartList<T> 
        {
        public:
            using LABELS_MAP = std::map<QString, int>;

            const LABELS_MAP&   labelsMap()const { return m_labels_map; };
            void                rebuildLabelsMap()const 
            {
                m_idx_primary = -1;
                m_labels_map.clear();
                auto Max = PartList<T>::m_parts.size();
                for (size_t i = 0; i < Max; i++) {
                    const auto& p = PartList<T>::m_parts[i];
                    QString lbl = p.typeLabel();
                    if (!lbl.isEmpty()) {
                        m_labels_map[lbl] = i;
                    }
                    if (p.isPrimary()) {
                        m_idx_primary = static_cast<int>(i);
                    }
                }                
            }

            int findByLabel(QString lbl)const
            {
                auto i = m_labels_map.find(lbl);
                if (i == m_labels_map.end()) {
                    return -1;
                }

                auto idx = i->second;
                if (idx < 0 || idx >= static_cast<int>(PartList<T>::m_parts.size())) {
                    qWarning() << "Invalid labels cache index" << idx << PartList<T>::m_parts.size();
                    return -1;
                }

                ///might want to comment out following 4 line
                const T& p = PartList<T>::m_parts[idx];
                if (p.typeLabel().compare(lbl) != 0) {
                    qWarning() << "Invalid labels cache value" << idx << p.typeLabel() << lbl;
                    return -1;
                }
                return idx;
            };

            void removeByLabel(QString lbl) 
            {
                int idx = findByLabel(lbl);
                if (idx != -1) {
                    PartList<T>::m_parts.erase(PartList<T>::m_parts.begin() + idx);
                    rebuildLabelsMap();
                }
            }

            int findPrimary()const
            {
                if (m_idx_primary == -1) {
                    return -1;
                }
                if (m_idx_primary < 0 || m_idx_primary >= static_cast<int>(PartList<T>::m_parts.size())) {
                    qWarning() << "Invalid primary cache index" << m_idx_primary << PartList<T>::m_parts.size();
                    return -1;
                }

                ///might want to comment out following 4 line
                const T& p = PartList<T>::m_parts[m_idx_primary];
                if (!p.isPrimary()) {
                    qWarning() << "Invalid primary cache value" << m_idx_primary;
                    return -1;
                }

                return m_idx_primary;
            }

        protected:
            mutable LABELS_MAP  m_labels_map;
            mutable int m_idx_primary{ -1 };
        };

        template <class T>
        class InfoList
        {
        public:
            bool add(std::shared_ptr<T> c) {
                if(findById(c->id())){
                    qWarning() << "ERROR. Duplicate ID not allowed in infolist" << c->id();
                    return false;
                }
                
                m_items.push_back(c);
                if (!c->id().isEmpty()) {
                    m_id2item[c->id()] = c;
                }
                return true;
            }
            
            bool add(std::unique_ptr<T> c) {
                std::shared_ptr<T> c2(c.release());
                return add(c2);
            }

            void clear() { m_items.clear(); m_id2item.clear(); m_retired.clear();};
            
            void retire(std::shared_ptr<T> c) {
                auto it = m_id2item.find(c->id());
                if (it != m_id2item.end()) {
                    m_id2item.erase(it);
                }

                auto it2 = m_items.begin();
                while(it2 != m_items.end()){
                    if((*it2)->id() == c->id()){
                        m_items.erase(it2);
                        break;
                    }
                    it2++;
                }

                c->markAsRetired();
                m_retired.push_back(c);
            }

            void remove(std::shared_ptr<T> c) {
                c->markAsDeleted();
            }

            void idlimbo(std::shared_ptr<T> c){
                if (c->id().isEmpty()) {
                    qWarning() << "ERROR. Expected valid cloud ID";
                    return;                    
                }
                
                if(findById(c->id())){
                    qWarning() << "ERROR. Duplicate ID not allowed in infolist" << c->id();
                    return;
                }
                
                c->markAsIdLimbo();
                m_id2item[c->id()] = c;
            }
            
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
                            retire(c);
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
            std::vector<std::shared_ptr<T>>& retired() { return m_retired; }
            const std::shared_ptr<T> findById(QString sid) {
                std::shared_ptr<T> rv;
                auto it = m_id2item.find(sid);
                if (it != m_id2item.end()) {
                    rv = it->second;
                }
                return rv;
            }

			ReplicaSyncResult synchronize2ReplicaLists(InfoList<T>& localList, InfoList<T>& remoteList) {
				ReplicaSyncResult rv;

				for (auto g_local : localList.m_items) {
					if (g_local->id().isEmpty()) {
						qWarning() << "expected valid object ID to synchronize" << localList.items().size() << remoteList.items().size();
						continue;
					}
					auto g_remote = remoteList.findById(g_local->id());
					if (!g_remote) {
						if (!g_local->isRemoved() && !g_local->isRetired()) {
							///clone
							auto g2 = g_local->clone();
							remoteList.add(std::move(g2));
							g_local->setRepoSyncCompleted(true);
							g2->setRepoSyncCompleted(true);
							rv.updated_remote++;
						}
					}
					else {
						if (g_remote->isModified()) {
							g_local->assignContent(*g_remote);
							g_local->setRepoSyncCompleted(true);
							g_remote->setRepoSyncCompleted(true);
							rv.updated_local++;
						}
						else if (g_remote->isRemoved()) {
							if (!g_local->isModified()) {
								g_local->markAsDeleted();
								g_local->setRepoSyncCompleted(true);
								g_remote->setRepoSyncCompleted(true);
								rv.updated_local++;
							}
						}

						if (!g_local->isRepoSyncCompleted()) {
							if (g_local->isModified()) {
								g_remote->assignContent(*g_local);
								g_local->setRepoSyncCompleted(true);
								g_remote->setRepoSyncCompleted(true);
								rv.updated_remote++;
							}
							else if (g_local->isRemoved()) {
								if (!g_remote->isModified()) {
									g_remote->markAsDeleted();
									g_local->setRepoSyncCompleted(true);
									g_remote->setRepoSyncCompleted(true);
									rv.updated_remote++;
								}
							}
						}
					}
				}//for
				return rv;
			};

			ReplicaSyncResult synchronizeReplica(InfoList<T>& remoteList) {
				ReplicaSyncResult rv;

				for (auto& o : m_items) {
					o->setRepoSyncCompleted(false);
				}
				for (auto& o : remoteList.m_items) {
					o->setRepoSyncCompleted(false);
				}

				auto r1 = synchronize2ReplicaLists(*this, remoteList);
				auto r2 = synchronize2ReplicaLists(remoteList, *this);

				rv.updated_local = r1.updated_local + r2.updated_remote;
				rv.updated_remote = r1.updated_remote + r2.updated_local;

				return rv;
			}
        protected:
            std::vector<std::shared_ptr<T>> m_items;
            std::map<QString, std::shared_ptr<T>> m_id2item;
            std::vector<std::shared_ptr<T>> m_retired;
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
                            lst.add(o->buildBatchRequest(googleQt::EBatchId::create));
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
