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

#include "CoilCoolingDXSingleSpeed.hpp"
#include "CoilCoolingDXSingleSpeed_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"
#include "ZoneHVACPackagedTerminalHeatPump.hpp"
#include "ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
#include "CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "CoilSystemCoolingDXHeatExchangerAssisted_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "Model.hpp"
#include <utilities/idd/OS_Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

namespace detail{

  // constructor
  CoilCoolingDXSingleSpeed_Impl::CoilCoolingDXSingleSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDXSingleSpeed::iddObjectType());
  }

  CoilCoolingDXSingleSpeed_Impl::CoilCoolingDXSingleSpeed_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
        : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingDXSingleSpeed::iddObjectType());
  }

  CoilCoolingDXSingleSpeed_Impl::CoilCoolingDXSingleSpeed_Impl(
      const CoilCoolingDXSingleSpeed_Impl& other,
      Model_Impl* model,
      bool keepHandle)
        : StraightComponent_Impl(other,model,keepHandle)
  {}


  ModelObject CoilCoolingDXSingleSpeed_Impl::clone(Model model) const
  {
    CoilCoolingDXSingleSpeed newCoil = StraightComponent_Impl::clone(model).cast<CoilCoolingDXSingleSpeed>();

    return newCoil;
  }

  std::vector<IddObjectType> CoilCoolingDXSingleSpeed_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_Curve_Biquadratic);
    result.push_back(IddObjectType::OS_Curve_Quadratic);
    result.push_back(IddObjectType::OS_Curve_Cubic);
    return result;
  }

  std::vector<ModelObject> CoilCoolingDXSingleSpeed_Impl::children() const
  {

    std::vector<ModelObject> result;
    if (OptionalCurve intermediate = optionalTotalCoolingCapacityFunctionOfTemperatureCurve()) {
      result.push_back(*intermediate);
    }
    if (OptionalCurve intermediate = optionalTotalCoolingCapacityFunctionOfFlowFractionCurve()) {
      result.push_back(*intermediate);
    }
    if (OptionalCurve intermediate = optionalEnergyInputRatioFunctionOfTemperatureCurve()) {
      result.push_back(*intermediate);
    }
    if (OptionalCurve intermediate = optionalEnergyInputRatioFunctionOfFlowFractionCurve()) {
      result.push_back(*intermediate);
    }
    if (OptionalCurve intermediate = optionalPartLoadFractionCorrelationCurve()) {
      result.push_back(*intermediate);
    }
    return result;
  }
  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& CoilCoolingDXSingleSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilCoolingDXSingleSpeed_Impl::iddObjectType() const {
    return CoilCoolingDXSingleSpeed::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilCoolingDXSingleSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Cooling_DX_SingleSpeedFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilCoolingDXSingleSpeed","Availability"));
    }
    if (std::find(b,e,OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterOperatingScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilCoolingDXSingleSpeed","Basin Heater Operation"));
    }
    return result;
  }

  unsigned CoilCoolingDXSingleSpeed_Impl::inletPort()
  {
    return OS_Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName;
  }

  unsigned CoilCoolingDXSingleSpeed_Impl::outletPort()
  {
    return OS_Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName;
  }

  Schedule CoilCoolingDXSingleSpeed_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<CoilCoolingDXSingleSpeed_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingDXSingleSpeed_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Cooling_DX_SingleSpeedFields::AvailabilityScheduleName,
                              "CoilCoolingDXSingleSpeed",
                              "Availability",
                              schedule);
    return result;
  }

  OptionalDouble CoilCoolingDXSingleSpeed_Impl::ratedCOP() const
  {
    return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedCOP);
  }

  void CoilCoolingDXSingleSpeed_Impl::setRatedCOP( boost::optional<double> value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedCOP,value.get());
  }

  void CoilCoolingDXSingleSpeed_Impl::setRatedCOP( double value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedCOP,value);
  }

  OptionalDouble CoilCoolingDXSingleSpeed_Impl::ratedEvaporatorFanPowerPerVolumeFlowRate()const
  {
    return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate);
  }

  void CoilCoolingDXSingleSpeed_Impl::setRatedEvaporatorFanPowerPerVolumeFlowRate( boost::optional<double> value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate,value.get());
  }

  void CoilCoolingDXSingleSpeed_Impl::setRatedEvaporatorFanPowerPerVolumeFlowRate( double value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate,value);
  }


  Curve CoilCoolingDXSingleSpeed_Impl::totalCoolingCapacityFunctionOfTemperatureCurve()const
  {
    OptionalCurve result = optionalTotalCoolingCapacityFunctionOfTemperatureCurve();
    OS_ASSERT(result);
    return result.get();
  }

  void CoilCoolingDXSingleSpeed_Impl::setTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve )
  {
    if( model() != curve.model() )
    {
      return;
    }

    if( ! curve.optionalCast<CurveBiquadratic>() )
    {
      return;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofTemperatureCurveName, curve.handle()));
  }

  Curve CoilCoolingDXSingleSpeed_Impl::totalCoolingCapacityFunctionOfFlowFractionCurve()const
  {
    OptionalCurve result = optionalTotalCoolingCapacityFunctionOfFlowFractionCurve();
    OS_ASSERT(result);
    return result.get();
  }

  void CoilCoolingDXSingleSpeed_Impl::setTotalCoolingCapacityFunctionOfFlowFractionCurve(const Curve& curve )
  {
    if( model() != curve.model() )
    {
      return;
    }

    bool accepted = false;

    if( curve.optionalCast<CurveQuadratic>() )
    {
      accepted = true;
    }
    else if( curve.optionalCast<CurveCubic>() )
    {
      accepted = true;
    }

    if( ! accepted )
    {
      return;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofFlowFractionCurveName, curve.handle()));
  }

  Curve CoilCoolingDXSingleSpeed_Impl::energyInputRatioFunctionOfTemperatureCurve()const
  {
    OptionalCurve result = optionalEnergyInputRatioFunctionOfTemperatureCurve();
    OS_ASSERT(result);
    return result.get();
  }

  void CoilCoolingDXSingleSpeed_Impl::setEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve )
  {
    if( model() != curve.model() )
    {
      return;
    }

    if( ! curve.optionalCast<Curve>() )
    {
      return;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName, curve.handle()));
  }

  Curve CoilCoolingDXSingleSpeed_Impl::energyInputRatioFunctionOfFlowFractionCurve()const
  {
    OptionalCurve result = optionalEnergyInputRatioFunctionOfFlowFractionCurve();
    OS_ASSERT(result);
    return result.get();
  }

  void CoilCoolingDXSingleSpeed_Impl::setEnergyInputRatioFunctionOfFlowFractionCurve(const Curve& curve )
  {
    if( model() != curve.model() )
    {
      return;
    }

    bool accepted = false;

    if( curve.optionalCast<CurveQuadratic>() )
    {
      accepted = true;
    }
    else if( curve.optionalCast<CurveCubic>() )
    {
      accepted = true;
    }

    if( ! accepted )
    {
      return;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName, curve.handle()));
  }

  Curve CoilCoolingDXSingleSpeed_Impl::partLoadFractionCorrelationCurve() const
  {
    OptionalCurve result = optionalPartLoadFractionCorrelationCurve();
    OS_ASSERT(result);
    return result.get();
  }

  void CoilCoolingDXSingleSpeed_Impl::setPartLoadFractionCorrelationCurve(const Curve& curve )
  {
    if( model() != curve.model() )
    {
      return;
    }

    bool accepted = false;

    if( curve.optionalCast<CurveQuadratic>() )
    {
      accepted = true;
    }
    else if( curve.optionalCast<CurveCubic>() )
    {
      accepted = true;
    }

    if( ! accepted )
    {
      return;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Cooling_DX_SingleSpeedFields::PartLoadFractionCorrelationCurveName, curve.handle()));
  }

  OptionalDouble CoilCoolingDXSingleSpeed_Impl::nominalTimeForCondensateRemovalToBegin() const
  {
    return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::NominalTimeforCondensateRemovaltoBegin);
  }

  void CoilCoolingDXSingleSpeed_Impl::setNominalTimeForCondensateRemovalToBegin( boost::optional<double> value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::NominalTimeforCondensateRemovaltoBegin,value.get());
  }

  void CoilCoolingDXSingleSpeed_Impl::setNominalTimeForCondensateRemovalToBegin( double value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::NominalTimeforCondensateRemovaltoBegin,value);
  }


  OptionalDouble CoilCoolingDXSingleSpeed_Impl::ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity()const
  {
    return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
  }

  void CoilCoolingDXSingleSpeed_Impl::setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity( boost::optional<double> value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity,value.get());
  }

  void CoilCoolingDXSingleSpeed_Impl::setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity( double value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity,value);
  }


  OptionalDouble CoilCoolingDXSingleSpeed_Impl::maximumCyclingRate()
  {
    return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::MaximumCyclingRate);
  }

  void CoilCoolingDXSingleSpeed_Impl::setMaximumCyclingRate( boost::optional<double> value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::MaximumCyclingRate,value.get());
  }

  void CoilCoolingDXSingleSpeed_Impl::setMaximumCyclingRate( double value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::MaximumCyclingRate,value);
  }


  OptionalDouble CoilCoolingDXSingleSpeed_Impl::latentCapacityTimeConstant()const
  {
    return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::LatentCapacityTimeConstant);
  }

  void CoilCoolingDXSingleSpeed_Impl::setLatentCapacityTimeConstant( boost::optional<double> value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::LatentCapacityTimeConstant,value.get());
  }

  void CoilCoolingDXSingleSpeed_Impl::setLatentCapacityTimeConstant( double value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::LatentCapacityTimeConstant,value);
  }


  OptionalString CoilCoolingDXSingleSpeed_Impl::condenserAirInletNodeName()const
  {
    return getString(OS_Coil_Cooling_DX_SingleSpeedFields::CondenserAirInletNodeName);
  }

  void CoilCoolingDXSingleSpeed_Impl::setCondenserAirInletNodeName( const boost::optional<std::string>& value )
  {
    setString(OS_Coil_Cooling_DX_SingleSpeedFields::CondenserAirInletNodeName,value.get());
  }

  std::string CoilCoolingDXSingleSpeed_Impl::condenserType()const
  {
    return getString(OS_Coil_Cooling_DX_SingleSpeedFields::CondenserType).get();
  }

  void CoilCoolingDXSingleSpeed_Impl::setCondenserType( const std::string& value )
  {
    setString(OS_Coil_Cooling_DX_SingleSpeedFields::CondenserType,value);
  }

  OptionalDouble CoilCoolingDXSingleSpeed_Impl::evaporativeCondenserEffectiveness()const
  {
    return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserEffectiveness);
  }

  void CoilCoolingDXSingleSpeed_Impl::setEvaporativeCondenserEffectiveness( boost::optional<double> value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields:: EvaporativeCondenserEffectiveness,value.get());
  }

  void CoilCoolingDXSingleSpeed_Impl::setEvaporativeCondenserEffectiveness( double value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields:: EvaporativeCondenserEffectiveness,value);
  }


  OptionalDouble CoilCoolingDXSingleSpeed_Impl::evaporativeCondenserAirFlowRate()const
  {
    return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate);
  }

  void CoilCoolingDXSingleSpeed_Impl::setEvaporativeCondenserAirFlowRate( OptionalDouble value )
  {
    if(value)
      setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate,*value);
    else
      setString(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate,"Autosize");
  }

  void CoilCoolingDXSingleSpeed_Impl::setEvaporativeCondenserAirFlowRate( double value )
  {
   setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate,value);
  }


  OptionalDouble CoilCoolingDXSingleSpeed_Impl::evaporativeCondenserPumpRatedPowerConsumption()const
  {
    return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption);
  }

  void CoilCoolingDXSingleSpeed_Impl::setEvaporativeCondenserPumpRatedPowerConsumption( OptionalDouble value )
  {
    if(value)
      setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption,*value);
    else
      setString(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption,"Autosize");
  }

  void CoilCoolingDXSingleSpeed_Impl::setEvaporativeCondenserPumpRatedPowerConsumption( double value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption,value);
  }


  OptionalDouble CoilCoolingDXSingleSpeed_Impl::crankcaseHeaterCapacity()const
  {
    return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::CrankcaseHeaterCapacity);
  }

  void CoilCoolingDXSingleSpeed_Impl::setCrankcaseHeaterCapacity( boost::optional<double> value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::CrankcaseHeaterCapacity,value.get());
  }

  void CoilCoolingDXSingleSpeed_Impl::setCrankcaseHeaterCapacity( double value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::CrankcaseHeaterCapacity,value);
  }


  OptionalDouble CoilCoolingDXSingleSpeed_Impl::maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation()const
  {
    return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
  }

  void CoilCoolingDXSingleSpeed_Impl::setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation( boost::optional<double> value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,value.get());
  }

  void CoilCoolingDXSingleSpeed_Impl::setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation( double value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,value);
  }


  //getSupplyWaterStorageTankName
  //setSupplyWaterStorageTankName

  //getCondensateCollectionWaterStorageTankName
  //setCondensateCollectionWaterStorageTankName

  OptionalDouble CoilCoolingDXSingleSpeed_Impl::basinHeaterCapacity()const
  {
    return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterCapacity);
  }

  void CoilCoolingDXSingleSpeed_Impl::setBasinHeaterCapacity( boost::optional<double> value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterCapacity,value.get());
  }

  void CoilCoolingDXSingleSpeed_Impl::setBasinHeaterCapacity( double value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterCapacity,value);
  }


  OptionalDouble CoilCoolingDXSingleSpeed_Impl::basinHeaterSetpointTemperature() const
  {
    return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterSetpointTemperature);
  }

  void CoilCoolingDXSingleSpeed_Impl::setBasinHeaterSetpointTemperature( boost::optional<double> value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterSetpointTemperature,value.get());
  }

  void CoilCoolingDXSingleSpeed_Impl::setBasinHeaterSetpointTemperature( double value )
  {
    setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterSetpointTemperature,value);
  }


  boost::optional<Schedule> CoilCoolingDXSingleSpeed_Impl::basinHeaterOperatingSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterOperatingScheduleName);
  }

  bool CoilCoolingDXSingleSpeed_Impl::setBasinHeaterOperatingSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterOperatingScheduleName,
                              "CoilCoolingDXSingleSpeed",
                              "Basin Heater Operation",
                              schedule);
    return result;
  }

  void CoilCoolingDXSingleSpeed_Impl::resetBasinHeaterOperatingSchedule() {
    bool result = setString(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterOperatingScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<HVACComponent> CoilCoolingDXSingleSpeed_Impl::containingHVACComponent() const
  {
    // AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

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
    std::vector<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass> bypassSystems = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>();

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

    airLoopHVACUnitaryHeatPumpAirToAirs = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

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


    // CoilSystemCoolingDXHeatExchangerAssisted
    {
      auto coilSystems = this->model().getConcreteModelObjects<CoilSystemCoolingDXHeatExchangerAssisted>();
      for( const auto & coilSystem : coilSystems ) {
        if( coilSystem.coolingCoil().handle() == this->handle() ) {
          return coilSystem;
        }
      }
    }

    return boost::none;
  }

  boost::optional<ZoneHVACComponent> CoilCoolingDXSingleSpeed_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACPackagedTerminalAirConditioner

    std::vector<ZoneHVACPackagedTerminalAirConditioner> zoneHVACPackagedTerminalAirConditioners;

    zoneHVACPackagedTerminalAirConditioners = this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalAirConditioner>();

    for( const auto & zoneHVACPackagedTerminalAirConditioner : zoneHVACPackagedTerminalAirConditioners )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACPackagedTerminalAirConditioner.coolingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return zoneHVACPackagedTerminalAirConditioner;
        }
      }
    }

    // ZoneHVACPackagedTerminalHeatPump

    std::vector<ZoneHVACPackagedTerminalHeatPump> zoneHVACPackagedTerminalHeatPumps;

    zoneHVACPackagedTerminalHeatPumps = this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalHeatPump>();

    for( const auto & zoneHVACPackagedTerminalHeatPump : zoneHVACPackagedTerminalHeatPumps )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACPackagedTerminalHeatPump.coolingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return zoneHVACPackagedTerminalHeatPump;
        }
      }
    }

    return boost::none;
  }

  boost::optional<Schedule> CoilCoolingDXSingleSpeed_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DX_SingleSpeedFields::AvailabilityScheduleName);
  }
  boost::optional<Curve> CoilCoolingDXSingleSpeed_Impl::optionalTotalCoolingCapacityFunctionOfTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofTemperatureCurveName);
  }
  boost::optional<Curve> CoilCoolingDXSingleSpeed_Impl::optionalTotalCoolingCapacityFunctionOfFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofFlowFractionCurveName);
  }
  boost::optional<Curve> CoilCoolingDXSingleSpeed_Impl::optionalEnergyInputRatioFunctionOfTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName);
  }
  boost::optional<Curve> CoilCoolingDXSingleSpeed_Impl::optionalEnergyInputRatioFunctionOfFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName);
  }
  boost::optional<Curve> CoilCoolingDXSingleSpeed_Impl::optionalPartLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_SingleSpeedFields::PartLoadFractionCorrelationCurveName);
  }

  boost::optional<ModelObject> CoilCoolingDXSingleSpeed_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  boost::optional<ModelObject> CoilCoolingDXSingleSpeed_Impl::basinHeaterOperatingScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = basinHeaterOperatingSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool CoilCoolingDXSingleSpeed_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  bool CoilCoolingDXSingleSpeed_Impl::setBasinHeaterOperatingScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setBasinHeaterOperatingSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetBasinHeaterOperatingSchedule();
    }
    return true;
  }

  boost::optional<double> CoilCoolingDXSingleSpeed_Impl::ratedTotalCoolingCapacity() const {
    return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedTotalCoolingCapacity,true);
  }

  OSOptionalQuantity CoilCoolingDXSingleSpeed_Impl::getRatedTotalCoolingCapacity(bool returnIP) const {
    OptionalDouble value = ratedTotalCoolingCapacity();
    return getQuantityFromDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedTotalCoolingCapacity, value, returnIP);
  }

  bool CoilCoolingDXSingleSpeed_Impl::isRatedTotalCoolingCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeedFields::RatedTotalCoolingCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  bool CoilCoolingDXSingleSpeed_Impl::setRatedTotalCoolingCapacity(boost::optional<double> ratedTotalCoolingCapacity) {
    bool result(false);
    if (ratedTotalCoolingCapacity) {
      result = setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedTotalCoolingCapacity, ratedTotalCoolingCapacity.get());
    }
    return result;
  }

  bool CoilCoolingDXSingleSpeed_Impl::setRatedTotalCoolingCapacity(const OSOptionalQuantity& ratedTotalCoolingCapacity) {
    bool result(false);
    OptionalDouble value;
    if (ratedTotalCoolingCapacity.isSet()) {
      value = getDoubleFromQuantity(OS_Coil_Cooling_DX_SingleSpeedFields::RatedTotalCoolingCapacity,ratedTotalCoolingCapacity.get());
      if (value) {
        result = setRatedTotalCoolingCapacity(value);
      }
    }
    else {
      result = setRatedTotalCoolingCapacity(value);
    }
    return result;
  }

  void CoilCoolingDXSingleSpeed_Impl::autosizeRatedTotalCoolingCapacity() {
    bool result = setString(OS_Coil_Cooling_DX_SingleSpeedFields::RatedTotalCoolingCapacity, "autosize");
    OS_ASSERT(result);
  }

  boost::optional<double> CoilCoolingDXSingleSpeed_Impl::ratedSensibleHeatRatio() const {
    return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedSensibleHeatRatio,true);
  }

  OSOptionalQuantity CoilCoolingDXSingleSpeed_Impl::getRatedSensibleHeatRatio(bool returnIP) const {
    OptionalDouble value = ratedSensibleHeatRatio();
    return getQuantityFromDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedSensibleHeatRatio, value, returnIP);
  }

  bool CoilCoolingDXSingleSpeed_Impl::isRatedSensibleHeatRatioAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeedFields::RatedSensibleHeatRatio, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  bool CoilCoolingDXSingleSpeed_Impl::setRatedSensibleHeatRatio(boost::optional<double> ratedSensibleHeatRatio) {
    bool result(false);
    if (ratedSensibleHeatRatio) {
      result = setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedSensibleHeatRatio, ratedSensibleHeatRatio.get());
    }
    return result;
  }

  bool CoilCoolingDXSingleSpeed_Impl::setRatedSensibleHeatRatio(const OSOptionalQuantity& ratedSensibleHeatRatio) {
    bool result(false);
    OptionalDouble value;
    if (ratedSensibleHeatRatio.isSet()) {
      value = getDoubleFromQuantity(OS_Coil_Cooling_DX_SingleSpeedFields::RatedSensibleHeatRatio,ratedSensibleHeatRatio.get());
      if (value) {
        result = setRatedSensibleHeatRatio(value);
      }
    }
    else {
      result = setRatedSensibleHeatRatio(value);
    }
    return result;
  }

  void CoilCoolingDXSingleSpeed_Impl::autosizeRatedSensibleHeatRatio() {
    bool result = setString(OS_Coil_Cooling_DX_SingleSpeedFields::RatedSensibleHeatRatio, "autosize");
    OS_ASSERT(result);
  }

  boost::optional<double> CoilCoolingDXSingleSpeed_Impl::ratedAirFlowRate() const {
    return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate,true);
  }

  OSOptionalQuantity CoilCoolingDXSingleSpeed_Impl::getRatedAirFlowRate(bool returnIP) const {
    OptionalDouble value = ratedAirFlowRate();
    return getQuantityFromDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate, value, returnIP);
  }

  bool CoilCoolingDXSingleSpeed_Impl::isRatedAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  bool CoilCoolingDXSingleSpeed_Impl::setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate) {
    bool result(false);
    if (ratedAirFlowRate) {
      result = setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate, ratedAirFlowRate.get());
    }
    return result;
  }

  bool CoilCoolingDXSingleSpeed_Impl::setRatedAirFlowRate(const OSOptionalQuantity& ratedAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (ratedAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate,ratedAirFlowRate.get());
      if (value) {
        result = setRatedAirFlowRate(value);
      }
    }
    else {
      result = setRatedAirFlowRate(value);
    }
    return result;
  }

  void CoilCoolingDXSingleSpeed_Impl::autosizeRatedAirFlowRate() {
    bool result = setString(OS_Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXSingleSpeed_Impl::addToNode(Node & node)
  {
    if( boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC() )
    {
      if( ! airLoop->demandComponent(node.handle()) )
      {
        return StraightComponent_Impl::addToNode( node );
      }
    }

    return false;
  }

}// detail

CoilCoolingDXSingleSpeed::CoilCoolingDXSingleSpeed(const Model& model)
  : StraightComponent(CoilCoolingDXSingleSpeed::iddObjectType(),model)
{
  model::CurveBiquadratic coolingCurveFofTemp = CurveBiquadratic(model);
  coolingCurveFofTemp.setCoefficient1Constant(0.942587793);
  coolingCurveFofTemp.setCoefficient2x(0.009543347);
  coolingCurveFofTemp.setCoefficient3xPOW2(0.000683770);
  coolingCurveFofTemp.setCoefficient4y(-0.011042676);
  coolingCurveFofTemp.setCoefficient5yPOW2(0.000005249);
  coolingCurveFofTemp.setCoefficient6xTIMESY(-0.000009720);
  coolingCurveFofTemp.setMinimumValueofx(17.0);
  coolingCurveFofTemp.setMaximumValueofx(22.0);
  coolingCurveFofTemp.setMinimumValueofy(13.0);
  coolingCurveFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic coolingCurveFofFlow = CurveQuadratic(model);
  coolingCurveFofFlow.setCoefficient1Constant(0.8);
  coolingCurveFofFlow.setCoefficient2x(0.2);
  coolingCurveFofFlow.setCoefficient3xPOW2(0.0);
  coolingCurveFofFlow.setMinimumValueofx(0.5);
  coolingCurveFofFlow.setMaximumValueofx(1.5);

  CurveBiquadratic energyInputRatioFofTemp = CurveBiquadratic(model);
  energyInputRatioFofTemp.setCoefficient1Constant(0.342414409);
  energyInputRatioFofTemp.setCoefficient2x(0.034885008);
  energyInputRatioFofTemp.setCoefficient3xPOW2(-0.000623700);
  energyInputRatioFofTemp.setCoefficient4y(0.004977216);
  energyInputRatioFofTemp.setCoefficient5yPOW2(0.000437951);
  energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.000728028);
  energyInputRatioFofTemp.setMinimumValueofx(17.0);
  energyInputRatioFofTemp.setMaximumValueofx(22.0);
  energyInputRatioFofTemp.setMinimumValueofy(13.0);
  energyInputRatioFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic energyInputRatioFofFlow = CurveQuadratic(model);
  energyInputRatioFofFlow.setCoefficient1Constant(1.1552);
  energyInputRatioFofFlow.setCoefficient2x(-0.1808);
  energyInputRatioFofFlow.setCoefficient3xPOW2(0.0256);
  energyInputRatioFofFlow.setMinimumValueofx(0.5);
  energyInputRatioFofFlow.setMaximumValueofx(1.5);

  CurveQuadratic partLoadFraction = CurveQuadratic(model);
  partLoadFraction.setCoefficient1Constant(0.85);
  partLoadFraction.setCoefficient2x(0.15);
  partLoadFraction.setCoefficient3xPOW2(0.0);
  partLoadFraction.setMinimumValueofx(0.0);
  partLoadFraction.setMaximumValueofx(1.0);

  autosizeRatedTotalCoolingCapacity();//autosize
  autosizeRatedSensibleHeatRatio();//autosize
  autosizeRatedAirFlowRate();//autosize
  setRatedCOP(3.0);
  setRatedEvaporatorFanPowerPerVolumeFlowRate(773.3);
  setTotalCoolingCapacityFunctionOfTemperatureCurve(coolingCurveFofTemp);
  setTotalCoolingCapacityFunctionOfFlowFractionCurve(coolingCurveFofFlow);
  setEnergyInputRatioFunctionOfTemperatureCurve(energyInputRatioFofTemp);
  setEnergyInputRatioFunctionOfFlowFractionCurve(energyInputRatioFofFlow);
  setPartLoadFractionCorrelationCurve(partLoadFraction);
  setEvaporativeCondenserEffectiveness(0.0);
  setEvaporativeCondenserAirFlowRate( boost::none );
  setEvaporativeCondenserPumpRatedPowerConsumption(boost::none);
  setCrankcaseHeaterCapacity(0.0);
  setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(0.0);
  //setSupplyWaterStorageTankName("");
  //setCondensateCollectionWaterStorageTankName("");
  setBasinHeaterCapacity(0.0);
  setBasinHeaterSetpointTemperature(10.0);
  //setBasinHeaterOperatingSchedule(boost::none);
  setString(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterOperatingScheduleName,"");
  setCondenserType("AirCooled");

  auto schedule = model.alwaysOnDiscreteSchedule();
  setAvailabilitySchedule(schedule);
}

