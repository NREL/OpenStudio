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

#include "CoilCoolingDXTwoStageWithHumidityControlMode.hpp"
#include "CoilCoolingDXTwoStageWithHumidityControlMode_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CoilPerformanceDXCooling.hpp"
#include "CoilPerformanceDXCooling_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "AirflowNetworkEquivalentDuct.hpp"
#include "AirflowNetworkEquivalentDuct_Impl.hpp"
#include <utilities/idd/OS_Coil_Cooling_DX_TwoStageWithHumidityControlMode_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include <utilities/idd/IddFactory.hxx>

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingDXTwoStageWithHumidityControlMode_Impl::CoilCoolingDXTwoStageWithHumidityControlMode_Impl(const IdfObject& idfObject,
                                                                                                       Model_Impl* model,
                                                                                                       bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDXTwoStageWithHumidityControlMode::iddObjectType());
  }

  CoilCoolingDXTwoStageWithHumidityControlMode_Impl::CoilCoolingDXTwoStageWithHumidityControlMode_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                       Model_Impl* model,
                                                                                                       bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingDXTwoStageWithHumidityControlMode::iddObjectType());
  }

  CoilCoolingDXTwoStageWithHumidityControlMode_Impl::CoilCoolingDXTwoStageWithHumidityControlMode_Impl(const CoilCoolingDXTwoStageWithHumidityControlMode_Impl& other,
                                                                                                       Model_Impl* model,
                                                                                                       bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilCoolingDXTwoStageWithHumidityControlMode_Impl::outputVariableNames() const
  {
    // TODO: static for now
    static std::vector<std::string> result{
      "Cooling Coil Total Cooling Rate",
      "Cooling Coil Total Cooling Energy",
      "Cooling Coil Sensible Cooling Rate",
      "Cooling Coil Sensible Cooling Energy",
      "Cooling Coil Latent Cooling Rate",
      "Cooling Coil Latent Cooling Energy",
      "Cooling Coil Electric Power",
      "Cooling Coil Electric Energy",
      "Cooling Coil Runtime Fraction",

      // condenserType = [AirCooled, EvaporativelyCooled]
      // if (this->condenserType() == "EvaporativelyCooled") {
        "Cooling Coil Condenser Inlet Temperature",
        "Cooling Coil Evaporative Condenser Water Volume",
        "Cooling Coil Evaporative Condenser Pump Electric Power",
        "Cooling Coil Evaporative Condenser Pump Electric Energy",
        "Cooling Coil Basin Heater Electric Power",
        "Cooling Coil Basin Heater Electric Energy",
        "Cooling Coil Evaporative Condenser Mains Supply Water Volume",
      // }

      // Storage tank isn't implemented
      // if has storage tank:
      // "Cooling Coil Condensate Volume Flow Rate",
      // "Cooling Coil Condensate Volume",
      //

      // Additional variables for Coil:Cooling:DX:TwoStageWithHumidityControlMode only:
      "Cooling Coil Stage 2 Runtime Fraction",
      "Cooling Coil Dehumidification Mode"

      // If not part of AirLoopHVAC:UnitaryHeatPump:AirToAir
      // (if part of a heat pump, crankcase heater is reported only for the heating coil):
      // if ( !this->containingHVACComponent().empty() ) {
      // "Cooling Coil Crankcase Heater Electric Power",
      // "Cooling Coil Crankcase Heater Electric Energy"
      // }

    };
    return result;

  }

  IddObjectType CoilCoolingDXTwoStageWithHumidityControlMode_Impl::iddObjectType() const {
    return CoilCoolingDXTwoStageWithHumidityControlMode::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilCoolingDXTwoStageWithHumidityControlMode_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AvailabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("CoilCoolingDXTwoStageWithHumidityControlMode","Availability Schedule"));
    }
    if (std::find(b,e,OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::BasinHeaterOperatingSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("CoilCoolingDXTwoStageWithHumidityControlMode","Basin Heater Operating Schedule"));
    }
    return result;
  }

  boost::optional<Schedule> CoilCoolingDXTwoStageWithHumidityControlMode_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AvailabilitySchedule);
  }

  double CoilCoolingDXTwoStageWithHumidityControlMode_Impl::crankcaseHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::CrankcaseHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXTwoStageWithHumidityControlMode_Impl::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  int CoilCoolingDXTwoStageWithHumidityControlMode_Impl::numberofCapacityStages() const {
    boost::optional<int> value = getInt(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NumberofCapacityStages,true);
    OS_ASSERT(value);
    return value.get();
  }

  int CoilCoolingDXTwoStageWithHumidityControlMode_Impl::numberofEnhancedDehumidificationModes() const {
    boost::optional<int> value = getInt(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NumberofEnhancedDehumidificationModes,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<CoilPerformanceDXCooling> CoilCoolingDXTwoStageWithHumidityControlMode_Impl::normalModeStage1CoilPerformance() const {
    return getObject<ModelObject>().getModelObjectTarget<CoilPerformanceDXCooling>(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NormalModeStage1CoilPerformance);
  }

  boost::optional<CoilPerformanceDXCooling> CoilCoolingDXTwoStageWithHumidityControlMode_Impl::normalModeStage1Plus2CoilPerformance() const {
    return getObject<ModelObject>().getModelObjectTarget<CoilPerformanceDXCooling>(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NormalModeStage1Plus2CoilPerformance);
  }

  boost::optional<CoilPerformanceDXCooling> CoilCoolingDXTwoStageWithHumidityControlMode_Impl::dehumidificationMode1Stage1CoilPerformance() const {
    return getObject<ModelObject>().getModelObjectTarget<CoilPerformanceDXCooling>(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::DehumidificationMode1Stage1CoilPerformance);
  }

  boost::optional<CoilPerformanceDXCooling> CoilCoolingDXTwoStageWithHumidityControlMode_Impl::dehumidificationMode1Stage1Plus2CoilPerformance() const {
    return getObject<ModelObject>().getModelObjectTarget<CoilPerformanceDXCooling>(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::DehumidificationMode1Stage1Plus2CoilPerformance);
  }

  double CoilCoolingDXTwoStageWithHumidityControlMode_Impl::basinHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::BasinHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXTwoStageWithHumidityControlMode_Impl::basinHeaterSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::BasinHeaterSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> CoilCoolingDXTwoStageWithHumidityControlMode_Impl::basinHeaterOperatingSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::BasinHeaterOperatingSchedule);
  }

  bool CoilCoolingDXTwoStageWithHumidityControlMode_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AvailabilitySchedule,
                              "CoilCoolingDXTwoStageWithHumidityControlMode",
                              "Availability Schedule",
                              schedule);
    return result;
  }

  void CoilCoolingDXTwoStageWithHumidityControlMode_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AvailabilitySchedule, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXTwoStageWithHumidityControlMode_Impl::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
    bool result = setDouble(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::CrankcaseHeaterCapacity, crankcaseHeaterCapacity);
    return result;
  }

  bool CoilCoolingDXTwoStageWithHumidityControlMode_Impl::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
    bool result = setDouble(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
    return result;
  }

  bool CoilCoolingDXTwoStageWithHumidityControlMode_Impl::setNumberofCapacityStages(int numberofCapacityStages) {
    bool result = setInt(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NumberofCapacityStages, numberofCapacityStages);
    return result;
  }

  bool CoilCoolingDXTwoStageWithHumidityControlMode_Impl::setNumberofEnhancedDehumidificationModes(int numberofEnhancedDehumidificationModes) {
    bool result = setInt(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NumberofEnhancedDehumidificationModes, numberofEnhancedDehumidificationModes);
    return result;
  }

  bool CoilCoolingDXTwoStageWithHumidityControlMode_Impl::setNormalModeStage1CoilPerformance(const boost::optional<CoilPerformanceDXCooling>& coilPerformanceDX) {
    bool result(false);
    if (coilPerformanceDX) {
      result = setPointer(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NormalModeStage1CoilPerformance, coilPerformanceDX.get().handle());
    }
    else {
      resetNormalModeStage1CoilPerformance();
      result = true;
    }
    return result;
  }

  void CoilCoolingDXTwoStageWithHumidityControlMode_Impl::resetNormalModeStage1CoilPerformance() {
    bool result = setString(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NormalModeStage1CoilPerformance, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXTwoStageWithHumidityControlMode_Impl::setNormalModeStage1Plus2CoilPerformance(const boost::optional<CoilPerformanceDXCooling>& coilPerformanceDX) {
    bool result(false);
    if (coilPerformanceDX) {
      result = setPointer(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NormalModeStage1Plus2CoilPerformance, coilPerformanceDX.get().handle());
    }
    else {
      resetNormalModeStage1Plus2CoilPerformance();
      result = true;
    }
    return result;
  }

  void CoilCoolingDXTwoStageWithHumidityControlMode_Impl::resetNormalModeStage1Plus2CoilPerformance() {
    bool result = setString(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NormalModeStage1Plus2CoilPerformance, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXTwoStageWithHumidityControlMode_Impl::setDehumidificationMode1Stage1CoilPerformance(const boost::optional<CoilPerformanceDXCooling>& coilPerformanceDX) {
    bool result(false);
    if (coilPerformanceDX) {
      result = setPointer(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::DehumidificationMode1Stage1CoilPerformance, coilPerformanceDX.get().handle());
    }
    else {
      resetDehumidificationMode1Stage1CoilPerformance();
      result = true;
    }
    return result;
  }

  void CoilCoolingDXTwoStageWithHumidityControlMode_Impl::resetDehumidificationMode1Stage1CoilPerformance() {
    bool result = setString(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::DehumidificationMode1Stage1CoilPerformance, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXTwoStageWithHumidityControlMode_Impl::setDehumidificationMode1Stage1Plus2CoilPerformance(const boost::optional<CoilPerformanceDXCooling>& coilPerformanceDX) {
    bool result(false);
    if (coilPerformanceDX) {
      result = setPointer(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::DehumidificationMode1Stage1Plus2CoilPerformance, coilPerformanceDX.get().handle());
    }
    else {
      resetDehumidificationMode1Stage1Plus2CoilPerformance();
      result = true;
    }
    return result;
  }

  void CoilCoolingDXTwoStageWithHumidityControlMode_Impl::resetDehumidificationMode1Stage1Plus2CoilPerformance() {
    bool result = setString(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::DehumidificationMode1Stage1Plus2CoilPerformance, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXTwoStageWithHumidityControlMode_Impl::setBasinHeaterCapacity(double basinHeaterCapacity) {
    bool result = setDouble(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::BasinHeaterCapacity, basinHeaterCapacity);
    return result;
  }

  bool CoilCoolingDXTwoStageWithHumidityControlMode_Impl::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
    bool result = setDouble(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature);
    return result;
  }

  bool CoilCoolingDXTwoStageWithHumidityControlMode_Impl::setBasinHeaterOperatingSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::BasinHeaterOperatingSchedule,
                              "CoilCoolingDXTwoStageWithHumidityControlMode",
                              "Basin Heater Operating Schedule",
                              schedule);
    return result;
  }

  void CoilCoolingDXTwoStageWithHumidityControlMode_Impl::resetBasinHeaterOperatingSchedule() {
    bool result = setString(OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::BasinHeaterOperatingSchedule, "");
    OS_ASSERT(result);
  }

  unsigned CoilCoolingDXTwoStageWithHumidityControlMode_Impl::inletPort() const {
    return OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AirInletNodeName;
  }

  unsigned CoilCoolingDXTwoStageWithHumidityControlMode_Impl::outletPort() const {
    return OS_Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AirOutletNodeName;
  }

  boost::optional<HVACComponent> CoilCoolingDXTwoStageWithHumidityControlMode_Impl::containingHVACComponent() const
  {
    // AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

    for( const auto & airLoopHVACUnitarySystem : airLoopHVACUnitarySystems )
    {
      if( boost::optional<HVACComponent> coolingCoil = airLoopHVACUnitarySystem.coolingCoil() )
      {
        if( coolingCoil->handle() == this->handle() )
        {
          return airLoopHVACUnitarySystem;
        }
      }
    }

    // AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass
    std::vector<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass> bypassSystems = model().getConcreteModelObjects<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>();

    for( const auto & bypassSystem : bypassSystems )
    {
      if( boost::optional<HVACComponent> coolingCoil = bypassSystem.coolingCoil() )
      {
        if( coolingCoil->handle() == this->handle() )
        {
          return bypassSystem;
        }
      }
    }

    // AirLoopHVACUnitaryHeatPumpAirToAir

    std::vector<AirLoopHVACUnitaryHeatPumpAirToAir> airLoopHVACUnitaryHeatPumpAirToAirs;

    airLoopHVACUnitaryHeatPumpAirToAirs = model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

    for( const auto & airLoopHVACUnitaryHeatPumpAirToAir : airLoopHVACUnitaryHeatPumpAirToAirs )
    {
      if( boost::optional<HVACComponent> coil = airLoopHVACUnitaryHeatPumpAirToAir.coolingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return airLoopHVACUnitaryHeatPumpAirToAir;
        }
      }
    }

    return boost::none;
  }

  ModelObject CoilCoolingDXTwoStageWithHumidityControlMode_Impl::clone(Model model) const
  {
    auto newCoil = StraightComponent_Impl::clone(model).cast<CoilCoolingDXTwoStageWithHumidityControlMode>();

    if (model != this->model()) {
      // If there are CoilPerformanceDXCooling objects, clone them as well
      if (boost::optional<CoilPerformanceDXCooling> coilPerf1 = normalModeStage1CoilPerformance())
      {
        CoilPerformanceDXCooling coilPerf1Clone = coilPerf1->clone(model).cast<CoilPerformanceDXCooling>();
        newCoil.setNormalModeStage1CoilPerformance(coilPerf1Clone);
      }

      if (boost::optional<CoilPerformanceDXCooling> coilPerf2 = normalModeStage1Plus2CoilPerformance())
      {
        CoilPerformanceDXCooling coilPerf2Clone = coilPerf2->clone(model).cast<CoilPerformanceDXCooling>();
        newCoil.setNormalModeStage1Plus2CoilPerformance(coilPerf2Clone);
      }

      if (boost::optional<CoilPerformanceDXCooling> coilPerf3 = dehumidificationMode1Stage1CoilPerformance())
      {
        CoilPerformanceDXCooling coilPerf3Clone = coilPerf3->clone(model).cast<CoilPerformanceDXCooling>();
        newCoil.setDehumidificationMode1Stage1CoilPerformance(coilPerf3Clone);
      }

      if (boost::optional<CoilPerformanceDXCooling> coilPerf4 = dehumidificationMode1Stage1Plus2CoilPerformance())
      {
        CoilPerformanceDXCooling coilPerf4Clone = coilPerf4->clone(model).cast<CoilPerformanceDXCooling>();
        newCoil.setDehumidificationMode1Stage1Plus2CoilPerformance(coilPerf4Clone);
      }
    }

    return newCoil;
  }

  std::vector<IddObjectType> CoilCoolingDXTwoStageWithHumidityControlMode_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_CoilPerformance_DX_Cooling);
    return result;
  }


  std::vector<ModelObject> CoilCoolingDXTwoStageWithHumidityControlMode_Impl::children() const
  {
    std::vector<ModelObject> result;
    if( auto mo = normalModeStage1CoilPerformance() ) {
      result.push_back(mo.get());
    }

    if( auto mo = normalModeStage1Plus2CoilPerformance() ) {
      result.push_back(mo.get());
    }

    if( auto mo = dehumidificationMode1Stage1CoilPerformance() ) {
      result.push_back(mo.get());
    }

    if( auto mo = dehumidificationMode1Stage1Plus2CoilPerformance() ) {
      result.push_back(mo.get());
    }
    std::vector<AirflowNetworkEquivalentDuct> myAFNItems = getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
    result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());

    return result;
  }

  AirflowNetworkEquivalentDuct CoilCoolingDXTwoStageWithHumidityControlMode_Impl::getAirflowNetworkEquivalentDuct(double length, double diameter)
  {
    boost::optional<AirflowNetworkEquivalentDuct> opt = airflowNetworkEquivalentDuct();
    if (opt) {
      if (opt->airPathLength() != length){
        opt->setAirPathLength(length);
      }
      if (opt->airPathHydraulicDiameter() != diameter){
        opt->setAirPathHydraulicDiameter(diameter);
      }
    }
    return AirflowNetworkEquivalentDuct(model(), length, diameter, handle());
  }

  boost::optional<AirflowNetworkEquivalentDuct> CoilCoolingDXTwoStageWithHumidityControlMode_Impl::airflowNetworkEquivalentDuct() const
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

  bool CoilCoolingDXTwoStageWithHumidityControlMode_Impl::addToNode(Node & node)
  {
    if( auto airLoop = node.airLoopHVAC() ) {
      if( ! airLoop->demandComponent(node.handle()) ) {
        return StraightComponent_Impl::addToNode( node );
      }
    }

    return false;
  }

} // detail

