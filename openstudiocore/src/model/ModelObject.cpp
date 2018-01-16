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

#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"
#include "Model_Impl.hpp"
#include "Component.hpp"
#include "LifeCycleCost.hpp"
#include "LifeCycleCost_Impl.hpp"
// #include "Relationship.hpp"
#include "ParentObject.hpp"
#include "ResourceObject.hpp"
#include "ResourceObject_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "CoilCoolingDXTwoStageWithHumidityControlMode.hpp"
#include "CoilCoolingDXTwoStageWithHumidityControlMode_Impl.hpp"
#include "CoilPerformanceDXCooling.hpp"
#include "CoilPerformanceDXCooling_Impl.hpp"


#include "ScheduleTypeRegistry.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"

#include "OutputVariable.hpp"
#include "OutputVariable_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/Attribute.hpp"
#include "../utilities/sql/SqlFileEnums.hpp"
#include "../utilities/sql/SqlFileTimeSeriesQuery.hpp"
#include "../utilities/sql/SqlFile.hpp"
#include "../utilities/idf/ValidityReport.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

#include <utilities/idd/OS_Output_Variable_FieldEnums.hxx>
#include "../utilities/data/TimeSeries.hpp"

#include <QMetaProperty>

#include <boost/algorithm/string.hpp>

using openstudio::Handle;
using openstudio::HandleVector;
using openstudio::WorkspaceObject;
using openstudio::OptionalWorkspaceObject;
using openstudio::WorkspaceObjectVector;
using openstudio::Workspace;

using std::vector;

// calls to register the meta types
// int __metaType1__ = qRegisterMetaType<boost::optional<openstudio::model::ModelObject> >("boost::optional<openstudio::model::ModelObject>");
// int __metaType2__ = qRegisterMetaType<std::vector<openstudio::model::ModelObject> >("std::vector<openstudio::model::ModelObject>" );

namespace openstudio {
namespace model {
namespace detail {

  ModelObject_Impl::ModelObject_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : WorkspaceObject_Impl(other,model,keepHandle)
  {}

