/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <standardsinterface/OpenStudioStandardsInterface.hpp>

// Model
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/ModelObject.hpp>
#include <model/ModelObject_Impl.hpp>
#include <model/Component.hpp>
#include <model/ComponentData.hpp>
#include <model/ComponentData_Impl.hpp>
#include <model/Relationship.hpp>

// Abstract ModelObjects
#include <model/ParentObject.hpp>

// Tree
#include <model/Site.hpp>
#include <model/Site_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/SubSurface.hpp>
#include <model/SubSurface_Impl.hpp>

// ResourceObjects
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ConstructionBase.hpp>
#include <model/ConstructionBase_Impl.hpp>
#include <model/OpaqueMaterial.hpp>
#include <model/OpaqueMaterial_Impl.hpp>

// Other
#include <utilities/data/Attribute.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/StringHelpers.hpp>

#include <boost/foreach.hpp>

using namespace openstudio::standardsinterface::result;
using namespace openstudio::model;

namespace openstudio {
namespace standardsinterface {

// CONSTRUCTORS

OpenStudioStandardsInterface::OpenStudioStandardsInterface(const model::Model& model) 
    : m_model(model) {}

OpenStudioStandardsInterface OpenStudioStandardsInterface::clone() const 
{
  model::Model modelClone = m_model.clone().cast<Model>();
  return OpenStudioStandardsInterface(modelClone);
}

// GETTERS

Model OpenStudioStandardsInterface::model() const {
  return m_model; 
}

std::string OpenStudioStandardsInterface::name() const {
  std::string result;
  OptionalBuilding oBldg = m_model.getOptionalUniqueModelObject<Building>();
  if (oBldg) {
    OptionalString oName = oBldg->name(true);
    if (oName) { result = *oName; }
  }
  return result;
}

Result< std::vector<model::ModelObject> > OpenStudioStandardsInterface::getObjects(const std::string& ddObjectType) const {

  model::ModelObjectVector result;

  // get data dictionary enum value
  if (!DataDictionary::isObjectType(ddObjectType)) {
    return ModelObjectVectorResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::ObjectType),ddObjectType));
  }
  ObjectType objTyp(ddObjectType);

  // execute based on enum value
  if (objTyp == ObjectType::Building) {
    OptionalBuilding oBldg = m_model.getOptionalUniqueModelObject<Building>();
    if (oBldg) { result.push_back(*oBldg); }
  }
  else if (objTyp == ObjectType::Zone) {
    SpaceVector zones = m_model.getModelObjects<Space>();
    result.insert(result.end(),zones.begin(),zones.end());
  }
  else if (objTyp == ObjectType::Ceiling) {
    SurfaceVector surfaces = m_model.getModelObjects<Surface>();
    BOOST_FOREACH(const Surface& surface,surfaces) {
      if (istringEqual("RoofCeiling", surface.surfaceType())){
        if (!istringEqual("Outdoors", surface.outsideBoundaryCondition())){
          result.push_back(surface); 
        }
      }
    }
  }
  else if (objTyp == ObjectType::Roof) {
    SurfaceVector surfaces = m_model.getModelObjects<Surface>();
    BOOST_FOREACH(const Surface& surface,surfaces) {
      if (istringEqual("RoofCeiling", surface.surfaceType())){
        if (istringEqual("Outdoors", surface.outsideBoundaryCondition())){
          result.push_back(surface); 
        }
      }
    }
  }
  else if (objTyp == ObjectType::Wall) {
    SurfaceVector surfaces = m_model.getModelObjects<Surface>();
    BOOST_FOREACH(const Surface& surface,surfaces) {
      if (istringEqual("Wall", surface.surfaceType())){
        result.push_back(surface); 
      }
    }
  }
  else if (objTyp == ObjectType::FenestrationSkylight) {
    SubSurfaceVector subSurfs = m_model.getModelObjects<SubSurface>();
    BOOST_FOREACH(const SubSurface& subSurf,subSurfs) {
      if (istringEqual("Skylight", subSurf.subSurfaceType())){
        result.push_back(subSurf); 
      }
    }
  }
  else if (objTyp == ObjectType::Window) {
    SubSurfaceVector subSurfs = m_model.getModelObjects<SubSurface>();
    BOOST_FOREACH(const SubSurface& subSurf,subSurfs) {
      if (istringEqual("FixedWindow", subSurf.subSurfaceType()) ||
          istringEqual("OperableWindow", subSurf.subSurfaceType()) ){
        result.push_back(subSurf); 
      }
    }
  }
  else if (objTyp == ObjectType::ConstructionAssembly) {
    ConstructionBaseVector constructions = m_model.getModelObjects<ConstructionBase>();
    result.insert(result.end(),constructions.begin(),constructions.end());
  }
  else {
    std::stringstream msg; 
    msg << "OpenStudioStandards is currently unable to return objects of type " << ddObjectType << ".";
    return ModelObjectVectorResult(RequestUnmapped,msg.str());
  }

  return ModelObjectVectorResult(result);
}

