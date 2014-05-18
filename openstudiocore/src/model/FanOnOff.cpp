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

#include <model/FanOnOff.hpp>
#include <model/FanOnOff_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Model.hpp>
#include <model/Loop.hpp>
#include <model/StraightComponent.hpp>
#include <model/StraightComponent_Impl.hpp>
#include <model/Curve.hpp>
#include <model/Curve_Impl.hpp>
#include <model/CurveCubic.hpp>
#include <model/CurveCubic_Impl.hpp>
#include <model/CurveExponent.hpp>
#include <model/CurveExponent_Impl.hpp>
#include <model/ZoneHVACComponent.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>
#include <model/ZoneHVACWaterToAirHeatPump.hpp>
#include <model/ZoneHVACWaterToAirHeatPump_Impl.hpp>
#include <model/ZoneHVACPackagedTerminalAirConditioner.hpp>
#include <model/ZoneHVACPackagedTerminalAirConditioner_Impl.hpp>
#include <model/ZoneHVACFourPipeFanCoil.hpp>
#include <model/ZoneHVACFourPipeFanCoil_Impl.hpp>
#include <model/ZoneHVACPackagedTerminalHeatPump.hpp>
#include <model/ZoneHVACPackagedTerminalHeatPump_Impl.hpp>
#include <model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp>
#include <model/ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp>
#include <model/AirLoopHVACUnitaryHeatPumpAirToAir.hpp>
#include <model/AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp>
#include <model/AirLoopHVACUnitarySystem.hpp>
#include <model/AirLoopHVACUnitarySystem_Impl.hpp>
#include <utilities/idd/OS_Fan_OnOff_FieldEnums.hxx>
#include <utilities/units/Unit.hpp>
#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  FanOnOff_Impl::FanOnOff_Impl(const IdfObject& idfObject,
                               Model_Impl* model,
                               bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == FanOnOff::iddObjectType());
  }

  FanOnOff_Impl::FanOnOff_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == FanOnOff::iddObjectType());
  }

  FanOnOff_Impl::FanOnOff_Impl(const FanOnOff_Impl& other,
                               Model_Impl* model,
                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& FanOnOff_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty())
    {
    }
    return result;
  }

  IddObjectType FanOnOff_Impl::iddObjectType() const
  {
    return FanOnOff::iddObjectType();
  }

  std::vector<openstudio::IdfObject> FanOnOff_Impl::remove()
  {
    return StraightComponent_Impl::remove();
  }

  std::vector<ModelObject> FanOnOff_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (boost::optional<Curve> intermediate = fanPowerRatioFunctionofSpeedRatioCurve()) {
      result.push_back(*intermediate);
    }
    if (boost::optional<Curve> intermediate = fanEfficiencyRatioFunctionofSpeedRatioCurve()) {
      result.push_back(*intermediate);
    }
    return result;
  }

  // Inlet and Outlet nodes

  unsigned FanOnOff_Impl::inletPort()
  {
    return OS_Fan_OnOffFields::AirInletNodeName;
  }

  unsigned FanOnOff_Impl::outletPort()
  {
    return OS_Fan_OnOffFields::AirOutletNodeName;
  }

  std::vector<ScheduleTypeKey> FanOnOff_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Fan_OnOffFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("FanOnOff","Availability"));
    }
    return result;
  }

  Schedule FanOnOff_Impl::availabilitySchedule() const
  {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value)
    {
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<FanOnOff_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    OS_ASSERT(value);
    return value.get();
  }

  bool FanOnOff_Impl::setAvailabilitySchedule(Schedule& schedule)
  {
    bool result = setSchedule(OS_Fan_OnOffFields::AvailabilityScheduleName,
                              "FanOnOff",
                              "Availability",
                              schedule);
    return result;
  }

  double FanOnOff_Impl::fanEfficiency() const
  {
    boost::optional<double> value = getDouble(OS_Fan_OnOffFields::FanEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FanOnOff_Impl::isFanEfficiencyDefaulted() const
  {
    return isEmpty(OS_Fan_OnOffFields::FanEfficiency);
  }

  double FanOnOff_Impl::pressureRise() const
  {
    boost::optional<double> value = getDouble(OS_Fan_OnOffFields::PressureRise,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> FanOnOff_Impl::maximumFlowRate() const
  {
    return getDouble(OS_Fan_OnOffFields::MaximumFlowRate,true);
  }

  bool FanOnOff_Impl::isMaximumFlowRateAutosized() const
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Fan_OnOffFields::MaximumFlowRate, true);
    if (value)
    {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double FanOnOff_Impl::motorEfficiency() const
  {
    boost::optional<double> value = getDouble(OS_Fan_OnOffFields::MotorEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FanOnOff_Impl::isMotorEfficiencyDefaulted() const
  {
    return isEmpty(OS_Fan_OnOffFields::MotorEfficiency);
  }

  boost::optional<double> FanOnOff_Impl::motorInAirstreamFraction() const
  {
    return getDouble(OS_Fan_OnOffFields::MotorInAirstreamFraction,true);
  }

  std::string FanOnOff_Impl::endUseSubcategory() const
  {
    boost::optional<std::string> value = getString(OS_Fan_OnOffFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FanOnOff_Impl::isEndUseSubcategoryDefaulted() const
  {
    return isEmpty(OS_Fan_OnOffFields::EndUseSubcategory);
  }

  bool FanOnOff_Impl::setFanEfficiency(double fanEfficiency)
  {
    bool result = setDouble(OS_Fan_OnOffFields::FanEfficiency, fanEfficiency);
    return result;
  }

  void FanOnOff_Impl::resetFanEfficiency()
  {
    bool result = setString(OS_Fan_OnOffFields::FanEfficiency, "");
    OS_ASSERT(result);
  }

  void FanOnOff_Impl::setPressureRise(double pressureRise)
  {
    bool result = setDouble(OS_Fan_OnOffFields::PressureRise, pressureRise);
    OS_ASSERT(result);
  }

  bool FanOnOff_Impl::setMaximumFlowRate(boost::optional<double> maximumFlowRate)
  {
    bool result(false);
    if (maximumFlowRate)
    {
      result = setDouble(OS_Fan_OnOffFields::MaximumFlowRate, maximumFlowRate.get());
    }
    else
    {
      resetMaximumFlowRate();
      result = true;
    }
    return result;
  }

  void FanOnOff_Impl::resetMaximumFlowRate()
  {
    bool result = setString(OS_Fan_OnOffFields::MaximumFlowRate, "");
    OS_ASSERT(result);
  }

  void FanOnOff_Impl::autosizeMaximumFlowRate()
  {
    bool result = setString(OS_Fan_OnOffFields::MaximumFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool FanOnOff_Impl::setMotorEfficiency(double motorEfficiency)
  {
    bool result = setDouble(OS_Fan_OnOffFields::MotorEfficiency, motorEfficiency);
    return result;
  }

  void FanOnOff_Impl::resetMotorEfficiency()
  {
    bool result = setString(OS_Fan_OnOffFields::MotorEfficiency, "");
    OS_ASSERT(result);
  }

  bool FanOnOff_Impl::setMotorInAirstreamFraction(boost::optional<double> motorInAirstreamFraction)
  {
    bool result(false);
    if (motorInAirstreamFraction)
    {
      result = setDouble(OS_Fan_OnOffFields::MotorInAirstreamFraction, motorInAirstreamFraction.get());
    }
    else
    {
      resetMotorInAirstreamFraction();
      result = true;
    }
    return result;
  }

  void FanOnOff_Impl::resetMotorInAirstreamFraction()
  {
    bool result = setString(OS_Fan_OnOffFields::MotorInAirstreamFraction, "");
    OS_ASSERT(result);
  }

  void FanOnOff_Impl::setEndUseSubcategory(std::string endUseSubcategory)
  {
    bool result = setString(OS_Fan_OnOffFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  void FanOnOff_Impl::resetEndUseSubcategory()
  {
    bool result = setString(OS_Fan_OnOffFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> FanOnOff_Impl::optionalAvailabilitySchedule() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_OnOffFields::AvailabilityScheduleName);
  }

  boost::optional<ModelObject> FanOnOff_Impl::availabilityScheduleAsModelObject() const
  {
    OptionalModelObject result = availabilitySchedule();
    return result;
   }

  // Curves

  Curve FanOnOff_Impl::fanPowerRatioFunctionofSpeedRatioCurve() const
  {
    boost::optional<Curve> curve = getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_OnOffFields::FanPowerRatioFunctionofSpeedRatioCurveName);
    OS_ASSERT(curve);
    return curve.get();
  }

  bool FanOnOff_Impl::setFanPowerRatioFunctionofSpeedRatioCurve(const Curve& curve )
  {
    if( model() != curve.model() )
    {
      return false;
    }
    return this->setPointer(OS_Fan_OnOffFields::FanPowerRatioFunctionofSpeedRatioCurveName,curve.handle());
  }


  Curve FanOnOff_Impl::fanEfficiencyRatioFunctionofSpeedRatioCurve() const
  {
    boost::optional<Curve> curve = getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_OnOffFields::FanEfficiencyRatioFunctionofSpeedRatioCurveName);
    OS_ASSERT(curve);
    return curve.get();
  }

  bool FanOnOff_Impl::setFanEfficiencyRatioFunctionofSpeedRatioCurve( const Curve& curve )
  {
    if( model() != curve.model() )
    {
      return false;
    }
    return this->setPointer(OS_Fan_OnOffFields::FanEfficiencyRatioFunctionofSpeedRatioCurveName,curve.handle());
  }

  ModelObject FanOnOff_Impl::clone(Model model) const
  {
    FanOnOff newFan = ModelObject_Impl::clone(model).cast<FanOnOff>();

    Curve curve1 = fanPowerRatioFunctionofSpeedRatioCurve();
    newFan.setFanPowerRatioFunctionofSpeedRatioCurve(curve1.clone(model).cast<Curve>());

    Curve curve2 = fanEfficiencyRatioFunctionofSpeedRatioCurve();
    newFan.setFanEfficiencyRatioFunctionofSpeedRatioCurve(curve2.clone(model).cast<Curve>());

    return newFan;
  }

  // Fan:OnOff can not be added to an AirLoopHVAC.
  // It can only be contained within another HVAC Component, such as Unitary, ZoneHVAC, etc.
  bool FanOnOff_Impl::addToNode(Node & node)
  {
    if( boost::optional<Loop> loop = node.loop() ) {
      return false;
    }
    else {
      return StraightComponent_Impl::addToNode(node);
    }
  }

  boost::optional<HVACComponent> FanOnOff_Impl::containingHVACComponent() const
  {
    // Process all types that might contain a FanOnOff object.

    // AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

    for( std::vector<AirLoopHVACUnitarySystem>::iterator it = airLoopHVACUnitarySystems.begin();
    it < airLoopHVACUnitarySystems.end();
    ++it )
    {
      if( boost::optional<HVACComponent> fan = it->supplyFan() )
      {
        if( fan->handle() == this->handle() )
        {
          return *it;
        }
      }
    }

    // AirLoopHVACUnitaryHeatPumpAirToAir
    std::vector<AirLoopHVACUnitaryHeatPumpAirToAir> airLoopHVACUnitaryHeatPumpAirToAirs = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

    for( std::vector<AirLoopHVACUnitaryHeatPumpAirToAir>::iterator it = airLoopHVACUnitaryHeatPumpAirToAirs.begin();
    it < airLoopHVACUnitaryHeatPumpAirToAirs.end();
    ++it )
    {
      if( boost::optional<HVACComponent> fan = it->supplyAirFan() )
      {
        if( fan->handle() == this->handle() )
        {
          return *it;
        }
      }
    }

    return boost::none;
  }

  boost::optional<ZoneHVACComponent> FanOnOff_Impl::containingZoneHVACComponent() const
  {
    std::vector<ZoneHVACComponent> zoneHVACComponent = this->model().getModelObjects<ZoneHVACComponent>();
    for( std::vector<ZoneHVACComponent>::iterator it = zoneHVACComponent.begin();
    it < zoneHVACComponent.end();
    ++it )
    {
      switch(it->iddObject().type().value())
      {
      case openstudio::IddObjectType::OS_ZoneHVAC_FourPipeFanCoil :
        {
          ZoneHVACFourPipeFanCoil component = it->cast<ZoneHVACFourPipeFanCoil>();
          if (component.supplyAirFan().handle() == this->handle()) return *it;
          break;
        }
      case openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump :
        {
          ZoneHVACPackagedTerminalHeatPump component = it->cast<ZoneHVACPackagedTerminalHeatPump>();
          if (component.supplyAirFan().handle() == this->handle()) return *it;
          break;
        }
      case openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner :
        {
          ZoneHVACPackagedTerminalAirConditioner component = it->cast<ZoneHVACPackagedTerminalAirConditioner>();
          if (component.supplyAirFan().handle() == this->handle()) return *it;
          break;
        }
      case openstudio::IddObjectType::OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlow :
        {
          ZoneHVACTerminalUnitVariableRefrigerantFlow component = it->cast<ZoneHVACTerminalUnitVariableRefrigerantFlow>();
          if (component.supplyAirFan().handle() == this->handle()) return *it;
          break;
        }
      case openstudio::IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump :
        {
          ZoneHVACWaterToAirHeatPump component = it->cast<ZoneHVACWaterToAirHeatPump>();
          if (component.supplyAirFan().handle() == this->handle()) return *it;
          break;
        }
      default:
        {
          break;
        }
      }
    }
    return boost::none;
  }

} // detail

FanOnOff::FanOnOff(const Model& model, Schedule& availabilitySchedule)
  : StraightComponent(FanOnOff::iddObjectType(),model)
  {
    OS_ASSERT(getImpl<detail::FanOnOff_Impl>());

    setAvailabilitySchedule(availabilitySchedule);

    bool ok = setFanEfficiency(0.6);
    OS_ASSERT(ok);
    setPressureRise(300);
    autosizeMaximumFlowRate();
    ok = setMotorEfficiency(0.8);
    OS_ASSERT(ok);
    ok = setMotorInAirstreamFraction(1.0);
    OS_ASSERT(ok);

    CurveExponent fanPowerFtSpeedCurve(model);
    fanPowerFtSpeedCurve.setName("Fan On Off Power Curve");
    fanPowerFtSpeedCurve.setCoefficient1Constant(1.0);
    fanPowerFtSpeedCurve.setCoefficient2Constant(0);
    fanPowerFtSpeedCurve.setCoefficient3Constant(0);
    ok = setFanPowerRatioFunctionofSpeedRatioCurve(fanPowerFtSpeedCurve);
    OS_ASSERT(ok);

    CurveCubic fanEfficiencyFtSpeedCurve(model);
    fanEfficiencyFtSpeedCurve.setName("Fan On Off Efficiency Curve");
    fanEfficiencyFtSpeedCurve.setCoefficient1Constant(1.0);
    fanEfficiencyFtSpeedCurve.setCoefficient2x(0.0);
    fanEfficiencyFtSpeedCurve.setCoefficient3xPOW2(0.0);
    fanEfficiencyFtSpeedCurve.setCoefficient4xPOW3(0.0);
    ok = setFanEfficiencyRatioFunctionofSpeedRatioCurve(fanEfficiencyFtSpeedCurve);
    OS_ASSERT(ok);
  }

FanOnOff::FanOnOff(const Model& model,
                   Schedule& availabilitySchedule,
                   Curve& fanPowerRatioFunctionofSpeedRatioCurve,
                   Curve& fanEfficiencyRatioFunctionofSpeedRatioCurve
                  )

  : StraightComponent(FanOnOff::iddObjectType(),model)
  {
    OS_ASSERT(getImpl<detail::FanOnOff_Impl>());

    setAvailabilitySchedule(availabilitySchedule);

    bool ok = setFanEfficiency(0.6);
    OS_ASSERT(ok);
    setPressureRise(300);
    autosizeMaximumFlowRate();
    ok = setMotorEfficiency(0.8);
    OS_ASSERT(ok);
    ok = setMotorInAirstreamFraction(1.0);
    OS_ASSERT(ok);

    ok = setFanPowerRatioFunctionofSpeedRatioCurve(fanPowerRatioFunctionofSpeedRatioCurve);
    OS_ASSERT(ok);

    ok = setFanEfficiencyRatioFunctionofSpeedRatioCurve(fanEfficiencyRatioFunctionofSpeedRatioCurve);
    OS_ASSERT(ok);
  }

IddObjectType FanOnOff::iddObjectType()
{
  return IddObjectType(IddObjectType::OS_Fan_OnOff);
}

// Field Availability Schedule Name

Schedule FanOnOff::availabilitySchedule() const
{
  return getImpl<detail::FanOnOff_Impl>()->availabilitySchedule();
}

bool FanOnOff::setAvailabilitySchedule(Schedule& schedule)
{
  return getImpl<detail::FanOnOff_Impl>()->setAvailabilitySchedule(schedule);
}

// Field Fan Efficiency

double FanOnOff::fanEfficiency() const
{
  return getImpl<detail::FanOnOff_Impl>()->fanEfficiency();
}

bool FanOnOff::isFanEfficiencyDefaulted() const
{
  return getImpl<detail::FanOnOff_Impl>()->isFanEfficiencyDefaulted();
}

bool FanOnOff::setFanEfficiency(double fanEfficiency)
{
  return getImpl<detail::FanOnOff_Impl>()->setFanEfficiency(fanEfficiency);
}

void FanOnOff::resetFanEfficiency()
{
  getImpl<detail::FanOnOff_Impl>()->resetFanEfficiency();
}

// Field Pressure Rise

double FanOnOff::pressureRise() const
{
  return getImpl<detail::FanOnOff_Impl>()->pressureRise();
}

void FanOnOff::setPressureRise(double pressureRise)
{
  getImpl<detail::FanOnOff_Impl>()->setPressureRise(pressureRise);
}

// Field Maximum Flow Rate

boost::optional<double> FanOnOff::maximumFlowRate() const
{
  return getImpl<detail::FanOnOff_Impl>()->maximumFlowRate();
}

bool FanOnOff::isMaximumFlowRateAutosized() const
{
  return getImpl<detail::FanOnOff_Impl>()->isMaximumFlowRateAutosized();
}

bool FanOnOff::setMaximumFlowRate(double maximumFlowRate)
{
  return getImpl<detail::FanOnOff_Impl>()->setMaximumFlowRate(maximumFlowRate);
}

void FanOnOff::resetMaximumFlowRate()
{
  getImpl<detail::FanOnOff_Impl>()->resetMaximumFlowRate();
}

void FanOnOff::autosizeMaximumFlowRate()
{
  getImpl<detail::FanOnOff_Impl>()->autosizeMaximumFlowRate();
}

// Field Motor Efficiency

double FanOnOff::motorEfficiency() const
{
  return getImpl<detail::FanOnOff_Impl>()->motorEfficiency();
}

bool FanOnOff::isMotorEfficiencyDefaulted() const
{
  return getImpl<detail::FanOnOff_Impl>()->isMotorEfficiencyDefaulted();
}

bool FanOnOff::setMotorEfficiency(double motorEfficiency)
{
  return getImpl<detail::FanOnOff_Impl>()->setMotorEfficiency(motorEfficiency);
}

void FanOnOff::resetMotorEfficiency()
{
  getImpl<detail::FanOnOff_Impl>()->resetMotorEfficiency();
}

// Field Motor In Airstream Fraction

boost::optional<double> FanOnOff::motorInAirstreamFraction() const
{
  return getImpl<detail::FanOnOff_Impl>()->motorInAirstreamFraction();
}

bool FanOnOff::setMotorInAirstreamFraction(double motorInAirstreamFraction)
{
  return getImpl<detail::FanOnOff_Impl>()->setMotorInAirstreamFraction(motorInAirstreamFraction);
}

void FanOnOff::resetMotorInAirstreamFraction()
{
  getImpl<detail::FanOnOff_Impl>()->resetMotorInAirstreamFraction();
}

// Field Fan Power Ratio Function of Speed Ratio Curve

Curve FanOnOff::fanPowerRatioFunctionofSpeedRatioCurve() const
{
  return getImpl<detail::FanOnOff_Impl>()->fanPowerRatioFunctionofSpeedRatioCurve();
}

bool FanOnOff::setFanPowerRatioFunctionofSpeedRatioCurve( const Curve& curve )
{
  return getImpl<detail::FanOnOff_Impl>()->setFanPowerRatioFunctionofSpeedRatioCurve( curve );
}

/* Field Fan Efficiency Ratio Function of Speed Ratio Curve */

Curve FanOnOff::fanEfficiencyRatioFunctionofSpeedRatioCurve() const
{
  return getImpl<detail::FanOnOff_Impl>()->fanEfficiencyRatioFunctionofSpeedRatioCurve();
}

bool FanOnOff::setFanEfficiencyRatioFunctionofSpeedRatioCurve( const Curve& curve )
{
  return getImpl<detail::FanOnOff_Impl>()->setFanEfficiencyRatioFunctionofSpeedRatioCurve( curve );
}

std::string FanOnOff::endUseSubcategory() const
{
  return getImpl<detail::FanOnOff_Impl>()->endUseSubcategory();
}

bool FanOnOff::isEndUseSubcategoryDefaulted() const
{
  return getImpl<detail::FanOnOff_Impl>()->isEndUseSubcategoryDefaulted();
}

// Field End-Use Subcategory

void FanOnOff::setEndUseSubcategory(std::string endUseSubcategory)
{
  getImpl<detail::FanOnOff_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void FanOnOff::resetEndUseSubcategory()
{
  getImpl<detail::FanOnOff_Impl>()->resetEndUseSubcategory();
}

/// @cond
FanOnOff::FanOnOff(boost::shared_ptr<detail::FanOnOff_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

