/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_MODELOBJECT_IMPL_HPP
#define MODEL_MODELOBJECT_IMPL_HPP

#include "ModelAPI.hpp"
#include "../utilities/idf/WorkspaceObject_Impl.hpp"

#include "../nano/nano_signal_slot.hpp"  // Signal-Slot replacement

#include "../utilities/core/Optional.hpp"

namespace openstudio {

class TimeSeries;

namespace model {

  class Model;
  class ModelObject;

  class Component;
  class LifeCycleCost;

  class FieldViewer;

  class ParentObject;
  class ResourceObject;

  class Schedule;

  class OutputVariable;
  class Meter;
  class Connection;

  class EMSActuatorNames;

  class AdditionalProperties;

  class ScheduleTypeKey;

  namespace detail {

    class Model_Impl;

    /** Implementation class for ModelObject. Each class deriving from ModelObject
   *  should provide an implementation class that derives from ModelObject_Impl. */
    class MODEL_API ModelObject_Impl : public openstudio::detail::WorkspaceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      /** Copy constructor for use by Model::Model(const Workspace&). Clones all
     *  WorkspaceObject_Impl data and hooks new ModelObject_Impl up to model. */
      ModelObject_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      /** Copy constructor. Assigns new handle, keeps same workspace pointer. Workspace should know
     *  whether copy is to live in original workspace, or in cloned workspace. If the latter,
     *  Workspace will need to update the workspace pointer, and any source and target pointer
     *  handles. */
      ModelObject_Impl(const ModelObject_Impl& other, Model_Impl* model, bool keepHandle);

      /** Creates a new ModelObject whose underlying data is identical to *this, except that a
     *  new handle is assigned, and it is placed in model. */
      virtual ModelObject clone(Model model) const;

      virtual ~ModelObject_Impl() = default;

      /// remove the object from the model, also removes any cost objects associated with this object
      /// return std::vector<IdfObject> containing any removed object(s)
      virtual std::vector<IdfObject> remove() override;

      //@}
      /** @name Components
     *
     *  Once a Component has been created, it can be saved to a ProjectDatabase for local storing
     *  and sharing, and can be uploaded to the online Building Component Library for web-enabled
     *  storing and sharing. Components are the preferred method for specifying
     *  and inserting alternative pieces of a model (constructions, schedules, etc.) in higher-level
     *  libraries such as standardsinterface and analysis. */
      //@{

      /** Method with virtual implementation for creating sharable Model snippets. Creates a component
     *  with this ModelObject as the primary object. Clones all data. */
      virtual Component createComponent() const;

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
      virtual boost::optional<openstudio::TimeSeries> getData(const OutputVariable& variable, const std::string& envPeriod) const;

      std::vector<LifeCycleCost> lifeCycleCosts() const;

      std::vector<IdfObject> removeLifeCycleCosts();

      /// REIMPLEMENT IN ALL CONCRETE MODEL OBJECTS (AND NOT IN ABSTRACT BASE CLASSES).
      virtual IddObjectType iddObjectType() const = 0;

      /** Returns this object's additional properties, constructing a new object if necessary. */
      AdditionalProperties additionalProperties() const;

      bool hasAdditionalProperties() const;

      /** Removes all additional properties that refer to this object. Returns removed objects. */
      std::vector<IdfObject> removeAdditionalProperties();

      boost::optional<std::string> cadObjectId() const;

      bool setCADObjectId(const std::string& cadObjectId);

      boost::optional<std::string> gbXMLId() const;

      bool setGBXMLId(const std::string& gbXMLId);

      boost::optional<std::string> displayName() const;

      bool setDisplayName(const std::string& displayName);

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

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const;

      virtual std::vector<std::string> emsInternalVariableNames() const;

      //@}
      /** @name HVAC System Connections */
      //@{

      boost::optional<ModelObject> connectedObject(unsigned port) const;

      boost::optional<unsigned> connectedObjectPort(unsigned port) const;

      //@}
      /** @name Nano Signals */
      //@{

      //@}

      /** Gets the autosized component value from the sql file **/
      boost::optional<double> getAutosizedValue(const std::string& valueName, const std::string& units, std::string overrideCompType = "") const;

     protected:
      ModelObject_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ModelObject_Impl(IddObjectType type, Model_Impl* model);

      /** Returns a bool for field index. Throws if unable to deduce a value. Should only
     *  be used for defaulted or required boolean fields. */
      bool getBooleanFieldValue(unsigned index) const;

      /** Set a bool for field index. Throws if unsuccessful (and returns false). Should only be used for defaulted
     *  or required boolean fields. */
      bool setBooleanFieldValue(unsigned index, bool value);

      /** This is solely to prevent implicit conversion of types to bool. This will cause a link time
     *  error if anything other than a bool is used for value. This has no implementation. */
      template <class T>
      bool setBooleanFieldValue(unsigned index, const T& value);

      /** Sets index to point to schedule if schedule's ScheduleTypeLimits are compatible with the
       *  ScheduleType in the ScheduleTypeRegistry for (className,scheduleDisplayName), or if
       *  schedule's ScheduleTypeLimits have not yet been set (in which case the ScheduleTypeRegistry
       *  is used to retrieve or create an appropriate one). */
      bool setSchedule(unsigned index, const std::string& className, const std::string& scheduleDisplayName, Schedule& schedule);

      /** For stuff that's plain missing from ComponentSizes table in E+, so getAutosizedValue can't work.
        * This method is slower as it executes a lot more queries (an average of 10, versus 1 for getAutosizedValue) */
      boost::optional<double> getAutosizedValueFromInitializationSummary(const std::string& valueName, const std::string& units) const;

     private:
      REGISTER_LOGGER("openstudio.model.ModelObject");

      // vector of connections to other ModelObject "ports"
      std::vector<std::shared_ptr<Connection>> m_connectionVector;

      boost::optional<ModelObject> parentAsModelObject() const;

      std::vector<ModelObject> resourcesAsModelObjects() const;

      bool setParentAsModelObject(boost::optional<ModelObject> modelObject);

      // set name from optional string for name property
      bool mf_setName(const boost::optional<std::string>& name);

      // get specific key value from optional key value which might be '*'
      boost::optional<std::string> getSpecificKeyValue(OptionalString keyValue) const;
    };

    using ModelObject_ImplPtr = std::shared_ptr<ModelObject_Impl>;

  }  // namespace detail
}  // namespace model
}  // namespace openstudio

#endif
