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
    BODY_NO_ARG_ARG_GBC(create_Async, LabelResource, body);
}

void LabelsRoutes::create_Async(
    const LabelResource& body,
    std::function<void(std::unique_ptr<LabelResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        <
        std::unique_ptr<LabelResource>,
        LabelResource::factory,
        LabelResource
        >
        (m_end_point->buildGmailUrl("labels", VoidType::instance()),
        body,
        completed_callback,
        failed_callback);
}

void LabelsRoutes::deleteOperation(const gmail::IdArg& arg ){
    VOID_RESULT_GBC(deleteOperation_Async, arg);
}

void LabelsRoutes::deleteOperation_Async(
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
    GOOGLE_BLOCKING_CALL(get_Async, LabelResource, arg);
}

void LabelsRoutes::get_Async(
    const gmail::IdArg& arg,
    std::function<void(std::unique_ptr<LabelResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<LabelResource>,
        LabelResource::factory
        >
        (m_end_point->buildGmailUrl("labels", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<LabelsResultList> LabelsRoutes::list(void){
    VOID_ARG_GBC(list_Async, LabelsResultList);
}

void LabelsRoutes::list_Async(
    std::function<void(std::unique_ptr<LabelsResultList>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<LabelsResultList>,
        LabelsResultList::factory
        >
        (m_end_point->buildGmailUrl("labels", VoidType::instance()),
        completed_callback,
        failed_callback);
}

std::unique_ptr<LabelResource> LabelsRoutes::update(const gmail::IdArg& arg, const LabelResource& body){
    BODY_ARG_GBC(update_Async, LabelResource, arg, body);
}

void LabelsRoutes::update_Async(
    const gmail::IdArg& arg,
    const LabelResource& body,
    std::function<void(std::unique_ptr<LabelResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->putStyle
        <
        std::unique_ptr<LabelResource>,
        LabelResource::factory,
        LabelResource
        >
        (m_end_point->buildGmailUrl("labels", arg),
        body,
        completed_callback,
        failed_callback);
}