Result< std::vector<model::ModelObject> > OpenStudioStandardsInterface::getObjects(
    const std::string& ddObjectTypeSourceOrParent, 
    const model::ModelObject& sopObject,
    const std::string& ddObjectTypeTargetOrChild) const 
{
  // get data dictionary enum values
  if (!DataDictionary::isObjectType(ddObjectTypeSourceOrParent)) {
    return ModelObjectVectorResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::ObjectType),
                                   ddObjectTypeSourceOrParent));
  }
  ObjectType sopObjTyp(ddObjectTypeSourceOrParent);
  if (!DataDictionary::isObjectType(ddObjectTypeTargetOrChild)) {
    return ModelObjectVectorResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::ObjectType),
                                   ddObjectTypeTargetOrChild));
  }
  ObjectType tocObjTyp(ddObjectTypeTargetOrChild);

  // verify object type
  std::string objTypMsg = verifyObjectType(sopObjTyp,sopObject);
  if (!objTypMsg.empty()) {
    return ModelObjectVectorResult(DataDictionaryError,objTypMsg);
  }

  std::stringstream msg;
  model::ModelObjectVector result;

  // default implementation
  std::string relationshipName = dataDictionaryTermToRelationshipName(tocObjTyp.valueName());
  boost::optional<Relationship> oRelationship = sopObject.getRelationship(relationshipName);
  if (oRelationship) {
    Relationship rel = *oRelationship;
    if (rel.isSingular()) {
      OptionalModelObject temp = rel.relatedModelObject();
      if (temp) {
        result.push_back(*temp);
      }
    }
    else {
      result = rel.relatedModelObjects();
    }
    return ModelObjectVectorResult(result);
  }

  // execute based on enum values
  if (sopObjTyp == ObjectType::Building) {
    // return general result since Building is top-level
    return getObjects(ddObjectTypeTargetOrChild);
  }
  else if (sopObjTyp == ObjectType::Zone) {
    model::Space space = sopObject.cast<Space>();
    // surfaces
    if ((tocObjTyp == ObjectType::Ceiling) || 
        (tocObjTyp == ObjectType::Roof) ||
        (tocObjTyp == ObjectType::Wall)) {
      SurfaceVector surfaces = getSubsetOfRecursiveChildren<Surface>(space);
      BOOST_FOREACH(const Surface& surface,surfaces) {
        if (verifyObjectType(tocObjTyp,surface).empty()) { 
          result.push_back(surface); 
        }
      }
    }
    // subsurfaces
    if (tocObjTyp == ObjectType::Window) {
      SubSurfaceVector subSurfaces = getSubsetOfRecursiveChildren<SubSurface>(space);
      BOOST_FOREACH(const SubSurface& subSurface,subSurfaces) {
        if (verifyObjectType(tocObjTyp,subSurface).empty()) { 
          result.push_back(subSurface); 
        }
      }
    }
  }
  else {    
    msg << "OpenStudioStandardsInterface is currently unable to return targets or children of "
        << "objects of type " << ddObjectTypeSourceOrParent << ".";
    return ModelObjectVectorResult(RequestUnmapped,msg.str());
  }

  return ModelObjectVectorResult(result);
}

Result<bool> OpenStudioStandardsInterface::booleanAttribute(const std::string& ddObjectType,
                                                            const model::ModelObject& object,
                                                            const std::string& ddBooleanAttribute) const {

  // get data dictionary enum values
  if (!DataDictionary::isObjectType(ddObjectType)) {
    return BoolResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::ObjectType),ddObjectType));
  }
  ObjectType objTyp(ddObjectType);
  if (!DataDictionary::isBooleanAttribute(ddBooleanAttribute)) {
    return BoolResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::Boolean),ddBooleanAttribute));
  }
  BooleanAttribute b(ddBooleanAttribute);

  // verify object type
  std::string objTypMsg = verifyObjectType(objTyp,object);
  if (!objTypMsg.empty()) { 
    return BoolResult(DataDictionaryError,objTypMsg); 
  }

  // default implementation
  std::string attributeName = dataDictionaryTermToAttributeName(b.valueName());
  boost::optional<Attribute> oAttribute = object.getAttribute(attributeName);
  if (oAttribute) {
    Attribute attr = *oAttribute;
    if ((attr.valueType() == AttributeValueType::Boolean)) {
      return BoolResult(attr.valueAsBoolean());
    }
  }

  result::Code errorCode;
  std::stringstream msg;
  if (object.isOptionalAttribute(attributeName)) {
    errorCode = result::IncompleteModel;
    msg << "OpenStudio Model " << object.briefDescription() << " has a boolean attribute '" 
        << ddBooleanAttribute << "', but it is not currently available. Please either simulate "
        << "this model and set the SqlFile, or add the data to the model description.";
  }
  else {
    errorCode = result::RequestUnmapped;
    msg << "OpenStudioStandardsInterface is currently unable to return the boolean attribute '" 
        << ddBooleanAttribute << "' for objects of type '" << ddObjectType << "'.";
  }
  return BoolResult(errorCode,msg.str());
}

Result<int> OpenStudioStandardsInterface::integerAttribute(
    const std::string& ddObjectType,
    const model::ModelObject& object,
    const std::string& ddIntegerAttribute) const 
{
  // get data dictionary enum values
  if (!DataDictionary::isObjectType(ddObjectType)) {
    return IntegerResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::ObjectType),ddObjectType));
  }
  ObjectType objTyp(ddObjectType);
  if (!DataDictionary::isIntegerAttribute(ddIntegerAttribute)) {
    return IntegerResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::Integer),ddIntegerAttribute));
  }
  IntegerAttribute ia(ddIntegerAttribute);

  // verify object type
  std::string objTypMsg = verifyObjectType(objTyp,object);
  if (!objTypMsg.empty()) { 
    return IntegerResult(DataDictionaryError,objTypMsg); 
  }

  // default implementation
  std::string attributeName = dataDictionaryTermToAttributeName(ia.valueName());
  boost::optional<Attribute> oAttribute = object.getAttribute(attributeName);
  if (oAttribute) {
    Attribute attr = *oAttribute;
    if (((attr.valueType() == AttributeValueType::Integer) || (attr.valueType() == AttributeValueType::Unsigned))) 
    {
      return IntegerResult(attr.valueAsInteger());
    }
  }

  result::Code errorCode;
  std::stringstream msg;
  if (object.isOptionalAttribute(attributeName)) {
    errorCode = result::IncompleteModel;
    msg << "OpenStudio Model " << object.briefDescription() << " has an integer attribute '" 
        << ddIntegerAttribute << "', but it is not currently available. Please either simulate "
        << "this model and set the SqlFile, or add the data to the model description.";
  }
  else {
    errorCode = result::RequestUnmapped;
    msg << "OpenStudioStandardsInterface is currently unable to return the integer attribute '" 
        << ddIntegerAttribute << "' for objects of type '" << ddObjectType << "'.";
  }
  return IntegerResult(errorCode,msg.str());
}