CoilCoolingDXTwoStageWithHumidityControlMode::CoilCoolingDXTwoStageWithHumidityControlMode(const Model& model)
  : StraightComponent(CoilCoolingDXTwoStageWithHumidityControlMode::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>());

  setNumberofCapacityStages(2);
  setNumberofEnhancedDehumidificationModes(1);
  setCrankcaseHeaterCapacity(0.0);
  setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(10.0);
  setBasinHeaterCapacity(0.0);
  setBasinHeaterSetpointTemperature(2.0);

  {
    CurveBiquadratic totalCoolingCapacityFunctionofTemperatureCurve(model);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient1Constant(1.067939449);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient2x(-0.031261829);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient3xPOW2(0.001974308);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient4y(-0.002726426);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient5yPOW2(-5.52654E-05);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient6xTIMESY(-6.31169E-05);
    totalCoolingCapacityFunctionofTemperatureCurve.setMinimumValueofx(-100);
    totalCoolingCapacityFunctionofTemperatureCurve.setMaximumValueofx(100);
    totalCoolingCapacityFunctionofTemperatureCurve.setMinimumValueofy(-100);
    totalCoolingCapacityFunctionofTemperatureCurve.setMaximumValueofy(100);

    CurveQuadratic totalCoolingCapacityFunctionofFlowFractionCurve(model);
    totalCoolingCapacityFunctionofFlowFractionCurve.setCoefficient1Constant(0.8);
    totalCoolingCapacityFunctionofFlowFractionCurve.setCoefficient2x(0.2);
    totalCoolingCapacityFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0);
    totalCoolingCapacityFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
    totalCoolingCapacityFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

    CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(model);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient1Constant(0.174059889);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient2x(0.022281508);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient3xPOW2(-0.000134077);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient4y(0.028298025);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient5yPOW2(0.00048510);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient6xTIMESY(-0.001677095);
    energyInputRatioFunctionofTemperatureCurve.setMinimumValueofx(-100);
    energyInputRatioFunctionofTemperatureCurve.setMaximumValueofx(100);
    energyInputRatioFunctionofTemperatureCurve.setMinimumValueofy(-100);
    energyInputRatioFunctionofTemperatureCurve.setMaximumValueofy(100);

    CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(model);
    energyInputRatioFunctionofFlowFractionCurve.setCoefficient1Constant(1.1552);
    energyInputRatioFunctionofFlowFractionCurve.setCoefficient2x(-0.1808);
    energyInputRatioFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0256);
    energyInputRatioFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
    energyInputRatioFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

    CurveQuadratic partLoadFractionCorrelationCurve(model);
    partLoadFractionCorrelationCurve.setCoefficient1Constant(0.85);
    partLoadFractionCorrelationCurve.setCoefficient2x(0.15);
    partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
    partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
    partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);

    CoilPerformanceDXCooling coilPerformanceDXCooling(model,
      totalCoolingCapacityFunctionofTemperatureCurve,
      totalCoolingCapacityFunctionofFlowFractionCurve,
      energyInputRatioFunctionofTemperatureCurve,
      energyInputRatioFunctionofFlowFractionCurve,
      partLoadFractionCorrelationCurve);

    coilPerformanceDXCooling.setGrossRatedCoolingCOP(3.0);
    coilPerformanceDXCooling.setFractionofAirFlowBypassedAroundCoil(0.5);

    setNormalModeStage1CoilPerformance(coilPerformanceDXCooling);
  }

  {
    CurveBiquadratic totalCoolingCapacityFunctionofTemperatureCurve(model);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient1Constant(1.067939449);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient2x(-0.031261829);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient3xPOW2(0.001974308);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient4y(-0.002726426);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient5yPOW2(-5.52654E-05);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient6xTIMESY(-6.31169E-05);
    totalCoolingCapacityFunctionofTemperatureCurve.setMinimumValueofx(-100);
    totalCoolingCapacityFunctionofTemperatureCurve.setMaximumValueofx(100);
    totalCoolingCapacityFunctionofTemperatureCurve.setMinimumValueofy(-100);
    totalCoolingCapacityFunctionofTemperatureCurve.setMaximumValueofy(100);

    CurveQuadratic totalCoolingCapacityFunctionofFlowFractionCurve(model);
    totalCoolingCapacityFunctionofFlowFractionCurve.setCoefficient1Constant(0.8);
    totalCoolingCapacityFunctionofFlowFractionCurve.setCoefficient2x(0.2);
    totalCoolingCapacityFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0);
    totalCoolingCapacityFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
    totalCoolingCapacityFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

    CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(model);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient1Constant(0.174059889);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient2x(0.022281508);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient3xPOW2(-0.000134077);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient4y(0.028298025);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient5yPOW2(0.00048510);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient6xTIMESY(-0.001677095);
    energyInputRatioFunctionofTemperatureCurve.setMinimumValueofx(-100);
    energyInputRatioFunctionofTemperatureCurve.setMaximumValueofx(100);
    energyInputRatioFunctionofTemperatureCurve.setMinimumValueofy(-100);
    energyInputRatioFunctionofTemperatureCurve.setMaximumValueofy(100);

    CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(model);
    energyInputRatioFunctionofFlowFractionCurve.setCoefficient1Constant(1.1552);
    energyInputRatioFunctionofFlowFractionCurve.setCoefficient2x(-0.1808);
    energyInputRatioFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0256);
    energyInputRatioFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
    energyInputRatioFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

    CurveQuadratic partLoadFractionCorrelationCurve(model);
    partLoadFractionCorrelationCurve.setCoefficient1Constant(0.85);
    partLoadFractionCorrelationCurve.setCoefficient2x(0.15);
    partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
    partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
    partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);

    CoilPerformanceDXCooling coilPerformanceDXCooling(model,
      totalCoolingCapacityFunctionofTemperatureCurve,
      totalCoolingCapacityFunctionofFlowFractionCurve,
      energyInputRatioFunctionofTemperatureCurve,
      energyInputRatioFunctionofFlowFractionCurve,
      partLoadFractionCorrelationCurve);

    coilPerformanceDXCooling.setGrossRatedCoolingCOP(3.0);
    coilPerformanceDXCooling.setFractionofAirFlowBypassedAroundCoil(0.0);

    setNormalModeStage1Plus2CoilPerformance(coilPerformanceDXCooling);
  }

  {
    CurveBiquadratic totalCoolingCapacityFunctionofTemperatureCurve(model);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient1Constant(0.596779741);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient2x(0.034216637);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient3xPOW2(0.000113924);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient4y(-0.00375859);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient5yPOW2(-9.17495E-05);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient6xTIMESY(-8.98373E-05);
    totalCoolingCapacityFunctionofTemperatureCurve.setMinimumValueofx(-100);
    totalCoolingCapacityFunctionofTemperatureCurve.setMaximumValueofx(100);
    totalCoolingCapacityFunctionofTemperatureCurve.setMinimumValueofy(-100);
    totalCoolingCapacityFunctionofTemperatureCurve.setMaximumValueofy(100);

    CurveQuadratic totalCoolingCapacityFunctionofFlowFractionCurve(model);
    totalCoolingCapacityFunctionofFlowFractionCurve.setCoefficient1Constant(0.8);
    totalCoolingCapacityFunctionofFlowFractionCurve.setCoefficient2x(0.2);
    totalCoolingCapacityFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0);
    totalCoolingCapacityFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
    totalCoolingCapacityFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

    CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(model);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient1Constant(0.435347586);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient2x(0.004015641);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient3xPOW2(0.000604235);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient4y(0.015824043);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient5yPOW2(0.000747287);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient6xTIMESY(-0.001779745);
    energyInputRatioFunctionofTemperatureCurve.setMinimumValueofx(-100);
    energyInputRatioFunctionofTemperatureCurve.setMaximumValueofx(100);
    energyInputRatioFunctionofTemperatureCurve.setMinimumValueofy(-100);
    energyInputRatioFunctionofTemperatureCurve.setMaximumValueofy(100);

    CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(model);
    energyInputRatioFunctionofFlowFractionCurve.setCoefficient1Constant(1.1552);
    energyInputRatioFunctionofFlowFractionCurve.setCoefficient2x(-0.1808);
    energyInputRatioFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0256);
    energyInputRatioFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
    energyInputRatioFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

    CurveQuadratic partLoadFractionCorrelationCurve(model);
    partLoadFractionCorrelationCurve.setCoefficient1Constant(0.85);
    partLoadFractionCorrelationCurve.setCoefficient2x(0.15);
    partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
    partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
    partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);


    CoilPerformanceDXCooling coilPerformanceDXCooling(model,
      totalCoolingCapacityFunctionofTemperatureCurve,
      totalCoolingCapacityFunctionofFlowFractionCurve,
      energyInputRatioFunctionofTemperatureCurve,
      energyInputRatioFunctionofFlowFractionCurve,
      partLoadFractionCorrelationCurve);

    coilPerformanceDXCooling.setGrossRatedCoolingCOP(2.7);
    coilPerformanceDXCooling.setFractionofAirFlowBypassedAroundCoil(0.5);

    setDehumidificationMode1Stage1CoilPerformance(coilPerformanceDXCooling);
  }

  {
    CurveBiquadratic totalCoolingCapacityFunctionofTemperatureCurve(model);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient1Constant(0.596779741);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient2x(0.034216637);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient3xPOW2(0.000113924);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient4y(-0.00375859);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient5yPOW2(-9.17495E-05);
    totalCoolingCapacityFunctionofTemperatureCurve.setCoefficient6xTIMESY(-8.98373E-05);
    totalCoolingCapacityFunctionofTemperatureCurve.setMinimumValueofx(-100);
    totalCoolingCapacityFunctionofTemperatureCurve.setMaximumValueofx(100);
    totalCoolingCapacityFunctionofTemperatureCurve.setMinimumValueofy(-100);
    totalCoolingCapacityFunctionofTemperatureCurve.setMaximumValueofy(100);

    CurveQuadratic totalCoolingCapacityFunctionofFlowFractionCurve(model);
    totalCoolingCapacityFunctionofFlowFractionCurve.setCoefficient1Constant(0.8);
    totalCoolingCapacityFunctionofFlowFractionCurve.setCoefficient2x(0.2);
    totalCoolingCapacityFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0);
    totalCoolingCapacityFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
    totalCoolingCapacityFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

    CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(model);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient1Constant(0.435347586);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient2x(0.004015641);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient3xPOW2(0.000604235);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient4y(0.015824043);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient5yPOW2(0.000747287);
    energyInputRatioFunctionofTemperatureCurve.setCoefficient6xTIMESY(-0.001779745);
    energyInputRatioFunctionofTemperatureCurve.setMinimumValueofx(-100);
    energyInputRatioFunctionofTemperatureCurve.setMaximumValueofx(100);
    energyInputRatioFunctionofTemperatureCurve.setMinimumValueofy(-100);
    energyInputRatioFunctionofTemperatureCurve.setMaximumValueofy(100);

    CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(model);
    energyInputRatioFunctionofFlowFractionCurve.setCoefficient1Constant(1.1552);
    energyInputRatioFunctionofFlowFractionCurve.setCoefficient2x(-0.1808);
    energyInputRatioFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0256);
    energyInputRatioFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
    energyInputRatioFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

    CurveQuadratic partLoadFractionCorrelationCurve(model);
    partLoadFractionCorrelationCurve.setCoefficient1Constant(0.85);
    partLoadFractionCorrelationCurve.setCoefficient2x(0.15);
    partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
    partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
    partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);


    CoilPerformanceDXCooling coilPerformanceDXCooling(model,
      totalCoolingCapacityFunctionofTemperatureCurve,
      totalCoolingCapacityFunctionofFlowFractionCurve,
      energyInputRatioFunctionofTemperatureCurve,
      energyInputRatioFunctionofFlowFractionCurve,
      partLoadFractionCorrelationCurve);

    coilPerformanceDXCooling.setGrossRatedCoolingCOP(2.7);
    coilPerformanceDXCooling.setFractionofAirFlowBypassedAroundCoil(0.0);

    setDehumidificationMode1Stage1Plus2CoilPerformance(coilPerformanceDXCooling);
  }
}

