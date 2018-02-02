/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "contact_group"
 www.prokarpaty.net
***********************************************************/

#include "gcontact/contact_group/ContactGroupRoutes.h"
#include "Endpoint.h"
#include "gcontact/GcontactRoutes.h"
using namespace googleQt;
using namespace contact_group;

std::unique_ptr<gcontact::ContactGroupListResult> ContactGroupRoutes::batch(const gcontact::BatchContactGroupArg& arg){
    return batch_Async(arg)->waitForResultAndRelease();
}

GoogleTask<gcontact::ContactGroupListResult>* ContactGroupRoutes::batch_Async(const gcontact::BatchContactGroupArg& arg)
{
    GoogleTask<gcontact::ContactGroupListResult>* t = m_end_point->produceTask<gcontact::ContactGroupListResult>();
    m_end_point->postContactStyleB<
        gcontact::ContactGroupListResult,
        gcontact::ContactGroupListResult::factory
        ,gcontact::BatchContactGroupArg>
        (m_end_point->buildContactGroupBatchUrl(arg),
        arg,
        t);
    return t;
}

void ContactGroupRoutes::batch_AsyncCB(
    const gcontact::BatchContactGroupArg& arg,
    std::function<void(std::unique_ptr<gcontact::ContactGroupListResult>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postContactStyleB
        <
        gcontact::ContactGroupListResult,
        gcontact::ContactGroupListResult::factory
        , gcontact::BatchContactGroupArg
        >
        (m_end_point->buildContactGroupBatchUrl(arg),
        arg,
        completed_callback,
        failed_callback);
}

std::unique_ptr<gcontact::ContactGroupListResult> ContactGroupRoutes::create(const gcontact::CreateContactGroupArg& arg){
    return create_Async(arg)->waitForResultAndRelease();
}

GoogleTask<gcontact::ContactGroupListResult>* ContactGroupRoutes::create_Async(const gcontact::CreateContactGroupArg& arg)
{
    GoogleTask<gcontact::ContactGroupListResult>* t = m_end_point->produceTask<gcontact::ContactGroupListResult>();
    m_end_point->postContactGroupStyleB<
        gcontact::ContactGroupListResult,
        gcontact::ContactGroupListResult::factory
        ,gcontact::CreateContactGroupArg>
        (m_end_point->buildContactGroupUrl(arg),
        arg,
        t);
    return t;
}

void ContactGroupRoutes::create_AsyncCB(
    const gcontact::CreateContactGroupArg& arg,
    std::function<void(std::unique_ptr<gcontact::ContactGroupListResult>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postContactGroupStyleB
        <
        gcontact::ContactGroupListResult,
        gcontact::ContactGroupListResult::factory
        , gcontact::CreateContactGroupArg
        >
        (m_end_point->buildContactGroupUrl(arg),
        arg,
        completed_callback,
        failed_callback);
}

void ContactGroupRoutes::deleteContactGroup(const gcontact::DeleteContactGroupArg& arg ){
    deleteContactGroup_Async(arg)->waitForResultAndRelease();
}

GoogleVoidTask* ContactGroupRoutes::deleteContactGroup_Async(const gcontact::DeleteContactGroupArg& arg)
{
    GoogleVoidTask* t = m_end_point->produceVoidTask();
    m_end_point->deleteContactGroupStyleB
        (m_end_point->buildContactGroupUrl(arg),
        arg,
        t);
    return t;
}

void ContactGroupRoutes::deleteContactGroup_AsyncCB(
    const gcontact::DeleteContactGroupArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->deleteContactGroupStyleB
        <
        gcontact::DeleteContactGroupArg
        >
        (m_end_point->buildContactGroupUrl(arg),
        arg,
        completed_callback,
        failed_callback);
}

std::unique_ptr<gcontact::ContactGroupListResult> ContactGroupRoutes::list(const gcontact::ContactGroupListArg& arg){
    return list_Async(arg)->waitForResultAndRelease();
}

GoogleTask<gcontact::ContactGroupListResult>* ContactGroupRoutes::list_Async(const gcontact::ContactGroupListArg& arg)
{
    GoogleTask<gcontact::ContactGroupListResult>* t = m_end_point->produceTask<gcontact::ContactGroupListResult>();
    m_end_point->getContactStyle<
        gcontact::ContactGroupListResult,
        gcontact::ContactGroupListResult::factory
        >
        (m_end_point->buildContactGroupUrl(arg),
        t);
    return t;
}

void ContactGroupRoutes::list_AsyncCB(
    const gcontact::ContactGroupListArg& arg,
    std::function<void(std::unique_ptr<gcontact::ContactGroupListResult>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getContactStyle
        <
        gcontact::ContactGroupListResult,
        gcontact::ContactGroupListResult::factory
        >
        (m_end_point->buildContactGroupUrl(arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<gcontact::ContactGroupListResult> ContactGroupRoutes::update(const gcontact::UpdateContactGroupArg& arg){
    return update_Async(arg)->waitForResultAndRelease();
}

GoogleTask<gcontact::ContactGroupListResult>* ContactGroupRoutes::update_Async(const gcontact::UpdateContactGroupArg& arg)
{
    GoogleTask<gcontact::ContactGroupListResult>* t = m_end_point->produceTask<gcontact::ContactGroupListResult>();
    m_end_point->putContactGroupStyleB<
        gcontact::ContactGroupListResult,
        gcontact::ContactGroupListResult::factory
        ,gcontact::UpdateContactGroupArg>
        (m_end_point->buildContactGroupUrl(arg),
        arg,
        t);
    return t;
}

void ContactGroupRoutes::update_AsyncCB(
    const gcontact::UpdateContactGroupArg& arg,
    std::function<void(std::unique_ptr<gcontact::ContactGroupListResult>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->putContactGroupStyleB
        <
        gcontact::ContactGroupListResult,
        gcontact::ContactGroupListResult::factory
        , gcontact::UpdateContactGroupArg
        >
        (m_end_point->buildContactGroupUrl(arg),
        arg,
        completed_callback,
        failed_callback);
}

