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
        GoogleTask<ResourcePermission>* create_Async(const gdrive::CreatePermissionArg& arg, const ResourcePermission& body);
        void create_AsyncCB(
            const gdrive::CreatePermissionArg&,
            const ResourcePermission& body,
            std::function<void(std::unique_ptr<ResourcePermission>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('delete')


            Delete permission

            */
        void deleteOperation(const gdrive::PermissionArg& );
        GoogleVoidTask* deleteOperation_Async(const gdrive::PermissionArg& arg);
        void deleteOperation_AsyncCB(
            const gdrive::PermissionArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('get')


            Get permission metadata.

            */
        std::unique_ptr<ResourcePermission> get(const gdrive::PermissionArg& arg);
        GoogleTask<ResourcePermission>* get_Async(const gdrive::PermissionArg& arg);
        void get_AsyncCB(
            const gdrive::PermissionArg&,
            std::function<void(std::unique_ptr<ResourcePermission>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('list')


            Lists a file's permissions.

            */
        std::unique_ptr<PermissionResourcesCollection> list(const gdrive::PermissionListArg& arg);
        GoogleTask<PermissionResourcesCollection>* list_Async(const gdrive::PermissionListArg& arg);
        void list_AsyncCB(
            const gdrive::PermissionListArg&,
            std::function<void(std::unique_ptr<PermissionResourcesCollection>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//PermissionsRoutes

}//permissions
}//googleQt
