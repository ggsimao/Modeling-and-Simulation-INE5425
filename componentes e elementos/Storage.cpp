/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Storage.cpp
 * Author: ...
 *
 * Created on ...
 */

#include "Storage.h"
#include "Attribute.h"
#include "Model.h"

Storage::Storage(ElementManager* elems)
    : ModelElement(Util::TypeOf<Storage>())
{
    _elems = elems;
    _count = 0;
    _initCStats();
}

Storage::Storage(ElementManager* elems, std::string name)
    : ModelElement(Util::TypeOf<Storage>())
{
    _name = name;
    _elems = elems;
    _count = 0;
    _initCStats();
}

void Storage::_initCStats()
{
    _cstatNumberInStorage = new StatisticsCollector("Number In Storage", this); /* TODO: ++ WHY THIS INSERT "DISPOSE" AND "10ENTITYTYPE" STATCOLL ?? */
    _elems->insert(Util::TypeOf<StatisticsCollector>(), _cstatNumberInStorage);
}

Storage::Storage(const Storage& orig)
    : ModelElement(orig)
{
}

Storage::~Storage()
{
    _elems->remove(Util::TypeOf<StatisticsCollector>(), _cstatNumberInStorage);
}

std::string Storage::show()
{
    return ModelElement::show() + ",stored=" + std::to_string(this->_count);
}

void Storage::insertElement()
{
    ++_count;
    this->_cstatNumberInStorage->getStatistics()->getCollector()->addValue(_count);
}

void Storage::removeElement()
{
    --_count;
    this->_cstatNumberInStorage->getStatistics()->getCollector()->addValue(_count);
}

void Storage::initBetweenReplication()
{
    _count = 0;
}

long int Storage::count()
{
    return _count;
}

void Storage::setAttributeName(std::string _attributeName)
{
    this->_attributeName = _attributeName;
}

std::string Storage::getAttributeName() const
{
    return _attributeName;
}

PluginInformation* Storage::GetPluginInformation()
{
    return new PluginInformation(Util::TypeOf<Storage>(), &Storage::LoadInstance);
}

ModelElement* Storage::LoadInstance(ElementManager* elems, std::map<std::string, std::string>* fields)
{
    Storage* newElement = new Storage(elems);
    try {
        newElement->_loadInstance(fields);
    } catch (const std::exception& e) {
    }
    return newElement;
}

bool Storage::_loadInstance(std::map<std::string, std::string>* fields)
{
    bool res = ModelElement::_loadInstance(fields);
    if (res) {
        this->_attributeName = (*fields->find("attributeName")).second;
    }
    return res;
}

std::map<std::string, std::string>* Storage::_saveInstance()
{
    std::map<std::string, std::string>* fields = ModelElement::_saveInstance();
    fields->emplace("attributeName", this->_attributeName);
    return fields;
}

bool Storage::_check(std::string* errorMessage)
{
    return _elems->check(Util::TypeOf<Attribute>(), _attributeName, "AttributeName", false, errorMessage);
}
