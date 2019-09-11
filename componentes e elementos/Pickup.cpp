/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Pickup.cpp
 * Authors: leonardo.schluter.leite,
 *          gustavo.tarciso,
 *          alexandre.behling
 *
 * Created on 22 de Maio de 2019, 18:41
 */

#include "Pickup.h"
#include "Model.h"
#include "Group.h"
#include "Attribute.h"

Pickup::Pickup(Model* model) : ModelComponent(model, Util::TypeOf<Pickup>()) {
}

Pickup::Pickup(const Pickup& orig) : ModelComponent(orig) {
}

Pickup::~Pickup() {
}

std::string Pickup::show() {
    return ModelComponent::show() +
	    ",quantity=" + this->_quantity +
	    ",startingRank=" + this->_startingRank;
}

ModelComponent* Pickup::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
    Pickup* newComponent = new Pickup(model);
    try {
	newComponent->_loadInstance(fields);
    } catch (const std::exception& e) {

    }
    return newComponent;
}

void Pickup::_execute(Entity* entity) {
    ElementManager* elements = this->_model->getElementManager();
    
    Group* group = new Group(elements, "GroupOf_"+entity->getId());
    elements->insert(Util::TypeOf<Group>(), group);
    entity->setAttributeValue("Entity.Group", group->getId());
  
  //starting rank starts at 1. 
  //so we test to see if there is entities enough in the queue to be picked up.
    double tnow = this->_model->getSimulation()->getSimulatedTime();
    unsigned int quantity, startingRank = 0;
    quantity = _model->parseExpression(this->_quantity);
    startingRank = _model->parseExpression(this->_startingRank);
    if((quantity + startingRank-1) <= this->_queue->size()){
        for(int i = startingRank-1; i < quantity + (startingRank-1);i++){
            Waiting* entityToRemove;
            if(startingRank == 1){ 
                entityToRemove = this->_queue->first();
            }else{
                entityToRemove = this->_queue->getElementAtRank(startingRank-1);
            }

            this->_queue->removeElement(entityToRemove, tnow);
            group->insertElement(entityToRemove->getEntity());
            _model->getTraceManager()->trace(Util::TraceLevel::blockInternal, "I'll pick the entity of ID = \"" + std::to_string(entityToRemove->getEntity()->getId()) + " and set it to the group of ID = "+ std::to_string(entity->getAttributeValue("Entity.Group")));
        }
        this->_model->sendEntityToComponent(entity, this->getNextComponents()->front(), 0.0);    
    }else{
        _model->getTraceManager()->trace(Util::TraceLevel::blockInternal, "Not enough entities in queue" );
    }
}

bool Pickup::_loadInstance(std::map<std::string, std::string>* fields) {
    bool res = ModelComponent::_loadInstance(fields);
    if (res) {
        std::string queueName = ((*(fields->find("queueName"))).second);
        Queue* queue = dynamic_cast<Queue*> (_model->getElementManager()->getElement(Util::TypeOf<Queue>(), queueName));
        this->_queue = queue;
	    this->_startingRank = std::stoi((*(fields->find("startingRank"))).second);
	    this->_quantity = ((*(fields->find("quantity"))).second);
    }
    return res;
}

void Pickup::_initBetweenReplications() {
    this->_queue->initBetweenReplication();
}

std::map<std::string, std::string>* Pickup::_saveInstance() {
     std::map<std::string, std::string>* fields = ModelComponent::_saveInstance(); //Util::TypeOf<Seize>());
    fields->emplace("startingRank", this->_startingRank);
    fields->emplace("quantity", this->_quantity);
    fields->emplace("queueId", std::to_string(this->_queue->getId()));
    fields->emplace("queueName", (this->_queue->getName()));
    return fields;
}

bool Pickup::_check(std::string* errorMessage) {
    bool resultAll = true;
    resultAll &= _model->checkExpression(this->_quantity, "quantity", errorMessage);
    resultAll &= _model->checkExpression(this->_startingRank, "startingRank", errorMessage);
    return resultAll;
}

PluginInformation* Pickup::GetPluginInformation(){
    return new PluginInformation(Util::TypeOf<Pickup>(), &Pickup::LoadInstance);
}


void Pickup::setQueueName(std::string queueName) throw () {
    Queue* queue = dynamic_cast<Queue*> (this->_model->getElementManager()->getElement(Util::TypeOf<Queue>(), queueName));
    if (queue != nullptr) {
	    this->_queue = queue;
    } else {
	    throw std::invalid_argument("Queue does not exist");
    }
}

Queue* Pickup::getQueue() const {
    return this->_queue;
}

void Pickup::setQuantity(std::string quantity){
  this->_quantity = quantity;
}

std::string Pickup::getQuantity() const {
    return this->_quantity;
}

void Pickup::setStartingRank(std::string startingRank){
  this->_startingRank = startingRank;
}

std::string Pickup::getStartingRank() const {
    return this->_startingRank;
}
