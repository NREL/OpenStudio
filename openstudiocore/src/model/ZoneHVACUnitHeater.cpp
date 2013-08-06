/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/ZoneHVACUnitHeater.hpp>
#include <model/ZoneHVACUnitHeater_Impl.hpp>

#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/HVACComponent_Impl.hpp>
#include <model/CoilHeatingWater.hpp>
#include <model/CoilHeatingWater_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneHVAC_UnitHeater_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>
#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACUnitHeater_Impl::ZoneHVACUnitHeater_Impl(const IdfObject& idfObject,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
    {
    BOOST_ASSERT(idfObject.iddObject().type() == ZoneHVACUnitHeater::iddObjectType());
    }

  ZoneHVACUnitHeater_Impl::ZoneHVACUnitHeater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
    {
    BOOST_ASSERT(other.iddObject().type() == ZoneHVACUnitHeater::iddObjectType());
    }

  ZoneHVACUnitHeater_Impl::ZoneHVACUnitHeater_Impl(const ZoneHVACUnitHeater_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  ModelObject ZoneHVACUnitHeater_Impl::clone(Model model) const
  {
  ZoneHVACUnitHeater unitHeaterClone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACUnitHeater>();

  HVACComponent supplyFanClone = this->supplyAirFan().clone(model).cast<HVACComponent>();

  HVACComponent heatingCoilClone = this->heatingCoil().clone(model).cast<HVACComponent>();

  unitHeaterClone.setSupplyAirFan(supplyFanClone);

  unitHeaterClone.setHeatingCoil(heatingCoilClone);

  return unitHeaterClone;
  }

  std::vector<IdfObject> ZoneHVACUnitHeater_Impl::remove()
  {
    if( boost::optional<CoilHeatingWater> waterHeatingCoil =
          heatingCoil().optionalCast<CoilHeatingWater>() )
    {
      if( boost::optional<PlantLoop> plantLoop = waterHeatingCoil->plantLoop() )
      {
        plantLoop->removeDemandBranchWithComponent( waterHeatingCoil.get() );
      }
    }
    return ZoneHVACComponent_Impl::remove();
  }

  const std::vector<std::string>& ZoneHVACUnitHeater_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;

    if (result.empty())
    {
      result.push_back("Zone Unit Heater Heating Rate");
      result.push_back("Zone Unit Heater Heating Energy");
      result.push_back("Zone Unit Heater Fan Electric Power");
      result.push_back("Zone Unit Heater Fan Electric Energy");
      result.push_back("Zone Unit Heater Fan Availability Status");
    }
    return result;
  }

  IddObjectType ZoneHVACUnitHeater_Impl::iddObjectType() const {
    return ZoneHVACUnitHeater::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACUnitHeater_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    
  if (std::find(b,e,OS_ZoneHVAC_UnitHeaterFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACUnitHeater","Availability"));
    }
    return result;
  }

  std::vector<ModelObject> ZoneHVACUnitHeater_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (OptionalHVACComponent intermediate = optionalSupplyAirFan()) 
  {
    result.push_back(*intermediate);
  }
    if (OptionalHVACComponent intermediate = optionalHeatingCoil()) 
  {
      result.push_back(*intermediate);
    }
    return result;
  }

  unsigned ZoneHVACUnitHeater_Impl::inletPort()
  {
    return OS_ZoneHVAC_UnitHeaterFields::AirInletNodeName;
  }

  unsigned ZoneHVACUnitHeater_Impl::outletPort()
  {
    return OS_ZoneHVAC_UnitHeaterFields::AirOutletNodeName;
  }

  Schedule ZoneHVACUnitHeater_Impl::availabilitySchedule() const 
  {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value) 
  {
      LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
    }
    return value.get();
  }

  HVACComponent ZoneHVACUnitHeater_Impl::supplyAirFan() const 
  {
    boost::optional<HVACComponent> value = optionalSupplyAirFan();
    if (!value) 
  {
      LOG_AND_THROW(briefDescription() << " does not have an Supply Air Fan attached.");
    }
    return value.get();
  }

  boost::optional<double> ZoneHVACUnitHeater_Impl::maximumSupplyAirFlowRate() const  
  {
    return getDouble(OS_ZoneHVAC_UnitHeaterFields::MaximumSupplyAirFlowRate,true);
  }

  bool ZoneHVACUnitHeater_Impl::isMaximumSupplyAirFlowRateAutosized() const 
  {
    
  bool result = false;
  boost::optional<std::string> value = getString(OS_ZoneHVAC_UnitHeaterFields::MaximumSupplyAirFlowRate, true);
    
  if (value) 
  {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  std::string ZoneHVACUnitHeater_Impl::fanControlType() const 
  {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_UnitHeaterFields::FanControlType,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  HVACComponent ZoneHVACUnitHeater_Impl::heatingCoil() const 
  {
    boost::optional<HVACComponent> value = optionalHeatingCoil();
    if (!value) 
  {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Coil attached.");
    }
    return value.get();
  }

  boost::optional<double> ZoneHVACUnitHeater_Impl::maximumHotWaterFlowRate() const 
  {
    return getDouble(OS_ZoneHVAC_UnitHeaterFields::MaximumHotWaterFlowRate,true);
  }

  bool ZoneHVACUnitHeater_Impl::isMaximumHotWaterFlowRateAutosized() const 
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_UnitHeaterFields::MaximumHotWaterFlowRate, true);
    if (value) 
  {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ZoneHVACUnitHeater_Impl::minimumHotWaterFlowRate() const 
  {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_UnitHeaterFields::MinimumHotWaterFlowRate,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACUnitHeater_Impl::isMinimumHotWaterFlowRateDefaulted() const 
  {
    return isEmpty(OS_ZoneHVAC_UnitHeaterFields::MinimumHotWaterFlowRate);
  }

  double ZoneHVACUnitHeater_Impl::heatingConvergenceTolerance() const 
  {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_UnitHeaterFields::HeatingConvergenceTolerance,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACUnitHeater_Impl::isHeatingConvergenceToleranceDefaulted() const 
  {
    return isEmpty(OS_ZoneHVAC_UnitHeaterFields::HeatingConvergenceTolerance);
  }

  bool ZoneHVACUnitHeater_Impl::setAvailabilitySchedule(Schedule& schedule) 
  {
    bool result = setSchedule(OS_ZoneHVAC_UnitHeaterFields::AvailabilityScheduleName,
                              "ZoneHVACUnitHeater",
                              "Availability",
                              schedule);
    return result;
  }

  bool ZoneHVACUnitHeater_Impl::setSupplyAirFan(const HVACComponent& HVACComponent) 
  {
    bool result = setPointer(OS_ZoneHVAC_UnitHeaterFields::SupplyAirFanName, HVACComponent.handle());
    return result;
  }

  bool ZoneHVACUnitHeater_Impl::setMaximumSupplyAirFlowRate(boost::optional<double> maximumSupplyAirFlowRate) 
  {
    bool result(false);
    if (maximumSupplyAirFlowRate) 
  {
      result = setDouble(OS_ZoneHVAC_UnitHeaterFields::MaximumSupplyAirFlowRate, maximumSupplyAirFlowRate.get());
    }

    return result;
  }

  void ZoneHVACUnitHeater_Impl::autosizeMaximumSupplyAirFlowRate() 
  {
    bool result = setString(OS_ZoneHVAC_UnitHeaterFields::MaximumSupplyAirFlowRate, "autosize");
    BOOST_ASSERT(result);
  }

  bool ZoneHVACUnitHeater_Impl::setFanControlType(std::string fanControlType) 
  {
    bool result = setString(OS_ZoneHVAC_UnitHeaterFields::FanControlType, fanControlType);
    return result;
  }

  bool ZoneHVACUnitHeater_Impl::setHeatingCoil(const HVACComponent & heatingCoil) 
  {
    bool result = setPointer(OS_ZoneHVAC_UnitHeaterFields::HeatingCoilName, heatingCoil.handle());
    return result;
  }

  bool ZoneHVACUnitHeater_Impl::setMaximumHotWaterFlowRate(boost::optional<double> maximumHotWaterFlowRate) 
  {
    bool result(false);
    if (maximumHotWaterFlowRate) 
  {
      result = setDouble(OS_ZoneHVAC_UnitHeaterFields::MaximumHotWaterFlowRate, maximumHotWaterFlowRate.get());
    }
    else 
  {
      resetMaximumHotWaterFlowRate();
      result = true;
    }
    
  return result;
  }

  void ZoneHVACUnitHeater_Impl::resetMaximumHotWaterFlowRate() 
  {
    bool result = setString(OS_ZoneHVAC_UnitHeaterFields::MaximumHotWaterFlowRate, "");
    BOOST_ASSERT(result);
  }

  void ZoneHVACUnitHeater_Impl::autosizeMaximumHotWaterFlowRate() 
  {
    bool result = setString(OS_ZoneHVAC_UnitHeaterFields::MaximumHotWaterFlowRate, "autosize");
    BOOST_ASSERT(result);
  }

  bool ZoneHVACUnitHeater_Impl::setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate) 
  {
    bool result = setDouble(OS_ZoneHVAC_UnitHeaterFields::MinimumHotWaterFlowRate, minimumHotWaterFlowRate);
    return result;
  }

  void ZoneHVACUnitHeater_Impl::resetMinimumHotWaterFlowRate() 
  {
    bool result = setString(OS_ZoneHVAC_UnitHeaterFields::MinimumHotWaterFlowRate, "");
    BOOST_ASSERT(result);
  }

  bool ZoneHVACUnitHeater_Impl::setHeatingConvergenceTolerance(double heatingConvergenceTolerance) 
  {
    bool result = setDouble(OS_ZoneHVAC_UnitHeaterFields::HeatingConvergenceTolerance, heatingConvergenceTolerance);
    return result;
  }

  void ZoneHVACUnitHeater_Impl::resetHeatingConvergenceTolerance() 
  {
    bool result = setString(OS_ZoneHVAC_UnitHeaterFields::HeatingConvergenceTolerance, "");
    BOOST_ASSERT(result);
  }

  boost::optional<Schedule> ZoneHVACUnitHeater_Impl::optionalAvailabilitySchedule() const 
  {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_UnitHeaterFields::AvailabilityScheduleName);
  }

  boost::optional<HVACComponent> ZoneHVACUnitHeater_Impl::optionalSupplyAirFan() const 
  {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_UnitHeaterFields::SupplyAirFanName);
  }

  boost::optional<HVACComponent> ZoneHVACUnitHeater_Impl::optionalHeatingCoil() const 
  {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_UnitHeaterFields::HeatingCoilName);
  }

  std::vector<std::string> ZoneHVACUnitHeater_Impl::fanControlTypeValues() const 
  {
    return ZoneHVACUnitHeater::fanControlTypeValues();
  }

  boost::optional<ModelObject> ZoneHVACUnitHeater_Impl::availabilityScheduleAsModelObject() const 
  {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  boost::optional<ModelObject> ZoneHVACUnitHeater_Impl::supplyAirFanAsModelObject() const 
  {
    OptionalModelObject result = supplyAirFan();
    return result;
  }

  boost::optional<ModelObject> ZoneHVACUnitHeater_Impl::heatingCoilAsModelObject() const 
  {
    OptionalModelObject result = heatingCoil();
    return result;
  }

  bool ZoneHVACUnitHeater_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) 
  {
    if (modelObject) 
  {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      
    if (intermediate) 
    {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  bool ZoneHVACUnitHeater_Impl::setSupplyAirFanAsModelObject(const boost::optional<ModelObject>& modelObject) 
  {
    if (modelObject) 
  {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) 
    {
        HVACComponent HVACComponent(*intermediate);
        return setSupplyAirFan(HVACComponent);
      }
    }
    return false;
  }

  bool ZoneHVACUnitHeater_Impl::setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) 
  {
    if (modelObject) 
  {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) 
    {
        setHeatingCoil(*intermediate);
        return true;
      }
    }
    return false;
  }
} // detail

