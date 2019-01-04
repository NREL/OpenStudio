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

#include "CoilCoolingDXTwoSpeed.hpp"
#include "CoilCoolingDXTwoSpeed_Impl.hpp"
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
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "Model.hpp"
#include <utilities/idd/OS_Coil_Cooling_DX_TwoSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail{

  // constructor
  CoilCoolingDXTwoSpeed_Impl::CoilCoolingDXTwoSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDXTwoSpeed::iddObjectType());
  }

  CoilCoolingDXTwoSpeed_Impl::CoilCoolingDXTwoSpeed_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
        : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingDXTwoSpeed::iddObjectType());
  }

  CoilCoolingDXTwoSpeed_Impl::CoilCoolingDXTwoSpeed_Impl(
      const CoilCoolingDXTwoSpeed_Impl& other,
      Model_Impl* model,
      bool keepHandle)
        : StraightComponent_Impl(other,model,keepHandle)
  {}

  ModelObject CoilCoolingDXTwoSpeed_Impl::clone(Model model) const
  {
    CoilCoolingDXTwoSpeed newCoil = StraightComponent_Impl::clone(model).cast<CoilCoolingDXTwoSpeed>();

    return newCoil;
  }

  std::vector<IddObjectType> CoilCoolingDXTwoSpeed_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_Curve_Biquadratic);
    result.push_back(IddObjectType::OS_Curve_Quadratic);
    result.push_back(IddObjectType::OS_Curve_Cubic);
    return result;
  }

  std::vector<ModelObject> CoilCoolingDXTwoSpeed_Impl::children() const
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
    if (OptionalCurve intermediate = optionalLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve()) {
      result.push_back(*intermediate);
    }
    if (OptionalCurve intermediate = optionalLowSpeedEnergyInputRatioFunctionOfTemperatureCurve()) {
      result.push_back(*intermediate);
    }

    return result;
  }

  const std::vector<std::string>& CoilCoolingDXTwoSpeed_Impl::outputVariableNames() const
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
        "Cooling Coil Evaporative Condenser Mains Supply Water Volume"
      // }

      // Storage tank isn't implemented
      // if has storage tank:
      // "Cooling Coil Condensate Volume Flow Rate",
      // "Cooling Coil Condensate Volume",
      //

      // If not part of AirLoopHVAC:UnitaryHeatPump:AirToAir
      // (if part of a heat pump, crankcase heater is reported only for the heating coil):
      // if ( !this->containingHVACComponent().empty() ) {
      // "Cooling Coil Crankcase Heater Electric Power",
      // "Cooling Coil Crankcase Heater Electric Energy",
      // }

    };
    return result;
  }

  IddObjectType CoilCoolingDXTwoSpeed_Impl::iddObjectType() const {
    return CoilCoolingDXTwoSpeed::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilCoolingDXTwoSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Cooling_DX_TwoSpeedFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilCoolingDXTwoSpeed","Availability"));
    }
    if (std::find(b,e,OS_Coil_Cooling_DX_TwoSpeedFields::BasinHeaterOperatingScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilCoolingDXTwoSpeed","Basin Heater Operation"));
    }
    return result;
  }

  Schedule CoilCoolingDXTwoSpeed_Impl::availabilitySchedule()const
  {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<CoilCoolingDXTwoSpeed_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingDXTwoSpeed_Impl::setAvailabilitySchedule(Schedule& schedule )
  {
    bool result = setSchedule(OS_Coil_Cooling_DX_TwoSpeedFields::AvailabilityScheduleName,
                              "CoilCoolingDXTwoSpeed",
                              "Availability",
                              schedule);
    return result;
  }

  // N1 , \field Rated High Speed Total Cooling Capacity
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::ratedHighSpeedTotalCoolingCapacity() const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedTotalCoolingCapacity);
  }

  bool CoilCoolingDXTwoSpeed_Impl::setRatedHighSpeedTotalCoolingCapacity( OptionalDouble value )
  {
    if(value)
    {
      return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedTotalCoolingCapacity,*value);
    }
    else
    {
      return setString(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedTotalCoolingCapacity,"Autosize");
    }
  }

  bool CoilCoolingDXTwoSpeed_Impl::setRatedHighSpeedTotalCoolingCapacity( double value )
  {
      return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedTotalCoolingCapacity,value);
  }


  // N2 , \field Rated High Speed Sensible Heat Ratio
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::ratedHighSpeedSensibleHeatRatio() const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedSensibleHeatRatio);
  }
  bool CoilCoolingDXTwoSpeed_Impl::setRatedHighSpeedSensibleHeatRatio( OptionalDouble value )
  {
    if(value)
    {
      return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedSensibleHeatRatio,*value);
    }
    else
    {
      return setString(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedSensibleHeatRatio,"Autosize");
    }
  }

  bool CoilCoolingDXTwoSpeed_Impl::setRatedHighSpeedSensibleHeatRatio( double value )
  {
    return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedSensibleHeatRatio,value);
  }


  // N3 , \field Rated High Speed COP
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::ratedHighSpeedCOP() const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedCOP);
  }

  bool CoilCoolingDXTwoSpeed_Impl::setRatedHighSpeedCOP( double value )
  {
    return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedCOP,value);
  }

  // N4 , \field Rated High Speed Air Flow Rate
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::ratedHighSpeedAirFlowRate() const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedAirFlowRate);
  }
  bool CoilCoolingDXTwoSpeed_Impl::setRatedHighSpeedAirFlowRate( OptionalDouble value )
  {
    if(value)
    {
      return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedAirFlowRate,*value);
    }
    else
    {
      return setString(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedAirFlowRate,"Autosize");
    }
  }
  bool CoilCoolingDXTwoSpeed_Impl::setRatedHighSpeedAirFlowRate( double value )
  {
    return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedAirFlowRate,value);
  }


  // A5 , \field Total Cooling Capacity Function of Temperature Curve Name
  //\object-list BiquadraticCurves
  Curve CoilCoolingDXTwoSpeed_Impl::totalCoolingCapacityFunctionOfTemperatureCurve()const
  {
    OptionalCurve result = optionalTotalCoolingCapacityFunctionOfTemperatureCurve();
    OS_ASSERT(result);
    return result.get();
  }
  bool CoilCoolingDXTwoSpeed_Impl::setTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve )
  {
    bool accepted = false;

    if( model() != curve.model() )
    {
      return accepted;
    }

    if( curve.optionalCast<CurveBiquadratic>() )
    {
      accepted = true;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Cooling_DX_TwoSpeedFields::TotalCoolingCapacityFunctionofTemperatureCurveName, curve.handle()));

    return accepted;
  }

  // A6 , \field Total Cooling Capacity Function of Flow Fraction Curve Name
  //\object-list QuadraticCubicCurves
  Curve CoilCoolingDXTwoSpeed_Impl::totalCoolingCapacityFunctionOfFlowFractionCurve()const
  {
    OptionalCurve result = optionalTotalCoolingCapacityFunctionOfFlowFractionCurve();
    OS_ASSERT(result);
    return result.get();
  }
  bool CoilCoolingDXTwoSpeed_Impl::setTotalCoolingCapacityFunctionOfFlowFractionCurve(const Curve& curve )
  {
    bool accepted = false;

    if( model() != curve.model() )
    {
      return accepted;
    }

    if( curve.optionalCast<CurveQuadratic>() )
    {
      accepted = true;
    }
    else if( curve.optionalCast<CurveCubic>() )
    {
      accepted = true;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Cooling_DX_TwoSpeedFields::TotalCoolingCapacityFunctionofFlowFractionCurveName, curve.handle()));

    return accepted;
  }

  // A7 , \field Energy Input Ratio Function of Temperature Curve Name
  //\object-list BiquadraticCurves
  Curve CoilCoolingDXTwoSpeed_Impl::energyInputRatioFunctionOfTemperatureCurve()const
  {
    OptionalCurve result = optionalEnergyInputRatioFunctionOfTemperatureCurve();
    OS_ASSERT(result);
    return result.get();
  }
  bool CoilCoolingDXTwoSpeed_Impl::setEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve )
  {
    bool accepted = false;

    if( model() != curve.model() )
    {
      return accepted;
    }

    if( curve.optionalCast<CurveBiquadratic>() )
    {
      accepted = true;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Cooling_DX_TwoSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName, curve.handle()));

    return accepted;
  }

  // A8 , \field Energy Input Ratio Function of Flow Fraction Curve Name
  //\object-list QuadraticCubicCurves
  Curve CoilCoolingDXTwoSpeed_Impl::energyInputRatioFunctionOfFlowFractionCurve()const
  {
    OptionalCurve result = optionalEnergyInputRatioFunctionOfFlowFractionCurve();
    OS_ASSERT(result);
    return result.get();
  }
  bool CoilCoolingDXTwoSpeed_Impl::setEnergyInputRatioFunctionOfFlowFractionCurve(const Curve& curve )
  {
    bool accepted = false;

    if( model() != curve.model() )
    {
      return accepted;
    }

    if( curve.optionalCast<CurveQuadratic>() )
    {
      accepted = true;
    }
    else if( curve.optionalCast<CurveCubic>() )
    {
      accepted = true;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Cooling_DX_TwoSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName, curve.handle()));

    return accepted;
  }

  // A9 , \field Part Load Fraction Correlation Curve Name
  //\object-list QuadraticCubicCurves
  Curve CoilCoolingDXTwoSpeed_Impl::partLoadFractionCorrelationCurve() const
  {
    OptionalCurve result = optionalPartLoadFractionCorrelationCurve();
    OS_ASSERT(result);
    return result.get();
  }
  bool CoilCoolingDXTwoSpeed_Impl::setPartLoadFractionCorrelationCurve(const Curve& curve )
  {
    bool accepted = false;
    if( model() != curve.model() )
    {
      return accepted;
    }

    if( curve.optionalCast<CurveQuadratic>() )
    {
      accepted = true;
    }
    else if( curve.optionalCast<CurveCubic>() )
    {
      accepted = true;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Cooling_DX_TwoSpeedFields::PartLoadFractionCorrelationCurveName, curve.handle()));

    return accepted;
  }

  // N5 , \field Rated Low Speed Total Cooling Capacity
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::ratedLowSpeedTotalCoolingCapacity() const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedTotalCoolingCapacity);
  }
  bool CoilCoolingDXTwoSpeed_Impl::setRatedLowSpeedTotalCoolingCapacity( OptionalDouble value )
  {
    if(value)
    {
      return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedTotalCoolingCapacity,*value);
    }
    else
    {
      return setString(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedTotalCoolingCapacity,"Autosize");
    }
  }
  bool CoilCoolingDXTwoSpeed_Impl::setRatedLowSpeedTotalCoolingCapacity( double value )
  {
    return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedTotalCoolingCapacity,value);
  }


  // N6 , \field Rated Low Speed Sensible Heat Ratio
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::ratedLowSpeedSensibleHeatRatio() const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedSensibleHeatRatio);
  }
  bool CoilCoolingDXTwoSpeed_Impl::setRatedLowSpeedSensibleHeatRatio( OptionalDouble value )
  {
    if(value)
    {
      return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedSensibleHeatRatio,*value);
    }
    else
    {
      return setString(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedSensibleHeatRatio,"Autosize");
    }
  }
  bool CoilCoolingDXTwoSpeed_Impl::setRatedLowSpeedSensibleHeatRatio( double value )
  {
    return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedSensibleHeatRatio,value);
  }


  // N7 , \field Rated Low Speed COP
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::ratedLowSpeedCOP() const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedCOP);
  }
  bool CoilCoolingDXTwoSpeed_Impl::setRatedLowSpeedCOP( double value )
  {
    return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedCOP,value);
  }

  // N8 , \field Rated Low Speed Air Flow Rate
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::ratedLowSpeedAirFlowRate() const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedAirFlowRate);
  }
  bool CoilCoolingDXTwoSpeed_Impl::setRatedLowSpeedAirFlowRate( OptionalDouble value )
  {
    if(value)
    {
      return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedAirFlowRate,*value);
    }
    else
    {
      return setString(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedAirFlowRate,"Autosize");
    }
  }
  bool CoilCoolingDXTwoSpeed_Impl::setRatedLowSpeedAirFlowRate( double value )
  {
    return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedAirFlowRate,value);
  }


  // A10, \field Low Speed Total Cooling Capacity Function of Temperature Curve Name
  //\object-list BiquadraticCurves
  Curve CoilCoolingDXTwoSpeed_Impl::lowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve()const
  {
    OptionalCurve result = optionalLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve();
    OS_ASSERT(result);
    return result.get();
  }
  bool CoilCoolingDXTwoSpeed_Impl::setLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve )
  {
    bool accepted = false;

    if( model() != curve.model() )
    {
      return accepted;
    }

    if( curve.optionalCast<CurveBiquadratic>() )
    {
      accepted = true;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedTotalCoolingCapacityFunctionofTemperatureCurveName, curve.handle()));

    return accepted;
  }

  // A11, \field Low Speed Energy Input Ratio Function of Temperature Curve Name
  //\object-list BiquadraticCurves
  Curve CoilCoolingDXTwoSpeed_Impl::lowSpeedEnergyInputRatioFunctionOfTemperatureCurve()const
  {
    OptionalCurve result = optionalLowSpeedEnergyInputRatioFunctionOfTemperatureCurve();
    OS_ASSERT(result);
    return result.get();
  }
  bool CoilCoolingDXTwoSpeed_Impl::setLowSpeedEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve )
  {
    bool accepted = false;

    if( model() != curve.model() )
    {
      return accepted;
    }

    if( curve.optionalCast<CurveBiquadratic>() )
    {
      accepted = true;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEnergyInputRatioFunctionofTemperatureCurveName, curve.handle()));

    return accepted;
  }

  // A12, \field Condenser Air Inlet Node Name
  OptionalString CoilCoolingDXTwoSpeed_Impl::condenserAirInletNodeName()const
  {
    return getString(OS_Coil_Cooling_DX_TwoSpeedFields::CondenserAirInletNodeName);
  }
  bool CoilCoolingDXTwoSpeed_Impl::setCondenserAirInletNodeName( const std::string& value )
  {
    return setString(OS_Coil_Cooling_DX_TwoSpeedFields::CondenserAirInletNodeName,value);
  }

  // A13, \field Condenser Type
  std::string CoilCoolingDXTwoSpeed_Impl::condenserType()const
  {
    return getString(OS_Coil_Cooling_DX_TwoSpeedFields::CondenserType).get();
  }
  bool CoilCoolingDXTwoSpeed_Impl::setCondenserType( const std::string& value )
  {
    return setString(OS_Coil_Cooling_DX_TwoSpeedFields::CondenserType,value);
  }

  // N9, \field High Speed Evaporative Condenser Effectiveness
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::highSpeedEvaporativeCondenserEffectiveness()const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserEffectiveness);
  }
  bool CoilCoolingDXTwoSpeed_Impl::setHighSpeedEvaporativeCondenserEffectiveness( double value )
  {
    return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserEffectiveness,value);
  }

  // N10, \field High Speed Evaporative Condenser Air Flow Rate
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::highSpeedEvaporativeCondenserAirFlowRate()const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserAirFlowRate);
  }
  bool CoilCoolingDXTwoSpeed_Impl::setHighSpeedEvaporativeCondenserAirFlowRate( OptionalDouble value )
  {
    if(value)
      return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserAirFlowRate,*value);
    else
      return setString(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserAirFlowRate,"Autosize");
  }
  bool CoilCoolingDXTwoSpeed_Impl::setHighSpeedEvaporativeCondenserAirFlowRate( double value )
  {
    return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserAirFlowRate,value);
  }


  // N11, \field High Speed Evaporative Condenser Pump Rated Power Consumption
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::highSpeedEvaporativeCondenserPumpRatedPowerConsumption()const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserPumpRatedPowerConsumption);
  }
  bool CoilCoolingDXTwoSpeed_Impl::setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption( OptionalDouble value )
  {
    if(value)
      return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserPumpRatedPowerConsumption,*value);
    else
      return setString(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserPumpRatedPowerConsumption,"Autosize");
  }
  bool CoilCoolingDXTwoSpeed_Impl::setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption( double value )
  {
    return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserPumpRatedPowerConsumption,value);
  }


  // N12, \field Low Speed Evaporative Condenser Effectiveness
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::lowSpeedEvaporativeCondenserEffectiveness()const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserEffectiveness);
  }
  bool CoilCoolingDXTwoSpeed_Impl::setLowSpeedEvaporativeCondenserEffectiveness( double value )
  {
    return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserEffectiveness,value);
  }

  // N13, \field Low Speed Evaporative Condenser Air Flow Rate
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::lowSpeedEvaporativeCondenserAirFlowRate()const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserAirFlowRate);
  }
  bool CoilCoolingDXTwoSpeed_Impl::setLowSpeedEvaporativeCondenserAirFlowRate( OptionalDouble value )
  {
    if(value)
      return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserAirFlowRate,*value);
    else
      return setString(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserAirFlowRate,"Autosize");
  }
  bool CoilCoolingDXTwoSpeed_Impl::setLowSpeedEvaporativeCondenserAirFlowRate( double value )
  {
    return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserAirFlowRate,value);
  }


  // N14, \field Low Speed Evaporative Condenser Pump Rated Power Consumption
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::lowSpeedEvaporativeCondenserPumpRatedPowerConsumption()const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserPumpRatedPowerConsumption);
  }
  bool CoilCoolingDXTwoSpeed_Impl::setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption( OptionalDouble value )
  {
    if(value)
      return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserPumpRatedPowerConsumption,*value);
    else
      return setString(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserPumpRatedPowerConsumption,"Autosize");
  }
  bool CoilCoolingDXTwoSpeed_Impl::setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption( double value )
  {
    return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserPumpRatedPowerConsumption,value);
  }


  //TODO
  // A14, \field Supply Water Storage Tank Name
  // getSupplyWaterStorageTankName
  // setSupplyWaterStorageTankName

  //TODO
  // A15, \field Condensate Collection Water Storage Tank Name
  // getCondensateCollectionWaterStorageTankName
  // setCondensateCollectionWaterStorageTankName

  // N15, \field Basin Heater Capacity
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::basinHeaterCapacity()const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::BasinHeaterCapacity);
  }
  bool CoilCoolingDXTwoSpeed_Impl::setBasinHeaterCapacity( double value )
  {
    return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::BasinHeaterCapacity,value);
  }

  // N16, \field Basin Heater Setpoint Temperature
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::basinHeaterSetpointTemperature() const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::BasinHeaterSetpointTemperature);
  }
  bool CoilCoolingDXTwoSpeed_Impl::setBasinHeaterSetpointTemperature( double value )
  {
    return setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::BasinHeaterSetpointTemperature,value);
  }

  // A16; \field Basin Heater Operating Schedule Name
  boost::optional<Schedule> CoilCoolingDXTwoSpeed_Impl::basinHeaterOperatingSchedule() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DX_TwoSpeedFields::BasinHeaterOperatingScheduleName);
  }
  bool CoilCoolingDXTwoSpeed_Impl::setBasinHeaterOperatingSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Cooling_DX_TwoSpeedFields::BasinHeaterOperatingScheduleName,
                              "CoilCoolingDXTwoSpeed",
                              "Basin Heater Operation",
                              schedule);
    return result;
  }

  void CoilCoolingDXTwoSpeed_Impl::resetBasinHeaterOperatingSchedule() {
    bool result = setString(OS_Coil_Cooling_DX_TwoSpeedFields::BasinHeaterOperatingScheduleName, "");
    OS_ASSERT(result);
  }

  // A3 , \field Air Inlet Node Name
  unsigned CoilCoolingDXTwoSpeed_Impl::inletPort() const
  {
    return OS_Coil_Cooling_DX_TwoSpeedFields::AirInletNodeName;
  }

  // A4 , \field Air Outlet Node Name
  unsigned CoilCoolingDXTwoSpeed_Impl::outletPort() const
  {
    return OS_Coil_Cooling_DX_TwoSpeedFields::AirOutletNodeName;
  }

  boost::optional<Schedule> CoilCoolingDXTwoSpeed_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DX_TwoSpeedFields::AvailabilityScheduleName);
  }
  boost::optional<Curve> CoilCoolingDXTwoSpeed_Impl::optionalTotalCoolingCapacityFunctionOfTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_TwoSpeedFields::TotalCoolingCapacityFunctionofTemperatureCurveName);
  }
  boost::optional<Curve> CoilCoolingDXTwoSpeed_Impl::optionalTotalCoolingCapacityFunctionOfFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_TwoSpeedFields::TotalCoolingCapacityFunctionofFlowFractionCurveName);
  }
  boost::optional<Curve> CoilCoolingDXTwoSpeed_Impl::optionalEnergyInputRatioFunctionOfTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_TwoSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName);
  }
  boost::optional<Curve> CoilCoolingDXTwoSpeed_Impl::optionalEnergyInputRatioFunctionOfFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_TwoSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName);
  }
  boost::optional<Curve> CoilCoolingDXTwoSpeed_Impl::optionalPartLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_TwoSpeedFields::PartLoadFractionCorrelationCurveName);
  }
  boost::optional<Curve> CoilCoolingDXTwoSpeed_Impl::optionalLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedTotalCoolingCapacityFunctionofTemperatureCurveName);
  }
  boost::optional<Curve> CoilCoolingDXTwoSpeed_Impl::optionalLowSpeedEnergyInputRatioFunctionOfTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEnergyInputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<ModelObject> CoilCoolingDXTwoSpeed_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  boost::optional<ModelObject> CoilCoolingDXTwoSpeed_Impl::basinHeaterOperatingScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = basinHeaterOperatingSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool CoilCoolingDXTwoSpeed_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  bool CoilCoolingDXTwoSpeed_Impl::setBasinHeaterOperatingScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

  bool CoilCoolingDXTwoSpeed_Impl::addToNode(Node & node)
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

  boost::optional<HVACComponent> CoilCoolingDXTwoSpeed_Impl::containingHVACComponent() const
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
    return boost::none;
  }

  boost::optional<double> CoilCoolingDXTwoSpeed_Impl::autosizedRatedHighSpeedTotalCoolingCapacity() const {
    return getAutosizedValue("Design Size High Speed Gross Rated Total Cooling Capacity", "W");
  }

  boost::optional<double> CoilCoolingDXTwoSpeed_Impl::autosizedRatedHighSpeedSensibleHeatRatio() const {
    return getAutosizedValue("Design Size High Speed Rated Sensible Heat Ratio", "");
  }

  boost::optional<double> CoilCoolingDXTwoSpeed_Impl::autosizedRatedHighSpeedAirFlowRate() const {
    return getAutosizedValue("Design Size High Speed Rated Air Flow Rate", "m3/s");
  }

  boost::optional<double> CoilCoolingDXTwoSpeed_Impl::autosizedRatedLowSpeedTotalCoolingCapacity() const {
    return getAutosizedValue("Design Size Low Speed Gross Rated Total Cooling Capacity", "W");
  }

  boost::optional<double> CoilCoolingDXTwoSpeed_Impl::autosizedRatedLowSpeedSensibleHeatRatio() const {
    return getAutosizedValue("Design Size Low Speed Gross Rated Sensible Heat Ratio", "");
  }

  boost::optional<double> CoilCoolingDXTwoSpeed_Impl::autosizedRatedLowSpeedAirFlowRate() const {
    return getAutosizedValue("Design Size Low Speed Rated Air Flow Rate", "m3/s");
  }

  // E+ output has a bug where this field label is incorrect
  boost::optional<double> CoilCoolingDXTwoSpeed_Impl::autosizedHighSpeedEvaporativeCondenserAirFlowRate() const {
    return getAutosizedValue("Design Size High Speed Evaporative Condenser Effectiveness", "m3/s");
  }

  // E+ output has a bug where this field label is incorrect
  boost::optional<double> CoilCoolingDXTwoSpeed_Impl::autosizedHighSpeedEvaporativeCondenserPumpRatedPowerConsumption() const {
    return getAutosizedValue("Design Size High Speed Evaporative Condenser Air Flow Rate", "W");
  }

  // E+ output has a bug where this field label is incorrect
  boost::optional<double> CoilCoolingDXTwoSpeed_Impl::autosizedLowSpeedEvaporativeCondenserAirFlowRate() const {
    return getAutosizedValue("Design Size Low Speed Evaporative Condenser Effectiveness", "m3/s");
  }

  // E+ output has a bug where this field label is incorrect
  boost::optional<double> CoilCoolingDXTwoSpeed_Impl::autosizedLowSpeedEvaporativeCondenserPumpRatedPowerConsumption() const {
    return getAutosizedValue("Design Size Low Speed Evaporative Condenser Air Flow Rate", "W");
  }

  void CoilCoolingDXTwoSpeed_Impl::autosize() {
    boost::optional<double> emptyVal;
    setRatedHighSpeedTotalCoolingCapacity(emptyVal);
    setRatedHighSpeedSensibleHeatRatio(emptyVal);
    setRatedHighSpeedAirFlowRate(emptyVal);
    setRatedLowSpeedTotalCoolingCapacity(emptyVal);
    setRatedLowSpeedSensibleHeatRatio(emptyVal);
    setRatedLowSpeedAirFlowRate(emptyVal);
    setHighSpeedEvaporativeCondenserAirFlowRate(emptyVal);
    setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption(emptyVal);
    setLowSpeedEvaporativeCondenserAirFlowRate(emptyVal);
    setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption(emptyVal);
  }

  void CoilCoolingDXTwoSpeed_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedRatedHighSpeedTotalCoolingCapacity();
    if (val) {
      setRatedHighSpeedTotalCoolingCapacity(val.get());
    }

    val = autosizedRatedHighSpeedSensibleHeatRatio();
    if (val) {
      setRatedHighSpeedSensibleHeatRatio(val.get());
    }

    val = autosizedRatedHighSpeedAirFlowRate();
    if (val) {
      setRatedHighSpeedAirFlowRate(val.get());
    }

    val = autosizedRatedLowSpeedTotalCoolingCapacity();
    if (val) {
      setRatedLowSpeedTotalCoolingCapacity(val.get());
    }

    val = autosizedRatedLowSpeedSensibleHeatRatio();
    if (val) {
      setRatedLowSpeedSensibleHeatRatio(val.get());
    }

    val = autosizedRatedLowSpeedAirFlowRate();
    if (val) {
      setRatedLowSpeedAirFlowRate(val.get());
    }

    val = autosizedHighSpeedEvaporativeCondenserAirFlowRate();
    if (val) {
      setHighSpeedEvaporativeCondenserAirFlowRate(val.get());
    }

    val = autosizedHighSpeedEvaporativeCondenserPumpRatedPowerConsumption();
    if (val) {
      setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption(val.get());
    }

    val = autosizedLowSpeedEvaporativeCondenserAirFlowRate();
    if (val) {
      setLowSpeedEvaporativeCondenserAirFlowRate(val.get());
    }

    val = autosizedLowSpeedEvaporativeCondenserPumpRatedPowerConsumption();
    if (val) {
      setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption(val.get());
    }

  }

}// detail



