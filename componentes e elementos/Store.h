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

#ifndef STORE_H
#define STORE_H

#include "ModelComponent.h"
#include "Storage.h"

class Store : public ModelComponent {
public:
    Store(Model* model);
    Store(const Store& orig);
    virtual ~Store();

public:
    virtual std::string show();

public:
    static PluginInformation* GetPluginInformation();
    static ModelComponent* LoadInstance(Model* model, std::map<std::string, std::string>* fields);
    void setStorageType(Storage::StorageType _storageType);
    Storage::StorageType getStorageType() const;
    void setStorageName(std::string storageName) throw();
    std::string getStorageName() const;
    void setAttributeName(std::string attributeName) throw();
    std::string getAttributeName() const;
    void setStorage(Storage* storage);
    Storage* getStorage() const;

protected:
    virtual void _execute(Entity* entity);
    virtual void _initBetweenReplications();
    virtual bool _loadInstance(std::map<std::string, std::string>* fields);
    virtual std::map<std::string, std::string>* _saveInstance();
    virtual bool _check(std::string* errorMessage);

private:
    Storage::StorageType _storageType;
    std::string _attributeName;
    Storage* _storage;
};

#endif /* STORE_H */
