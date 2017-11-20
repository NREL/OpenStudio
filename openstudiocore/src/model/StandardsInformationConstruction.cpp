/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "StandardsInformationConstruction.hpp"
#include "StandardsInformationConstruction_Impl.hpp"

#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "LayeredConstruction.hpp"
#include "LayeredConstruction_Impl.hpp"
#include "Material.hpp"

#include "Model.hpp"

#include <utilities/idd/OS_StandardsInformation_Construction_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../utilities/core/Assert.hpp"

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
    return getString(OS_StandardsInformation_ConstructionFields::IntendedSurfaceType,true,true);
  }

  boost::optional<std::string> StandardsInformationConstruction_Impl::standardsConstructionType() const {
    return getString(OS_StandardsInformation_ConstructionFields::StandardsConstructionType,true,true);
  }

  std::vector<std::string> StandardsInformationConstruction_Impl::suggestedStandardsConstructionTypes() const {
    std::vector<std::string> result;

    boost::optional<std::string> intendedSurfaceType = this->intendedSurfaceType();
    boost::optional<std::string> standardsConstructionType = this->standardsConstructionType();

    // todo: pull from standards JSON file, for now just hard code here
    // note: didn't add DemsingCeiling choices here. Thinking Intended Surface Types should instead be DemisingCeiling
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

    for (const StandardsInformationConstruction& other : this->model().getConcreteModelObjects<StandardsInformationConstruction>()){
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

      auto it = std::remove_if(result.begin(), result.end(), finder);
      result.resize(std::distance(result.begin(), it));
    }

    // sort
    std::sort(result.begin(), result.end(), IstringCompare());

    // make unique
    // DLM: have to sort before calling unique, unique only works on consecutive elements
    auto it = std::unique(result.begin(), result.end(), IstringEqual());
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
    OptionalString choiceValue = getString(OS_StandardsInformation_ConstructionFields::PerturbableLayerType,true,true);
    OS_ASSERT(choiceValue);
    if (istringEqual("Not Applicable",*choiceValue)) { return result; }
    if (istringEqual("Other",*choiceValue)) {
      OptionalString otherValue = getString(OS_StandardsInformation_ConstructionFields::OtherPerturbableLayerType,true,true);
      if (otherValue) { result = *otherValue; }
    }
    else { result = *choiceValue; }
    return result;
  }

  bool StandardsInformationConstruction_Impl::isPerturbableLayerTypeDefaulted() const {
    return isEmpty(OS_StandardsInformation_ConstructionFields::PerturbableLayerType);
  }

  boost::optional<std::string> StandardsInformationConstruction_Impl::otherPerturbableLayerType() const {
    return getString(OS_StandardsInformation_ConstructionFields::OtherPerturbableLayerType,true,true);
  }

  std::vector<std::string> StandardsInformationConstruction_Impl::suggestedConstructionStandards() const {

    std::vector<std::string> result;

    boost::optional<std::string> constructionStandard = this->constructionStandard();

    // include values from json
    //parseStandardsMap();

    //QMap<QString, QVariant> materials = m_standardsMap["materials"].toMap();
    //for (QString material_name : materials.uniqueKeys()) {
    //  QMap<QString, QVariant> material = materials[material_name].toMap();
    //  QString tmp = material["material_standard"].toString();
    //  if (!tmp.isEmpty()){
    //    result.push_back(toString(tmp));
    //  }
    //}

    // todo: pull from standards JSON file, for now just hard code here
    result.push_back("CEC Title24-2013");

    // include values from model
    for (const StandardsInformationConstruction& other : this->model().getConcreteModelObjects<StandardsInformationConstruction>()){
      if (other.handle() == this->handle()){
        continue;
      }

      boost::optional<std::string> otherConstructionStandard = other.constructionStandard();
      if (otherConstructionStandard){
        result.push_back(*otherConstructionStandard);
      }
    }

    // remove current
    IstringFind finder;
    if (constructionStandard){
      finder.addTarget(*constructionStandard);

      auto it = std::remove_if(result.begin(), result.end(), finder);
      result.resize(std::distance(result.begin(), it));
    }

    // sort
    std::sort(result.begin(), result.end(), IstringCompare());

    // make unique
    // DLM: have to sort before calling unique, unique only works on consecutive elements
    auto it = std::unique(result.begin(), result.end(), IstringEqual());
    result.resize(std::distance(result.begin(), it));

    // add current to front
    if (constructionStandard){
      result.insert(result.begin(), *constructionStandard);
    }

    return result;

  }

  boost::optional<std::string> StandardsInformationConstruction_Impl::constructionStandard() const {
    return getString(OS_StandardsInformation_ConstructionFields::ConstructionStandard,true,true);
  }

  std::vector<std::string> StandardsInformationConstruction_Impl::suggestedConstructionStandardSources() const {
    std::vector<std::string> result;

    boost::optional<std::string> constructionStandard = this->constructionStandard();
    boost::optional<std::string> constructionStandardSource = this->constructionStandardSource();

    if (!constructionStandard){
      if (constructionStandardSource){
        result.push_back(*constructionStandardSource);
      }
      return result;
    }


    // todo: pull from standards JSON file, for now just hard code here
    if (istringEqual(*constructionStandard, "CEC Title24-2013")){
      //result.push_back("NA6");
    }

    // include values from model

    for (const StandardsInformationConstruction& other : this->model().getConcreteModelObjects<StandardsInformationConstruction>()){
      if (other.handle() == this->handle()){
        continue;
      }

      boost::optional<std::string> otherConstructionStandard = other.constructionStandard();
      if (!otherConstructionStandard){
        continue;
      } else if (*constructionStandard != *otherConstructionStandard){
        continue;
      }

      boost::optional<std::string> otherConstructionStandardSource = other.constructionStandardSource();
      if (otherConstructionStandardSource){
        result.push_back(*otherConstructionStandardSource);
      }
    }

    // remove standardsBuildingType
    IstringFind finder;
    if (constructionStandardSource){
      finder.addTarget(*constructionStandardSource);

      auto it = std::remove_if(result.begin(), result.end(), finder);
      result.resize(std::distance(result.begin(), it));
    }

    // sort
    std::sort(result.begin(), result.end(), IstringCompare());

    // make unique
    // DLM: have to sort before calling unique, unique only works on consecutive elements
    auto it = std::unique(result.begin(), result.end(), IstringEqual());
    result.resize(std::distance(result.begin(), it));

    // add current to front
    if (constructionStandardSource){
      result.insert(result.begin(), *constructionStandardSource);
    }

    return result;
  }

  boost::optional<std::string> StandardsInformationConstruction_Impl::constructionStandardSource() const {
    return getString(OS_StandardsInformation_ConstructionFields::ConstructionStandardSource,true,true);
  }

  boost::optional<std::string> StandardsInformationConstruction_Impl::fenestrationType() const {
    return getString(OS_StandardsInformation_ConstructionFields::FenestrationType,true,true);
  }

  boost::optional<std::string> StandardsInformationConstruction_Impl::fenestrationAssemblyContext() const {
    return getString(OS_StandardsInformation_ConstructionFields::FenestrationAssemblyContext,true,true);
  }

  boost::optional<std::string> StandardsInformationConstruction_Impl::fenestrationNumberOfPanes() const {
    return getString(OS_StandardsInformation_ConstructionFields::FenestrationNumberofPanes,true,true);
  }

  boost::optional<std::string> StandardsInformationConstruction_Impl::fenestrationFrameType() const {
    return getString(OS_StandardsInformation_ConstructionFields::FenestrationFrameType,true,true);
  }

  boost::optional<std::string> StandardsInformationConstruction_Impl::fenestrationDividerType() const {
    return getString(OS_StandardsInformation_ConstructionFields::FenestrationDividerType,true,true);
  }

  boost::optional<std::string> StandardsInformationConstruction_Impl::fenestrationTint() const {
    return getString(OS_StandardsInformation_ConstructionFields::FenestrationTint,true,true);
  }

  boost::optional<std::string> StandardsInformationConstruction_Impl::fenestrationGasFill() const {
    return getString(OS_StandardsInformation_ConstructionFields::FenestrationGasFill,true,true);
  }

  bool StandardsInformationConstruction_Impl::fenestrationLowEmissivityCoating() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_StandardsInformation_ConstructionFields::FenestrationLowEmissivityCoating,true,true);
    if (value){
      result = openstudio::istringEqual(value.get(), "True");
    }
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

  void StandardsInformationConstruction_Impl::setOtherPerturbableLayerType(const std::string& otherPerturbableLayerType) {
    bool result = setString(OS_StandardsInformation_ConstructionFields::OtherPerturbableLayerType, otherPerturbableLayerType);
    OS_ASSERT(result);
  }

  void StandardsInformationConstruction_Impl::resetOtherPerturbableLayerType() {
    bool result = setString(OS_StandardsInformation_ConstructionFields::OtherPerturbableLayerType, "");
    OS_ASSERT(result);
  }

  void StandardsInformationConstruction_Impl::setConstructionStandard(const std::string& constructionStandard) {
    bool result = setString(OS_StandardsInformation_ConstructionFields::ConstructionStandard, constructionStandard);
    OS_ASSERT(result);
  }

  void StandardsInformationConstruction_Impl::resetConstructionStandard() {
    bool result = setString(OS_StandardsInformation_ConstructionFields::ConstructionStandard, "");
    OS_ASSERT(result);
  }

  void StandardsInformationConstruction_Impl::setConstructionStandardSource(const std::string& constructionStandardSource) {
    bool result= setString(OS_StandardsInformation_ConstructionFields::ConstructionStandardSource, constructionStandardSource);
    OS_ASSERT(result);
  }

  void StandardsInformationConstruction_Impl::resetConstructionStandardSource() {
    bool result = setString(OS_StandardsInformation_ConstructionFields::ConstructionStandardSource, "");
    OS_ASSERT(result);
  }

  bool StandardsInformationConstruction_Impl::setFenestrationType(const std::string& fenestrationType) {
    bool result = setString(OS_StandardsInformation_ConstructionFields::FenestrationType, fenestrationType);
    return result;
  }

  void StandardsInformationConstruction_Impl::resetFenestrationType() {
    bool result = setString(OS_StandardsInformation_ConstructionFields::FenestrationType, "");
    OS_ASSERT(result);
  }

  bool StandardsInformationConstruction_Impl::setFenestrationAssemblyContext(const std::string& fenestrationAssemblyContext) {
    bool result = setString(OS_StandardsInformation_ConstructionFields::FenestrationAssemblyContext, fenestrationAssemblyContext);
    return result;
  }

  void StandardsInformationConstruction_Impl::resetFenestrationAssemblyContext() {
    bool result = setString(OS_StandardsInformation_ConstructionFields::FenestrationAssemblyContext, "");
    OS_ASSERT(result);
  }

  bool StandardsInformationConstruction_Impl::setFenestrationNumberOfPanes(const std::string& fenestrationNumberOfPanes) {
    bool result = setString(OS_StandardsInformation_ConstructionFields::FenestrationNumberofPanes, fenestrationNumberOfPanes);
    return result;
  }

  void StandardsInformationConstruction_Impl::resetFenestrationNumberOfPanes() {
    bool result = setString(OS_StandardsInformation_ConstructionFields::FenestrationNumberofPanes, "");
    OS_ASSERT(result);
  }

  bool StandardsInformationConstruction_Impl::setFenestrationFrameType(const std::string& fenestrationFrameType) {
    bool result = setString(OS_StandardsInformation_ConstructionFields::FenestrationFrameType, fenestrationFrameType);
    return result;
  }

  void StandardsInformationConstruction_Impl::resetFenestrationFrameType() {
    bool result = setString(OS_StandardsInformation_ConstructionFields::FenestrationFrameType, "");
    OS_ASSERT(result);
  }

  bool StandardsInformationConstruction_Impl::setFenestrationDividerType(const std::string& fenestrationDividerType) {
    bool result = setString(OS_StandardsInformation_ConstructionFields::FenestrationDividerType, fenestrationDividerType);
    return result;
  }

  void StandardsInformationConstruction_Impl::resetFenestrationDividerType() {
    bool result = setString(OS_StandardsInformation_ConstructionFields::FenestrationDividerType, "");
    OS_ASSERT(result);
  }

  bool StandardsInformationConstruction_Impl::setFenestrationTint(const std::string& fenestrationTint) {
    bool result = setString(OS_StandardsInformation_ConstructionFields::FenestrationTint, fenestrationTint);
    return result;
  }

  void StandardsInformationConstruction_Impl::resetFenestrationTint() {
    bool result = setString(OS_StandardsInformation_ConstructionFields::FenestrationTint, "");
    OS_ASSERT(result);
  }

  bool StandardsInformationConstruction_Impl::setFenestrationGasFill(const std::string& fenestrationGasFill) {
    bool result = setString(OS_StandardsInformation_ConstructionFields::FenestrationGasFill, fenestrationGasFill);
    return result;
  }

  void StandardsInformationConstruction_Impl::resetFenestrationGasFill() {
    bool result = setString(OS_StandardsInformation_ConstructionFields::FenestrationGasFill, "");
    OS_ASSERT(result);
  }

  void StandardsInformationConstruction_Impl::setFenestrationLowEmissivityCoating(bool fenestrationLowEmissivityCoating) {
    bool result;
    if (fenestrationLowEmissivityCoating) {
      result = setString(OS_StandardsInformation_ConstructionFields::FenestrationLowEmissivityCoating, "True");
    } else {
      result = setString(OS_StandardsInformation_ConstructionFields::FenestrationLowEmissivityCoating, "False");
    }
    OS_ASSERT(result);
  }

  void StandardsInformationConstruction_Impl::resetFenestrationLowEmissivityCoating() {
    bool result = setString(OS_StandardsInformation_ConstructionFields::FenestrationLowEmissivityCoating, "");
    OS_ASSERT(result);
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
  for (const std::string& candidate : candidates) {
    if ((candidate == "Not Applicable") || (candidate == "Other")) { continue; }
    result.push_back(candidate);
  }
  return result;
}

