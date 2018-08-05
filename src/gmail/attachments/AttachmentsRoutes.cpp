/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "attachments"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gmail/attachments/AttachmentsRoutes.h"
#include "Endpoint.h"
#include "gmail/GmailRoutes.h"
using namespace googleQt;
using namespace attachments;

std::unique_ptr<MessageAttachment> AttachmentsRoutes::get(const gmail::AttachmentIdArg& arg){
    return get_Async(arg)->waitForResultAndRelease();
}

GoogleTask<MessageAttachment>* AttachmentsRoutes::get_Async(const gmail::AttachmentIdArg& arg)
{
    GoogleTask<MessageAttachment>* t = m_end_point->produceTask<MessageAttachment>();
    m_end_point->getStyle<
        MessageAttachment,
        MessageAttachment::factory
        >
        (m_end_point->buildGmailAttachmentUrl(arg),
        t);
    return t;
}

void AttachmentsRoutes::get_AsyncCB(
    const gmail::AttachmentIdArg& arg,
    std::function<void(std::unique_ptr<MessageAttachment>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        MessageAttachment,
        MessageAttachment::factory
        >
        (m_end_point->buildGmailAttachmentUrl(arg),
        completed_callback,
        failed_callback);
}

