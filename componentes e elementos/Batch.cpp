#include "Batch.h"

/*!
 * Constructor
 *
 * @param model is the object of the model executing.
 */
Batch::Batch(Model* model) : ModelComponent(model, Util::TypeOf<Batch>()) {
    this->_batchesCount = 0u;
    this->_batchSize = 1u;
    this->_permanent = false;
    this->_representativeEntityTypeName = "";

    initializeGroupAttribute();
}

Batch::Batch(const Batch& orig) : ModelComponent(orig) {}

Batch::~Batch() {}

/*!
 * Display information about the Batch component
 */
std::string Batch::show() {
    return ModelComponent::show() +
	    ",batchSize=" + std::to_string(static_cast<unsigned int> (this->_batchSize)) +
	    ",batchName=\"" + this->_batchName + "\"";
}

ModelComponent* Batch::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
    Batch* newComponent = new Batch(model);

    try {
      newComponent->_loadInstance(fields);
    } catch (const std::exception& e) { }

    return newComponent;
}

/*!
 *  Set the size of the batch to be created when grouping entities.
 */
void Batch::setBatchSize(unsigned int value) {
    this->_batchSize = value;
}

/*!
 *  Set the name of the batch to be created when grouping entities.
 */
void Batch::setBatchName(std::string value) {
    this->_batchName = value;
}

/*!
 *    Set the type of the batch to be created when grouping entities. Whether
 *  permanent or not
 */
void Batch::setPermanentBatch(bool value) {
    this->_permanent = value;
}

/*!
 *  Set the type of the batch to be created when grouping entities.
 */
void Batch::setByAttributeBatch(bool value) {
    this->_byAttribute = value;
}

/*!
 *  Set the name of the batch to be created when grouping entities.
 */
void Batch::setAttributeName(std::string value) {
    this->_attributeName = value;
}

/*!
 *  Set the entity type of the batch to be created when grouping entities.
 */
void Batch::setRepresentativeEntityTypeName(std::string value) {
    this->_representativeEntityTypeName = value;
}

/*!
 *  Execute the default behavior of the Batch component, which is group a set
 * of entities into a single one. Can be used as an abstraction to a group
 * of entities. It creates a temporary entity with a group reference
 */
void Batch::_execute(Entity* entity) {
    double currentTime = this->_model->getSimulation()->getSimulatedTime();
    double attributeValue = entity->getAttributeValue("Entity.AttributeValue");
    std::string batchEntityName = entity->getEntityTypeName() + "_Batch";
    std::string batchesCountStr = std::to_string(this->_batchesCount);
    std::string copyId = std::to_string(entity->getAttributeValue("Entity.CopyNumber"));

    if (this->_byAttribute)
    	attributeValue = entity->getAttributeValue(this->_attributeName);

    std::string batchEntityID = batchEntityName + batchesCountStr + std::to_string(attributeValue) + copyId;

    ElementManager* elementManager = this->_model->getElementManager();
    TraceManager* traceManager = this->_model->getTraceManager();

    traceManager->trace(Util::TraceLevel::blockInternal, "Arrival of entity " + std::to_string(entity->getId()) +
                                                         " at time " + std::to_string(currentTime));
    Group* group = (Group*) elementManager->getElement(Util::TypeOf<Group>(), batchEntityID);

    if (group == nullptr) {
        group = new Group(elementManager, batchEntityID);
        elementManager->insert(Util::TypeOf<Group>(), group);
    }

    group->insertElement(entity);

    unsigned int alreadyGrouped = group->size();

    if (alreadyGrouped == this->_batchSize) {
        traceManager->trace(Util::TraceLevel::blockInternal, "Release of batch entity " + std::to_string(entity->getId()) +
                                                             " at time " + std::to_string(currentTime));

        Entity* batchEntity = new Entity(elementManager);
        EntityType* batchEntityType = (EntityType*) elementManager->getElement(Util::TypeOf<EntityType>(), batchEntityName);

        if (batchEntityType == nullptr) {
            batchEntityType = new EntityType(elementManager, batchEntityName);
            elementManager->insert(Util::TypeOf<EntityType>(), batchEntityType);
        }

        batchEntity->setEntityType(batchEntityType);
        batchEntity->setAttributeValue("Entity.GroupRankID", (double)elementManager->getRankOf(Util::TypeOf<Group>(), batchEntityID));
        batchEntity->setAttributeValue("Entity.CopyNumber", 0);
        batchEntity->setAttributeValue("Entity.AttributeValue", (double)attributeValue);

        if (this->_permanent) { batchEntity->setAttributeValue("Entity.Permanent", 1.0); }

        elementManager->insert(Util::TypeOf<Entity>(), batchEntity);

        this->_model->sendEntityToComponent(batchEntity, this->getNextComponents()->front(), 0.0);
        this->_batchesCount++;
    }
}

