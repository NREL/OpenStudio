/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "ElectricEquipmentITEAirCooledDefinition.hpp"
#include "ElectricEquipmentITEAirCooledDefinition_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "../model/CurveBiquadratic.hpp"
#include "../model/CurveBiquadratic_Impl.hpp"
#include "../model/CurveQuadratic.hpp"
#include "../model/CurveQuadratic_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"
#include "attributes.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ElectricEquipment_ITE_AirCooled_Definition_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"
#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ElectricEquipmentITEAirCooledDefinition_Impl::ElectricEquipmentITEAirCooledDefinition_Impl(const IdfObject& idfObject,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : SpaceLoadDefinition_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ElectricEquipmentITEAirCooledDefinition::iddObjectType());
  }

  ElectricEquipmentITEAirCooledDefinition_Impl::ElectricEquipmentITEAirCooledDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ElectricEquipmentITEAirCooledDefinition::iddObjectType());
  }

  ElectricEquipmentITEAirCooledDefinition_Impl::ElectricEquipmentITEAirCooledDefinition_Impl(const ElectricEquipmentITEAirCooledDefinition_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ElectricEquipmentITEAirCooledDefinition_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    // Not appropriate: output is listed in ElectricEquipment instead
    return result;
  }

  IddObjectType ElectricEquipmentITEAirCooledDefinition_Impl::iddObjectType() const {
    return ElectricEquipmentITEAirCooledDefinition::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ElectricEquipmentITEAirCooledDefinition_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::SupplyTemperatureDifferenceSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("ElectricEquipmentITEAirCooledDefinition","Supply Temperature Difference Schedule"));
    }
    if (std::find(b,e,OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::ReturnTemperatureDifferenceSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("ElectricEquipmentITEAirCooledDefinition","Return Temperature Difference Schedule"));
    }
    return result;
  }

  std::string ElectricEquipmentITEAirCooledDefinition_Impl::airFlowCalculationMethod() const {
    boost::optional<std::string> value = getString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::AirFlowCalculationMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ElectricEquipmentITEAirCooledDefinition_Impl::designPowerInputCalculationMethod() const {
    boost::optional<std::string> value = getString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignPowerInputCalculationMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ElectricEquipmentITEAirCooledDefinition_Impl::wattsperUnit() const {
    boost::optional<double> result;
    if (istringEqual("Watts/Unit", this->designPowerInputCalculationMethod())) {
      result = getDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::WattsperUnit, true);
      //OS_ASSERT(result);
    }
    return result;
  }

  boost::optional<double> ElectricEquipmentITEAirCooledDefinition_Impl::wattsperZoneFloorArea() const {
    boost::optional<double> result;
    if (istringEqual("Watts/Area", this->designPowerInputCalculationMethod())) {
      result = getDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::WattsperZoneFloorArea, true);
      //OS_ASSERT(result);
    }
    return result;
  }

  Curve ElectricEquipmentITEAirCooledDefinition_Impl::cPUPowerInputFunctionofLoadingandAirTemperatureCurve() const {
    boost::optional<Curve> value = optionalCPUPowerInputFunctionofLoadingandAirTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have a CPU Power Input Function of Loading and Air Temperature Curve attached.");
    }
    return value.get();
  }

  boost::optional<Curve> ElectricEquipmentITEAirCooledDefinition_Impl::optionalCPUPowerInputFunctionofLoadingandAirTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::CPUPowerInputFunctionofLoadingandAirTemperatureCurveName);
  }

  double ElectricEquipmentITEAirCooledDefinition_Impl::designFanPowerInputFraction() const {
    boost::optional<double> value = getDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignFanPowerInputFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::isDesignFanPowerInputFractionDefaulted() const {
    return isEmpty(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignFanPowerInputFraction);
  }

  boost::optional<double> ElectricEquipmentITEAirCooledDefinition_Impl::designFanAirFlowRateperPowerInput() const {
    return getDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignFanAirFlowRateperPowerInput, true);
  }

  OSOptionalQuantity ElectricEquipmentITEAirCooledDefinition_Impl::getDesignFanAirFlowRateperPowerInput(bool returnIP) const {
    OptionalDouble value = designFanAirFlowRateperPowerInput();
    return getQuantityFromDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignFanAirFlowRateperPowerInput, value, returnIP);
  }

  Curve ElectricEquipmentITEAirCooledDefinition_Impl::airFlowFunctionofLoadingandAirTemperatureCurve() const {
    boost::optional<Curve> value = optionalAirFlowFunctionofLoadingandAirTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Air Flow Function of Loading and Air Temperature Curve attached.");
    }
    return value.get();
  }

  boost::optional<Curve> ElectricEquipmentITEAirCooledDefinition_Impl::optionalAirFlowFunctionofLoadingandAirTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::AirFlowFunctionofLoadingandAirTemperatureCurveName);
  }

  Curve ElectricEquipmentITEAirCooledDefinition_Impl::fanPowerInputFunctionofFlowCurve() const {
    boost::optional<Curve> value = optionalFanPowerInputFunctionofFlowCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have a Fan Power Input Function of Flow Curve attached.");
    }
    return value.get();
  }

  boost::optional<Curve> ElectricEquipmentITEAirCooledDefinition_Impl::optionalFanPowerInputFunctionofFlowCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::FanPowerInputFunctionofFlowCurveName);
  }

  double ElectricEquipmentITEAirCooledDefinition_Impl::designEnteringAirTemperature() const {
    boost::optional<double> value = getDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignEnteringAirTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::isDesignEnteringAirTemperatureDefaulted() const {
    return isEmpty(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignEnteringAirTemperature);
  }

  std::string ElectricEquipmentITEAirCooledDefinition_Impl::environmentalClass() const {
    boost::optional<std::string> value = getString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::EnvironmentalClass,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ElectricEquipmentITEAirCooledDefinition_Impl::airInletConnectionType() const {
    boost::optional<std::string> value = getString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::AirInletConnectionType,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ElectricEquipmentITEAirCooledDefinition_Impl::designRecirculationFraction() const {
    boost::optional<double> value = getDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignRecirculationFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::isDesignRecirculationFractionDefaulted() const {
    return isEmpty(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignRecirculationFraction);
  }

  boost::optional<Curve> ElectricEquipmentITEAirCooledDefinition_Impl::recirculationFunctionofLoadingandSupplyTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::RecirculationFunctionofLoadingandSupplyTemperatureCurveName);
  }

  double ElectricEquipmentITEAirCooledDefinition_Impl::designElectricPowerSupplyEfficiency() const {
    boost::optional<double> value = getDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignElectricPowerSupplyEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::isDesignElectricPowerSupplyEfficiencyDefaulted() const {
    return isEmpty(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignElectricPowerSupplyEfficiency);
  }

  boost::optional<Curve> ElectricEquipmentITEAirCooledDefinition_Impl::electricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::ElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurveName);
  }

  double ElectricEquipmentITEAirCooledDefinition_Impl::fractionofElectricPowerSupplyLossestoZone() const {
    boost::optional<double> value = getDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::FractionofElectricPowerSupplyLossestoZone,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::isFractionofElectricPowerSupplyLossestoZoneDefaulted() const {
    return isEmpty(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::FractionofElectricPowerSupplyLossestoZone);
  }

  double ElectricEquipmentITEAirCooledDefinition_Impl::supplyTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::SupplyTemperatureDifference, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::isSupplyTemperatureDifferenceDefaulted() const {
    return isEmpty(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::SupplyTemperatureDifference);
  }

  boost::optional<Schedule> ElectricEquipmentITEAirCooledDefinition_Impl::supplyTemperatureDifferenceSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::SupplyTemperatureDifferenceSchedule);
  }

  double ElectricEquipmentITEAirCooledDefinition_Impl::returnTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::ReturnTemperatureDifference, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::isReturnTemperatureDifferenceDefaulted() const {
    return isEmpty(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::ReturnTemperatureDifference);
  }

  boost::optional<Schedule> ElectricEquipmentITEAirCooledDefinition_Impl::returnTemperatureDifferenceSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::ReturnTemperatureDifferenceSchedule);
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setAirFlowCalculationMethod(const std::string& airFlowCalculationMethod) {
    std::string wmethod(airFlowCalculationMethod);
    boost::to_lower(wmethod);

    if (wmethod == "flowfromsystem") {
      return setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::AirFlowCalculationMethod, "FlowFromSystem"); 
    } else if (wmethod == "flowcontrolwithapproachtemperatures") {
      bool result;
      result = setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::AirFlowCalculationMethod, "FlowControlWithApproachTemperatures");
      OS_ASSERT(result);
      result = setSupplyTemperatureDifference(supplyTemperatureDifference());
      OS_ASSERT(result);
      result = setReturnTemperatureDifference(returnTemperatureDifference());
      OS_ASSERT(result);
      return result;
    }

    return false;
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setDesignPowerInputCalculationMethod(const std::string& designPowerInputCalculationMethod, double floorArea) {
    std::string wmethod(designPowerInputCalculationMethod);
    boost::to_lower(wmethod);

    if (wmethod == "watts/unit") {
      return setWattsperUnit(getWattsperUnit(floorArea));
    } else if (wmethod == "watts/area") {
      return setWattsperZoneFloorArea(getWattsperZoneFloorArea(floorArea));
    }

    return false;
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setWattsperUnit(boost::optional<double> wattsperUnit) {
    bool result = true;
    if (wattsperUnit) {
      if (*wattsperUnit < 0) {
        result = false;
      } else {
        result = setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignPowerInputCalculationMethod, "Watts/Unit");
        OS_ASSERT(result);
        result = setDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::WattsperUnit, wattsperUnit.get());
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("Watts/Unit", this->designPowerInputCalculationMethod())) {
        result = setDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::WattsperUnit, 0.0);
      }
    }
    return result;
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setWattsperZoneFloorArea(boost::optional<double> wattsperZoneFloorArea) {
    bool result = true;
    if (wattsperZoneFloorArea) {
      if (*wattsperZoneFloorArea < 0) {
        result = false;
      } else {
        result = setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignPowerInputCalculationMethod, "Watts/Area");
        OS_ASSERT(result);
        result = setDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::WattsperZoneFloorArea, wattsperZoneFloorArea.get());
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("Watts/Area", this->designPowerInputCalculationMethod())) {
        result = setDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::WattsperZoneFloorArea, 0.0);
      }
    }
    return result;
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setCPUPowerInputFunctionofLoadingandAirTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::CPUPowerInputFunctionofLoadingandAirTemperatureCurveName, curve.handle());
    return result;
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setDesignFanPowerInputFraction(double designFanPowerInputFraction) {
    bool result = setDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignFanPowerInputFraction, designFanPowerInputFraction);
    return result;
  }

  void ElectricEquipmentITEAirCooledDefinition_Impl::resetDesignFanPowerInputFraction() {
    bool result = setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignFanPowerInputFraction, "");
    OS_ASSERT(result);
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setDesignFanAirFlowRateperPowerInput(double designFanAirFlowRateperPowerInput) {
    bool result = setDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignFanAirFlowRateperPowerInput, designFanAirFlowRateperPowerInput);
    return result;
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setAirFlowFunctionofLoadingandAirTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::AirFlowFunctionofLoadingandAirTemperatureCurveName, curve.handle());
    return result;
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setFanPowerInputFunctionofFlowCurve(const Curve& curve) {
    bool result = setPointer(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::FanPowerInputFunctionofFlowCurveName, curve.handle());
    return result;
  }

  void ElectricEquipmentITEAirCooledDefinition_Impl::setDesignEnteringAirTemperature(double designEnteringAirTemperature) {
    bool result = setDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignEnteringAirTemperature, designEnteringAirTemperature);
    OS_ASSERT(result);
  }

  void ElectricEquipmentITEAirCooledDefinition_Impl::resetDesignEnteringAirTemperature() {
    bool result = setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignEnteringAirTemperature, "");
    OS_ASSERT(result);
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setEnvironmentalClass(const std::string& environmentalClass) {
    bool result = setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::EnvironmentalClass, environmentalClass);
    return result;
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setAirInletConnectionType(const std::string& airInletConnectionType) {
    bool result = setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::AirInletConnectionType, airInletConnectionType);
    return result;
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setDesignRecirculationFraction(double designRecirculationFraction) {
    bool result = setDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignRecirculationFraction, designRecirculationFraction);
    return result;
  }

  void ElectricEquipmentITEAirCooledDefinition_Impl::resetDesignRecirculationFraction() {
    bool result = setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignRecirculationFraction, "");
    OS_ASSERT(result);
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setRecirculationFunctionofLoadingandSupplyTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::RecirculationFunctionofLoadingandSupplyTemperatureCurveName, curve.handle());
    return result;
  }

  void ElectricEquipmentITEAirCooledDefinition_Impl::resetRecirculationFunctionofLoadingandSupplyTemperatureCurve() {
    bool result = setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::RecirculationFunctionofLoadingandSupplyTemperatureCurveName, "");
    OS_ASSERT(result);
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setDesignElectricPowerSupplyEfficiency(double designElectricPowerSupplyEfficiency) {
    bool result = setDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignElectricPowerSupplyEfficiency, designElectricPowerSupplyEfficiency);
    return result;
  }

  void ElectricEquipmentITEAirCooledDefinition_Impl::resetDesignElectricPowerSupplyEfficiency() {
    bool result = setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignElectricPowerSupplyEfficiency, "");
    OS_ASSERT(result);
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve(const Curve& curve) {
    bool result = setPointer(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::ElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurveName, curve.handle());
    return result;
  }

  void ElectricEquipmentITEAirCooledDefinition_Impl::resetElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve() {
    bool result = setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::ElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurveName, "");
    OS_ASSERT(result);
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setFractionofElectricPowerSupplyLossestoZone(double fractionofElectricPowerSupplyLossestoZone) {
    bool result = setDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::FractionofElectricPowerSupplyLossestoZone, fractionofElectricPowerSupplyLossestoZone);
    return result;
  }

  void ElectricEquipmentITEAirCooledDefinition_Impl::resetFractionofElectricPowerSupplyLossestoZone() {
    bool result = setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::FractionofElectricPowerSupplyLossestoZone, "");
    OS_ASSERT(result);
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setSupplyTemperatureDifference(double supplyTemperatureDifference) {
    bool result = setDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::SupplyTemperatureDifference, supplyTemperatureDifference);
    OS_ASSERT(result);
    return result;
  }

  void ElectricEquipmentITEAirCooledDefinition_Impl::resetSupplyTemperatureDifference() {
    bool result = setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::SupplyTemperatureDifference, "");
    OS_ASSERT(result);
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setSupplyTemperatureDifferenceSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::SupplyTemperatureDifferenceSchedule,
                              "ElectricEquipmentITEAirCooledDefinition",
                              "Supply Temperature Difference Schedule",
                              schedule);
    return result;
  }

  void ElectricEquipmentITEAirCooledDefinition_Impl::resetSupplyTemperatureDifferenceSchedule() {
    bool result = setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::SupplyTemperatureDifferenceSchedule, "");
    OS_ASSERT(result);
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setReturnTemperatureDifference(double returnTemperatureDifference) {
    bool result = setDouble(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::ReturnTemperatureDifference, returnTemperatureDifference);
    OS_ASSERT(result);
    return result;
  }

  void ElectricEquipmentITEAirCooledDefinition_Impl::resetReturnTemperatureDifference() {
    bool result = setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::ReturnTemperatureDifference, "");
    OS_ASSERT(result);
  }

  bool ElectricEquipmentITEAirCooledDefinition_Impl::setReturnTemperatureDifferenceSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::ReturnTemperatureDifferenceSchedule,
                              "ElectricEquipmentITEAirCooledDefinition",
                              "Return Temperature Difference Schedule",
                              schedule);
    return result;
  }

  void ElectricEquipmentITEAirCooledDefinition_Impl::resetReturnTemperatureDifferenceSchedule() {
    bool result = setString(OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::ReturnTemperatureDifferenceSchedule, "");
    OS_ASSERT(result);
  }


  //Other functions
  double ElectricEquipmentITEAirCooledDefinition_Impl::getWattsperUnit(double floorArea) const {
    std::string method = designPowerInputCalculationMethod();

    if (method == "Watts/Unit") {
      return wattsperUnit().get();
    } else if (method == "Watts/Area") {
      return wattsperZoneFloorArea().get() * floorArea;
    }

    OS_ASSERT(false);
    return 0.0;
  }

  double ElectricEquipmentITEAirCooledDefinition_Impl::getWattsperZoneFloorArea(double floorArea) const {
    std::string method = designPowerInputCalculationMethod();

    if (method == "Watts/Unit") {
      if (equal(floorArea, 0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return wattsperUnit().get() / floorArea;
    } else if (method == "Watts/Area") {
      return wattsperZoneFloorArea().get();
    }

    OS_ASSERT(false);
    return 0.0;
  }

} // detail

//define ElectricEquipmentITEAirCooledDefinition with default curves
ElectricEquipmentITEAirCooledDefinition::ElectricEquipmentITEAirCooledDefinition(const Model& model)
  : SpaceLoadDefinition(ElectricEquipmentITEAirCooledDefinition::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>());

  //assign default curves
  //reference: Comparative Study of High Ambient Inlet Temperature Effects on the Performance of Air vs. Liquid Cooled IT Equipment
  CurveBiquadratic cPUPowerInputFunctionofLoadingandAirTemperatureCurve(model);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setCoefficient1Constant(-0.035289);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setCoefficient2x(1.0);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setCoefficient3xPOW2(0.0);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setCoefficient4y(0.0015684);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setCoefficient5yPOW2(0.0);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setCoefficient6xTIMESY(0.0);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setMinimumValueofx(0.0);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setMaximumValueofx(1.5);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setMinimumValueofy(-10);
  cPUPowerInputFunctionofLoadingandAirTemperatureCurve.setMaximumValueofy(99.0);
  setCPUPowerInputFunctionofLoadingandAirTemperatureCurve(cPUPowerInputFunctionofLoadingandAirTemperatureCurve);
  
  //reference: Comparative Study of High Ambient Inlet Temperature Effects on the Performance of Air vs. Liquid Cooled IT Equipment
  CurveBiquadratic airFlowFunctionofLoadingandAirTemperatureCurve(model);
  airFlowFunctionofLoadingandAirTemperatureCurve.setCoefficient1Constant(-1.025);
  airFlowFunctionofLoadingandAirTemperatureCurve.setCoefficient2x(0.9);
  airFlowFunctionofLoadingandAirTemperatureCurve.setCoefficient3xPOW2(0.0);
  airFlowFunctionofLoadingandAirTemperatureCurve.setCoefficient4y(0.05);
  airFlowFunctionofLoadingandAirTemperatureCurve.setCoefficient5yPOW2(0.0);
  airFlowFunctionofLoadingandAirTemperatureCurve.setCoefficient6xTIMESY(0.0);
  airFlowFunctionofLoadingandAirTemperatureCurve.setMinimumValueofx(0.0);
  airFlowFunctionofLoadingandAirTemperatureCurve.setMaximumValueofx(1.5);
  airFlowFunctionofLoadingandAirTemperatureCurve.setMinimumValueofy(-10);
  airFlowFunctionofLoadingandAirTemperatureCurve.setMaximumValueofy(99.0);
  setAirFlowFunctionofLoadingandAirTemperatureCurve(airFlowFunctionofLoadingandAirTemperatureCurve);

  CurveQuadratic fanPowerInputFunctionofFlowCurve(model);
  fanPowerInputFunctionofFlowCurve.setCoefficient1Constant(0.0);
  fanPowerInputFunctionofFlowCurve.setCoefficient2x(1.0);
  fanPowerInputFunctionofFlowCurve.setCoefficient3xPOW2(0.0);
  fanPowerInputFunctionofFlowCurve.setMinimumValueofx(0.0);
  fanPowerInputFunctionofFlowCurve.setMaximumValueofx(99.0);
  setFanPowerInputFunctionofFlowCurve(fanPowerInputFunctionofFlowCurve);

  bool test = this->setWattsperUnit(0.0);
  OS_ASSERT(test);
}

