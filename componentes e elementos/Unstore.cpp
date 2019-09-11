/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Unstore.h
 * Author: ...
 *
 * Created on 24 de Junho de 2019, 13:54
 */

#include "Unstore.h"
#include "Model.h"
#include "Attribute.h"

Unstore::Unstore(Model* model)
    : ModelComponent(model, Util::TypeOf<Unstore>())
{
}

Unstore::Unstore(const Unstore& orig)
    : ModelComponent(orig)
{
}

Unstore::~Unstore()
{
}

std::string Unstore::show()
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

ModelComponent* Unstore::LoadInstance(Model* model, std::map<std::string, std::string>* fields)
{
    Unstore* newComponent = new Unstore(model);
    try {
        newComponent->_loadInstance(fields);
    } catch (const std::exception& e) {
    }
    return newComponent;
}

void Unstore::setStorageType(Storage::StorageType _storageType)
{
    this->_storageType = _storageType;
}

Storage::StorageType Unstore::getStorageType() const
{
    return _storageType;
}

void Unstore::setStorageName(std::string storageName) throw()
{
    _storage = dynamic_cast<Storage*>(_model->getElementManager()->getElement(Util::TypeOf<Storage>(), storageName));
    if (_storage == nullptr) {
        _storage = new Storage(_model->getElementManager());
        _storage->setName(storageName);
        _model->getElementManager()->insert(Util::TypeOf<Storage>(), _storage);
    }
}

std::string Unstore::getStorageName() const
{
    return _storage->getName();
}

void Unstore::setAttributeName(std::string attributeName) throw()
{
    _attributeName = attributeName;
}

std::string Unstore::getAttributeName() const
{
    return _attributeName;
}

void Unstore::setStorage(Storage* storage)
{
    this->_storage = storage;
}

Storage* Unstore::getStorage() const
{
    return _storage;
}

void Unstore::_execute(Entity* entity)
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

    storage->removeElement();

    _model->getTraceManager()->traceSimulation(
        Util::TraceLevel::blockInternal,
        _model->getSimulation()->getSimulatedTime(),
        entity,
        this,
        "Entity unstores one unit in \"" + storage->getName() + "\", now with " + std::to_string(storage->count()) + " units");

    _model->sendEntityToComponent(entity, this->getNextComponents()->front(), 0.0);
}

bool Unstore::_loadInstance(std::map<std::string, std::string>* fields)
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

void Unstore::_initBetweenReplications()
{
}

std::map<std::string, std::string>* Unstore::_saveInstance()
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

bool Unstore::_check(std::string* errorMessage)
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

PluginInformation* Unstore::GetPluginInformation()
{
    return new PluginInformation(Util::TypeOf<Unstore>(), &Unstore::LoadInstance);
}
