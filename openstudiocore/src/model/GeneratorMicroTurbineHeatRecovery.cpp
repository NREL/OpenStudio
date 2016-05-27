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

#include "GeneratorMicroTurbineHeatRecovery.hpp"
#include "GeneratorMicroTurbineHeatRecovery_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "GeneratorMicroTurbineHeatRecovery.hpp"
#include "GeneratorMicroTurbineHeatRecovery_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
// TODO: add the tables class if they get added to OS later?
//#include "DataTables.hpp"


#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_MicroTurbine_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  GeneratorMicroTurbineHeatRecovery_Impl::GeneratorMicroTurbineHeatRecovery_Impl(const IdfObject& idfObject,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : Generator_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GeneratorMicroTurbineHeatRecovery::iddObjectType());
  }

  GeneratorMicroTurbineHeatRecovery_Impl::GeneratorMicroTurbineHeatRecovery_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : Generator_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GeneratorMicroTurbineHeatRecovery::iddObjectType());
  }

  GeneratorMicroTurbineHeatRecovery_Impl::GeneratorMicroTurbineHeatRecovery_Impl(const GeneratorMicroTurbineHeatRecovery_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : Generator_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GeneratorMicroTurbineHeatRecovery_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType GeneratorMicroTurbineHeatRecovery_Impl::iddObjectType() const {
    return GeneratorMicroTurbineHeatRecovery::iddObjectType();
  }
  
  // Add an implementation of StraightComponent_Impl?
  
  // TODO: this is actually an optional... should I handle that explicitly?
  unsigned GeneratorMicroTurbineHeatRecovery_Impl::inletPort()
  {
    return OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterInletNodeName;
  }

  unsigned GeneratorMicroTurbineHeatRecovery_Impl::outletPort()
  {
    return OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterOutletNodeName;
  }

  //boost::optional<Connection> GeneratorMicroTurbineHeatRecovery_Impl::heatRecoveryWaterInletNode() const {
  //  return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterInletNodeName);
  //}

  //boost::optional<Connection> GeneratorMicroTurbineHeatRecovery_Impl::heatRecoveryWaterOutletNode() const {
  //  return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterOutletNodeName);
  //}

  double GeneratorMicroTurbineHeatRecovery_Impl::referenceThermalEfficiencyUsingLowerHeatValue() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbineHeatRecoveryFields::ReferenceThermalEfficiencyUsingLowerHeatValue,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineHeatRecoveryFields::ReferenceThermalEfficiencyUsingLowerHeatValue);
  }

  boost::optional<double> GeneratorMicroTurbineHeatRecovery_Impl::referenceInletWaterTemperature() const {
    return getDouble(OS_Generator_MicroTurbineHeatRecoveryFields::ReferenceInletWaterTemperature,true);
  }

  std::string GeneratorMicroTurbineHeatRecovery_Impl::heatRecoveryWaterFlowOperatingMode() const {
    boost::optional<std::string> value = getString(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterFlowOperatingMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::isHeatRecoveryWaterFlowOperatingModeDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterFlowOperatingMode);
  }

  boost::optional<double> GeneratorMicroTurbineHeatRecovery_Impl::referenceHeatRecoveryWaterFlowRate() const {
    return getDouble(OS_Generator_MicroTurbineHeatRecoveryFields::ReferenceHeatRecoveryWaterFlowRate,true);
  }

  boost::optional<Curve> GeneratorMicroTurbineHeatRecovery_Impl::heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbineHeatRecovery_Impl::thermalEfficiencyFunctionofTemperatureandElevationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineHeatRecoveryFields::ThermalEfficiencyFunctionofTemperatureandElevationCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbineHeatRecovery_Impl::heatRecoveryRateFunctionofPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryRateFunctionofPartLoadRatioCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbineHeatRecovery_Impl::heatRecoveryRateFunctionofInletWaterTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryRateFunctionofInletWaterTemperatureCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbineHeatRecovery_Impl::heatRecoveryRateFunctionofWaterFlowRateCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryRateFunctionofWaterFlowRateCurveName);
  }

  double GeneratorMicroTurbineHeatRecovery_Impl::minimumHeatRecoveryWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbineHeatRecoveryFields::MinimumHeatRecoveryWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::isMinimumHeatRecoveryWaterFlowRateDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineHeatRecoveryFields::MinimumHeatRecoveryWaterFlowRate);
  }

  double GeneratorMicroTurbineHeatRecovery_Impl::maximumHeatRecoveryWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbineHeatRecoveryFields::MaximumHeatRecoveryWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::isMaximumHeatRecoveryWaterFlowRateDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineHeatRecoveryFields::MaximumHeatRecoveryWaterFlowRate);
  }

  boost::optional<double> GeneratorMicroTurbineHeatRecovery_Impl::maximumHeatRecoveryWaterTemperature() const {
    return getDouble(OS_Generator_MicroTurbineHeatRecoveryFields::MaximumHeatRecoveryWaterTemperature,true);
  }

  
  /*bool GeneratorMicroTurbineHeatRecovery_Impl::setHeatRecoveryWaterInletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterInletNodeName, connection.get().handle());
    }
    else {
      resetHeatRecoveryWaterInletNode();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetHeatRecoveryWaterInletNode() {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterInletNodeName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setHeatRecoveryWaterOutletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterOutletNodeName, connection.get().handle());
    }
    else {
      resetHeatRecoveryWaterOutletNode();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetHeatRecoveryWaterOutletNode() {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterOutletNodeName, "");
    OS_ASSERT(result);
  } */

  bool GeneratorMicroTurbineHeatRecovery_Impl::setReferenceThermalEfficiencyUsingLowerHeatValue(double referenceThermalEfficiencyUsingLowerHeatValue) {
    bool result = setDouble(OS_Generator_MicroTurbineHeatRecoveryFields::ReferenceThermalEfficiencyUsingLowerHeatValue, referenceThermalEfficiencyUsingLowerHeatValue);
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetReferenceThermalEfficiencyUsingLowerHeatValue() {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::ReferenceThermalEfficiencyUsingLowerHeatValue, "");
    OS_ASSERT(result);
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::setReferenceInletWaterTemperature(boost::optional<double> referenceInletWaterTemperature) {
    bool result(false);
    if (referenceInletWaterTemperature) {
      result = setDouble(OS_Generator_MicroTurbineHeatRecoveryFields::ReferenceInletWaterTemperature, referenceInletWaterTemperature.get());
    }
    else {
      resetReferenceInletWaterTemperature();
      result = true;
    }
    OS_ASSERT(result);
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetReferenceInletWaterTemperature() {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::ReferenceInletWaterTemperature, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setHeatRecoveryWaterFlowOperatingMode(std::string heatRecoveryWaterFlowOperatingMode) {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterFlowOperatingMode, heatRecoveryWaterFlowOperatingMode);
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetHeatRecoveryWaterFlowOperatingMode() {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterFlowOperatingMode, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setReferenceHeatRecoveryWaterFlowRate(boost::optional<double> referenceHeatRecoveryWaterFlowRate) {
    bool result(false);
    if (referenceHeatRecoveryWaterFlowRate) {
      result = setDouble(OS_Generator_MicroTurbineHeatRecoveryFields::ReferenceHeatRecoveryWaterFlowRate, referenceHeatRecoveryWaterFlowRate.get());
    }
    else {
      resetReferenceHeatRecoveryWaterFlowRate();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetReferenceHeatRecoveryWaterFlowRate() {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::ReferenceHeatRecoveryWaterFlowRate, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      if(model() != curve.get().model())
      {
        LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
        return false;
      }
      result = setPointer(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurveName, curve.get().handle());
    }
    else {
      resetHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setThermalEfficiencyFunctionofTemperatureandElevationCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      if(model() != curve.get().model())
      {
        LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
        return false;
      }
      result = setPointer(OS_Generator_MicroTurbineHeatRecoveryFields::ThermalEfficiencyFunctionofTemperatureandElevationCurveName, curve.get().handle());
    }
    else {
      resetThermalEfficiencyFunctionofTemperatureandElevationCurve();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetThermalEfficiencyFunctionofTemperatureandElevationCurve() {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::ThermalEfficiencyFunctionofTemperatureandElevationCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setHeatRecoveryRateFunctionofPartLoadRatioCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      if(model() != curve.get().model())
      {
        LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
        return false;
      }
      result = setPointer(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryRateFunctionofPartLoadRatioCurveName, curve.get().handle());
    }
    else {
      resetHeatRecoveryRateFunctionofPartLoadRatioCurve();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetHeatRecoveryRateFunctionofPartLoadRatioCurve() {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryRateFunctionofPartLoadRatioCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setHeatRecoveryRateFunctionofInletWaterTemperatureCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      if(model() != curve.get().model())
      {
        LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
        return false;
      }
      result = setPointer(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryRateFunctionofInletWaterTemperatureCurveName, curve.get().handle());
    }
    else {
      resetHeatRecoveryRateFunctionofInletWaterTemperatureCurve();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetHeatRecoveryRateFunctionofInletWaterTemperatureCurve() {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryRateFunctionofInletWaterTemperatureCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setHeatRecoveryRateFunctionofWaterFlowRateCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      if(model() != curve.get().model())
      {
        LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
        return false;
      }
      result = setPointer(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryRateFunctionofWaterFlowRateCurveName, curve.get().handle());
    }
    else {
      resetHeatRecoveryRateFunctionofWaterFlowRateCurve();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetHeatRecoveryRateFunctionofWaterFlowRateCurve() {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryRateFunctionofWaterFlowRateCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setMinimumHeatRecoveryWaterFlowRate(double minimumHeatRecoveryWaterFlowRate) {
    bool result = setDouble(OS_Generator_MicroTurbineHeatRecoveryFields::MinimumHeatRecoveryWaterFlowRate, minimumHeatRecoveryWaterFlowRate);
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetMinimumHeatRecoveryWaterFlowRate() {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::MinimumHeatRecoveryWaterFlowRate, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setMaximumHeatRecoveryWaterFlowRate(double maximumHeatRecoveryWaterFlowRate) {
    bool result = setDouble(OS_Generator_MicroTurbineHeatRecoveryFields::MaximumHeatRecoveryWaterFlowRate, maximumHeatRecoveryWaterFlowRate);
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetMaximumHeatRecoveryWaterFlowRate() {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::MaximumHeatRecoveryWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::setMaximumHeatRecoveryWaterTemperature(boost::optional<double> maximumHeatRecoveryWaterTemperature) {
    bool result(false);
    if (maximumHeatRecoveryWaterTemperature) {
      result = setDouble(OS_Generator_MicroTurbineHeatRecoveryFields::MaximumHeatRecoveryWaterTemperature, maximumHeatRecoveryWaterTemperature.get());
    }
    else {
      resetMaximumHeatRecoveryWaterTemperature();
      result = true;
    }
    OS_ASSERT(result);
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetMaximumHeatRecoveryWaterTemperature() {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::MaximumHeatRecoveryWaterTemperature, "");
    OS_ASSERT(result);
  }

} // detail

GeneratorMicroTurbineHeatRecovery::GeneratorMicroTurbineHeatRecovery(const Model& model)
  : StraightComponent(GeneratorMicroTurbineHeatRecovery::iddObjectType(),model)
{}

IddObjectType GeneratorMicroTurbineHeatRecovery::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_MicroTurbine);
}

std::vector<std::string> GeneratorMicroTurbineHeatRecovery::fuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_MicroTurbineHeatRecoveryFields::FuelType);
}

std::vector<std::string> GeneratorMicroTurbineHeatRecovery::heatRecoveryWaterFlowOperatingModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterFlowOperatingMode);
}

/*
boost::optional<Connection> GeneratorMicroTurbineHeatRecovery::heatRecoveryWaterInletNode() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->heatRecoveryWaterInletNode();
}

boost::optional<Connection> GeneratorMicroTurbineHeatRecovery::heatRecoveryWaterOutletNode() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->heatRecoveryWaterOutletNode();
}
*/

double GeneratorMicroTurbineHeatRecovery::referenceThermalEfficiencyUsingLowerHeatValue() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->referenceThermalEfficiencyUsingLowerHeatValue();
}

bool GeneratorMicroTurbineHeatRecovery::isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted();
}

boost::optional<double> GeneratorMicroTurbineHeatRecovery::referenceInletWaterTemperature() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->referenceInletWaterTemperature();
}

std::string GeneratorMicroTurbineHeatRecovery::heatRecoveryWaterFlowOperatingMode() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->heatRecoveryWaterFlowOperatingMode();
}

bool GeneratorMicroTurbineHeatRecovery::isHeatRecoveryWaterFlowOperatingModeDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->isHeatRecoveryWaterFlowOperatingModeDefaulted();
}

boost::optional<double> GeneratorMicroTurbineHeatRecovery::referenceHeatRecoveryWaterFlowRate() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->referenceHeatRecoveryWaterFlowRate();
}

boost::optional<Curve> GeneratorMicroTurbineHeatRecovery::heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve();
}

boost::optional<Curve> GeneratorMicroTurbineHeatRecovery::thermalEfficiencyFunctionofTemperatureandElevationCurve() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->thermalEfficiencyFunctionofTemperatureandElevationCurve();
}