ElectricEquipmentITEAirCooledDefinition::ElectricEquipmentITEAirCooledDefinition(const Model& model,
  Curve& cPUPowerInputFunctionofLoadingandAirTemperatureCurve,
  Curve& airFlowFunctionofLoadingandAirTemperatureCurve,
  Curve& fanPowerInputFunctionofFlowCurve)
  : SpaceLoadDefinition(ElectricEquipmentITEAirCooledDefinition::iddObjectType(), model) 
{
  OS_ASSERT(getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>());

  setCPUPowerInputFunctionofLoadingandAirTemperatureCurve(cPUPowerInputFunctionofLoadingandAirTemperatureCurve);
  setAirFlowFunctionofLoadingandAirTemperatureCurve(airFlowFunctionofLoadingandAirTemperatureCurve);
  setFanPowerInputFunctionofFlowCurve(fanPowerInputFunctionofFlowCurve);

  bool test = this->setWattsperUnit(0.0);
  OS_ASSERT(test);
}


IddObjectType ElectricEquipmentITEAirCooledDefinition::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ElectricEquipment_ITE_AirCooled_Definition);
}

std::vector<std::string> ElectricEquipmentITEAirCooledDefinition::validAirFlowCalculationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::AirFlowCalculationMethod);
}

std::vector<std::string> ElectricEquipmentITEAirCooledDefinition::validDesignPowerInputCalculationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignPowerInputCalculationMethod);
}