Result<double> OpenStudioStandardsInterface::doubleAttribute(const std::string& ddObjectType,
                                                           const model::ModelObject& object,
                                                           const std::string& ddDoubleAttribute) const 
{  
  // get data dictionary enum values
  if (!DataDictionary::isObjectType(ddObjectType)) {
    return DoubleResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::ObjectType),ddObjectType));
  }
  ObjectType objTyp(ddObjectType);
  if (!DataDictionary::isDoubleAttribute(ddDoubleAttribute)) {
    return DoubleResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::Double),ddDoubleAttribute));
  }
  DoubleAttribute d(ddDoubleAttribute);

  // verify object type
  std::string objTypMsg = verifyObjectType(objTyp,object);
  if (!objTypMsg.empty()) { 
    return DoubleResult(DataDictionaryError,objTypMsg); 
  }

  // default implementation
  std::string attributeName = dataDictionaryTermToAttributeName(d.valueName());
  boost::optional<Attribute> oAttribute = object.getAttribute(attributeName);
  if (oAttribute) {
    Attribute attr = *oAttribute;
    if ((attr.valueType() == AttributeValueType::Double)) {
      return DoubleResult(attr.valueAsDouble());
    }
  }

  result::Code errorCode;
  std::stringstream msg;
  if (object.isOptionalAttribute(attributeName)) {
    errorCode = result::IncompleteModel;
    msg << "OpenStudio Model " << object.briefDescription() << " has a double attribute '" 
        << ddDoubleAttribute << "', but it is not currently available. Please either simulate "
        << "this model and set the SqlFile, or add the data to the model description.";
  }
  else {
    errorCode = result::RequestUnmapped;
    msg << "OpenStudioStandardsInterface is currently unable to return the double attribute '" 
        << ddDoubleAttribute << "' for objects of type '" << ddObjectType << "'.";
  }
  return DoubleResult(errorCode,msg.str());
}

Result<std::string> OpenStudioStandardsInterface::enumAttribute(const std::string& ddObjectType,
                                                         const model::ModelObject& object,
                                                         const std::string& ddEnumAttribute) const {

  // get data dictionary enum value for ddEnumAttribute
  if (!DataDictionary::isEnumAttribute(ddEnumAttribute)) {
    return StringResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech("Enum"),ddEnumAttribute));
  }
  EnumAttribute e(ddEnumAttribute);

  // get data dictionary enum value for ddObjectType
  if (!DataDictionary::isObjectType(ddObjectType)) {
    return StringResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::ObjectType),ddObjectType));
  }
  ObjectType objTyp(ddObjectType);

  // verify object type
  std::string objTypMsg = verifyObjectType(objTyp,object);
  if (!objTypMsg.empty()) { 
    return StringResult(DataDictionaryError,objTypMsg); 
  }

  // default implementation
  std::string result;
  std::string attributeName = dataDictionaryTermToAttributeName(e.valueName());
  boost::optional<Attribute> oAttribute = object.getAttribute(attributeName);
  if (oAttribute) {
    Attribute attr = *oAttribute;
    if ((attr.valueType() == AttributeValueType::String)) {
      result = attributeValueToDataDictionaryEnumAttributeValue(objTyp,
                                                                object,
                                                                e,
                                                                attr.valueAsString());
      if (!result.empty()) { return StringResult(result); }
    }
  }

  // also access site attributes through building
  if (objTyp == ObjectType::Building) {
    OptionalSite site = m_model.getOptionalUniqueModelObject<Site>();
    if (site) {
      attributeName = dataDictionaryTermToAttributeName(e.valueName());
      oAttribute = site->getAttribute(attributeName);
      if (oAttribute && (oAttribute->valueType() == AttributeValueType::String)) {
        result = attributeValueToDataDictionaryEnumAttributeValue(objTyp,
                                                                  object,
                                                                  e,
                                                                  oAttribute->valueAsString());
        if (!result.empty()) { return StringResult(result); }
      }
    }
  }

  result::Code errorCode;
  std::stringstream msg;
  if (object.isOptionalAttribute(attributeName)) {
    errorCode = result::IncompleteModel;
    msg << "OpenStudio Model " << object.briefDescription() << " has an enum attribute '" 
        << ddEnumAttribute << "', but it is not currently available. Please either simulate "
        << "this model and set the SqlFile, or add the data to the model description.";
  }
  else {
    errorCode = result::RequestUnmapped;
    msg << "OpenStudioStandardsInterface is currently unable to return the enum attribute '" 
        << ddEnumAttribute << "' for objects of type '" << ddObjectType << "'.";
  }
  return StringResult(errorCode,msg.str());
}

// SETTERS

Result<result::none> OpenStudioStandardsInterface::setName(const std::string newName) {

  ModelObjectVectorResult bldgsResult = getObjects("building");
  if (bldgsResult) {
    ModelObjectVector buildings = *bldgsResult;
    if (buildings.size() == 1) {
      bool result = buildings[0].setName(newName);
      if (result) { return VoidResult(); }
      else { 
        return VoidResult(ModelError,"Model setName operation failed."); 
      }
    }
    else {
      return VoidResult(ModelError,
                        "Unable to set model's name because it has multiple building objects.");
    }
  }

  return VoidResult(IncompleteModel,
                    "Unable to set model's name because there are no building objects.");

}

Result<result::none> OpenStudioStandardsInterface::setObject(const std::string& ddObjectTypeSourceOrParent, 
                                                             model::ModelObject& sopObject,
                                                             const std::string& ddObjectTypeTargetOrChild,
                                                             model::ModelObject& tocObject) 
{
  // get data dictionary enum values
  if (!DataDictionary::isObjectType(ddObjectTypeSourceOrParent)) {
    return VoidResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::ObjectType),ddObjectTypeSourceOrParent));
  }
  ObjectType sopObjTyp(ddObjectTypeSourceOrParent);
  if (!DataDictionary::isObjectType(ddObjectTypeTargetOrChild)) {
    return VoidResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::ObjectType),ddObjectTypeTargetOrChild));
  }
  ObjectType tocObjTyp(ddObjectTypeTargetOrChild);

  // verify object types
  std::string objTypMsg = verifyObjectType(sopObjTyp,sopObject);
  if (!objTypMsg.empty()) { 
    return VoidResult(DataDictionaryError,objTypMsg); 
  }
  objTypMsg = verifyObjectType(tocObjTyp,tocObject);
  if (!objTypMsg.empty() && !tocObject.optionalCast<ComponentData>()) { 
    return VoidResult(DataDictionaryError,objTypMsg); 
  }

  // default implementation
  std::stringstream msg;
  std::string relationshipName = dataDictionaryTermToRelationshipName(tocObjTyp.valueName());
  boost::optional<Relationship> oRelationship = sopObject.getRelationship(relationshipName);
  if (oRelationship) {
    Relationship rel = *oRelationship;
    if (rel.isSettable() && rel.isSingular()) {
      bool result = rel.setRelatedModelObject(tocObject);
      if (result) {
        return VoidResult();
      }
      else {
        msg << "Setting the " << tocObjTyp.valueName() << " of " << sopObjTyp.valueName() 
            << " object " << sopObject.briefDescription() << " failed in the OpenStudio Model.";
        return VoidResult(result::ModelError,msg.str());
      }
    }
  }
    
  msg << "OpenStudioStandardsInterface is currently unable to set targets or children of "
      << "objects of type " << ddObjectTypeSourceOrParent << ".";
  return VoidResult(result::RequestUnmapped,msg.str());
}

Result<result::none> OpenStudioStandardsInterface::setAttribute(const std::string& ddObjectType,
                                                                model::ModelObject& object,
                                                                const std::string& ddBooleanAttribute,
                                                                bool value) 
{
  // get data dictionary enum values
  if (!DataDictionary::isObjectType(ddObjectType)) {
    return VoidResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::ObjectType),ddObjectType));
  }
  ObjectType objTyp(ddObjectType);
  if (!DataDictionary::isBooleanAttribute(ddBooleanAttribute)) {
    return VoidResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::Boolean),ddBooleanAttribute));
  }
  BooleanAttribute b(ddBooleanAttribute);

  // verify object type
  std::string objTypMsg = verifyObjectType(objTyp,object);
  if (!objTypMsg.empty()) { 
    return VoidResult(DataDictionaryError,objTypMsg); 
  }

  // default implementation
  std::stringstream msg;
  std::string attributeName = dataDictionaryTermToAttributeName(b.valueName());
  if (object.isSettableAttribute(attributeName)) {
    if (object.setAttribute(attributeName, value)) { 
      return VoidResult(); 
    } 
    else {
      msg << "Request to set boolean attribute '" << ddBooleanAttribute << "' to '" << value 
          << "' failed for " << object.briefDescription() << ".";
      return VoidResult(ModelError,msg.str());
    }
  }

  msg << "OpenStudioStandardsInterface is currently unable to set the boolean attribute '" 
      << ddBooleanAttribute << "' for objects of type '" << ddObjectType << "'.";
  return VoidResult(RequestUnmapped,msg.str());
}

Result<result::none> OpenStudioStandardsInterface::setAttribute(const std::string& ddObjectType,
                                                      model::ModelObject& object,
                                                      const std::string& ddIntegerAttribute,
                                                      int value) 
{
  // get data dictionary enum values
  if (!DataDictionary::isObjectType(ddObjectType)) {
    return VoidResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::ObjectType),ddObjectType));
  }
  ObjectType objTyp(ddObjectType);
  if (!DataDictionary::isIntegerAttribute(ddIntegerAttribute)) {
    return VoidResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::Integer),ddIntegerAttribute));
  }
  IntegerAttribute ia(ddIntegerAttribute);

  // verify object type
  std::string objTypMsg = verifyObjectType(objTyp,object);
  if (!objTypMsg.empty()) { 
    return VoidResult(DataDictionaryError,objTypMsg); 
  }

  // default implementation
  std::stringstream msg;
  std::string attributeName = dataDictionaryTermToAttributeName(ia.valueName());
  if (object.isSettableAttribute(attributeName)) {
    if (object.setAttribute(attributeName, value)) {
      return VoidResult(); 
    } 
    else {
      msg << "Request to set integer attribute '" << ddIntegerAttribute << "' to '" << value 
          << "' failed for " << object.briefDescription() << ".";
      return VoidResult(ModelError,msg.str());
    }
  }

  msg << "OpenStudioStandardsInterface is currently unable to set the integer attribute '" 
      << ddIntegerAttribute << "' for objects of type '" << ddObjectType << "'.";

  return VoidResult(result::RequestUnmapped,msg.str());
}