ZoneHVACUnitHeater::ZoneHVACUnitHeater(const Model& model,
                     Schedule & availabilitySchedule,
                                       HVACComponent & supplyAirFan,
                     HVACComponent & heatingCoil)
  : ZoneHVACComponent(ZoneHVACUnitHeater::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::ZoneHVACUnitHeater_Impl>());

  bool ok = true;
      
  ok = setAvailabilitySchedule(availabilitySchedule);
     
  if (!ok) 
  {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to "
                << availabilitySchedule.briefDescription() << ".");
  }

  ok = setSupplyAirFan(supplyAirFan);
  BOOST_ASSERT(ok);

  ok = setHeatingCoil(heatingCoil);
  BOOST_ASSERT(ok);

  autosizeMaximumSupplyAirFlowRate();

  setFanControlType("OnOff");

  setMinimumHotWaterFlowRate(0.0);

  setHeatingConvergenceTolerance(0.001);

  setString(OS_ZoneHVAC_UnitHeaterFields::AvailabilityManagerListName,"");
}

IddObjectType ZoneHVACUnitHeater::iddObjectType() 
{
  return IddObjectType(IddObjectType::OS_ZoneHVAC_UnitHeater);
}

std::vector<std::string> ZoneHVACUnitHeater::fanControlTypeValues() 
{
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_UnitHeaterFields::FanControlType);
}

