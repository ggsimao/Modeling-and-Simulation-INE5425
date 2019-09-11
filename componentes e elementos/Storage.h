/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Storage.h
 * Author: ...
 *
 * Created on ...
 */

#ifndef STORAGE_H
#define STORAGE_H

#include "ElementManager.h"
#include "Entity.h"
#include "List.h"
#include "ModelElement.h"
#include "Plugin.h"
#include "StatisticsCollector.h"
#include "Waiting.h"

class Storage : public ModelElement {
public:
    enum class StorageType : int {
        STORAGE = 1,
        SET = 2,
        ATTRIBUTE = 3,
        EXPRESSION = 4,
    };

public:
    Storage(ElementManager* elems);
    Storage(ElementManager* elems, std::string name);
    Storage(const Storage& orig);
    virtual ~Storage();

public:
    virtual std::string show();

public:
    static PluginInformation* GetPluginInformation();
    static ModelElement* LoadInstance(ElementManager* elems, std::map<std::string, std::string>* fields);

public:
    void insertElement();
    void removeElement();
    void initBetweenReplication();
    long int count();
    void setAttributeName(std::string _attributeName);
    std::string getAttributeName() const;

public: //g&s
protected:
    virtual bool _loadInstance(std::map<std::string, std::string>* fields);
    virtual std::map<std::string, std::string>* _saveInstance();
    virtual bool _check(std::string* errorMessage);

private:
    ElementManager* _elems;
    void _initCStats();

private:
    long int _count;

private: //1::1
    StatisticsCollector* _cstatNumberInStorage; // = new StatisticsCollector("Number In Storage");
    std::string _attributeName = "";
};

#endif /* STORAGE_H */
