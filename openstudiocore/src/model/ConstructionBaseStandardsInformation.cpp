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

#include <model/ConstructionBaseStandardsInformation.hpp>
#include <model/ConstructionBaseStandardsInformation_Impl.hpp>
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

  ConstructionBaseStandardsInformation_Impl::ConstructionBaseStandardsInformation_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ConstructionBaseStandardsInformation::iddObjectType());
  }

  ConstructionBaseStandardsInformation_Impl::ConstructionBaseStandardsInformation_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ConstructionBaseStandardsInformation::iddObjectType());
  }

  ConstructionBaseStandardsInformation_Impl::ConstructionBaseStandardsInformation_Impl(
      const ConstructionBaseStandardsInformation_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ConstructionBase> ConstructionBaseStandardsInformation_Impl::construction() const {
    return getObject<ConstructionBaseStandardsInformation>().getModelObjectTarget<ConstructionBase>(
        OS_StandardsInformation_ConstructionFields::ConstructionName);
  }

  std::string ConstructionBaseStandardsInformation_Impl::intendedSurfaceType() const {
    OptionalString oResult = getString(OS_StandardsInformation_ConstructionFields::IntendedSurfaceType,true);
    if (oResult) { return *oResult; }
    return std::string();
  }

  std::string ConstructionBaseStandardsInformation_Impl::constructionType() const {
    OptionalString oResult = getString(OS_StandardsInformation_ConstructionFields::ConstructionType,true);
    if (oResult) { return *oResult; }
    return std::string();
  }

  boost::optional<Material> ConstructionBaseStandardsInformation_Impl::perturbableLayer() const {
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

  std::string ConstructionBaseStandardsInformation_Impl::perturbableLayerType() const {
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

  boost::optional<ParentObject> ConstructionBaseStandardsInformation_Impl::parent() const {
    OptionalConstructionBase candidate = construction();
    if (candidate) { return candidate->cast<ParentObject>(); }
    return boost::none;
  }

  const std::vector<std::string>& ConstructionBaseStandardsInformation_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ConstructionBaseStandardsInformation_Impl::iddObjectType() const {
    return ConstructionBaseStandardsInformation::iddObjectType();
  }

  void ConstructionBaseStandardsInformation_Impl::setConstruction(
      const ConstructionBase& construction) 
  {
    bool ok = setPointer(OS_StandardsInformation_ConstructionFields::ConstructionName,
                         construction.handle());
    OS_ASSERT(ok);
  }

  bool ConstructionBaseStandardsInformation_Impl::setIntendedSurfaceType(const std::string& type) {
    return setString(OS_StandardsInformation_ConstructionFields::IntendedSurfaceType,type);
  }

  void ConstructionBaseStandardsInformation_Impl::setConstructionType(const std::string& type) {
    bool ok = setString(OS_StandardsInformation_ConstructionFields::ConstructionType,type);
    OS_ASSERT(ok);
  }

  bool ConstructionBaseStandardsInformation_Impl::setPerturbableLayer(unsigned layerIndex) {
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

  bool ConstructionBaseStandardsInformation_Impl::setPerturbableLayer(const Material& material) {
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

  void ConstructionBaseStandardsInformation_Impl::resetPerturbableLayer() {
    bool ok = setString(OS_StandardsInformation_ConstructionFields::PerturbableLayer,"");
    OS_ASSERT(ok);
    ok = setString(OS_StandardsInformation_ConstructionFields::PerturbableLayerType,"");
    OS_ASSERT(ok);
  }

  void ConstructionBaseStandardsInformation_Impl::setPerturbableLayerType(const std::string& type) 
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

  bool ConstructionBaseStandardsInformation_Impl::setParent(ParentObject& newParent) {
    OptionalConstructionBase candidate = newParent.optionalCast<ConstructionBase>();
    if (candidate) { 
      setConstruction(*candidate);
      return true;
    }
    return false;
  }

} // detail

ConstructionBaseStandardsInformation::ConstructionBaseStandardsInformation(Model& model)
  : ModelObject(ConstructionBaseStandardsInformation::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ConstructionBaseStandardsInformation_Impl>());
}

ConstructionBaseStandardsInformation::ConstructionBaseStandardsInformation(
    const ConstructionBase& construction)
  : ModelObject(ConstructionBaseStandardsInformation::iddObjectType(),construction.model())
{
  OS_ASSERT(getImpl<detail::ConstructionBaseStandardsInformation_Impl>());
  setConstruction(construction);
}

std::vector<std::string> ConstructionBaseStandardsInformation::standardPerturbableLayerTypeValues() {
  StringVector result;
  StringVector candidates = getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                                           OS_StandardsInformation_ConstructionFields::PerturbableLayerType);
  BOOST_FOREACH(const std::string& candidate,candidates) {
    if ((candidate == "Not Applicable") || (candidate == "Other")) { continue; }
    result.push_back(candidate);
  }
  return result;
}

std::vector<std::string> ConstructionBaseStandardsInformation::intendedSurfaceTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_StandardsInformation_ConstructionFields::IntendedSurfaceType);
}

