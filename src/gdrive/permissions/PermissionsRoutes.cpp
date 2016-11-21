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
    return m_end_point->postStyle<std::unique_ptr<ResourcePermission>, ResourcePermission::factory, ResourcePermission>(m_end_point->buildGdriveUrl("permissions", arg), body);
}

void PermissionsRoutes::deleteOperation(const gdrive::PermissionArg& arg ){
    m_end_point->deleteStyle<std::unique_ptr<VoidType>, VoidType, NotAnException>(m_end_point->buildGdriveUrl("permissions", arg) );
}

std::unique_ptr<ResourcePermission> PermissionsRoutes::get(const gdrive::PermissionArg& arg){
    return m_end_point->getStyle<std::unique_ptr<ResourcePermission>, ResourcePermission::factory, NotAnException>(m_end_point->buildGdriveUrl("permissions", arg));
}

std::unique_ptr<PermissionResourcesCollection> PermissionsRoutes::list(const gdrive::PermissionListArg& arg){
    return m_end_point->getStyle<std::unique_ptr<PermissionResourcesCollection>, PermissionResourcesCollection::factory, NotAnException>(m_end_point->buildGdriveUrl("permissions", arg));
}