Schedule ZoneHVACUnitHeater::availabilitySchedule() const 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->availabilitySchedule();
}

HVACComponent ZoneHVACUnitHeater::supplyAirFan() const 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->supplyAirFan();
}

boost::optional<double> ZoneHVACUnitHeater::maximumSupplyAirFlowRate() const 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->maximumSupplyAirFlowRate();
}

bool ZoneHVACUnitHeater::isMaximumSupplyAirFlowRateAutosized() const 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->isMaximumSupplyAirFlowRateAutosized();
}

std::string ZoneHVACUnitHeater::fanControlType() const 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->fanControlType();
}

HVACComponent ZoneHVACUnitHeater::heatingCoil() const 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->heatingCoil();
}

boost::optional<double> ZoneHVACUnitHeater::maximumHotWaterFlowRate() const 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->maximumHotWaterFlowRate();
}

bool ZoneHVACUnitHeater::isMaximumHotWaterFlowRateAutosized() const 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->isMaximumHotWaterFlowRateAutosized();
}

double ZoneHVACUnitHeater::minimumHotWaterFlowRate() const 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->minimumHotWaterFlowRate();
}

bool ZoneHVACUnitHeater::isMinimumHotWaterFlowRateDefaulted() const 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->isMinimumHotWaterFlowRateDefaulted();
}