IddObjectType CoilCoolingDXTwoStageWithHumidityControlMode::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Cooling_DX_TwoStageWithHumidityControlMode);
}

boost::optional<Schedule> CoilCoolingDXTwoStageWithHumidityControlMode::availabilitySchedule() const {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->availabilitySchedule();
}

double CoilCoolingDXTwoStageWithHumidityControlMode::crankcaseHeaterCapacity() const {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->crankcaseHeaterCapacity();
}

double CoilCoolingDXTwoStageWithHumidityControlMode::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();
}

int CoilCoolingDXTwoStageWithHumidityControlMode::numberofCapacityStages() const {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->numberofCapacityStages();
}

int CoilCoolingDXTwoStageWithHumidityControlMode::numberofEnhancedDehumidificationModes() const {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->numberofEnhancedDehumidificationModes();
}

boost::optional<CoilPerformanceDXCooling> CoilCoolingDXTwoStageWithHumidityControlMode::normalModeStage1CoilPerformance() const {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->normalModeStage1CoilPerformance();
}

boost::optional<CoilPerformanceDXCooling> CoilCoolingDXTwoStageWithHumidityControlMode::normalModeStage1Plus2CoilPerformance() const {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->normalModeStage1Plus2CoilPerformance();
}

boost::optional<CoilPerformanceDXCooling> CoilCoolingDXTwoStageWithHumidityControlMode::dehumidificationMode1Stage1CoilPerformance() const {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->dehumidificationMode1Stage1CoilPerformance();
}

