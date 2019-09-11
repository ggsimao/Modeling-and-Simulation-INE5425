#include "Leave.h"
#include "Model.h"

Leave::Leave(Model* model) : ModelComponent(model, Util::TypeOf<Leave>()) {
}

Leave::Leave(const Leave& orig) : ModelComponent(orig) {
}

Leave::~Leave() {
    if(_seize != nullptr) 
        delete _seize; 
    if(_delay != nullptr)
        delete _delay;
}

void Leave::setStation(Station* station) {
    _station = station;
}

void Leave::setQueueResource(Queue* queue, Resource* resource) {
    _queue = queue;
    _resource = resource;
    _seize = new Seize(_model);
    _seize->setResource(_resource);
    _seize->setQueue(_queue);
}

void Leave::connect() {
    if (_station != nullptr) { 
        this->connect(_station->getFirstComponent());
    }  

}

void Leave::connect(ModelComponent* directlyConnected) {
    if (directlyConnected != nullptr){
        _connected = true;
        if(_seize != nullptr && _delay != nullptr) {
            this->getNextComponents()->insert(_seize);
            _seize->getNextComponents()->insert(_delay); 
            _delay->getNextComponents()->insert(directlyConnected);   
        } else if (_seize != nullptr){
            this->getNextComponents()->insert(_seize);
            _seize->getNextComponents()->insert(directlyConnected); 
        } else if (_delay != nullptr){
            this->getNextComponents()->insert(_delay);
            _delay->getNextComponents()->insert(directlyConnected);  
        } else {
            this->getNextComponents()->insert(directlyConnected);
        }
    }

}

void Leave::setDelayExpression(std::string delayExpression, Util::TimeUnit delayTimeUnit) {
    _delayExpression = delayExpression;
    _delayTimeUnit = delayTimeUnit;
    _delay = new Delay(_model);
    _delay->setDelayTimeUnit(_delayTimeUnit);
}

std::string Leave::show() {
    std::string s = "";
    if (_station != nullptr)
        s += ",connectedStation='" + _station->getName() + "'";
    if (_queue != nullptr && _resource != nullptr)
        s += ",seizeResource=1,resource='" + _resource->getName() + "',queue='" + _queue->getName() + "'";
    return ModelComponent::show() + s;
}

ModelComponent* Leave::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
    Leave* newComponent = new Leave(model);
    try {
	newComponent->_loadInstance(fields);
    } catch (const std::exception& e) {

    }
    return newComponent;
}

void Leave::_execute(Entity* entity) {
    _model->getTraceManager()->trace(Util::TraceLevel::blockInternal, "Entity " + std::to_string(entity->getId()) + " leaving to the next station");
    double moveTime = _model->parseExpression(_delayExpression) * Util::TimeUnitConvert(_delayTimeUnit, _model->getInfos()->getReplicationLengthTimeUnit());
    _delay->setDelayExpression(std::to_string(moveTime));
    entity->getEntityType()->getCstatTransferTime()->getStatistics()->getCollector()->addValue(moveTime);
    entity->setAttributeValue("Entity.TransferTime", entity->getAttributeValue("Entity.TransferTime") + moveTime);
    this->_model->sendEntityToComponent(entity, this->getNextComponents()->front(), 0.0);
}

bool Leave::_loadInstance(std::map<std::string, std::string>* fields) {
    bool res = ModelComponent::_loadInstance(fields);
    if (res) {
    }
    return res;
}

void Leave::_initBetweenReplications() {
    if (_resource != nullptr) {_resource->initBetweenReplications();}
    if (_queue != nullptr) {_queue->initBetweenReplication();}
}

std::map<std::string, std::string>* Leave::_saveInstance() {
    std::map<std::string, std::string>* fields = ModelComponent::_saveInstance();
    if(_queue != nullptr && _resource != nullptr) {
        fields->emplace("queueId", std::to_string(this->_queue->getId()));
        fields->emplace("queueName", (this->_queue->getName()));
        fields->emplace("resourceId", std::to_string(this->_resource->getId()));
        fields->emplace("resourceName", (this->_resource->getName()));
    }
    if(_delay != nullptr) {
        fields->emplace("delayExpression", this->_delayExpression);
        fields->emplace("delayExpressionTimeUnit", std::to_string(static_cast<int> (this->_delayTimeUnit)));
    }
    if(_station != nullptr) {
        fields->emplace("stationId", std::to_string(this->_station->getId()));
        fields->emplace("stationName", (this->_station->getName()));
    }
    return fields;
}

bool Leave::_check(std::string* errorMessage) {
    bool resultAll = true;
    if (_resource != nullptr) {resultAll &= _model->getElementManager()->check(Util::TypeOf<Resource>(), _resource, "Resource", errorMessage);}
    if (_queue != nullptr) {resultAll &= _model->getElementManager()->check(Util::TypeOf<Queue>(), _queue, "Queue", errorMessage);}
    resultAll &= (_station != nullptr || this->getNextComponents()->front() != nullptr);
    resultAll &= _connected;
    if(_resource == nullptr)
        resultAll &= (_queue == nullptr);
    if(_queue == nullptr)
        resultAll &= (_resource == nullptr);
    return resultAll;
}

PluginInformation* Leave::GetPluginInformation(){
    return new PluginInformation(Util::TypeOf<Leave>(), &Leave::LoadInstance);
}