Result<result::none> OpenStudioStandardsInterface::setAttribute(const std::string& ddObjectType,
                                                      model::ModelObject& object,
                                                      const std::string& ddDoubleAttribute,
                                                      double value) 
{

  // get data dictionary enum values
  if (!DataDictionary::isObjectType(ddObjectType)) {
    return VoidResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::ObjectType),ddObjectType));
  }
  ObjectType objTyp(ddObjectType);
  if (!DataDictionary::isDoubleAttribute(ddDoubleAttribute)) {
    return VoidResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::Double),ddDoubleAttribute));
  }
  DoubleAttribute d(ddDoubleAttribute);

  // verify object type
  std::string objTypMsg = verifyObjectType(objTyp,object);
  if (!objTypMsg.empty()) { 
    return VoidResult(DataDictionaryError,objTypMsg); 
  }

  // default implementation
  std::stringstream msg;
  std::string attributeName = dataDictionaryTermToAttributeName(d.valueName());
  if (object.isSettableAttribute(attributeName)) {
    if (object.setAttribute(attributeName, value)) {
      return VoidResult(); 
    }
    else {
      msg << "Request to set double attribute '" << ddDoubleAttribute << "' to '" << value 
          << "' failed for " << object.briefDescription() << ".";
      return VoidResult(result::ModelError,msg.str());
    }
  }

  msg << "OpenStudioStandardsInterface is currently unable to set the double attribute '" 
      << ddDoubleAttribute << "' for objects of type '" << ddObjectType << "'.";

  return VoidResult(result::RequestUnmapped,msg.str());
}

Result<result::none> OpenStudioStandardsInterface::setAttribute(const std::string& ddObjectType,
                                                      model::ModelObject& object,
                                                      const std::string& ddEnumAttribute,
                                                      std::string value) 
{
  // get data dictionary enum value for ddEnumAttribute
  if (!DataDictionary::isEnumAttribute(ddEnumAttribute)) {
    return VoidResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech("Enum"),ddEnumAttribute));
  }
  EnumAttribute e(ddEnumAttribute);

  // get data dictionary enum values
  if (!DataDictionary::isObjectType(ddObjectType)) {
    return VoidResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::ObjectType),ddObjectType));
  }
  ObjectType objTyp(ddObjectType);

  // validate enum value to be set
  if (!DataDictionary::isEnumValue(e,value)) {
    return VoidResult(DataDictionaryError,dataDictionaryErrorMessage(e,value));
  }

  // verify object type
  std::string objTypMsg = verifyObjectType(objTyp,object);
  if (!objTypMsg.empty()) { 
    return VoidResult(DataDictionaryError,objTypMsg); 
  }

  // default implementation
  std::stringstream msg;
  std::string attributeName = dataDictionaryTermToAttributeName(e.valueName());
  if (object.isSettableAttribute(attributeName)) {
    if (object.setAttribute(attributeName, value)) {
      return VoidResult(); 
    }
    else {
      msg << "Request to set enum attribute '" << ddEnumAttribute << "' to '" << value 
          << "' failed for " << object.briefDescription() << ".";
      return VoidResult(result::ModelError,msg.str());
    }
  }
  // also access Site's attributes through building
  if (objTyp == ObjectType::Building) {
    Site site = m_model.getUniqueModelObject<Site>();
    std::string attributeName = dataDictionaryTermToAttributeName(e.valueName());
    if (site.isSettableAttribute(attributeName)) {
      if (site.setAttribute(attributeName, value)) {
        return VoidResult(); 
      }
      else {
        msg << "Request to set enum attribute '" << ddEnumAttribute << "' to '" << value 
            << "' failed for " << object.briefDescription() << ".";
        return VoidResult(result::ModelError,msg.str());
      }
    }
  }

  msg << "OpenStudioStandardsInterface is currently unable to set the enum attribute '" 
      << ddEnumAttribute << "' for objects of type '" << ddObjectType << "'.";

  return VoidResult(result::RequestUnmapped,msg.str());
}

Result<result::none> OpenStudioStandardsInterface::setAttribute(const std::string& ddObjectType,
                                                                model::ModelObject& object,
                                                                const std::string& ddEnumAttribute,
                                                                const char* value) 
{
  return setAttribute(ddObjectType,object,ddEnumAttribute,std::string(value));
}

// SIMULATION

Result<result::none> OpenStudioStandardsInterface::requestAttribute(const std::string& ddObjectType,
                                                                    const std::string& ddAttribute) 
{
  // get data dictionary terms
  if (!DataDictionary::isObjectType(ddObjectType)) {
    return VoidResult(DataDictionaryError,
        dataDictionaryErrorMessage(PartsOfSpeech(PartsOfSpeech::ObjectType),ddObjectType));
  }
  ObjectType objTyp(ddObjectType);

  OptionalPartsOfSpeech pos = DataDictionary::partOfSpeech(ddAttribute);
  if (!pos) {
    return VoidResult(DataDictionaryError,dataDictionaryErrorMessage(ddAttribute));
  }

  return VoidResult(RequestUnmapped,"OpenStudioStandardsInterface is currently unable to request attributes that require simulation.");
}

Result<result::none> OpenStudioStandardsInterface::simulate(const std::string& energyPlusPath,
                                                  const std::string& energyPlusIddPath) 
{
  return VoidResult(RequestUnmapped);
}

// STATIC QUERIES

std::vector<ObjectType> OpenStudioStandardsInterface::supportedObjectTypes() {
  static std::vector<ObjectType> result;
  if (result.empty()) {
    result.push_back(ObjectType(ObjectType::Ceiling));
    result.push_back(ObjectType(ObjectType::Roof));
    result.push_back(ObjectType(ObjectType::Wall));
    result.push_back(ObjectType(ObjectType::Window));
    result.push_back(ObjectType(ObjectType::Building));
    result.push_back(ObjectType(ObjectType::Zone));
    result.push_back(ObjectType(ObjectType::ConstructionAssembly));
    result.push_back(ObjectType(ObjectType::Insulation));
  }
  return result;
}

std::vector<ObjectType> OpenStudioStandardsInterface::acceptedSynonyms(const ObjectType& ot) {
  static std::map<ObjectType, std::vector<ObjectType> > m_objectTypeSynonymMap;
  if (m_objectTypeSynonymMap.empty()) {
    m_objectTypeSynonymMap[ObjectType(ObjectType::Zone)].push_back(ObjectType(ObjectType::ThermalBlock));
    m_objectTypeSynonymMap[ObjectType(ObjectType::Window)].push_back(ObjectType(ObjectType::FenestrationVertical));
  }
  ObjectTypeVector result;
  std::map<ObjectType, std::vector<ObjectType> >::iterator it = m_objectTypeSynonymMap.find(ot);
  if (it != m_objectTypeSynonymMap.end()) { result = it->second; }
  return result;
}

std::vector<BooleanAttribute> OpenStudioStandardsInterface::supportedBooleanAttributes(const ObjectType& ot) {
  static std::map<ObjectType,std::vector<BooleanAttribute> > m_booleanAttributeMap;
  if (m_booleanAttributeMap.empty()) {
  }
  BooleanAttributeVector result;
  std::map<ObjectType,std::vector<BooleanAttribute> >::iterator it = m_booleanAttributeMap.find(ot);
  if (it != m_booleanAttributeMap.end()) { result = it->second; }
  return result;
}

std::vector<IntegerAttribute> OpenStudioStandardsInterface::supportedIntegerAttributes(const ObjectType& ot) {
  static std::map<ObjectType,std::vector<IntegerAttribute> > m_integerAttributeMap;
  if (m_integerAttributeMap.empty()) {
    m_integerAttributeMap[ObjectType(ObjectType::Building)].push_back(IntegerAttribute(IntegerAttribute::AboveGradeStoryCount));
  }
  IntegerAttributeVector result;
  std::map<ObjectType,std::vector<IntegerAttribute> >::iterator it = m_integerAttributeMap.find(ot);
  if (it != m_integerAttributeMap.end()) { result = it->second; }
  return result;
}

std::vector<DoubleAttribute> OpenStudioStandardsInterface::supportedDoubleAttributes(const ObjectType& ot) {
  static std::map<ObjectType,std::vector<DoubleAttribute> > m_doubleAttributeMap;
  if (m_doubleAttributeMap.empty()) {
    m_doubleAttributeMap[ObjectType(ObjectType::Building)].push_back(DoubleAttribute(DoubleAttribute::FloorArea));
    m_doubleAttributeMap[ObjectType(ObjectType::Building)].push_back(DoubleAttribute(DoubleAttribute::TotalConditionedFloorArea));
    m_doubleAttributeMap[ObjectType(ObjectType::Building)].push_back(DoubleAttribute(DoubleAttribute::LightingPower));
    m_doubleAttributeMap[ObjectType(ObjectType::Building)].push_back(DoubleAttribute(DoubleAttribute::LightingPowerDensity));
    m_doubleAttributeMap[ObjectType(ObjectType::Building)].push_back(DoubleAttribute(DoubleAttribute::OccupantDensity));
    m_doubleAttributeMap[ObjectType(ObjectType::Zone)].push_back(DoubleAttribute(DoubleAttribute::FloorArea));
    m_doubleAttributeMap[ObjectType(ObjectType::Zone)].push_back(DoubleAttribute(DoubleAttribute::LightingPower));
    m_doubleAttributeMap[ObjectType(ObjectType::Zone)].push_back(DoubleAttribute(DoubleAttribute::LightingPowerDensity));
    m_doubleAttributeMap[ObjectType(ObjectType::Zone)].push_back(DoubleAttribute(DoubleAttribute::OccupantDensity));
    m_doubleAttributeMap[ObjectType(ObjectType::Ceiling)].push_back(DoubleAttribute(DoubleAttribute::UFactor));
    m_doubleAttributeMap[ObjectType(ObjectType::Ceiling)].push_back(DoubleAttribute(DoubleAttribute::HeatCapacity));
    m_doubleAttributeMap[ObjectType(ObjectType::Roof)].push_back(DoubleAttribute(DoubleAttribute::UFactor));
    m_doubleAttributeMap[ObjectType(ObjectType::Roof)].push_back(DoubleAttribute(DoubleAttribute::HeatCapacity));
    m_doubleAttributeMap[ObjectType(ObjectType::Wall)].push_back(DoubleAttribute(DoubleAttribute::UFactor));
    m_doubleAttributeMap[ObjectType(ObjectType::Wall)].push_back(DoubleAttribute(DoubleAttribute::HeatCapacity));
    m_doubleAttributeMap[ObjectType(ObjectType::Window)].push_back(DoubleAttribute(DoubleAttribute::UFactor));
    m_doubleAttributeMap[ObjectType(ObjectType::Insulation)].push_back(DoubleAttribute(DoubleAttribute::Thickness));
    m_doubleAttributeMap[ObjectType(ObjectType::Insulation)].push_back(DoubleAttribute(DoubleAttribute::RValue));
  }
  DoubleAttributeVector result;
  std::map<ObjectType,std::vector<DoubleAttribute> >::iterator it = m_doubleAttributeMap.find(ot);
  if (it != m_doubleAttributeMap.end()) { result = it->second; }
  return result;
}

