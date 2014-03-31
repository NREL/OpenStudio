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

#include <model/StandardsInformationConstruction.hpp>
#include <model/StandardsInformationConstruction_Impl.hpp>
#include <model/ConstructionBase.hpp>
#include <model/ConstructionBase_Impl.hpp>
#include <model/LayeredConstruction.hpp>
#include <model/LayeredConstruction_Impl.hpp>
#include <model/Material.hpp>

#include <model/Model.hpp>

#include <utilities/idd/OS_StandardsInformation_Construction_FieldEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  StandardsInformationConstruction_Impl::StandardsInformationConstruction_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == StandardsInformationConstruction::iddObjectType());
  }

  StandardsInformationConstruction_Impl::StandardsInformationConstruction_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == StandardsInformationConstruction::iddObjectType());
  }

  StandardsInformationConstruction_Impl::StandardsInformationConstruction_Impl(
      const StandardsInformationConstruction_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  ConstructionBase StandardsInformationConstruction_Impl::construction() const {
    boost::optional<ConstructionBase> result = getObject<StandardsInformationConstruction>().getModelObjectTarget<ConstructionBase>(
        OS_StandardsInformation_ConstructionFields::ConstructionName);
    OS_ASSERT(result);
    return *result;
  }

  boost::optional<std::string> StandardsInformationConstruction_Impl::intendedSurfaceType() const {
    return getString(OS_StandardsInformation_ConstructionFields::IntendedSurfaceType,false,true);
  }

  boost::optional<std::string> StandardsInformationConstruction_Impl::standardsConstructionType() const {
    return getString(OS_StandardsInformation_ConstructionFields::StandardsConstructionType,false,true);
  }

  std::vector<std::string> StandardsInformationConstruction_Impl::suggestedStandardsConstructionTypes() const {
    std::vector<std::string> result;

    boost::optional<std::string> intendedSurfaceType = this->intendedSurfaceType();
    boost::optional<std::string> standardsConstructionType = this->standardsConstructionType();

    // todo: pull from standards JSON file, for now just hard code here
	  // note: didn't add DemsingCeiling choices here. Thinking Intended Surafce Types should instead be DemisingCeiling
	  if (intendedSurfaceType){
      if (istringEqual(*intendedSurfaceType, "ExteriorFloor")){
        result.push_back("Mass");
        result.push_back("SteelFramed");
		    result.push_back("WoodFramed");
	    }
    }
	  if (intendedSurfaceType){
      if (istringEqual(*intendedSurfaceType, "ExteriorWall")){
        result.push_back("Mass");
        result.push_back("Metal");
        result.push_back("SteelFramed");
		    result.push_back("WoodFramed");
	    }
    }
	  if (intendedSurfaceType){
      if (istringEqual(*intendedSurfaceType, "ExteriorRoof")){
        result.push_back("IEAD");
        result.push_back("Metal");
	    }
    }
    if (intendedSurfaceType){
      if (istringEqual(*intendedSurfaceType, "InteriorFloor")){
        result.push_back("Mass");
        result.push_back("SteelFramed");
		    result.push_back("WoodFramed");
	    }
    }
    if (intendedSurfaceType){
      if (istringEqual(*intendedSurfaceType, "InterorWall")){
        result.push_back("Mass");
        result.push_back("Metal");
        result.push_back("SteelFramed");
		    result.push_back("WoodFramed");
	    }
    }
  	if (intendedSurfaceType){
      if (istringEqual(*intendedSurfaceType, "InteriorCeling")){
        result.push_back("Mass");
        result.push_back("SteelFramed");
		    result.push_back("WoodFramed");
	    }
    }
	  if (intendedSurfaceType){
      if (istringEqual(*intendedSurfaceType, "DemisingFloor")){
        result.push_back("Mass");
        result.push_back("SteelFramed");
		    result.push_back("WoodFramed");
	    }
    }
    if (intendedSurfaceType){
      if (istringEqual(*intendedSurfaceType, "DemisingWall")){
        result.push_back("Mass");
        result.push_back("Metal");
        result.push_back("SteelFramed");
		    result.push_back("WoodFramed");
	    }
    }
    if (intendedSurfaceType){
      if (istringEqual(*intendedSurfaceType, "GroundContactFloor")){
        result.push_back("Heated");
        result.push_back("Unheated");
	    }
    }

	// include values from model

    BOOST_FOREACH(const StandardsInformationConstruction& other, this->model().getConcreteModelObjects<StandardsInformationConstruction>()){
      if (other.handle() == this->handle()){
        continue;
      }

      boost::optional<std::string> otherIntendedSurfaceType = other.intendedSurfaceType();
      if (intendedSurfaceType && otherIntendedSurfaceType){
        // need to be the same
        if (intendedSurfaceType.get() != otherIntendedSurfaceType.get()){
          continue;
        }
      }else if (!intendedSurfaceType && !otherIntendedSurfaceType){
        // both empty
      }else{
        // different
        continue;
      }

      boost::optional<std::string> otherStandardsConstructionType = other.standardsConstructionType();
      if (otherStandardsConstructionType){
        result.push_back(*otherStandardsConstructionType);
      }
    }

    // remove standardsBuildingType
    IstringFind finder;
    if (standardsConstructionType){
      finder.addTarget(*standardsConstructionType);
    }
    std::vector<std::string>::iterator it = std::remove_if(result.begin(), result.end(), finder); 
    result.resize( std::distance(result.begin(),it) ); 

    // sort
    std::sort(result.begin(), result.end(), IstringCompare());

    // make unique
    // DLM: have to sort before calling unique, unique only works on consecutive elements
    it = std::unique(result.begin(), result.end(), IstringEqual()); 
    result.resize( std::distance(result.begin(),it) ); 

    // add current to front
    if (standardsConstructionType){
      result.insert(result.begin(), *standardsConstructionType);
    }
  
    return result;
  }

  boost::optional<Material> StandardsInformationConstruction_Impl::perturbableLayer() const {
    OptionalUnsigned oLayerIndex = getUnsigned(OS_StandardsInformation_ConstructionFields::PerturbableLayer);
    if (!oLayerIndex) { return boost::none; }
    unsigned layerIndex(*oLayerIndex);

    OptionalConstructionBase oConstructionBase = construction();
    if (!oConstructionBase) { return boost::none; }
    OptionalLayeredConstruction oLayeredConstruction = oConstructionBase->optionalCast<LayeredConstruction>();
    if (!oLayeredConstruction) { return boost::none; }
    LayeredConstruction layeredConstruction(*oLayeredConstruction);
    if (layeredConstruction.numLayers() > layerIndex) {
      return layeredConstruction.getLayer(layerIndex);
    }
    return boost::none;
  }

  std::string StandardsInformationConstruction_Impl::perturbableLayerType() const {
    std::string result;
    OptionalString choiceValue = getString(OS_StandardsInformation_ConstructionFields::PerturbableLayerType,true);
    OS_ASSERT(choiceValue);
    if (istringEqual("Not Applicable",*choiceValue)) { return result; }
    if (istringEqual("Other",*choiceValue)) {
      OptionalString otherValue = getString(OS_StandardsInformation_ConstructionFields::OtherPerturbableLayerType);
      if (otherValue) { result = *otherValue; }
    }
    else { result = *choiceValue; }
    return result; 
  }

  boost::optional<ParentObject> StandardsInformationConstruction_Impl::parent() const {
    OptionalConstructionBase candidate = construction();
    if (candidate) { return candidate->cast<ParentObject>(); }
    return boost::none;
  }

  bool StandardsInformationConstruction_Impl::setParent(ParentObject& newParent) {
    OptionalConstructionBase candidate = newParent.optionalCast<ConstructionBase>();
    if (candidate){
      if (candidate->getModelObjectSources<StandardsInformationConstruction>().empty()){
        bool test = this->setPointer(OS_StandardsInformation_ConstructionFields::ConstructionName, candidate->handle());
        return test;
      }
    }
    return false;
  }

  const std::vector<std::string>& StandardsInformationConstruction_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType StandardsInformationConstruction_Impl::iddObjectType() const {
    return StandardsInformationConstruction::iddObjectType();
  }

  bool StandardsInformationConstruction_Impl::setIntendedSurfaceType(const std::string& type) {
    return setString(OS_StandardsInformation_ConstructionFields::IntendedSurfaceType,type);
  }

  void StandardsInformationConstruction_Impl::resetIntendedSurfaceType() {
    bool ok = setString(OS_StandardsInformation_ConstructionFields::IntendedSurfaceType,"");
    OS_ASSERT(ok);
  }

  void StandardsInformationConstruction_Impl::setStandardsConstructionType(const std::string& type) {
    bool ok = setString(OS_StandardsInformation_ConstructionFields::StandardsConstructionType,type);
    OS_ASSERT(ok);
  }

  void StandardsInformationConstruction_Impl::resetStandardsConstructionType() {
    bool ok = setString(OS_StandardsInformation_ConstructionFields::StandardsConstructionType,"");
    OS_ASSERT(ok);
  }

  bool StandardsInformationConstruction_Impl::setPerturbableLayer(unsigned layerIndex) {
    bool ok(true);

    OptionalConstructionBase oConstructionBase = construction();
    if (!oConstructionBase) { return false; }
    OptionalLayeredConstruction oLayeredConstruction = 
        oConstructionBase->optionalCast<LayeredConstruction>();
    if (!oLayeredConstruction) { return false; }

    if (layerIndex >= oLayeredConstruction->numLayers()) { return false; }
    
    ok = setUnsigned(OS_StandardsInformation_ConstructionFields::PerturbableLayer,layerIndex);
    OS_ASSERT(ok);
    return true;
  }

  bool StandardsInformationConstruction_Impl::setPerturbableLayer(const Material& material) {
    bool ok(true);

    OptionalConstructionBase oConstructionBase = construction();
    if (!oConstructionBase) { return false; }
    OptionalLayeredConstruction oLayeredConstruction = 
        oConstructionBase->optionalCast<LayeredConstruction>();
    if (!oLayeredConstruction) { return false; }

    UnsignedVector layerIndices = oLayeredConstruction->getLayerIndices(material);
    if (layerIndices.empty()) { return false; }
    
    ok = setUnsigned(OS_StandardsInformation_ConstructionFields::PerturbableLayer,layerIndices[0]);
    OS_ASSERT(ok);
    return true;
  }

  void StandardsInformationConstruction_Impl::resetPerturbableLayer() {
    bool ok = setString(OS_StandardsInformation_ConstructionFields::PerturbableLayer,"");
    OS_ASSERT(ok);
    ok = setString(OS_StandardsInformation_ConstructionFields::PerturbableLayerType,"");
    OS_ASSERT(ok);
  }

  void StandardsInformationConstruction_Impl::setPerturbableLayerType(const std::string& type) 
  {
    bool ok(true);

    if (type.empty()) {
      // clear any existing values
      ok = setString(OS_StandardsInformation_ConstructionFields::PerturbableLayerType,"");
      OS_ASSERT(ok); 
      ok = setString(OS_StandardsInformation_ConstructionFields::OtherPerturbableLayerType,"");
      OS_ASSERT(ok);
      return;
    }

    ok = setString(OS_StandardsInformation_ConstructionFields::PerturbableLayerType,type);
    if (!ok) {
      ok = setString(OS_StandardsInformation_ConstructionFields::PerturbableLayerType,"Other");
      OS_ASSERT(ok);
      ok = setString(OS_StandardsInformation_ConstructionFields::OtherPerturbableLayerType,type);
      OS_ASSERT(ok);
    }
    else {
      ok = setString(OS_StandardsInformation_ConstructionFields::OtherPerturbableLayerType,"");
      OS_ASSERT(ok);
    }
  }

  void StandardsInformationConstruction_Impl::resetPerturbableLayerType() 
  {
    bool ok = setString(OS_StandardsInformation_ConstructionFields::PerturbableLayerType,"");
    OS_ASSERT(ok); 
    ok = setString(OS_StandardsInformation_ConstructionFields::OtherPerturbableLayerType,"");
    OS_ASSERT(ok);
  }
} // detail

