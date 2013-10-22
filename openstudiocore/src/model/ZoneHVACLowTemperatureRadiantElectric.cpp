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

#include <model/ConstructionWithInternalSource.hpp>
#include <model/ConstructionWithInternalSource_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/HVACComponent_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/ZoneHVACLowTemperatureRadiantElectric.hpp>
#include <model/ZoneHVACLowTemperatureRadiantElectric_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneHVAC_LowTemperatureRadiant_Electric_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>
#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

	ZoneHVACLowTemperatureRadiantElectric_Impl::ZoneHVACLowTemperatureRadiantElectric_Impl(const IdfObject& idfObject,
																																												 Model_Impl* model,
																																												 bool keepHandle)
		: ZoneHVACComponent_Impl(idfObject,model,keepHandle)
				{
		BOOST_ASSERT(idfObject.iddObject().type() == ZoneHVACLowTemperatureRadiantElectric::iddObjectType());
				}

	ZoneHVACLowTemperatureRadiantElectric_Impl::ZoneHVACLowTemperatureRadiantElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
																																												 Model_Impl* model,
																																												 bool keepHandle)
		: ZoneHVACComponent_Impl(other,model,keepHandle)
				{
		BOOST_ASSERT(other.iddObject().type() == ZoneHVACLowTemperatureRadiantElectric::iddObjectType());
				}

	ZoneHVACLowTemperatureRadiantElectric_Impl::ZoneHVACLowTemperatureRadiantElectric_Impl(const ZoneHVACLowTemperatureRadiantElectric_Impl& other,
																																												 Model_Impl* model,
																																												 bool keepHandle)
		: ZoneHVACComponent_Impl(other,model,keepHandle)
				{}
				
		ModelObject ZoneHVACLowTemperatureRadiantElectric_Impl::clone(Model model) const
	{
				ZoneHVACLowTemperatureRadiantElectric zoneHVACLowTemperatureRadiantElectricClone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACLowTemperatureRadiantElectric>();

				return zoneHVACLowTemperatureRadiantElectricClone;
	}

	const std::vector<std::string>& ZoneHVACLowTemperatureRadiantElectric_Impl::outputVariableNames() const
	{
		static std::vector<std::string> result;
		if (result.empty())
		{
		}
		return result;
	}

	IddObjectType ZoneHVACLowTemperatureRadiantElectric_Impl::iddObjectType() const 
	{
		return ZoneHVACLowTemperatureRadiantElectric::iddObjectType();
	}

	std::vector<ScheduleTypeKey> ZoneHVACLowTemperatureRadiantElectric_Impl::getScheduleTypeKeys(const Schedule& schedule) const
	{
		std::vector<ScheduleTypeKey> result;
		UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
		UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
		if (std::find(b,e,OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::AvailabilityScheduleName) != e)
		{
			result.push_back(ScheduleTypeKey("ZoneHVACLowTemperatureRadiantElectric","Availability"));
		}
		if (std::find(b,e,OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::HeatingSetpointTemperatureScheduleName) != e)
		{
			result.push_back(ScheduleTypeKey("ZoneHVACLowTemperatureRadiantElectric","Heating Setpoint Temperature"));
		}
		return result;
	}

	unsigned ZoneHVACLowTemperatureRadiantElectric_Impl::inletPort()
	{
		return (0); // this object has no inlet or outlet node
	}

	unsigned ZoneHVACLowTemperatureRadiantElectric_Impl::outletPort()
	{
		return (0); // this object has no inlet or outlet node
	}

	Schedule ZoneHVACLowTemperatureRadiantElectric_Impl::availabilitySchedule() const 
	{
		boost::optional<Schedule> value = optionalAvailabilitySchedule();
		if (!value) 
		{
			// it is an error if we get here, however we don't want to crash
			// so we hook up to global always on schedule
			LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
			value = this->model().alwaysOnDiscreteSchedule();
			BOOST_ASSERT(value);
			const_cast<ZoneHVACLowTemperatureRadiantElectric_Impl*>(this)->setAvailabilitySchedule(*value);
			value = optionalAvailabilitySchedule();
		}
		BOOST_ASSERT(value);
		return value.get();
	}
	
	Schedule ZoneHVACLowTemperatureRadiantElectric_Impl::heatingSetpointTemperatureSchedule() const 
	{
		boost::optional<Schedule> value = optionalHeatingSetpointTemperatureSchedule();
		
		if (!value) 
		{
		//  // it is an error if we get here, however we don't want to crash
		//  // so we hook up to global temperature schedule
			LOG(Error, "Required heating temperature setpoint schedule not set");
		//  value = this->model().alwaysOnDiscreteSchedule();
		//  BOOST_ASSERT(value);
		//  const_cast<ZoneHVACLowTemperatureRadiantElectric_Impl*>(this)->setAvailabilitySchedule(*value);
		//  value = optionalAvailabilitySchedule();
		}
		//BOOST_ASSERT(value);
		return value.get();
	}

	boost::optional<std::string> ZoneHVACLowTemperatureRadiantElectric_Impl::radiantSurfaceType() const 
	{
		return getString(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::RadiantSurfaceType,true);
	}

	std::vector<Surface> ZoneHVACLowTemperatureRadiantElectric_Impl::surfaces() const {    
		
		//vector to hold all of the surfaces that this radiant system is attached to
		std::vector<Surface> surfaces;

		//get the thermal zone this equipment belongs to
		if (boost::optional<ThermalZone> thermalZone = this->thermalZone()) {

			//loop through all the spaces in this zone
			BOOST_FOREACH(const Space& space, thermalZone->spaces()){
		
				//loop through all the surfaces in this space
				BOOST_FOREACH(const Surface& surface, space.surfaces()){

					//skip surfaces whose construction is not internal source
					if(boost::optional<ConstructionWithInternalSource> construction = surface.construction()->optionalCast<ConstructionWithInternalSource>()){
				
						//TODO change this to not optional when idd change is made
						//get the strings for requested surface types and current surface type
						std::string surfGrpName = this->radiantSurfaceType().get();
						std::string surfaceType = surface.surfaceType();

						//if the current surface is of the type requested, add it to the vector of surfaces
						if(istringEqual("RoofCeiling", surfaceType) && istringEqual("Ceilings",surfGrpName)){
							surfaces.push_back(surface);
						}
						else if(istringEqual("Floor", surfaceType) && istringEqual("Floors",surfGrpName)){
							surfaces.push_back(surface);
						}
						else if(istringEqual("Floor", surfaceType) || (istringEqual("Ceiling", surfaceType) && istringEqual("CeilingsandFloors",surfGrpName))){
							surfaces.push_back(surface);
						}
						else if(istringEqual("AllSurfaces",surfGrpName)){
							surfaces.push_back(surface);
						}
					}
				}
			}    
		}

		return surfaces;
	}

	boost::optional<double> ZoneHVACLowTemperatureRadiantElectric_Impl::maximumElectricalPowertoPanel() const 
	{
		return getDouble(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::MaximumElectricalPowertoPanel,true);
	}

	bool ZoneHVACLowTemperatureRadiantElectric_Impl::isMaximumElectricalPowertoPanelDefaulted() const 
	{
		return isEmpty(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::MaximumElectricalPowertoPanel);
	}

	bool ZoneHVACLowTemperatureRadiantElectric_Impl::isMaximumElectricalPowertoPanelAutosized() const 
	{
		bool result = false;
		boost::optional<std::string> value = getString(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::MaximumElectricalPowertoPanel, true);
		if (value) 
		{
			result = openstudio::istringEqual(value.get(), "autosize");
		}
		return result;
	}

	std::string ZoneHVACLowTemperatureRadiantElectric_Impl::temperatureControlType() const 
	{
		boost::optional<std::string> value = getString(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::TemperatureControlType,true);
		BOOST_ASSERT(value);
		return value.get();
	}

	bool ZoneHVACLowTemperatureRadiantElectric_Impl::isTemperatureControlTypeDefaulted() const 
	{
		return isEmpty(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::TemperatureControlType);
	}

	double ZoneHVACLowTemperatureRadiantElectric_Impl::heatingThrottlingRange() const 
	{
		boost::optional<double> value = getDouble(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::HeatingThrottlingRange,true);
		BOOST_ASSERT(value);
		return value.get();
	}

	bool ZoneHVACLowTemperatureRadiantElectric_Impl::isHeatingThrottlingRangeDefaulted() const 
	{
		return isEmpty(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::HeatingThrottlingRange);
	}

	bool ZoneHVACLowTemperatureRadiantElectric_Impl::setAvailabilitySchedule(Schedule& schedule) 
	{
		bool result = setSchedule(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::AvailabilityScheduleName,
															"ZoneHVACLowTemperatureRadiantElectric",
															"Availability",
															schedule);
		return result;
	}
	
	bool ZoneHVACLowTemperatureRadiantElectric_Impl::setHeatingSetpointTemperatureSchedule(Schedule& schedule) 
	{
		bool result = setSchedule(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::HeatingSetpointTemperatureScheduleName,
															"ZoneHVACLowTemperatureRadiantElectric",
															"Heating Setpoint Temperature",
															schedule);
		return result;
	}

	//void ZoneHVACLowTemperatureRadiantElectric_Impl::resetHeatingSetpointTemperatureSchedule() 
	//{
	//  bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::HeatingSetpointTemperatureScheduleName, "");
	//  BOOST_ASSERT(result);
	//}

	bool ZoneHVACLowTemperatureRadiantElectric_Impl::setRadiantSurfaceType(boost::optional<std::string> radiantSurfaceType) 
	{
		bool result(false);
		if (radiantSurfaceType) 
		{
			result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::RadiantSurfaceType, radiantSurfaceType.get());
		}
		else 
		{
			resetRadiantSurfaceType();
			result = true;
		}
		return result;
	}

	void ZoneHVACLowTemperatureRadiantElectric_Impl::resetRadiantSurfaceType() 
	{
		bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::RadiantSurfaceType, "Ceilings");
		BOOST_ASSERT(result);
	}

	bool ZoneHVACLowTemperatureRadiantElectric_Impl::setMaximumElectricalPowertoPanel(boost::optional<double> maximumElectricalPowertoPanel) 
	{
		bool result(false);
		if (maximumElectricalPowertoPanel) 
		{
			result = setDouble(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::MaximumElectricalPowertoPanel, maximumElectricalPowertoPanel.get());
		}
		else 
		{
			resetMaximumElectricalPowertoPanel();
			result = true;
		}
		return result;
	}

	void ZoneHVACLowTemperatureRadiantElectric_Impl::resetMaximumElectricalPowertoPanel() 
	{
		bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::MaximumElectricalPowertoPanel, "");
		BOOST_ASSERT(result);
	}

	void ZoneHVACLowTemperatureRadiantElectric_Impl::autosizeMaximumElectricalPowertoPanel() 
	{
		bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::MaximumElectricalPowertoPanel, "autosize");
		BOOST_ASSERT(result);
	}

	bool ZoneHVACLowTemperatureRadiantElectric_Impl::setTemperatureControlType(std::string temperatureControlType) 
	{
		bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::TemperatureControlType, temperatureControlType);
		return result;
	}

	void ZoneHVACLowTemperatureRadiantElectric_Impl::resetTemperatureControlType() 
	{
		bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::TemperatureControlType, "");
		BOOST_ASSERT(result);
	}

	bool ZoneHVACLowTemperatureRadiantElectric_Impl::setHeatingThrottlingRange(double heatingThrottlingRange) 
	{
		bool result = setDouble(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::HeatingThrottlingRange, heatingThrottlingRange);
		return result;
	}

	void ZoneHVACLowTemperatureRadiantElectric_Impl::resetHeatingThrottlingRange() 
	{
		bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::HeatingThrottlingRange, "");
		BOOST_ASSERT(result);
	}

	boost::optional<Schedule> ZoneHVACLowTemperatureRadiantElectric_Impl::optionalAvailabilitySchedule() const 
	{
		return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::AvailabilityScheduleName);
	}
	
	boost::optional<Schedule> ZoneHVACLowTemperatureRadiantElectric_Impl::optionalHeatingSetpointTemperatureSchedule() const 
	{
		return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::HeatingSetpointTemperatureScheduleName);
	}

	std::vector<std::string> ZoneHVACLowTemperatureRadiantElectric_Impl::radiantSurfaceTypeValues() const 
	{
		return ZoneHVACLowTemperatureRadiantElectric::radiantSurfaceTypeValues();
	}

	std::vector<std::string> ZoneHVACLowTemperatureRadiantElectric_Impl::temperatureControlTypeValues() const 
	{
		return ZoneHVACLowTemperatureRadiantElectric::temperatureControlTypeValues();
	}
	
	boost::optional<ThermalZone> ZoneHVACLowTemperatureRadiantElectric_Impl::thermalZone() const
	{
		boost::optional<ThermalZone> result;
		Model m = this->model();
		std::vector<ThermalZone> thermalZones = m.getModelObjects<ThermalZone>();
		BOOST_FOREACH(ThermalZone& thermalZone, thermalZones){
			std::vector<ModelObject> equipments = thermalZone.equipment(); 
			BOOST_FOREACH(ModelObject& equipment, equipments){
				if (equipment.handle() == this->handle()){
					result = thermalZone;
				}
			}
		}

		return result;
	}

	//reimplemented to override the base-class method in ZoneHVACComponent
	//because this component doesn't get attached to the zone inlet and zone outlet nodes
	bool ZoneHVACLowTemperatureRadiantElectric_Impl::addToThermalZone(ThermalZone & thermalZone)
	{
		Model m = this->model();

		if( thermalZone.model() != m )
		{
			return false;
		}

		removeFromThermalZone();

		thermalZone.setUseIdealAirLoads(false);

		thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

		return true;
	}

	//reimplemented to override the base-class method in ZoneHVACComponent
	//because this component doesn't get attached to the zone inlet and zone outlet nodes
	//and therefore doesn't need to be removed from them when removed from the zone
	void ZoneHVACLowTemperatureRadiantElectric_Impl::removeFromThermalZone()
	{
		boost::optional<ThermalZone> thermalZone = this->thermalZone();
		Model m = this->model();
		ModelObject thisObject = this->getObject<ModelObject>();
		std::vector<ThermalZone> thermalZones = m.getModelObjects<ThermalZone>();
		for( std::vector<ThermalZone>::iterator it = thermalZones.begin();
				 it != thermalZones.end();
				 it++ )
		{
			std::vector<ModelObject> equipment = it->equipment();

			if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() )
			{
				it->removeEquipment(thisObject);

				break;
			}
		}
	}

} // detail