std::vector<EnumAttribute> OpenStudioStandardsInterface::supportedEnumAttributes(const ObjectType& ot) {
  static std::map<ObjectType,std::vector<EnumAttribute> > m_enumAttributeMap;
  if (m_enumAttributeMap.empty()) {
    m_enumAttributeMap[ObjectType(ObjectType::Building)].push_back(EnumAttribute(EnumAttribute::ClimateZone));
    m_enumAttributeMap[ObjectType(ObjectType::Ceiling)].push_back(EnumAttribute(EnumAttribute::PartitionThermalType));
    m_enumAttributeMap[ObjectType(ObjectType::Ceiling)].push_back(EnumAttribute(EnumAttribute::CeilingConstructionType));
    m_enumAttributeMap[ObjectType(ObjectType::Roof)].push_back(EnumAttribute(EnumAttribute::PartitionThermalType));
    m_enumAttributeMap[ObjectType(ObjectType::Roof)].push_back(EnumAttribute(EnumAttribute::RoofConstructionType));
    m_enumAttributeMap[ObjectType(ObjectType::Wall)].push_back(EnumAttribute(EnumAttribute::PartitionThermalType));
    m_enumAttributeMap[ObjectType(ObjectType::Wall)].push_back(EnumAttribute(EnumAttribute::WallConstructionType));
    m_enumAttributeMap[ObjectType(ObjectType::Window)].push_back(EnumAttribute(EnumAttribute::PartitionThermalType));
  }
  EnumAttributeVector result;
  std::map<ObjectType,std::vector<EnumAttribute> >::iterator it = m_enumAttributeMap.find(ot);
  if (it != m_enumAttributeMap.end()) { result = it->second; }
  return result;
}

// PRIVATE

/// @cond

std::string OpenStudioStandardsInterface::dataDictionaryTermToAttributeName(const std::string& ddTerm) const
{
  // map to handle different names for data dictionary versus model attributes
  static std::map<std::string,std::string> ddTermToModelAttributeNameMap;
  if (ddTermToModelAttributeNameMap.empty()) {
    ddTermToModelAttributeNameMap.insert(std::map<std::string,std::string>::value_type(
        DoubleAttribute(DoubleAttribute::TotalConditionedFloorArea).valueName(),
        "conditionedFloorArea"));
    ddTermToModelAttributeNameMap.insert(std::map<std::string,std::string>::value_type(
        DoubleAttribute(DoubleAttribute::LightingPowerDensity).valueName(),
        "lightingPowerPerFloorArea"));
    ddTermToModelAttributeNameMap.insert(std::map<std::string,std::string>::value_type(
        DoubleAttribute(DoubleAttribute::OccupantDensity).valueName(),
        "peoplePerFloorArea"));
    ddTermToModelAttributeNameMap.insert(std::map<std::string,std::string>::value_type(
        DoubleAttribute(DoubleAttribute::RValue).valueName(),
        "thermalResistance"));

    ddTermToModelAttributeNameMap.insert(std::map<std::string,std::string>::value_type(
        EnumAttribute(EnumAttribute::ClimateZone).valueName(),
        "activeClimateZoneValue"));
    ddTermToModelAttributeNameMap.insert(std::map<std::string,std::string>::value_type(
        EnumAttribute(EnumAttribute::CeilingConstructionType).valueName(),
        "constructionType"));
    ddTermToModelAttributeNameMap.insert(std::map<std::string,std::string>::value_type(
        EnumAttribute(EnumAttribute::RoofConstructionType).valueName(),
        "constructionType"));
    ddTermToModelAttributeNameMap.insert(std::map<std::string,std::string>::value_type(
        EnumAttribute(EnumAttribute::WallConstructionType).valueName(),
        "constructionType"));
    ddTermToModelAttributeNameMap.insert(std::map<std::string,std::string>::value_type(
        EnumAttribute(EnumAttribute::PartitionThermalType).valueName(),
        "outsideBoundaryCondition"));

    ddTermToModelAttributeNameMap.insert(std::map<std::string,std::string>::value_type(
        IntegerAttribute(IntegerAttribute::AboveGradeStoryCount).valueName(),
        "numberOfAboveGroundStories"));
  }
  std::map<std::string,std::string>::const_iterator it = ddTermToModelAttributeNameMap.find(ddTerm);
  if (it != ddTermToModelAttributeNameMap.end()) { return it->second; }
  return toLowerCamelCase(ddTerm);
}

std::string OpenStudioStandardsInterface::dataDictionaryTermToRelationshipName(
    const std::string& ddTerm) const
{
  // map to handle different names for data dictionary versus model attributes
  static std::map<std::string,std::string> ddTermToModelRelationshipNameMap;

  ddTermToModelRelationshipNameMap.insert(std::map<std::string,std::string>::value_type(
      ObjectType(ObjectType::ConstructionAssembly).valueName(),
      "construction"));

  std::map<std::string,std::string>::const_iterator it = ddTermToModelRelationshipNameMap.find(ddTerm);
  if (it != ddTermToModelRelationshipNameMap.end()) { 
    return it->second;
  }
  return toLowerCamelCase(ddTerm);
}

