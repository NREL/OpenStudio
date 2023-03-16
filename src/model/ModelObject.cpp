/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"
#include "Model_Impl.hpp"
#include "Component.hpp"
#include "LifeCycleCost.hpp"
#include "LifeCycleCost_Impl.hpp"
#include "ParentObject.hpp"
#include "ResourceObject.hpp"
#include "ResourceObject_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "CoilCoolingDXTwoStageWithHumidityControlMode.hpp"
#include "CoilCoolingDXTwoStageWithHumidityControlMode_Impl.hpp"
#include "CoilPerformanceDXCooling.hpp"
#include "CoilPerformanceDXCooling_Impl.hpp"
#include "AdditionalProperties.hpp"
#include "AdditionalProperties_Impl.hpp"

#include "ScheduleTypeRegistry.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"

#include "OutputVariable.hpp"
#include "OutputVariable_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/ContainersMove.hpp"
#include "../utilities/sql/SqlFileEnums.hpp"
#include "../utilities/sql/SqlFileTimeSeriesQuery.hpp"
#include "../utilities/sql/SqlFile.hpp"
#include "../utilities/idf/ValidityReport.hpp"

#include <utilities/idd/OS_Output_Variable_FieldEnums.hxx>
#include "../utilities/data/TimeSeries.hpp"

#include <boost/algorithm/string.hpp>
#include <fmt/core.h>

using openstudio::Handle;
using openstudio::HandleVector;
using openstudio::WorkspaceObject;
using openstudio::OptionalWorkspaceObject;
using openstudio::WorkspaceObjectVector;
using openstudio::Workspace;

using std::vector;

namespace openstudio {
namespace model {
  namespace detail {

    ModelObject_Impl::ModelObject_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : WorkspaceObject_Impl(other, model, keepHandle) {}

    ModelObject_Impl::ModelObject_Impl(const ModelObject_Impl& other, Model_Impl* model, bool keepHandle)
      : WorkspaceObject_Impl(other, model, keepHandle) {}

    // GETTERS

    Model ModelObject_Impl::model() const {
      return workspace().cast<Model>();
    }

    std::vector<LifeCycleCost> ModelObject_Impl::lifeCycleCosts() const {
      return getObject<ModelObject>().getModelObjectSources<LifeCycleCost>();
    }

    std::vector<IdfObject> ModelObject_Impl::removeLifeCycleCosts() {
      std::vector<IdfObject> removedCosts;
      std::vector<LifeCycleCost> lifeCycleCosts = this->lifeCycleCosts();
      for (LifeCycleCost& lifeCycleCost : lifeCycleCosts) {
        openstudio::detail::concat_helper(removedCosts, lifeCycleCost.remove());
      }
      return removedCosts;
    }

    std::vector<ResourceObject> ModelObject_Impl::resources() const {
      // resources are ResourceObjects pointed to ...
      ResourceObjectVector resourceObjects = getObject<ModelObject>().getModelObjectTargets<ResourceObject>();

      // create vector of unique resource objects except for a ResourceObject that is this object's parent
      OptionalParentObject opo = parent();
      std::set<Handle> uniqueResourceHandles;
      ResourceObjectVector result;
      for (const ResourceObject& resourceObject : resourceObjects) {

        Handle handle = resourceObject.handle();
        if (opo && (opo->handle() == handle)) {
          continue;
        }

        auto it = uniqueResourceHandles.find(handle);
        if (it != uniqueResourceHandles.end()) {
          continue;
        }

        uniqueResourceHandles.insert(handle);
        result.push_back(resourceObject);
      }

      return result;
    }

    /** Get all output variables associated with this object, must run simulation to generate associate data. */
    OutputVariableVector ModelObject_Impl::outputVariables() const {
      OutputVariableVector variables;
      std::vector<std::string> variableNames = this->outputVariableNames();
      OptionalString name = this->name();

      for (OutputVariable& variable : this->model().getConcreteModelObjects<OutputVariable>()) {
        std::string keyValue = variable.keyValue();
        std::string variableName = variable.variableName();

        if (name) {
          if (istringEqual(*name, keyValue) || (keyValue == "*")) {
            // match
          } else {
            continue;  // no match
          }
        } else {
          // match
        }

        if (std::find(variableNames.begin(), variableNames.end(), variableName) != variableNames.end()) {
          variables.emplace_back(std::move(variable));
        }
      }
      return variables;
    }

