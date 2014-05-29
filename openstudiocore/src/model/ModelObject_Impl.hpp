/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_MODELOBJECT_IMPL_HPP
#define MODEL_MODELOBJECT_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp" // required for Q_PROPERTY

#include "../utilities/idf/WorkspaceObject_Impl.hpp"

#include "../utilities/core/Optional.hpp"

#include <QObject>
#include <QVariant>
#include <QStringList>

namespace openstudio {

class Attribute;
class TimeSeries;

namespace model {

class Model;
class Component;
class LifeCycleCosts;

class Relationship;
class FieldViewer;

class ParentObject;
class ResourceObject;

class OutputVariable;
class Meter;
class Connection;

namespace detail {

  class Model_Impl;

  /** Implemetation class for ModelObject. Each class deriving from ModelObject
   *  should provide an implementation class that derives from ModelObject_Impl. */
  class MODEL_API ModelObject_Impl : public openstudio::detail::WorkspaceObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(openstudio::IddObjectType iddObjectType READ iddObjectType);
    Q_PROPERTY(openstudio::UUID handle READ handle);
    Q_PROPERTY(boost::optional<std::string> name READ name WRITE mf_setName STORED true);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> parent READ parentAsModelObject WRITE setParentAsModelObject);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> resources READ resourcesAsModelObjects);
   public:

    /** @name Constructors and Destructors */
    //@{

    /** Copy constructor for use by Model::Model(const Workspace&). Clones all
     *  WorkspaceObject_Impl data and hooks new ModelObject_Impl up to model. */
    ModelObject_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    /** Copy constructor. Assigns new handle, keeps same workspace pointer. Workspace should know
     *  whether copy is to live in original workspace, or in cloned workspace. If the latter,
     *  Workspace will need to update the workspace pointer, and any source and target pointer
     *  handles. */
    ModelObject_Impl(const ModelObject_Impl& other, Model_Impl* model, bool keepHandle);

    /** Creates a new ModelObject whose underlying data is identical to *this, except that a
     *  new handle is assigned, and it is placed in model. */
    virtual ModelObject clone(Model model) const;

    virtual ~ModelObject_Impl() {}

    /// remove the object from the model, also removes any cost objects associated with this object
    /// return std::vector<IdfObject> containing any removed object(s)
    virtual std::vector<IdfObject> remove();

    //@}
    /** @name Components and Relationships
     *
     *  Once a Component has been created, it can be saved to a ProjectDatabase for local storing
     *  and sharing, and can be uploaded to the online Building Component Library for web-enabled
     *  storing and sharing. Components plus Relationships are the preferred method for specifying
     *  and inserting alternative pieces of a model (constructions, schedules, etc.) in higher-level
     *  libraries such as standardsinterface and analysis. */
    //@{

    /** Method with virtual implementation for creating sharable Model snippets. Creates a component
     *  with this ModelObject as the primary object. Clones all data. */
    virtual Component createComponent() const;

    /** Return all valid relationship names. */
    std::vector<std::string> relationshipNames() const;

    /** Return all \link Relationship relationships\endlink for this ModelObject. */
    std::vector<Relationship> relationships() const;

    /** Get the \link Relationship relationship\endlink named name, if it exists. */
    boost::optional<Relationship> getRelationship(const std::string& name) const;

    bool setRelationship(const std::string& name, boost::optional<ModelObject> relatedModelObject);

    bool setRelationship(const std::string& name, const Component& component);

    //@}
    /** @name Attributes
     *
     *  String-based getters and setters for commonly used attributes, such as a zone's lighting
     *  power density or a fan's efficiency. Attributes are the preferred method for accessing
     *  basic data (double, int, bool, and string) in higher-level libraries such as
     *  standardsinterface and analysis. */
    //@{

    /** Return all valid attribute names. */
    std::vector<std::string> attributeNames() const;

    /** Return all \link Attribute Attributes\endlink for this ModelObject. */
    std::vector<openstudio::Attribute> attributes() const;

    /** Get the attribute named name, if it exists. */
    boost::optional<openstudio::Attribute> getAttribute(const std::string& name) const;

    /** Is the named attribute settable. */
    bool isSettableAttribute(const std::string& name) const;

    /** Is the named attribute optional. */
    bool isOptionalAttribute(const std::string& name) const;

    /** Set the attribute named name, if it exists. */
    bool setAttribute(const std::string& name, bool value);

    /** \overload */
    bool setAttribute(const std::string& name, int value);

    /** \overload */
    bool setAttribute(const std::string& name, unsigned value);

