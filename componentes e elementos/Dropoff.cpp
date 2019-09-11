/*
 * File:   Dropoff.cpp
 * Author: bruno.hass.gabriel.muller.joao.meyer
 *
 * Created on 16 de Junho de 2019
 */

#include "Model.h"
#include "Group.h"
#include "Dropoff.h"


/**
 * Base constructor
 */
Dropoff::Dropoff(Model* model) : ModelComponent(model, Util::TypeOf<Dropoff>())
{
    // Nothing
}

/**
 * Copy constructor
 */
Dropoff::Dropoff(const Dropoff& drop) : ModelComponent(drop)
{
    // Nothing
}

/**
 * Destructor
 */
Dropoff::~Dropoff()
{
    // Nothing
}

/**
 * Returns some info, as a string of this component.
 *
 * It will return the size of the dropoff component.
 *
 * @return String with the info.
 */
std::string Dropoff::show()
{
    std::string result = ModelComponent::show();
    result += ",dropoffSize=" + std::to_string(this->_dropoffSize);
    return ModelComponent::show() + result;
}

ModelComponent* Dropoff::LoadInstance(
    Model* model,
    std::map<std::string, std::string>* fields)
{
    Dropoff* newComponent = new Dropoff(model);
    try
    {
        newComponent->_loadInstance(fields);
    }
    catch (const std::exception& e)
    {

    }
    return newComponent;
}

PluginInformation* Dropoff::GetPluginInformation()
{
    return new PluginInformation(
        Util::TypeOf<Dropoff>(),
        &Dropoff::LoadInstance);
}


/**
 * Setter for size of dropoff.
 *
 * @param size A positive int.
 */
void Dropoff::setDropoffSize(unsigned int size)
{
    this->_dropoffSize = size;
}

/**
 * Execution method
 *
 * This method performs some things. It assumes that there are elements already
 * in the respectively named group. It will remove the first `_dropoffSize`
 * elements of the entity's group and send them away at once. In the end, it
 * will send the received entity.
 *
 * In other words, it will output the splitted elements of the group first.
 * Only after sending them to the next component that it will send the received
 * entity.
 *
 * @param entity Entity to apply execution to
 */
void Dropoff::_execute(Entity* entity)
{
    ElementManager* elementManager = this->_model->getElementManager();

    // Getting group from entity
    std::string groupType = Util::TypeOf<Group>();
    Util::identitifcation groupId = entity->getAttributeValue("Entity.Group");
    Group* group = (Group*) elementManager->getElement(groupType, groupId);



    // Dropoff
    for (unsigned int i = 0; i < this->_dropoffSize; i++)
    {
        // Remove from group
        Entity* groupEntity = group->first();
        group->removeElement(groupEntity);

        // Send it away
        ModelComponent* nextComponent = this->getNextComponents()->front();
        this->_model->sendEntityToComponent(groupEntity, nextComponent, 0.0);
    }

    TraceManager* trace = this->_model->getTraceManager();
    trace->trace(Util::TraceLevel::simulation, "Nice");

    // Send rest of group away, if any
    if (group->size() > 0)
    {
        ModelComponent* nextComponent = this->getNextComponents()->front();
        this->_model->sendEntityToComponent(entity, nextComponent, 0.0);
    }
}

void Dropoff::_initBetweenReplications()
{
    // Nothing
}

/**
 * Loads dropoff size.
 */
bool Dropoff::_loadInstance(std::map<std::string, std::string>* fields)
{
    unsigned int dropoffSize = (unsigned int) std::stoi(fields->at("DropoffSize"));
    this->_dropoffSize = dropoffSize;
    return ModelComponent::_loadInstance(fields);
}

/**
 * Saves dropoff size.
 */
std::map<std::string, std::string>* Dropoff::_saveInstance() {
    std::map<std::string, std::string>* fields = ModelComponent::_saveInstance();

    std::string dropoffSizeStr = std::to_string(this->_dropoffSize);
    std::pair<std::string, std::string> dropoffSize("DropoffSize", dropoffSizeStr);
    fields->insert(dropoffSize);

    return fields;
}

bool Dropoff::_check(std::string* errorMessage) {
    return ModelComponent::_check(errorMessage);
}