// create a new CoilCoolingDXSingleSpeed object in the model's workspace
CoilCoolingDXSingleSpeed::CoilCoolingDXSingleSpeed(const Model& model,
                                                   Schedule& availabilitySchedule,
                                                   const Curve& coolingCurveFofTemp,
                                                   const Curve& coolingCurveFofFlow,
                                                   const Curve& energyInputRatioFofTemp,
                                                   const Curve& energyInputRatioFofFlow,
                                                   const Curve& partLoadFraction)
  : StraightComponent(CoilCoolingDXSingleSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingDXSingleSpeed_Impl>());
  bool ok = setAvailabilitySchedule(availabilitySchedule);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to "
                  << availabilitySchedule.briefDescription() << ".");
  }
  autosizeRatedTotalCoolingCapacity();//autosize
  autosizeRatedSensibleHeatRatio();//autosize
  autosizeRatedAirFlowRate();//autosize
  setRatedCOP(3.0);
  setRatedEvaporatorFanPowerPerVolumeFlowRate(773.3);
  setTotalCoolingCapacityFunctionOfTemperatureCurve(coolingCurveFofTemp);
  setTotalCoolingCapacityFunctionOfFlowFractionCurve(coolingCurveFofFlow);
  setEnergyInputRatioFunctionOfTemperatureCurve(energyInputRatioFofTemp);
  setEnergyInputRatioFunctionOfFlowFractionCurve(energyInputRatioFofFlow);
  setPartLoadFractionCorrelationCurve(partLoadFraction);
  setEvaporativeCondenserEffectiveness(0.0);
  setEvaporativeCondenserAirFlowRate( boost::none );
  setEvaporativeCondenserPumpRatedPowerConsumption(boost::none);
  setCrankcaseHeaterCapacity(0.0);
  setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(0.0);
  //setSupplyWaterStorageTankName("");
  //setCondensateCollectionWaterStorageTankName("");
  setBasinHeaterCapacity(0.0);
  setBasinHeaterSetpointTemperature(10.0);
  //setBasinHeaterOperatingSchedule(boost::none);
  setString(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterOperatingScheduleName,"");
  setCondenserType("AirCooled");
}

