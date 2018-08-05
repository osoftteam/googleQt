/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "labels"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gmail/labels/LabelsRoutes.h"
#include "Endpoint.h"
#include "gmail/GmailRoutes.h"
using namespace googleQt;
using namespace labels;

std::unique_ptr<LabelResource> LabelsRoutes::create(const LabelResource& body){
    return create_Async(body)->waitForResultAndRelease();
}

GoogleTask<LabelResource>* LabelsRoutes::create_Async(const LabelResource& body)
{
    GoogleTask<LabelResource>* t = m_end_point->produceTask<LabelResource>();
    m_end_point->postStyle<
        LabelResource,
        LabelResource::factory,
        LabelResource>
        (m_end_point->buildGmailUrl("labels", VoidType::instance()),
        body,
        t);
    return t;
}

void LabelsRoutes::create_AsyncCB(
    const LabelResource& body,
    std::function<void(std::unique_ptr<LabelResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        <
        LabelResource,
        LabelResource::factory,
        LabelResource
        >
        (m_end_point->buildGmailUrl("labels", VoidType::instance()),
        body,
        completed_callback,
        failed_callback);
}

void LabelsRoutes::deleteOperation(const gmail::IdArg& arg ){
    deleteOperation_Async(arg)->waitForResultAndRelease();
}

GoogleVoidTask* LabelsRoutes::deleteOperation_Async(const gmail::IdArg& arg)
{
    GoogleVoidTask* t = m_end_point->produceVoidTask();
    m_end_point->deleteStyle
        (m_end_point->buildGmailUrl("labels", arg),
        t);
    return t;
}

void LabelsRoutes::deleteOperation_AsyncCB(
    const gmail::IdArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->deleteStyle
        (m_end_point->buildGmailUrl("labels", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<LabelResource> LabelsRoutes::get(const gmail::IdArg& arg){
    return get_Async(arg)->waitForResultAndRelease();
}

GoogleTask<LabelResource>* LabelsRoutes::get_Async(const gmail::IdArg& arg)
{
    GoogleTask<LabelResource>* t = m_end_point->produceTask<LabelResource>();
    m_end_point->getStyle<
        LabelResource,
        LabelResource::factory
        >
        (m_end_point->buildGmailUrl("labels", arg),
        t);
    return t;
}

void LabelsRoutes::get_AsyncCB(
    const gmail::IdArg& arg,
    std::function<void(std::unique_ptr<LabelResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        LabelResource,
        LabelResource::factory
        >
        (m_end_point->buildGmailUrl("labels", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<LabelsResultList> LabelsRoutes::list(void){
    return list_Async()->waitForResultAndRelease();
}

GoogleTask<LabelsResultList>* LabelsRoutes::list_Async()
{
    GoogleTask<LabelsResultList>* t = m_end_point->produceTask<LabelsResultList>();
    m_end_point->getStyle<
        LabelsResultList,
        LabelsResultList::factory
        >
        (m_end_point->buildGmailUrl("labels", VoidType::instance()),
        t);
    return t;
}

void LabelsRoutes::list_AsyncCB(
    std::function<void(std::unique_ptr<LabelsResultList>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        LabelsResultList,
        LabelsResultList::factory
        >
        (m_end_point->buildGmailUrl("labels", VoidType::instance()),
        completed_callback,
        failed_callback);
}

std::unique_ptr<LabelResource> LabelsRoutes::update(const gmail::IdArg& arg, const LabelResource& body){
    return update_Async(arg, body)->waitForResultAndRelease();
}

GoogleTask<LabelResource>* LabelsRoutes::update_Async(const gmail::IdArg& arg, const LabelResource& body)
{
    GoogleTask<LabelResource>* t = m_end_point->produceTask<LabelResource>();
    m_end_point->putStyle<
        LabelResource,
        LabelResource::factory,
        LabelResource>
        (m_end_point->buildGmailUrl("labels", arg),
        body,
        t);
    return t;
}

void LabelsRoutes::update_AsyncCB(
    const gmail::IdArg& arg,
    const LabelResource& body,
    std::function<void(std::unique_ptr<LabelResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->putStyle
        <
        LabelResource,
        LabelResource::factory,
        LabelResource
        >
        (m_end_point->buildGmailUrl("labels", arg),
        body,
        completed_callback,
        failed_callback);
}