ZoneHVACLowTemperatureRadiantElectric::ZoneHVACLowTemperatureRadiantElectric(const Model& model, Schedule & availabilitySchedule, Schedule & heatingTemperatureSchedule)
	: ZoneHVACComponent(ZoneHVACLowTemperatureRadiantElectric::iddObjectType(),model)
{
	BOOST_ASSERT(getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>());


	bool ok = true;
	
	ok = setAvailabilitySchedule(availabilitySchedule);
		 
	if (!ok) 
	{
		//remove();
		LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to "
								<< availabilitySchedule.briefDescription() << ".");
	}
	
	ok = setHeatingSetpointTemperatureSchedule(heatingTemperatureSchedule);
		 
	if (!ok) 
	{
		//remove();
		//LOG_AND_THROW("Unable to set " << briefDescription() << "'s heating temperature schedule to "
		//            << schedule.briefDescription() << ".");
	}
	
	resetRadiantSurfaceType();
	autosizeMaximumElectricalPowertoPanel();
	setTemperatureControlType("MeanAirTemperature");
	setHeatingThrottlingRange(2.0);
	//setString(OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::HeatingSetpointTemperatureScheduleName,"");
	
}

IddObjectType ZoneHVACLowTemperatureRadiantElectric::iddObjectType() 
{
	return IddObjectType(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_Electric);
}

