/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Store.h
 * Author: ...
 *
 * Created on 24 de Junho de 2019, 13:54
 */

#include "Store.h"
#include "Model.h"
#include "Attribute.h"

Store::Store(Model* model)
    : ModelComponent(model, Util::TypeOf<Store>())
{
}

Store::Store(const Store& orig)
    : ModelComponent(orig)
{
}

Store::~Store()
{
}

std::string Store::show()
{
    std::string out = ModelComponent::show();

    if (this->_storageType == Storage::StorageType::STORAGE) {
        out += "storageType=\"Storage\",storageName=\"" + _storage->getName() + "\"";
    }

    else if (this->_storageType == Storage::StorageType::SET) {
        out += "storageType=\"Set\"";
    }

    else if (this->_storageType == Storage::StorageType::ATTRIBUTE) {
        out += "storageType=\"Attribute\",attributeName=\"" + _attributeName + "\"";
    }

    else if (this->_storageType == Storage::StorageType::EXPRESSION) {
        out += "storageType=\"Expression\"";
    }

    return out;
}

ModelComponent* Store::LoadInstance(Model* model, std::map<std::string, std::string>* fields)
{
    Store* newComponent = new Store(model);
    try {
        newComponent->_loadInstance(fields);
    } catch (const std::exception& e) {
    }
    return newComponent;
}

void Store::setStorageType(Storage::StorageType _storageType)
{
    this->_storageType = _storageType;
}

Storage::StorageType Store::getStorageType() const
{
    return _storageType;
}

void Store::setStorageName(std::string storageName) throw()
{
    _storage = dynamic_cast<Storage*>(_model->getElementManager()->getElement(Util::TypeOf<Storage>(), storageName));
    if (_storage == nullptr) {
        _storage = new Storage(_model->getElementManager());
        _storage->setName(storageName);
        _model->getElementManager()->insert(Util::TypeOf<Storage>(), _storage);
    }
}

std::string Store::getStorageName() const
{
    return _storage->getName();
}

void Store::setAttributeName(std::string attributeName) throw()
{
    _attributeName = attributeName;
}

std::string Store::getAttributeName() const
{
    return _attributeName;
}

void Store::setStorage(Storage* storage)
{
    this->_storage = storage;
}

Storage* Store::getStorage() const
{
    return _storage;
}

void Store::_execute(Entity* entity)
{
    Storage* storage;

    if (this->_storageType == Storage::StorageType::STORAGE) {
        storage = _storage;
    }

    else if (this->_storageType == Storage::StorageType::SET) {
        // ...
    }

    else if (this->_storageType == Storage::StorageType::ATTRIBUTE) {
        Util::identitifcation storageId = entity->getAttributeValue(_attributeName);
        storage = dynamic_cast<Storage*>(_model->getElementManager()->getElement(Util::TypeOf<Storage>(), storageId));
    }

    else if (this->_storageType == Storage::StorageType::EXPRESSION) {
        // ...
    }

    if (storage == nullptr) {
        throw std::invalid_argument("Storage does not exist");
    }

    storage->insertElement();

    _model->getTraceManager()->traceSimulation(
        Util::TraceLevel::blockInternal,
        _model->getSimulation()->getSimulatedTime(),
        entity,
        this,
        "Entity stores one unit in \"" + storage->getName() + "\", now with " + std::to_string(storage->count()) + " units");

    _model->sendEntityToComponent(entity, this->getNextComponents()->front(), 0.0);
}

bool Store::_loadInstance(std::map<std::string, std::string>* fields)
{
    bool res = ModelComponent::_loadInstance(fields);

    if (res) {
        std::string storageType = (*(fields->find("storageType"))).second;

        if (storageType == "Storage") {
            _storageType = Storage::StorageType::STORAGE;
            std::string storageName = ((*(fields->find("storageName"))).second);
            _storage = dynamic_cast<Storage*>(_model->getElementManager()->getElement(Util::TypeOf<Storage>(), storageName));
            if (_storage == nullptr) {
                throw std::invalid_argument("Storage does not exist");
            }
        }

        else if (storageType == "Set") {
            // ...
        }


        else if (storageType == "Attribute") {
            _storageType = Storage::StorageType::ATTRIBUTE;
            std::string attributeName = ((*(fields->find("attributeName"))).second);
        }

        else if (storageType == "Expression") {
            // ...
        }
    }
    return res;
}

void Store::_initBetweenReplications()
{
}

std::map<std::string, std::string>* Store::_saveInstance()
{
    std::map<std::string, std::string>* fields = ModelComponent::_saveInstance();

    if (_storageType == Storage::StorageType::STORAGE) {
        fields->emplace("storageType", "Storage");
        fields->emplace("storageName", (_storage->getName()));
    }

    else if (_storageType == Storage::StorageType::SET) {
        fields->emplace("storageType", "Set");
        // ...
    }

    else if (_storageType == Storage::StorageType::ATTRIBUTE) {
        fields->emplace("storageType", "Attribute");
        fields->emplace("attributeName", _attributeName);
    }

    else if (_storageType == Storage::StorageType::EXPRESSION) {
        fields->emplace("storageType", "Expression");
        // ...
    }

    return fields;
}

bool Store::_check(std::string* errorMessage)
{
    if (_storageType == Storage::StorageType::STORAGE) {
        if (_storage == nullptr) {
            errorMessage->append("Storage unset in " + this->getName());
            return false;
        }
    }

    else if (_storageType == Storage::StorageType::SET) {
        // ...
    }

    else if (_storageType == Storage::StorageType::ATTRIBUTE) {
        Attribute* attribute = dynamic_cast<Attribute*>(_model->getElementManager()->getElement(Util::TypeOf<Attribute>(), _attributeName));
        if (attribute == nullptr) {
            errorMessage->append("Invalid attribute name in " + this->getName());
            return false;
        }
    }

    else if (_storageType == Storage::StorageType::EXPRESSION) {
        // ...
    }

    else {
        errorMessage->append("Invalid storage type in " + this->getName());
        return false;
    }

    return true;
}

PluginInformation* Store::GetPluginInformation()
{
    return new PluginInformation(Util::TypeOf<Store>(), &Store::LoadInstance);
}