//This section defines the default values the object gets when created.

//Coil:Cooling:DX:TwoSpeed,
//Main Cooling Coil 1, !- Name
//CoolingCoilAvailSched, !- Availability Schedule Name
//autosize, !- Rated High Speed Total Cooling Capacity {W}
//0.68, !- Rated High Speed Sensible Heat Ratio
//3.0, !- Rated High Speed COP
//autosize, !- Rated High Speed Air Flow Rate {m3/s}
//Mixed Air Node 1, !- Air Inlet Node Name
//Main Cooling Coil 1 Outlet Node, !- Air Outlet Node Name
//VarSpeedCoolCapFT, !- Total Cooling Capacity Function of Temperature Curve Name
//PackagedRatedCoolCapFFlow, !- Total Cooling Capacity Function of Flow Fraction Curve Name
//VarSpeedCoolEIRFT, !- Energy Input Ratio Function of Temperature Curve Name
//PackagedRatedCoolEIRFFlow, !- Energy Input Ratio Function of Flow Fraction Curve Name
//VarSpeedCyclingPLFFPLR, !- Part Load Fraction Correlation Curve Name
//autosize, !- Rated Low Speed Total Cooling Capacity {W}
//0.69, !- Rated Low Speed Sensible Heat Ratio
//4.2, !- Rated Low Speed COP
//autosize, !- Rated Low Speed Air Flow Rate {m3/s}
//VarSpeedCoolCapLSFT, !- Low Speed Total Cooling Capacity Function of Temperature Curve Name
//VarSpeedCoolEIRLSFT, !- Low Speed Energy Input Ratio Function of Temperature Curve Name
//Main Cooling Coil 1 Condenser Node; !- Condenser Air Inlet Node Name