std::vector<std::string> ElectricEquipmentITEAirCooledDefinition::validEnvironmentalClassValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::EnvironmentalClass);
}

std::vector<std::string> ElectricEquipmentITEAirCooledDefinition::validAirInletConnectionTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ElectricEquipment_ITE_AirCooled_DefinitionFields::AirInletConnectionType);
}

std::string ElectricEquipmentITEAirCooledDefinition::airFlowCalculationMethod() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->airFlowCalculationMethod();
}

std::string ElectricEquipmentITEAirCooledDefinition::designPowerInputCalculationMethod() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->designPowerInputCalculationMethod();
}

boost::optional<double> ElectricEquipmentITEAirCooledDefinition::wattsperUnit() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->wattsperUnit();
}

boost::optional<double> ElectricEquipmentITEAirCooledDefinition::wattsperZoneFloorArea() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->wattsperZoneFloorArea();
}

Curve ElectricEquipmentITEAirCooledDefinition::cPUPowerInputFunctionofLoadingandAirTemperatureCurve() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->cPUPowerInputFunctionofLoadingandAirTemperatureCurve();
}

double ElectricEquipmentITEAirCooledDefinition::designFanPowerInputFraction() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->designFanPowerInputFraction();
}

bool ElectricEquipmentITEAirCooledDefinition::isDesignFanPowerInputFractionDefaulted() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->isDesignFanPowerInputFractionDefaulted();
}

