/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "revisions"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gdrive/revisions/RevisionsRoutes.h"
#include "Endpoint.h"
#include "gdrive/GdriveRoutes.h"
using namespace googleQt;
using namespace revisions;

void RevisionsRoutes::deleteOperation(const gdrive::DeleteRevisionArg& arg ){
    deleteOperation_Async(arg)->waitForResultAndRelease();
}

GoogleVoidTask* RevisionsRoutes::deleteOperation_Async(const gdrive::DeleteRevisionArg& arg)
{
    GoogleVoidTask* t = m_end_point->produceVoidTask();
    m_end_point->deleteStyle
        (m_end_point->buildGdriveUrl("revisions", arg),
        t);
    return t;
}

void RevisionsRoutes::deleteOperation_AsyncCB(
    const gdrive::DeleteRevisionArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->deleteStyle
        (m_end_point->buildGdriveUrl("revisions", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<RevisionResource> RevisionsRoutes::get(const gdrive::GetRevisionArg& arg){
    return get_Async(arg)->waitForResultAndRelease();
}

GoogleTask<RevisionResource>* RevisionsRoutes::get_Async(const gdrive::GetRevisionArg& arg)
{
    GoogleTask<RevisionResource>* t = m_end_point->produceTask<RevisionResource>();
    m_end_point->getStyle<
        RevisionResource,
        RevisionResource::factory
        >
        (m_end_point->buildGdriveUrl("revisions", arg),
        t);
    return t;
}

void RevisionsRoutes::get_AsyncCB(
    const gdrive::GetRevisionArg& arg,
    std::function<void(std::unique_ptr<RevisionResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        RevisionResource,
        RevisionResource::factory
        >
        (m_end_point->buildGdriveUrl("revisions", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<RevisionResourceCollection> RevisionsRoutes::list(const gdrive::ListRevisionArg& arg){
    return list_Async(arg)->waitForResultAndRelease();
}

GoogleTask<RevisionResourceCollection>* RevisionsRoutes::list_Async(const gdrive::ListRevisionArg& arg)
{
    GoogleTask<RevisionResourceCollection>* t = m_end_point->produceTask<RevisionResourceCollection>();
    m_end_point->getStyle<
        RevisionResourceCollection,
        RevisionResourceCollection::factory
        >
        (m_end_point->buildGdriveUrl("revisions", arg),
        t);
    return t;
}

void RevisionsRoutes::list_AsyncCB(
    const gdrive::ListRevisionArg& arg,
    std::function<void(std::unique_ptr<RevisionResourceCollection>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        RevisionResourceCollection,
        RevisionResourceCollection::factory
        >
        (m_end_point->buildGdriveUrl("revisions", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<RevisionResource> RevisionsRoutes::update(const gdrive::UpdateRevisionArg& arg){
    return update_Async(arg)->waitForResultAndRelease();
}

GoogleTask<RevisionResource>* RevisionsRoutes::update_Async(const gdrive::UpdateRevisionArg& arg)
{
    GoogleTask<RevisionResource>* t = m_end_point->produceTask<RevisionResource>();
    m_end_point->updateStyle<
        RevisionResource,
        RevisionResource::factory
        ,gdrive::UpdateRevisionArg>
        (m_end_point->buildGdriveUrl("revisions", arg),
        arg,
        t);
    return t;
}

void RevisionsRoutes::update_AsyncCB(
    const gdrive::UpdateRevisionArg& arg,
    std::function<void(std::unique_ptr<RevisionResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->updateStyle
        <
        RevisionResource,
        RevisionResource::factory
        , gdrive::UpdateRevisionArg
        >
        (m_end_point->buildGdriveUrl("revisions", arg),
        arg,
        completed_callback,
        failed_callback);
}

