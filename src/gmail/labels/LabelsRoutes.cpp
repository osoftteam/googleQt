/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "labels"
 www.prokarpaty.net
***********************************************************/

#include "gmail/labels/LabelsRoutes.h"
#include "Endpoint.h"
#include "gmail/GmailRoutes.h"
using namespace googleQt;
using namespace labels;

std::unique_ptr<LabelResource> LabelsRoutes::create(const LabelResource& body){
    BODY_NO_ARG_ARG_GBC(create_AsyncCB, LabelResource, body);
}

GoogleTask<LabelResource>* LabelsRoutes::create_Async(const LabelResource& body)
{
    GoogleTask<LabelResource>* t = new GoogleTask<LabelResource>();
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
    VOID_RESULT_GBC(deleteOperation_AsyncCB, arg);
}

GoogleVoidTask* LabelsRoutes::deleteOperation_Async(const gmail::IdArg& arg)
{
    GoogleVoidTask* t = new GoogleVoidTask();
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
    GOOGLE_BLOCKING_CALL(get_AsyncCB, LabelResource, arg);
}

GoogleTask<LabelResource>* LabelsRoutes::get_Async(const gmail::IdArg& arg)
{
    GoogleTask<LabelResource>* t = new GoogleTask<LabelResource>();
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
    VOID_ARG_GBC(list_AsyncCB, LabelsResultList);
}

GoogleTask<LabelsResultList>* LabelsRoutes::list_Async()
{
    GoogleTask<LabelsResultList>* t = new GoogleTask<LabelsResultList>();
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
    BODY_ARG_GBC(update_AsyncCB, LabelResource, arg, body);
}

GoogleTask<LabelResource>* LabelsRoutes::update_Async(const gmail::IdArg& arg, const LabelResource& body)
{
    GoogleTask<LabelResource>* t = new GoogleTask<LabelResource>();
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