/*!
 *  Load a previous instance of the Batch class stored.
 */
bool Batch::_loadInstance(std::map<std::string, std::string>* fields) {
    bool res = ModelComponent::_loadInstance(fields);

    if (res) {
      this->_batchSize = std::stoi((*(fields->find("batchSize"))).second);
      this->_batchName = (*(fields->find("batchName"))).second;
      this->_permanent = (*(fields->find("permanent"))).second == "true";
      this->_attributeName = (*(fields->find("attributeName"))).second;
      this->_representativeEntityTypeName = (*(fields->find("representativeType"))).second;
    }

    return res;
}

/*!
 *  Store current instance of Batch in a std::map, to be used as settings
 *  backup
 */
std::map<std::string, std::string>* Batch::_saveInstance() {
    std::map<std::string, std::string>* fields = ModelComponent::_saveInstance();

    fields->emplace("batchSize", std::to_string(this->_batchSize));
    fields->emplace("batchName", this->_batchName);
    fields->emplace("permanent", std::to_string(this->_permanent));
    fields->emplace("attributeName", this->_attributeName);
    fields->emplace("representativeType", this->_representativeEntityTypeName);

    return fields;
}

/*!
 *    Execute the clean up before each new replication. Simply resets the
 *  counter for already grouped entities and erase existing groups of
 *  entities from the model, if any
 */
void Batch::_initBetweenReplications() {
    this->_batchesCount = 0u;

    ElementManager* elementManager = this->_model->getElementManager();
    Group* existingGroup = (Group*) elementManager->getElement(Util::TypeOf<Group>(), this->_batchName);

    if (existingGroup != nullptr)
      elementManager->remove(Util::TypeOf<Group>(), existingGroup);
}

/*!
 *  Initialize internal batch specific attributes.
 */
void Batch::initializeGroupAttribute() {
    ElementManager* elementManager = this->_model->getElementManager();
    Attribute* attr = (Attribute*) elementManager->getElement(Util::TypeOf<Attribute>(), "Entity.GroupRankID");

    if (attr == nullptr)
        elementManager->insert(Util::TypeOf<Attribute>(), new Attribute("Entity.GroupRankID"));

    attr = (Attribute*) elementManager->getElement(Util::TypeOf<Attribute>(), "Entity.Permanent");
    if (attr == nullptr)
        elementManager->insert(Util::TypeOf<Attribute>(), new Attribute("Entity.Permanent"));

    attr = (Attribute*) elementManager->getElement(Util::TypeOf<Attribute>(), "Entity.CopyNumber");
    if (attr == nullptr)
        elementManager->insert(Util::TypeOf<Attribute>(), new Attribute("Entity.CopyNumber"));

    attr = (Attribute*) elementManager->getElement(Util::TypeOf<Attribute>(), "Entity.AttributeValue");
    if (attr == nullptr)
        elementManager->insert(Util::TypeOf<Attribute>(), new Attribute("Entity.AttributeValue"));
}

bool Batch::_check(std::string* errorMessage) {
    return ModelComponent::_check(errorMessage);
}

PluginInformation* Batch::GetPluginInformation(){
    return new PluginInformation(Util::TypeOf<Batch>(), &Batch::LoadInstance);
}