CoilCoolingDXSingleSpeed::CoilCoolingDXSingleSpeed(std::shared_ptr<detail::CoilCoolingDXSingleSpeed_Impl> p)
  : StraightComponent(std::move(p))
{}

Schedule CoilCoolingDXSingleSpeed::availabilitySchedule()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->availabilitySchedule();
}

// deprecated
Schedule CoilCoolingDXSingleSpeed::getAvailabilitySchedule()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->availabilitySchedule();
}

bool CoilCoolingDXSingleSpeed::setAvailabilitySchedule(Schedule& schedule)
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setAvailabilitySchedule( schedule );
}

OptionalDouble CoilCoolingDXSingleSpeed::ratedCOP() const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratedCOP();
}

// deprecated
OptionalDouble CoilCoolingDXSingleSpeed::getRatedCOP() const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratedCOP();
}
void CoilCoolingDXSingleSpeed::setRatedCOP( boost::optional<double> value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedCOP( value );
}
void CoilCoolingDXSingleSpeed::setRatedCOP( double value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedCOP( value );
}

OptionalDouble CoilCoolingDXSingleSpeed::ratedEvaporatorFanPowerPerVolumeFlowRate()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratedEvaporatorFanPowerPerVolumeFlowRate();
}
// deprecated
OptionalDouble CoilCoolingDXSingleSpeed::getRatedEvaporatorFanPowerPerVolumeFlowRate()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratedEvaporatorFanPowerPerVolumeFlowRate();
}
void CoilCoolingDXSingleSpeed::setRatedEvaporatorFanPowerPerVolumeFlowRate( boost::optional<double> value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedEvaporatorFanPowerPerVolumeFlowRate( value );
}
void CoilCoolingDXSingleSpeed::setRatedEvaporatorFanPowerPerVolumeFlowRate( double value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedEvaporatorFanPowerPerVolumeFlowRate( value );
}


