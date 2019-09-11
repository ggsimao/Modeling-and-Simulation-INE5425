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

#ifndef UNSTORE_H
#define UNSTORE_H

#include "ModelComponent.h"
#include "Storage.h"

class Unstore : public ModelComponent {
public:
    Unstore(Model* model);
    Unstore(const Unstore& orig);
    virtual ~Unstore();

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

#endif /* UNSTORE_H */