std::vector<std::string> StandardsInformationConstruction::intendedSurfaceTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_StandardsInformation_ConstructionFields::IntendedSurfaceType);
}

std::vector<std::string> StandardsInformationConstruction::fenestrationTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_StandardsInformation_ConstructionFields::FenestrationType);
}

std::vector<std::string> StandardsInformationConstruction::fenestrationAssemblyContextValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_StandardsInformation_ConstructionFields::FenestrationAssemblyContext);
}

std::vector<std::string> StandardsInformationConstruction::fenestrationNumberOfPanesValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_StandardsInformation_ConstructionFields::FenestrationNumberofPanes);
}

std::vector<std::string> StandardsInformationConstruction::fenestrationFrameTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_StandardsInformation_ConstructionFields::FenestrationFrameType);
}

std::vector<std::string> StandardsInformationConstruction::fenestrationDividerTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_StandardsInformation_ConstructionFields::FenestrationDividerType);
}

std::vector<std::string> StandardsInformationConstruction::fenestrationTintValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_StandardsInformation_ConstructionFields::FenestrationTint);
}

std::vector<std::string> StandardsInformationConstruction::fenestrationGasFillValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_StandardsInformation_ConstructionFields::FenestrationGasFill);
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

bool StandardsInformationConstruction::isPerturbableLayerTypeDefaulted() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->isPerturbableLayerTypeDefaulted();
}

