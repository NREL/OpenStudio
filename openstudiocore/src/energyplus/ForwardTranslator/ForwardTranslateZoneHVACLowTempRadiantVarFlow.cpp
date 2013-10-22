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
#include <model/ZoneHVACLowTempRadiantVarFlow.hpp>
#include <model/ZoneHVACLowTempRadiantVarFlow_Impl.hpp>
#include <model/ZoneHVACEquipmentList.hpp>
#include <model/ZoneHVACEquipmentList_Impl.hpp>
#include <model/StraightComponent.hpp>
#include <model/StraightComponent_Impl.hpp>
#include <model/CoilHeatingLowTempRadiantVarFlow.hpp>
#include <model/CoilHeatingLowTempRadiantVarFlow_Impl.hpp>
#include <model/CoilCoolingLowTempRadiantVarFlow.hpp>
#include <model/CoilCoolingLowTempRadiantVarFlow_Impl.hpp>
#include <model/ZoneHVACComponent.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>

#include <utilities/idf/IdfExtensibleGroup.hpp>


#include <utilities/idd/ZoneHVAC_LowTemperatureRadiant_VariableFlow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACLowTempRadiantVarFlow(ZoneHVACLowTempRadiantVarFlow& modelObject)
{
	boost::optional<std::string> s;
	boost::optional<double> value;
	boost::optional<ModelObject> temp;

	IdfObject idfObject(IddObjectType::ZoneHVAC_LowTemperatureRadiant_VariableFlow);
	m_idfObjects.push_back(idfObject);

	//Name
	std::string baseName = modelObject.name().get();
	idfObject.setName(baseName);
	
	// AvailabilityScheduleName
	if( boost::optional<Schedule> schedule = modelObject.availabilitySchedule() )
	{
		if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
		{
			idfObject.setString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::AvailabilityScheduleName,_schedule->name().get());
		}
	}
	
	//field Zone Name
	boost::optional<std::string> thermalZoneName;
	if( boost::optional<ThermalZone> zone = modelObject.thermalZone() )
	{
		if( (s = zone->name()) )
		{
			thermalZoneName = s;

			idfObject.setString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::ZoneName,thermalZoneName.get());
		}
	}
 
	//field Surface Name or Radiant Surface Type

	//create a new surface group object
	IdfObject _surfaceGroup(IddObjectType::ZoneHVAC_LowTemperatureRadiant_SurfaceGroup);

	//create a name for the surface group
	std::string sname = baseName + "" + modelObject.radiantSurfaceType().get();
	_surfaceGroup.setName(sname);
	
	//attach the surface group to the zone low temp radiant object
	idfObject.setString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::SurfaceNameorRadiantSurfaceGroupName,sname);

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

 
	//field Hydronic Tubing Inside Diameter
	if( (value = modelObject.hydronicTubingInsideDiameter()) )
	{
		idfObject.setDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HydronicTubingInsideDiameter,value.get());
	}

	//field Hydronic Tubing Length
	if( modelObject.isHydronicTubingLengthAutosized() )
	{
		idfObject.setString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HydronicTubingLength,"Autosize");
	}		
	else if( (value = modelObject.hydronicTubingLength()) )
	{
		idfObject.setDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HydronicTubingLength,value.get());
	}

	//field Temperature Control Type
	if(boost::optional<std::string> tempCtrlType= modelObject.temperatureControlType() )
	{
		idfObject.setString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::TemperatureControlType,tempCtrlType.get());
	}
	
	// Heating Coil
	HVACComponent heatingCoil = modelObject.heatingCoil();
	boost::optional<CoilHeatingLowTempRadiantVarFlow>  coilOptionalHeating = heatingCoil.optionalCast<CoilHeatingLowTempRadiantVarFlow>();

	if (coilOptionalHeating)
	{
		CoilHeatingLowTempRadiantVarFlow coilHeat = *coilOptionalHeating;
		
		// field Maximum Hot Water Flow
		if( coilHeat.isMaximumHotWaterFlowAutosized() )
		{
				idfObject.setString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow,"Autosize");
		}				
		else if( (value = coilHeat.maximumHotWaterFlow()) )
		{
				idfObject.setDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow,value.get());
		}
		
		// field Heating Water Inlet Node Name 
		temp = coilHeat.inletModelObject();
		if(temp)
		{
			s = temp->name();
			if(s)
			{
				idfObject.setString(openstudio::ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingWaterInletNodeName,*s);
			}
		}

		//field Heating Water Outlet Node Name 
		temp = coilHeat.outletModelObject();
		if(temp)
		{
			s = temp->name();
			if(s)
			{
				idfObject.setString(openstudio::ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingWaterOutletNodeName,*s);
			}
		}
	
		//field Heating Control Throttling Range
		if( (value = coilHeat.heatingControlThrottlingRange()) )
		{
				idfObject.setDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingControlThrottlingRange,value.get());
		}
		
		//field Heating Control Temperature Schedule Name
		if( boost::optional<Schedule> schedule = coilHeat.heatingControlTemperatureSchedule() )
		{
			if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
			{
				idfObject.setString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingControlTemperatureScheduleName,_schedule->name().get());
			}
		}
	}
		
	// Cooling Coil
	HVACComponent coolingCoil = modelObject.coolingCoil();
	boost::optional<CoilCoolingLowTempRadiantVarFlow>  coilOptionalCooling = coolingCoil.optionalCast<CoilCoolingLowTempRadiantVarFlow>();
		
	if (coilOptionalCooling)
	{
		CoilCoolingLowTempRadiantVarFlow coilCool = *coilOptionalCooling;

		// Field Maximum Cold Water Flow
		if( coilCool.isMaximumColdWaterFlowAutosized() )
		{
			idfObject.setString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow,"Autosize");
		}
		else if( (value = coilCool.maximumColdWaterFlow()) )
		{
			idfObject.setDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow,value.get());
		}
				
		// field Cooling Water Inlet Node Name 
		temp = coilCool.inletModelObject();
		if(temp)
		{
			s = temp->name();
			if(s)
			{
				idfObject.setString(openstudio::ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingWaterInletNodeName,*s);
			}
		}

		//field Cooling Water Outlet Node Name 
		temp = coilCool.outletModelObject();
		if(temp)
		{
			s = temp->name();
			if(s)
			{
				idfObject.setString(openstudio::ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingWaterOutletNodeName,*s);
			}
		}

		//field Cooling Control Throttling Range
		
		if( (value = coilCool.coolingControlThrottlingRange()) )
		{
			idfObject.setDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingControlThrottlingRange,value.get());
		}

		//field Cooling Control Temperature Schedule Name
		if( boost::optional<Schedule> schedule = coilCool.coolingControlTemperatureSchedule() )
		{
			if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
			{
				idfObject.setString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingControlTemperatureScheduleName,_schedule->name().get());
			}
		}
		
		//field Condensation Control Type
		idfObject.setString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CondensationControlType,coilCool.condensationControlType());

		//field Condensation Control Dewpoint Offset
		if( (value = coilCool.condensationControlDewpointOffset()) )
		{
			idfObject.setDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CondensationControlDewpointOffset,value.get());
		}
	}
	
	//field Number of Circuits
	idfObject.setString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::NumberofCircuits,modelObject.numberofCircuits());

	//field Circuit Length
	idfObject.setDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CircuitLength,modelObject.circuitLength());

	return idfObject;
}








} // energyplus

} // openstudio

