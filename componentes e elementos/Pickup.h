/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Pickup.h
 * Authors: leonardo.schluter.leite,
 *          gustavo.tarciso,
 *          alexandre.behling
 * Created on 22 de Maio de 2019, 18:41
 */


#ifndef PICKUP_H
#define DUPICKUP_HMY_H

#include "ModelComponent.h"
#include "Queue.h"

 //! Pickup class
 /*!
    \brief This class implements the Pickup block.
           The pickup block removes a certain amount of consecutive entities from a queue, from a given position.
           The amount of entities must not exceed the total queue size.
           The entities removed from the queue are added into a group specified by the incoming entity
 */

class Pickup : public ModelComponent {
public:

    //! \fn Pickup(Model* model);
    /*!
        \param model is a Model used as parameter to the super class ModelComponent constructor
        \brief This constructor creates a instance of ModelComponent of type Pickup given a model.
               The model parameter is the model this block belongs to
    */
    Pickup(Model* model);

    //! \fn Pickup(const Pickup& orig);
    /*!
        \param orig reference to another Pickup
        \brief The class constructor, creates a cloned object
    */
    Pickup(const Pickup& orig);

    //! \fn virtual ~Pickup();
    /*!
        \brief The class destructor
    */
    virtual ~Pickup();
public:

    //! \fn virtual std::string show();
    /*!
        \return a string showing information from the component,
        \brief This function invokes show() function from ModelComponent, given the information from the component,
               the starting position of the queue to start removing and the amount of elements to be removed
    */
    virtual std::string show();
public:

    //! \fn static PluginInformation* GetPluginInformation()
    /*!
        \return a PluginInformation object with the information of what components are conected to
        \brief this function get what components are conected to this pickup
    */
    static PluginInformation* GetPluginInformation();

    //! \fn static ModelComponent* LoadInstance(Model* model, std::map<std::string, std::string>* fields)
    /*!
        Load context saved in a previous moment
    */
    static ModelComponent* LoadInstance(Model* model, std::map<std::string, std::string>* fields);

    //! \fn void setQueueName(std::string queueName)
    /*!
        \param queueName the name of the queue to be taken
        \brief set the component queue to a entity queue, searched in model's ElementManager
    */
    void setQueueName(std::string queueName) throw();

    //! \fn Queue* getQueue()
    /*!
        \return the component queue
        \brief Give the component queue
    */
    Queue* getQueue() const;

    //! \fn std::string getQuantity() 
    /*!
        \return a string representing the amount of elements to be removed from the class queue
        \brief Give the amount of elements to be removed from the class queue
    */
    std::string getQuantity() const;

    //! \fn void setStartingRank(std::string startingRank)
    /*!
        \param a string with the starting rank
        \brief sets the starting rank, it means that this method sets which element from the queue will be the starting point
               to the elements removal.
    */
    void setStartingRank(std::string startingRank);

    //! \fn std::string getStartingRank()
    /*!
        \return a string representing the starting point of the elements removal in the queue
        \brief Give the starting point of the elements removal in the queue
    */
    std::string getStartingRank() const;

    //! \fn void setQuantity(std::string quantity)
    /*!
        \paran a string representing the quantity of elements to remove fom queue, setted in the class atribute _quantity 
        \brief Give the elements amount to be removed from the queue
    */
    void setQuantity(std::string quantity);

protected:
    virtual void _execute(Entity* entity);
    virtual void _initBetweenReplications();
    virtual bool _loadInstance(std::map<std::string, std::string>* fields);
    virtual std::map<std::string, std::string>* _saveInstance();
    virtual bool _check(std::string* errorMessage);
private:
    Queue* _queue; /*!< Entity queue used to group the entities>*/
    std::string _quantity = "1"; /*!<Quantity of elements to remove from queue */
    std::string _startingRank = "1"; /*!<Position of the first element to be removed from the queue */
};

#endif /* PICKUP_H */