    /** Get data associated with this output variable and this object. */
    openstudio::OptionalTimeSeries ModelObject_Impl::getData(const OutputVariable& variable, const std::string& envPeriod) const {
      openstudio::OptionalTimeSeries result;
      OptionalSqlFile sqlFile = this->model().sqlFile();

      if (sqlFile) {
        OptionalString variableName = variable.getString(OS_Output_VariableFields::VariableName);
        OptionalString keyValue = variable.getString(OS_Output_VariableFields::KeyValue);
        OptionalString reportingFrequency = variable.getString(OS_Output_VariableFields::ReportingFrequency, true);

        // solve any issues if key value is '*'
        keyValue = getSpecificKeyValue(keyValue);

        if (variableName && keyValue) {
          if (!reportingFrequency) {
            // default to hourly frequency
            LOG(Warn, "Variable does not specify reporting frequency, defaulting to hourly.");
            reportingFrequency = ReportingFrequency(ReportingFrequency::Hourly).valueName();
          }
          // use query interface because replaces requested strings with exact strings in database
          ReportingFrequency rf(ReportingFrequency::Hourly);
          try {
            ReportingFrequency rf(boost::trim_copy(*reportingFrequency));
          } catch (...) {
            LOG(Warn, "Unrecognized reporting frequency string. Defaulting to hourly.");
          }
          SqlFileTimeSeriesQuery q(envPeriod, rf, *variableName, *keyValue);
          TimeSeriesVector tsVec = sqlFile->timeSeries(q);
          if (tsVec.size() == 1) {
            result = tsVec[0];
          }
        }
      }

      return result;
    }

    // set name from optional string
    bool ModelObject_Impl::mf_setName(const boost::optional<std::string>& name) {
      bool result = false;
      if (name) {
        result = this->setName(*name).has_value();
      }
      return result;
    }

    // get specific key value from optional key value which might be '*'
    OptionalString ModelObject_Impl::getSpecificKeyValue(OptionalString keyValue) const {
      OptionalString name = this->name();
      if (keyValue && name) {
        if (*keyValue == "*") {
          keyValue = name;
        }
      } else if (keyValue) {
        if (*keyValue == "*") {
          LOG(Error, "Variable specifies '*' for key value and object has no name.");
          return {};
        }
      } else if (name) {
        LOG(Warn, "Variable does not specify key value, using object name.");
        keyValue = name;
      } else {
        LOG(Error, "Variable does not specify key value and object has no name.");
        return {};
      }
      return keyValue;
    }

    boost::optional<double> ModelObject_Impl::getAutosizedValueFromInitializationSummary(const std::string& valueName,
                                                                                         const std::string& units) const {

      // Get the object name
      if (!name()) {
        LOG(Warn, "This object does not have a name, cannot retrieve the autosized value '" + valueName + "'.");
        return boost::none;
      }

      // Check that the model has a sql file
      if (!model().sqlFile()) {
        LOG(Warn, "This model has no sql file, cannot retrieve the autosized value '" + valueName + "'.");
        return boost::none;
      }

      // Get the object name and transform to the way it is recorded
      // in the sql file
      std::string sqlName = name().get();
      boost::to_upper(sqlName);

      // Query the InitializationSummary -> Component Sizing table to get
      // the row names that contains information for this component.
      const std::string rowsQuery = R"(
      SELECT RowName FROM TabularDataWithStrings
        WHERE ReportName = 'InitializationSummary'
        AND ReportForString = 'Entire Facility'
        AND TableName = 'Component Sizing Information'
        AND Value = ?;)";

      boost::optional<std::vector<std::string>> rowNames = model().sqlFile().get().execAndReturnVectorOfString(rowsQuery,
                                                                                                               // bindArgs
                                                                                                               sqlName);

      // Warn if the query failed
      if (!rowNames) {
        LOG(Warn, "Could not find a component called '" + sqlName + "' in any rows of the InitializationSummary Component Sizing table.");
        return boost::none;
      }

