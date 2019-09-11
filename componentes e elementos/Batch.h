#ifndef BATCH_H
#define BATCH_H

#include <string>

#include "ModelComponent.h"
#include "Model.h"
#include "Attribute.h"
#include "Group.h"

class Batch : public ModelComponent {
public:
    Batch(Model* model);
    Batch(const Batch& orig);
    virtual ~Batch();
public:
    virtual std::string show();
public:
    static PluginInformation* GetPluginInformation();
    static ModelComponent* LoadInstance(Model* model, std::map<std::string, std::string>* fields);

    void setBatchSize(unsigned int value);
    void setBatchName(std::string value);
    void setPermanentBatch(bool value);
    void setByAttributeBatch(bool value);
    void setAttributeName(std::string value);
    void setRepresentativeEntityTypeName(std::string value);
protected:
    virtual void _execute(Entity* entity);
    virtual void _initBetweenReplications();
    virtual bool _loadInstance(std::map<std::string, std::string>* fields);
    virtual std::map<std::string, std::string>* _saveInstance();
    virtual bool _check(std::string* errorMessage);
private:
    void initializeGroupAttribute();

    unsigned int _batchSize;
    unsigned int _batchesCount;
    std::string _batchName;
    bool _permanent;
    bool _byAttribute;
    std::string _attributeName;
    std::string _representativeEntityTypeName;
};

#endif /* BATCH_H */