boost::optional<Curve> GeneratorMicroTurbineHeatRecovery::heatRecoveryRateFunctionofPartLoadRatioCurve() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->heatRecoveryRateFunctionofPartLoadRatioCurve();
}

boost::optional<Curve> GeneratorMicroTurbineHeatRecovery::heatRecoveryRateFunctionofInletWaterTemperatureCurve() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->heatRecoveryRateFunctionofInletWaterTemperatureCurve();
}

boost::optional<Curve> GeneratorMicroTurbineHeatRecovery::heatRecoveryRateFunctionofWaterFlowRateCurve() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->heatRecoveryRateFunctionofWaterFlowRateCurve();
}

double GeneratorMicroTurbineHeatRecovery::minimumHeatRecoveryWaterFlowRate() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->minimumHeatRecoveryWaterFlowRate();
}

bool GeneratorMicroTurbineHeatRecovery::isMinimumHeatRecoveryWaterFlowRateDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->isMinimumHeatRecoveryWaterFlowRateDefaulted();
}

double GeneratorMicroTurbineHeatRecovery::maximumHeatRecoveryWaterFlowRate() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->maximumHeatRecoveryWaterFlowRate();
}

bool GeneratorMicroTurbineHeatRecovery::isMaximumHeatRecoveryWaterFlowRateDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->isMaximumHeatRecoveryWaterFlowRateDefaulted();
}

