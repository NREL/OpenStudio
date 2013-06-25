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

#include <model/ConstructionWithInternalSource.hpp>
#include <model/ConstructionWithInternalSource_Impl.hpp>
#include <model/Model.hpp>

#include <model/Material.hpp>
#include <model/Material_Impl.hpp>
#include <model/OpaqueMaterial.hpp>
#include <model/ModelExtensibleGroup.hpp>

#include <utilities/idd/OS_Construction_InternalSource_FieldEnums.hxx>

#include <boost/foreach.hpp>

namespace openstudio {
namespace model {

namespace detail {

  ConstructionWithInternalSource_Impl::ConstructionWithInternalSource_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : LayeredConstruction_Impl(idfObject, model, keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == ConstructionWithInternalSource::iddObjectType());
  }

  ConstructionWithInternalSource_Impl::ConstructionWithInternalSource_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : LayeredConstruction_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == ConstructionWithInternalSource::iddObjectType());
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

  int ConstructionWithInternalSource_Impl::sourcePresentAfterLayerNumber() const
  {
    boost::optional<int> value = getInt(OS_Construction_InternalSourceFields::SourcePresentAfterLayerNumber,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool ConstructionWithInternalSource_Impl::setSourcePresentAfterLayerNumber(int sourcePresentAfterLayerNumber) {
    bool result = setInt(OS_Construction_InternalSourceFields::SourcePresentAfterLayerNumber,sourcePresentAfterLayerNumber);
    return result;
  }

  int ConstructionWithInternalSource_Impl::temperatureCalculationRequestedAfterLayerNumber() const
  {
    boost::optional<int> value = getInt(OS_Construction_InternalSourceFields::TemperatureCalculationRequestedAfterLayerNumber,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool ConstructionWithInternalSource_Impl::setTemperatureCalculationRequestedAfterLayerNumber(int temperatureCalculationRequestedAfterLayerNumber) {
    bool result = setInt(OS_Construction_InternalSourceFields::TemperatureCalculationRequestedAfterLayerNumber,temperatureCalculationRequestedAfterLayerNumber);
    return result;
  }

  int ConstructionWithInternalSource_Impl::dimensionsForTheCTFCalculation() const
  {
    boost::optional<int> value = getInt(OS_Construction_InternalSourceFields::DimensionsfortheCTFCalculation,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool ConstructionWithInternalSource_Impl::setDimensionsForTheCTFCalculation(int dimensionsForTheCTFCalculation) {
    bool result = setInt(OS_Construction_InternalSourceFields::DimensionsfortheCTFCalculation,dimensionsForTheCTFCalculation);
    return result;
  }

  double ConstructionWithInternalSource_Impl::tubeSpacing() const
  {
    boost::optional<double> value = getDouble(OS_Construction_InternalSourceFields::TubeSpacing,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  Quantity ConstructionWithInternalSource_Impl::getTubeSpacing(bool returnIP) const {
    OSOptionalQuantity value = getQuantity(OS_Construction_InternalSourceFields::TubeSpacing,true,returnIP);
    BOOST_ASSERT(value.isSet());
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

    int reverseSourcePresentAfterLayerNumber = this->numLayers() - this->sourcePresentAfterLayerNumber();
    int reverseTemperatureCalculationRequestedAfterLayerNumber = this->numLayers() - this->temperatureCalculationRequestedAfterLayerNumber();
    int dimensionsForTheCTFCalculation = this->dimensionsForTheCTFCalculation();
    double tubeSpacing = this->tubeSpacing();

    Model model = this->model();
    BOOST_FOREACH(const ConstructionWithInternalSource& other, model.getModelObjects<ConstructionWithInternalSource>()) {
      
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
} // detail

ConstructionWithInternalSource::ConstructionWithInternalSource(const Model& model)
  : LayeredConstruction(ConstructionWithInternalSource::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::ConstructionWithInternalSource_Impl>());
}

ConstructionWithInternalSource::ConstructionWithInternalSource(const std::vector<OpaqueMaterial>& opaqueMaterials)
  : LayeredConstruction(ConstructionWithInternalSource::iddObjectType(),opaqueMaterials[0].model())
{
  //BOOST_ASSERT(getImpl<detail::ConstructionWithInternalSource_Impl>());
  int ok = this->numLayers();
  if(ok < 2){
    LOG_AND_THROW("Cannot create a internal source construction with fewer than 2 layers");
  }
  else{
    setSourcePresentAfterLayerNumber(this->numLayers());
    setTemperatureCalculationRequestedAfterLayerNumber(this->numLayers());
  }
  setDimensionsForTheCTFCalculation(1);
  setTubeSpacing(0.154);
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

Quantity ConstructionWithInternalSource::getTubeSpacing(bool returnIP) const {
  return getImpl<detail::ConstructionWithInternalSource_Impl>()->getTubeSpacing(returnIP);
}

bool ConstructionWithInternalSource::setTubeSpacing(double tubeSpacing) {
  return getImpl<detail::ConstructionWithInternalSource_Impl>()->setTubeSpacing(tubeSpacing);
}

bool ConstructionWithInternalSource::setTubeSpacing(const Quantity& tubeSpacing) {
  return getImpl<detail::ConstructionWithInternalSource_Impl>()->setTubeSpacing(tubeSpacing);
}

ConstructionWithInternalSource ConstructionWithInternalSource::reverseConstructionWithInternalSource() const
{
  return getImpl<detail::ConstructionWithInternalSource_Impl>()->reverseConstructionWithInternalSource();
}

/// @cond
ConstructionWithInternalSource::ConstructionWithInternalSource(
    boost::shared_ptr<detail::ConstructionWithInternalSource_Impl> impl)
  : LayeredConstruction(impl)
{}
/// @endcond

} // model
} // openstudio