boost::optional<CoilPerformanceDXCooling> CoilCoolingDXTwoStageWithHumidityControlMode::dehumidificationMode1Stage1Plus2CoilPerformance() const {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->dehumidificationMode1Stage1Plus2CoilPerformance();
}

double CoilCoolingDXTwoStageWithHumidityControlMode::basinHeaterCapacity() const {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->basinHeaterCapacity();
}

double CoilCoolingDXTwoStageWithHumidityControlMode::basinHeaterSetpointTemperature() const {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->basinHeaterSetpointTemperature();
}

boost::optional<Schedule> CoilCoolingDXTwoStageWithHumidityControlMode::basinHeaterOperatingSchedule() const {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->basinHeaterOperatingSchedule();
}

bool CoilCoolingDXTwoStageWithHumidityControlMode::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->setAvailabilitySchedule(schedule);
}

void CoilCoolingDXTwoStageWithHumidityControlMode::resetAvailabilitySchedule() {
  getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->resetAvailabilitySchedule();
}

bool CoilCoolingDXTwoStageWithHumidityControlMode::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->setCrankcaseHeaterCapacity(crankcaseHeaterCapacity);
}

bool CoilCoolingDXTwoStageWithHumidityControlMode::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
}

bool CoilCoolingDXTwoStageWithHumidityControlMode::setNumberofCapacityStages(int numberofCapacityStages) {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->setNumberofCapacityStages(numberofCapacityStages);
}