// create a new CoilCoolingDXTwoSpeed object in the model's workspace
CoilCoolingDXTwoSpeed::CoilCoolingDXTwoSpeed(const Model& model,
                                             Schedule& availabilitySchedule,
                                             const Curve& coolingCurveFofTemp,
                                             const Curve& coolingCurveFofFlow,
                                             const Curve& energyInputRatioFofTemp,
                                             const Curve& energyInputRatioFofFlow,
                                             const Curve& partLoadFraction,
                                             const Curve& lowSpeedCoolingCurveFofTemp,
                                             const Curve& lowSpeedEnergyInputRatioFofTemp)
  : StraightComponent(CoilCoolingDXTwoSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingDXTwoSpeed_Impl>());
  //  A2 , \field Availability Schedule Name
  bool ok = setAvailabilitySchedule(availabilitySchedule);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to "
                  << availabilitySchedule.briefDescription() << ".");
  }
  //  N1 , \field Rated High Speed Total Cooling Capacity
  setRatedHighSpeedTotalCoolingCapacity(OptionalDouble());//autosize
  //  N2 , \field Rated High Speed Sensible Heat Ratio
  setRatedHighSpeedSensibleHeatRatio(OptionalDouble());//autosize
  //  N3 , \field Rated High Speed COP
  setRatedHighSpeedCOP(3.0);
  //  N4 , \field Rated High Speed Air Flow Rate
  setRatedHighSpeedAirFlowRate(OptionalDouble());//autosize
  //  A5 , \field Total Cooling Capacity Function of Temperature Curve Name
  setTotalCoolingCapacityFunctionOfTemperatureCurve(coolingCurveFofTemp);
  //  A6 , \field Total Cooling Capacity Function of Flow Fraction Curve Name
  setTotalCoolingCapacityFunctionOfFlowFractionCurve(coolingCurveFofFlow);
  //  A7 , \field Energy Input Ratio Function of Temperature Curve Name
  setEnergyInputRatioFunctionOfTemperatureCurve(energyInputRatioFofTemp);
  //  A8 , \field Energy Input Ratio Function of Flow Fraction Curve Name
  setEnergyInputRatioFunctionOfFlowFractionCurve(energyInputRatioFofFlow);
  //  A9 , \field Part Load Fraction Correlation Curve Name
  setPartLoadFractionCorrelationCurve(partLoadFraction);
  //  N5 , \field Rated Low Speed Total Cooling Capacity
  setRatedLowSpeedTotalCoolingCapacity(OptionalDouble());//autosize
  //  N6 , \field Rated Low Speed Sensible Heat Ratio
  setRatedLowSpeedSensibleHeatRatio(OptionalDouble());//autosize
  //  N7 , \field Rated Low Speed COP
  setRatedLowSpeedCOP(3.0);
  //  N8 , \field Rated Low Speed Air Flow Rate
  setRatedHighSpeedAirFlowRate(OptionalDouble());//autosize
  //  A10, \field Low Speed Total Cooling Capacity Function of Temperature Curve Name
  setLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve(lowSpeedCoolingCurveFofTemp);
  //  A11, \field Low Speed Energy Input Ratio Function of Temperature Curve Name
  setLowSpeedEnergyInputRatioFunctionOfTemperatureCurve(lowSpeedEnergyInputRatioFofTemp);
  //  A13, \field Condenser Type
  setCondenserType("AirCooled");
  //   N9, \field High Speed Evaporative Condenser Effectiveness
  setHighSpeedEvaporativeCondenserEffectiveness(0.0);
  //  N10, \field High Speed Evaporative Condenser Air Flow Rate
  setHighSpeedEvaporativeCondenserAirFlowRate(boost::none);//autosize
  //  N11, \field High Speed Evaporative Condenser Pump Rated Power Consumption
  setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption(boost::none);//autosize
  //  N12, \field Low Speed Evaporative Condenser Effectiveness
  setLowSpeedEvaporativeCondenserEffectiveness(0.0);
  //  N13, \field Low Speed Evaporative Condenser Air Flow Rate
  setLowSpeedEvaporativeCondenserAirFlowRate(boost::none);//autosize
  //  N14, \field Low Speed Evaporative Condenser Pump Rated Power Consumption
  setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption(boost::none);//autosize
  //  A14, \field Supply Water Storage Tank Name
  //setSupplyWaterStorageTankName
  //  A15, \field Condensate Collection Water Storage Tank Name
  //setCondensateCollectionWaterStorageTankName
  //  N15, \field Basin Heater Capacity
  setBasinHeaterCapacity(0.0);
  //  N16, \field Basin Heater Setpoint Temperature
  setBasinHeaterSetpointTemperature(10.0);
  //  A16; \field Basin Heater Operating Schedule Name
  setString(OS_Coil_Cooling_DX_TwoSpeedFields::BasinHeaterOperatingScheduleName,"");
}