  ModelObject_Impl::ModelObject_Impl(const ModelObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : WorkspaceObject_Impl(other,model,keepHandle)
  {}

  // GETTERS

  Model ModelObject_Impl::model() const {
    return workspace().cast<Model>();
  }

  std::vector<LifeCycleCost> ModelObject_Impl::lifeCycleCosts() const
  {
    return getObject<ModelObject>().getModelObjectSources<LifeCycleCost>();
  }

  std::vector<IdfObject> ModelObject_Impl::removeLifeCycleCosts()
  {
    std::vector<IdfObject> removedCosts;
    std::vector<LifeCycleCost> lifeCycleCosts = this->lifeCycleCosts();
    for (LifeCycleCost& lifeCycleCost : lifeCycleCosts){
      std::vector<IdfObject> tmp = lifeCycleCost.remove();
      removedCosts.insert(removedCosts.end(), tmp.begin(), tmp.end());
    }
    return removedCosts;
  }

  // std::vector<std::string> ModelObject_Impl::attributeNames() const {
  //   StringVector result;
  //   const QMetaObject* metaobject = metaObject();
  //   int n = metaobject->propertyCount();
  //   for (int i = 0; i < n; ++i) {
  //     QMetaProperty metaproperty = metaobject->property(i);
  //     std::string typeName = metaproperty.typeName();

  //     // if it is a model object it should be a relationship rather than an attribute
  //     // if it is a string vector, it is used as a Qt Property, not as an attribute
  //     if ((typeName != "boost::optional<openstudio::model::ModelObject>") &&
  //         (typeName != "std::vector<openstudio::model::ModelObject>") &&
  //         (typeName != "std::vector<std::string>")){
  //       std::string candidate(metaproperty.name());
  //       if (candidate != "objectName") { // filter out QOBJECT property
  //         result.push_back(candidate);
  //       }
  //     }
  //   }
  //   return result;
  // }

  // std::vector<openstudio::Attribute> ModelObject_Impl::attributes() const {
  //   std::vector<openstudio::Attribute> result;
  //   const QMetaObject* metaobject = metaObject();
  //   int n = metaobject->propertyCount();
  //   for (int i = 0; i < n; ++i) {
  //     QMetaProperty metaproperty = metaobject->property(i);
  //     std::string typeName = metaproperty.typeName();

  //     // if it is a model object it should be a relationship rather than an attribute
  //     if ((typeName != "boost::optional<openstudio::model::ModelObject>") &&
  //         (typeName != "std::vector<openstudio::model::ModelObject>") &&
  //         (typeName != "std::vector<std::string>")){
  //       std::string candidate(metaproperty.name());
  //       if (candidate != "objectName") { // filter out QOBJECT property
  //         try {
  //           QVariant value = metaproperty.read(this);
  //           boost::optional<openstudio::Attribute> attribute = openstudio::Attribute::fromQVariant(candidate, value);
  //           if(attribute){
  //             result.push_back(*attribute);
  //           }
  //         }
  //         catch (...) {}
  //       }
  //     }
  //   }
  //   return result;
  // }

  // boost::optional<openstudio::Attribute> ModelObject_Impl::getAttribute(const std::string& name) const
  // {
  //   boost::optional<openstudio::Attribute> result;

  //   const QMetaObject* metaobject = this->metaObject();
  //   int index = metaobject->indexOfProperty(name.c_str());
  //   if (index < 0){
  //     return result;
  //   }
  //   QMetaProperty metaproperty = metaobject->property(index);
  //   OS_ASSERT(metaproperty.isValid());
  //   std::string typeName = metaproperty.typeName();

  //   // if it is a model object it should be a relationship rather than an attribute
  //   if ((typeName != "boost::optional<openstudio::model::ModelObject>") &&
  //       (typeName != "std::vector<openstudio::model::ModelObject>") &&
  //       (typeName != "std::vector<std::string>")){
  //     if (name != "objectName") { // filter out QOBJECT property
  //       try {
  //         QVariant value = metaproperty.read(this);
  //         result = openstudio::Attribute::fromQVariant(name, value);
  //       }
  //       catch(...) {}
  //     }
  //   }

  //   return result;
  // }

  // bool ModelObject_Impl::isSettableAttribute(const std::string& name) const
  // {
  //   bool result = false;

  //   const QMetaObject* metaobject = this->metaObject();
  //   int index = metaobject->indexOfProperty(name.c_str());
  //   if (index < 0){
  //     return result;
  //   }
  //   QMetaProperty metaproperty = metaobject->property(index);
  //   OS_ASSERT(metaproperty.isValid());
  //   std::string typeName = metaproperty.typeName();

  //   // if it is a model object it should be a relationship rather than an attribute
  //   if ((typeName != "boost::optional<openstudio::model::ModelObject>") &&
  //       (typeName != "std::vector<openstudio::model::ModelObject>") &&
  //       (typeName != "std::vector<std::string>")){
  //     if (name != "objectName") { // filter out QOBJECT property
  //       result = metaproperty.isWritable();
  //     }
  //   }

  //   return result;
  // }

  // bool ModelObject_Impl::isOptionalAttribute(const std::string& name) const
  // {
  //   bool result = false;

  //   const QMetaObject* metaobject = this->metaObject();
  //   int index = metaobject->indexOfProperty(name.c_str());
  //   if (index < 0){
  //     return result;
  //   }
  //   QMetaProperty metaproperty = metaobject->property(index);
  //   OS_ASSERT(metaproperty.isValid());

  //   std::string typeName = metaproperty.typeName();

  //   // may need to convert from type to optional type
  //   if (typeName == "boost::optional<int>") {
  //     result = true;
  //   }else if (typeName == "boost::optional<unsigned>") {
  //     result = true;
  //   }else if (typeName == "boost::optional<double>") {
  //     result = true;
  //   }else if (typeName == "boost::optional<std::string>") {
  //     result = true;
  //   }else if (typeName == "openstudio::OSOptionalQuantity") {
  //     result = true;
  //   }else if (typeName == "boost::optional<openstudio::Attribute>") {
  //     result = true;
  //   }

  //   return result;
  // }

  // bool ModelObject_Impl::setAttribute(const std::string& name, bool value) {
  //   return setAttribute(name, QVariant::fromValue(value));
  // }

  // bool ModelObject_Impl::setAttribute(const std::string& name, int value) {
  //   return setAttribute(name, QVariant::fromValue(value));
  // }

  // bool ModelObject_Impl::setAttribute(const std::string& name, unsigned value) {
  //   return setAttribute(name, QVariant::fromValue(value));
  // }

  // bool ModelObject_Impl::setAttribute(const std::string& name, double value) {
  //   return setAttribute(name, QVariant::fromValue(value));
  // }

  // bool ModelObject_Impl::setAttribute(const std::string& name, const Quantity& value) {
  //   return setAttribute(name, QVariant::fromValue(value));
  // }

  // bool ModelObject_Impl::setAttribute(const std::string& name, const std::string& value) {
  //   return setAttribute(name, QVariant::fromValue(value));
  // }

  // bool ModelObject_Impl::setAttribute(const std::string& name, const char* value) {
  //   return setAttribute(name, QVariant::fromValue(std::string(value)));
  // }

  // bool ModelObject_Impl::setAttribute(const std::string& name, const QVariant& value)
  // {
  //   bool result = false;

  //   const QMetaObject* metaobject = this->metaObject();
  //   int index = metaobject->indexOfProperty(name.c_str());
  //   if (index < 0){
  //     return result;
  //   }
  //   QMetaProperty metaproperty = metaobject->property(index);
  //   OS_ASSERT(metaproperty.isValid());

  //   if (!metaproperty.isWritable()){
  //     return result;
  //   }

  //   std::string typeName = metaproperty.typeName();

  //   QVariant finalValue = value;

  //   // may need to convert from type to optional type
  //   if (typeName == "boost::optional<int>") {
  //     if (value.canConvert<int>()){
  //       boost::optional<int> newValue = value.value<int>();
  //       finalValue = QVariant::fromValue(newValue);
  //     }
  //   }else if (typeName == "boost::optional<unsigned>") {
  //     if (value.canConvert<unsigned>()){
  //       boost::optional<unsigned> newValue = value.value<unsigned>();
  //       finalValue = QVariant::fromValue(newValue);
  //     }
  //   }else if (typeName == "boost::optional<double>") {
  //     if (value.canConvert<double>()){
  //       boost::optional<double> newValue = value.value<double>();
  //       finalValue = QVariant::fromValue(newValue);
  //     }
  //   }else if (typeName == "boost::optional<std::string>") {
  //     if (value.canConvert<std::string>()){
  //       boost::optional<std::string> newValue = value.value<std::string>();
  //       finalValue = QVariant::fromValue(newValue);
  //     }
  //   }else if (typeName == "openstudio::OSOptionalQuantity") {
  //     if (value.canConvert<openstudio::Quantity>()) {
  //       openstudio::OSOptionalQuantity newValue(value.value<openstudio::Quantity>());
  //       finalValue = QVariant::fromValue(newValue);
  //     }
  //   }else if (typeName == "boost::optional<openstudio::Attribute>") {
  //     LOG(Error, "openstudio::Attribute is not yet registered with QMetaType");
  //     //if (value.canConvert<openstudio::Attribute>()){
  //     //  boost::optional<openstudio::Attribute> newValue = value.value<openstudio::Attribute>();
  //     //  finalValue = QVariant::fromValue(newValue);
  //     //}
  //   }

  //   // if it is a model object it should be a relationship rather than an attribute
  //   if ((typeName != "boost::optional<openstudio::model::ModelObject>") &&
  //       (typeName != "std::vector<openstudio::model::ModelObject>")){
  //     if (name != "objectName") { // filter out QOBJECT property
  //       result = metaproperty.write(this, finalValue);

  //       if (result){
  //         // test that change worked
  //         QVariant newValue = metaproperty.read(this);
  //         boost::optional<openstudio::Attribute> finalAttribute = openstudio::Attribute::fromQVariant(name, finalValue);
  //         boost::optional<openstudio::Attribute> newAttribute = openstudio::Attribute::fromQVariant(name, newValue);

  //         if (finalAttribute){
  //           result = newAttribute && (*finalAttribute == *newAttribute);
  //         }else{
  //           result = !newAttribute;
  //         }
  //       }
  //     }
  //   }

  //   return result;
  // }

  // bool ModelObject_Impl::resetAttribute(const std::string& name)
  // {
  //   bool result = false;

  //   const QMetaObject* metaobject = this->metaObject();
  //   int index = metaobject->indexOfProperty(name.c_str());
  //   if (index < 0){
  //     return result;
  //   }
  //   QMetaProperty metaproperty = metaobject->property(index);
  //   OS_ASSERT(metaproperty.isValid());

  //   if (!metaproperty.isWritable()){
  //     return result;
  //   }

  //   std::string typeName = metaproperty.typeName();

  //   QVariant finalValue;

  //   // may need to convert from type to optional type
  //   if (typeName == "boost::optional<int>") {
  //     finalValue = QVariant::fromValue(boost::optional<int>());
  //   }else if (typeName == "boost::optional<unsigned>") {
  //     finalValue = QVariant::fromValue(boost::optional<unsigned>());
  //   }else if (typeName == "boost::optional<double>") {
  //     finalValue = QVariant::fromValue(boost::optional<double>());
  //   }else if (typeName == "boost::optional<std::string>") {
  //     finalValue = QVariant::fromValue(boost::optional<std::string>());
  //   }else if (typeName == "openstudio::OSOptionalQuantity") {
  //     finalValue = QVariant::fromValue(openstudio::OSOptionalQuantity());
  //   }else if (typeName == "boost::optional<openstudio::Attribute>") {
  //     finalValue = QVariant::fromValue(boost::optional<openstudio::Attribute>());
  //   }else{
  //     return result;
  //   }

  //   // if it is a model object it should be a relationship rather than an attribute
  //   if ((typeName != "boost::optional<openstudio::model::ModelObject>") &&
  //       (typeName != "std::vector<openstudio::model::ModelObject>") &&
  //       (typeName != "std::vector<std::string>")){
  //     if (name != "objectName") { // filter out QOBJECT property
  //       result = metaproperty.write(this, finalValue);

  //       if (result){
  //         // test that change worked
  //         QVariant newValue = metaproperty.read(this);
  //         boost::optional<openstudio::Attribute> finalAttribute = openstudio::Attribute::fromQVariant(name, finalValue);
  //         boost::optional<openstudio::Attribute> newAttribute = openstudio::Attribute::fromQVariant(name, newValue);

  //         if (finalAttribute){
  //           result = newAttribute && (*finalAttribute == *newAttribute);
  //         }else{
  //           result = !newAttribute;
  //         }
  //       }
  //     }
  //   }

  //   return result;
  // }

  // std::vector<std::string> ModelObject_Impl::relationshipNames() const
  // {
  //   StringVector result;
  //   const QMetaObject* metaobject = metaObject();
  //   int n = metaobject->propertyCount();
  //   for (int i = 0; i < n; ++i) {
  //     QMetaProperty metaproperty = metaobject->property(i);
  //     std::string typeName = metaproperty.typeName();

  //     // if it is a model object it should be a relationship rather than an attribute
  //     if ((typeName == "boost::optional<openstudio::model::ModelObject>") ||
  //         (typeName == "std::vector<openstudio::model::ModelObject>")){
  //       std::string candidate(metaproperty.name());
  //       if (candidate != "objectName") { // filter out QOBJECT property
  //         result.push_back(candidate);
  //       }
  //     }
  //   }
  //   return result;
  // }

  // std::vector<Relationship> ModelObject_Impl::relationships() const
  // {
  //   RelationshipVector result;
  //   const QMetaObject* metaobject = metaObject();
  //   int n = metaobject->propertyCount();
  //   for (int i = 0; i < n; ++i) {
  //     QMetaProperty metaproperty = metaobject->property(i);
  //     std::string typeName = metaproperty.typeName();

  //     // if it is a model object it should be a relationship rather than an attribute
  //     if ((typeName == "boost::optional<openstudio::model::ModelObject>") ||
  //         (typeName == "std::vector<openstudio::model::ModelObject>")){
  //       std::string candidate(metaproperty.name());
  //       if (candidate != "objectName") { // filter out QOBJECT property
  //         result.push_back(Relationship(getObject<ModelObject>(),candidate));
  //       }
  //     }
  //   }
  //   return result;
  // }

  // boost::optional<Relationship> ModelObject_Impl::getRelationship(const std::string& name) const
  // {
  //   boost::optional<Relationship> result;

  //   const QMetaObject* metaobject = this->metaObject();
  //   int index = metaobject->indexOfProperty(name.c_str());
  //   if (index < 0){
  //     return result;
  //   }
  //   QMetaProperty metaproperty = metaobject->property(index);
  //   OS_ASSERT(metaproperty.isValid());
  //   std::string typeName = metaproperty.typeName();

  //   // if it is a model object it should be a relationship rather than an attribute
  //   if ((typeName == "boost::optional<openstudio::model::ModelObject>") ||
  //       (typeName == "std::vector<openstudio::model::ModelObject>")){
  //     if (name != "objectName") { // filter out QOBJECT property
  //       result = Relationship(getObject<ModelObject>(), name);
  //     }
  //   }
  //   return result;
  // }

  // bool ModelObject_Impl::setRelationship(const std::string& name,
  //                                        boost::optional<ModelObject> relatedModelObject)
  // {
  //   OptionalRelationship relationship = getRelationship(name);
  //   if (relationship) {
  //     return relationship->setRelatedModelObject(relatedModelObject);
  //   }
  //   return false;
  // }

  // bool ModelObject_Impl::setRelationship(const std::string& name, const Component& component) {
  //   OptionalRelationship relationship = getRelationship(name);
  //   if (relationship) {
  //     return relationship->setRelatedModelObject(component);
  //   }
  //   return false;
  // }

  std::vector<ResourceObject> ModelObject_Impl::resources() const {
    // resources are ResourceObjects pointed to ...
    ResourceObjectVector resourceObjects = getObject<ModelObject>().getModelObjectTargets<ResourceObject>();

    // create vector of unique resource objects except for a ResourceObject that is this object's parent
    OptionalParentObject opo = parent();
    std::set<Handle> uniqueResourceHandles;
    ResourceObjectVector result;
    for (const ResourceObject& resourceObject : resourceObjects){

      Handle handle = resourceObject.handle();
      if (opo && (opo->handle() == handle)){
        continue;
      }

      auto it = uniqueResourceHandles.find(handle);
      if (it != uniqueResourceHandles.end()){
        continue;
      }

      uniqueResourceHandles.insert(handle);
      result.push_back(resourceObject);
    }

    return result;
  }

  /** Get all output variables associated with this object, must run simulation to generate associate data. */
  OutputVariableVector ModelObject_Impl::outputVariables() const
  {
    OutputVariableVector variables;
    std::vector<std::string> variableNames = this->outputVariableNames();
    OptionalString name = this->name();

    for (const OutputVariable& variable : this->model().getConcreteModelObjects<OutputVariable>()){
      std::string keyValue = variable.keyValue();
      std::string variableName = variable.variableName();

      if (name){
        if (istringEqual(*name, keyValue) || (keyValue == "*")){
          // match
        }else{
          continue; // no match
        }
      }else{
        // match
      }

      if (std::find(variableNames.begin(), variableNames.end(), variableName) != variableNames.end()){
        variables.push_back(variable);
      }
    }
    return variables;
  }

  /** Get data associated with this output variable and this object. */
  openstudio::OptionalTimeSeries ModelObject_Impl::getData(const OutputVariable& variable, const std::string& envPeriod) const
  {
    openstudio::OptionalTimeSeries result;
    OptionalSqlFile sqlFile = this->model().sqlFile();

    if (sqlFile){
      OptionalString variableName = variable.getString(OS_Output_VariableFields::VariableName);
      OptionalString keyValue = variable.getString(OS_Output_VariableFields::KeyValue);
      OptionalString reportingFrequency = variable.getString(OS_Output_VariableFields::ReportingFrequency, true);

      // solve any issues if key value is '*'
      keyValue = getSpecificKeyValue(keyValue);

      if (variableName && keyValue){
        if (!reportingFrequency){
          // default to hourly frequency
          LOG(Warn, "Variable does not specify reporting frequency, defaulting to hourly.");
          reportingFrequency = ReportingFrequency(ReportingFrequency::Hourly).valueName();
        }
        // use query interface because replaces requested strings with exact strings in database
        ReportingFrequency rf(ReportingFrequency::Hourly);
        try { ReportingFrequency rf(boost::trim_copy(*reportingFrequency)); }
        catch (...) {
          LOG(Warn, "Unrecognized reporting frequency string. Defaulting to hourly.");
        }
        SqlFileTimeSeriesQuery q(envPeriod,rf,*variableName,*keyValue);
        TimeSeriesVector tsVec = sqlFile->timeSeries(q);
        if (tsVec.size() == 1) { result = tsVec[0]; }
      }
    }

    return result;
  }

  // set name from optional string
  bool ModelObject_Impl::mf_setName(const boost::optional<std::string>& name)
  {
    bool result = false;
    if (name) {
      result = this->setName(*name);
    }
    return result;
  }

  // get specific key value from optional key value which might be '*'
  OptionalString ModelObject_Impl::getSpecificKeyValue(OptionalString keyValue) const
  {
    OptionalString name = this->name();
    if (keyValue && name){
      if (*keyValue == "*"){
        keyValue = name;
      }
    }else if (keyValue){
      if (*keyValue == "*"){
        LOG(Error, "Variable specifies '*' for key value and object has no name.");
        return OptionalString();
      }
    }else if (name){
      LOG(Warn, "Variable does not specify key value, using object name.");
      keyValue = name;
    }else{
      LOG(Error, "Variable does not specify key value and object has no name.");
      return OptionalString();
    }
    return keyValue;
  }

  /** Gets the autosized component value from the sql file **/
  boost::optional<double> ModelObject_Impl::getAutosizedValue(std::string valueName, std::string units) const {
    boost::optional < double > result;

    // Get the object name
    if (!name()) {
      LOG(Warn, "This object does not have a name, cannot retrieve the autosized value '" + valueName + "'.");
      return result;
    }

    // Get the object name and transform to the way it is recorded
    // in the sql file
    std::string sqlName = name().get();
    boost::to_upper(sqlName);

    // Get the object type and transform to the way it is recorded
    // in the sql file
    std::string sqlObjectType = iddObject().type().valueDescription();
    boost::replace_all(sqlObjectType, "OS:", "");

    // Special logic to deal with EnergyPlus inconsistencies
    if (sqlObjectType == "CoilPerformance:DX:Cooling") {
      // Get the parent object
      boost::optional<CoilCoolingDXTwoStageWithHumidityControlMode> parentCoil;
      auto coilTwoSpdHumCtrls = this->model().getConcreteModelObjects<CoilCoolingDXTwoStageWithHumidityControlMode>();
      for (const auto & coilInModel : coilTwoSpdHumCtrls) {
        // Check the coil performance objects in this coil to see if one of them is this object
        auto coilPerf = coilInModel.normalModeStage1CoilPerformance();
        if (coilPerf) {
          if (coilPerf->handle() == this->handle()) {
            parentCoil = coilInModel;
            break;
          }
        }

        coilPerf = coilInModel.normalModeStage1Plus2CoilPerformance();
        if (coilPerf) {
          if (coilPerf->handle() == this->handle()) {
            parentCoil = coilInModel;
            break;
          }
        }

        coilPerf = coilInModel.dehumidificationMode1Stage1CoilPerformance();
        if (coilPerf) {
          if (coilPerf->handle() == this->handle()) {
            parentCoil = coilInModel;
            break;
          }
        }

        coilPerf = coilInModel.dehumidificationMode1Stage1Plus2CoilPerformance();
        if (coilPerf) {
          if (coilPerf->handle() == this->handle()) {
            parentCoil = coilInModel;
            break;
          }
        }
      }

      if (!parentCoil) {
        LOG(Warn, "The CoilPerformance:DX:Cooling object called " + sqlName + " does not have a parent CoilCoolingDXTwoStageWithHumidityControlMode, cannot retrieve the autosized value.");
        return result;
      }

      std::string parSqlName = parentCoil->name().get();
      boost::to_upper(parSqlName);
      // Join the parent and child object names, like:
      // COIL COOLING DX TWO STAGE WITH HUMIDITY CONTROL MODE 1:COIL PERFORMANCE DX COOLING 1
      sqlName = parSqlName + std::string(":") + sqlName;
    }

    // Check that the model has a sql file
    if (!model().sqlFile()) {
      LOG(Warn, "This model has no sql file, cannot retrieve the autosized value '" + valueName + "'.");
      return result;
    }

    // Query the Intialization Summary -> Component Sizing table to get
    // the row names that contains information for this component.
    std::stringstream rowsQuery;
    rowsQuery << "SELECT RowName ";
    rowsQuery << "FROM tabulardatawithstrings ";
    rowsQuery << "WHERE ReportName='Initialization Summary' ";
    rowsQuery << "AND ReportForString='Entire Facility' ";
    rowsQuery << "AND TableName = 'Component Sizing Information' ";
    rowsQuery << "AND Value='" + sqlName + "'";

    boost::optional<std::vector<std::string>> rowNames = model().sqlFile().get().execAndReturnVectorOfString(rowsQuery.str());

    // Warn if the query failed
    if (!rowNames) {
      LOG(Warn, "Could not find a component called '" + sqlName + "' in any rows of the Initialization Summary Component Sizing table.");
      return result;
    }

    // Query each row of the Intialization Summary -> Component Sizing table
    // that contains this component to get the desired value.
    std::string valueNameAndUnits = valueName + std::string(" [") + units + std::string("]");
    if (units == "") {
      valueNameAndUnits = valueName;
    } else if (units == "typo_in_energyplus") {
      valueNameAndUnits = valueName + std::string(" []");
    }

    for (std::string rowName : rowNames.get()) {
      std::stringstream rowCheckQuery;
      rowCheckQuery << "SELECT Value ";
      rowCheckQuery << "FROM tabulardatawithstrings ";
      rowCheckQuery << "WHERE ReportName='Initialization Summary' ";
      rowCheckQuery << "AND ReportForString='Entire Facility' ";
      rowCheckQuery << "AND TableName = 'Component Sizing Information' ";
      rowCheckQuery << "AND RowName='" << rowName << "' ";
      rowCheckQuery << "AND Value='" << valueNameAndUnits << "'";
      boost::optional<std::string> rowValueName = model().sqlFile().get().execAndReturnFirstString(rowCheckQuery.str());
      // Check if the query succeeded
      if (!rowValueName) {
        continue;
      }
      // This is the right row
      std::stringstream valQuery;
      valQuery << "SELECT Value ";
      valQuery << "FROM tabulardatawithstrings ";
      valQuery << "WHERE ReportName='Initialization Summary' ";
      valQuery << "AND ReportForString='Entire Facility' ";
      valQuery << "AND TableName = 'Component Sizing Information' ";
      valQuery << "AND ColumnName='Value' ";
      valQuery << "AND RowName='" << rowName << "' ";
      boost::optional<double> val = model().sqlFile().get().execAndReturnFirstDouble(valQuery.str());
      // Check if the query succeeded
      if (val) {
        result = val.get();
        break;
      }
    }


    if (!result) {
      LOG(Debug, "The autosized value query for " + valueNameAndUnits + " of " + sqlName + " returned no value.");
    }

    return result;

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

  boost::optional<ModelObject> ModelObject_Impl::connectedObject(unsigned port) const
  {
    OptionalWorkspaceObject wo = this->getTarget(port);
    if ( wo ) {

      Connection connection = wo->cast<Connection>();

      boost::optional<ModelObject> targetObject = connection.targetObject();
      boost::optional<ModelObject> sourceObject = connection.sourceObject();
      if( sourceObject && targetObject ) {
        if( sourceObject->handle() == this->handle() ){
          return targetObject;
        }
        if( targetObject->handle() == this->handle() ){
          return sourceObject;
        }
      }
    }
    return boost::optional<ModelObject>();
  }

  boost::optional<unsigned> ModelObject_Impl::connectedObjectPort(unsigned port) const
  {
    OptionalWorkspaceObject wo = this->getTarget(port);
    if(wo)
    {
      Connection connection = wo->cast<Connection>();

      if( boost::optional<ModelObject> sourceObject = connection.sourceObject() ) {
        if( sourceObject->handle() == this->handle() ){
          return connection.targetObjectPort();
        }
      }
      if( boost::optional<ModelObject> targetObject = connection.targetObject() ) {
        if( targetObject->handle() == this->handle() ){
          return connection.sourceObjectPort();
        }
      }
    }
    return boost::optional<unsigned>();
  }


  // void ModelObject_Impl::requestProperties(const QStringList& names)
  // {
  //   QVariantMap properties;

  //   QStringList::const_iterator it = names.begin();
  //   QStringList::const_iterator itend = names.end();
  //   for (; it != itend; ++it){
  //     QVariant value = QObject::property(it->toStdString().c_str());
  //     if (value.isNull() || !value.isValid()){
  //       LOG(Warn, "ModelObject of type '" << this->iddObjectType().valueName() << "' does not have property '" << it->toStdString() << "'");
  //     }
  //     properties.insert(*it, value);
  //   }
  //   this->reportProperties.nano_emit(properties);
  // }

  // void ModelObject_Impl::setProperties(const QVariantMap& properties)
  // {
  //   bool anyFailures = false;

  //   QVariantMap::const_iterator it = properties.begin();
  //   QVariantMap::const_iterator itend = properties.end();
  //   for (; it != itend; ++it){
  //     bool test = QObject::setProperty(it.key().toStdString().c_str(), it.value());
  //     if (!test){
  //       anyFailures = true;
  //       LOG(Warn, "Failed to set property '" << it.key().toStdString() << "' for ModelObject of type '" << this->iddObjectType().valueName() << "'");
  //     }
  //   }

  //   if (anyFailures){
  //     this->onChange.nano_emit();
  //   }
  // }

  ModelObject ModelObject_Impl::clone(Model model) const
  {
    WorkspaceObjectVector result;

    // No children because ModelObject.
    // Can have resources and costs.
    WorkspaceObjectVector toAdd(1u,getObject<WorkspaceObject>());

    // add costs
    std::vector<LifeCycleCost> lifeCycleCosts = this->lifeCycleCosts();
    toAdd.insert(toAdd.end(), lifeCycleCosts.begin(), lifeCycleCosts.end());

    // If same model, non-recursive insert of resources should be sufficient.
    Model m = this->model();
    if (model == m) {
      ResourceObjectVector resources = this->resources();
      WorkspaceObjectVector toInsert = castVector<WorkspaceObject>(resources);
      result = m.addAndInsertObjects(toAdd,toInsert);
      // adding this better have worked
      OS_ASSERT(result.size() == 1u + lifeCycleCosts.size() + resources.size());
      // inserting resources better have worked
      unsigned i = 1 + lifeCycleCosts.size();
      for (const ResourceObject& resource : resources) {
        OS_ASSERT(result[i] == resource);
        ++i;
      }
      return result[0].cast<ModelObject>();
    }

    // Not the same model. Resource handling is more complicated.
    result = model.addAndInsertObjects(
        toAdd,
        castArray<WorkspaceObject>(getRecursiveResourceSubTrees(getObject<ModelObject>(), true)));
    // Operation should work.
    OS_ASSERT(result.size() > 0u);
    return result[0].cast<ModelObject>();
  }

  /// remove the object from the model, also removes any cost objects associated with this object
  /// return std::vector<IdfObject> containing any removed object(s)
  std::vector<IdfObject> ModelObject_Impl::remove()
  {
    std::vector<IdfObject> result;
    std::vector<IdfObject> removedCosts = this->removeLifeCycleCosts();
    result = WorkspaceObject_Impl::remove();
    result.insert(result.end(), removedCosts.begin(), removedCosts.end());

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

  boost::optional<ParentObject> ModelObject_Impl::parent() const
  {
    return boost::optional<ParentObject>();
  }

  bool ModelObject_Impl::setParent(ParentObject& newParent)
  {
    return false;
  }

  ModelObject_Impl::ModelObject_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : WorkspaceObject_Impl(idfObject, model, keepHandle)
  {
  }

  ModelObject_Impl::ModelObject_Impl( IddObjectType type, Model_Impl* model)
    : WorkspaceObject_Impl(IdfObject(type), model)
  {
  }

  bool ModelObject_Impl::getBooleanFieldValue(unsigned index) const {
    OptionalString oStr = getString(index,true);
    OS_ASSERT(oStr);
    std::string temp = *oStr;
    boost::to_lower(temp);
    if (temp == "yes") { return true; }
    if (temp == "no") { return false; }
    LOG_AND_THROW("Unable to deduce a value for boolean field " << index << " in "
        << briefDescription() << ".");
    return false;
  }

  bool ModelObject_Impl::setBooleanFieldValue(unsigned index, bool value) {
    bool ok(true);
    if (value) {
      ok = setString(index,"Yes");
    }
    else {
      ok = setString(index,"No");
    }
    if (!ok) {
      LOG_AND_THROW("Unable to set boolean field " << index << " in " << briefDescription() << ".");
    }
    return true;
  }

  bool ModelObject_Impl::setSchedule(unsigned index,
                                     const std::string& className,
                                     const std::string& scheduleDisplayName,
                                     Schedule& schedule)
  {
    if (schedule.model() != model()) {
      return false;
    }
    bool result = checkOrAssignScheduleTypeLimits(className,scheduleDisplayName,schedule);
    if (!result) {
      return result;
    }
    return setPointer(index,schedule.handle());
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

  std::vector<ScheduleTypeKey> ModelObject_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
    return std::vector<ScheduleTypeKey>();
  }

} // detail

bool ModelObject::operator < (const ModelObject& right) const
{
  return (getImpl<detail::ModelObject_Impl>()) < (right.getImpl<detail::ModelObject_Impl>()) ;
}

// GETTERS

// DLM@20101011: todo, fix later
Model ModelObject::model() const {
  return getImpl<detail::ModelObject_Impl>()->model();
}

std::vector<LifeCycleCost> ModelObject::lifeCycleCosts() const
{
  return getImpl<detail::ModelObject_Impl>()->lifeCycleCosts();
}

std::vector<IdfObject> ModelObject::removeLifeCycleCosts()
{
  return getImpl<detail::ModelObject_Impl>()->removeLifeCycleCosts();
}

IddObjectType ModelObject::iddObjectType() const
{
  return getImpl<detail::ModelObject_Impl>()->iddObjectType();
}

// std::vector<std::string> ModelObject::attributeNames() const {
//   return getImpl<detail::ModelObject_Impl>()->attributeNames();
// }

// std::vector<openstudio::Attribute> ModelObject::attributes() const {
//   return getImpl<detail::ModelObject_Impl>()->attributes();
// }

// boost::optional<openstudio::Attribute> ModelObject::getAttribute(const std::string& name) const {
//   return getImpl<detail::ModelObject_Impl>()->getAttribute(name);
// }

// bool ModelObject::isSettableAttribute(const std::string& name) const
// {
//   return getImpl<detail::ModelObject_Impl>()->isSettableAttribute(name);
// }

// bool ModelObject::isOptionalAttribute(const std::string& name) const
// {
//   return getImpl<detail::ModelObject_Impl>()->isOptionalAttribute(name);
// }

// bool ModelObject::setAttribute(const std::string& name, bool value) {
//   return getImpl<detail::ModelObject_Impl>()->setAttribute(name, value);
// }

// bool ModelObject::setAttribute(const std::string& name, int value) {
//   return getImpl<detail::ModelObject_Impl>()->setAttribute(name, value);
// }

// bool ModelObject::setAttribute(const std::string& name, double value) {
//   return getImpl<detail::ModelObject_Impl>()->setAttribute(name, value);
// }

// bool ModelObject::setAttribute(const std::string& name, const Quantity& value) {
//   return getImpl<detail::ModelObject_Impl>()->setAttribute(name, value);
// }

// bool ModelObject::setAttribute(const std::string& name, unsigned value) {
//   return getImpl<detail::ModelObject_Impl>()->setAttribute(name, value);
// }

// bool ModelObject::setAttribute(const std::string& name, const char* value) {
//   return getImpl<detail::ModelObject_Impl>()->setAttribute(name, value);
// }

// bool ModelObject::setAttribute(const std::string& name, const std::string& value) {
//   return getImpl<detail::ModelObject_Impl>()->setAttribute(name, value);
// }

// bool ModelObject::resetAttribute(const std::string& name)
// {
//   return getImpl<detail::ModelObject_Impl>()->resetAttribute(name);
// }

// std::vector<std::string> ModelObject::relationshipNames() const {
//   return getImpl<detail::ModelObject_Impl>()->relationshipNames();
// }

// std::vector<Relationship> ModelObject::relationships() const {
//   return getImpl<detail::ModelObject_Impl>()->relationships();
// }

// boost::optional<Relationship> ModelObject::getRelationship(const std::string& name) const {
//   return getImpl<detail::ModelObject_Impl>()->getRelationship(name);
// }

// bool ModelObject::setRelationship(const std::string& name,
//                                   boost::optional<ModelObject> relatedModelObject)
// {
//   return getImpl<detail::ModelObject_Impl>()->setRelationship(name,relatedModelObject);
// }

// bool ModelObject::setRelationship(const std::string& name, const Component& component) {
//   return getImpl<detail::ModelObject_Impl>()->setRelationship(name,component);
// }


std::vector<ResourceObject> ModelObject::resources() const {
  return getImpl<detail::ModelObject_Impl>()->resources();
}

/** Get all output variables names that could be associated with this object.
 *   Each concrete class should override this.*/
const std::vector<std::string>& ModelObject::outputVariableNames() const
{
  return getImpl<detail::ModelObject_Impl>()->outputVariableNames();
}

/** Get all output variables associated with this object, must run simulation to generate associate data. */
OutputVariableVector ModelObject::outputVariables() const
{
  return getImpl<detail::ModelObject_Impl>()->outputVariables();
}

/** Get data associated with this output variable and this object. */
openstudio::OptionalTimeSeries ModelObject::getData(const OutputVariable& variable, const std::string& envPeriod) const
{
  return getImpl<detail::ModelObject_Impl>()->getData(variable, envPeriod);
}

/// equality test
bool ModelObject::operator==(const ModelObject& other) const
{
  return (handle() == other.handle());
}

/// in-equality test
bool ModelObject::operator!=(const ModelObject& other) const
{
  return (handle() != other.handle());
}

std::vector<ScheduleTypeKey> ModelObject::getScheduleTypeKeys(const Schedule& schedule) const {
  return getImpl<detail::ModelObject_Impl>()->getScheduleTypeKeys(schedule);
}

boost::optional<ModelObject> ModelObject::connectedObject(unsigned port) const
{
  return getImpl<detail::ModelObject_Impl>()->connectedObject(port);
}

boost::optional<unsigned> ModelObject::connectedObjectPort(unsigned port) const
{
  return getImpl<detail::ModelObject_Impl>()->connectedObjectPort(port);
}

ModelObject ModelObject::clone() const
{
  return getImpl<detail::ModelObject_Impl>()->clone(model());
}

ModelObject ModelObject::clone(Model model) const
{
  return getImpl<detail::ModelObject_Impl>()->clone(model);
}

Component ModelObject::createComponent() const {
  return getImpl<detail::ModelObject_Impl>()->createComponent();
}

OptionalParentObject ModelObject::parent() const
{
  return getImpl<detail::ModelObject_Impl>()->parent();
}

bool ModelObject::setParent(ParentObject& newParent)
{
  return getImpl<detail::ModelObject_Impl>()->setParent(newParent);
}

ModelObject::ModelObject(IddObjectType type, const Model& model, bool fastName)
  : WorkspaceObject(model.getImpl<detail::Model_Impl>()->createObject(IdfObject(type, fastName),false))
{
  // create object of correct type
  OS_ASSERT(getImpl<detail::ModelObject_Impl>());
  OS_ASSERT(!getImpl<detail::ModelObject_Impl>()->initialized());
  // add to Workspace
  openstudio::detail::WorkspaceObject_ImplPtrVector impls;
  impls.push_back(getImpl<openstudio::detail::WorkspaceObject_Impl>());
  model.getImpl<detail::Model_Impl>()->addObjects(impls,false);
  // object should be initialized and ready to go
  if (!getImpl<detail::ModelObject_Impl>()->initialized()){
    LOG_AND_THROW("ModelObject not initialized: " << std::endl << getImpl<openstudio::detail::WorkspaceObject_Impl>()->idfObject());
  }
}

ModelObject::ModelObject(std::shared_ptr<detail::ModelObject_Impl> p)
  : WorkspaceObject(std::move(p))
{}

/** Gets the autosized component value from the sql file **/
boost::optional<double> ModelObject::getAutosizedValue(std::string valueName, std::string units) const
{
  return getImpl<detail::ModelObject_Impl>()->getAutosizedValue(valueName, units);
}

} // model
} // openstudio
