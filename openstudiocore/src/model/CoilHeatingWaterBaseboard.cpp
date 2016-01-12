/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