      // Query each row of the InitializationSummary -> Component Sizing table
      // that contains this component to get the desired value.
      std::string valueNameAndUnits = valueName + std::string(" [") + units + std::string("]");
      if (units.empty()) {
        valueNameAndUnits = valueName;
      } else if (units == "typo_in_energyplus") {
        valueNameAndUnits = valueName + std::string(" []");
      }

      for (const std::string& rowName : rowNames.get()) {
        const std::string rowCheckQuery = R"(
        SELECT Value FROM TabularDataWithStrings
          WHERE ReportName = 'InitializationSummary'
          AND ReportForString = 'Entire Facility'
          AND TableName = 'Component Sizing Information'
          AND RowName = ?
          AND Value = ?;)";
        const boost::optional<std::string> rowValueName = model().sqlFile().get().execAndReturnFirstString(rowCheckQuery,
                                                                                                           // bindArgs
                                                                                                           rowName, valueNameAndUnits);
        // Check if the query succeeded
        if (!rowValueName) {
          continue;
        }
        // This is the right row
        const std::string valQuery = R"(
        SELECT Value FROM TabularDataWithStrings
          WHERE ReportName = 'InitializationSummary'
          AND ReportForString = 'Entire Facility'
          AND TableName = 'Component Sizing Information'
          AND ColumnName='Value'
          AND RowName = ?;)";
        boost::optional<double> val = model().sqlFile().get().execAndReturnFirstDouble(valQuery,
                                                                                       // bindArgs
                                                                                       rowName);
        // Check if the query succeeded and return if so
        if (val) {
          return val;
        }
      }

