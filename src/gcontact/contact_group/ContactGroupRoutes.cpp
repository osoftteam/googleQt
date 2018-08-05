/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "contact_group"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gcontact/contact_group/ContactGroupRoutes.h"
#include "Endpoint.h"
#include "gcontact/GcontactRoutes.h"
using namespace googleQt;
using namespace contact_group;

std::unique_ptr<gcontact::BatchGroupList> ContactGroupRoutes::batch(const gcontact::BatchGroupArg& arg){
    return batch_Async(arg)->waitForResultAndRelease();
}

GoogleTask<gcontact::BatchGroupList>* ContactGroupRoutes::batch_Async(const gcontact::BatchGroupArg& arg)
{
    GoogleTask<gcontact::BatchGroupList>* t = m_end_point->produceTask<gcontact::BatchGroupList>();
    m_end_point->postContactStyleB<
        gcontact::BatchGroupList,
        gcontact::BatchGroupList::factory
        ,gcontact::BatchGroupArg>
        (m_end_point->buildContactGroupBatchUrl(arg),
        arg,
        t);
    return t;
}

void ContactGroupRoutes::batch_AsyncCB(
    const gcontact::BatchGroupArg& arg,
    std::function<void(std::unique_ptr<gcontact::BatchGroupList>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postContactStyleB
        <
        gcontact::BatchGroupList,
        gcontact::BatchGroupList::factory
        , gcontact::BatchGroupArg
        >
        (m_end_point->buildContactGroupBatchUrl(arg),
        arg,
        completed_callback,
        failed_callback);
}

std::unique_ptr<gcontact::GroupList> ContactGroupRoutes::create(const gcontact::CreateContactGroupArg& arg){
    return create_Async(arg)->waitForResultAndRelease();
}

GoogleTask<gcontact::GroupList>* ContactGroupRoutes::create_Async(const gcontact::CreateContactGroupArg& arg)
{
    GoogleTask<gcontact::GroupList>* t = m_end_point->produceTask<gcontact::GroupList>();
    m_end_point->postContactGroupStyleB<
        gcontact::GroupList,
        gcontact::GroupList::factory
        ,gcontact::CreateContactGroupArg>
        (m_end_point->buildContactGroupUrl(arg),
        arg,
        t);
    return t;
}

void ContactGroupRoutes::create_AsyncCB(
    const gcontact::CreateContactGroupArg& arg,
    std::function<void(std::unique_ptr<gcontact::GroupList>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postContactGroupStyleB
        <
        gcontact::GroupList,
        gcontact::GroupList::factory
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

std::unique_ptr<gcontact::GroupList> ContactGroupRoutes::list(const gcontact::ContactGroupListArg& arg){
    return list_Async(arg)->waitForResultAndRelease();
}

GoogleTask<gcontact::GroupList>* ContactGroupRoutes::list_Async(const gcontact::ContactGroupListArg& arg)
{
    GoogleTask<gcontact::GroupList>* t = m_end_point->produceTask<gcontact::GroupList>();
    m_end_point->getContactStyle<
        gcontact::GroupList,
        gcontact::GroupList::factory
        >
        (m_end_point->buildContactGroupUrl(arg),
        t);
    return t;
}

void ContactGroupRoutes::list_AsyncCB(
    const gcontact::ContactGroupListArg& arg,
    std::function<void(std::unique_ptr<gcontact::GroupList>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getContactStyle
        <
        gcontact::GroupList,
        gcontact::GroupList::factory
        >
        (m_end_point->buildContactGroupUrl(arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<gcontact::GroupList> ContactGroupRoutes::update(const gcontact::UpdateContactGroupArg& arg){
    return update_Async(arg)->waitForResultAndRelease();
}

GoogleTask<gcontact::GroupList>* ContactGroupRoutes::update_Async(const gcontact::UpdateContactGroupArg& arg)
{
    GoogleTask<gcontact::GroupList>* t = m_end_point->produceTask<gcontact::GroupList>();
    m_end_point->putContactGroupStyleB<
        gcontact::GroupList,
        gcontact::GroupList::factory
        ,gcontact::UpdateContactGroupArg>
        (m_end_point->buildContactGroupUrl(arg),
        arg,
        t);
    return t;
}

void ContactGroupRoutes::update_AsyncCB(
    const gcontact::UpdateContactGroupArg& arg,
    std::function<void(std::unique_ptr<gcontact::GroupList>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->putContactGroupStyleB
        <
        gcontact::GroupList,
        gcontact::GroupList::factory
        , gcontact::UpdateContactGroupArg
        >
        (m_end_point->buildContactGroupUrl(arg),
        arg,
        completed_callback,
        failed_callback);
}

