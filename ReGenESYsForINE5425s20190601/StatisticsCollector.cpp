/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StatisticsCollector.cpp
 * Author: rafael.luiz.cancian
 * 
 * Created on 30 de Agosto de 2018, 17:24
 */

#include "StatisticsCollector.h"
#include "Traits.h"

StatisticsCollector::StatisticsCollector() : ModelElement(Util::TypeOf<StatisticsCollector>()) {
    _initStaticsAndCollector();
    _generateReportInformation = true;
}

StatisticsCollector::StatisticsCollector(std::string name) : ModelElement(Util::TypeOf<StatisticsCollector>()) {
    _name = name;
    _initStaticsAndCollector();
}

StatisticsCollector::StatisticsCollector(std::string name, ModelElement* parent) : ModelElement(Util::TypeOf<StatisticsCollector>()) {
    _name = name;
    _parent = parent;
    _initStaticsAndCollector();
}

void StatisticsCollector::_initStaticsAndCollector() {
    Collector_if* collector = new Traits<ModelComponent>::StatisticsCollector_CollectorImplementation();
    _statistics = new Traits<ModelComponent>::StatisticsCollector_StatisticsImplementation(collector);
}

StatisticsCollector::StatisticsCollector(const StatisticsCollector& orig) : ModelElement(orig) {
}

StatisticsCollector::~StatisticsCollector() {
}

std::string StatisticsCollector::show() {
    std::string parentStr = "";
    if (_parent != nullptr) {
	try {
	    parentStr = _parent->getName();
	} catch (...) { // if parent changed or deleted, can cause seg fault
	    parentStr = "<<INCONSISTENT>>"; /*TODO ++*/
	}
    }
    return ModelElement::show() +
	    ",parent=\"" + parentStr + "\"" +
	    ",numElements=" + std::to_string(_statistics->numElements());
}

ModelElement* StatisticsCollector::getParent() const {
    return _parent;
}

Statistics_if* StatisticsCollector::getStatistics() const {
    return _statistics;
}

PluginInformation* StatisticsCollector::GetPluginInformation() {
    PluginInformation* info = new PluginInformation(Util::TypeOf<StatisticsCollector>(), &StatisticsCollector::LoadInstance);
    info->setGenerateReport(true);
    return info;
}

ModelElement* StatisticsCollector::LoadInstance(ElementManager* elems, std::map<std::string, std::string>* fields) {
    StatisticsCollector* newElement = new StatisticsCollector();
    try {
	newElement->_loadInstance(fields);
    } catch (const std::exception& e) {

    }
    return newElement;
}

bool StatisticsCollector::_loadInstance(std::map<std::string, std::string>* fields) {
    bool res = ModelElement::_loadInstance(fields);
    if (res) {
    }
    return res;
}

std::map<std::string, std::string>* StatisticsCollector::_saveInstance() {
    std::map<std::string, std::string>* fields = ModelElement::_saveInstance(); //Util::TypeOf<StatisticsCollector>());
    std::string parentId = "", parentTypename = "";
    if (this->_parent != nullptr) {
	parentId = std::to_string(_parent->getId());
	parentTypename = _parent->getTypename();
    }
    fields->emplace("parentTypename", parentTypename);
    fields->emplace("parentId", parentId);
    return fields;
}

bool StatisticsCollector::_check(std::string* errorMessage) {
    return true;
}