std::vector<std::string> ZoneHVACLowTemperatureRadiantElectric::radiantSurfaceTypeValues() 
{
	return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
												OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::RadiantSurfaceType);
}

std::vector<std::string> ZoneHVACLowTemperatureRadiantElectric::temperatureControlTypeValues() 
{
	return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
												OS_ZoneHVAC_LowTemperatureRadiant_ElectricFields::TemperatureControlType);
}

Schedule ZoneHVACLowTemperatureRadiantElectric::availabilitySchedule() const 
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->availabilitySchedule();
}

Schedule ZoneHVACLowTemperatureRadiantElectric::heatingSetpointTemperatureSchedule() const 
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->heatingSetpointTemperatureSchedule();
}

boost::optional<std::string> ZoneHVACLowTemperatureRadiantElectric::radiantSurfaceType() const 
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->radiantSurfaceType();
}

std::vector<Surface> ZoneHVACLowTemperatureRadiantElectric::surfaces() const {
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->surfaces();
}

boost::optional<double> ZoneHVACLowTemperatureRadiantElectric::maximumElectricalPowertoPanel() const 
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->maximumElectricalPowertoPanel();
}

bool ZoneHVACLowTemperatureRadiantElectric::isMaximumElectricalPowertoPanelDefaulted() const 
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->isMaximumElectricalPowertoPanelDefaulted();
}

