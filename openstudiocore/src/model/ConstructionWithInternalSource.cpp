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

#include "ConstructionWithInternalSource.hpp"
#include "ConstructionWithInternalSource_Impl.hpp"
#include "Model.hpp"

#include "Material.hpp"
#include "Material_Impl.hpp"
#include "OpaqueMaterial.hpp"
#include "ModelExtensibleGroup.hpp"

#include "../utilities/core/Assert.hpp"
#include <utilities/idd/OS_Construction_InternalSource_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

namespace detail {

  ConstructionWithInternalSource_Impl::ConstructionWithInternalSource_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : LayeredConstruction_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ConstructionWithInternalSource::iddObjectType());
  }

  ConstructionWithInternalSource_Impl::ConstructionWithInternalSource_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : LayeredConstruction_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ConstructionWithInternalSource::iddObjectType());
  }

  ConstructionWithInternalSource_Impl::ConstructionWithInternalSource_Impl(
      const ConstructionWithInternalSource_Impl& other,Model_Impl* model,bool keepHandle)
    : LayeredConstruction_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ConstructionWithInternalSource_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ConstructionWithInternalSource_Impl::iddObjectType() const {
    return ConstructionWithInternalSource::iddObjectType();
  }

  int ConstructionWithInternalSource_Impl::renderingColorIndex() const
  {
    return OS_Construction_InternalSourceFields::SurfaceRenderingName;
  }

  bool ConstructionWithInternalSource_Impl::eraseLayer(unsigned layerIndex)
  {
    if (this->numLayers() < 3){
      return false;
    }
    bool result = LayeredConstruction_Impl::eraseLayer(layerIndex);
    if (result){
      onNumLayersChanged();
    }
    return result;
  }

  bool ConstructionWithInternalSource_Impl::setLayers(const std::vector<Material>& materials)
  {
    if (materials.size() < 2){
      return false;
    }
    bool result = LayeredConstruction_Impl::setLayers(materials);
    if (result){
      onNumLayersChanged();
    }
    return result;
  }

  bool ConstructionWithInternalSource_Impl::setLayer(const ModelPartitionMaterial& modelPartitionMaterial)
  {
    return false;
  }

  int ConstructionWithInternalSource_Impl::sourcePresentAfterLayerNumber() const
  {
    boost::optional<int> value = getInt(OS_Construction_InternalSourceFields::SourcePresentAfterLayerNumber,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ConstructionWithInternalSource_Impl::setSourcePresentAfterLayerNumber(int sourcePresentAfterLayerNumber) {
    if (sourcePresentAfterLayerNumber < 1 || sourcePresentAfterLayerNumber > (int)this->numLayers()){
      return false;
    }
    bool result = setInt(OS_Construction_InternalSourceFields::SourcePresentAfterLayerNumber,sourcePresentAfterLayerNumber);
    return result;
  }

  int ConstructionWithInternalSource_Impl::temperatureCalculationRequestedAfterLayerNumber() const
  {
    boost::optional<int> value = getInt(OS_Construction_InternalSourceFields::TemperatureCalculationRequestedAfterLayerNumber,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ConstructionWithInternalSource_Impl::setTemperatureCalculationRequestedAfterLayerNumber(int temperatureCalculationRequestedAfterLayerNumber) {
    if (temperatureCalculationRequestedAfterLayerNumber < 1 || temperatureCalculationRequestedAfterLayerNumber > (int)this->numLayers()){
      return false;
    }
    bool result = setInt(OS_Construction_InternalSourceFields::TemperatureCalculationRequestedAfterLayerNumber,temperatureCalculationRequestedAfterLayerNumber);
    return result;
  }

  int ConstructionWithInternalSource_Impl::dimensionsForTheCTFCalculation() const
  {
    boost::optional<int> value = getInt(OS_Construction_InternalSourceFields::DimensionsfortheCTFCalculation,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ConstructionWithInternalSource_Impl::setDimensionsForTheCTFCalculation(int dimensionsForTheCTFCalculation) {
    if (dimensionsForTheCTFCalculation < 1 || dimensionsForTheCTFCalculation > 2){
      return false;
    }
    bool result = setInt(OS_Construction_InternalSourceFields::DimensionsfortheCTFCalculation,dimensionsForTheCTFCalculation);
    return result;
  }

  double ConstructionWithInternalSource_Impl::tubeSpacing() const
  {
    boost::optional<double> value = getDouble(OS_Construction_InternalSourceFields::TubeSpacing,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity ConstructionWithInternalSource_Impl::getTubeSpacing(bool returnIP) const {
    OSOptionalQuantity value = getQuantity(OS_Construction_InternalSourceFields::TubeSpacing,true,returnIP);
    OS_ASSERT(value.isSet());
    return value.get();
  }

  openstudio::Quantity ConstructionWithInternalSource_Impl::tubeSpacing_SI() const {
    return getTubeSpacing(false);
  }

  openstudio::Quantity ConstructionWithInternalSource_Impl::tubeSpacing_IP() const {
    return getTubeSpacing(true);
  }

  bool ConstructionWithInternalSource_Impl::setTubeSpacing(double tubeSpacing) {
    bool result = setDouble(OS_Construction_InternalSourceFields::TubeSpacing,tubeSpacing);
    return result;
  }

  bool ConstructionWithInternalSource_Impl::setTubeSpacing(const Quantity& tubeSpacing) {
    OptionalDouble value = getDoubleFromQuantity(OS_Construction_InternalSourceFields::TubeSpacing,tubeSpacing);
    if (!value) {
      return false;
    }
    return setTubeSpacing(value.get());
  }

  ConstructionWithInternalSource ConstructionWithInternalSource_Impl::reverseConstructionWithInternalSource() const
  {
    MaterialVector reverseLayers(this->layers());
    std::reverse(reverseLayers.begin(), reverseLayers.end());

    int numLayers = (int)this->numLayers();
    int reverseSourcePresentAfterLayerNumber = numLayers - this->sourcePresentAfterLayerNumber();
    int reverseTemperatureCalculationRequestedAfterLayerNumber = numLayers - this->temperatureCalculationRequestedAfterLayerNumber();
    int dimensionsForTheCTFCalculation = this->dimensionsForTheCTFCalculation();
    double tubeSpacing = this->tubeSpacing();

    Model model = this->model();
    for (const ConstructionWithInternalSource& other : model.getConcreteModelObjects<ConstructionWithInternalSource>()) {

      if (other.sourcePresentAfterLayerNumber() != reverseSourcePresentAfterLayerNumber){
        continue;
      }

      if (other.temperatureCalculationRequestedAfterLayerNumber() != reverseTemperatureCalculationRequestedAfterLayerNumber){
        continue;
      }

      if (other.dimensionsForTheCTFCalculation() != dimensionsForTheCTFCalculation){
        continue;
      }

      if (other.tubeSpacing() != tubeSpacing){
        continue;
      }

      MaterialVector layers = other.layers();
      if (layers.size() != reverseLayers.size()){
        continue;
      }

      bool test = true;
      for (unsigned i = 0; i < layers.size(); ++i){
        if (layers[i].handle() != reverseLayers[i].handle()){
          test = false;
          break; // break out of loop over layers
        }
      }

      if (test){
        return other;
      }
    }

    // TODO: this should also copy (and modify) standards information object

    // no match, make one
    ConstructionWithInternalSource result(model);
    result.setName(this->name().get() + " Reversed");
    result.setSourcePresentAfterLayerNumber(reverseSourcePresentAfterLayerNumber);
    result.setTemperatureCalculationRequestedAfterLayerNumber(reverseTemperatureCalculationRequestedAfterLayerNumber);
    result.setDimensionsForTheCTFCalculation(dimensionsForTheCTFCalculation);
    result.setTubeSpacing(tubeSpacing);
    result.setLayers(reverseLayers);

    return result;
  }

  void ConstructionWithInternalSource_Impl::onNumLayersChanged()
  {
    int numLayers = (int)this->numLayers();
    if (this->sourcePresentAfterLayerNumber() > numLayers-1){
      bool test = setString(OS_Construction_InternalSourceFields::SourcePresentAfterLayerNumber, "");
      OS_ASSERT(test);
    }
    if (this->temperatureCalculationRequestedAfterLayerNumber() > numLayers-1){
      bool test = setString(OS_Construction_InternalSourceFields::TemperatureCalculationRequestedAfterLayerNumber, "");
      OS_ASSERT(test);
    }
  }


} // detail

ConstructionWithInternalSource::ConstructionWithInternalSource(const Model& model)
  : LayeredConstruction(ConstructionWithInternalSource::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ConstructionWithInternalSource_Impl>());
}

ConstructionWithInternalSource::ConstructionWithInternalSource(const std::vector<OpaqueMaterial>& opaqueMaterials)
  : LayeredConstruction(ConstructionWithInternalSource::iddObjectType(),
  (opaqueMaterials.empty() ? openstudio::model::Model() : opaqueMaterials.at(0).model()))
{
  if (opaqueMaterials.empty()){
    // DLM: do not remove, this was only added to a temporary model
    //this->remove();
    LOG_AND_THROW("Cannot create an internal source construction with empty layers");
  }else if (opaqueMaterials.size() < 2){
    this->remove();
    LOG_AND_THROW("Cannot create an internal source construction with fewer than 2 layers");
  } else if (opaqueMaterials.size() > 10){
    this->remove();
    LOG_AND_THROW("Cannot create an internal source construction with more than 10 layers");
  }

  std::vector<Material> materials = castVector<Material>(opaqueMaterials);
  bool ok = setLayers(materials);
  OS_ASSERT(ok);
}

IddObjectType ConstructionWithInternalSource::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Construction_InternalSource);
  return result;
}

int ConstructionWithInternalSource::sourcePresentAfterLayerNumber() const {
  return getImpl<detail::ConstructionWithInternalSource_Impl>()->sourcePresentAfterLayerNumber();
}

bool ConstructionWithInternalSource::setSourcePresentAfterLayerNumber(int sourcePresentAfterLayerNumber) {
  return getImpl<detail::ConstructionWithInternalSource_Impl>()->setSourcePresentAfterLayerNumber(sourcePresentAfterLayerNumber);
}

int ConstructionWithInternalSource::temperatureCalculationRequestedAfterLayerNumber() const {
  return getImpl<detail::ConstructionWithInternalSource_Impl>()->temperatureCalculationRequestedAfterLayerNumber();
}

bool ConstructionWithInternalSource::setTemperatureCalculationRequestedAfterLayerNumber(int temperatureCalculationRequestedAfterLayerNumber) {
  return getImpl<detail::ConstructionWithInternalSource_Impl>()->setTemperatureCalculationRequestedAfterLayerNumber(temperatureCalculationRequestedAfterLayerNumber);
}

int ConstructionWithInternalSource::dimensionsForTheCTFCalculation() const {
  return getImpl<detail::ConstructionWithInternalSource_Impl>()->dimensionsForTheCTFCalculation();
}

bool ConstructionWithInternalSource::setDimensionsForTheCTFCalculation(int dimensionsForTheCTFCalculation) {
  return getImpl<detail::ConstructionWithInternalSource_Impl>()->setDimensionsForTheCTFCalculation(dimensionsForTheCTFCalculation);
}

double ConstructionWithInternalSource::tubeSpacing() const {
  return getImpl<detail::ConstructionWithInternalSource_Impl>()->tubeSpacing();
}

bool ConstructionWithInternalSource::setTubeSpacing(double tubeSpacing) {
  return getImpl<detail::ConstructionWithInternalSource_Impl>()->setTubeSpacing(tubeSpacing);
}

ConstructionWithInternalSource ConstructionWithInternalSource::reverseConstructionWithInternalSource() const
{
  return getImpl<detail::ConstructionWithInternalSource_Impl>()->reverseConstructionWithInternalSource();
}

/// @cond
ConstructionWithInternalSource::ConstructionWithInternalSource(
    std::shared_ptr<detail::ConstructionWithInternalSource_Impl> impl)
  : LayeredConstruction(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
