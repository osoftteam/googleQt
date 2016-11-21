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

            /**
            ApiRoute('delete')


            Immediately and permanently deletes the specified label and removes
            it from any messages and threads that it is applied to.

            */
        void deleteOperation(const gmail::IdArg& );

            /**
            ApiRoute('get')


            Get the specified label info

            */
        std::unique_ptr<LabelResource> get(const gmail::IdArg& arg);

            /**
            ApiRoute('list')


            Lists all labels in the user's mailbox.

            */
        std::unique_ptr<LabelsResultList> list(void);

            /**
            ApiRoute('update')


            Updates the specified label

            */
        std::unique_ptr<LabelResource> update(const gmail::IdArg& arg, const LabelResource& body);

    protected:
    };//LabelsRoutes

}//labels
}//googleQt