bool CoilCoolingDXTwoStageWithHumidityControlMode::setNumberofEnhancedDehumidificationModes(int numberofEnhancedDehumidificationModes) {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->setNumberofEnhancedDehumidificationModes(numberofEnhancedDehumidificationModes);
}

bool CoilCoolingDXTwoStageWithHumidityControlMode::setNormalModeStage1CoilPerformance(const CoilPerformanceDXCooling& coilPerformanceDX) {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->setNormalModeStage1CoilPerformance(coilPerformanceDX);
}

void CoilCoolingDXTwoStageWithHumidityControlMode::resetNormalModeStage1CoilPerformance() {
  getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->resetNormalModeStage1CoilPerformance();
}

bool CoilCoolingDXTwoStageWithHumidityControlMode::setNormalModeStage1Plus2CoilPerformance(const CoilPerformanceDXCooling& coilPerformanceDX) {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->setNormalModeStage1Plus2CoilPerformance(coilPerformanceDX);
}

void CoilCoolingDXTwoStageWithHumidityControlMode::resetNormalModeStage1Plus2CoilPerformance() {
  getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->resetNormalModeStage1Plus2CoilPerformance();
}

bool CoilCoolingDXTwoStageWithHumidityControlMode::setDehumidificationMode1Stage1CoilPerformance(const CoilPerformanceDXCooling& coilPerformanceDX) {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->setDehumidificationMode1Stage1CoilPerformance(coilPerformanceDX);
}