boost::optional<double> GeneratorMicroTurbineHeatRecovery::maximumHeatRecoveryWaterTemperature() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->maximumHeatRecoveryWaterTemperature();
}

/*
bool GeneratorMicroTurbineHeatRecovery::setHeatRecoveryWaterInletNode(const Connection& connection) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setHeatRecoveryWaterInletNode(connection);
}

void GeneratorMicroTurbineHeatRecovery::resetHeatRecoveryWaterInletNode() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetHeatRecoveryWaterInletNode();
}

bool GeneratorMicroTurbineHeatRecovery::setHeatRecoveryWaterOutletNode(const Connection& connection) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setHeatRecoveryWaterOutletNode(connection);
}

void GeneratorMicroTurbineHeatRecovery::resetHeatRecoveryWaterOutletNode() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetHeatRecoveryWaterOutletNode();
} */

bool GeneratorMicroTurbineHeatRecovery::setReferenceThermalEfficiencyUsingLowerHeatValue(double referenceThermalEfficiencyUsingLowerHeatValue) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setReferenceThermalEfficiencyUsingLowerHeatValue(referenceThermalEfficiencyUsingLowerHeatValue);
}

void GeneratorMicroTurbineHeatRecovery::resetReferenceThermalEfficiencyUsingLowerHeatValue() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetReferenceThermalEfficiencyUsingLowerHeatValue();
}