bool ZoneHVACLowTemperatureRadiantElectric::isMaximumElectricalPowertoPanelAutosized() const 
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->isMaximumElectricalPowertoPanelAutosized();
}

std::string ZoneHVACLowTemperatureRadiantElectric::temperatureControlType() const 
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->temperatureControlType();
}

bool ZoneHVACLowTemperatureRadiantElectric::isTemperatureControlTypeDefaulted() const 
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->isTemperatureControlTypeDefaulted();
}

double ZoneHVACLowTemperatureRadiantElectric::heatingThrottlingRange() const 
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->heatingThrottlingRange();
}

bool ZoneHVACLowTemperatureRadiantElectric::isHeatingThrottlingRangeDefaulted() const 
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->isHeatingThrottlingRangeDefaulted();
}

bool ZoneHVACLowTemperatureRadiantElectric::setAvailabilitySchedule(Schedule& schedule) 
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->setAvailabilitySchedule(schedule);
}

bool ZoneHVACLowTemperatureRadiantElectric::setHeatingSetpointTemperatureSchedule(Schedule& schedule) 
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->setHeatingSetpointTemperatureSchedule(schedule);
}

bool ZoneHVACLowTemperatureRadiantElectric::setRadiantSurfaceType(std::string radiantSurfaceType) 
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->setRadiantSurfaceType(radiantSurfaceType);
}

