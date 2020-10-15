/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "CoilLiquidDesiccantSimple.hpp"
#include "CoilLiquidDesiccantSimple_Impl.hpp"

#include "ControllerWaterCoil.hpp"
#include "ControllerWaterCoil_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ScheduleCompact.hpp"
#include "ScheduleCompact_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "AirflowNetworkEquivalentDuct.hpp"
#include "AirflowNetworkEquivalentDuct_Impl.hpp"
// #include "WaterStorageTank.hpp"
// #include "WaterStorageTank_Impl.hpp"

#include <utilities/idd/OS_Coil_LiquidDesiccant_Simple_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilLiquidDesiccantSimple_Impl::CoilLiquidDesiccantSimple_Impl(const IdfObject& idfObject,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : WaterToAirComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilLiquidDesiccantSimple::iddObjectType());
  }

  CoilLiquidDesiccantSimple_Impl::CoilLiquidDesiccantSimple_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilLiquidDesiccantSimple::iddObjectType());
  }

  CoilLiquidDesiccantSimple_Impl::CoilLiquidDesiccantSimple_Impl(const CoilLiquidDesiccantSimple_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {}

  CoilLiquidDesiccantSimple_Impl::~CoilLiquidDesiccantSimple_Impl(){}

  std::vector<openstudio::IdfObject> CoilLiquidDesiccantSimple_Impl::remove() {
    if( isRemovable() ) {
      return WaterToAirComponent_Impl::remove();
    }

    return std::vector<IdfObject>();
  }

  bool CoilLiquidDesiccantSimple_Impl::removeFromPlantLoop() {
    if( boost::optional<ControllerWaterCoil> controller = this->controllerWaterCoil() )
    {
      controller->remove();
    }

    return WaterToAirComponent_Impl::removeFromPlantLoop();
  }

  ModelObject CoilLiquidDesiccantSimple_Impl::clone(Model model) const {
    CoilLiquidDesiccantSimple newCoil = WaterToAirComponent_Impl::clone( model ).optionalCast<CoilLiquidDesiccantSimple>().get();

    return newCoil;
  }

  IddObjectType CoilLiquidDesiccantSimple_Impl::iddObjectType() const {
    return CoilLiquidDesiccantSimple::iddObjectType();
  }

  const std::vector<std::string>& CoilLiquidDesiccantSimple_Impl::outputVariableNames() const {
    static const std::vector<std::string> result{
      // TODO
    };
    return result;
  }

  std::vector<ScheduleTypeKey> CoilLiquidDesiccantSimple_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_LiquidDesiccant_SimpleFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilLiquidDesiccantSimple","Availability"));
    }
    return result;
  }

  unsigned CoilLiquidDesiccantSimple_Impl::airInletPort() const {
    return OS_Coil_LiquidDesiccant_SimpleFields::AirInletNodeName;
  }

  unsigned CoilLiquidDesiccantSimple_Impl::airOutletPort() const {
    return OS_Coil_LiquidDesiccant_SimpleFields::AirOutletNodeName;
  }

  unsigned CoilLiquidDesiccantSimple_Impl::waterInletPort() const {
    return OS_Coil_LiquidDesiccant_SimpleFields::WaterInletNodeName;
  }

  unsigned CoilLiquidDesiccantSimple_Impl::waterOutletPort() const {
    return OS_Coil_LiquidDesiccant_SimpleFields::WaterOutletNodeName;
  }

  bool CoilLiquidDesiccantSimple_Impl::addToNode(Node & node) {
    // TODO

    return true;
  }

  boost::optional<HVACComponent> CoilLiquidDesiccantSimple_Impl::containingHVACComponent() const {
    // TODO

    return boost::none;
  }

  boost::optional<ZoneHVACComponent> CoilLiquidDesiccantSimple_Impl::containingZoneHVACComponent() const {
    // TODO

    return boost::none;
  }

  std::vector<ModelObject> CoilLiquidDesiccantSimple_Impl::children() const {
    std::vector<ModelObject> result;
    std::vector<AirflowNetworkEquivalentDuct> myAFNItems = getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
    result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());
    return result;
  }

  Schedule CoilLiquidDesiccantSimple_Impl::availabilitySchedule() const {
    OptionalSchedule value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_LiquidDesiccant_SimpleFields::AvailabilityScheduleName);
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<CoilLiquidDesiccantSimple_Impl*>(this)->setAvailabilitySchedule(*value);
      value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_LiquidDesiccant_SimpleFields::AvailabilityScheduleName);
    }
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::designWaterFlowRate() {
    return getDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignWaterFlowRate);
  }

  bool CoilLiquidDesiccantSimple_Impl::isDesignWaterFlowRateAutosized() {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_LiquidDesiccant_SimpleFields::DesignWaterFlowRate, true);
    if (value) {
      result = istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::designAirFlowRate() {
    return getDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignAirFlowRate);
  }

  bool CoilLiquidDesiccantSimple_Impl::isDesignAirFlowRateAutosized() {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_LiquidDesiccant_SimpleFields::DesignAirFlowRate, true);
    if (value) {
      result = istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::designInletWaterTemperature() {
    return getDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignInletWaterTemperature);
  }

  bool CoilLiquidDesiccantSimple_Impl::isDesignInletWaterTemperatureAutosized() {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_LiquidDesiccant_SimpleFields::DesignInletWaterTemperature, true);
    if (value) {
      result = istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::designInletAirTemperature() {
    return getDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignInletAirTemperature);
  }

  bool CoilLiquidDesiccantSimple_Impl::isDesignInletAirTemperatureAutosized() {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_LiquidDesiccant_SimpleFields::DesignInletAirTemperature, true);
    if (value) {
      result = istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::designOutletAirTemperature() {
    return getDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignOutletAirTemperature);
  }

  bool CoilLiquidDesiccantSimple_Impl::isDesignOutletAirTemperatureAutosized() {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_LiquidDesiccant_SimpleFields::DesignOutletAirTemperature, true);
    if (value) {
      result = istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::designInletAirHumidityRatio() {
    return getDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignInletAirHumidityRatio);
  }

  bool CoilLiquidDesiccantSimple_Impl::isDesignInletAirHumidityRatioAutosized() {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_LiquidDesiccant_SimpleFields::DesignInletAirHumidityRatio, true);
    if (value) {
      result = istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::designOutletAirHumidityRatio() {
    return getDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignOutletAirHumidityRatio);
  }

  bool CoilLiquidDesiccantSimple_Impl::isDesignOutletAirHumidityRatioAutosized() {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_LiquidDesiccant_SimpleFields::DesignOutletAirHumidityRatio, true);
    if (value) {
      result = istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::designInletSolutionConcentration() {
    return getDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignInletSolutionConcentration);
  }

  bool CoilLiquidDesiccantSimple_Impl::isDesignInletSolutionConcentrationAutosized() {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_LiquidDesiccant_SimpleFields::DesignInletSolutionConcentration, true);
    if (value) {
      result = istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::designFanPowerPerUnitMassFlowRate() {
    return getDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignFanPowerPerUnitMassFlowRate);
  }

  bool CoilLiquidDesiccantSimple_Impl::isDesignFanPowerPerUnitMassFlowRateAutosized() {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_LiquidDesiccant_SimpleFields::DesignFanPowerPerUnitMassFlowRate, true);
    if (value) {
      result = istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::outdoorAirFlowRates() {
    return getDouble(OS_Coil_LiquidDesiccant_SimpleFields::OutdoorAirFlowRates);
  }

  bool CoilLiquidDesiccantSimple_Impl::isOutdoorAirFlowRatesAutosized() {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_LiquidDesiccant_SimpleFields::OutdoorAirFlowRates, true);
    if (value) {
      result = istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::designPumpPower() {
    return getDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignPumpPower);
  }

  bool CoilLiquidDesiccantSimple_Impl::isDesignPumpPowerAutosized() {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_LiquidDesiccant_SimpleFields::DesignPumpPower, true);
    if (value) {
      result = istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::designEffectivenessAtNormalCondition() {
    return getDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignEffectivenessAtNormalCondition);
  }

  bool CoilLiquidDesiccantSimple_Impl::isDesignEffectivenessAtNornmalConditionAutosized() {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_LiquidDesiccant_SimpleFields::DesignEffectivenessAtNormalCondition, true);
    if (value) {
      result = istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  std::string CoilLiquidDesiccantSimple_Impl::typeOfOperationMode() {
    boost::optional<std::string> value = getString(OS_Coil_LiquidDesiccant_SimpleFields::TypeOfOperationMode, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilLiquidDesiccantSimple_Impl::isTypeOfOperationModeDefaulted() {
    return isEmpty(OS_Coil_LiquidDesiccant_SimpleFields::TypeOfOperationMode);
  }

  std::string CoilLiquidDesiccantSimple_Impl::airSource() {
    boost::optional<std::string> value = getString(OS_Coil_LiquidDesiccant_SimpleFields::AirSource, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilLiquidDesiccantSimple_Impl::isAirSourceDefaulted() {
    return isEmpty(OS_Coil_LiquidDesiccant_SimpleFields::AirSource);
  }

  std::string CoilLiquidDesiccantSimple_Impl::material() {
    boost::optional<std::string> value = getString(OS_Coil_LiquidDesiccant_SimpleFields::Material, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilLiquidDesiccantSimple_Impl::isMaterialDefaulted() {
    return isEmpty(OS_Coil_LiquidDesiccant_SimpleFields::Material);
  }

  // boost::optional<WaterStorageTank> CoilLiquidDesiccantSimple_Impl::condensateCollectionWaterStorageTank() const {
  //   return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(OS_Coil_LiquidDesiccant_SimpleFields::CondensateCollectionWaterStorageTankName);
  // }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::designLiquidDesiccantConcentrationDifference() {
    return getDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignLiquidDesiccantConcentrationDifference);
  }

  bool CoilLiquidDesiccantSimple_Impl::CoilLiquidDesiccantSimple_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_LiquidDesiccant_SimpleFields::AvailabilityScheduleName,
                              "CoilLiquidDesiccantSimple",
                              "Availability",
                              schedule);
    return result;
  }

  bool CoilLiquidDesiccantSimple_Impl::setDesignWaterFlowRate(double designWaterFlowRate) {
    bool result = setDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignWaterFlowRate, designWaterFlowRate);
    OS_ASSERT(result);
    return result;
  }

  void CoilLiquidDesiccantSimple_Impl::autosizeDesignWaterFlowRate() {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::DesignWaterFlowRate, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilLiquidDesiccantSimple_Impl::setDesignAirFlowRate(double designAirFlowRate) {
    bool result = setDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignAirFlowRate, designAirFlowRate);
    OS_ASSERT(result);
    return result;
  }

  void CoilLiquidDesiccantSimple_Impl::autosizeDesignAirFlowRate() {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::DesignAirFlowRate, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilLiquidDesiccantSimple_Impl::setDesignInletWaterTemperature(double designInletWaterTemperature) {
    bool result = setDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignInletWaterTemperature, designInletWaterTemperature);
    OS_ASSERT(result);
    return result;
  }

  void CoilLiquidDesiccantSimple_Impl::autosizeDesignInletWaterTemperature() {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::DesignInletWaterTemperature, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilLiquidDesiccantSimple_Impl::setDesignInletAirTemperature(double designInletAirTemperature) {
    bool result = setDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignInletAirTemperature, designInletAirTemperature);
    OS_ASSERT(result);
    return result;
  }

  void CoilLiquidDesiccantSimple_Impl::autosizeDesignInletAirTemperature() {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::DesignInletAirTemperature, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilLiquidDesiccantSimple_Impl::setDesignOutletAirTemperature(double designOutletAirTemperature) {
    bool result = setDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignOutletAirTemperature, designOutletAirTemperature);
    OS_ASSERT(result);
    return result;
  }

  void CoilLiquidDesiccantSimple_Impl::autosizeDesignOutletAirTemperature() {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::DesignOutletAirTemperature, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilLiquidDesiccantSimple_Impl::setDesignInletAirHumidityRatio(double designInletAirHumidityRatio) {
    bool result = setDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignInletAirHumidityRatio, designInletAirHumidityRatio);
    OS_ASSERT(result);
    return result;
  }

  void CoilLiquidDesiccantSimple_Impl::autosizeDesignInletAirHumidityRatio() {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::DesignInletAirHumidityRatio, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilLiquidDesiccantSimple_Impl::setDesignOutletAirHumidityRatio(double designOutletAirHumidityRatio) {
    bool result = setDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignOutletAirHumidityRatio, designOutletAirHumidityRatio);
    OS_ASSERT(result);
    return result;
  }

  void CoilLiquidDesiccantSimple_Impl::autosizeDesignOutletAirHumidityRatio() {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::DesignOutletAirHumidityRatio, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilLiquidDesiccantSimple_Impl::setDesignInletSolutionConcentration(double designInletSolutionConcentration) {
    bool result = setDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignInletSolutionConcentration, designInletSolutionConcentration);
    OS_ASSERT(result);
    return result;
  }

  void CoilLiquidDesiccantSimple_Impl::autosizeDesignInletSolutionConcentration() {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::DesignInletSolutionConcentration, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilLiquidDesiccantSimple_Impl::setDesignFanPowerPerUnitMassFlowRate(double designFanPowerPerUnitMassFlowRate) {
    bool result = setDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignFanPowerPerUnitMassFlowRate, designFanPowerPerUnitMassFlowRate);
    OS_ASSERT(result);
    return result;
  }

  void CoilLiquidDesiccantSimple_Impl::autosizeDesignFanPowerPerUnitMassFlowRate() {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::DesignFanPowerPerUnitMassFlowRate, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilLiquidDesiccantSimple_Impl::setOutdoorAirFlowRates(double outdoorAirFlowRates) {
    bool result = setDouble(OS_Coil_LiquidDesiccant_SimpleFields::OutdoorAirFlowRates, outdoorAirFlowRates);
    OS_ASSERT(result);
    return result;
  }

  void CoilLiquidDesiccantSimple_Impl::autosizeOutdoorAirFlowRates() {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::OutdoorAirFlowRates, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilLiquidDesiccantSimple_Impl::setDesignPumpPower(double designPumpPower) {
    bool result = setDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignPumpPower, designPumpPower);
    OS_ASSERT(result);
    return result;
  }

  void CoilLiquidDesiccantSimple_Impl::autosizeDesignPumpPower() {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::DesignPumpPower, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilLiquidDesiccantSimple_Impl::setDesignEffectivenessAtNormalCondition(double designEffectivenessAtNormalCondition) {
    bool result = setDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignEffectivenessAtNormalCondition, designEffectivenessAtNormalCondition);
    OS_ASSERT(result);
    return result;
  }

  void CoilLiquidDesiccantSimple_Impl::autosizeDesignEffectivenessAtNormalCondition() {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::DesignEffectivenessAtNormalCondition, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilLiquidDesiccantSimple_Impl::setTypeOfOperationMode(std::string typeOfOperationMode) {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::TypeOfOperationMode, typeOfOperationMode);
    OS_ASSERT(result);
    return result;
  }

  void CoilLiquidDesiccantSimple_Impl::resetTypeOfOperationMode() {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::TypeOfOperationMode, "");
    OS_ASSERT(result);
  }

  bool CoilLiquidDesiccantSimple_Impl::setAirSource(std::string airSource) {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::AirSource, airSource);
    OS_ASSERT(result);
    return result;
  }

  void CoilLiquidDesiccantSimple_Impl::resetAirSource() {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::AirSource, "");
    OS_ASSERT(result);
  }

  bool CoilLiquidDesiccantSimple_Impl::setMaterial(std::string material) {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::Material, material);
    OS_ASSERT(result);
    return result;
  }

  void CoilLiquidDesiccantSimple_Impl::resetMaterial() {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::Material, "");
    OS_ASSERT(result);
  }

  // bool CoilLiquidDesiccantSimple_Impl::setCondensateCollectionWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank) {
  //   bool result(false);
  //   if (waterStorageTank) {
  //     result = setPointer(OS_Coil_LiquidDesiccant_SimpleFields::CondensateCollectionWaterStorageTankName, waterStorageTank.get().handle());
  //   }
  //   else {
  //     resetCondensateCollectionWaterStorageTank();
  //     result = true;
  //   }
  //   return result;
  // }

  // void CoilLiquidDesiccantSimple_Impl::resetCondensateCollectionWaterStorageTank() {
  //   bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::CondensateCollectionWaterStorageTankName, "");
  //   OS_ASSERT(result);
  // }

  bool CoilLiquidDesiccantSimple_Impl::setDesignLiquidDesiccantConcentrationDifference(double designLiquidDesiccantConcentrationDifference) {
    bool result = setDouble(OS_Coil_LiquidDesiccant_SimpleFields::DesignLiquidDesiccantConcentrationDifference, designLiquidDesiccantConcentrationDifference);
    OS_ASSERT(result);
    return result;
  }

  void CoilLiquidDesiccantSimple_Impl::resetDesignLiquidDesiccantConcentrationDifference() {
    bool result = setString(OS_Coil_LiquidDesiccant_SimpleFields::DesignLiquidDesiccantConcentrationDifference, "");
    OS_ASSERT(result);
  }

  AirflowNetworkEquivalentDuct CoilLiquidDesiccantSimple_Impl::getAirflowNetworkEquivalentDuct(double length, double diameter) {
    boost::optional<AirflowNetworkEquivalentDuct> opt = airflowNetworkEquivalentDuct();
    if (opt) {
      if (opt->airPathLength() != length){
        opt->setAirPathLength(length);
      }
      if (opt->airPathHydraulicDiameter() != diameter){
        opt->setAirPathHydraulicDiameter(diameter);
      }
      return opt.get();
    }
    return AirflowNetworkEquivalentDuct(model(), length, diameter, handle());
  }

  boost::optional<AirflowNetworkEquivalentDuct> CoilLiquidDesiccantSimple_Impl::airflowNetworkEquivalentDuct() const
  {
    std::vector<AirflowNetworkEquivalentDuct> myAFN = getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>
      (AirflowNetworkEquivalentDuct::iddObjectType());
    auto count = myAFN.size();
    if (count == 1) {
      return myAFN[0];
    } else if (count > 1) {
      LOG(Warn, briefDescription() << " has more than one AirflowNetwork EquivalentDuct attached, returning first.");
      return myAFN[0];
    }
    return boost::none;
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::autosizedDesignWaterFlowRate() const {
    return getAutosizedValue("Design Size Design Water Flow Rate", "m3/s");
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::autosizedDesignAirFlowRate() const {
    return getAutosizedValue("Design Size Design Air Flow Rate", "m3/s");
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::autosizedDesignInletWaterTemperature() const {
    return getAutosizedValue("Design Size Design Inlet Water Temperature", "C");
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::autosizedDesignInletAirTemperature() const {
    return getAutosizedValue("Design Size Design Inlet Air Temperature", "C");
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::autosizedDesignOutletAirTemperature() const {
    return getAutosizedValue("Design Size Design Outlet Air Temperature", "C");
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::autosizedDesignInletAirHumidityRatio() const {
    return getAutosizedValue("Design Size Design Inlet Air Humidity Ratio", "kgWater/kgDryAir");
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::autosizedDesignOutletAirHumidityRatio() const {
    return getAutosizedValue("Design Size Design Outlet Air Humidity Ratio", "kgWater/kgDryAir");
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::autosizedDesignInletSolutionConcentration() const {
    return getAutosizedValue("Design Size Design Inlet Solution Concentration", "dimensionless");
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::autosizedDesignFanPowerPerUnitMassFlowRate() const {
    return getAutosizedValue("Design Size Design Can Power Per Unit Mass Flow Rate", "watt");
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::autosizedOutdoorAirFlowRates() const {
    return getAutosizedValue("Design Size Design Outdoor Air Flow Rates", "m3/s");
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::autosizedDesignPumpPower() const {
    return getAutosizedValue("Design Size Design Pump Power", "watt");
  }

  boost::optional<double> CoilLiquidDesiccantSimple_Impl::autosizedDesignEffectivenessAtNormalCondition() const {
    return getAutosizedValue("Design Size Design Effectiveness At Normal Condition", "watt");
  }

  void CoilLiquidDesiccantSimple_Impl::autosize() {
    autosizeDesignWaterFlowRate();
    autosizeDesignAirFlowRate();
    autosizeDesignInletWaterTemperature();
    autosizeDesignInletAirTemperature();
    autosizeDesignOutletAirTemperature();
    autosizeDesignInletAirHumidityRatio();
    autosizeDesignOutletAirHumidityRatio();
    autosizeDesignInletSolutionConcentration();
    autosizeDesignFanPowerPerUnitMassFlowRate();
    autosizeOutdoorAirFlowRates();
    autosizeDesignPumpPower();
    autosizeDesignEffectivenessAtNormalCondition();
  }

  void CoilLiquidDesiccantSimple_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedDesignWaterFlowRate();
    if (val) {
      setDesignWaterFlowRate(val.get());
    }

    val = autosizedDesignAirFlowRate();
    if (val) {
      setDesignAirFlowRate(val.get());
    }

    val = autosizedDesignInletWaterTemperature();
    if (val) {
      setDesignInletWaterTemperature(val.get());
    }

    val = autosizedDesignInletAirTemperature();
    if (val) {
      setDesignInletAirTemperature(val.get());
    }

    val = autosizedDesignOutletAirTemperature();
    if (val) {
      setDesignOutletAirTemperature(val.get());
    }

    val = autosizedDesignInletAirHumidityRatio();
    if (val) {
      setDesignInletAirHumidityRatio(val.get());
    }

    val = autosizedDesignOutletAirHumidityRatio();
    if (val) {
      setDesignOutletAirHumidityRatio(val.get());
    }

    val = autosizedDesignInletSolutionConcentration();
    if (val) {
      setDesignInletSolutionConcentration(val.get());
    }

    val = autosizedDesignFanPowerPerUnitMassFlowRate();
    if (val) {
      setDesignFanPowerPerUnitMassFlowRate(val.get());
    }

    val = autosizedOutdoorAirFlowRates();
    if (val) {
      setOutdoorAirFlowRates(val.get());
    }

    val = autosizedDesignPumpPower();
    if (val) {
      setDesignPumpPower(val.get());
    }

    val = autosizedDesignEffectivenessAtNormalCondition();
    if (val) {
      setDesignEffectivenessAtNormalCondition(val.get());
    }
  }

  boost::optional<ModelObject> CoilLiquidDesiccantSimple_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  bool CoilLiquidDesiccantSimple_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

} // detail

CoilLiquidDesiccantSimple::CoilLiquidDesiccantSimple(const Model& model, Schedule& availabilitySchedule)
  : WaterToAirComponent(CoilLiquidDesiccantSimple::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilLiquidDesiccantSimple_Impl>());

  setAvailabilitySchedule(availabilitySchedule);
}

CoilLiquidDesiccantSimple::CoilLiquidDesiccantSimple(const Model& model)
  : WaterToAirComponent(CoilLiquidDesiccantSimple::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilLiquidDesiccantSimple_Impl>());

  auto schedule = model.alwaysOnDiscreteSchedule();
  setAvailabilitySchedule(schedule);
}

CoilLiquidDesiccantSimple::CoilLiquidDesiccantSimple(std::shared_ptr<detail::CoilLiquidDesiccantSimple_Impl> p)
  : WaterToAirComponent(std::move(p))
{}

IddObjectType CoilLiquidDesiccantSimple::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Coil_LiquidDesiccant_Simple);
  return result;
}

Schedule CoilLiquidDesiccantSimple::availabilitySchedule() const {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->availabilitySchedule();
}

boost::optional<double> CoilLiquidDesiccantSimple::designWaterFlowRate() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->designWaterFlowRate();
}

bool CoilLiquidDesiccantSimple::isDesignWaterFlowRateAutosized() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->isDesignWaterFlowRateAutosized();
}

boost::optional<double> CoilLiquidDesiccantSimple::designAirFlowRate() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->designAirFlowRate();
}

bool CoilLiquidDesiccantSimple::isDesignAirFlowRateAutosized() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->isDesignAirFlowRateAutosized();
}

boost::optional<double> CoilLiquidDesiccantSimple::designInletWaterTemperature() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->designInletWaterTemperature();
}

