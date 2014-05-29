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

#include "LayeredConstruction.hpp"
#include "LayeredConstruction_Impl.hpp"
#include "Model.hpp"

#include "StandardsInformationConstruction.hpp"
#include "Material.hpp"
#include "OpaqueMaterial.hpp"
#include "OpaqueMaterial_Impl.hpp"
#include "AirGap.hpp"
#include "AirGap_Impl.hpp"
#include "RoofVegetation.hpp"
#include "RoofVegetation_Impl.hpp"
#include "FenestrationMaterial.hpp"
#include "FenestrationMaterial_Impl.hpp"
#include "SimpleGlazing.hpp"
#include "SimpleGlazing_Impl.hpp"
#include "StandardGlazing.hpp"
#include "StandardGlazing_Impl.hpp"
#include "RefractionExtinctionGlazing.hpp"
#include "RefractionExtinctionGlazing_Impl.hpp"
#include "GasLayer.hpp"
#include "GasLayer_Impl.hpp"
#include "ShadingMaterial.hpp"
#include "ShadingMaterial_Impl.hpp"
#include "ModelPartitionMaterial.hpp"
#include "ModelPartitionMaterial_Impl.hpp"

#include "ModelExtensibleGroup.hpp"

#include "../utilities/idf/ValidityReport.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  LayeredConstruction_Impl::LayeredConstruction_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ConstructionBase_Impl(idfObject, model, keepHandle)
  {}

  LayeredConstruction_Impl::LayeredConstruction_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : ConstructionBase_Impl(other, model, keepHandle)
  {}
  
  LayeredConstruction_Impl::LayeredConstruction_Impl(const LayeredConstruction_Impl& other, 
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : ConstructionBase_Impl(other, model, keepHandle)
  {}

  std::vector<Material> LayeredConstruction_Impl::layers() const {
    MaterialVector result;

    // loop through extensible groups
    for (const IdfExtensibleGroup& idfGroup : extensibleGroups()) {
      ModelExtensibleGroup group = idfGroup.cast<ModelExtensibleGroup>();
      // get object pointed to by extensible group
      // unit test checks that both layered constructions have extensible groups of size 1
      // implementation must change if that test starts failing
      OptionalMaterial oMaterial = group.getModelObjectTarget<Material>(0);
      if (!oMaterial) {
        LOG(Warn,"Skipping layer " << group.groupIndex() << " in " << briefDescription()
            << ", as there is no Material object referenced by the corresponding field.");
        continue;
      }
      result.push_back(*oMaterial);
    }

    return result;
  }

  Material LayeredConstruction_Impl::getLayer(unsigned layerIndex) const {
    if (layerIndex >= numLayers()) { 
      LOG_AND_THROW("Asked to get material layer indexed " << layerIndex << ", but " 
          << briefDescription() << " has just " << numLayers() << " layers.");
    }

    IdfExtensibleGroup idfGroup = getExtensibleGroup(layerIndex);
    OS_ASSERT(!idfGroup.empty());
    ModelExtensibleGroup group = idfGroup.cast<ModelExtensibleGroup>();
    OptionalMaterial oMaterial = group.getModelObjectTarget<Material>(0);
    if (!oMaterial) {
      LOG_AND_THROW("There is no material at layerIndex " << layerIndex << " in " 
          << briefDescription() << ".");
    }
    return *oMaterial;
  }

  bool LayeredConstruction_Impl::eraseLayer(unsigned layerIndex) {
    
    layerIndex = mf_clearNullLayers(layerIndex);

    MaterialVector layers = this->layers();
    if (!(layerIndex < layers.size())) { return false; }
    auto layersBegin = layers.begin();
    auto toEraseIt = layersBegin;
    while (static_cast<unsigned>(toEraseIt - layersBegin) < layerIndex) { ++toEraseIt; }
    layers.erase(toEraseIt);

    if ((model().strictnessLevel() < StrictnessLevel::Final) || 
        LayeredConstruction::layersAreValid(layers)) {
      return !eraseExtensibleGroup(layerIndex).empty();
    }

    return false;
  }

  bool LayeredConstruction_Impl::insertLayer(unsigned layerIndex, 
                                             const Material& material) 
  {
    OS_ASSERT(material.model() == model());
    layerIndex = mf_clearNullLayers(layerIndex);

    unsigned n = numLayers();
    MaterialVector layers = this->layers();
    auto layersBegin = layers.begin();
    auto layersEnd = layers.end();
    auto insertAtIt = layersBegin;
    while ((static_cast<unsigned>(insertAtIt - layersBegin) < layerIndex) &&
           (insertAtIt != layersEnd)) 
    { ++insertAtIt; }
    layers.insert(insertAtIt, material);
    OS_ASSERT(layers.size() == ++n);
    if ((model().strictnessLevel() < StrictnessLevel::Final) || 
        LayeredConstruction::layersAreValid(layers)) 
    {
      IdfExtensibleGroup idfGroup = insertExtensibleGroup(layerIndex,StringVector());
      OS_ASSERT(!idfGroup.empty());
      ModelExtensibleGroup group = idfGroup.cast<ModelExtensibleGroup>();
      bool ok = group.setPointer(0,material.handle());
      OS_ASSERT(ok);
      return true;
    }

    return false;
  }

  bool LayeredConstruction_Impl::setLayer(unsigned layerIndex, 
                                          const Material& material)
  {
    OS_ASSERT(material.model() == model());
    layerIndex = mf_clearNullLayers(layerIndex);
    if (layerIndex >= numLayers()) {
      LOG(Info,"Asked to change the Material at layer " << layerIndex << " in "
          << briefDescription() << ", but there are only " << numLayers() << " layers.");
      return false;
    }

    MaterialVector layers = this->layers();
    layers[layerIndex] = material;
    if ((model().strictnessLevel() < StrictnessLevel::Final) || 
        LayeredConstruction::layersAreValid(layers)) {
      ModelExtensibleGroup group = getExtensibleGroup(layerIndex).cast<ModelExtensibleGroup>();
      OS_ASSERT(!group.empty());
      bool ok = group.setPointer(0,material.handle());
      OS_ASSERT(ok);
      return true;
    }

    return false;
  }

  bool LayeredConstruction_Impl::setLayers(const std::vector<Material>& materials) {
    
    if ((model().strictnessLevel() < StrictnessLevel::Final) || 
        LayeredConstruction::layersAreValid(materials)) 
    {
      clearExtensibleGroups();
      for (const Material& material : materials) {
        OS_ASSERT(material.model() == model());
        ModelExtensibleGroup group = pushExtensibleGroup(StringVector()).cast<ModelExtensibleGroup>();
        OS_ASSERT(!group.empty());
        bool ok = group.setPointer(0,material.handle());
        OS_ASSERT(ok);
      }
      return true;
    }

    return false;
  }

  bool LayeredConstruction_Impl::setLayer(const ModelPartitionMaterial& modelPartitionMaterial) {

    OS_ASSERT(modelPartitionMaterial.model() == model());
    clearExtensibleGroups();
    ModelExtensibleGroup group = pushExtensibleGroup(StringVector()).cast<ModelExtensibleGroup>();
    OS_ASSERT(!group.empty());
    bool ok = group.setPointer(0,modelPartitionMaterial.handle());
    OS_ASSERT(ok);
    return true;
  }

  bool LayeredConstruction_Impl::setUFactor(double value) {
    if (isFenestration()) {
      FenestrationMaterialVector fenestrationLayers = castVector<FenestrationMaterial>(layers());
      // can only set if one layer == SimpleGlazing
      if ((fenestrationLayers.size() == 1) && 
          (fenestrationLayers[0].optionalCast<SimpleGlazing>()))
      {
        SimpleGlazing simpleGlazing = fenestrationLayers[0].cast<SimpleGlazing>();
        return simpleGlazing.setUFactor(value);
      }
    }
    return false;
  }

  bool LayeredConstruction_Impl::setUFactor(double value, double filmResistance) {
    if (isOpaque()) {
      // convert to conductance
      double thermalResistance = 1.0/value - filmResistance;
      return setThermalConductance(1.0/thermalResistance);
    }
    if (isFenestration()) {
      FenestrationMaterialVector fenestrationLayers = castVector<FenestrationMaterial>(layers());
      if (fenestrationLayers.size() == 1) {
        if (fenestrationLayers[0].optionalCast<SimpleGlazing>()) { return setUFactor(value); }
        if (fenestrationLayers[0].optionalCast<StandardGlazing>() || 
            fenestrationLayers[0].optionalCast<RefractionExtinctionGlazing>()) 
        {
          // convert to conductance
          double thermalResistance = 1.0/value - filmResistance;
          return setThermalConductance(1.0/thermalResistance);
        }
      }
    }
    return false;
  }

  bool LayeredConstruction_Impl::setThermalConductance(double value) {
    if (isOpaque() && !isGreenRoof()) {
      LOG(Trace,"Attempting to set the thermal conductance of an opaque construction to " << value << ".")
      OpaqueMaterialVector opaqueLayers = castVector<OpaqueMaterial>(layers());
      // can set if one layer ...
      if (opaqueLayers.size() == 1) {
        LOG(Trace,"Single-layer construction, setting conductance of " << std::endl << opaqueLayers[0] << ".");
        return opaqueLayers[0].setThermalConductance(value);
      }      
      // ... or if perturbable construction is called out
      StandardsInformationConstruction stdsInfo = standardsInformation();
      OptionalMaterial oMaterial = stdsInfo.perturbableLayer();
      if (oMaterial && thermalConductance()) {
        OpaqueMaterial perturbableLayer = oMaterial->cast<OpaqueMaterial>();
        // calculate required resistance of perturbableLayer
        double desiredResistance = 1.0/value;
        double currentResistance = 1.0/thermalConductance().get();
        double layerResistance = perturbableLayer.thermalResistance();
        //                              desired           - contribution from other layers
        double desiredLayerResistance = desiredResistance - (currentResistance - layerResistance);
        LOG(Trace,"Attempting to set thermal conductance of " << std::endl << perturbableLayer << " to "
            << desiredLayerResistance << ".");
        return perturbableLayer.setThermalResistance(desiredLayerResistance);
      }
    }
    if (isFenestration()) {
      FenestrationMaterialVector fenestrationLayers = castVector<FenestrationMaterial>(layers());
      // can only set if one layer and StandardGlazing or RefractionExtinctionGlazing
      if (fenestrationLayers.size() == 1) {
        OptionalStandardGlazing oStandardGlazing = fenestrationLayers[0].optionalCast<StandardGlazing>();
        if (oStandardGlazing) {
          return oStandardGlazing->setThermalConductance(value);
        }
        OptionalRefractionExtinctionGlazing oRefractionExtinctionGlazing = fenestrationLayers[0].optionalCast<RefractionExtinctionGlazing>();
        if (oRefractionExtinctionGlazing) {
          return oRefractionExtinctionGlazing->setThermalConductance(value);
        }
      }
    }
    return false;
  }

  bool LayeredConstruction_Impl::setThermalConductance(double value, double filmResistance) {
    if (isOpaque()) {
      return setThermalConductance(value);
    }
    if (isFenestration()) {
      FenestrationMaterialVector fenestrationLayers = castVector<FenestrationMaterial>(layers());
      if (fenestrationLayers.size() == 1) {
        if (fenestrationLayers[0].optionalCast<StandardGlazing>() || 
            fenestrationLayers[0].optionalCast<RefractionExtinctionGlazing>()) 
        {
          return setThermalConductance(value);
        }
        if (fenestrationLayers[0].optionalCast<SimpleGlazing>()) { 
          // convert to u-factor
          double thermalResistance = 1.0/value + filmResistance;
          return setUFactor(1.0/thermalResistance);
        }
      }
    }
    return false;
  }

  bool LayeredConstruction_Impl::setConductance(double value) {
    return setThermalConductance(value);
  }

  bool LayeredConstruction_Impl::setConductance(double value,double filmResistance) {
    return setThermalConductance(value,filmResistance);
  }

  bool LayeredConstruction_Impl::isOpaque() const {
    MaterialVector layers = this->layers();
    for (const Material& layer : layers) {
      if (!layer.optionalCast<OpaqueMaterial>()) { return false; }
    }
    return true;
  }

  bool LayeredConstruction_Impl::isFenestration() const {
    MaterialVector layers = this->layers();
    for (const Material& layer : layers) {
      if (!layer.optionalCast<FenestrationMaterial>()) { return false; }
    }
    return true;
  }

  bool LayeredConstruction_Impl::isSolarDiffusing() const {

    // empty material is not solar diffusing
    MaterialVector layers = this->layers();
    if (layers.empty()){
      return false;
    }

    // if not fenestration then this is not solar diffusing
    if (!this->isFenestration()){
      return false;
    }

    // return true if any layer is solar diffusing
    for (const Material& layer : layers) {
      if (layer.optionalCast<StandardGlazing>()) { 
        if (layer.cast<StandardGlazing>().solarDiffusing()){
          return true;
        }
      }else if (layer.optionalCast<RefractionExtinctionGlazing>()){
        if (layer.cast<RefractionExtinctionGlazing>().solarDiffusing()){
          return true;
        }
      }else if (layer.optionalCast<SimpleGlazing>()) {
        // DLM: we do not know if this is solar diffusing or not
        // would need to add a field to OS:WindowMaterial:SimpleGlazingSystem
      }
    }

    // otherwise false
    return false;
  }

  bool LayeredConstruction_Impl::isModelPartition() const {
    MaterialVector layers = this->layers();
    for (const Material& layer : layers) {
      if (!layer.optionalCast<ModelPartitionMaterial>()) { return false; }
    }
    return true;
  }

  bool LayeredConstruction_Impl::isGreenRoof() const {
    MaterialVector layers = this->layers();
    if ((layers.size() > 0) && (layers[0].optionalCast<RoofVegetation>())) {
      return true;
    }
    return false;
  }

  bool LayeredConstruction_Impl::isSymmetric() const {
    MaterialVector layers = this->layers();
    MaterialVector reverseLayers(layers);
    std::reverse(reverseLayers.begin(), reverseLayers.end());
    unsigned N = layers.size();
    for (unsigned i = 0; i < N; ++i){
      if (layers[i].handle() != reverseLayers[i].handle()){
        return false;
      }
    }
    return true;
  }

  bool LayeredConstruction_Impl::equalLayers(const LayeredConstruction& other) const
  {
    MaterialVector layers = this->layers();
    MaterialVector otherLayers(other.layers());

    if (layers.size() != otherLayers.size()){
      return false;
    }

    for (unsigned i = 0; i < layers.size(); ++i){
      if (layers[i].handle() != otherLayers[i].handle()){
        return false;
      }
    }

    return true;
  }

  bool LayeredConstruction_Impl::reverseEqualLayers(const LayeredConstruction& other) const
  {
    MaterialVector layers = this->layers();
    MaterialVector reverseLayers(other.layers());

    if (layers.size() != reverseLayers.size()){
      return false;
    }

    std::reverse(reverseLayers.begin(), reverseLayers.end());

    for (unsigned i = 0; i < layers.size(); ++i){
      if (layers[i].handle() != reverseLayers[i].handle()){
        return false;
      }
    }

    return true;
  }

  std::vector<unsigned> LayeredConstruction_Impl::nullLayers() const {
    UnsignedVector result;

    // loop through extensible groups
    for (const IdfExtensibleGroup& idfGroup : extensibleGroups()) {
      ModelExtensibleGroup group = idfGroup.cast<ModelExtensibleGroup>();
      OptionalMaterial oMaterial = group.getModelObjectTarget<Material>(0);
      if (!oMaterial) {
        result.push_back(group.groupIndex()); // non-Material object referenced
      }
    }

    return result;
  }

  unsigned LayeredConstruction_Impl::numLayers() const {
    return this->layers().size();
  }

  std::vector<unsigned> LayeredConstruction_Impl::getLayerIndices(const Material& material) const 
  {
    UnsignedVector result;
    for (const ModelExtensibleGroup& group :
                  castVector<ModelExtensibleGroup>(extensibleGroups())) 
    {
      OptionalMaterial oMaterial = group.getModelObjectTarget<Material>(0);
      if (oMaterial && (material == *oMaterial)) {
        result.push_back(group.groupIndex());
      }
    }
    return result;
  }

  boost::optional<double> LayeredConstruction_Impl::uFactor() const {
    if (isFenestration()) {
      FenestrationMaterialVector fenestrationLayers = castVector<FenestrationMaterial>(layers());
      if (fenestrationLayers.size() == 1) {
        OptionalSimpleGlazing oSimpleGlazing = fenestrationLayers[0].optionalCast<SimpleGlazing>();
        if (oSimpleGlazing) { 
          return oSimpleGlazing->uFactor(); 
        }
      }
    }
    return boost::none;
  }

  boost::optional<double> LayeredConstruction_Impl::uFactor(double filmResistance) const {
    OptionalDouble result = uFactor();
    if (result) { return result; }
    result = thermalConductance();
    if (result) {
      result = 1.0/(1.0/(*result) + filmResistance);
    }
    return result;
  }


  boost::optional<double> LayeredConstruction_Impl::thermalConductance() const {
    if (isOpaque()) {
      OpaqueMaterialVector opaqueLayers = castVector<OpaqueMaterial>(layers());
      double thermalResistance(0.0);
      for (const OpaqueMaterial layer : opaqueLayers) {
        thermalResistance += layer.thermalResistance();
      }
      return 1.0/thermalResistance;
    }
    return boost::none;
  }

  boost::optional<double> LayeredConstruction_Impl::thermalConductance(double filmResistance) const 
  {
    OptionalDouble result = thermalConductance();
    if (result) { return result; }
    result = uFactor();
    if (result) {
      result = 1.0/(1.0/(*result) - filmResistance);
    }
    return result;
  }

  boost::optional<double> LayeredConstruction_Impl::heatCapacity() const {
    if (isOpaque()) {
      OpaqueMaterialVector opaqueLayers = castVector<OpaqueMaterial>(layers());
      double heatCapacity(0.0);
      for (const OpaqueMaterial layer : opaqueLayers) {
        OptionalDouble temp = layer.heatCapacity();
        if (temp) { heatCapacity += *temp; }
      }
      return heatCapacity;
    }
    return boost::none;
  }

  boost::optional<double> LayeredConstruction_Impl::interiorVisibleAbsorptance() const {
    MaterialVector layers = this->layers();
    if (layers.size() > 0) {
      return layers.back().interiorVisibleAbsorptance();
    }
    return boost::none;
  }

  boost::optional<double> LayeredConstruction_Impl::exteriorVisibleAbsorptance() const {
    MaterialVector layers = this->layers();
    if (layers.size() > 0) {
      return layers.front().exteriorVisibleAbsorptance();
    }
    return boost::none;
  }

  boost::optional<double> LayeredConstruction_Impl::visibleTransmittance() const {
    if (isOpaque()) { return 0.0; }
    MaterialVector layers = this->layers();
    OptionalDouble result;
    if (layers.size() == 1) { 
      try {
        result = layers[0].getVisibleTransmittance();
      }
      catch (...) {}
    }
    return result;
  }

  ValidityReport LayeredConstruction_Impl::validityReport(StrictnessLevel level,
                                                          bool checkNames) const 
  {
    ValidityReport report(level,getObject<model::LayeredConstruction>());
    populateValidityReport(report,checkNames);
    return report;
  }

  boost::optional<OpaqueMaterial> LayeredConstruction_Impl::insulation() const {
    StandardsInformationConstruction stdsInfo = standardsInformation();
    boost::optional<std::string> perturbableLayerType = stdsInfo.perturbableLayerType();
    if (perturbableLayerType){
      if (istringEqual(*perturbableLayerType,"Insulation")) {
        OptionalMaterial result = stdsInfo.perturbableLayer();
        if (result) {
          return result->optionalCast<OpaqueMaterial>();
        }
      }
    }
    return boost::none;
  }

  bool LayeredConstruction_Impl::setInsulation(const OpaqueMaterial& insulationLayer) {
    UnsignedVector indices = getLayerIndices(insulationLayer);
    if (indices.size() == 1) {
      StandardsInformationConstruction stdsInfo = standardsInformation();
      stdsInfo.setPerturbableLayer(indices[0]);
      stdsInfo.setPerturbableLayerType("Insulation");
      return true;
    }
    return false;
  }

  void LayeredConstruction_Impl::resetInsulation() {
    StandardsInformationConstruction stdsInfo = standardsInformation();
    boost::optional<std::string> perturbableLayerType = stdsInfo.perturbableLayerType();
    if (perturbableLayerType){
      if (istringEqual(*perturbableLayerType,"Insulation")) {
        stdsInfo.resetPerturbableLayer();
      }
    }
  }

  void LayeredConstruction_Impl::ensureUniqueLayers()
  {
    // loop through extensible groups
    for (const IdfExtensibleGroup& idfGroup : extensibleGroups()) {
      ModelExtensibleGroup group = idfGroup.cast<ModelExtensibleGroup>();
      // get object pointed to by extensible group
      // unit test checks that both layered constructions have extensible groups of size 1
      // implementation must change if that test starts failing
      OptionalMaterial oMaterial = group.getModelObjectTarget<Material>(0);
      if (!oMaterial) {
        LOG(Warn,"Skipping layer " << group.groupIndex() << " in " << briefDescription()
            << ", as there is no Material object referenced by the corresponding field.");
        continue;
      }
      unsigned numSources = oMaterial->getModelObjectSources<LayeredConstruction>().size();
      if (numSources > 1){
        ModelObject newMaterial = oMaterial->clone();
        bool test = group.setPointer(0, newMaterial.handle());
        OS_ASSERT(test);
      } 
    }
  }

  void LayeredConstruction_Impl::populateValidityReport(ValidityReport& report,bool checkNames) const
  {
    // Inherit lower-level errors
    ModelObject_Impl::populateValidityReport(report,checkNames);

    if (report.level() > StrictnessLevel::None) {
      // construction should be of one type
      if (!((numLayers() == 0) || isOpaque() || isFenestration() || isModelPartition())) {
        report.insertError(DataError(getObject<LayeredConstruction>(),DataErrorType::DataType));
      }
    }

    if (report.level() == StrictnessLevel::Final) {
      // opaque and fenestration layers should be valid, taken as a whole
      if (isOpaque() && !LayeredConstruction::layersAreValid(castVector<OpaqueMaterial>(layers()))) {
        report.insertError(DataError(getObject<LayeredConstruction>(),DataErrorType::DataType));
      }
      if (isFenestration() && !LayeredConstruction::layersAreValid(castVector<FenestrationMaterial>(layers()))) {
        report.insertError(DataError(getObject<LayeredConstruction>(),DataErrorType::DataType));
      }
      // there should be no nullLayers()
      for (unsigned layerIndex : nullLayers()) {
        report.insertError(DataError(iddObject().index(ExtensibleIndex(layerIndex,0)),
                                     getObject<LayeredConstruction>(),
                                     DataErrorType::NullAndRequired));
      }
    }
  }

  unsigned LayeredConstruction_Impl::mf_clearNullLayers(unsigned layerIndex) {
    UnsignedVector indices = nullLayers();
    for (int i = indices.size()-1; i >= 0; --i) {
      unsigned index = indices[i];
      StringVector eraseResult = eraseExtensibleGroup(index);
      OS_ASSERT(!eraseResult.empty());
      if (layerIndex > index) { --layerIndex; }
    }
    return layerIndex;
  }

  boost::optional<ModelObject> LayeredConstruction_Impl::insulationAsModelObject() const {
    OptionalModelObject result;
    OptionalOpaqueMaterial intermediate = insulation();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool LayeredConstruction_Impl::setInsulationAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalOpaqueMaterial intermediate = modelObject->optionalCast<OpaqueMaterial>();
      if (intermediate) {
        return setInsulation(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetInsulation();
    }
    return true;
  }

} // detail

LayeredConstruction::LayeredConstruction(IddObjectType type,const Model& model)
  : ConstructionBase(type,model) 
{
  OS_ASSERT(getImpl<detail::LayeredConstruction_Impl>());
}

LayeredConstruction::LayeredConstruction(std::shared_ptr<detail::LayeredConstruction_Impl> impl)
  : ConstructionBase(impl)
{}

std::vector<Material> LayeredConstruction::layers() const {
  return getImpl<detail::LayeredConstruction_Impl>()->layers();
}

Material LayeredConstruction::getLayer(unsigned layerIndex) const {
  return getImpl<detail::LayeredConstruction_Impl>()->getLayer(layerIndex);
}

bool LayeredConstruction::eraseLayer(unsigned layerIndex) {
  return getImpl<detail::LayeredConstruction_Impl>()->eraseLayer(layerIndex);
}

bool LayeredConstruction::insertLayer(unsigned layerIndex, const Material& material) {
  return getImpl<detail::LayeredConstruction_Impl>()->insertLayer(layerIndex,material);
}

bool LayeredConstruction::setLayer(unsigned layerIndex, const Material& material) {
  return getImpl<detail::LayeredConstruction_Impl>()->setLayer(layerIndex,material);
}

bool LayeredConstruction::setLayers(const std::vector<Material>& materials) {
  return getImpl<detail::LayeredConstruction_Impl>()->setLayers(materials);
}

bool LayeredConstruction::setLayer(const ModelPartitionMaterial& modelPartitionMaterial) {
  return getImpl<detail::LayeredConstruction_Impl>()->setLayer(modelPartitionMaterial);
}

bool LayeredConstruction::isSymmetric() const{
  return getImpl<detail::LayeredConstruction_Impl>()->isSymmetric();
}

bool LayeredConstruction::equalLayers(const LayeredConstruction& other) const{
  return getImpl<detail::LayeredConstruction_Impl>()->equalLayers(other);
}

bool LayeredConstruction::reverseEqualLayers(const LayeredConstruction& other) const{
  return getImpl<detail::LayeredConstruction_Impl>()->reverseEqualLayers(other);
}

std::vector<unsigned> LayeredConstruction::nullLayers() const {
  return getImpl<detail::LayeredConstruction_Impl>()->nullLayers();
}

unsigned LayeredConstruction::numLayers() const {
  return getImpl<detail::LayeredConstruction_Impl>()->numLayers();
}

std::vector<unsigned> LayeredConstruction::getLayerIndices(const Material& material) const {
  return getImpl<detail::LayeredConstruction_Impl>()->getLayerIndices(material);
}

boost::optional<OpaqueMaterial> LayeredConstruction::insulation() const {
  return getImpl<detail::LayeredConstruction_Impl>()->insulation();
}

bool LayeredConstruction::setInsulation(const OpaqueMaterial& insulationLayer) {
  return getImpl<detail::LayeredConstruction_Impl>()->setInsulation(insulationLayer);
}

void LayeredConstruction::resetInsulation() {
  getImpl<detail::LayeredConstruction_Impl>()->resetInsulation();
}

void LayeredConstruction::ensureUniqueLayers() {
  getImpl<detail::LayeredConstruction_Impl>()->ensureUniqueLayers();
}

bool LayeredConstruction::layersAreValid(const std::vector<Material>& materials)
{
  if (materials.empty()){
    return false;
  }

  if (materials[0].optionalCast<OpaqueMaterial>()){
    std::vector<OpaqueMaterial> opaqueMaterials;
    for (const Material& material : materials){
      if (!material.optionalCast<OpaqueMaterial>()){
        LOG(Info,"Not all materials can be converted to OpaqueMaterial.");
        return false;
      }
      opaqueMaterials.push_back(material.cast<OpaqueMaterial>());
    }
    return layersAreValid(opaqueMaterials);
  }

  if (materials[0].optionalCast<FenestrationMaterial>()){
    std::vector<FenestrationMaterial> fenestrationMaterials;
    for (const Material& material : materials){
      if (!material.optionalCast<FenestrationMaterial>()){
        LOG(Info,"Not all materials can be converted to FenestrationMaterial.");
        return false;
      }
      fenestrationMaterials.push_back(material.cast<FenestrationMaterial>());
    }
    return layersAreValid(fenestrationMaterials);
  }

  if (materials[0].optionalCast<ModelPartitionMaterial>()){
    if (materials.size() == 1){
      return true;
    }else{
      LOG(Info,"More than one ModelPartitionMaterial are not allowed in a layered construction.");
      return false;
    }
  }

  return false;
}

bool LayeredConstruction::layersAreValid(const std::vector<OpaqueMaterial>& opaqueMaterials)
{
  // Rule 1: AirGap must have non-AirGap on either side.
  // Rule 2: RoofVegetation must be on exterior layer.

  bool previousWasNonAirGap = false;
  for (unsigned i = 0, n = opaqueMaterials.size(); i < n; ++i) {
    // Rule 1
    if (opaqueMaterials[i].optionalCast<AirGap>()) {
      if (!previousWasNonAirGap) {
        LOG(Info,"Proposed OpaqueMaterials are invalid because an AirGap at layer " << i
            << " either starts the construction, or is preceded by " << "another AirGap.");
        return false;
      }
      previousWasNonAirGap = false;
    }
    else { previousWasNonAirGap = true; }
    // Rule 2
    if (opaqueMaterials[i].optionalCast<RoofVegetation>() && (i > 0)) {
      LOG(Info,"Proposed OpaqueMaterials are invalid because RoofVegetation must be at layer 0, "
          << "not layer " << i << ".");
      return false;
    }
  }

  // Rule 1 -- Cannot end on an AirGap.
  if ((opaqueMaterials.size() > 0) && (!previousWasNonAirGap)) {
    LOG(Info,"Proposed OpaqueMaterials are invalid because an AirGap at layer "
        << opaqueMaterials.size() << " is the last layer in the construction.");
    return false;
  }

  return true;
}

bool LayeredConstruction::layersAreValid(const std::vector<FenestrationMaterial>& fenestrationMaterials)
{
  // Rule 1 -- SimpleGlazing cannot be combined with other Glazings or any GasLayers.
  // Rule 2 -- GasLayers must have non-GasLayer on either side.
  // Rule 3 -- Ultimately, GasLayers must have Glazing on either side.
  bool hasSimpleGlazing = false;
  bool hasGlazing = false;
  bool hasGasLayer = false;
  bool previousWasNonGasLayer = false;
  bool gasLayerEnclosed = true;
  for (const auto & fenestrationMaterial : fenestrationMaterials) {
    bool isGlazing = fenestrationMaterial.optionalCast<Glazing>();
    bool isSimpleGlazing = fenestrationMaterial.optionalCast<SimpleGlazing>();
    bool isGasLayer = fenestrationMaterial.optionalCast<GasLayer>();
    // Rule 1
    if (isSimpleGlazing) {
      if (hasGlazing) { return false; }
      if (hasGasLayer) { return false; }
      hasSimpleGlazing = true;
      hasGlazing = true;
    }
    if (isGlazing && !isSimpleGlazing && hasSimpleGlazing) { return false; }
    if (isGasLayer && hasSimpleGlazing) { return false; }
    // Rules 2 and 3
    if (isGasLayer) {
      if (!previousWasNonGasLayer) { return false; }
      if (!hasGlazing) { return false; }
      hasGasLayer = true;
      previousWasNonGasLayer = false;
      gasLayerEnclosed = false;
    }
    else {
      previousWasNonGasLayer = true;
    }
    // Final Updates of State
    if (isGlazing) {
      hasGlazing = true;
      gasLayerEnclosed = true;
    }
  }

  // Rule 3
  if ((fenestrationMaterials.size() > 0) && (!previousWasNonGasLayer)) {
    return false;
  }
  if (!gasLayerEnclosed) { return false; }

  return true;
}

} // model
} // openstudio
