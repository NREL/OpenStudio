/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include <model/CoilCoolingDXTwoSpeed.hpp>
#include <model/CoilCoolingDXTwoSpeed_Impl.hpp>
#include <model/CurveQuadratic.hpp>
#include <model/CurveQuadratic_Impl.hpp>
#include <model/CurveBiquadratic.hpp>
#include <model/CurveBiquadratic_Impl.hpp>
#include <model/CurveCubic.hpp>
#include <model/CurveCubic_Impl.hpp>
#include <model/Curve.hpp>
#include <model/Curve_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
<<<<<<< HEAD
#include <model/AirLoopHVACUnitarySystem.hpp>
#include <model/AirLoopHVACUnitarySystem_Impl.hpp>
=======
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
>>>>>>> develop

#include <model/Model.hpp>
#include <utilities/idd/OS_Coil_Cooling_DX_TwoSpeed_FieldEnums.hxx>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <boost/foreach.hpp>

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

    Curve ccfot = totalCoolingCapacityFunctionOfTemperatureCurve();
    newCoil.setTotalCoolingCapacityFunctionOfTemperatureCurve(ccfot.clone(model).cast<Curve>());

    Curve ccfof = totalCoolingCapacityFunctionOfFlowFractionCurve();
    newCoil.setTotalCoolingCapacityFunctionOfFlowFractionCurve(ccfof.clone(model).cast<Curve>());

    Curve eifot = energyInputRatioFunctionOfTemperatureCurve();
    newCoil.setEnergyInputRatioFunctionOfTemperatureCurve(eifot.clone(model).cast<Curve>());

    Curve eifof = energyInputRatioFunctionOfFlowFractionCurve();
    newCoil.setEnergyInputRatioFunctionOfFlowFractionCurve(eifof.clone(model).cast<Curve>());

    Curve plfcc = partLoadFractionCorrelationCurve();
    newCoil.setPartLoadFractionCorrelationCurve(plfcc.clone(model).cast<Curve>());

    Curve lsccfot = lowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve();
    newCoil.setLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve(lsccfot.clone(model).cast<Curve>());

    Curve lseirfot = lowSpeedEnergyInputRatioFunctionOfTemperatureCurve();
    newCoil.setLowSpeedEnergyInputRatioFunctionOfTemperatureCurve(lseirfot.clone(model).cast<Curve>());

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
    static std::vector<std::string> result;
    if (result.empty()){
    }
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

  void CoilCoolingDXTwoSpeed_Impl::setRatedHighSpeedTotalCoolingCapacity( OptionalDouble value )
  {
    if(value)
    {
      setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedTotalCoolingCapacity,*value);
    }
    else
    {
      setString(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedTotalCoolingCapacity,"Autosize");
    }
  }

  // N2 , \field Rated High Speed Sensible Heat Ratio
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::ratedHighSpeedSensibleHeatRatio() const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedSensibleHeatRatio);
  }
  void CoilCoolingDXTwoSpeed_Impl::setRatedHighSpeedSensibleHeatRatio( OptionalDouble value )
  {
    if(value)
    {
      setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedSensibleHeatRatio,*value);
    }
    else
    {
      setString(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedSensibleHeatRatio,"Autosize");
    }
  }

  // N3 , \field Rated High Speed COP
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::ratedHighSpeedCOP() const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedCOP);
  }
  void CoilCoolingDXTwoSpeed_Impl::setRatedHighSpeedCOP( double value )
  {
    setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedCOP,value);
  }

  // N4 , \field Rated High Speed Air Flow Rate
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::ratedHighSpeedAirFlowRate() const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedAirFlowRate);
  }
  void CoilCoolingDXTwoSpeed_Impl::setRatedHighSpeedAirFlowRate( OptionalDouble value )
  {
    if(value)
    {
      setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedAirFlowRate,*value);
    }
    else
    {
      setString(OS_Coil_Cooling_DX_TwoSpeedFields::RatedHighSpeedAirFlowRate,"Autosize");
    }
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
  void CoilCoolingDXTwoSpeed_Impl::setRatedLowSpeedTotalCoolingCapacity( OptionalDouble value )
  {
    if(value)
    {
      setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedTotalCoolingCapacity,*value);
    }
    else
    {
      setString(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedTotalCoolingCapacity,"Autosize");
    }
  }

  // N6 , \field Rated Low Speed Sensible Heat Ratio
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::ratedLowSpeedSensibleHeatRatio() const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedSensibleHeatRatio);
  }
  void CoilCoolingDXTwoSpeed_Impl::setRatedLowSpeedSensibleHeatRatio( OptionalDouble value )
  {
    if(value)
    {
      setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedSensibleHeatRatio,*value);
    }
    else
    {
      setString(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedSensibleHeatRatio,"Autosize");
    }
  }

  // N7 , \field Rated Low Speed COP
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::ratedLowSpeedCOP() const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedCOP);
  }
  void CoilCoolingDXTwoSpeed_Impl::setRatedLowSpeedCOP( double value )
  {
    setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedCOP,value);
  }

  // N8 , \field Rated Low Speed Air Flow Rate
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::ratedLowSpeedAirFlowRate() const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedAirFlowRate);
  }
  void CoilCoolingDXTwoSpeed_Impl::setRatedLowSpeedAirFlowRate( OptionalDouble value )
  {
    if(value)
    {
      setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedAirFlowRate,*value);
    }
    else
    {
      setString(OS_Coil_Cooling_DX_TwoSpeedFields::RatedLowSpeedAirFlowRate,"Autosize");
    }
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
  void CoilCoolingDXTwoSpeed_Impl::setCondenserAirInletNodeName( const std::string& value )
  {
    setString(OS_Coil_Cooling_DX_TwoSpeedFields::CondenserAirInletNodeName,value);
  }

  // A13, \field Condenser Type
  std::string CoilCoolingDXTwoSpeed_Impl::condenserType()const
  {
    return getString(OS_Coil_Cooling_DX_TwoSpeedFields::CondenserType).get();
  }
  void CoilCoolingDXTwoSpeed_Impl::setCondenserType( const std::string& value )
  {
    setString(OS_Coil_Cooling_DX_TwoSpeedFields::CondenserType,value);
  }

  // N9, \field High Speed Evaporative Condenser Effectiveness
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::highSpeedEvaporativeCondenserEffectiveness()const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserEffectiveness);
  }
  void CoilCoolingDXTwoSpeed_Impl::setHighSpeedEvaporativeCondenserEffectiveness( double value )
  {
    setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserEffectiveness,value);
  }

  // N10, \field High Speed Evaporative Condenser Air Flow Rate
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::highSpeedEvaporativeCondenserAirFlowRate()const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserAirFlowRate);
  }
  void CoilCoolingDXTwoSpeed_Impl::setHighSpeedEvaporativeCondenserAirFlowRate( OptionalDouble value )
  {
    if(value)
      setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserAirFlowRate,*value);
    else
      setString(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserAirFlowRate,"Autosize");
  }

  // N11, \field High Speed Evaporative Condenser Pump Rated Power Consumption
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::highSpeedEvaporativeCondenserPumpRatedPowerConsumption()const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserPumpRatedPowerConsumption);
  }
  void CoilCoolingDXTwoSpeed_Impl::setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption( OptionalDouble value )
  {
    if(value)
      setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserPumpRatedPowerConsumption,*value);
    else
      setString(OS_Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserPumpRatedPowerConsumption,"Autosize");
  }

  // N12, \field Low Speed Evaporative Condenser Effectiveness
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::lowSpeedEvaporativeCondenserEffectiveness()const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserEffectiveness);
  }
  void CoilCoolingDXTwoSpeed_Impl::setLowSpeedEvaporativeCondenserEffectiveness( double value )
  {
    setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserEffectiveness,value);
  }

  // N13, \field Low Speed Evaporative Condenser Air Flow Rate
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::lowSpeedEvaporativeCondenserAirFlowRate()const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserAirFlowRate);
  }
  void CoilCoolingDXTwoSpeed_Impl::setLowSpeedEvaporativeCondenserAirFlowRate( OptionalDouble value )
  {
    if(value)
      setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserAirFlowRate,*value);
    else
      setString(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserAirFlowRate,"Autosize");
  }

  // N14, \field Low Speed Evaporative Condenser Pump Rated Power Consumption
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::lowSpeedEvaporativeCondenserPumpRatedPowerConsumption()const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserPumpRatedPowerConsumption);
  }
  void CoilCoolingDXTwoSpeed_Impl::setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption( OptionalDouble value )
  {
    if(value)
      setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserPumpRatedPowerConsumption,*value);
    else
      setString(OS_Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserPumpRatedPowerConsumption,"Autosize");
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
  void CoilCoolingDXTwoSpeed_Impl::setBasinHeaterCapacity( double value )
  {
    setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::BasinHeaterCapacity,value);
  }

  // N16, \field Basin Heater Setpoint Temperature
  OptionalDouble CoilCoolingDXTwoSpeed_Impl::basinHeaterSetpointTemperature() const
  {
    return getDouble(OS_Coil_Cooling_DX_TwoSpeedFields::BasinHeaterSetpointTemperature);
  }
  void CoilCoolingDXTwoSpeed_Impl::setBasinHeaterSetpointTemperature( double value )
  {
    setDouble(OS_Coil_Cooling_DX_TwoSpeedFields::BasinHeaterSetpointTemperature,value);
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
  unsigned CoilCoolingDXTwoSpeed_Impl::inletPort()
  {
    return OS_Coil_Cooling_DX_TwoSpeedFields::AirInletNodeName;
  }

  // A4 , \field Air Outlet Node Name
  unsigned CoilCoolingDXTwoSpeed_Impl::outletPort()
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

<<<<<<< HEAD
  boost::optional<HVACComponent> CoilCoolingDXTwoSpeed_Impl::containingHVACComponent() const
  {
    // AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

    for( std::vector<AirLoopHVACUnitarySystem>::iterator it = airLoopHVACUnitarySystems.begin();
    it < airLoopHVACUnitarySystems.end();
    ++it )
    {
      if( boost::optional<HVACComponent> coolingCoil = it->coolingCoil() )
      {
        if( coolingCoil->handle() == this->handle() )
        {
          return *it;
        }
      }
    }
    return boost::none;
=======
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
>>>>>>> develop
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

CoilCoolingDXTwoSpeed::CoilCoolingDXTwoSpeed(boost::shared_ptr<detail::CoilCoolingDXTwoSpeed_Impl> p)
  : StraightComponent(p)
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
void CoilCoolingDXTwoSpeed::setRatedHighSpeedTotalCoolingCapacity( OptionalDouble value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedHighSpeedTotalCoolingCapacity( value );
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
void CoilCoolingDXTwoSpeed::setRatedHighSpeedSensibleHeatRatio( OptionalDouble value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedHighSpeedSensibleHeatRatio( value );
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
void CoilCoolingDXTwoSpeed::setRatedHighSpeedCOP( double value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedHighSpeedCOP( value );
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
void CoilCoolingDXTwoSpeed::setRatedHighSpeedAirFlowRate( OptionalDouble value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedHighSpeedAirFlowRate( value );
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
void CoilCoolingDXTwoSpeed::setRatedLowSpeedTotalCoolingCapacity( OptionalDouble value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedLowSpeedTotalCoolingCapacity( value );
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
void CoilCoolingDXTwoSpeed::setRatedLowSpeedSensibleHeatRatio( OptionalDouble value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedLowSpeedSensibleHeatRatio( value );
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
void CoilCoolingDXTwoSpeed::setRatedLowSpeedCOP( double value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedLowSpeedCOP( value );
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
void CoilCoolingDXTwoSpeed::setRatedLowSpeedAirFlowRate( OptionalDouble value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setRatedLowSpeedAirFlowRate( value );
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
void CoilCoolingDXTwoSpeed::setCondenserAirInletNodeName( const std::string& value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setCondenserAirInletNodeName( value );
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
void CoilCoolingDXTwoSpeed::setCondenserType( const std::string& value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setCondenserType( value );
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
void CoilCoolingDXTwoSpeed::setHighSpeedEvaporativeCondenserEffectiveness( double value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setHighSpeedEvaporativeCondenserEffectiveness( value );
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
void CoilCoolingDXTwoSpeed::setHighSpeedEvaporativeCondenserAirFlowRate( OptionalDouble value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setHighSpeedEvaporativeCondenserAirFlowRate( value );
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
void CoilCoolingDXTwoSpeed::setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption( OptionalDouble value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption( value );
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
void CoilCoolingDXTwoSpeed::setLowSpeedEvaporativeCondenserEffectiveness( double value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setLowSpeedEvaporativeCondenserEffectiveness( value );
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
void CoilCoolingDXTwoSpeed::setLowSpeedEvaporativeCondenserAirFlowRate( OptionalDouble value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setLowSpeedEvaporativeCondenserAirFlowRate( value );
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
void CoilCoolingDXTwoSpeed::setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption( OptionalDouble value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption( value );
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
void CoilCoolingDXTwoSpeed::setBasinHeaterCapacity( double value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setBasinHeaterCapacity( value );
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
void CoilCoolingDXTwoSpeed::setBasinHeaterSetpointTemperature( double value )
{
  getImpl<detail::CoilCoolingDXTwoSpeed_Impl>()->setBasinHeaterSetpointTemperature( value );
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

} // model
} // openstudio