IddObjectType ConstructionBaseStandardsInformation::iddObjectType() {
  IddObjectType result(IddObjectType::OS_StandardsInformation_Construction);
  return result;
}

boost::optional<ConstructionBase> ConstructionBaseStandardsInformation::construction() const {
  return getImpl<detail::ConstructionBaseStandardsInformation_Impl>()->construction();
}

std::string ConstructionBaseStandardsInformation::intendedSurfaceType() const {
  return getImpl<detail::ConstructionBaseStandardsInformation_Impl>()->intendedSurfaceType();
}

std::string ConstructionBaseStandardsInformation::constructionType() const {
  return getImpl<detail::ConstructionBaseStandardsInformation_Impl>()->constructionType();
}

boost::optional<Material> ConstructionBaseStandardsInformation::perturbableLayer() const {
  return getImpl<detail::ConstructionBaseStandardsInformation_Impl>()->perturbableLayer();
}

std::string ConstructionBaseStandardsInformation::perturbableLayerType() const {
  return getImpl<detail::ConstructionBaseStandardsInformation_Impl>()->perturbableLayerType();
}

void ConstructionBaseStandardsInformation::setConstruction(const ConstructionBase& construction) {
  getImpl<detail::ConstructionBaseStandardsInformation_Impl>()->setConstruction(construction);
}

bool ConstructionBaseStandardsInformation::setIntendedSurfaceType(const std::string& type) {
  return getImpl<detail::ConstructionBaseStandardsInformation_Impl>()->setIntendedSurfaceType(type);
}

void ConstructionBaseStandardsInformation::setConstructionType(const std::string& type) {
  return getImpl<detail::ConstructionBaseStandardsInformation_Impl>()->setConstructionType(type);
}

bool ConstructionBaseStandardsInformation::setPerturbableLayer(unsigned layerIndex) {
  return getImpl<detail::ConstructionBaseStandardsInformation_Impl>()->setPerturbableLayer(layerIndex);
}

bool ConstructionBaseStandardsInformation::setPerturbableLayer(const Material& material) {
  return getImpl<detail::ConstructionBaseStandardsInformation_Impl>()->setPerturbableLayer(material);
}

void ConstructionBaseStandardsInformation::resetPerturbableLayer() {
  getImpl<detail::ConstructionBaseStandardsInformation_Impl>()->resetPerturbableLayer();
}

void ConstructionBaseStandardsInformation::setPerturbableLayerType(const std::string& type) {
  return getImpl<detail::ConstructionBaseStandardsInformation_Impl>()->setPerturbableLayerType(type);
}

/// @cond
ConstructionBaseStandardsInformation::ConstructionBaseStandardsInformation(
    boost::shared_ptr<detail::ConstructionBaseStandardsInformation_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio
