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

#include "CoilHeatingWaterBaseboard.hpp"
#include "CoilHeatingWaterBaseboard_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "ZoneHVACBaseboardConvectiveWater.hpp"
#include "ZoneHVACBaseboardConvectiveWater_Impl.hpp"

#include <utilities/idd/OS_Coil_Heating_Water_Baseboard_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingWaterBaseboard_Impl::CoilHeatingWaterBaseboard_Impl(const IdfObject& idfObject,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingWaterBaseboard::iddObjectType());
  }

  CoilHeatingWaterBaseboard_Impl::CoilHeatingWaterBaseboard_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingWaterBaseboard::iddObjectType());
  }

  CoilHeatingWaterBaseboard_Impl::CoilHeatingWaterBaseboard_Impl(const CoilHeatingWaterBaseboard_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingWaterBaseboard_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingWaterBaseboard_Impl::iddObjectType() const {
    return CoilHeatingWaterBaseboard::iddObjectType();
  }
  unsigned CoilHeatingWaterBaseboard_Impl::inletPort()
{
  return OS_Coil_Heating_Water_BaseboardFields::WaterInletNodeName;
}

  unsigned CoilHeatingWaterBaseboard_Impl::outletPort()
{
  return OS_Coil_Heating_Water_BaseboardFields::WaterOutletNodeName;
}

  boost::optional<ZoneHVACComponent> CoilHeatingWaterBaseboard_Impl::containingZoneHVACComponent() const
  {
    // this coil can only be found in a ZoneHVACBaseboardConvectiveWater
    // check all ZoneHVACBaseboardConvectiveWaters in the model, seeing if this coil
    // is inside of one of them.  Return the one it is inside of

    // declare a vector to hold all of the zoneHVACBaseboardConvectiveWater
    std::vector<ZoneHVACBaseboardConvectiveWater> zoneHVACBaseboardConvectiveWaters;
    // populate the vector with all of them
    zoneHVACBaseboardConvectiveWaters = this->model().getConcreteModelObjects<ZoneHVACBaseboardConvectiveWater>();
    // loop through each one, seeing if the coil is contained by the zonehvacbaseboard
    for( const auto & zoneHVACBaseboardConvectiveWater : zoneHVACBaseboardConvectiveWaters )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACBaseboardConvectiveWater.heatingCoil() )
      {
        if( coil->handle() == this->handle() )  //if the handles match, this coil is inside of a zonehvacbaseboard
        {
          return zoneHVACBaseboardConvectiveWater;
        }
      }
    }

    // if the coil isn't inside any zonehvacbaseboards (which currently should never happen), return nothing
    return boost::none;
  }

  std::string CoilHeatingWaterBaseboard_Impl::heatingDesignCapacityMethod() const {
    boost::optional<std::string> value = getString(OS_Coil_Heating_Water_BaseboardFields::HeatingDesignCapacityMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilHeatingWaterBaseboard_Impl::heatingDesignCapacity() const {
    return getDouble(OS_Coil_Heating_Water_BaseboardFields::HeatingDesignCapacity,true);
  }

  bool CoilHeatingWaterBaseboard_Impl::isHeatingDesignCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_Water_BaseboardFields::HeatingDesignCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilHeatingWaterBaseboard_Impl::heatingDesignCapacityPerFloorArea() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_Water_BaseboardFields::HeatingDesignCapacityPerFloorArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingWaterBaseboard_Impl::fractionofAutosizedHeatingDesignCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_Water_BaseboardFields::FractionofAutosizedHeatingDesignCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilHeatingWaterBaseboard_Impl::uFactorTimesAreaValue() const {
    return getDouble(OS_Coil_Heating_Water_BaseboardFields::UFactorTimesAreaValue,true);
  }

  bool CoilHeatingWaterBaseboard_Impl::isUFactorTimesAreaValueDefaulted() const {
    return isEmpty(OS_Coil_Heating_Water_BaseboardFields::UFactorTimesAreaValue);
  }

  bool CoilHeatingWaterBaseboard_Impl::isUFactorTimesAreaValueAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_Water_BaseboardFields::UFactorTimesAreaValue, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilHeatingWaterBaseboard_Impl::maximumWaterFlowRate() const {
    return getDouble(OS_Coil_Heating_Water_BaseboardFields::MaximumWaterFlowRate,true);
  }

  bool CoilHeatingWaterBaseboard_Impl::isMaximumWaterFlowRateDefaulted() const {
    return isEmpty(OS_Coil_Heating_Water_BaseboardFields::MaximumWaterFlowRate);
  }

  bool CoilHeatingWaterBaseboard_Impl::isMaximumWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_Water_BaseboardFields::MaximumWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilHeatingWaterBaseboard_Impl::convergenceTolerance() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_Water_BaseboardFields::ConvergenceTolerance,true);
    OS_ASSERT(value);
    return value.get();
  }

   bool CoilHeatingWaterBaseboard_Impl::isConvergenceToleranceDefaulted() const {
    return isEmpty(OS_Coil_Heating_Water_BaseboardFields::ConvergenceTolerance);
  }

  bool CoilHeatingWaterBaseboard_Impl::setHeatingDesignCapacityMethod(std::string heatingDesignCapacityMethod) {
    bool result = setString(OS_Coil_Heating_Water_BaseboardFields::HeatingDesignCapacityMethod, heatingDesignCapacityMethod);
    return result;
  }

  bool CoilHeatingWaterBaseboard_Impl::setHeatingDesignCapacity(boost::optional<double> heatingDesignCapacity) {
    bool result(false);
    if (heatingDesignCapacity) {
      result = setDouble(OS_Coil_Heating_Water_BaseboardFields::HeatingDesignCapacity, heatingDesignCapacity.get());
    }
    return result;
  }

  void CoilHeatingWaterBaseboard_Impl::autosizeHeatingDesignCapacity() {
    bool result = setString(OS_Coil_Heating_Water_BaseboardFields::HeatingDesignCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingWaterBaseboard_Impl::setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea) {
    bool result = setDouble(OS_Coil_Heating_Water_BaseboardFields::HeatingDesignCapacityPerFloorArea, heatingDesignCapacityPerFloorArea);
    return result;
  }

  bool CoilHeatingWaterBaseboard_Impl::setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity) {
    bool result = setDouble(OS_Coil_Heating_Water_BaseboardFields::FractionofAutosizedHeatingDesignCapacity, fractionofAutosizedHeatingDesignCapacity);
    return result;
  }

   void CoilHeatingWaterBaseboard_Impl::setUFactorTimesAreaValue(boost::optional<double> uFactorTimesAreaValue) {
    bool result(false);
    if (uFactorTimesAreaValue) {
      result = setDouble(OS_Coil_Heating_Water_BaseboardFields::UFactorTimesAreaValue, uFactorTimesAreaValue.get());
    }
    else {
      resetUFactorTimesAreaValue();
      result = true;
    }
    OS_ASSERT(result);
  }

 void CoilHeatingWaterBaseboard_Impl::resetUFactorTimesAreaValue() {
    bool result = setString(OS_Coil_Heating_Water_BaseboardFields::UFactorTimesAreaValue, "");
    OS_ASSERT(result);
  }

  void CoilHeatingWaterBaseboard_Impl::autosizeUFactorTimesAreaValue() {
    bool result = setString(OS_Coil_Heating_Water_BaseboardFields::UFactorTimesAreaValue, "autosize");
    OS_ASSERT(result);
  }

  void CoilHeatingWaterBaseboard_Impl::setMaximumWaterFlowRate(boost::optional<double> maximumWaterFlowRate) {
    bool result(false);
    if (maximumWaterFlowRate) {
      result = setDouble(OS_Coil_Heating_Water_BaseboardFields::MaximumWaterFlowRate, maximumWaterFlowRate.get());
    }
    else {
      resetMaximumWaterFlowRate();
      result = true;
    }
    OS_ASSERT(result);
  }

 void CoilHeatingWaterBaseboard_Impl::resetMaximumWaterFlowRate() {
    bool result = setString(OS_Coil_Heating_Water_BaseboardFields::MaximumWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void CoilHeatingWaterBaseboard_Impl::autosizeMaximumWaterFlowRate() {
    bool result = setString(OS_Coil_Heating_Water_BaseboardFields::MaximumWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingWaterBaseboard_Impl::setConvergenceTolerance(double convergenceTolerance) {
    bool result = setDouble(OS_Coil_Heating_Water_BaseboardFields::ConvergenceTolerance, convergenceTolerance);
    return result;
  }

  void CoilHeatingWaterBaseboard_Impl::resetConvergenceTolerance() {
    bool result = setString(OS_Coil_Heating_Water_BaseboardFields::ConvergenceTolerance, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingWaterBaseboard_Impl::addToNode(Node & node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() )
    {
      if( plant->demandComponent(node.handle()) )
      {
        return StraightComponent_Impl::addToNode(node);
      }
    }

    return false;
  }


} // detail

CoilHeatingWaterBaseboard::CoilHeatingWaterBaseboard(const Model& model)
  : StraightComponent(CoilHeatingWaterBaseboard::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingWaterBaseboard_Impl>());

  setHeatingDesignCapacityMethod("HeatingDesignCapacity");
  autosizeHeatingDesignCapacity();
  setHeatingDesignCapacityPerFloorArea(0.0);
  setFractionofAutosizedHeatingDesignCapacity(0.8);
}
unsigned CoilHeatingWaterBaseboard::inletPort()
{
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->inletPort();
}

unsigned CoilHeatingWaterBaseboard::outletPort()
{
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->outletPort();
}

IddObjectType CoilHeatingWaterBaseboard::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_Water_Baseboard);
}

std::vector<std::string> CoilHeatingWaterBaseboard::heatingDesignCapacityMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_Heating_Water_BaseboardFields::HeatingDesignCapacityMethod);
}

std::string CoilHeatingWaterBaseboard::heatingDesignCapacityMethod() const {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->heatingDesignCapacityMethod();
}

boost::optional<double> CoilHeatingWaterBaseboard::heatingDesignCapacity() const {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->heatingDesignCapacity();
}

bool CoilHeatingWaterBaseboard::isHeatingDesignCapacityAutosized() const {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->isHeatingDesignCapacityAutosized();
}

double CoilHeatingWaterBaseboard::heatingDesignCapacityPerFloorArea() const {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->heatingDesignCapacityPerFloorArea();
}

double CoilHeatingWaterBaseboard::fractionofAutosizedHeatingDesignCapacity() const {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->fractionofAutosizedHeatingDesignCapacity();
}

boost::optional<double> CoilHeatingWaterBaseboard::uFactorTimesAreaValue() const {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->uFactorTimesAreaValue();
}

bool CoilHeatingWaterBaseboard::isUFactorTimesAreaValueDefaulted() const {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->isUFactorTimesAreaValueDefaulted();
}

bool CoilHeatingWaterBaseboard::isUFactorTimesAreaValueAutosized() const {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->isUFactorTimesAreaValueAutosized();
}

boost::optional<double> CoilHeatingWaterBaseboard::maximumWaterFlowRate() const {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->maximumWaterFlowRate();
}

bool CoilHeatingWaterBaseboard::isMaximumWaterFlowRateDefaulted() const {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->isMaximumWaterFlowRateDefaulted();
}

bool CoilHeatingWaterBaseboard::isMaximumWaterFlowRateAutosized() const {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->isMaximumWaterFlowRateAutosized();
}

double CoilHeatingWaterBaseboard::convergenceTolerance() const {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->convergenceTolerance();
}

bool CoilHeatingWaterBaseboard::isConvergenceToleranceDefaulted() const {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->isConvergenceToleranceDefaulted();
}

bool CoilHeatingWaterBaseboard::setHeatingDesignCapacityMethod(std::string heatingDesignCapacityMethod) {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->setHeatingDesignCapacityMethod(heatingDesignCapacityMethod);
}

bool CoilHeatingWaterBaseboard::setHeatingDesignCapacity(double heatingDesignCapacity) {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->setHeatingDesignCapacity(heatingDesignCapacity);
}

void CoilHeatingWaterBaseboard::autosizeHeatingDesignCapacity() {
  getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->autosizeHeatingDesignCapacity();
}

bool CoilHeatingWaterBaseboard::setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea) {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->setHeatingDesignCapacityPerFloorArea(heatingDesignCapacityPerFloorArea);
}

bool CoilHeatingWaterBaseboard::setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity) {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->setFractionofAutosizedHeatingDesignCapacity(fractionofAutosizedHeatingDesignCapacity);
}

void CoilHeatingWaterBaseboard::setUFactorTimesAreaValue(double uFactorTimesAreaValue) {
  getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->setUFactorTimesAreaValue(uFactorTimesAreaValue);
}

void CoilHeatingWaterBaseboard::resetUFactorTimesAreaValue() {
  getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->resetUFactorTimesAreaValue();
}

void CoilHeatingWaterBaseboard::autosizeUFactorTimesAreaValue() {
  getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->autosizeUFactorTimesAreaValue();
}

void CoilHeatingWaterBaseboard::setMaximumWaterFlowRate(double maximumWaterFlowRate) {
  getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->setMaximumWaterFlowRate(maximumWaterFlowRate);
}

void CoilHeatingWaterBaseboard::resetMaximumWaterFlowRate() {
  getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->resetMaximumWaterFlowRate();
}

void CoilHeatingWaterBaseboard::autosizeMaximumWaterFlowRate() {
  getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->autosizeMaximumWaterFlowRate();
}

bool CoilHeatingWaterBaseboard::setConvergenceTolerance(double convergenceTolerance) {
  return getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->setConvergenceTolerance(convergenceTolerance);
}

void CoilHeatingWaterBaseboard::resetConvergenceTolerance() {
  getImpl<detail::CoilHeatingWaterBaseboard_Impl>()->resetConvergenceTolerance();
}

CoilHeatingWaterBaseboard::CoilHeatingWaterBaseboard(std::shared_ptr<detail::CoilHeatingWaterBaseboard_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