Curve CoilCoolingDXSingleSpeed::totalCoolingCapacityFunctionOfTemperatureCurve()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->totalCoolingCapacityFunctionOfTemperatureCurve();
}
// deprecated
Curve CoilCoolingDXSingleSpeed::getTotalCoolingCapacityFunctionOfTemperatureCurve()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->totalCoolingCapacityFunctionOfTemperatureCurve();
}
void CoilCoolingDXSingleSpeed::setTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setTotalCoolingCapacityFunctionOfTemperatureCurve( curve );
}

Curve CoilCoolingDXSingleSpeed::totalCoolingCapacityFunctionOfFlowFractionCurve()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->totalCoolingCapacityFunctionOfFlowFractionCurve();
}
// deprecated
Curve CoilCoolingDXSingleSpeed::getTotalCoolingCapacityFunctionOfFlowFractionCurve()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->totalCoolingCapacityFunctionOfFlowFractionCurve();
}
void CoilCoolingDXSingleSpeed::setTotalCoolingCapacityFunctionOfFlowFractionCurve(const Curve& curve )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setTotalCoolingCapacityFunctionOfFlowFractionCurve( curve );
}

Curve CoilCoolingDXSingleSpeed::energyInputRatioFunctionOfTemperatureCurve()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->energyInputRatioFunctionOfTemperatureCurve();
}
// deprecated
Curve CoilCoolingDXSingleSpeed::getEnergyInputRatioFunctionOfTemperatureCurve()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->energyInputRatioFunctionOfTemperatureCurve();
}
void CoilCoolingDXSingleSpeed::setEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setEnergyInputRatioFunctionOfTemperatureCurve( curve );
}

