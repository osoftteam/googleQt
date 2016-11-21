/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "permissions"
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include "GoogleRouteBase.h"
#include "gdrive/permissions/PermissionsPermissionResourcesCollection.h"
#include "gdrive/permissions/PermissionsResourcePermission.h"

namespace googleQt{
namespace permissions{


    class PermissionsRoutes: public GoogleRouteBase{
    public:
        PermissionsRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('create')


            Create new permission

            */
        std::unique_ptr<ResourcePermission> create(const gdrive::CreatePermissionArg& arg, const ResourcePermission& body);

            /**
            ApiRoute('delete')


            Delete permission

            */
        void deleteOperation(const gdrive::PermissionArg& );

            /**
            ApiRoute('get')


            Get permission metadata.

            */
        std::unique_ptr<ResourcePermission> get(const gdrive::PermissionArg& arg);

            /**
            ApiRoute('list')


            Lists a file's permissions.

            */
        std::unique_ptr<PermissionResourcesCollection> list(const gdrive::PermissionListArg& arg);

    protected:
    };//PermissionsRoutes

}//permissions
}//googleQt