void CoilCoolingDXTwoStageWithHumidityControlMode::resetDehumidificationMode1Stage1CoilPerformance() {
  getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->resetDehumidificationMode1Stage1CoilPerformance();
}

bool CoilCoolingDXTwoStageWithHumidityControlMode::setDehumidificationMode1Stage1Plus2CoilPerformance(const CoilPerformanceDXCooling& coilPerformanceDX) {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->setDehumidificationMode1Stage1Plus2CoilPerformance(coilPerformanceDX);
}

void CoilCoolingDXTwoStageWithHumidityControlMode::resetDehumidificationMode1Stage1Plus2CoilPerformance() {
  getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->resetDehumidificationMode1Stage1Plus2CoilPerformance();
}

bool CoilCoolingDXTwoStageWithHumidityControlMode::setBasinHeaterCapacity(double basinHeaterCapacity) {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
}

bool CoilCoolingDXTwoStageWithHumidityControlMode::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->setBasinHeaterSetpointTemperature(basinHeaterSetpointTemperature);
}

bool CoilCoolingDXTwoStageWithHumidityControlMode::setBasinHeaterOperatingSchedule(Schedule& schedule) {
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->setBasinHeaterOperatingSchedule(schedule);
}

void CoilCoolingDXTwoStageWithHumidityControlMode::resetBasinHeaterOperatingSchedule() {
  getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->resetBasinHeaterOperatingSchedule();
}

AirflowNetworkEquivalentDuct CoilCoolingDXTwoStageWithHumidityControlMode::getAirflowNetworkEquivalentDuct(double length, double diameter)
{
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->getAirflowNetworkEquivalentDuct(length, diameter);
}

boost::optional<AirflowNetworkEquivalentDuct> CoilCoolingDXTwoStageWithHumidityControlMode::airflowNetworkEquivalentDuct() const
{
  return getImpl<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl>()->airflowNetworkEquivalentDuct();
}

/// @cond
CoilCoolingDXTwoStageWithHumidityControlMode::CoilCoolingDXTwoStageWithHumidityControlMode(std::shared_ptr<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

