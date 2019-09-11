#ifndef ENTER_H
#define ENTER_H

#include "ModelComponent.h"
#include "Station.h"
#include "Resource.h"
#include "Release.h"
#include <string>

class Enter : public ModelComponent {
public:
    Enter(Model* model);
    Enter(const Enter& orig);
    virtual ~Enter();
public:
    virtual std::string show();
public:
    static PluginInformation* GetPluginInformation();
    static ModelComponent* LoadInstance(Model* model, std::map<std::string, std::string>* fields);
    void setStation(Station* station);
    void setResource(Resource* resource);
    void connect(ModelComponent* directlyConnected);
protected:
    virtual void _execute(Entity* entity);
    virtual void _initBetweenReplications();
    virtual bool _loadInstance(std::map<std::string, std::string>* fields);
    virtual std::map<std::string, std::string>* _saveInstance();
    virtual bool _check(std::string* errorMessage);
private:
    Station* _station{nullptr};
    Resource* _resource{nullptr};
    Release* _release{nullptr};
    bool _connected{false};
};

#endif /* ENTER_H */
