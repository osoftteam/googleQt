#pragma once
#include "google/endpoint/ApiUtil.h"

namespace googleQt {
	namespace gtask {
		class TaskListIdArg : public QParamArg
		{
		public:
			TaskListIdArg(QString tasklist_id = "");
			void build(const QString& link_path, QUrl& url)const override;

			/**
			* Task list identifier.
			*/
			QString getTasklist()const { return m_tasklist; }
			void    setTasklist(QString val) { m_tasklist = val; }

#ifdef API_QT_AUTOTEST
			static std::unique_ptr<TaskListIdArg> EXAMPLE();
#endif //API_QT_AUTOTEST

		protected:
			QString m_tasklist;
		};

		class TaskListContainerIdArg : public TaskListIdArg
		{
		public:
			TaskListContainerIdArg(QString tasklist_id = "");
			void build(const QString& link_path, QUrl& url)const override;

#ifdef API_QT_AUTOTEST
			static std::unique_ptr<TaskListContainerIdArg> EXAMPLE();
#endif //API_QT_AUTOTEST
		};


		class TaskListClearArg : public TaskListIdArg
		{
		public:
			TaskListClearArg(QString tasklist_id = "");
			void build(const QString& link_path, QUrl& url)const override;
			//QString arg()const override;
#ifdef API_QT_AUTOTEST
			static std::unique_ptr<TaskListClearArg> EXAMPLE();
#endif //API_QT_AUTOTEST
		};

		class TaskIdArg : public TaskListIdArg 
		{
		public:
			TaskIdArg();
			TaskIdArg(QString task_list_id, QString task_id);

			void build(const QString& link_path, QUrl& url)const override;

			/**
			* Task identifier.
			*/
			QString getTask()const { return m_task; }
			void    setTask(QString val) { m_task = val; }


#ifdef API_QT_AUTOTEST
			static std::unique_ptr<TaskIdArg> EXAMPLE();
#endif //API_QT_AUTOTEST

		protected:
			QString m_task;
		};
		
		class TaskInsertArg : public TaskListIdArg
		{
		public:
			TaskInsertArg();
			void build(const QString& link_path, QUrl& url)const override;

			/**
			* Parent task identifier. If the task is created at the top level, this parameter is omitted. Optional.
			*/
			QString getParent()const { return m_parent; }
			void    setParent(QString val) { m_parent = val; }

			/**
			* Previous sibling task identifier. If the task is 
			* created at the first position among its siblings, this parameter is omitted. Optional.
			*/
			QString getPrevious()const { return m_previous; }
			void    setPrevious(QString val) { m_previous = val; }


#ifdef API_QT_AUTOTEST
			static std::unique_ptr<TaskInsertArg> EXAMPLE();
#endif //API_QT_AUTOTEST

		protected:
			QString  m_parent;
			QString  m_previous;
		};

		class TaskMoveArg : public TaskIdArg
		{
		public:
			TaskMoveArg();
			TaskMoveArg(QString task_list_id, QString task_id);
			void build(const QString& link_path, QUrl& url)const override;

			/**
			* New parent task identifier. If the task is moved to the top level, this parameter is omitted. Optional.
			*/
			QString getParent()const { return m_parent; }
			void    setParent(QString val) { m_parent = val; }

			/**
			* New previous sibling task identifier. If the task is moved to the first position among its siblings, 
			* this parameter is omitted. Optional.
			*/
			QString getPrevious()const { return m_previous; }
			void    setPrevious(QString val) { m_previous = val; }


#ifdef API_QT_AUTOTEST
			static std::unique_ptr<TaskMoveArg> EXAMPLE();
#endif //API_QT_AUTOTEST

		protected:
			QString  m_parent;
			QString  m_previous;
		};
		

		class TaskListArg : public TaskListIdArg 
		{
		public:

			TaskListArg(QString tasklist = "");
			void build(const QString& link_path, QUrl& url)const override;

			/**
			* Upper bound for a task's completion date (as a RFC 3339 timestamp) to filter by. 
			* Optional. The default is not to filter by completion date.
			*/
			QString  getCompletedMax()const { return  m_completedMax; }
			void     setCompletedMax(QString val) { m_completedMax = val; }

			/**
			* Lower bound for a task's completion date (as a RFC 3339 timestamp) to filter by. 
			* Optional. The default is not to filter by completion date.
			*/
			QString  getCompletedMin()const { return m_completedMin; }
			void     setCompletedMin(QString val) { m_completedMin = val; }

			/**
			* Upper bound for a task's due date (as a RFC 3339 timestamp) to filter by. 
			* Optional. The default is not to filter by due date.
			*/
			QString  getDueMax()const { return m_dueMax; }
			void     setDueMax(QString val) { m_dueMax = val; }

			/**
			* Lower bound for a task's due date (as a RFC 3339 timestamp) to filter by. 
			* Optional. The default is not to filter by due date.
			*/
			QString  getDueMin()const { return m_dueMin; }
			void     setDueMin(QString val) { m_dueMin = val; }

			/**
			* Maximum number of task lists returned on one page. Optional. The default is 100.
			*/
			int      getMaxResults()const { return m_maxResults; }
			void     setMaxResults(int val) { m_maxResults = val; }

			/**
			* Token specifying the result page to return. Optional.
			*/
			QString  getPageToken()const { return m_pageToken; }
			void     setPageToken(QString val) { m_pageToken = val; }

			/**
			* Flag indicating whether deleted tasks are returned in the result. Optional. The default is False.
			*/
			bool     getShowDeleted()const { return m_showDeleted; }
			void     setShowDeleted(bool val) { m_showDeleted = val; }

			/**
			* Flag indicating whether hidden tasks are returned in the result. Optional. The default is False.
			*/
			bool     getShowHidden()const { return m_showHidden; }
			void     setShowHidden(bool val) { m_showHidden = val; }

			/**
			* Lower bound for a task's last modification time (as a RFC 3339 timestamp) to filter by. 
			* Optional. The default is not to filter by last modification time.
			*/
			const QDateTime&  getUpdatedMin()const { return m_updatedMin; }
			void     setUpdatedMin(const QDateTime& val) { m_updatedMin = val; }

#ifdef API_QT_AUTOTEST
			static std::unique_ptr<TaskListArg> EXAMPLE();
#endif //API_QT_AUTOTEST

		protected:
			QString  m_completedMax;
			QString  m_completedMin;
			QString  m_dueMax;
			QString  m_dueMin;
			int      m_maxResults;
			QString  m_pageToken;
			bool     m_showDeleted;
			bool     m_showHidden;
			QDateTime m_updatedMin;
		};


		class TasklistsListArg : public QParamArg
		{
		public:
			TasklistsListArg();
			TasklistsListArg(QString pageToken);

			/**
			*  Maximum number of task lists returned on one page. Optional. The default is 100.
			*/
			int           getMaxResults()const { return m_maxResults; };
			void          setMaxResults(int val) { m_maxResults = val; }

			/**
			* Token specifying the result page to return. Optional.
			*/
			QString       getPageToken()const { return m_pageToken; }
			void          setPageToken(QString v) { m_pageToken = v; }

			void build(const QString& link_path, QUrl& url)const override;

#ifdef API_QT_AUTOTEST
			static std::unique_ptr<TasklistsListArg> EXAMPLE();
#endif //API_QT_AUTOTEST

		protected:
			int         m_maxResults;
			QString     m_pageToken;
		};
	}//gtask
}//googleQt
