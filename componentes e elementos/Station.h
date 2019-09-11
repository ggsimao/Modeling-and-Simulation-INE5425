
#ifndef STATION_H
#define STATION_H

#include "ModelElement.h"
#include "ElementManager.h"
#include "Plugin.h"
#include "StatisticsCollector.h"
#include "Model.h"
#include "ModelComponent.h"

class Station : public ModelElement {
public:
    Station(ElementManager* elems);
    Station(ElementManager* elems, std::string name);
    Station(const Station& orig);
    virtual ~Station();
public:
    virtual std::string show();
public:
    static PluginInformation* GetPluginInformation();
    static ModelElement* LoadInstance(ElementManager* elems, std::map<std::string, std::string>* fields);
    void setFirstComponent(ModelComponent* component);
    ModelComponent* getFirstComponent();
protected:
    virtual bool _loadInstance(std::map<std::string, std::string>* fields);
    virtual std::map<std::string, std::string>* _saveInstance();
    virtual bool _check(std::string* errorMessage);
private:
    ElementManager* _elems;
    ModelComponent* _firstComponent{nullptr};
};

#endif /* STATION_H */