boost::optional<std::string> StandardsInformationConstruction::otherPerturbableLayerType() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->otherPerturbableLayerType();
}

std::vector<std::string> StandardsInformationConstruction::suggestedConstructionStandards() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->suggestedConstructionStandards();
}

boost::optional<std::string> StandardsInformationConstruction::constructionStandard() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->constructionStandard();
}

std::vector<std::string> StandardsInformationConstruction::suggestedConstructionStandardSources() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->suggestedConstructionStandardSources();
}

boost::optional<std::string> StandardsInformationConstruction::constructionStandardSource() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->constructionStandardSource();
}

boost::optional<std::string> StandardsInformationConstruction::fenestrationType() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->fenestrationType();
}

boost::optional<std::string> StandardsInformationConstruction::fenestrationAssemblyContext() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->fenestrationAssemblyContext();
}

boost::optional<std::string> StandardsInformationConstruction::fenestrationNumberOfPanes() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->fenestrationNumberOfPanes();
}

boost::optional<std::string> StandardsInformationConstruction::fenestrationFrameType() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->fenestrationFrameType();
}

boost::optional<std::string> StandardsInformationConstruction::fenestrationDividerType() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->fenestrationDividerType();
}

boost::optional<std::string> StandardsInformationConstruction::fenestrationTint() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->fenestrationTint();
}

