/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Remove.cpp
 * Author: rlcancian
 * 
 * Created on 03 de Junho de 2019, 15:20
 */

#include "Remove.h"

Remove::Remove(Model* model) : ModelComponent(model, Util::TypeOf<Remove>()) {
}

Remove::Remove(const Remove& orig) : ModelComponent(orig) {
}

Remove::~Remove() {
}

std::string Remove::show() {
    return ModelComponent::show() +
	    ",queue=\'" + this->getQueue()->getAttributeName() + "\"" +
	    ",rank=" + std::to_string(this->getRank()) +
	    ",nextModule=" + this->getNextModule()->getName();
}
PluginInformation* Remove::GetPluginInformation() {
    PluginInformation* info = new PluginInformation(Util::TypeOf<Remove>(), &Remove::LoadInstance);
    info->insertDynamicLibFileDependence("queue.so");
    return info;
}
ModelComponent* Remove::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
    Remove* newComponent = new Remove(model);
    try {
	    newComponent->_loadInstance(fields);
    } catch (const std::exception& e) {

    }
    return newComponent;
}

Queue* Remove::getQueue() {
    return _queue;
}
ModelComponent* Remove::getNextModule() {
    return _nextModule;
}
int Remove::getRank() {
    return _rank;
}

void Remove::_execute(Entity* entity) {
    if (_queue->size()) {
        Waiting* element = _queue->getAtRank(_rank);
        Entity* entityFromQueue = _queue->getAtRank(_rank)->getEntity();
        _queue->removeElement(element);
        _model->sendEntityToComponent(entity, _nextModule, 0.0);
        _model->sendEntityToComponent(entityFromQueue, _nextModule, 0.0);
    }
}
bool Remove::_loadInstance(std::map<std::string, std::string>* fields) {
    bool res = ModelComponent::_loadInstance(fields);
    if (res) {
        std::string queueName = ((*(fields->find("queueName"))).second);
        this->_queue = dynamic_cast<Queue*> (_model->getElementManager()->getElement(Util::TypeOf<Queue>(), queueName));
        std::string resourceName = ((*(fields->find("nextModuleName"))).second);
        this->_nextModule = dynamic_cast<ModelComponent*> (_model->getElementManager()->getElement(Util::TypeOf<ModelComponent*>(), resourceName));
        this->_rank = std::stoi((*(fields->find("rank"))).second);
    }
    return res;
}
void Remove::_initBetweenReplications() {
    this->_queue->initBetweenReplications();
}
std::map<std::string, std::string>* Remove::_saveInstance() {
    std::map<std::string, std::string>* fields = ModelComponent::_saveInstance(); //Util::TypeOf<Seize>());
    fields->emplace("rank", this->_rank);
    fields->emplace("queueId", std::to_string(this->_queue->getId()));
    fields->emplace("queueName", (this->_queue->getName()));
    fields->emplace("nextModuleName", (this->_nextModule->getName()));
    fields->emplace("resourceId", std::to_string(this->_nextModule->getId()));
    return fields;
}
bool Remove::_check(std::string* errorMessage) {
    return true;
}

