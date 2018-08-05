/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "permissions"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gdrive/permissions/PermissionsRoutes.h"
#include "Endpoint.h"
#include "gdrive/GdriveRoutes.h"
using namespace googleQt;
using namespace permissions;

std::unique_ptr<ResourcePermission> PermissionsRoutes::create(const gdrive::CreatePermissionArg& arg, const ResourcePermission& body){
    return create_Async(arg, body)->waitForResultAndRelease();
}

GoogleTask<ResourcePermission>* PermissionsRoutes::create_Async(const gdrive::CreatePermissionArg& arg, const ResourcePermission& body)
{
    GoogleTask<ResourcePermission>* t = m_end_point->produceTask<ResourcePermission>();
    m_end_point->postStyle<
        ResourcePermission,
        ResourcePermission::factory,
        ResourcePermission>
        (m_end_point->buildGdriveUrl("permissions", arg),
        body,
        t);
    return t;
}

void PermissionsRoutes::create_AsyncCB(
    const gdrive::CreatePermissionArg& arg,
    const ResourcePermission& body,
    std::function<void(std::unique_ptr<ResourcePermission>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        <
        ResourcePermission,
        ResourcePermission::factory,
        ResourcePermission
        >
        (m_end_point->buildGdriveUrl("permissions", arg),
        body,
        completed_callback,
        failed_callback);
}

void PermissionsRoutes::deleteOperation(const gdrive::PermissionArg& arg ){
    deleteOperation_Async(arg)->waitForResultAndRelease();
}

GoogleVoidTask* PermissionsRoutes::deleteOperation_Async(const gdrive::PermissionArg& arg)
{
    GoogleVoidTask* t = m_end_point->produceVoidTask();
    m_end_point->deleteStyle
        (m_end_point->buildGdriveUrl("permissions", arg),
        t);
    return t;
}

void PermissionsRoutes::deleteOperation_AsyncCB(
    const gdrive::PermissionArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->deleteStyle
        (m_end_point->buildGdriveUrl("permissions", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<ResourcePermission> PermissionsRoutes::get(const gdrive::PermissionArg& arg){
    return get_Async(arg)->waitForResultAndRelease();
}

GoogleTask<ResourcePermission>* PermissionsRoutes::get_Async(const gdrive::PermissionArg& arg)
{
    GoogleTask<ResourcePermission>* t = m_end_point->produceTask<ResourcePermission>();
    m_end_point->getStyle<
        ResourcePermission,
        ResourcePermission::factory
        >
        (m_end_point->buildGdriveUrl("permissions", arg),
        t);
    return t;
}

void PermissionsRoutes::get_AsyncCB(
    const gdrive::PermissionArg& arg,
    std::function<void(std::unique_ptr<ResourcePermission>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        ResourcePermission,
        ResourcePermission::factory
        >
        (m_end_point->buildGdriveUrl("permissions", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<PermissionResourcesCollection> PermissionsRoutes::list(const gdrive::PermissionListArg& arg){
    return list_Async(arg)->waitForResultAndRelease();
}

GoogleTask<PermissionResourcesCollection>* PermissionsRoutes::list_Async(const gdrive::PermissionListArg& arg)
{
    GoogleTask<PermissionResourcesCollection>* t = m_end_point->produceTask<PermissionResourcesCollection>();
    m_end_point->getStyle<
        PermissionResourcesCollection,
        PermissionResourcesCollection::factory
        >
        (m_end_point->buildGdriveUrl("permissions", arg),
        t);
    return t;
}

void PermissionsRoutes::list_AsyncCB(
    const gdrive::PermissionListArg& arg,
    std::function<void(std::unique_ptr<PermissionResourcesCollection>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        PermissionResourcesCollection,
        PermissionResourcesCollection::factory
        >
        (m_end_point->buildGdriveUrl("permissions", arg),
        completed_callback,
        failed_callback);
}