void GeneratorMicroTurbineHeatRecovery::setReferenceInletWaterTemperature(double referenceInletWaterTemperature) {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setReferenceInletWaterTemperature(referenceInletWaterTemperature);
}

void GeneratorMicroTurbineHeatRecovery::resetReferenceInletWaterTemperature() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetReferenceInletWaterTemperature();
}

bool GeneratorMicroTurbineHeatRecovery::setHeatRecoveryWaterFlowOperatingMode(std::string heatRecoveryWaterFlowOperatingMode) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setHeatRecoveryWaterFlowOperatingMode(heatRecoveryWaterFlowOperatingMode);
}

void GeneratorMicroTurbineHeatRecovery::resetHeatRecoveryWaterFlowOperatingMode() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetHeatRecoveryWaterFlowOperatingMode();
}

bool GeneratorMicroTurbineHeatRecovery::setReferenceHeatRecoveryWaterFlowRate(double referenceHeatRecoveryWaterFlowRate) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setReferenceHeatRecoveryWaterFlowRate(referenceHeatRecoveryWaterFlowRate);
}

void GeneratorMicroTurbineHeatRecovery::resetReferenceHeatRecoveryWaterFlowRate() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetReferenceHeatRecoveryWaterFlowRate();
}

bool GeneratorMicroTurbineHeatRecovery::setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve(curve);
}

