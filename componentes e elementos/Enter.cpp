#include "Enter.h"
#include "Model.h"

Enter::Enter(Model* model) : ModelComponent(model, Util::TypeOf<Enter>()) {
}

Enter::Enter(const Enter& orig) : ModelComponent(orig) {
}

Enter::~Enter() {
    if(_release != nullptr)
        delete _release;
}

std::string Enter::show() {
    std::string s = "";
    if(_station != nullptr)
        s += ",connectedStation='" + _station->getName() + "'";
    if(_release != nullptr)
        s += ",releaseResource=1,resource='" + _resource->getName() + "'";
    return ModelComponent::show() + s;
}

ModelComponent* Enter::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
    Enter* newComponent = new Enter(model);
    try {
	newComponent->_loadInstance(fields);
    } catch (const std::exception& e) {

    }
    return newComponent;
}

void Enter::setStation(Station* station){
    _station = station;
    _station->setFirstComponent(this);
}

void Enter::setResource(Resource* resource) {
    _resource = resource;
    _release = new Release(_model);
    _release->setResource(_resource);
}

void Enter::connect(ModelComponent* directlyConnected) {
    _connected = true;
    if (_release != nullptr) {
        this->getNextComponents()->insert(_release);
        _release->getNextComponents()->insert(directlyConnected);
    } else {
        this->getNextComponents()->insert(directlyConnected);
    }
}    

void Enter::_execute(Entity* entity) {
    _model->getTraceManager()->trace(Util::TraceLevel::blockInternal, "Entity " + std::to_string(entity->getId()) + " arrived in " + this->getName());
    this->_model->sendEntityToComponent(entity, this->getNextComponents()->front(), 0.0);
}

bool Enter::_loadInstance(std::map<std::string, std::string>* fields) {
    bool res = ModelComponent::_loadInstance(fields);
    if (res) {
    }
    return res;
}

void Enter::_initBetweenReplications() {
    if(_resource != nullptr)
        this->_resource->initBetweenReplications();
}

std::map<std::string, std::string>* Enter::_saveInstance() {
    std::map<std::string, std::string>* fields = ModelComponent::_saveInstance(); 
    if(_resource != nullptr) fields->emplace("resourceId", std::to_string(this->_resource->getId()));
    if(_resource != nullptr) fields->emplace("resourceName", (this->_resource->getName()));
    if(_station != nullptr) fields->emplace("stationId", std::to_string(this->_station->getId()));
    if(_station != nullptr) fields->emplace("stationName", (this->_station->getName()));
    return fields;
}

bool Enter::_check(std::string* errorMessage) {
    bool resultAll = true;
    resultAll &= _connected;
    if(_resource != nullptr) {resultAll &= _model->getElementManager()->check(Util::TypeOf<Resource>(), _resource, "Resource", errorMessage);}    
    return resultAll;
}

PluginInformation* Enter::GetPluginInformation(){
    return new PluginInformation(Util::TypeOf<Enter>(), &Enter::LoadInstance);
}
