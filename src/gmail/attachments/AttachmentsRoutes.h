/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "attachments"
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "GoogleRouteBase.h"
#include "gmail/attachments/AttachmentsMessageAttachment.h"

namespace googleQt{
namespace attachments{


    class AttachmentsRoutes: public GoogleRouteBase{
    public:
        AttachmentsRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('get')


            Get the specified attachment

            */
        std::unique_ptr<MessageAttachment> get(const gmail::AttachmentIdArg& arg);
        GoogleTask<MessageAttachment>* get_Async(const gmail::AttachmentIdArg& arg);
        void get_AsyncCB(
            const gmail::AttachmentIdArg&,
            std::function<void(std::unique_ptr<MessageAttachment>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//AttachmentsRoutes

}//attachments
}//googleQt