CoilCoolingDXTwoSpeed::CoilCoolingDXTwoSpeed(const Model& model)
  : StraightComponent(CoilCoolingDXTwoSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingDXTwoSpeed_Impl>());

  auto availabilitySchedule = model.alwaysOnDiscreteSchedule();
  setAvailabilitySchedule(availabilitySchedule);

  CurveBiquadratic coolingCurveFofTemp = CurveBiquadratic(model);
  coolingCurveFofTemp.setCoefficient1Constant(0.42415);
  coolingCurveFofTemp.setCoefficient2x(0.04426);
  coolingCurveFofTemp.setCoefficient3xPOW2(-0.00042);
  coolingCurveFofTemp.setCoefficient4y(0.00333);
  coolingCurveFofTemp.setCoefficient5yPOW2(-0.00008);
  coolingCurveFofTemp.setCoefficient6xTIMESY(-0.00021);
  coolingCurveFofTemp.setMinimumValueofx(17.0);
  coolingCurveFofTemp.setMaximumValueofx(22.0);
  coolingCurveFofTemp.setMinimumValueofy(13.0);
  coolingCurveFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic coolingCurveFofFlow = CurveQuadratic(model);
  coolingCurveFofFlow.setCoefficient1Constant(0.77136);
  coolingCurveFofFlow.setCoefficient2x(0.34053);
  coolingCurveFofFlow.setCoefficient3xPOW2(-0.11088);
  coolingCurveFofFlow.setMinimumValueofx(0.75918);
  coolingCurveFofFlow.setMaximumValueofx(1.13877);

  CurveBiquadratic energyInputRatioFofTemp = CurveBiquadratic(model);
  energyInputRatioFofTemp.setCoefficient1Constant(1.23649);
  energyInputRatioFofTemp.setCoefficient2x(-0.02431);
  energyInputRatioFofTemp.setCoefficient3xPOW2(0.00057);
  energyInputRatioFofTemp.setCoefficient4y(-0.01434);
  energyInputRatioFofTemp.setCoefficient5yPOW2(0.00063);
  energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.00038);
  energyInputRatioFofTemp.setMinimumValueofx(17.0);
  energyInputRatioFofTemp.setMaximumValueofx(22.0);
  energyInputRatioFofTemp.setMinimumValueofy(13.0);
  energyInputRatioFofTemp.setMaximumValueofy(46.0);

  CurveQuadratic energyInputRatioFofFlow = CurveQuadratic(model);
  energyInputRatioFofFlow.setCoefficient1Constant(1.20550);
  energyInputRatioFofFlow.setCoefficient2x(-0.32953);
  energyInputRatioFofFlow.setCoefficient3xPOW2(0.12308);
  energyInputRatioFofFlow.setMinimumValueofx(0.75918);
  energyInputRatioFofFlow.setMaximumValueofx(1.13877);

  CurveQuadratic partLoadFraction = CurveQuadratic(model);
  partLoadFraction.setCoefficient1Constant(0.77100);
  partLoadFraction.setCoefficient2x(0.22900);
  partLoadFraction.setCoefficient3xPOW2(0.0);
  partLoadFraction.setMinimumValueofx(0.0);
  partLoadFraction.setMaximumValueofx(1.0);

  CurveBiquadratic coolingLowSpdCurveFofTemp = CurveBiquadratic(model);
  coolingLowSpdCurveFofTemp.setCoefficient1Constant(0.42415);
  coolingLowSpdCurveFofTemp.setCoefficient2x(0.04426);
  coolingLowSpdCurveFofTemp.setCoefficient3xPOW2(-0.00042);
  coolingLowSpdCurveFofTemp.setCoefficient4y(0.00333);
  coolingLowSpdCurveFofTemp.setCoefficient5yPOW2(-0.00008);
  coolingLowSpdCurveFofTemp.setCoefficient6xTIMESY(-0.00021);
  coolingLowSpdCurveFofTemp.setMinimumValueofx(17.0);
  coolingLowSpdCurveFofTemp.setMaximumValueofx(22.0);
  coolingLowSpdCurveFofTemp.setMinimumValueofy(13.0);
  coolingLowSpdCurveFofTemp.setMaximumValueofy(46.0);

  CurveBiquadratic energyLowSpdInputRatioFofTemp = CurveBiquadratic(model);
  energyLowSpdInputRatioFofTemp.setCoefficient1Constant(1.23649);
  energyLowSpdInputRatioFofTemp.setCoefficient2x(-0.02431);
  energyLowSpdInputRatioFofTemp.setCoefficient3xPOW2(0.00057);
  energyLowSpdInputRatioFofTemp.setCoefficient4y(-0.01434);
  energyLowSpdInputRatioFofTemp.setCoefficient5yPOW2(0.00063);
  energyLowSpdInputRatioFofTemp.setCoefficient6xTIMESY(-0.00038);
  energyLowSpdInputRatioFofTemp.setMinimumValueofx(17.0);
  energyLowSpdInputRatioFofTemp.setMaximumValueofx(22.0);
  energyLowSpdInputRatioFofTemp.setMinimumValueofy(13.0);
  energyLowSpdInputRatioFofTemp.setMaximumValueofy(46.0);

  setRatedHighSpeedTotalCoolingCapacity(OptionalDouble());//autosize
  setRatedHighSpeedSensibleHeatRatio(OptionalDouble());//autosize
  setRatedHighSpeedCOP(3.0);
  setRatedHighSpeedAirFlowRate(OptionalDouble());//autosize
  setTotalCoolingCapacityFunctionOfTemperatureCurve(coolingCurveFofTemp);
  setTotalCoolingCapacityFunctionOfFlowFractionCurve(coolingCurveFofFlow);
  setEnergyInputRatioFunctionOfTemperatureCurve(energyInputRatioFofTemp);
  setEnergyInputRatioFunctionOfFlowFractionCurve(energyInputRatioFofFlow);
  setPartLoadFractionCorrelationCurve(partLoadFraction);
  setRatedLowSpeedTotalCoolingCapacity(OptionalDouble());//autosize
  setRatedLowSpeedSensibleHeatRatio(0.69);//autosize
  setRatedLowSpeedCOP(3.0);
  setRatedHighSpeedAirFlowRate(OptionalDouble());//autosize
  setLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve(coolingLowSpdCurveFofTemp);
  setLowSpeedEnergyInputRatioFunctionOfTemperatureCurve(energyLowSpdInputRatioFofTemp);
  setCondenserType("AirCooled");
  setHighSpeedEvaporativeCondenserEffectiveness(0.0);
  setHighSpeedEvaporativeCondenserAirFlowRate(boost::none);//autosize
  setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption(boost::none);//autosize
  setLowSpeedEvaporativeCondenserEffectiveness(0.0);
  setLowSpeedEvaporativeCondenserAirFlowRate(boost::none);//autosize
  setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption(boost::none);//autosize
  setBasinHeaterCapacity(10.0);
  setBasinHeaterSetpointTemperature(2.0);
  setString(OS_Coil_Cooling_DX_TwoSpeedFields::BasinHeaterOperatingScheduleName,"");
}

