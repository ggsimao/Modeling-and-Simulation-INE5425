#ifndef LEAVE_H
#define LEAVE_H

#include "ModelComponent.h"
#include "Station.h"
#include "Queue.h"
#include "Resource.h"
#include "Seize.h"
#include "Delay.h"

class Leave : public ModelComponent {
public:
    Leave(Model* model);
    Leave(const Leave& orig);
    virtual ~Leave();
public:
    virtual std::string show();
public:
    static PluginInformation* GetPluginInformation();
    static ModelComponent* LoadInstance(Model* model, std::map<std::string, std::string>* fields);
    void setDelayExpression(std::string delayExpression, Util::TimeUnit delayTimeUnit);
    void setStation(Station* station);
    void setQueueResource(Queue* queue, Resource* resource);
    void connect(ModelComponent* directlyConnected);
    void connect();
protected:
    virtual void _execute(Entity* entity);
    virtual void _initBetweenReplications();
    virtual bool _loadInstance(std::map<std::string, std::string>* fields);
    virtual std::map<std::string, std::string>* _saveInstance();
    virtual bool _check(std::string* errorMessage);
private:
    std::string _delayExpression{"0"};
    Util::TimeUnit _delayTimeUnit{Util::TimeUnit::second};
    Station* _station{nullptr};
    Queue* _queue{nullptr};
    Resource* _resource{nullptr};
    Seize* _seize{nullptr};
    Delay* _delay{nullptr};
    bool _connected{false};
};

#endif /* LEAVE_H */