boost::optional<double> ElectricEquipmentITEAirCooledDefinition::designFanAirFlowRateperPowerInput() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->designFanAirFlowRateperPowerInput();
}

Curve ElectricEquipmentITEAirCooledDefinition::airFlowFunctionofLoadingandAirTemperatureCurve() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->airFlowFunctionofLoadingandAirTemperatureCurve();
}

Curve ElectricEquipmentITEAirCooledDefinition::fanPowerInputFunctionofFlowCurve() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->fanPowerInputFunctionofFlowCurve();
}

double ElectricEquipmentITEAirCooledDefinition::designEnteringAirTemperature() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->designEnteringAirTemperature();
}

bool ElectricEquipmentITEAirCooledDefinition::isDesignEnteringAirTemperatureDefaulted() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->isDesignEnteringAirTemperatureDefaulted();
}

std::string ElectricEquipmentITEAirCooledDefinition::environmentalClass() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->environmentalClass();
}

std::string ElectricEquipmentITEAirCooledDefinition::airInletConnectionType() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->airInletConnectionType();
}

double ElectricEquipmentITEAirCooledDefinition::designRecirculationFraction() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->designRecirculationFraction();
}

bool ElectricEquipmentITEAirCooledDefinition::isDesignRecirculationFractionDefaulted() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->isDesignRecirculationFractionDefaulted();
}

