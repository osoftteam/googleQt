/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "labels"
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "GoogleRouteBase.h"
#include "gmail/labels/LabelsLabelResource.h"
#include "gmail/labels/LabelsLabelsResultList.h"

namespace googleQt{
namespace labels{


    class LabelsRoutes: public GoogleRouteBase{
    public:
        LabelsRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('create')


            Creates a new label.

            */
        std::unique_ptr<LabelResource> create(const LabelResource& body);
        void create_Async(
            const LabelResource& body,
            std::function<void(std::unique_ptr<LabelResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('delete')


            Immediately and permanently deletes the specified label and removes
            it from any messages and threads that it is applied to.

            */
        void deleteOperation(const gmail::IdArg& );
        void deleteOperation_Async(
            const gmail::IdArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('get')


            Get the specified label info

            */
        std::unique_ptr<LabelResource> get(const gmail::IdArg& arg);
        void get_Async(
            const gmail::IdArg&,
            std::function<void(std::unique_ptr<LabelResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('list')


            Lists all labels in the user's mailbox.

            */
        std::unique_ptr<LabelsResultList> list(void);
        void list_Async(
            std::function<void(std::unique_ptr<LabelsResultList>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('update')


            Updates the specified label

            */
        std::unique_ptr<LabelResource> update(const gmail::IdArg& arg, const LabelResource& body);
        void update_Async(
            const gmail::IdArg&,
            const LabelResource& body,
            std::function<void(std::unique_ptr<LabelResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//LabelsRoutes

}//labels
}//googleQt