Curve CoilCoolingDXSingleSpeed::energyInputRatioFunctionOfFlowFractionCurve()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->energyInputRatioFunctionOfFlowFractionCurve();
}
// deprecated
Curve CoilCoolingDXSingleSpeed::getEnergyInputRatioFunctionOfFlowFractionCurve()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->energyInputRatioFunctionOfFlowFractionCurve();
}
void CoilCoolingDXSingleSpeed::setEnergyInputRatioFunctionOfFlowFractionCurve(const Curve& curve )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setEnergyInputRatioFunctionOfFlowFractionCurve( curve );
}

Curve CoilCoolingDXSingleSpeed::partLoadFractionCorrelationCurve()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->partLoadFractionCorrelationCurve();
}
// deprecated
Curve CoilCoolingDXSingleSpeed::getPartLoadFractionCorrelationCurve()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->partLoadFractionCorrelationCurve();
}
void CoilCoolingDXSingleSpeed::setPartLoadFractionCorrelationCurve(const Curve& curve )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setPartLoadFractionCorrelationCurve( curve );
}

OptionalDouble CoilCoolingDXSingleSpeed::nominalTimeForCondensateRemovalToBegin() const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->nominalTimeForCondensateRemovalToBegin();
}
// deprecated
OptionalDouble CoilCoolingDXSingleSpeed::getNominalTimeForCondensateRemovalToBegin() const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->nominalTimeForCondensateRemovalToBegin();
}
void CoilCoolingDXSingleSpeed::setNominalTimeForCondensateRemovalToBegin( boost::optional<double> value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setNominalTimeForCondensateRemovalToBegin( value );
}
void CoilCoolingDXSingleSpeed::setNominalTimeForCondensateRemovalToBegin( double value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setNominalTimeForCondensateRemovalToBegin( value );
}