void ZoneHVACLowTemperatureRadiantElectric::resetRadiantSurfaceType() 
{
	getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->resetRadiantSurfaceType();
}

bool ZoneHVACLowTemperatureRadiantElectric::setMaximumElectricalPowertoPanel(double maximumElectricalPowertoPanel) 
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->setMaximumElectricalPowertoPanel(maximumElectricalPowertoPanel);
}

void ZoneHVACLowTemperatureRadiantElectric::resetMaximumElectricalPowertoPanel() 
{
	getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->resetMaximumElectricalPowertoPanel();
}

void ZoneHVACLowTemperatureRadiantElectric::autosizeMaximumElectricalPowertoPanel() 
{
	getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->autosizeMaximumElectricalPowertoPanel();
}

bool ZoneHVACLowTemperatureRadiantElectric::setTemperatureControlType(std::string temperatureControlType) 
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->setTemperatureControlType(temperatureControlType);
}

void ZoneHVACLowTemperatureRadiantElectric::resetTemperatureControlType() 
{
	getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->resetTemperatureControlType();
}

bool ZoneHVACLowTemperatureRadiantElectric::setHeatingThrottlingRange(double heatingThrottlingRange) 
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->setHeatingThrottlingRange(heatingThrottlingRange);
}

void ZoneHVACLowTemperatureRadiantElectric::resetHeatingThrottlingRange() 
{
	getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->resetHeatingThrottlingRange();
}

boost::optional<ThermalZone> ZoneHVACLowTemperatureRadiantElectric::thermalZone() const
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->thermalZone();
}

bool ZoneHVACLowTemperatureRadiantElectric::addToThermalZone(ThermalZone & thermalZone)
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->addToThermalZone(thermalZone);
}

void ZoneHVACLowTemperatureRadiantElectric::removeFromThermalZone()
{
	return getImpl<detail::ZoneHVACLowTemperatureRadiantElectric_Impl>()->removeFromThermalZone();
}

/// @cond
ZoneHVACLowTemperatureRadiantElectric::ZoneHVACLowTemperatureRadiantElectric(boost::shared_ptr<detail::ZoneHVACLowTemperatureRadiantElectric_Impl> impl)
	: ZoneHVACComponent(impl)
{}
/// @endcond

} // model
} // openstudio

