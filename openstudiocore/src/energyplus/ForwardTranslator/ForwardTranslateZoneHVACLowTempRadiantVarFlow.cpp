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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/ModelObject_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ConstructionWithInternalSource.hpp"
#include "../../model/ConstructionWithInternalSource_Impl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/ZoneHVACLowTempRadiantVarFlow.hpp"
#include "../../model/ZoneHVACLowTempRadiantVarFlow_Impl.hpp"
#include "../../model/ZoneHVACEquipmentList.hpp"
#include "../../model/ZoneHVACEquipmentList_Impl.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
#include "../../model/CoilHeatingLowTempRadiantVarFlow.hpp"
#include "../../model/CoilHeatingLowTempRadiantVarFlow_Impl.hpp"
#include "../../model/CoilCoolingLowTempRadiantVarFlow.hpp"
#include "../../model/CoilCoolingLowTempRadiantVarFlow_Impl.hpp"
#include "../../model/ZoneHVACComponent.hpp"
#include "../../model/ZoneHVACComponent_Impl.hpp"

#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"


#include <utilities/idd/ZoneHVAC_LowTemperatureRadiant_VariableFlow_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
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

  //loop through all surfaces, adding up their area
  for (const Surface& surface : modelObject.surfaces()){
    totalAreaOfSurfaces = totalAreaOfSurfaces + surface.grossArea();
  }

  //loop through all the surfaces, adding them and their flow fractions (weighted per-area)
  for (const Surface& surface : modelObject.surfaces()){
    IdfExtensibleGroup group = _surfaceGroup.pushExtensibleGroup();
    OS_ASSERT(group.numFields() == 2);
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

    // Heating Design Capacity Method - introduced in 8.2.0 and not yet supported in OS
    idfObject.setString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacityMethod,"HeatingDesignCapacity");

    // Heating Design Capacity - introduced in 8.2.0 and not yet supported in OS
    idfObject.setString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacity,"Autosize");

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

    // Cooling Design Capacity Method - introduced in 8.2.0 and not yet supported in OS
    idfObject.setString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacityMethod,"CoolingDesignCapacity");

    // Cooling Design Capacity - introduced in 8.2.0 and not yet supported in OS
    idfObject.setString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacity,"Autosize");

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