CoilCoolingDXTwoSpeed::CoilCoolingDXTwoSpeed(std::shared_ptr<detail::CoilCoolingDXTwoSpeed_Impl> p)
  : StraightComponent(std::move(p))
{}

//  A2 , \field Availability Schedule Name
Schedule CoilCoolingDXTwoSpeed::availabilitySchedule()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->availabilitySchedule();
}
Schedule CoilCoolingDXTwoSpeed::getAvailabilitySchedule()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->availabilitySchedule();
}
bool CoilCoolingDXTwoSpeed::setAvailabilitySchedule(Schedule& schedule )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setAvailabilitySchedule( schedule );
}

//  N1 , \field Rated High Speed Total Cooling Capacity
OptionalDouble CoilCoolingDXTwoSpeed::ratedHighSpeedTotalCoolingCapacity() const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->ratedHighSpeedTotalCoolingCapacity();
}
OptionalDouble CoilCoolingDXTwoSpeed::getRatedHighSpeedTotalCoolingCapacity() const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->ratedHighSpeedTotalCoolingCapacity();
}
bool CoilCoolingDXTwoSpeed::setRatedHighSpeedTotalCoolingCapacity( OptionalDouble value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedHighSpeedTotalCoolingCapacity( value );
}
bool CoilCoolingDXTwoSpeed::setRatedHighSpeedTotalCoolingCapacity( double value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedHighSpeedTotalCoolingCapacity( value );
}