bool CoilLiquidDesiccantSimple::isDesignInletWaterTemperatureAutosized() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->isDesignInletWaterTemperatureAutosized();
}

boost::optional<double> CoilLiquidDesiccantSimple::designInletAirTemperature() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->designInletAirTemperature();
}

bool CoilLiquidDesiccantSimple::isDesignInletAirTemperatureAutosized() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->isDesignInletAirTemperatureAutosized();
}

boost::optional<double> CoilLiquidDesiccantSimple::designOutletAirTemperature() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->designOutletAirTemperature();
}

bool CoilLiquidDesiccantSimple::isDesignOutletAirTemperatureAutosized() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->isDesignOutletAirTemperatureAutosized();
}

boost::optional<double> CoilLiquidDesiccantSimple::designInletAirHumidityRatio() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->designInletAirHumidityRatio();
}

bool CoilLiquidDesiccantSimple::isDesignInletAirHumidityRatioAutosized() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->isDesignInletAirHumidityRatioAutosized();
}

boost::optional<double> CoilLiquidDesiccantSimple::designOutletAirHumidityRatio() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->designOutletAirHumidityRatio();
}

bool CoilLiquidDesiccantSimple::isDesignOutletAirHumidityRatioAutosized() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->isDesignOutletAirHumidityRatioAutosized();
}

