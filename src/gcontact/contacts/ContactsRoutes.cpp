/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "contacts"
 www.prokarpaty.net
***********************************************************/

#include "gcontact/contacts/ContactsRoutes.h"
#include "Endpoint.h"
#include "gcontact/GcontactRoutes.h"
using namespace googleQt;
using namespace contacts;

std::unique_ptr<ContactCollectionRes> ContactsRoutes::list(const gcontact::ContactsListArg& arg){
    return list_Async(arg)->waitForResultAndRelease();
}

GoogleTask<ContactCollectionRes>* ContactsRoutes::list_Async(const gcontact::ContactsListArg& arg)
{
    GoogleTask<ContactCollectionRes>* t = m_end_point->produceTask<ContactCollectionRes>();
    m_end_point->getContactStyle<
        ContactCollectionRes,
        ContactCollectionRes::factory
        >
        (m_end_point->buildContactUrl(arg),
        t);
    return t;
}

void ContactsRoutes::list_AsyncCB(
    const gcontact::ContactsListArg& arg,
    std::function<void(std::unique_ptr<ContactCollectionRes>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getContactStyle
        <
        ContactCollectionRes,
        ContactCollectionRes::factory
        >
        (m_end_point->buildContactUrl(arg),
        completed_callback,
        failed_callback);
}