//  N2 , \field Rated High Speed Sensible Heat Ratio
OptionalDouble CoilCoolingDXTwoSpeed::ratedHighSpeedSensibleHeatRatio() const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->ratedHighSpeedSensibleHeatRatio();
}
OptionalDouble CoilCoolingDXTwoSpeed::getRatedHighSpeedSensibleHeatRatio() const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->ratedHighSpeedSensibleHeatRatio();
}
bool CoilCoolingDXTwoSpeed::setRatedHighSpeedSensibleHeatRatio( OptionalDouble value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedHighSpeedSensibleHeatRatio( value );
}
bool CoilCoolingDXTwoSpeed::setRatedHighSpeedSensibleHeatRatio( double value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedHighSpeedSensibleHeatRatio( value );
}


//  N3 , \field Rated High Speed COP
OptionalDouble CoilCoolingDXTwoSpeed::ratedHighSpeedCOP() const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->ratedHighSpeedCOP();
}
OptionalDouble CoilCoolingDXTwoSpeed::getRatedHighSpeedCOP() const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->ratedHighSpeedCOP();
}
bool CoilCoolingDXTwoSpeed::setRatedHighSpeedCOP( double value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedHighSpeedCOP( value );
}

//  N4 , \field Rated High Speed Air Flow Rate
OptionalDouble CoilCoolingDXTwoSpeed::ratedHighSpeedAirFlowRate() const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->ratedHighSpeedAirFlowRate();
}
OptionalDouble CoilCoolingDXTwoSpeed::getRatedHighSpeedAirFlowRate() const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->ratedHighSpeedAirFlowRate();
}
bool CoilCoolingDXTwoSpeed::setRatedHighSpeedAirFlowRate( OptionalDouble value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedHighSpeedAirFlowRate( value );
}
bool CoilCoolingDXTwoSpeed::setRatedHighSpeedAirFlowRate( double value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedHighSpeedAirFlowRate( value );
}


//  A5 , \field Total Cooling Capacity Function of Temperature Curve Name
Curve CoilCoolingDXTwoSpeed::totalCoolingCapacityFunctionOfTemperatureCurve()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->totalCoolingCapacityFunctionOfTemperatureCurve();
}
Curve CoilCoolingDXTwoSpeed::getTotalCoolingCapacityFunctionOfTemperatureCurve()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->totalCoolingCapacityFunctionOfTemperatureCurve();
}
bool CoilCoolingDXTwoSpeed::setTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setTotalCoolingCapacityFunctionOfTemperatureCurve( curve );
}

//  A6 , \field Total Cooling Capacity Function of Flow Fraction Curve Name
Curve CoilCoolingDXTwoSpeed::totalCoolingCapacityFunctionOfFlowFractionCurve()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->totalCoolingCapacityFunctionOfFlowFractionCurve();
}
Curve CoilCoolingDXTwoSpeed::getTotalCoolingCapacityFunctionOfFlowFractionCurve()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->totalCoolingCapacityFunctionOfFlowFractionCurve();
}
bool CoilCoolingDXTwoSpeed::setTotalCoolingCapacityFunctionOfFlowFractionCurve(const Curve& curve )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setTotalCoolingCapacityFunctionOfFlowFractionCurve( curve );
}