boost::optional<std::string> StandardsInformationConstruction::fenestrationGasFill() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->fenestrationGasFill();
}

bool StandardsInformationConstruction::fenestrationLowEmissivityCoating() const {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->fenestrationLowEmissivityCoating();
}

bool StandardsInformationConstruction::setIntendedSurfaceType(const std::string& type) {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->setIntendedSurfaceType(type);
}

void StandardsInformationConstruction::resetIntendedSurfaceType() {
  getImpl<detail::StandardsInformationConstruction_Impl>()->resetIntendedSurfaceType();
}

void StandardsInformationConstruction::setStandardsConstructionType(const std::string& type) {
  getImpl<detail::StandardsInformationConstruction_Impl>()->setStandardsConstructionType(type);
}

void StandardsInformationConstruction::resetStandardsConstructionType() {
  getImpl<detail::StandardsInformationConstruction_Impl>()->resetStandardsConstructionType();
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

void StandardsInformationConstruction::setOtherPerturbableLayerType(const std::string& otherPerturbableLayerType) {
  getImpl<detail::StandardsInformationConstruction_Impl>()->setOtherPerturbableLayerType(otherPerturbableLayerType);
}

void StandardsInformationConstruction::resetOtherPerturbableLayerType() {
  getImpl<detail::StandardsInformationConstruction_Impl>()->resetOtherPerturbableLayerType();
}

void StandardsInformationConstruction::setConstructionStandard(const std::string& constructionStandard) {
  getImpl<detail::StandardsInformationConstruction_Impl>()->setConstructionStandard(constructionStandard);
}

void StandardsInformationConstruction::resetConstructionStandard() {
  getImpl<detail::StandardsInformationConstruction_Impl>()->resetConstructionStandard();
}

void StandardsInformationConstruction::setConstructionStandardSource(const std::string& constructionStandardSource) {
  getImpl<detail::StandardsInformationConstruction_Impl>()->setConstructionStandardSource(constructionStandardSource);
}

void StandardsInformationConstruction::resetConstructionStandardSource() {
  getImpl<detail::StandardsInformationConstruction_Impl>()->resetConstructionStandardSource();
}

bool StandardsInformationConstruction::setFenestrationType(const std::string& fenestrationType) {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->setFenestrationType(fenestrationType);
}

void StandardsInformationConstruction::resetFenestrationType() {
  getImpl<detail::StandardsInformationConstruction_Impl>()->resetFenestrationType();
}

bool StandardsInformationConstruction::setFenestrationAssemblyContext(const std::string& fenestrationAssemblyContext) {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->setFenestrationAssemblyContext(fenestrationAssemblyContext);
}

void StandardsInformationConstruction::resetFenestrationAssemblyContext() {
  getImpl<detail::StandardsInformationConstruction_Impl>()->resetFenestrationAssemblyContext();
}

bool StandardsInformationConstruction::setFenestrationNumberOfPanes(const std::string& fenestrationNumberOfPanes) {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->setFenestrationNumberOfPanes(fenestrationNumberOfPanes);
}

void StandardsInformationConstruction::resetFenestrationNumberOfPanes() {
  getImpl<detail::StandardsInformationConstruction_Impl>()->resetFenestrationNumberOfPanes();
}

bool StandardsInformationConstruction::setFenestrationFrameType(const std::string& fenestrationFrameType) {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->setFenestrationFrameType(fenestrationFrameType);
}

void StandardsInformationConstruction::resetFenestrationFrameType() {
  getImpl<detail::StandardsInformationConstruction_Impl>()->resetFenestrationFrameType();
}

bool StandardsInformationConstruction::setFenestrationDividerType(const std::string& fenestrationDividerType) {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->setFenestrationDividerType(fenestrationDividerType);
}

void StandardsInformationConstruction::resetFenestrationDividerType() {
  getImpl<detail::StandardsInformationConstruction_Impl>()->resetFenestrationDividerType();
}

bool StandardsInformationConstruction::setFenestrationTint(const std::string& fenestrationTint) {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->setFenestrationTint(fenestrationTint);
}

void StandardsInformationConstruction::resetFenestrationTint() {
  getImpl<detail::StandardsInformationConstruction_Impl>()->resetFenestrationTint();
}

bool StandardsInformationConstruction::setFenestrationGasFill(const std::string& fenestrationGasFill) {
  return getImpl<detail::StandardsInformationConstruction_Impl>()->setFenestrationGasFill(fenestrationGasFill);
}

void StandardsInformationConstruction::resetFenestrationGasFill() {
  getImpl<detail::StandardsInformationConstruction_Impl>()->resetFenestrationGasFill();
}

void StandardsInformationConstruction::setFenestrationLowEmissivityCoating(bool fenestrationLowEmissivityCoating) {
  getImpl<detail::StandardsInformationConstruction_Impl>()->setFenestrationLowEmissivityCoating(fenestrationLowEmissivityCoating);
}

void StandardsInformationConstruction::resetFenestrationLowEmissivityCoating() {
  getImpl<detail::StandardsInformationConstruction_Impl>()->resetFenestrationLowEmissivityCoating();
}

/// @cond
StandardsInformationConstruction::StandardsInformationConstruction(
    std::shared_ptr<detail::StandardsInformationConstruction_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

