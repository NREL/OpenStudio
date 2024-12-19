/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
  class FoundationKivaSettings;
  class OutputControlFiles;
  class OutputControlReportingTolerances;
  class OutputControlTableStyle;
  class OutputControlTimestamp;
  class OutputDiagnostics;
  class OutputDebuggingData;
  class OutputJSON;
  class OutputSQLite;
  class OutputEnergyManagementSystem;
  class OutputTableSummaryReports;
  class OutputSchedules;
  class OutputConstructions;
  class PerformancePrecisionTradeoffs;
  class LifeCycleCostParameters;
  class SizingParameters;
  class RadianceParameters;
  class RunPeriod;
  class RunPeriodControlDaylightSavingTime;
  class YearDescription;
  class Site;
  class SiteGroundReflectance;
  class SiteWaterMainsTemperature;
  class SiteGroundTemperatureBuildingSurface;
  class SiteGroundTemperatureFCfactorMethod;
  class SiteGroundTemperatureDeep;
  class SiteGroundTemperatureShallow;
  class Facility;
  class WeatherFile;
  class SimulationControl;
  class LightingSimulationControl;
  class AirflowNetworkSimulationControl;
  class InsideSurfaceConvectionAlgorithm;
  class OutsideSurfaceConvectionAlgorithm;
  class HeatBalanceAlgorithm;
  class ZoneAirContaminantBalance;
  class ZoneAirHeatBalanceAlgorithm;
  class ZoneAirMassFlowConservation;
  class ZoneCapacitanceMultiplierResearchSpecial;
  class ConvergenceLimits;
  class ShadowCalculation;
  class Timestep;
  class ClimateZones;
  class EnvironmentalImpactFactors;
  class ExternalInterface;
  class Component;
  class ComponentData;
  class Schedule;
  class Node;
  class SpaceType;
  class PythonPluginSearchPaths;

  namespace detail {
    class Model_Impl;
    class ModelObject_Impl;
  }  // namespace detail

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
  class MODEL_API Model : public openstudio::Workspace
  {
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

    virtual ~Model() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Model(const Model& other) = default;
    Model(Model&& other) = default;
    Model& operator=(const Model&) = default;
    Model& operator=(Model&&) = default;

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

    /** Get the OutputControlFiles object if there is one, this implementation uses a cached reference to the OutputControlFiles
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<OutputControlFiles>(). */
    boost::optional<OutputControlFiles> outputControlFiles() const;

    /** Get the OutputControlReportingTolerances object if there is one, this implementation uses a cached reference to the OutputControlReportingTolerances
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<OutputControlReportingTolerances>(). */
    boost::optional<OutputControlReportingTolerances> outputControlReportingTolerances() const;

    /** Get the OutputControlTableStyle object if there is one, this implementation uses a cached reference to the OutputControlTableStyle
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<OutputControlTableStyle>(). */
    boost::optional<OutputControlTableStyle> outputControlTableStyle() const;

    /** Get the OutputControlTimestamp object if there is one, this implementation uses a cached reference to the OutputControlTimestamp
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<OutputControlTimestamp>(). */
    boost::optional<OutputControlTimestamp> outputControlTimestamp() const;

    /** Get the OutputDiagnostics object if there is one, this implementation uses a cached reference to the OutputDiagnostics
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<OutputDiagnostics>(). */
    boost::optional<OutputDiagnostics> outputDiagnostics() const;

    /** Get the OutputDebuggingData object if there is one, this implementation uses a cached reference to the OutputDebuggingData
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<OutputDebuggingData>(). */
    boost::optional<OutputDebuggingData> outputDebuggingData() const;

    /** Get the OutputJSON object if there is one, this implementation uses a cached reference to the OutputJSON
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<OutputJSON>(). */
    boost::optional<OutputJSON> outputJSON() const;

    /** Get the OutputSQLite object if there is one, this implementation uses a cached reference to the OutputSQLite
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<OutputSQLite>(). */
    boost::optional<OutputSQLite> outputSQLite() const;

    /** Get the OutputEnergyManagementSystem object if there is one, this implementation uses a cached reference to the OutputEnergyManagementSystem
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<OutputControlFiles>(). */
    boost::optional<OutputEnergyManagementSystem> outputEnergyManagementSystem() const;

    /** Get the OutputTableSummaryReports object if there is one, this implementation uses a cached reference to the OutputTableSummaryReports
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<OutputTableSummaryReports>(). */
    boost::optional<OutputTableSummaryReports> outputTableSummaryReports() const;

    /** Get the OutputSchedules object if there is one, this implementation uses a cached reference to the OutputSchedules
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<OutputSchedules>(). */
    boost::optional<OutputSchedules> outputSchedules() const;

    /** Get the OutputConstructions object if there is one, this implementation uses a cached reference to the OutputConstructions
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<OutputConstructions>(). */
    boost::optional<OutputConstructions> outputConstructions() const;

    /** Get the PerformancePrecisionTradeoffs object if there is one, this implementation uses a cached reference to the PerformancePrecisionTradeoffs
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<PerformancePrecisionTradeoffs>(). */
    boost::optional<PerformancePrecisionTradeoffs> performancePrecisionTradeoffs() const;

    /** Get the LifeCycleCostParameters object if there is one, this implementation uses a cached reference to the LifeCycleCostParameters
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<LifeCycleCostParameters>(). */
    boost::optional<LifeCycleCostParameters> lifeCycleCostParameters() const;

    /** Get the SizingParameters object if there is one, this implementation uses a cached reference to the SizingParameters
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<SizingParameters>(). */
    boost::optional<SizingParameters> sizingParameters() const;

    /** Get the RadianceParameters object if there is one, this implementation uses a cached reference to the RadianceParameters
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<RadianceParameters>(). */
    boost::optional<RadianceParameters> radianceParameters() const;

    /** Get the RunPeriod object if there is one, this implementation uses a cached reference to the RunPeriod
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<RunPeriod>(). */
    boost::optional<RunPeriod> runPeriod() const;

    /** Get the RunPeriodControlDaylightSavingTime object if there is one, this implementation uses a cached reference to the RunPeriodControlDaylightSavingTime
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<RunPeriodControlDaylightSavingTime>(). */
    boost::optional<RunPeriodControlDaylightSavingTime> runPeriodControlDaylightSavingTime() const;

    /** Get the YearDescription object if there is one, this implementation uses a cached reference to the YearDescription
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<YearDescription>(). */
    boost::optional<YearDescription> yearDescription() const;

    /** Get the Site object if there is one, this implementation uses a cached reference to the Site
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<Site>(). */
    boost::optional<Site> site() const;

    /** Get the SiteGroundReflectance object if there is one, this implementation uses a cached reference to the SiteGroundReflectance
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<SiteGroundReflectance>(). */
    boost::optional<SiteGroundReflectance> siteGroundReflectance() const;

    /** Get the SiteWaterMainsTemperature object if there is one, this implementation uses a cached reference to the SiteWaterMainsTemperature
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<SiteWaterMainsTemperature>(). */
    boost::optional<SiteWaterMainsTemperature> siteWaterMainsTemperature() const;

    /** Get the SiteGroundTemperatureBuildingSurface object if there is one, this implementation uses a cached reference to the SiteGroundTemperatureBuildingSurface
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<SiteGroundTemperatureBuildingSurface>(). */
    boost::optional<SiteGroundTemperatureBuildingSurface> siteGroundTemperatureBuildingSurface() const;

    /** Get the SiteGroundTemperatureFCfactorMethod object if there is one, this implementation uses a cached reference to the SiteGroundTemperatureFCfactorMethod
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<SiteGroundTemperatureFCfactorMethod>(). */
    boost::optional<SiteGroundTemperatureFCfactorMethod> siteGroundTemperatureFCfactorMethod() const;

    /** Get the SiteGroundTemperatureDeep object if there is one, this implementation uses a cached reference to the SiteGroundTemperatureDeep
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<SiteGroundTemperatureDeep>(). */
    boost::optional<SiteGroundTemperatureDeep> siteGroundTemperatureDeep() const;

    /** Get the SiteGroundTemperatureShallow object if there is one, this implementation uses a cached reference to the SiteGroundTemperatureShallow
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<SiteGroundTemperatureShallow>(). */
    boost::optional<SiteGroundTemperatureShallow> siteGroundTemperatureShallow() const;

    /** Get the Facility object if there is one, this implementation uses a cached reference to the Facility
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<Facility>(). */
    boost::optional<Facility> facility() const;

    /** Get the WeatherFile object if there is one, this implementation uses a cached reference to the WeatherFile
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<WeatherFile>(). */
    boost::optional<WeatherFile> weatherFile() const;

    /** Get the SimulationControl object if there is one, this implementation uses a cached reference to the SimulationControl
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<SimulationControl>(). */
    boost::optional<SimulationControl> simulationControl() const;

    /** Get the LightingSimulationControl object if there is one, this implementation uses a cached reference to the LightingSimulationControl
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<LightingSimulationControl>(). */
    boost::optional<LightingSimulationControl> lightingSimulationControl() const;

    /** Get the AirflowNetworkSimulationControl object if there is one, this implementation uses a cached reference to the AirflowNetworkSimulationControl
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<AirflowNetworkSimulationControl>(). */
    boost::optional<AirflowNetworkSimulationControl> airflowNetworkSimulationControl() const;

    /** Get the InsideSurfaceConvectionAlgorithm object if there is one, this implementation uses a cached reference to the InsideSurfaceConvectionAlgorithm
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<InsideSurfaceConvectionAlgorithm>(). */
    boost::optional<InsideSurfaceConvectionAlgorithm> insideSurfaceConvectionAlgorithm() const;

    /** Get the OutsideSurfaceConvectionAlgorithm object if there is one, this implementation uses a cached reference to the OutsideSurfaceConvectionAlgorithm
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<OutsideSurfaceConvectionAlgorithm>(). */
    boost::optional<OutsideSurfaceConvectionAlgorithm> outsideSurfaceConvectionAlgorithm() const;

    /** Get the HeatBalanceAlgorithm object if there is one, this implementation uses a cached reference to the HeatBalanceAlgorithm
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<HeatBalanceAlgorithm>(). */
    boost::optional<HeatBalanceAlgorithm> heatBalanceAlgorithm() const;

    /** Get the ZoneAirContaminantBalance object if there is one, this implementation uses a cached reference to the ZoneAirContaminantBalance
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<ZoneAirContaminantBalance>(). */
    boost::optional<ZoneAirContaminantBalance> zoneAirContaminantBalance() const;

    /** Get the ZoneAirHeatBalanceAlgorithm object if there is one, this implementation uses a cached reference to the ZoneAirHeatBalanceAlgorithm
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<ZoneAirHeatBalanceAlgorithm>(). */
    boost::optional<ZoneAirHeatBalanceAlgorithm> zoneAirHeatBalanceAlgorithm() const;

    /** Get the ZoneAirMassFlowConservation object if there is one, this implementation uses a cached reference to the ZoneAirMassFlowConservation
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<ZoneAirMassFlowConservation>(). */
    boost::optional<ZoneAirMassFlowConservation> zoneAirMassFlowConservation() const;

    /** Get the ZoneCapacitanceMultiplierResearchSpecial object if there is one, this implementation uses a cached reference to the ZoneCapacitanceMultiplierResearchSpecial
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>(). */
    boost::optional<ZoneCapacitanceMultiplierResearchSpecial> zoneCapacitanceMultiplierResearchSpecial() const;

    /** Get the ConvergenceLimits object if there is one, this implementation uses a cached reference to the ConvergenceLimits
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<ConvergenceLimits>(). */
    boost::optional<ConvergenceLimits> convergenceLimits() const;

    /** Get the ShadowCalculation object if there is one, this implementation uses a cached reference to the ShadowCalculation
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<ShadowCalculation>(). */
    boost::optional<ShadowCalculation> shadowCalculation() const;

    /** Get the Timestep object if there is one, this implementation uses a cached reference to the Timestep
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<Timestep>(). */
    boost::optional<Timestep> timestep() const;

    /** Get the ClimateZones object if there is one, this implementation uses a cached reference to the ClimateZones
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<ClimateZones>(). */
    boost::optional<ClimateZones> climateZones() const;

    /** Get the EnvironmentalImpactFactors object if there is one, this implementation uses a cached reference to the EnvironmentalImpactFactors
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<EnvironmentalImpactFactors>(). */
    boost::optional<EnvironmentalImpactFactors> environmentalImpactFactors() const;

    /** Get the ExternalInterface object if there is one, this implementation uses a cached reference to the ExternalInterface
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<ExternalInterface>(). */
    boost::optional<ExternalInterface> externalInterface() const;

    /** Get the PythonPluginSearchPaths object if there is one, this implementation uses a cached reference to the PythonPluginSearchPaths
   *  object which can be significantly faster than calling getOptionalUniqueModelObject<PythonPluginSearchPaths>(). */
    boost::optional<PythonPluginSearchPaths> pythonPluginSearchPaths() const;

    /** Get or create the YearDescription object if there is one, then call method from YearDescription. */
    // DLM: this is due to issues exporting the model::YearDescription object because of name conflict with utilities::YearDescription.
    boost::optional<int> calendarYear() const;
    std::string dayofWeekforStartDay() const;
    bool isDayofWeekforStartDayDefaulted() const;
    bool isLeapYear() const;
    bool isIsLeapYearDefaulted() const;
    bool setCalendarYear(int calendarYear);
    void resetCalendarYear();
    bool setDayofWeekforStartDay(const std::string& dayofWeekforStartDay);
    void resetDayofWeekforStartDay();
    bool setIsLeapYear(bool isLeapYear);
    void resetIsLeapYear();
    int assumedYear();
    openstudio::Date makeDate(openstudio::MonthOfYear monthOfYear, unsigned dayOfMonth);
    openstudio::Date makeDate(unsigned monthOfYear, unsigned dayOfMonth);
    openstudio::Date makeDate(openstudio::NthDayOfWeekInMonth n, openstudio::DayOfWeek dayOfWeek, openstudio::MonthOfYear monthOfYear);
    openstudio::Date makeDate(unsigned dayOfYear);

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
        if (p) {
          result = T(std::move(p));
        }
      }
      return result;
    }

    /** Returns the unique ModelObject of type T, creates a one if none are found.
   *
   *  \todo Use of this template method requires knowledge of the size of the implementation object.
   *  Therefore, to use model.getUniqueModelObject<Facility>() the user must include both
   *  Facility.hpp and Facility_Impl.hpp. It may be better to instantiate each version of this
   *  template method to avoid exposing the implementation objects, this is an open question.
   *
   *  Note that template specilizations are provided below for objects were there is a
   *  performance gain to be had by caching the unique model object
   *  eg: getUniqueModelObject<YearDescription>() */
    template <typename T>
    T getUniqueModelObject() {
      // NOTE: all UniqueModelObjects are Concrete. Call getObjectsByType to avoid returning a huge vector
      std::vector<WorkspaceObject> objects = this->getObjectsByType(T::iddObjectType());
      // std::vector<WorkspaceObject> objects = this->allObjects();
      for (const auto& wo : objects) {
        std::shared_ptr<typename T::ImplType> p = wo.getImpl<typename T::ImplType>();
        if (p) {
          return T(std::move(p));
        }
      }
      return T(*this);  // make a new T
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
      // NOTE: all UniqueModelObjects are Concrete. Call getObjectsByType to avoid returning a huge vector
      std::vector<WorkspaceObject> objects = this->getObjectsByType(T::iddObjectType());
      // std::vector<WorkspaceObject> objects = this->allObjects();
      for (const auto& wo : objects) {
        std::shared_ptr<typename T::ImplType> p = wo.getImpl<typename T::ImplType>();
        if (p) {
          result = T(std::move(p));
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
    std::vector<T> getModelObjects(bool sorted = false) const {
      std::vector<T> result;
      std::vector<WorkspaceObject> objects = this->objects(sorted);
      result.reserve(objects.size());
      for (const auto& wo : objects) {
        std::shared_ptr<typename T::ImplType> p = wo.getImpl<typename T::ImplType>();
        if (p) {
          result.push_back(T(std::move(p)));
        }
      }
      return result;
    }

    /** Returns all \link ModelObject ModelObjects \endlink of type T, using T::iddObjectType() to
   *  speed up the search. This method will only work for concrete model objects (leaves in the
   *  ModelObject inheritance tree), hence the name. */
    template <typename T>
    std::vector<T> getConcreteModelObjects() const {
      std::vector<T> result;
      std::vector<WorkspaceObject> objects = this->getObjectsByType(T::iddObjectType());
      result.reserve(objects.size());
      for (const auto& wo : objects) {
        std::shared_ptr<typename T::ImplType> p = wo.getImpl<typename T::ImplType>();
        if (p) {
          // emplace_back(std::move(p)) did not work, calling a protected constructor...
          // the std::allocator for vector can forward to free functions...
          result.push_back(T(std::move(p)));
        }
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
    std::vector<T> getModelObjects(const std::vector<openstudio::Handle>& handles) const {
      std::vector<T> result;
      result.reserve(handles.size());
      std::vector<WorkspaceObject> objects = this->getObjects(handles);
      for (const auto& wo : objects) {
        std::shared_ptr<typename T::ImplType> p = wo.getImpl<typename T::ImplType>();
        if (p) {
          result.push_back(T(std::move(p)));
        }
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
    boost::optional<T> getModelObjectByName(const std::string& name) const {
      boost::optional<T> result;
      std::vector<T> intermediate = getModelObjectsByName<T>(name, true);
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
    std::vector<T> getModelObjectsByName(const std::string& name, bool exactMatch = true) const {
      std::vector<T> result;
      std::vector<WorkspaceObject> objects = this->getObjectsByName(name, exactMatch);
      result.reserve(objects.size());
      for (auto& wo : objects) {
        std::shared_ptr<typename T::ImplType> p = wo.getImpl<typename T::ImplType>();
        if (p) {
          result.push_back(T(p));
        }
      }
      return result;
    }

    template <typename T>
    boost::optional<T> getConcreteModelObjectByName(const std::string& name) const {
      boost::optional<T> result;
      boost::optional<WorkspaceObject> object = this->getObjectByTypeAndName(T::iddObjectType(), name);
      if (object) {
        std::shared_ptr<typename T::ImplType> p = object->getImpl<typename T::ImplType>();
        if (p) {
          result = T(std::move(p));
        }
      }
      return result;
    }

    template <typename T>
    std::vector<T> getConcreteModelObjectsByName(const std::string& name) const {
      std::vector<T> result;
      std::vector<WorkspaceObject> objects = this->getObjectsByTypeAndName(T::iddObjectType(), name);
      result.reserve(objects.size());
      for (auto& wo : objects) {
        std::shared_ptr<typename T::ImplType> p = wo.getImpl<typename T::ImplType>();
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
    std::vector<ModelObject> modelObjects(bool sorted = false) const;

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
    void connect(ModelObject sourceObject, unsigned sourcePort, ModelObject targetObject, unsigned targetPort) const;

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
    using ImplType = detail::Model_Impl;

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
  using OptionalModel = boost::optional<Model>;

  /** \relates Model */
  using ModelVector = std::vector<Model>;

  //DLM@20110614: why is this here?  seems like something for the unit tests.

  /** Compare inputResult to outputResult. The intention is for inputResult to be the value of
 *  attributeName calculated using input data, and for outputResult to be the value of
 *  attributeName extracted from the Model::sqlFile(). Returns false and logs a message on the
 *  Model log channel at logLevel if the (relative) error is greater than tol. \relates Model */
  bool MODEL_API compareInputAndOutput(const ModelObject& object, const std::string& attributeName, double inputResult, double outputResult,
                                       double tol, LogLevel logLevel = LogLevel::Debug);

  /// Returns an example model useful for testing.
  MODEL_API Model exampleModel();

  /// Adds example model objects to an existing model.
  MODEL_API void addExampleModelObjects(Model& model);

  // Template specializations for getUniqueModelObject to use caching
  template <>
  MODEL_API Building Model::getUniqueModelObject<Building>();

  template <>
  MODEL_API FoundationKivaSettings Model::getUniqueModelObject<FoundationKivaSettings>();

  template <>
  MODEL_API OutputControlFiles Model::getUniqueModelObject<OutputControlFiles>();

  template <>
  MODEL_API OutputControlReportingTolerances Model::getUniqueModelObject<OutputControlReportingTolerances>();

  template <>
  MODEL_API OutputControlTableStyle Model::getUniqueModelObject<OutputControlTableStyle>();

  template <>
  MODEL_API OutputControlTimestamp Model::getUniqueModelObject<OutputControlTimestamp>();

  template <>
  MODEL_API OutputDiagnostics Model::getUniqueModelObject<OutputDiagnostics>();

  template <>
  MODEL_API OutputDebuggingData Model::getUniqueModelObject<OutputDebuggingData>();

  template <>
  MODEL_API OutputJSON Model::getUniqueModelObject<OutputJSON>();

  template <>
  MODEL_API OutputSQLite Model::getUniqueModelObject<OutputSQLite>();

  template <>
  MODEL_API OutputEnergyManagementSystem Model::getUniqueModelObject<OutputEnergyManagementSystem>();

  template <>
  MODEL_API OutputTableSummaryReports Model::getUniqueModelObject<OutputTableSummaryReports>();

  template <>
  MODEL_API OutputSchedules Model::getUniqueModelObject<OutputSchedules>();

  template <>
  MODEL_API OutputConstructions Model::getUniqueModelObject<OutputConstructions>();

  template <>
  MODEL_API PerformancePrecisionTradeoffs Model::getUniqueModelObject<PerformancePrecisionTradeoffs>();

  template <>
  MODEL_API LifeCycleCostParameters Model::getUniqueModelObject<LifeCycleCostParameters>();

  template <>
  MODEL_API SizingParameters Model::getUniqueModelObject<SizingParameters>();

  template <>
  MODEL_API RadianceParameters Model::getUniqueModelObject<RadianceParameters>();

  template <>
  MODEL_API RunPeriod Model::getUniqueModelObject<RunPeriod>();

  template <>
  MODEL_API RunPeriodControlDaylightSavingTime Model::getUniqueModelObject<RunPeriodControlDaylightSavingTime>();

  template <>
  MODEL_API YearDescription Model::getUniqueModelObject<YearDescription>();

  template <>
  MODEL_API Site Model::getUniqueModelObject<Site>();

  template <>
  MODEL_API SiteGroundReflectance Model::getUniqueModelObject<SiteGroundReflectance>();

  template <>
  MODEL_API SiteWaterMainsTemperature Model::getUniqueModelObject<SiteWaterMainsTemperature>();

  template <>
  MODEL_API SiteGroundTemperatureBuildingSurface Model::getUniqueModelObject<SiteGroundTemperatureBuildingSurface>();

  template <>
  MODEL_API SiteGroundTemperatureFCfactorMethod Model::getUniqueModelObject<SiteGroundTemperatureFCfactorMethod>();

  template <>
  MODEL_API SiteGroundTemperatureDeep Model::getUniqueModelObject<SiteGroundTemperatureDeep>();

  template <>
  MODEL_API SiteGroundTemperatureShallow Model::getUniqueModelObject<SiteGroundTemperatureShallow>();

  template <>
  MODEL_API Facility Model::getUniqueModelObject<Facility>();

  template <>
  MODEL_API WeatherFile Model::getUniqueModelObject<WeatherFile>();

  template <>
  MODEL_API SimulationControl Model::getUniqueModelObject<SimulationControl>();

  template <>
  MODEL_API LightingSimulationControl Model::getUniqueModelObject<LightingSimulationControl>();

  template <>
  MODEL_API AirflowNetworkSimulationControl Model::getUniqueModelObject<AirflowNetworkSimulationControl>();

  template <>
  MODEL_API InsideSurfaceConvectionAlgorithm Model::getUniqueModelObject<InsideSurfaceConvectionAlgorithm>();

  template <>
  MODEL_API OutsideSurfaceConvectionAlgorithm Model::getUniqueModelObject<OutsideSurfaceConvectionAlgorithm>();

  template <>
  MODEL_API HeatBalanceAlgorithm Model::getUniqueModelObject<HeatBalanceAlgorithm>();

  template <>
  MODEL_API ZoneAirContaminantBalance Model::getUniqueModelObject<ZoneAirContaminantBalance>();

  template <>
  MODEL_API ZoneAirHeatBalanceAlgorithm Model::getUniqueModelObject<ZoneAirHeatBalanceAlgorithm>();

  template <>
  MODEL_API ZoneAirMassFlowConservation Model::getUniqueModelObject<ZoneAirMassFlowConservation>();

  template <>
  MODEL_API ZoneCapacitanceMultiplierResearchSpecial Model::getUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>();

  template <>
  MODEL_API ConvergenceLimits Model::getUniqueModelObject<ConvergenceLimits>();

  template <>
  MODEL_API ShadowCalculation Model::getUniqueModelObject<ShadowCalculation>();

  template <>
  MODEL_API Timestep Model::getUniqueModelObject<Timestep>();

  template <>
  MODEL_API ClimateZones Model::getUniqueModelObject<ClimateZones>();

  template <>
  MODEL_API EnvironmentalImpactFactors Model::getUniqueModelObject<EnvironmentalImpactFactors>();

  template <>
  MODEL_API ExternalInterface Model::getUniqueModelObject<ExternalInterface>();

  template <>
  MODEL_API PythonPluginSearchPaths Model::getUniqueModelObject<PythonPluginSearchPaths>();

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MODEL_HPP
