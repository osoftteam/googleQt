/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "permissions"
 www.prokarpaty.net
***********************************************************/

#include "gdrive/permissions/PermissionsRoutes.h"
#include "Endpoint.h"
#include "gdrive/GdriveRoutes.h"
using namespace googleQt;
using namespace permissions;

std::unique_ptr<ResourcePermission> PermissionsRoutes::create(const gdrive::CreatePermissionArg& arg, const ResourcePermission& body){
    BODY_ARG_GBC(create_Async, ResourcePermission, arg, body);
}

void PermissionsRoutes::create_Async(
    const gdrive::CreatePermissionArg& arg,
    const ResourcePermission& body,
    std::function<void(std::unique_ptr<ResourcePermission>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        <
        std::unique_ptr<ResourcePermission>,
        ResourcePermission::factory,
        ResourcePermission
        >
        (m_end_point->buildGdriveUrl("permissions", arg),
        body,
        completed_callback,
        failed_callback);
}

void PermissionsRoutes::deleteOperation(const gdrive::PermissionArg& arg ){
    VOID_RESULT_GBC(deleteOperation_Async, arg);
}

void PermissionsRoutes::deleteOperation_Async(
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
    GOOGLE_BLOCKING_CALL(get_Async, ResourcePermission, arg);
}

void PermissionsRoutes::get_Async(
    const gdrive::PermissionArg& arg,
    std::function<void(std::unique_ptr<ResourcePermission>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<ResourcePermission>,
        ResourcePermission::factory
        >
        (m_end_point->buildGdriveUrl("permissions", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<PermissionResourcesCollection> PermissionsRoutes::list(const gdrive::PermissionListArg& arg){
    GOOGLE_BLOCKING_CALL(list_Async, PermissionResourcesCollection, arg);
}

void PermissionsRoutes::list_Async(
    const gdrive::PermissionListArg& arg,
    std::function<void(std::unique_ptr<PermissionResourcesCollection>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<PermissionResourcesCollection>,
        PermissionResourcesCollection::factory
        >
        (m_end_point->buildGdriveUrl("permissions", arg),
        completed_callback,
        failed_callback);
}

