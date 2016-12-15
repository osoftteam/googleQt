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
        GoogleTask<LabelResource>* create_Async(const LabelResource& body);
        void create_AsyncCB(
            const LabelResource& body,
            std::function<void(std::unique_ptr<LabelResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('delete')


            Immediately and permanently deletes the specified label and removes
            it from any messages and threads that it is applied to.

            */
        void deleteOperation(const gmail::IdArg& );
        GoogleVoidTask* deleteOperation_Async(const gmail::IdArg& arg);
        void deleteOperation_AsyncCB(
            const gmail::IdArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('get')


            Get the specified label info

            */
        std::unique_ptr<LabelResource> get(const gmail::IdArg& arg);
        GoogleTask<LabelResource>* get_Async(const gmail::IdArg& arg);
        void get_AsyncCB(
            const gmail::IdArg&,
            std::function<void(std::unique_ptr<LabelResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('list')


            Lists all labels in the user's mailbox.

            */
        std::unique_ptr<LabelsResultList> list();
        GoogleTask<LabelsResultList>* list_Async();
        void list_AsyncCB(
            std::function<void(std::unique_ptr<LabelsResultList>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('update')


            Updates the specified label

            */
        std::unique_ptr<LabelResource> update(const gmail::IdArg& arg, const LabelResource& body);
        GoogleTask<LabelResource>* update_Async(const gmail::IdArg& arg, const LabelResource& body);
        void update_AsyncCB(
            const gmail::IdArg&,
            const LabelResource& body,
            std::function<void(std::unique_ptr<LabelResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//LabelsRoutes

}//labels
}//googleQt