boost::optional<Curve> ElectricEquipmentITEAirCooledDefinition::recirculationFunctionofLoadingandSupplyTemperatureCurve() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->recirculationFunctionofLoadingandSupplyTemperatureCurve();
}

double ElectricEquipmentITEAirCooledDefinition::designElectricPowerSupplyEfficiency() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->designElectricPowerSupplyEfficiency();
}

bool ElectricEquipmentITEAirCooledDefinition::isDesignElectricPowerSupplyEfficiencyDefaulted() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->isDesignElectricPowerSupplyEfficiencyDefaulted();
}

boost::optional<Curve> ElectricEquipmentITEAirCooledDefinition::electricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->electricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve();
}

double ElectricEquipmentITEAirCooledDefinition::fractionofElectricPowerSupplyLossestoZone() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->fractionofElectricPowerSupplyLossestoZone();
}

bool ElectricEquipmentITEAirCooledDefinition::isFractionofElectricPowerSupplyLossestoZoneDefaulted() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->isFractionofElectricPowerSupplyLossestoZoneDefaulted();
}

double ElectricEquipmentITEAirCooledDefinition::supplyTemperatureDifference() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->supplyTemperatureDifference();
}

bool ElectricEquipmentITEAirCooledDefinition::isSupplyTemperatureDifferenceDefaulted() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->isSupplyTemperatureDifferenceDefaulted();
}

