/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification 
 www.prokarpaty.net
***********************************************************/

#include "GdriveAutotest.h"
#ifdef API_QT_AUTOTEST
using namespace googleQt;
using namespace googleQt;

static GdriveRoutes* cl;


static void call_get_from_About(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("About").arg("get");
    std::unique_ptr<gdrive::AboutArg> arg = gdrive::AboutArg::EXAMPLE();
    cl->getAbout()->get(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_create_from_Comments(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Comments").arg("create");
    std::unique_ptr<gdrive::CreateCommentArg> arg = gdrive::CreateCommentArg::EXAMPLE();
    std::unique_ptr<comments::Comment> arg2 = comments::Comment::EXAMPLE();
    cl->getComments()->create(*(arg.get()) , *(arg2.get()));
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_deleteOperation_from_Comments(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Comments").arg("deleteOperation");
    std::unique_ptr<gdrive::DeleteCommentArg> arg = gdrive::DeleteCommentArg::EXAMPLE();
    cl->getComments()->deleteOperation(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_get_from_Comments(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Comments").arg("get");
    std::unique_ptr<gdrive::GetCommentArg> arg = gdrive::GetCommentArg::EXAMPLE();
    cl->getComments()->get(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_list_from_Comments(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Comments").arg("list");
    std::unique_ptr<gdrive::CommentListArg> arg = gdrive::CommentListArg::EXAMPLE();
    cl->getComments()->list(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_copy_from_Files(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Files").arg("copy");
    std::unique_ptr<gdrive::CopyFileArg> arg = gdrive::CopyFileArg::EXAMPLE();
    std::unique_ptr<files::FileResource> arg2 = files::FileResource::EXAMPLE();
    cl->getFiles()->copy(*(arg.get()) , *(arg2.get()));
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_create_from_Files(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Files").arg("create");
    std::unique_ptr<gdrive::CreateFileArg> arg = gdrive::CreateFileArg::EXAMPLE();
    std::unique_ptr<files::FileResource> arg2 = files::FileResource::EXAMPLE();
    cl->getFiles()->create(*(arg.get()) , *(arg2.get()));
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_deleteOperation_from_Files(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Files").arg("deleteOperation");
    std::unique_ptr<gdrive::DeleteFileArg> arg = gdrive::DeleteFileArg::EXAMPLE();
    cl->getFiles()->deleteOperation(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_emptyTrash_from_Files(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Files").arg("emptyTrash");
    std::unique_ptr<gdrive::EmptyTrashArg> arg = gdrive::EmptyTrashArg::EXAMPLE();
    cl->getFiles()->emptyTrash(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_get_from_Files(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Files").arg("get");
    std::unique_ptr<gdrive::GetFileArg> arg = gdrive::GetFileArg::EXAMPLE();
    cl->getFiles()->get(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_list_from_Files(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Files").arg("list");
    std::unique_ptr<gdrive::FileListArg> arg = gdrive::FileListArg::EXAMPLE();
    cl->getFiles()->list(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_create_from_Permissions(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Permissions").arg("create");
    std::unique_ptr<gdrive::CreatePermissionArg> arg = gdrive::CreatePermissionArg::EXAMPLE();
    std::unique_ptr<permissions::ResourcePermission> arg2 = permissions::ResourcePermission::EXAMPLE();
    cl->getPermissions()->create(*(arg.get()) , *(arg2.get()));
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_deleteOperation_from_Permissions(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Permissions").arg("deleteOperation");
    std::unique_ptr<gdrive::PermissionArg> arg = gdrive::PermissionArg::EXAMPLE();
    cl->getPermissions()->deleteOperation(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_get_from_Permissions(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Permissions").arg("get");
    std::unique_ptr<gdrive::PermissionArg> arg = gdrive::PermissionArg::EXAMPLE();
    cl->getPermissions()->get(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_list_from_Permissions(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Permissions").arg("list");
    std::unique_ptr<gdrive::PermissionListArg> arg = gdrive::PermissionListArg::EXAMPLE();
    cl->getPermissions()->list(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}


static void test_call_AboutRoutes(){
    call_get_from_About();
}

static void test_call_CommentsRoutes(){
    call_create_from_Comments();
    call_deleteOperation_from_Comments();
    call_get_from_Comments();
    call_list_from_Comments();
}

static void test_call_FilesRoutes(){
    call_copy_from_Files();
    call_create_from_Files();
    call_deleteOperation_from_Files();
    call_emptyTrash_from_Files();
    call_get_from_Files();
    call_list_from_Files();
}

static void test_call_PermissionsRoutes(){
    call_create_from_Permissions();
    call_deleteOperation_from_Permissions();
    call_get_from_Permissions();
    call_list_from_Permissions();
}


void GdriveAutotest::generateCalls(){
    ApiAutotest::INSTANCE() << "";
    ApiAutotest::INSTANCE() << "============ autotest for module: gdrive ============";
    test_call_AboutRoutes();
    test_call_CommentsRoutes();
    test_call_FilesRoutes();
    test_call_PermissionsRoutes();
}

GdriveAutotest::GdriveAutotest(GoogleClient& c)
{
    cl = c.gdrive();
}
#endif //API_QT_AUTOTEST