boost::optional<double> CoilLiquidDesiccantSimple::designInletSolutionConcentration() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->designInletSolutionConcentration();
}

bool CoilLiquidDesiccantSimple::isDesignInletSolutionConcentrationAutosized() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->isDesignInletSolutionConcentrationAutosized();
}

boost::optional<double> CoilLiquidDesiccantSimple::designFanPowerPerUnitMassFlowRate() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->designFanPowerPerUnitMassFlowRate();
}

bool CoilLiquidDesiccantSimple::isDesignFanPowerPerUnitMassFlowRateAutosized() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->isDesignFanPowerPerUnitMassFlowRateAutosized();
}

boost::optional<double> CoilLiquidDesiccantSimple::outdoorAirFlowRates() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->outdoorAirFlowRates();
}

bool CoilLiquidDesiccantSimple::isOutdoorAirFlowRatesAutosized() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->isOutdoorAirFlowRatesAutosized();
}

boost::optional<double> CoilLiquidDesiccantSimple::designPumpPower() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->designPumpPower();
}

bool CoilLiquidDesiccantSimple::isDesignPumpPowerAutosized() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->isDesignPumpPowerAutosized();
}

boost::optional<double> CoilLiquidDesiccantSimple::designEffectivenessAtNormalCondition() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->designEffectivenessAtNormalCondition();
}