void GeneratorMicroTurbineHeatRecovery::resetHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve();
}

bool GeneratorMicroTurbineHeatRecovery::setThermalEfficiencyFunctionofTemperatureandElevationCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setThermalEfficiencyFunctionofTemperatureandElevationCurve(curve);
}

void GeneratorMicroTurbineHeatRecovery::resetThermalEfficiencyFunctionofTemperatureandElevationCurve() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetThermalEfficiencyFunctionofTemperatureandElevationCurve();
}

bool GeneratorMicroTurbineHeatRecovery::setHeatRecoveryRateFunctionofPartLoadRatioCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setHeatRecoveryRateFunctionofPartLoadRatioCurve(curve);
}

void GeneratorMicroTurbineHeatRecovery::resetHeatRecoveryRateFunctionofPartLoadRatioCurve() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetHeatRecoveryRateFunctionofPartLoadRatioCurve();
}

bool GeneratorMicroTurbineHeatRecovery::setHeatRecoveryRateFunctionofInletWaterTemperatureCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setHeatRecoveryRateFunctionofInletWaterTemperatureCurve(curve);
}

void GeneratorMicroTurbineHeatRecovery::resetHeatRecoveryRateFunctionofInletWaterTemperatureCurve() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetHeatRecoveryRateFunctionofInletWaterTemperatureCurve();
}

bool GeneratorMicroTurbineHeatRecovery::setHeatRecoveryRateFunctionofWaterFlowRateCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setHeatRecoveryRateFunctionofWaterFlowRateCurve(curve);
}

void GeneratorMicroTurbineHeatRecovery::resetHeatRecoveryRateFunctionofWaterFlowRateCurve() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetHeatRecoveryRateFunctionofWaterFlowRateCurve();
}

bool GeneratorMicroTurbineHeatRecovery::setMinimumHeatRecoveryWaterFlowRate(double minimumHeatRecoveryWaterFlowRate) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setMinimumHeatRecoveryWaterFlowRate(minimumHeatRecoveryWaterFlowRate);
}

void GeneratorMicroTurbineHeatRecovery::resetMinimumHeatRecoveryWaterFlowRate() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetMinimumHeatRecoveryWaterFlowRate();
}

bool GeneratorMicroTurbineHeatRecovery::setMaximumHeatRecoveryWaterFlowRate(double maximumHeatRecoveryWaterFlowRate) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setMaximumHeatRecoveryWaterFlowRate(maximumHeatRecoveryWaterFlowRate);
}

void GeneratorMicroTurbineHeatRecovery::resetMaximumHeatRecoveryWaterFlowRate() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetMaximumHeatRecoveryWaterFlowRate();
}

void GeneratorMicroTurbineHeatRecovery::setMaximumHeatRecoveryWaterTemperature(double maximumHeatRecoveryWaterTemperature) {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setMaximumHeatRecoveryWaterTemperature(maximumHeatRecoveryWaterTemperature);
}

void GeneratorMicroTurbineHeatRecovery::resetMaximumHeatRecoveryWaterTemperature() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetMaximumHeatRecoveryWaterTemperature();
}


/// New Methods
  // Problably useless...
  //boost::optional<std::string> GeneratorMicroTurbineHeatRecovery_Impl::heatRecoveryWaterInletNodeName() const {
  //  return getString(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterInletNodeName,true);
  //}

  /*ModelObject GeneratorMicroTurbineHeatRecovery_Impl::clone(Model model) const
  {
    GeneratorMicroTurbineHeatRecovery newMCHP = StraightComponent_Impl::clone(model).cast<GeneratorMicroTurbineHeatRecovery>();

    return newMCHP;
  }
  
  bool GeneratorMicroTurbineHeatRecovery_Impl::addToNode(Node & node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() )
    {
      if( plant->supplyComponent(node.handle()) )
      {
        if( StraightComponent_Impl::addToNode(node) )
        {
          return true;
        }
      }
    }

    return false;
  }
*/



/// @cond
GeneratorMicroTurbineHeatRecovery::GeneratorMicroTurbineHeatRecovery(std::shared_ptr<detail::GeneratorMicroTurbineHeatRecovery_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