double ZoneHVACUnitHeater::heatingConvergenceTolerance() const 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->heatingConvergenceTolerance();
}

bool ZoneHVACUnitHeater::isHeatingConvergenceToleranceDefaulted() const 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->isHeatingConvergenceToleranceDefaulted();
}

bool ZoneHVACUnitHeater::setAvailabilitySchedule(Schedule& schedule) 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->setAvailabilitySchedule(schedule);
}

bool ZoneHVACUnitHeater::setSupplyAirFan(const HVACComponent& HVACComponent) 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->setSupplyAirFan(HVACComponent);
}

bool ZoneHVACUnitHeater::setMaximumSupplyAirFlowRate(double maximumSupplyAirFlowRate) 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->setMaximumSupplyAirFlowRate(maximumSupplyAirFlowRate);
}

void ZoneHVACUnitHeater::autosizeMaximumSupplyAirFlowRate() 
{
  getImpl<detail::ZoneHVACUnitHeater_Impl>()->autosizeMaximumSupplyAirFlowRate();
}

bool ZoneHVACUnitHeater::setFanControlType(std::string fanControlType) 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->setFanControlType(fanControlType);
}

bool ZoneHVACUnitHeater::setHeatingCoil(const HVACComponent & heatingCoil) {
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->setHeatingCoil(heatingCoil);
}

bool ZoneHVACUnitHeater::setMaximumHotWaterFlowRate(double maximumHotWaterFlowRate) 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->setMaximumHotWaterFlowRate(maximumHotWaterFlowRate);
}

void ZoneHVACUnitHeater::resetMaximumHotWaterFlowRate() 
{
  getImpl<detail::ZoneHVACUnitHeater_Impl>()->resetMaximumHotWaterFlowRate();
}

void ZoneHVACUnitHeater::autosizeMaximumHotWaterFlowRate() 
{
  getImpl<detail::ZoneHVACUnitHeater_Impl>()->autosizeMaximumHotWaterFlowRate();
}

bool ZoneHVACUnitHeater::setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate) 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->setMinimumHotWaterFlowRate(minimumHotWaterFlowRate);
}

void ZoneHVACUnitHeater::resetMinimumHotWaterFlowRate() 
{
  getImpl<detail::ZoneHVACUnitHeater_Impl>()->resetMinimumHotWaterFlowRate();
}

bool ZoneHVACUnitHeater::setHeatingConvergenceTolerance(double heatingConvergenceTolerance) 
{
  return getImpl<detail::ZoneHVACUnitHeater_Impl>()->setHeatingConvergenceTolerance(heatingConvergenceTolerance);
}

void ZoneHVACUnitHeater::resetHeatingConvergenceTolerance() 
{
  getImpl<detail::ZoneHVACUnitHeater_Impl>()->resetHeatingConvergenceTolerance();
}

ZoneHVACUnitHeater::ZoneHVACUnitHeater(boost::shared_ptr<detail::ZoneHVACUnitHeater_Impl> impl)
  : ZoneHVACComponent(impl)
{}

} // model
} // openstudio

