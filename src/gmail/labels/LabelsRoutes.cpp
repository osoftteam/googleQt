/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "labels"
 www.prokarpaty.net
***********************************************************/

#include "gmail/labels/LabelsRoutes.h"
#include "Endpoint.h"
#include "gmail/GmailRoutes.h"
using namespace googleQt;
using namespace labels;

std::unique_ptr<LabelResource> LabelsRoutes::create(const LabelResource& body){
    return m_end_point->postStyle<std::unique_ptr<LabelResource>, LabelResource::factory, LabelResource >(m_end_point->buildGmailUrl("labels", VoidType::instance()), body);
}

void LabelsRoutes::deleteOperation(const gmail::IdArg& arg ){
    m_end_point->deleteStyle<std::unique_ptr<VoidType>, VoidType, NotAnException>(m_end_point->buildGmailUrl("labels", arg) );
}

std::unique_ptr<LabelResource> LabelsRoutes::get(const gmail::IdArg& arg){
    return m_end_point->getStyle<std::unique_ptr<LabelResource>, LabelResource::factory, NotAnException>(m_end_point->buildGmailUrl("labels", arg));
}

std::unique_ptr<LabelsResultList> LabelsRoutes::list(void){
    return m_end_point->getStyle<std::unique_ptr<LabelsResultList>, LabelsResultList::factory, NotAnException >(m_end_point->buildGmailUrl("labels", VoidType::instance()));
}

std::unique_ptr<LabelResource> LabelsRoutes::update(const gmail::IdArg& arg, const LabelResource& body){
    return m_end_point->putStyle<std::unique_ptr<LabelResource>, LabelResource::factory, LabelResource>(m_end_point->buildGmailUrl("labels", arg), body);
}