bool CoilLiquidDesiccantSimple::isDesignEffectivenessAtNornmalConditionAutosized() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->isDesignEffectivenessAtNornmalConditionAutosized();
}

std::string CoilLiquidDesiccantSimple::typeOfOperationMode() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->typeOfOperationMode();
}

bool CoilLiquidDesiccantSimple::isTypeOfOperationModeDefaulted() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->isTypeOfOperationModeDefaulted();
}

std::string CoilLiquidDesiccantSimple::airSource() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->airSource();
}

bool CoilLiquidDesiccantSimple::isAirSourceDefaulted() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->isAirSourceDefaulted();
}

std::string CoilLiquidDesiccantSimple::material() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->material();
}

bool CoilLiquidDesiccantSimple::isMaterialDefaulted() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->isMaterialDefaulted();
}

// boost::optional<WaterStorageTank> CoilLiquidDesiccantSimple::condensateCollectionWaterStorageTank() const {
//   return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->condensateCollectionWaterStorageTank();
// }

boost::optional<double> CoilLiquidDesiccantSimple::designLiquidDesiccantConcentrationDifference() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->designLiquidDesiccantConcentrationDifference();
}

bool CoilLiquidDesiccantSimple::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setAvailabilitySchedule(schedule);
}

bool CoilLiquidDesiccantSimple::setDesignWaterFlowRate(double designWaterFlowRate) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setDesignWaterFlowRate(designWaterFlowRate);
}

