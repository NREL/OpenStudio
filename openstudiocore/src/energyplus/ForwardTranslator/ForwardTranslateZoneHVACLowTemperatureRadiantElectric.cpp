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

#include <energyplus/ForwardTranslator.hpp>
#include <model/Model.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ModelObject.hpp>
#include <model/ModelObject_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/ConstructionWithInternalSource.hpp>
#include <model/ConstructionWithInternalSource_Impl.hpp>
#include <model/Construction.hpp>
#include <model/Construction_Impl.hpp>
#include <model/ZoneHVACLowTemperatureRadiantElectric.hpp>
#include <model/ZoneHVACLowTemperatureRadiantElectric_Impl.hpp>
#include <model/ZoneHVACEquipmentList.hpp>
#include <model/ZoneHVACEquipmentList_Impl.hpp>
#include <model/StraightComponent.hpp>
#include <model/StraightComponent_Impl.hpp>
//#include <model/CoilHeatingLowTempRadiantVarFlow.hpp>
//#include <model/CoilHeatingLowTempRadiantVarFlow_Impl.hpp>
//#include <model/CoilCoolingLowTempRadiantVarFlow.hpp>
//#include <model/CoilCoolingLowTempRadiantVarFlow_Impl.hpp>
#include <model/ZoneHVACComponent.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>

#include <utilities/idf/IdfExtensibleGroup.hpp>


#include <utilities/idd/ZoneHVAC_LowTemperatureRadiant_Electric_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACLowTemperatureRadiantElectric(ZoneHVACLowTemperatureRadiantElectric & modelObject )
{
	boost::optional<std::string> s;
	boost::optional<double> value;
	boost::optional<ModelObject> temp;
	
	IdfObject idfObject(IddObjectType::ZoneHVAC_LowTemperatureRadiant_Electric);
	m_idfObjects.push_back(idfObject);
	
	// Field Name
	std::string baseName = modelObject.name().get();
	idfObject.setName(baseName);
	
	// Field Availability Schedule Name
	if( boost::optional<Schedule> schedule = modelObject.availabilitySchedule() ) 
	{
		if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
		{
			idfObject.setString(ZoneHVAC_LowTemperatureRadiant_ElectricFields::AvailabilityScheduleName,_schedule->name().get());
		}																					
	}
	
	// Field Zone Name
	boost::optional<std::string> thermalZoneName;
	if( boost::optional<ThermalZone> zone = modelObject.thermalZone() )
	{
		if( (s = zone->name()) )
		{
			thermalZoneName = s;

			idfObject.setString(ZoneHVAC_LowTemperatureRadiant_ElectricFields::ZoneName,thermalZoneName.get());
		}
	}
	
	//field Surface Name or Radiant Surface Type

	//create a new surface group object
	IdfObject _surfaceGroup(IddObjectType::ZoneHVAC_LowTemperatureRadiant_SurfaceGroup);

	//create a name for the surface group
	std::string sname = baseName + "" + modelObject.radiantSurfaceType().get();
	_surfaceGroup.setName(sname);
	
	//attach the surface group to the zone low temp radiant object
	idfObject.setString(ZoneHVAC_LowTemperatureRadiant_ElectricFields::SurfaceNameorRadiantSurfaceGroupName,sname);

	//get rid of any existing surface (just to be safe)
	idfObject.clearExtensibleGroups();  
 
	//aggregator for total area; will be used to create weighted area
	double totalAreaOfSurfaces = 0;

	//loop thorugh all surfaces, adding up their area
	BOOST_FOREACH(const Surface& surface, modelObject.surfaces()){
		totalAreaOfSurfaces = totalAreaOfSurfaces + surface.grossArea();
	}

	//loop through all the surfaces, adding them and their flow fractions (weighted per-area)
	BOOST_FOREACH(const Surface& surface, modelObject.surfaces()){
		IdfExtensibleGroup group = _surfaceGroup.pushExtensibleGroup();
		BOOST_ASSERT(group.numFields() == 2);
		group.setString(0, surface.name().get());        
		group.setDouble(1, (surface.grossArea()/totalAreaOfSurfaces) );     
	}
 
	//add the surface group to the list of idf objects
	m_idfObjects.push_back(_surfaceGroup);
	
	// Field Maximum Electrical Power to Panel
	if( modelObject.isMaximumElectricalPowertoPanelAutosized() )
	{
		idfObject.setString(ZoneHVAC_LowTemperatureRadiant_ElectricFields::MaximumElectricalPowertoPanel,"Autosize");
	}
	else if( (value = modelObject.maximumElectricalPowertoPanel()) )
	{
		idfObject.setDouble(ZoneHVAC_LowTemperatureRadiant_ElectricFields::MaximumElectricalPowertoPanel,value.get());
	}
	
	// Field Temperature Control Type
	if(boost::optional<std::string> tempCtrlType= modelObject.temperatureControlType() )
	{
		idfObject.setString(ZoneHVAC_LowTemperatureRadiant_ElectricFields::TemperatureControlType,tempCtrlType.get());
	}
	
	// Field Heating Throttling Range
	if( (value = modelObject.heatingThrottlingRange()) )
	{
		idfObject.setDouble(ZoneHVAC_LowTemperatureRadiant_ElectricFields::HeatingThrottlingRange,value.get());
	}
	
	// Field Heating Setpoint Temperature Schedule Name
	if( boost::optional<Schedule> schedule = modelObject.heatingSetpointTemperatureSchedule() )
	{
		if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
		{
			idfObject.setString(ZoneHVAC_LowTemperatureRadiant_ElectricFields::HeatingSetpointTemperatureScheduleName,_schedule->name().get());
		}
	}
 
 return idfObject;
 
}

} // energyplus

} // openstudio