boost::optional<Schedule> ElectricEquipmentITEAirCooledDefinition::supplyTemperatureDifferenceSchedule() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->supplyTemperatureDifferenceSchedule();
}

double ElectricEquipmentITEAirCooledDefinition::returnTemperatureDifference() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->returnTemperatureDifference();
}

bool ElectricEquipmentITEAirCooledDefinition::isReturnTemperatureDifferenceDefaulted() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->isReturnTemperatureDifferenceDefaulted();
}

boost::optional<Schedule> ElectricEquipmentITEAirCooledDefinition::returnTemperatureDifferenceSchedule() const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->returnTemperatureDifferenceSchedule();
}

bool ElectricEquipmentITEAirCooledDefinition::setAirFlowCalculationMethod(const std::string& airFlowCalculationMethod) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setAirFlowCalculationMethod(airFlowCalculationMethod);
}

bool ElectricEquipmentITEAirCooledDefinition::setDesignPowerInputCalculationMethod(const std::string& designPowerInputCalculationMethod, double floorArea) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setDesignPowerInputCalculationMethod(designPowerInputCalculationMethod, floorArea);
}

bool ElectricEquipmentITEAirCooledDefinition::setWattsperUnit(double wattsperUnit) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setWattsperUnit(wattsperUnit);
}

