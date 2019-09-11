/*
 * File:   Dropoff.h
 * Author: bruno.hass.gabriel.muller.joao.meyer
 *
 * Created on 16 de Junho de 2019
 */

#ifndef DROPOFF_H
#define DROPOFF_H

#include "ModelComponent.h"

class Dropoff : public ModelComponent {
public:
    Dropoff(Model* model);
    Dropoff(const Dropoff& orig);
    virtual ~Dropoff();
    virtual std::string show();
    static PluginInformation* GetPluginInformation();
    static ModelComponent* LoadInstance(Model* model, std::map<std::string, std::string>* fields);

    void setDropoffSize(unsigned int size);
protected:
    virtual void _execute(Entity* entity);
    virtual void _initBetweenReplications();
    virtual bool _loadInstance(std::map<std::string, std::string>* fields);
    virtual std::map<std::string, std::string>* _saveInstance();
    virtual bool _check(std::string* errorMessage);

    unsigned int _dropoffSize = 0;
};

#endif /* DROPOFF_H */