OptionalDouble CoilCoolingDXSingleSpeed::ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity();
}
// deprecated
OptionalDouble CoilCoolingDXSingleSpeed::getRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity();
}
void CoilCoolingDXSingleSpeed::setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity( boost::optional<double> value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity( value );
}
void CoilCoolingDXSingleSpeed::setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity( double value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity( value );
}


OptionalDouble CoilCoolingDXSingleSpeed::maximumCyclingRate()
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->maximumCyclingRate();
}
// deprecated
OptionalDouble CoilCoolingDXSingleSpeed::getMaximumCyclingRate()
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->maximumCyclingRate();
}
void CoilCoolingDXSingleSpeed::setMaximumCyclingRate( boost::optional<double> value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setMaximumCyclingRate( value );
}
void CoilCoolingDXSingleSpeed::setMaximumCyclingRate( double value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setMaximumCyclingRate( value );
}


OptionalDouble CoilCoolingDXSingleSpeed::latentCapacityTimeConstant()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->latentCapacityTimeConstant();
}
// deprecated
OptionalDouble CoilCoolingDXSingleSpeed::getLatentCapacityTimeConstant()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->latentCapacityTimeConstant();
}
void CoilCoolingDXSingleSpeed::setLatentCapacityTimeConstant( boost::optional<double> value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setLatentCapacityTimeConstant( value );
}
void CoilCoolingDXSingleSpeed::setLatentCapacityTimeConstant( double value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setLatentCapacityTimeConstant( value );
}


OptionalString CoilCoolingDXSingleSpeed::condenserAirInletNodeName()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->condenserAirInletNodeName();
}
// deprecated
OptionalString CoilCoolingDXSingleSpeed::getCondenserAirInletNodeName()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->condenserAirInletNodeName();
}
void CoilCoolingDXSingleSpeed::setCondenserAirInletNodeName( const boost::optional<std::string>& value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setCondenserAirInletNodeName( value );
}

std::string CoilCoolingDXSingleSpeed::condenserType()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->condenserType();
}
// deprecated
std::string CoilCoolingDXSingleSpeed::getCondenserType()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->condenserType();
}
void CoilCoolingDXSingleSpeed::setCondenserType( const std::string& value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setCondenserType( value );
}

OptionalDouble CoilCoolingDXSingleSpeed::evaporativeCondenserEffectiveness()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->evaporativeCondenserEffectiveness();
}
// deprecated
OptionalDouble CoilCoolingDXSingleSpeed::getEvaporativeCondenserEffectiveness()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->evaporativeCondenserEffectiveness();
}
void CoilCoolingDXSingleSpeed::setEvaporativeCondenserEffectiveness( boost::optional<double> value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setEvaporativeCondenserEffectiveness( value );
}
void CoilCoolingDXSingleSpeed::setEvaporativeCondenserEffectiveness( double value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setEvaporativeCondenserEffectiveness( value );
}


OptionalDouble CoilCoolingDXSingleSpeed::evaporativeCondenserAirFlowRate()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->evaporativeCondenserAirFlowRate();
}
// deprecated
OptionalDouble CoilCoolingDXSingleSpeed::getEvaporativeCondenserAirFlowRate()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->evaporativeCondenserAirFlowRate();
}
void CoilCoolingDXSingleSpeed::setEvaporativeCondenserAirFlowRate( OptionalDouble value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setEvaporativeCondenserAirFlowRate( value );
}
void CoilCoolingDXSingleSpeed::setEvaporativeCondenserAirFlowRate( double value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setEvaporativeCondenserAirFlowRate( value );
}


OptionalDouble CoilCoolingDXSingleSpeed::evaporativeCondenserPumpRatedPowerConsumption()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->evaporativeCondenserPumpRatedPowerConsumption();
}
// deprecated
OptionalDouble CoilCoolingDXSingleSpeed::getEvaporativeCondenserPumpRatedPowerConsumption()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->evaporativeCondenserPumpRatedPowerConsumption();
}
void CoilCoolingDXSingleSpeed::setEvaporativeCondenserPumpRatedPowerConsumption( OptionalDouble value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setEvaporativeCondenserPumpRatedPowerConsumption( value );
}
void CoilCoolingDXSingleSpeed::setEvaporativeCondenserPumpRatedPowerConsumption( double value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setEvaporativeCondenserPumpRatedPowerConsumption( value );
}


OptionalDouble CoilCoolingDXSingleSpeed::crankcaseHeaterCapacity()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->crankcaseHeaterCapacity();
}
// deprecated
OptionalDouble CoilCoolingDXSingleSpeed::getCrankcaseHeaterCapacity()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->crankcaseHeaterCapacity();
}
void CoilCoolingDXSingleSpeed::setCrankcaseHeaterCapacity( boost::optional<double> value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setCrankcaseHeaterCapacity( value );
}
void CoilCoolingDXSingleSpeed::setCrankcaseHeaterCapacity( double value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setCrankcaseHeaterCapacity( value );
}