bool ElectricEquipmentITEAirCooledDefinition::setWattsperZoneFloorArea(double wattsperZoneFloorArea) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setWattsperZoneFloorArea(wattsperZoneFloorArea);
}

bool ElectricEquipmentITEAirCooledDefinition::setCPUPowerInputFunctionofLoadingandAirTemperatureCurve(const Curve& curve) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setCPUPowerInputFunctionofLoadingandAirTemperatureCurve(curve);
}

bool ElectricEquipmentITEAirCooledDefinition::setDesignFanPowerInputFraction(double designFanPowerInputFraction) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setDesignFanPowerInputFraction(designFanPowerInputFraction);
}

void ElectricEquipmentITEAirCooledDefinition::resetDesignFanPowerInputFraction() {
  getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->resetDesignFanPowerInputFraction();
}

bool ElectricEquipmentITEAirCooledDefinition::setDesignFanAirFlowRateperPowerInput(double designFanAirFlowRateperPowerInput) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setDesignFanAirFlowRateperPowerInput(designFanAirFlowRateperPowerInput);
}

bool ElectricEquipmentITEAirCooledDefinition::setAirFlowFunctionofLoadingandAirTemperatureCurve(const Curve& curve) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setAirFlowFunctionofLoadingandAirTemperatureCurve(curve);
}

bool ElectricEquipmentITEAirCooledDefinition::setFanPowerInputFunctionofFlowCurve(const Curve& curve) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setFanPowerInputFunctionofFlowCurve(curve);
}

void ElectricEquipmentITEAirCooledDefinition::setDesignEnteringAirTemperature(double designEnteringAirTemperature) {
  getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setDesignEnteringAirTemperature(designEnteringAirTemperature);
}

void ElectricEquipmentITEAirCooledDefinition::resetDesignEnteringAirTemperature() {
  getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->resetDesignEnteringAirTemperature();
}

bool ElectricEquipmentITEAirCooledDefinition::setEnvironmentalClass(const std::string& environmentalClass) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setEnvironmentalClass(environmentalClass);
}

bool ElectricEquipmentITEAirCooledDefinition::setAirInletConnectionType(const std::string& airInletConnectionType) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setAirInletConnectionType(airInletConnectionType);
}