std::string OpenStudioStandardsInterface::attributeValueToDataDictionaryEnumAttributeValue(
    const ObjectType& objectType,
    const model::ModelObject& object,
    const EnumAttribute& attribute,
    const std::string& value) const
{
  std::string result(value);

  if (attribute == EnumAttribute::PartitionThermalType) {
    boost::to_lower(result);
    if (result == "adiabatic") { result = "Adiabatic"; }
    else if ((result == "surface") || (result == "zone")) { result = "Interzone"; }
    else if ((result == "outdoors") || (result == "othersidecoefficients") || 
             (result == "othersideconditionsmodel"))
    { result = "Exterior"; }
    else if (!result.empty()) { result = "Underground"; } // Ground, GroundFCfactorMethod, GroundSlab..., GroundBasement...
    else { result = ""; }

    if (result == "Interzone") {
      // check for Demising
      // TODO: Flag need for simulation?!
      OptionalSurface oSurface = object.optionalCast<Surface>();
      if (!oSurface) {
        OptionalSubSurface oSubSurface = object.optionalCast<SubSurface>();
        if (oSubSurface) { oSurface = oSubSurface->surface(); }
      }
      if (oSurface) {
        
        OptionalSurface adjacentSurface = oSurface->adjacentSurface();
        
        if (adjacentSurface){

          OptionalSpace thisSpace = oSurface->space();
          OptionalSpace adjacentSpace = adjacentSurface->space();
          
          if (thisSpace && adjacentSpace){

            OptionalThermalZone thisZone = thisSpace->thermalZone();
            OptionalThermalZone adjacentZone = adjacentSpace->thermalZone();
            
            if (thisZone && adjacentZone) {
              bool thisConditioned = thisZone->isConditioned();
              bool adjacentConditioned = adjacentZone->isConditioned();
              if ((thisConditioned || adjacentConditioned) && 
                  !(thisConditioned && adjacentConditioned)) 
              {
                result = "Demising";
              }
            }
          }
        }
      }
    }
  }
  if (!result.empty() && !DataDictionary::isEnumValue(attribute,result)) {
    LOG(Debug,"Invalid attribute, value pair: " << attribute.valueDescription() << ", " << result);
  }
  BOOST_ASSERT(result.empty() || DataDictionary::isEnumValue(attribute,result));

  if (!result.empty()) {
    result = DataDictionary::standardEnumValue(attribute,result);
  }
  return result;
}

// ERROR CHECKING HELPERS

std::string OpenStudioStandardsInterface::verifyObjectType(const ObjectType& objTyp,
                                                           const model::ModelObject& object) const
{
  std::string result;

  // execute based on enum value
  if (objTyp == ObjectType::Building) {
    if (!object.optionalCast<Building>()) { 
      result = modelInputErrorMessage(objTyp.valueName(),object); 
    }
  }
  else if (objTyp == ObjectType::Zone) {
    if (!object.optionalCast<Space>()) { 
      result = modelInputErrorMessage(objTyp.valueName(),object); 
    }
  }
  else if (objTyp == ObjectType::Ceiling) {
    if (!object.optionalCast<Surface>() || 
        !istringEqual("RoofCeiling", object.cast<Surface>().surfaceType())) 
    {
       result = modelInputErrorMessage(objTyp.valueName(),object);
    }
  }
  else if (objTyp == ObjectType::Roof) {
    if (!object.optionalCast<Surface>() || 
        !istringEqual("RoofCeiling", object.cast<Surface>().surfaceType())) 
    {
       result = modelInputErrorMessage(objTyp.valueName(),object);
    }
  }
  else if (objTyp == ObjectType::FenestrationSkylight) {
    if (!object.optionalCast<SubSurface>() || 
        !istringEqual("Skylight", object.cast<SubSurface>().subSurfaceType())) 
    {
       result = modelInputErrorMessage(objTyp.valueName(),object);
    }
    else {
      // parent should be roof
      OptionalParentObject opo = object.parent();
      if (!opo || !verifyObjectType(ObjectType::Roof,*opo).empty()) {
        result = modelInputErrorMessage(objTyp.valueName(),object);
      }
    }
  }
  else if (objTyp == ObjectType::Window) {
    if (!object.optionalCast<SubSurface>() || 
        !(istringEqual("FixedWindow", object.cast<SubSurface>().subSurfaceType()) || 
        istringEqual("OperableWindow", object.cast<SubSurface>().subSurfaceType()))) 
    {
       result = modelInputErrorMessage(objTyp.valueName(),object);
    }
  }
  else if (objTyp == ObjectType::ConstructionAssembly) {
    if (!object.optionalCast<ConstructionBase>()) {
      result = modelInputErrorMessage(objTyp.valueName(),object);
    }
  }
  else if (objTyp == ObjectType::Insulation) {
    if (!object.optionalCast<OpaqueMaterial>()) {
      result = modelInputErrorMessage(objTyp.valueName(),object);
    }
  }

  return result;
}

// ERROR MESSAGE HELPERS

std::string OpenStudioStandardsInterface::dataDictionaryErrorMessage(const std::string& entry) const {
  std::stringstream ss;
  ss << "Cannot parse " << entry << " as a data dictionary term.";
  return ss.str();
}

std::string OpenStudioStandardsInterface::dataDictionaryErrorMessage(
    const PartsOfSpeech& type,
    const std::string& entry) const 
{
  std::stringstream ss;
  ss << "Cannot execute OpenStudioStandardsInterface call because '" << entry
     << "' is not listed as a " << type.valueDescription()
     << " in the DataDictionary.";
  return ss.str();

}

std::string OpenStudioStandardsInterface::dataDictionaryErrorMessage(
    const EnumAttribute& e,
    const std::string& value) const 
{
  std::stringstream ss;
  ss << "Cannot identify " << value << " with a particular value in the data dictionary"
     << " enumeration " << e.valueDescription() << ".";
  return ss.str();
}

std::string OpenStudioStandardsInterface::modelInputErrorMessage(
    const std::string& ddObjectType, const model::ModelObject& object) const 
{
  std::stringstream ss;
  ss << "Cannot convert " << object.briefDescription() << " to DataDictionary ObjectType '" 
     << ddObjectType << "'.";
  return ss.str();
}

/// @endcond

} // standardsinterface
} // openstudio