StandardsInformationConstruction::StandardsInformationConstruction(
    const ConstructionBase& construction)
  : ModelObject(StandardsInformationConstruction::iddObjectType(),construction.model())
{
  OS_ASSERT(getImpl<detail::StandardsInformationConstruction_Impl>());
  bool ok = setPointer(OS_StandardsInformation_ConstructionFields::ConstructionName,construction.handle());
  OS_ASSERT(ok);
}

std::vector<std::string> StandardsInformationConstruction::standardPerturbableLayerTypeValues() {
  StringVector result;
  StringVector candidates = getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                                           OS_StandardsInformation_ConstructionFields::PerturbableLayerType);
  BOOST_FOREACH(const std::string& candidate,candidates) {
    if ((candidate == "Not Applicable") || (candidate == "Other")) { continue; }
    result.push_back(candidate);
  }
  return result;
}

std::vector<std::string> StandardsInformationConstruction::intendedSurfaceTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_StandardsInformation_ConstructionFields::IntendedSurfaceType);
}

IddObjectType StandardsInformationConstruction::iddObjectType() {
  IddObjectType result(IddObjectType::OS_StandardsInformation_Construction);
  return result;
}

ConstructionBase StandardsInformationConstruction::construction() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->construction();
}

boost::optional<std::string> StandardsInformationConstruction::intendedSurfaceType() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->intendedSurfaceType();
}