//  A7 , \field Energy Input Ratio Function of Temperature Curve Name
Curve CoilCoolingDXTwoSpeed::energyInputRatioFunctionOfTemperatureCurve()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->energyInputRatioFunctionOfTemperatureCurve();
}
Curve CoilCoolingDXTwoSpeed::getEnergyInputRatioFunctionOfTemperatureCurve()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->energyInputRatioFunctionOfTemperatureCurve();
}
bool CoilCoolingDXTwoSpeed::setEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setEnergyInputRatioFunctionOfTemperatureCurve( curve );
}

//  A8 , \field Energy Input Ratio Function of Flow Fraction Curve Name
Curve CoilCoolingDXTwoSpeed::energyInputRatioFunctionOfFlowFractionCurve()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->energyInputRatioFunctionOfFlowFractionCurve();
}
Curve CoilCoolingDXTwoSpeed::getEnergyInputRatioFunctionOfFlowFractionCurve()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->energyInputRatioFunctionOfFlowFractionCurve();
}
bool CoilCoolingDXTwoSpeed::setEnergyInputRatioFunctionOfFlowFractionCurve(const Curve& curve )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setEnergyInputRatioFunctionOfFlowFractionCurve( curve );
}

//  A9 , \field Part Load Fraction Correlation Curve Name
Curve CoilCoolingDXTwoSpeed::partLoadFractionCorrelationCurve()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->partLoadFractionCorrelationCurve();
}
Curve CoilCoolingDXTwoSpeed::getPartLoadFractionCorrelationCurve()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->partLoadFractionCorrelationCurve();
}
bool CoilCoolingDXTwoSpeed::setPartLoadFractionCorrelationCurve(const Curve& curve )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setPartLoadFractionCorrelationCurve( curve );
}

//  N5 , \field Rated Low Speed Total Cooling Capacity
OptionalDouble CoilCoolingDXTwoSpeed::ratedLowSpeedTotalCoolingCapacity() const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->ratedLowSpeedTotalCoolingCapacity();
}
OptionalDouble CoilCoolingDXTwoSpeed::getRatedLowSpeedTotalCoolingCapacity() const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->ratedLowSpeedTotalCoolingCapacity();
}
bool CoilCoolingDXTwoSpeed::setRatedLowSpeedTotalCoolingCapacity( OptionalDouble value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedLowSpeedTotalCoolingCapacity( value );
}
bool CoilCoolingDXTwoSpeed::setRatedLowSpeedTotalCoolingCapacity( double value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedLowSpeedTotalCoolingCapacity( value );
}


//  N6 , \field Rated Low Speed Sensible Heat Ratio
OptionalDouble CoilCoolingDXTwoSpeed::ratedLowSpeedSensibleHeatRatio() const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->ratedLowSpeedSensibleHeatRatio();
}
OptionalDouble CoilCoolingDXTwoSpeed::getRatedLowSpeedSensibleHeatRatio() const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->ratedLowSpeedSensibleHeatRatio();
}
bool CoilCoolingDXTwoSpeed::setRatedLowSpeedSensibleHeatRatio( OptionalDouble value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedLowSpeedSensibleHeatRatio( value );
}
bool CoilCoolingDXTwoSpeed::setRatedLowSpeedSensibleHeatRatio( double value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedLowSpeedSensibleHeatRatio( value );
}


//  N7 , \field Rated Low Speed COP
OptionalDouble CoilCoolingDXTwoSpeed::ratedLowSpeedCOP() const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->ratedLowSpeedCOP();
}
OptionalDouble CoilCoolingDXTwoSpeed::getRatedLowSpeedCOP() const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->ratedLowSpeedCOP();
}
bool CoilCoolingDXTwoSpeed::setRatedLowSpeedCOP( double value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedLowSpeedCOP( value );
}

//  N8 , \field Rated Low Speed Air Flow Rate
OptionalDouble CoilCoolingDXTwoSpeed::ratedLowSpeedAirFlowRate() const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->ratedLowSpeedAirFlowRate();
}
OptionalDouble CoilCoolingDXTwoSpeed::getRatedLowSpeedAirFlowRate() const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->ratedLowSpeedAirFlowRate();
}
bool CoilCoolingDXTwoSpeed::setRatedLowSpeedAirFlowRate( OptionalDouble value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedLowSpeedAirFlowRate( value );
}
bool CoilCoolingDXTwoSpeed::setRatedLowSpeedAirFlowRate( double value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedLowSpeedAirFlowRate( value );
}


//  A10, \field Low Speed Total Cooling Capacity Function of Temperature Curve Name
Curve CoilCoolingDXTwoSpeed::lowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->lowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve();
}
Curve CoilCoolingDXTwoSpeed::getLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->lowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve();
}
bool CoilCoolingDXTwoSpeed::setLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve( curve );
}

//  A11, \field Low Speed Energy Input Ratio Function of Temperature Curve Name
Curve CoilCoolingDXTwoSpeed::lowSpeedEnergyInputRatioFunctionOfTemperatureCurve()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->lowSpeedEnergyInputRatioFunctionOfTemperatureCurve();
}
Curve CoilCoolingDXTwoSpeed::getLowSpeedEnergyInputRatioFunctionOfTemperatureCurve()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->lowSpeedEnergyInputRatioFunctionOfTemperatureCurve();
}
bool CoilCoolingDXTwoSpeed::setLowSpeedEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setLowSpeedEnergyInputRatioFunctionOfTemperatureCurve( curve );
}

//  A12, \field Condenser Air Inlet Node Name
OptionalString CoilCoolingDXTwoSpeed::condenserAirInletNodeName()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->condenserAirInletNodeName();
}
OptionalString CoilCoolingDXTwoSpeed::getCondenserAirInletNodeName()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->condenserAirInletNodeName();
}
bool CoilCoolingDXTwoSpeed::setCondenserAirInletNodeName( const std::string& value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setCondenserAirInletNodeName( value );
}

//  A13, \field Condenser Type
std::string CoilCoolingDXTwoSpeed::condenserType()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->condenserType();
}
std::string CoilCoolingDXTwoSpeed::getCondenserType()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->condenserType();
}
bool CoilCoolingDXTwoSpeed::setCondenserType( const std::string& value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setCondenserType( value );
}

//   N9, \field High Speed Evaporative Condenser Effectiveness
OptionalDouble CoilCoolingDXTwoSpeed::highSpeedEvaporativeCondenserEffectiveness()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->highSpeedEvaporativeCondenserEffectiveness();
}
OptionalDouble CoilCoolingDXTwoSpeed::getHighSpeedEvaporativeCondenserEffectiveness()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->highSpeedEvaporativeCondenserEffectiveness();
}
bool CoilCoolingDXTwoSpeed::setHighSpeedEvaporativeCondenserEffectiveness( double value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setHighSpeedEvaporativeCondenserEffectiveness( value );
}

//  N10, \field High Speed Evaporative Condenser Air Flow Rate
OptionalDouble CoilCoolingDXTwoSpeed::highSpeedEvaporativeCondenserAirFlowRate()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->highSpeedEvaporativeCondenserAirFlowRate();
}
OptionalDouble CoilCoolingDXTwoSpeed::getHighSpeedEvaporativeCondenserAirFlowRate()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->highSpeedEvaporativeCondenserAirFlowRate();
}
bool CoilCoolingDXTwoSpeed::setHighSpeedEvaporativeCondenserAirFlowRate( OptionalDouble value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setHighSpeedEvaporativeCondenserAirFlowRate( value );
}
bool CoilCoolingDXTwoSpeed::setHighSpeedEvaporativeCondenserAirFlowRate( double value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setHighSpeedEvaporativeCondenserAirFlowRate( value );
}