void CoilLiquidDesiccantSimple::autosizeDesignWaterFlowRate() {
  getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizeDesignWaterFlowRate();
}

bool CoilLiquidDesiccantSimple::setDesignAirFlowRate(double designAirFlowRate) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setDesignAirFlowRate(designAirFlowRate);
}

void CoilLiquidDesiccantSimple::autosizeDesignAirFlowRate() {
  getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizeDesignAirFlowRate();
}

bool CoilLiquidDesiccantSimple::setDesignInletWaterTemperature(double designInletWaterTemperature) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setDesignInletWaterTemperature(designInletWaterTemperature);
}

void CoilLiquidDesiccantSimple::autosizeDesignInletWaterTemperature() {
  getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizeDesignInletWaterTemperature();
}

bool CoilLiquidDesiccantSimple::setDesignInletAirTemperature(double designInletAirTemperature) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setDesignInletAirTemperature(designInletAirTemperature);
}

void CoilLiquidDesiccantSimple::autosizeDesignInletAirTemperature() {
  getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizeDesignInletAirTemperature();
}

bool CoilLiquidDesiccantSimple::setDesignOutletAirTemperature(double designOutletAirTemperature) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setDesignOutletAirTemperature(designOutletAirTemperature);
}

void CoilLiquidDesiccantSimple::autosizeDesignOutletAirTemperature() {
  getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizeDesignOutletAirTemperature();
}