OptionalDouble CoilCoolingDXSingleSpeed::maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation();
}
// deprecated
OptionalDouble CoilCoolingDXSingleSpeed::getMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation();
}
void CoilCoolingDXSingleSpeed::setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation( boost::optional<double> value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation( value );
}
void CoilCoolingDXSingleSpeed::setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation( double value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation( value );
}


//getSupplyWaterStorageTankName
//setSupplyWaterStorageTankName

//getCondensateCollectionWaterStorageTankName
//setCondensateCollectionWaterStorageTankName

OptionalDouble CoilCoolingDXSingleSpeed::basinHeaterCapacity()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->basinHeaterCapacity();
}
// deprecated
OptionalDouble CoilCoolingDXSingleSpeed::getBasinHeaterCapacity()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->basinHeaterCapacity();
}
void CoilCoolingDXSingleSpeed::setBasinHeaterCapacity( boost::optional<double> value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setBasinHeaterCapacity( value );
}
void CoilCoolingDXSingleSpeed::setBasinHeaterCapacity( double value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setBasinHeaterCapacity( value );
}


OptionalDouble CoilCoolingDXSingleSpeed::basinHeaterSetpointTemperature()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->basinHeaterSetpointTemperature();
}
// deprecated
OptionalDouble CoilCoolingDXSingleSpeed::getBasinHeaterSetpointTemperature()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->basinHeaterSetpointTemperature();
}
void CoilCoolingDXSingleSpeed::setBasinHeaterSetpointTemperature( boost::optional<double> value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setBasinHeaterSetpointTemperature( value );
}
void CoilCoolingDXSingleSpeed::setBasinHeaterSetpointTemperature( double value )
{
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setBasinHeaterSetpointTemperature( value );
}


OptionalSchedule CoilCoolingDXSingleSpeed::basinHeaterOperatingSchedule()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->basinHeaterOperatingSchedule();
}

// deprecated
OptionalSchedule CoilCoolingDXSingleSpeed::getBasinHeaterOperatingSchedule()const
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->basinHeaterOperatingSchedule();
}

bool CoilCoolingDXSingleSpeed::setBasinHeaterOperatingSchedule(Schedule& schedule)
{
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setBasinHeaterOperatingSchedule( schedule );
}

void CoilCoolingDXSingleSpeed::resetBasinHeaterOperatingSchedule() {
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->resetBasinHeaterOperatingSchedule();
}

IddObjectType CoilCoolingDXSingleSpeed::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Coil_Cooling_DX_SingleSpeed);
  return result;
}

boost::optional<double> CoilCoolingDXSingleSpeed::ratedTotalCoolingCapacity() const {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratedTotalCoolingCapacity();
}

OSOptionalQuantity CoilCoolingDXSingleSpeed::getRatedTotalCoolingCapacity(bool returnIP) const {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->getRatedTotalCoolingCapacity(returnIP);
}

bool CoilCoolingDXSingleSpeed::isRatedTotalCoolingCapacityAutosized() const {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->isRatedTotalCoolingCapacityAutosized();
}

bool CoilCoolingDXSingleSpeed::setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity) {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedTotalCoolingCapacity(ratedTotalCoolingCapacity);
}

bool CoilCoolingDXSingleSpeed::setRatedTotalCoolingCapacity(const Quantity& ratedTotalCoolingCapacity) {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedTotalCoolingCapacity(ratedTotalCoolingCapacity);
}

void CoilCoolingDXSingleSpeed::autosizeRatedTotalCoolingCapacity() {
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizeRatedTotalCoolingCapacity();
}

boost::optional<double> CoilCoolingDXSingleSpeed::ratedSensibleHeatRatio() const {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratedSensibleHeatRatio();
}

OSOptionalQuantity CoilCoolingDXSingleSpeed::getRatedSensibleHeatRatio(bool returnIP) const {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->getRatedSensibleHeatRatio(returnIP);
}

bool CoilCoolingDXSingleSpeed::isRatedSensibleHeatRatioAutosized() const {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->isRatedSensibleHeatRatioAutosized();
}

bool CoilCoolingDXSingleSpeed::setRatedSensibleHeatRatio(double ratedSensibleHeatRatio) {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedSensibleHeatRatio(ratedSensibleHeatRatio);
}

bool CoilCoolingDXSingleSpeed::setRatedSensibleHeatRatio(const Quantity& ratedSensibleHeatRatio) {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedSensibleHeatRatio(ratedSensibleHeatRatio);
}

void CoilCoolingDXSingleSpeed::autosizeRatedSensibleHeatRatio() {
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizeRatedSensibleHeatRatio();
}

boost::optional<double> CoilCoolingDXSingleSpeed::ratedAirFlowRate() const {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratedAirFlowRate();
}

OSOptionalQuantity CoilCoolingDXSingleSpeed::getRatedAirFlowRate(bool returnIP) const {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->getRatedAirFlowRate(returnIP);
}

bool CoilCoolingDXSingleSpeed::isRatedAirFlowRateAutosized() const {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->isRatedAirFlowRateAutosized();
}

bool CoilCoolingDXSingleSpeed::setRatedAirFlowRate(double ratedAirFlowRate) {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedAirFlowRate(ratedAirFlowRate);
}

bool CoilCoolingDXSingleSpeed::setRatedAirFlowRate(const Quantity& ratedAirFlowRate) {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedAirFlowRate(ratedAirFlowRate);
}

void CoilCoolingDXSingleSpeed::autosizeRatedAirFlowRate() {
  getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizeRatedAirFlowRate();
}

} // model
} // openstudio