//  N11, \field High Speed Evaporative Condenser Pump Rated Power Consumption
OptionalDouble CoilCoolingDXTwoSpeed::highSpeedEvaporativeCondenserPumpRatedPowerConsumption()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->highSpeedEvaporativeCondenserPumpRatedPowerConsumption();
}
OptionalDouble CoilCoolingDXTwoSpeed::getHighSpeedEvaporativeCondenserPumpRatedPowerConsumption()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->highSpeedEvaporativeCondenserPumpRatedPowerConsumption();
}
bool CoilCoolingDXTwoSpeed::setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption( OptionalDouble value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption( value );
}
bool CoilCoolingDXTwoSpeed::setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption( double value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption( value );
}


//  N12, \field Low Speed Evaporative Condenser Effectiveness
OptionalDouble CoilCoolingDXTwoSpeed::lowSpeedEvaporativeCondenserEffectiveness()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->lowSpeedEvaporativeCondenserEffectiveness();
}
OptionalDouble CoilCoolingDXTwoSpeed::getLowSpeedEvaporativeCondenserEffectiveness()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->lowSpeedEvaporativeCondenserEffectiveness();
}
bool CoilCoolingDXTwoSpeed::setLowSpeedEvaporativeCondenserEffectiveness( double value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setLowSpeedEvaporativeCondenserEffectiveness( value );
}

//  N13, \field Low Speed Evaporative Condenser Air Flow Rate
OptionalDouble CoilCoolingDXTwoSpeed::lowSpeedEvaporativeCondenserAirFlowRate()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->lowSpeedEvaporativeCondenserAirFlowRate();
}
OptionalDouble CoilCoolingDXTwoSpeed::getLowSpeedEvaporativeCondenserAirFlowRate()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->lowSpeedEvaporativeCondenserAirFlowRate();
}
bool CoilCoolingDXTwoSpeed::setLowSpeedEvaporativeCondenserAirFlowRate( OptionalDouble value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setLowSpeedEvaporativeCondenserAirFlowRate( value );
}
bool CoilCoolingDXTwoSpeed::setLowSpeedEvaporativeCondenserAirFlowRate( double value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setLowSpeedEvaporativeCondenserAirFlowRate( value );
}


//  N14, \field Low Speed Evaporative Condenser Pump Rated Power Consumption
OptionalDouble CoilCoolingDXTwoSpeed::lowSpeedEvaporativeCondenserPumpRatedPowerConsumption()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->lowSpeedEvaporativeCondenserPumpRatedPowerConsumption();
}
OptionalDouble CoilCoolingDXTwoSpeed::getLowSpeedEvaporativeCondenserPumpRatedPowerConsumption()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->lowSpeedEvaporativeCondenserPumpRatedPowerConsumption();
}
bool CoilCoolingDXTwoSpeed::setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption( OptionalDouble value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption( value );
}
bool CoilCoolingDXTwoSpeed::setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption( double value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption( value );
}


//  A14, \field Supply Water Storage Tank Name
  //getSupplyWaterStorageTankName
  //setSupplyWaterStorageTankName

//  A15, \field Condensate Collection Water Storage Tank Name
  //getCondensateCollectionWaterStorageTankName
  //setCondensateCollectionWaterStorageTankName

//  N15, \field Basin Heater Capacity
OptionalDouble CoilCoolingDXTwoSpeed::basinHeaterCapacity()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->basinHeaterCapacity();
}
OptionalDouble CoilCoolingDXTwoSpeed::getBasinHeaterCapacity()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->basinHeaterCapacity();
}
bool CoilCoolingDXTwoSpeed::setBasinHeaterCapacity( double value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setBasinHeaterCapacity( value );
}

//  N16, \field Basin Heater Setpoint Temperature
OptionalDouble CoilCoolingDXTwoSpeed::basinHeaterSetpointTemperature()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->basinHeaterSetpointTemperature();
}
OptionalDouble CoilCoolingDXTwoSpeed::getBasinHeaterSetpointTemperature()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->basinHeaterSetpointTemperature();
}
bool CoilCoolingDXTwoSpeed::setBasinHeaterSetpointTemperature( double value )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setBasinHeaterSetpointTemperature( value );
}

//  A16; \field Basin Heater Operating Schedule Name
OptionalSchedule CoilCoolingDXTwoSpeed::basinHeaterOperatingSchedule()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->basinHeaterOperatingSchedule();
}
OptionalSchedule CoilCoolingDXTwoSpeed::getBasinHeaterOperatingSchedule()const
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->basinHeaterOperatingSchedule();
}
bool CoilCoolingDXTwoSpeed::setBasinHeaterOperatingSchedule(Schedule& schedule )
{
  return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setBasinHeaterOperatingSchedule( schedule );
}
void CoilCoolingDXTwoSpeed::resetBasinHeaterOperatingSchedule() {
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->resetBasinHeaterOperatingSchedule();
}

IddObjectType CoilCoolingDXTwoSpeed::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Coil_Cooling_DX_TwoSpeed);
  return result;
}

  boost::optional<double> CoilCoolingDXTwoSpeed::autosizedRatedHighSpeedTotalCoolingCapacity() const {
    return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->autosizedRatedHighSpeedTotalCoolingCapacity();
  }

  boost::optional<double> CoilCoolingDXTwoSpeed::autosizedRatedHighSpeedSensibleHeatRatio() const {
    return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->autosizedRatedHighSpeedSensibleHeatRatio();
  }

  boost::optional<double> CoilCoolingDXTwoSpeed::autosizedRatedHighSpeedAirFlowRate() const {
    return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->autosizedRatedHighSpeedAirFlowRate();
  }

  boost::optional<double> CoilCoolingDXTwoSpeed::autosizedRatedLowSpeedTotalCoolingCapacity() const {
    return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->autosizedRatedLowSpeedTotalCoolingCapacity();
  }

  boost::optional<double> CoilCoolingDXTwoSpeed::autosizedRatedLowSpeedSensibleHeatRatio() const {
    return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->autosizedRatedLowSpeedSensibleHeatRatio();
  }

  boost::optional<double> CoilCoolingDXTwoSpeed::autosizedRatedLowSpeedAirFlowRate() const {
    return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->autosizedRatedLowSpeedAirFlowRate();
  }

  boost::optional<double> CoilCoolingDXTwoSpeed::autosizedHighSpeedEvaporativeCondenserAirFlowRate() const {
    return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->autosizedHighSpeedEvaporativeCondenserAirFlowRate();
  }

  boost::optional<double> CoilCoolingDXTwoSpeed::autosizedHighSpeedEvaporativeCondenserPumpRatedPowerConsumption() const {
    return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->autosizedHighSpeedEvaporativeCondenserPumpRatedPowerConsumption();
  }

  boost::optional<double> CoilCoolingDXTwoSpeed::autosizedLowSpeedEvaporativeCondenserAirFlowRate() const {
    return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->autosizedLowSpeedEvaporativeCondenserAirFlowRate();
  }

  boost::optional<double> CoilCoolingDXTwoSpeed::autosizedLowSpeedEvaporativeCondenserPumpRatedPowerConsumption() const {
    return getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->autosizedLowSpeedEvaporativeCondenserPumpRatedPowerConsumption();
  }

} // model
} // openstudio