bool CoilLiquidDesiccantSimple::setDesignInletAirHumidityRatio(double designInletAirHumidityRatio) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setDesignInletAirHumidityRatio(designInletAirHumidityRatio);
}

void CoilLiquidDesiccantSimple::autosizeDesignInletAirHumidityRatio() {
  getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizeDesignInletAirHumidityRatio();
}

bool CoilLiquidDesiccantSimple::setDesignOutletAirHumidityRatio(double designOutletAirHumidityRatio) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setDesignOutletAirHumidityRatio(designOutletAirHumidityRatio);
}

void CoilLiquidDesiccantSimple::autosizeDesignOutletAirHumidityRatio() {
  getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizeDesignOutletAirHumidityRatio();
}

bool CoilLiquidDesiccantSimple::setDesignInletSolutionConcentration(double designInletSolutionConcentration) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setDesignInletSolutionConcentration(designInletSolutionConcentration);
}

void CoilLiquidDesiccantSimple::autosizeDesignInletSolutionConcentration() {
  getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizeDesignInletSolutionConcentration();
}

bool CoilLiquidDesiccantSimple::setDesignFanPowerPerUnitMassFlowRate(double designFanPowerPerUnitMassFlowRate) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setDesignFanPowerPerUnitMassFlowRate(designFanPowerPerUnitMassFlowRate);
}