bool ElectricEquipmentITEAirCooledDefinition::setDesignRecirculationFraction(double designRecirculationFraction) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setDesignRecirculationFraction(designRecirculationFraction);
}

void ElectricEquipmentITEAirCooledDefinition::resetDesignRecirculationFraction() {
  getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->resetDesignRecirculationFraction();
}

bool ElectricEquipmentITEAirCooledDefinition::setRecirculationFunctionofLoadingandSupplyTemperatureCurve(const Curve& curve) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setRecirculationFunctionofLoadingandSupplyTemperatureCurve(curve);
}

void ElectricEquipmentITEAirCooledDefinition::resetRecirculationFunctionofLoadingandSupplyTemperatureCurve() {
  getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->resetRecirculationFunctionofLoadingandSupplyTemperatureCurve();
}

bool ElectricEquipmentITEAirCooledDefinition::setDesignElectricPowerSupplyEfficiency(double designElectricPowerSupplyEfficiency) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setDesignElectricPowerSupplyEfficiency(designElectricPowerSupplyEfficiency);
}

void ElectricEquipmentITEAirCooledDefinition::resetDesignElectricPowerSupplyEfficiency() {
  getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->resetDesignElectricPowerSupplyEfficiency();
}

bool ElectricEquipmentITEAirCooledDefinition::setElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve(const Curve& curve) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve(curve);
}

void ElectricEquipmentITEAirCooledDefinition::resetElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve() {
  getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->resetElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve();
}

bool ElectricEquipmentITEAirCooledDefinition::setFractionofElectricPowerSupplyLossestoZone(double fractionofElectricPowerSupplyLossestoZone) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setFractionofElectricPowerSupplyLossestoZone(fractionofElectricPowerSupplyLossestoZone);
}

void ElectricEquipmentITEAirCooledDefinition::resetFractionofElectricPowerSupplyLossestoZone() {
  getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->resetFractionofElectricPowerSupplyLossestoZone();
}

bool ElectricEquipmentITEAirCooledDefinition::setSupplyTemperatureDifference(double supplyTemperatureDifference) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setSupplyTemperatureDifference(supplyTemperatureDifference);
}

void ElectricEquipmentITEAirCooledDefinition::resetSupplyTemperatureDifference() {
  getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->resetSupplyTemperatureDifference();
}

bool ElectricEquipmentITEAirCooledDefinition::setSupplyTemperatureDifferenceSchedule(Schedule& schedule) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setSupplyTemperatureDifferenceSchedule(schedule);
}

void ElectricEquipmentITEAirCooledDefinition::resetSupplyTemperatureDifferenceSchedule() {
  getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->resetSupplyTemperatureDifferenceSchedule();
}

bool ElectricEquipmentITEAirCooledDefinition::setReturnTemperatureDifference(double returnTemperatureDifference) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setReturnTemperatureDifference(returnTemperatureDifference);
}

void ElectricEquipmentITEAirCooledDefinition::resetReturnTemperatureDifference() {
  getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->resetReturnTemperatureDifference();
}

bool ElectricEquipmentITEAirCooledDefinition::setReturnTemperatureDifferenceSchedule(Schedule& schedule) {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->setReturnTemperatureDifferenceSchedule(schedule);
}

void ElectricEquipmentITEAirCooledDefinition::resetReturnTemperatureDifferenceSchedule() {
  getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->resetReturnTemperatureDifferenceSchedule();
}

double ElectricEquipmentITEAirCooledDefinition::getWattsperUnit(double floorArea) const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->getWattsperUnit(floorArea);
}

double ElectricEquipmentITEAirCooledDefinition::getWattsperZoneFloorArea(double floorArea) const {
  return getImpl<detail::ElectricEquipmentITEAirCooledDefinition_Impl>()->getWattsperZoneFloorArea(floorArea);
}

/// @cond
ElectricEquipmentITEAirCooledDefinition::ElectricEquipmentITEAirCooledDefinition(std::shared_ptr<detail::ElectricEquipmentITEAirCooledDefinition_Impl> impl)
  : SpaceLoadDefinition(impl)
{}
/// @endcond

} // model
} // openstudio