      LOG(Debug, "The autosized value query for " + valueNameAndUnits + " of " + sqlName + " returned no value.");
      return boost::none;
    }

    /** Gets the autosized component value from the sql file **/
    boost::optional<double> ModelObject_Impl::getAutosizedValue(const std::string& valueName, const std::string& units,
                                                                std::string overrideCompType) const {

      // Get the object name
      if (!name()) {
        LOG(Warn, "This object does not have a name, cannot retrieve the autosized value '" + valueName + "'.");
        return boost::none;
      }

      // Check that the model has a sql file
      if (!model().sqlFile()) {
        LOG(Warn, "This model has no sql file, cannot retrieve the autosized value '" + valueName + "'.");
        return boost::none;
      }

      // Get the object name and transform to the way it is recorded
      // in the sql file
      std::string sqlName = name().get();
      boost::to_upper(sqlName);

      // Get the object type and transform to the way it is recorded in the sql file
      if (overrideCompType.empty()) {
        overrideCompType = iddObject().type().valueDescription();
        boost::replace_all(overrideCompType, "OS:", "");
      }

      const std::string directQuery = R"sql(
      SELECT Value FROM ComponentSizes
        WHERE CompType = ?
          AND CompName = ?
          AND Description = ?
          AND Units = ?;
    )sql";
      boost::optional<double> val = model().sqlFile().get().execAndReturnFirstDouble(directQuery,
                                                                                     // bindArgs
                                                                                     overrideCompType, sqlName, valueName, units);
      if (!val) {
        LOG(Debug, fmt::format(R"sql(The direct query failed:
SELECT Value FROM ComponentSizes
  WHERE CompType = '{}'
    AND CompName = '{}'
    AND Description = '{}'
    AND Units = '{}';)sql",
                               overrideCompType, sqlName, valueName, units));
      }

      return val;
    }

    //void ModelObject_Impl::connect(unsigned outletPort, ModelObject target, unsigned inletPort)
    //{
    //  OptionalConnection connection = getOutletConnection(outletPort);
    //  connection->targetModelObject(target);
    //  connection->inletPort(inletPort);
    //  target.setInletConnection(connection.get());
    //}

    //OptionalConnection ModelObject_Impl::getOutletConnection(unsigned portID)
    //{
    //  ConnectionVector connectionVector;
    //  connectionVector = Connection::getConnection(model());
    //  OptionalConnection connection;
    //
    //  for (connection : connectionVector)
    //  {
    //    if(connection->getSourceModelObject())
    //    {
    //      if(connection->getSourceModelObject()->handle() == handle())
    //      {
    //        if(connection->getOutletPort() == portID)
    //        {
    //          return connection;
    //        }
    //      }
    //    }
    //  }
    //  connection = Connection(model());
    //  openstudio::Handle h = this->handle();
    //  ModelObject modelObject = model().getModelObject(h).get();
    //  connection->sourceModelObject(modelObject);
    //  connection->outletPort(portID);
    //  return connection;
    //}

    //void ModelObject_Impl::setOutletConnection(Connection _connection)
    //{
    //  ConnectionVector connectionVector;
    //  connectionVector = Connection::getConnection(model());
    //  OptionalConnection connection;
    //
    //  for (connection : connectionVector)
    //  {
    //    if(connection->getSourceModelObject())
    //    {
    //      if(connection->getSourceModelObject()->handle() == handle())
    //      {
    //        if(connection->getOutletPort() == _connection.getOutletPort())
    //        {
    //          connection = _connection;
    //          return;
    //        }
    //      }
    //    }
    //  }
    //}

    //OptionalConnection ModelObject_Impl::getInletConnection(unsigned portID)
    //{
    //  ConnectionVector connectionVector;
    //  connectionVector = Connection::getConnection(model());
    //  OptionalConnection connection;
    //
    //  for (connection : connectionVector)
    //  {
    //    if(connection->getTargetModelObject())
    //    {
    //      if(connection->getTargetModelObject()->handle() == handle())
    //      {
    //        if(connection->getInletPort() == portID)
    //        {
    //          return connection;
    //        }
    //      }
    //    }
    //  }
    //  connection = Connection(model());
    //  openstudio::Handle h = this->handle();
    //  ModelObject modelObject = model().getModelObject(h).get();
    //  connection->targetModelObject(modelObject);
    //  connection->inletPort(portID);
    //  return connection;
    //}

    //void ModelObject_Impl::setInletConnection(Connection _connection)
    //{
    //  ConnectionVector connectionVector;
    //  connectionVector = Connection::getConnection(model());
    //  OptionalConnection connection;
    //
    //  for (connection : connectionVector)
    //  {
    //    if(connection->getTargetModelObject())
    //    {
    //      if(connection->getTargetModelObject()->handle() == handle())
    //      {
    //        if(connection->getInletPort() == _connection.getInletPort())
    //        {
    //          connection = _connection;
    //          return;
    //        }
    //      }
    //    }
    //  }
    //}

    //boost::optional<ModelObject> ModelObject_Impl::getOutletModelObject(unsigned outletPortID)
    //{
    //  boost::optional<ModelObject> obj;
    //  OptionalConnection connection;
    //  connection = getOutletConnection(outletPortID);
    //  if(connection)
    //  {
    //    obj = connection->getTargetModelObject();
    //  }
    //  return obj;
    //}

    //boost::optional<ModelObject> ModelObject_Impl::getInletModelObject(unsigned inletPortID)
    //{
    //  OptionalConnection connection;
    //  OptionalModelObject obj;
    //  if(connection = getInletConnection(inletPortID))
    //  {
    //    obj = connection->getSourceModelObject();
    //  }
    //  return obj;
    //}

    boost::optional<ModelObject> ModelObject_Impl::connectedObject(unsigned port) const {
      OptionalWorkspaceObject wo = this->getTarget(port);
      if (wo) {

        auto connection = wo->cast<Connection>();

        boost::optional<ModelObject> targetObject = connection.targetObject();
        boost::optional<ModelObject> sourceObject = connection.sourceObject();
        if (sourceObject && targetObject) {
          if (sourceObject->handle() == this->handle()) {
            return targetObject;
          }
          if (targetObject->handle() == this->handle()) {
            return sourceObject;
          }
        }
      }
      return {};
    }

    boost::optional<unsigned> ModelObject_Impl::connectedObjectPort(unsigned port) const {
      OptionalWorkspaceObject wo = this->getTarget(port);
      if (wo) {
        auto connection = wo->cast<Connection>();

        if (boost::optional<ModelObject> sourceObject = connection.sourceObject()) {
          if (sourceObject->handle() == this->handle()) {
            return connection.targetObjectPort();
          }
        }
        if (boost::optional<ModelObject> targetObject = connection.targetObject()) {
          if (targetObject->handle() == this->handle()) {
            return connection.sourceObjectPort();
          }
        }
      }
      return {};
    }

    ModelObject ModelObject_Impl::clone(Model model) const {
      // UniqueModelObject.
      if (this->iddObject().properties().unique) {
        Model m = this->model();
        if (model == m) {
          // Return self
          LOG(Info, "Cannot clone a UniqueModelObject into the same model, returning self, for " << briefDescription());
          return getObject<ModelObject>();
        } else {
          // Remove any existing objects (there should really be only one)
          for (auto& wo : model.getObjectsByType(this->iddObject())) {
            LOG(Info, "Removing existing UniqueModelObject in the target model: " << wo.briefDescription());
            wo.remove();
          }
        }
      }

      // Business as usual...

      WorkspaceObjectVector result;

      // No children because ModelObject.
      // Can have resources and costs.
      WorkspaceObjectVector toAdd(1u, getObject<WorkspaceObject>());

      // add costs
      std::vector<LifeCycleCost> lifeCycleCosts = this->lifeCycleCosts();
      toAdd.insert(toAdd.end(), lifeCycleCosts.begin(), lifeCycleCosts.end());

      // add additional properties
      AdditionalPropertiesVector props = getObject<ModelObject>().getModelObjectSources<AdditionalProperties>();
      toAdd.insert(toAdd.end(), props.begin(), props.end());

      std::string s = toString(this->handle());

      // If same model, non-recursive insert of resources should be sufficient.
      Model m = this->model();
      if (model == m) {
        ResourceObjectVector resources = this->resources();
        WorkspaceObjectVector toInsert = castVector<WorkspaceObject>(resources);
        result = m.addAndInsertObjects(toAdd, toInsert);
        // adding this better have worked
        OS_ASSERT(result.size() == 1u + lifeCycleCosts.size() + props.size() + resources.size());
        // inserting resources better have worked
        unsigned i = 1 + lifeCycleCosts.size() + props.size();
        for (const ResourceObject& resource : resources) {
          OS_ASSERT(result[i] == resource);
          ++i;
        }
        return result[0].cast<ModelObject>();
      }

      // Not the same model. Resource handling is more complicated.
      result = model.addAndInsertObjects(toAdd, castArray<WorkspaceObject>(getRecursiveResourceSubTrees(getObject<ModelObject>(), true)));
      // Operation should work.
      OS_ASSERT(!result.empty());
      return result[0].cast<ModelObject>();
    }

    /// remove the object from the model, also removes any cost objects associated with this object
    /// return std::vector<IdfObject> containing any removed object(s)
    std::vector<IdfObject> ModelObject_Impl::remove() {
      // We need to **guarantee** that WorkspaceObject_Impl::remove() is called last. C++ does not guarantee the order in which function parameters
      // are evaluated, so I can't pass the three remove to concat<IdfObject>(Args&&...) (on GCC it was calling Workspace_Impl::remove first)
      auto result = concat<IdfObject>(this->removeLifeCycleCosts(), this->removeAdditionalProperties());
      openstudio::detail::concat_helper(result, WorkspaceObject_Impl::remove());
      return result;
    }

    Component ModelObject_Impl::createComponent() const {
      // component contents is defined by clone
      Model temp;
      ModelObject cloneOfThis = this->clone(temp);
      // ensure that the first object in contents is cloneOfThis (the primary object).
      ModelObjectVector contents = castVector<ModelObject>(temp.objects(true));
      OS_ASSERT(contents[0] == cloneOfThis);
      return Component(contents);
    }

    boost::optional<ParentObject> ModelObject_Impl::parent() const {
      return {};
    }

    bool ModelObject_Impl::setParent(ParentObject& /*newParent*/) {
      return false;
    }

    ModelObject_Impl::ModelObject_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : WorkspaceObject_Impl(idfObject, model, keepHandle) {}

    ModelObject_Impl::ModelObject_Impl(IddObjectType type, Model_Impl* model) : WorkspaceObject_Impl(IdfObject(type), model) {}

    bool ModelObject_Impl::getBooleanFieldValue(unsigned index) const {
      OptionalString oStr = getString(index, true);
      OS_ASSERT(oStr);
      std::string temp = *oStr;
      boost::to_lower(temp);
      if (temp == "yes") {
        return true;
      }
      if (temp == "no") {
        return false;
      }
      LOG_AND_THROW("Unable to deduce a value for boolean field " << index << " in " << briefDescription() << ".");
      return false;
    }

    bool ModelObject_Impl::setBooleanFieldValue(unsigned index, bool value) {
      bool ok(true);
      if (value) {
        ok = setString(index, "Yes");
      } else {
        ok = setString(index, "No");
      }
      if (!ok) {
        LOG_AND_THROW("Unable to set boolean field " << index << " in " << briefDescription() << ".");
      }
      return true;
    }

    bool ModelObject_Impl::setSchedule(unsigned index, const std::string& className, const std::string& scheduleDisplayName, Schedule& schedule) {
      if (schedule.model() != model()) {
        return false;
      }
      bool result = checkOrAssignScheduleTypeLimits(className, scheduleDisplayName, schedule);
      if (!result) {
        if (boost::optional<ScheduleTypeLimits> scheduleTypeLimits = schedule.scheduleTypeLimits()) {
          LOG(Warn, "For " << briefDescription() << " cannot set Schedule " << scheduleDisplayName << "=" << schedule.nameString()
                           << " because it has an incompatible ScheduleTypeLimits");
        }
        return result;
      }
      return setPointer(index, schedule.handle());
    }

    boost::optional<ModelObject> ModelObject_Impl::parentAsModelObject() const {
      boost::optional<ModelObject> result;
      OptionalParentObject intermediate = parent();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    std::vector<ModelObject> ModelObject_Impl::resourcesAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(resources());
      return result;
    }

    bool ModelObject_Impl::setParentAsModelObject(boost::optional<ModelObject> modelObject) {
      if (modelObject) {
        OptionalParentObject intermediate = modelObject->optionalCast<ParentObject>();
        if (intermediate) {
          return setParent(*intermediate);
        }
      }
      return false;
    }

    std::vector<ScheduleTypeKey> ModelObject_Impl::getScheduleTypeKeys(const Schedule& /*schedule*/) const {
      return {};
    }

    std::vector<EMSActuatorNames> ModelObject_Impl::emsActuatorNames() const {
      return {};
    }

    std::vector<std::string> ModelObject_Impl::emsInternalVariableNames() const {
      return {};
    }

    AdditionalProperties ModelObject_Impl::additionalProperties() const {
      AdditionalPropertiesVector candidates = getObject<ModelObject>().getModelObjectSources<AdditionalProperties>();
      if (candidates.size() > 1) {
        for (unsigned i = 1, n = candidates.size(); i < n; ++i) {
          // do a merge before removing
          candidates[0].merge(candidates[i]);
          candidates[i].remove();
        }
        LOG(Warn, "Removed extraneous ModelObjectAdditionalProperties objects pointing to " << briefDescription() << ".");
      }
      if (!candidates.empty()) {
        return candidates[0];
      }
      return AdditionalProperties(getObject<ModelObject>());
    }

    std::vector<IdfObject> ModelObject_Impl::removeAdditionalProperties() {
      std::vector<IdfObject> removed;
      AdditionalPropertiesVector candidates = getObject<ModelObject>().getModelObjectSources<AdditionalProperties>();
      for (AdditionalProperties& candidate : candidates) {
        std::vector<IdfObject> tmp = candidate.remove();
        openstudio::detail::concat_helper(removed, std::move(tmp));
      }
      return removed;
    }

    bool ModelObject_Impl::hasAdditionalProperties() const {
      AdditionalPropertiesVector candidates = getObject<ModelObject>().getModelObjectSources<AdditionalProperties>();
      return !candidates.empty();
    }

    boost::optional<std::string> ModelObject_Impl::cadObjectId() const {
      boost::optional<std::string> result;
      if (hasAdditionalProperties()) {
        AdditionalProperties additionalProperties = this->additionalProperties();
        if (additionalProperties.hasFeature("CADObjectId")) {
          result = additionalProperties.getFeatureAsString("CADObjectId");
        }
      }
      return result;
    }

    bool ModelObject_Impl::setCADObjectId(const std::string& cadObjectId) {
      return this->additionalProperties().setFeature("CADObjectId", cadObjectId);
    }

    boost::optional<std::string> ModelObject_Impl::gbXMLId() const {
      boost::optional<std::string> result;
      if (hasAdditionalProperties()) {
        AdditionalProperties additionalProperties = this->additionalProperties();
        if (additionalProperties.hasFeature("gbXMLId")) {
          result = additionalProperties.getFeatureAsString("gbXMLId");
        }
      }
      return result;
    }

    bool ModelObject_Impl::setGBXMLId(const std::string& gbXMLId) {
      return this->additionalProperties().setFeature("gbXMLId", gbXMLId);
    }

    boost::optional<std::string> ModelObject_Impl::displayName() const {
      boost::optional<std::string> result;
      if (hasAdditionalProperties()) {
        AdditionalProperties additionalProperties = this->additionalProperties();
        if (additionalProperties.hasFeature("displayName")) {
          result = additionalProperties.getFeatureAsString("displayName");
        }
      }
      return result;
    }

    bool ModelObject_Impl::setDisplayName(const std::string& displayName) {
      return this->additionalProperties().setFeature("displayName", displayName);
    }

  }  // namespace detail

  bool ModelObject::operator<(const ModelObject& right) const {
    return (getImpl<detail::ModelObject_Impl>()) < (right.getImpl<detail::ModelObject_Impl>());
  }

  // GETTERS

  // DLM@20101011: todo, fix later
  Model ModelObject::model() const {
    return getImpl<detail::ModelObject_Impl>()->model();
  }

  std::vector<LifeCycleCost> ModelObject::lifeCycleCosts() const {
    return getImpl<detail::ModelObject_Impl>()->lifeCycleCosts();
  }

  std::vector<IdfObject> ModelObject::removeLifeCycleCosts() {
    return getImpl<detail::ModelObject_Impl>()->removeLifeCycleCosts();
  }

  IddObjectType ModelObject::iddObjectType() const {
    return getImpl<detail::ModelObject_Impl>()->iddObjectType();
  }

  std::vector<ResourceObject> ModelObject::resources() const {
    return getImpl<detail::ModelObject_Impl>()->resources();
  }

  /** Get all output variables names that could be associated with this object.
 *   Each concrete class should override this.*/
  const std::vector<std::string>& ModelObject::outputVariableNames() const {
    return getImpl<detail::ModelObject_Impl>()->outputVariableNames();
  }

  /** Get all output variables associated with this object, must run simulation to generate associate data. */
  OutputVariableVector ModelObject::outputVariables() const {
    return getImpl<detail::ModelObject_Impl>()->outputVariables();
  }

  /** Get data associated with this output variable and this object. */
  openstudio::OptionalTimeSeries ModelObject::getData(const OutputVariable& variable, const std::string& envPeriod) const {
    return getImpl<detail::ModelObject_Impl>()->getData(variable, envPeriod);
  }

  /// equality test
  bool ModelObject::operator==(const ModelObject& other) const {
    return (handle() == other.handle());
  }

  /// in-equality test
  bool ModelObject::operator!=(const ModelObject& other) const {
    return (handle() != other.handle());
  }

  std::vector<ScheduleTypeKey> ModelObject::getScheduleTypeKeys(const Schedule& schedule) const {
    return getImpl<detail::ModelObject_Impl>()->getScheduleTypeKeys(schedule);
  }

  boost::optional<ModelObject> ModelObject::connectedObject(unsigned port) const {
    return getImpl<detail::ModelObject_Impl>()->connectedObject(port);
  }

  boost::optional<unsigned> ModelObject::connectedObjectPort(unsigned port) const {
    return getImpl<detail::ModelObject_Impl>()->connectedObjectPort(port);
  }

  ModelObject ModelObject::clone() const {
    return getImpl<detail::ModelObject_Impl>()->clone(model());
  }

  ModelObject ModelObject::clone(Model model) const {
    return getImpl<detail::ModelObject_Impl>()->clone(model);
  }

  Component ModelObject::createComponent() const {
    return getImpl<detail::ModelObject_Impl>()->createComponent();
  }

  OptionalParentObject ModelObject::parent() const {
    return getImpl<detail::ModelObject_Impl>()->parent();
  }

  bool ModelObject::setParent(ParentObject& newParent) {
    return getImpl<detail::ModelObject_Impl>()->setParent(newParent);
  }

  AdditionalProperties ModelObject::additionalProperties() const {
    return getImpl<detail::ModelObject_Impl>()->additionalProperties();
  }

  bool ModelObject::hasAdditionalProperties() const {
    return getImpl<detail::ModelObject_Impl>()->hasAdditionalProperties();
  }

  std::vector<IdfObject> ModelObject::removeAdditionalProperties() {
    return getImpl<detail::ModelObject_Impl>()->removeAdditionalProperties();
  }

  boost::optional<std::string> ModelObject::cadObjectId() const {
    return getImpl<detail::ModelObject_Impl>()->cadObjectId();
  }

  bool ModelObject::setCADObjectId(const std::string& cadObjectId) {
    return getImpl<detail::ModelObject_Impl>()->setCADObjectId(cadObjectId);
  }

  boost::optional<std::string> ModelObject::gbXMLId() const {
    return getImpl<detail::ModelObject_Impl>()->gbXMLId();
  }

  bool ModelObject::setGBXMLId(const std::string& gbXMLId) {
    return getImpl<detail::ModelObject_Impl>()->setGBXMLId(gbXMLId);
  }

  boost::optional<std::string> ModelObject::displayName() const {
    return getImpl<detail::ModelObject_Impl>()->displayName();
  }

  bool ModelObject::setDisplayName(const std::string& displayName) {
    return getImpl<detail::ModelObject_Impl>()->setDisplayName(displayName);
  }

  ModelObject::ModelObject(IddObjectType type, const Model& model, bool fastName)
    : WorkspaceObject(model.getImpl<detail::Model_Impl>()->createObject(IdfObject(type, fastName), false)) {
    // create object of correct type
    OS_ASSERT(getImpl<detail::ModelObject_Impl>());
    OS_ASSERT(!getImpl<detail::ModelObject_Impl>()->initialized());
    // add to Workspace
    openstudio::detail::WorkspaceObject_ImplPtrVector impls;
    impls.push_back(getImpl<openstudio::detail::WorkspaceObject_Impl>());
    model.getImpl<detail::Model_Impl>()->addObjects(impls, false);
    // object should be initialized and ready to go
    if (!getImpl<detail::ModelObject_Impl>()->initialized()) {
      LOG_AND_THROW("ModelObject not initialized: " << '\n' << getImpl<openstudio::detail::WorkspaceObject_Impl>()->idfObject());
    }
  }

  ModelObject::ModelObject(std::shared_ptr<detail::ModelObject_Impl> p) : WorkspaceObject(std::move(p)) {}

  /** Gets the autosized component value from the sql file **/
  boost::optional<double> ModelObject::getAutosizedValue(const std::string& valueName, const std::string& units) const {
    return getImpl<detail::ModelObject_Impl>()->getAutosizedValue(valueName, units);
  }

  std::vector<EMSActuatorNames> ModelObject::emsActuatorNames() const {
    return getImpl<detail::ModelObject_Impl>()->emsActuatorNames();
  }

  std::vector<std::string> ModelObject::emsInternalVariableNames() const {
    return getImpl<detail::ModelObject_Impl>()->emsInternalVariableNames();
  }

  EMSActuatorNames::EMSActuatorNames(const std::string& componentTypeName, const std::string& controlTypeName)
    : m_componentTypeName(componentTypeName), m_controlTypeName(controlTypeName) {}

  std::string EMSActuatorNames::controlTypeName() const {
    return m_controlTypeName;
  }

  std::string EMSActuatorNames::componentTypeName() const {
    return m_componentTypeName;
  }

  ScheduleTypeKey::ScheduleTypeKey(const std::string& className, const std::string& scheduleDisplayName)
    : m_className(className), m_scheduleDisplayName(scheduleDisplayName) {}
  std::string ScheduleTypeKey::className() const {
    return m_className;
  }
  std::string ScheduleTypeKey::scheduleDisplayName() const {
    return m_scheduleDisplayName;
  }

}  // namespace model
}  // namespace openstudio