boost::optional<std::string> StandardsInformationConstruction::standardsConstructionType() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->standardsConstructionType();
}

std::vector<std::string> StandardsInformationConstruction::suggestedStandardsConstructionTypes() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->suggestedStandardsConstructionTypes();
}

boost::optional<Material> StandardsInformationConstruction::perturbableLayer() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->perturbableLayer();
}

boost::optional<std::string> StandardsInformationConstruction::perturbableLayerType() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->perturbableLayerType();
}

bool StandardsInformationConstruction::setIntendedSurfaceType(const std::string& type) {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->setIntendedSurfaceType(type);
}

void StandardsInformationConstruction::resetIntendedSurfaceType() {
  getImpl<detail::StandardsInformationConstruction_Impl>()->resetIntendedSurfaceType();
}

void StandardsInformationConstruction::setStandardsConstructionType(const std::string& type) {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->setStandardsConstructionType(type);
}

void StandardsInformationConstruction::resetStandardsConstructionType() {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->resetStandardsConstructionType();
}

bool StandardsInformationConstruction::setPerturbableLayer(unsigned layerIndex) {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->setPerturbableLayer(layerIndex);
}

bool StandardsInformationConstruction::setPerturbableLayer(const Material& material) {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->setPerturbableLayer(material);
}

void StandardsInformationConstruction::resetPerturbableLayer() {
  getImpl<detail::StandardsInformationConstruction_Impl>()->resetPerturbableLayer();
}

void StandardsInformationConstruction::setPerturbableLayerType(const std::string& type) {
  getImpl<detail::StandardsInformationConstruction_Impl>()->setPerturbableLayerType(type);
}

void StandardsInformationConstruction::resetPerturbableLayerType() {
  getImpl<detail::StandardsInformationConstruction_Impl>()->resetPerturbableLayerType();
}

/// @cond
StandardsInformationConstruction::StandardsInformationConstruction(
    boost::shared_ptr<detail::StandardsInformationConstruction_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio
