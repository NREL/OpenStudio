/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_MODEL_HPP
#define MODEL_MODEL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

#include "../utilities/idf/Workspace.hpp"
#include "../utilities/filetypes/WorkflowJSON.hpp"
#include "../utilities/core/Assert.hpp"

#include <vector>

namespace openstudio {

class SqlFile;
class Date;
class MonthOfYear;
class DayOfWeek;
class NthDayOfWeekInMonth;

namespace model {

class Building;
class LifeCycleCostParameters;
class RunPeriod;
class YearDescription;
class WeatherFile;
class Component;
class ComponentData;
class Schedule;
class Node;
class SpaceType;
class FoundationKivaSettings;

namespace detail {
  class Model_Impl;
  class ModelObject_Impl;
}

/** Model derives from Workspace and is a container for \link ModelObject ModelObjects
 *  \endlink as defined by the OpenStudio IDD. The OpenStudio Model is primarily a container for
 *  \link ModelObject ModelObjects \endlink which together define a complete or partial model of a
 *  building for energy simulation. A ModelObject cannot exist outside of a Model which ensures that
 *  each ModelObject can access the full Model to search for related ModelObjects. Model also accepts
 *  a connection to EnergyPlus simulation output through the SqlFile access to the SQLite output of
 *  EnergyPlus. Each ModelObject may then access simulation output in its own context. Connection to
 *  simulation data from other simulation engines is planned for the future.
 *
 *  The OpenStudio Model input data model is based on the schema defined by the OpenStudio IDD.
 *  Each concrete ModelObject wraps a particular data type in the OpenStudio IDD and Model enforces
 *  collection level constraints such as preventing multiple definitions of unique objects, updating
 *  reference fields that point from one ModelObject to another when the target's name changes, etc.
 *  Most of \link Model Model's \endlink capabilities for managing data according to an IDD schema
 *  are implemented in the Workspace base class. See the Workspace documentation in the utilities
 *  sub-project for more information about Workspace. */
class MODEL_API Model : public openstudio::Workspace {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Creates a new, empty Model. */
  Model();

  /** Creates a new Model with one ModelObject for each IdfObject in the given IdfFile.
   *  Any unwrapped IDD types will be wrapped with GenericModelObject. */
  explicit Model(const openstudio::IdfFile& idfFile);

  /** Creates a new Model with one ModelObject for each WorkspaceObjects in the given Workspace.
   *  Any unwrapped IDD types will be wrapped with GenericModelObject. */
  explicit Model(const openstudio::Workspace& workspace);

  virtual ~Model() {}

  //@}
  /** @name Getters */
  //@{

  /// Get the WorkflowJSON
  WorkflowJSON workflowJSON() const;

  /** Returns the EnergyPlus output SqlFile if set. */
  boost::optional<SqlFile> sqlFile() const;

  /** Get the Building object if there is one, this implementation uses a cached reference to the Building
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<Building>(). */
  boost::optional<Building> building() const;
  
  /** Get the FoundationKivaSettings object if there is one, this implementation uses a cached reference to the FoundationKivaSettings
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<FoundationKivaSettings>(). */
  boost::optional<FoundationKivaSettings> foundationKivaSettings() const;  

  /** Get the LifeCycleCostParameters object if there is one, this implementation uses a cached reference to the LifeCycleCostParameters
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<LifeCycleCostParameters>(). */
  boost::optional<LifeCycleCostParameters> lifeCycleCostParameters() const;

  /** Get the RunPeriod object if there is one, this implementation uses a cached reference to the RunPeriod
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<RunPeriod>(). */
  boost::optional<RunPeriod> runPeriod() const;

  /** Get the YearDescription object if there is one, this implementation uses a cached reference to the YearDescription
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<YearDescription>(). */
  boost::optional<YearDescription> yearDescription() const;

  /** Get or create the YearDescription object if there is one, then call method from YearDescription. */
  // DLM: this is due to issues exporting the model::YearDescription object because of name conflict with utilities::YearDescription.
  boost::optional<int> calendarYear() const;
  std::string dayofWeekforStartDay() const;
  bool isDayofWeekforStartDayDefaulted() const;
  bool isLeapYear() const;
  bool isIsLeapYearDefaulted() const;
  bool setCalendarYear(int calendarYear);
  void resetCalendarYear();
  bool setDayofWeekforStartDay(std::string dayofWeekforStartDay);
  void resetDayofWeekforStartDay();
  bool setIsLeapYear(bool isLeapYear);
  void resetIsLeapYear();
  int assumedYear();
  openstudio::Date makeDate(openstudio::MonthOfYear monthOfYear, unsigned dayOfMonth);
  openstudio::Date makeDate(unsigned monthOfYear, unsigned dayOfMonth);
  openstudio::Date makeDate(openstudio::NthDayOfWeekInMonth n, openstudio::DayOfWeek dayOfWeek, openstudio::MonthOfYear monthOfYear);
  openstudio::Date makeDate(unsigned dayOfYear);

  /** Get the WeatherFile object if there is one, this implementation uses a cached reference to the WeatherFile
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<WeatherFile>(). */
  boost::optional<WeatherFile> weatherFile() const;

  /** Get an always on schedule with discrete type limits if there is one.
   *  create a new schedule if necessary and add it to the model */
  Schedule alwaysOnDiscreteSchedule() const;

  /** Get the always on schedule with discrete type limits name. */
  std::string alwaysOnDiscreteScheduleName() const;

  /** Get an always off schedule with discrete type limits if there is one.
   *  create a new schedule if necessary and add it to the model */
  Schedule alwaysOffDiscreteSchedule() const;

  /** Get the always off schedule with discrete type limits name. */
  std::string alwaysOffDiscreteScheduleName() const;

  /** Get an always on schedule with continuous type limits if there is one.
  *  create a new schedule if necessary and add it to the model */
  Schedule alwaysOnContinuousSchedule() const;

  /** Get the always on schedule with continuous type limits name.*/
  std::string alwaysOnContinuousScheduleName() const;

  /** Get a Node named 'Model Outdoor Air Node' (intended to be forward translated to an OutdoorAir:Node) and not connected to a PlantLoop or AirLoopHVAC.
  *  create a new Node if necessary and add it to the model */
  Node outdoorAirNode() const;

  /** Get the space type used for plenums if there is one.
   *  Create a new space type if necessary and add it to the model */
  SpaceType plenumSpaceType() const;

  /** Get the space type name used for plenums. */
  std::string plenumSpaceTypeName() const;

  //@}
  /** @name Setters */
  //@{

  /**  Set the WorkflowJSON. */
  bool setWorkflowJSON(const WorkflowJSON& workflowJSON);

  /** Reset the WorkflowJSON. */
  void resetWorkflowJSON();

  /** Sets the EnergyPlus output SqlFile.  SqlFile must correspond to EnergyPlus
   *  simulation of this Model. */
  bool setSqlFile(const SqlFile& sqlFile);

  /** Resets the EnergyPlus output SqlFile. */
  bool resetSqlFile();

  //@}
  /** @name Template Methods */
  //@{

  /** Returns the ModelObject referenced by handle if it is of type T. This method can be used with
   *  T as a concrete type (e.g. Zone) or as an abstract class (e.g. ParentObject).
   *
   *  \todo Use of this template method requires knowledge of the size of the implementation object.
   *  Therefore, to use model.getModelObject<Zone>(handle) the user must include both Zone.hpp and
   *  Zone_Impl.hpp.  It may be better to instantiate each version of this template method to avoid
   *  exposing the implementation objects, this is an open question. */
  template <typename T>
  boost::optional<T> getModelObject(const Handle& handle) const {
    boost::optional<T> result;
    boost::optional<WorkspaceObject> wo = this->getObject(handle);
    if (wo) {
      std::shared_ptr<typename T::ImplType> p = wo->getImpl<typename T::ImplType>();
      if (p){ result = T(p); }
    }
    return result;
  }

  /** Returns the unique ModelObject of type T, creates a one if none are found.
   *
   *  \todo Use of this template method requires knowledge of the size of the implementation object.
   *  Therefore, to use model.getUniqueModelObject<Facility>() the user must include both
   *  Facility.hpp and Facility_Impl.hpp. It may be better to instantiate each version of this
   *  template method to avoid exposing the implementation objects, this is an open question. */
  template <typename T>
  T getUniqueModelObject() {
    std::vector<WorkspaceObject> objects = this->allObjects();
    std::shared_ptr<typename T::ImplType> p;
    for(std::vector<WorkspaceObject>::const_iterator it = objects.begin(), itend = objects.end(); it < itend; ++it){
      p = it->getImpl<typename T::ImplType>();
      if (p){
        return T(p);
      }
    }
    return T(*this); // make a new T
  }

  /** Returns the unique ModelObject of type T if it is found.
   *
   *  \todo Use of this template method requires knowledge of the size of the implementation object.
   *  Therefore, to use model.getOptionalUniqueModelObject<Facility>() the user must include both
   *  Facility.hpp and Facility_Impl.hpp.  It may be better to instantiate each version of this
   *  template method to avoid exposing the implementation objects, this is an open question. */
  template <typename T>
  boost::optional<T> getOptionalUniqueModelObject() const {
    boost::optional<T> result;
    std::vector<WorkspaceObject> objects = this->allObjects();
    for(std::vector<WorkspaceObject>::const_iterator it = objects.begin(), itend = objects.end(); it < itend; ++it){
      std::shared_ptr<typename T::ImplType> p = it->getImpl<typename T::ImplType>();
      if (p){
        result = T(p);
        break;
      }
    }
    return result;
  }

  /** Returns all \link ModelObject ModelObjects \endlink of type T. This method can be used with T
   *  as a concrete type (e.g. Zone) or as an abstract class (e.g. ParentObject).
   *
   *  \todo Use of this template method requires knowledge of the size of the implementation object.
   *  Therefore, to use model.getModelObjects<Zone>(), the user must include both Zone.hpp and
   *  Zone_Impl.hpp.  It may be better to instantiate each version of this template method to avoid
   *  exposing the implementation objects, this is an open question. */
  template <typename T>
  std::vector<T> getModelObjects(bool sorted=false) const
  {
    std::vector<T> result;
    std::vector<WorkspaceObject> objects = this->objects(sorted);
    result.reserve(objects.size());
    for(std::vector<WorkspaceObject>::const_iterator it = objects.begin(), itend = objects.end(); it < itend; ++it)
    {
      std::shared_ptr<typename T::ImplType> p = it->getImpl<typename T::ImplType>();
      if (p) { result.push_back(T(p)); }
    }
    return result;
  }

  /** Returns all \link ModelObject ModelObjects \endlink of type T, using T::iddObjectType() to
   *  speed up the search. This method will only work for concrete model objects (leaves in the
   *  ModelObject inheritance tree), hence the name. */
  template <typename T>
  std::vector<T> getConcreteModelObjects() const
  {
    std::vector<T> result;
    std::vector<WorkspaceObject> objects = this->getObjectsByType(T::iddObjectType());
    for(std::vector<WorkspaceObject>::const_iterator it = objects.begin(), itend = objects.end(); it < itend; ++it)
    {
      std::shared_ptr<typename T::ImplType> p = it->getImpl<typename T::ImplType>();
      if (p) { result.push_back(T(p)); }
    }
    return result;
  }

  /** Returns the subset of \link ModelObject ModelObjects \endlink referenced by handles
   *  which are of type T. This method can be used with T as a concrete type (e.g. Zone) or
   *  as an abstract class (e.g. ParentObject).
   *
   *  \todo Use of this template method requires knowledge of the size of the implementation object.
   *  Therefore, to use model.getModelObjects<Zone>(handles) the user must include both Zone.hpp and
   *  Zone_Impl.hpp. It may be better to instantiate each version of this template method to avoid
   *  exposing the implementation objects, this is an open question. */
  template <typename T>
  std::vector<T> getModelObjects(const std::vector<openstudio::Handle>& handles) const
  {
    std::vector<T> result;
    result.reserve(handles.size());
    std::vector<WorkspaceObject> objects = this->getObjects(handles);
    for(std::vector<WorkspaceObject>::const_iterator it = objects.begin(), itend = objects.end(); it < itend; ++it)
    {
      std::shared_ptr<typename T::ImplType> p = it->getImpl<typename T::ImplType>();
      if (p){ result.push_back(T(p)); }
    }
    return result;
  }

  /** Returns the \link ModelObject ModelObjects \endlink of type T with name (exactMatch). This method
   *  can be used with T as a concrete type (e.g. Zone) or as an abstract class (e.g. ParentObject), but
   *  please see getConcreteModelObjectByName if T is concrete.
   *
   *  \todo Use of this template method requires knowledge of the size of the implementation object.
   *  Therefore, to use model.getModelObjectsByName<Zone>("Zone1") the user must include both
   *  Zone.hpp and Zone_Impl.hpp. It may be better to instantiate each version of this template
   *  method to avoid exposing the implementation objects, this is an open question. */
  template <typename T>
  boost::optional<T> getModelObjectByName(const std::string& name) const
  {
    boost::optional<T> result;
    std::vector<T> intermediate = getModelObjectsByName<T>(name,true);
    if (!intermediate.empty()) {
      OS_ASSERT(intermediate.size() == 1u);
      result = intermediate[0];
    }
    return result;
  }

  /** Returns all \link ModelObject ModelObjects \endlink of type T with given name. This method can
   *  be used with T as a concrete type (e.g. Zone) or as an abstract class (e.g. ParentObject).
   *
   *  \todo Use of this template method requires knowledge of the size of the implementation object.
   *  Therefore, to use model.getModelObjectsByName<Zone>("Zone1") the user must include both
   *  Zone.hpp and Zone_Impl.hpp. It may be better to instantiate each version of this template
   *  method to avoid exposing the implementation objects, this is an open question. */
  template <typename T>
  std::vector<T> getModelObjectsByName(const std::string& name, bool exactMatch=true) const
  {
    std::vector<T> result;
    std::vector<WorkspaceObject> objects = this->getObjectsByName(name,exactMatch);
    for(std::vector<WorkspaceObject>::const_iterator it = objects.begin(), itend = objects.end(); it < itend; ++it)
    {
      std::shared_ptr<typename T::ImplType> p = it->getImpl<typename T::ImplType>();
      if (p) {
        result.push_back(T(p));
      }
    }
    return result;
  }

  template <typename T>
  boost::optional<T> getConcreteModelObjectByName(const std::string& name) const
  {
    boost::optional<T> result;
    boost::optional<WorkspaceObject> object = this->getObjectByTypeAndName(T::iddObjectType(),name);
    if (object) {
      std::shared_ptr<typename T::ImplType> p = object->getImpl<typename T::ImplType>();
      if (p) {
        result = T(p);
      }
    }
    return result;
  }

  template<typename T>
  std::vector<T> getConcreteModelObjectsByName(const std::string& name) const
  {
    std::vector<T> result;
    std::vector<WorkspaceObject> objects = this->getObjectsByTypeAndName(T::iddObjectType(),name);
    for(std::vector<WorkspaceObject>::const_iterator it = objects.begin(), itend = objects.end(); it < itend; ++it)
    {
      std::shared_ptr<typename T::ImplType> p = it->getImpl<typename T::ImplType>();
      if (p) {
        result.push_back(T(p));
      }
    }
    return result;
  }

  //@}

  /** Load Model from file, attempts to load WorkflowJSON from standard path. */
  static boost::optional<Model> load(const path& osmPath);

  /** Load Model and WorkflowJSON from files, fails if either osm or workflowJSON cannot be loaded. */
  static boost::optional<Model> load(const path& osmPath, const path& workflowJSONPath);

  /// Equality test, tests if this Model shares the same implementation object with other.
  bool operator==(const Model& other) const;

  /** Get all model objects. If sorted, then the objects are returned in the preferred order. */
  std::vector<ModelObject> modelObjects(bool sorted=false) const;

  // DLM@20110614: looks like this is returning a ComponentData, not a primary object?
  /** Inserts Component into Model and returns the primary object, if possible. */
  boost::optional<ComponentData> insertComponent(const Component& component);

  // DLM@20110614: should we have a template method for this?
  /** Removes all \link ResourceObject ResourceObjects\endlink with
   *  nonResourceObjectUseCount() == 0. All objects removed in the course of the purge
   *  are returned to support undos. Note that ResourceObjects may have children that
   *  are not ResourceObjects, and these may be removed as well. */
  std::vector<openstudio::IdfObject> purgeUnusedResourceObjects();

  /** Removes all \link ResourceObject ResourceObjects\endlink of given IddObjectType with
   *  directUseCount() == 0. All objects removed in the course of the purge
   *  are returned to support undos. Note that ResourceObjects may have children that
   *  are not ResourceObjects, and these may be removed as well. */
  std::vector<openstudio::IdfObject> purgeUnusedResourceObjects(IddObjectType iddObjectType);

  // DLM@20110614: Kyle can you fill in here?
  /// Connects the sourcePort on the source ModelObject to the targetPort on the target ModelObject.
  void connect(ModelObject sourceObject,
               unsigned sourcePort,
               ModelObject targetObject,
               unsigned targetPort) const;

  // DLM@20110614: Kyle can you fill in here?
  /// Disconnects the port on the given ModelObject.
  void disconnect(ModelObject object, unsigned port);

  // DLM@20110614: why is this here, can we deprecate it?
  /// @cond
  detail::Model_Impl* rawImpl() const;
  /// @endcond

  /** For each object in the model with autosizable fields,
   *  sets all autosizable fields to 'autosize'.
   *  Fields that previously contained hard-sized
   *  values will be overwritten by 'autosize.'
   */
  void autosize();

  /** For each object in the model with autosizable fields,
   *  retrieves the autosized values from the sizing run and then
   *  sets these values in the object explicitly.
   *  Requires a sql file with sizing run results from a
   *  previous simulation.
   *  For example, if a ChillerElectricEIR's Reference Capacity
   *  was previously autosized to 120,000W by the sizing run,
   *  this method would find the 120,000W in the sql file and then
   *  set the Reference Capacity field to 120,000W explicitly.  Next
   *  time a simulation is run, the chiller's capacity will be 120,000W,
   *  it will not be autosized during the sizing run.
   */
  void applySizingValues();

 protected:
  /// @cond
  typedef detail::Model_Impl ImplType;

  friend class openstudio::IdfObject;
  friend class ModelObject;
  friend class detail::ModelObject_Impl;

  friend class openstudio::Workspace;
  friend class detail::Model_Impl;

  /** Protected constructor from impl. */
  Model(std::shared_ptr<detail::Model_Impl> impl);

  virtual void addVersionObject() override;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.Model");
};

/** \relates Model */
typedef boost::optional<Model> OptionalModel;

/** \relates Model */
typedef std::vector<Model> ModelVector;

//DLM@20110614: why is this here?  seems like something for the unit tests.

/** Compare inputResult to outputResult. The intention is for inputResult to be the value of
 *  attributeName calculated using input data, and for outputResult to be the value of
 *  attributeName extracted from the Model::sqlFile(). Returns false and logs a message on the
 *  Model log channel at logLevel if the (relative) error is greater than tol. \relates Model */
bool MODEL_API compareInputAndOutput(const ModelObject& object,
                                     const std::string& attributeName,
                                     double inputResult,
                                     double outputResult,
                                     double tol,
                                     LogLevel logLevel=LogLevel::Debug);

/// Returns an example model useful for testing.
MODEL_API Model exampleModel();

/// Adds example model objects to an existing model.
MODEL_API void addExampleModelObjects(Model& model);

} // model
} // openstudio

#endif // MODEL_MODEL_HPP