void CoilLiquidDesiccantSimple::autosizeDesignFanPowerPerUnitMassFlowRate() {
  getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizeDesignFanPowerPerUnitMassFlowRate();
}

bool CoilLiquidDesiccantSimple::setOutdoorAirFlowRates(double outdoorAirFlowRates) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setOutdoorAirFlowRates(outdoorAirFlowRates);
}

void CoilLiquidDesiccantSimple::autosizeOutdoorAirFlowRates() {
  getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizeOutdoorAirFlowRates();
}

bool CoilLiquidDesiccantSimple::setDesignPumpPower(double designPumpPower) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setDesignPumpPower(designPumpPower);
}

void CoilLiquidDesiccantSimple::autosizeDesignPumpPower() {
  getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizeDesignPumpPower();
}

bool CoilLiquidDesiccantSimple::setDesignEffectivenessAtNormalCondition(double designEffectivenessAtNormalCondition) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setDesignEffectivenessAtNormalCondition(designEffectivenessAtNormalCondition);
}

void CoilLiquidDesiccantSimple::autosizeDesignEffectivenessAtNormalCondition() {
  getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizeDesignEffectivenessAtNormalCondition();
}

bool CoilLiquidDesiccantSimple::setTypeOfOperationMode(std::string typeOfOperationMode) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setTypeOfOperationMode(typeOfOperationMode);
}

