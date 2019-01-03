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
#include "../../model/ZoneHVACLowTemperatureRadiantElectric.hpp"
#include "../../model/ZoneHVACLowTemperatureRadiantElectric_Impl.hpp"
#include "../../model/ZoneHVACEquipmentList.hpp"
#include "../../model/ZoneHVACEquipmentList_Impl.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
//#include <model/CoilHeatingLowTempRadiantVarFlow.hpp>
//#include <model/CoilHeatingLowTempRadiantVarFlow_Impl.hpp>
//#include <model/CoilCoolingLowTempRadiantVarFlow.hpp>
//#include <model/CoilCoolingLowTempRadiantVarFlow_Impl.hpp>
#include "../../model/ZoneHVACComponent.hpp"
#include "../../model/ZoneHVACComponent_Impl.hpp"

#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"


#include <utilities/idd/ZoneHVAC_LowTemperatureRadiant_Electric_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
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

  // Field Maximum Electrical Power to Panel
  // Maps to HeatingDesignCapacity in 8.2.0 and above
  if( modelObject.isMaximumElectricalPowertoPanelAutosized() )
  {
    idfObject.setString(ZoneHVAC_LowTemperatureRadiant_ElectricFields::HeatingDesignCapacity,"Autosize");
  }
  else if( (value = modelObject.maximumElectricalPowertoPanel()) )
  {
    idfObject.setDouble(ZoneHVAC_LowTemperatureRadiant_ElectricFields::HeatingDesignCapacity,value.get());
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