    /** \overload */
    bool setAttribute(const std::string& name, double value);

    /** \overload */
    bool setAttribute(const std::string& name, const Quantity& value);

    /** \overload */
    bool setAttribute(const std::string& name, const std::string& value);

    /** \overload */
    bool setAttribute(const std::string& name, const char* value);

    /** \overload */
    bool setAttribute(const std::string& name, const QVariant& value);

    /** Reset the attribute attribute, e.g. for optional types. */
    bool resetAttribute(const std::string& name);

    //@}
    /** @name Getters */
    //@{

    /** Returns the Model that contains this object. */
    Model model() const;

    /// return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    /** Get the resources directly used by this ModelObject. */
    virtual std::vector<ResourceObject> resources() const;

    /** Get all output variables names that could be associated with this object. These variables
     *  may or may not be available for each simulation, need to check report variable dictionary
     *  to see if the variable is available. Each concrete class should override this method.*/
    virtual const std::vector<std::string>& outputVariableNames() const = 0;

    /** Get all output variables associated with this object, must run simulation to generate data. */
    virtual std::vector<OutputVariable> outputVariables() const;

    /** Get data associated with this output variable and this object. */
    virtual boost::optional<openstudio::TimeSeries> getData(const OutputVariable& variable,
                                                            const std::string& envPeriod) const;

    std::vector<LifeCycleCost> lifeCycleCosts() const;

    std::vector<IdfObject> removeLifeCycleCosts();

    /// REIMPLEMENT IN ALL CONCRETE MODEL OBJECTS (AND NOT IN ABSTRACT BASE CLASSES).
    virtual IddObjectType iddObjectType() const=0;

    //@}
    /** @name Setters */
    //@{

    /// set the parent, child may have to call non-const methods on the parent
    virtual bool setParent(ParentObject& newParent);

    //@}
    /** @name Queries */
    //@{

    /** Return the ScheduleTypeKeys indicating how schedule is used in this object. If schedule is not directly
     *  used by this object, return value will be .empty(). 
     *
     *  REIMPLEMENT IN ALL CONCRETE MODELOBJECTS THAT CAN POINT TO SCHEDULES. */
    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name HVAC System Connections */
    //@{

    boost::optional<ModelObject> connectedObject(unsigned port) const;

    boost::optional<unsigned> connectedObjectPort(unsigned port) const;

    //@}
   public slots:

    void requestProperties(const QStringList& names);

    void setProperties(const QVariantMap& properties);

   signals:

    void reportProperties(const QVariantMap& properties);

   protected:

    ModelObject_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ModelObject_Impl(IddObjectType type, Model_Impl* model);

    /** Returns a bool for field index. Throws if unable to deduce a value. Should only
     *  be used for defaulted or required boolean fields. */
    bool getBooleanFieldValue(unsigned index) const;

    /** Set a bool for field index. Throws if unsuccessful. Should only be used for defaulted
     *  or required boolean fields. */
    void setBooleanFieldValue(unsigned index, bool value);

    /** This is solely to prevent implicit conversion of types to bool. This will cause a link time
     *  error if anything other than a bool is used for value. This has no implementation. */
    template <class T>
    void setBooleanFieldValue(unsigned index, const T& value);

    /** Sets index to point to schedule if schedule's ScheduleTypeLimits are compatible with the 
     *  ScheduleType in the ScheduleTypeRegistry for (className,scheduleDisplayName), or if 
     *  schedule's ScheduleTypeLimits have not yet been set (in which case the ScheduleTypeRegistry
     *  is used to retrieve or create an appropriate one). */
    bool setSchedule(unsigned index,
                     const std::string& className,
                     const std::string& scheduleDisplayName,
                     Schedule& schedule);

    virtual void populateValidityReport(ValidityReport& report,bool checkNames) const;

   private:

    REGISTER_LOGGER("openstudio.model.ModelObject");

    // vector of connections to other ModelObject "ports"
    std::vector<std::shared_ptr<Connection> > m_connectionVector;

    boost::optional<ModelObject> parentAsModelObject() const;

    std::vector<ModelObject> resourcesAsModelObjects() const;

    bool setParentAsModelObject(boost::optional<ModelObject> modelObject);

    // set name from optional string for name property
    bool mf_setName(const boost::optional<std::string>& name);

    // get specific key value from optional key value which might be '*'
    boost::optional<std::string> getSpecificKeyValue(OptionalString keyValue) const;
  };

  typedef std::shared_ptr<ModelObject_Impl> ModelObject_ImplPtr;

} // detail
} // model
} // openstudio

#endif