void CoilLiquidDesiccantSimple::resetTypeOfOperationMode() {
  getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->resetTypeOfOperationMode();
}

bool CoilLiquidDesiccantSimple::setAirSource(std::string airSource) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setAirSource(airSource);
}

void CoilLiquidDesiccantSimple::resetAirSource() {
  getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->resetAirSource();
}

bool CoilLiquidDesiccantSimple::setMaterial(std::string material) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setMaterial(material);
}

void CoilLiquidDesiccantSimple::resetMaterial() {
  getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->resetMaterial();
}


// bool CoilLiquidDesiccantSimple::setCondensateCollectionWaterStorageTank(const WaterStorageTank& waterStorageTank) {
//   return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setCondensateCollectionWaterStorageTank(waterStorageTank);
// }

// void CoilLiquidDesiccantSimple::resetCondensateCollectionWaterStorageTank() {
//   getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->resetCondensateCollectionWaterStorageTank();
// }

bool CoilLiquidDesiccantSimple::setDesignLiquidDesiccantConcentrationDifference(double designLiquidDesiccantConcentrationDifference) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->setDesignLiquidDesiccantConcentrationDifference(designLiquidDesiccantConcentrationDifference);
}

void CoilLiquidDesiccantSimple::resetDesignLiquidDesiccantConcentrationDifference() {
  getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->resetDesignLiquidDesiccantConcentrationDifference();
}

boost::optional<ControllerWaterCoil> CoilLiquidDesiccantSimple::controllerWaterCoil() {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->controllerWaterCoil();
}

AirflowNetworkEquivalentDuct CoilLiquidDesiccantSimple::getAirflowNetworkEquivalentDuct(double length, double diameter) {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->getAirflowNetworkEquivalentDuct(length, diameter);
}

boost::optional<AirflowNetworkEquivalentDuct> CoilLiquidDesiccantSimple::airflowNetworkEquivalentDuct() const {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->airflowNetworkEquivalentDuct();
}

boost::optional<double> CoilLiquidDesiccantSimple::autosizedDesignWaterFlowRate() const {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizedDesignWaterFlowRate();
}

boost::optional<double> CoilLiquidDesiccantSimple::autosizedDesignAirFlowRate() const {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizedDesignAirFlowRate();
}

boost::optional<double> CoilLiquidDesiccantSimple::autosizedDesignInletWaterTemperature() const {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizedDesignInletWaterTemperature();
}

boost::optional<double> CoilLiquidDesiccantSimple::autosizedDesignInletAirTemperature() const {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizedDesignInletAirTemperature();
}

boost::optional<double> CoilLiquidDesiccantSimple::autosizedDesignOutletAirTemperature() const {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizedDesignOutletAirTemperature();
}

boost::optional<double> CoilLiquidDesiccantSimple::autosizedDesignInletAirHumidityRatio() const {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizedDesignInletAirHumidityRatio();
}

boost::optional<double> CoilLiquidDesiccantSimple::autosizedDesignOutletAirHumidityRatio() const {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizedDesignOutletAirHumidityRatio();
}

boost::optional<double> CoilLiquidDesiccantSimple::autosizedDesignInletSolutionConcentration() const {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizedDesignInletSolutionConcentration();
}

boost::optional<double> CoilLiquidDesiccantSimple::autosizedDesignFanPowerPerUnitMassFlowRate() const {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizedDesignFanPowerPerUnitMassFlowRate();
}

boost::optional<double> CoilLiquidDesiccantSimple::autosizedOutdoorAirFlowRates() const {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizedOutdoorAirFlowRates();
}

boost::optional<double> CoilLiquidDesiccantSimple::autosizedDesignPumpPower() const {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizedDesignPumpPower();
}

boost::optional<double> CoilLiquidDesiccantSimple::autosizedDesignEffectivenessAtNormalCondition() const {
  return getImpl<detail::CoilLiquidDesiccantSimple_Impl>()->autosizedDesignEffectivenessAtNormalCondition();
}

} // model
} // openstudio
