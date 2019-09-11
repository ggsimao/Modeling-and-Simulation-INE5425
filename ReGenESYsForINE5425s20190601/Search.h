/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Search.h
 * Author: rlcancian
 *
 * Created on 03 de Junho de 2019, 15:20
 */

#ifndef SEARCH_H
#define SEARCH_H

#include "ModelComponent.h"
#include "Model.h"

class Search : public ModelComponent {
public:
    Search(Model* model);
    Search(const Search& orig);
    virtual ~Search();
public:
    virtual std::string show();
public:
    static PluginInformation* GetPluginInformation();
    static ModelComponent* LoadInstance(Model* model, std::map<std::string, std::string>* fields);
public: // g & s
    // Queue* getQueue();
    // ModelComponent* getNextModule();
    // int getRank();
protected:
    virtual void _execute(Entity* entity);
    virtual bool _loadInstance(std::map<std::string, std::string>* fields);
    virtual void _initBetweenReplications();
    virtual std::map<std::string, std::string>* _saveInstance();
    virtual bool _check(std::string* errorMessage);
private:
    // Queue* _queue;
    // ModelComponent* _nextModule;
    // int _rank;

};

#endif /* SEARCH_H */

