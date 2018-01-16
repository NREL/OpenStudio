/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirTerminalDualDuctVAVOutdoorAir.hpp"
#include "../../model/AirTerminalDualDuctVAVOutdoorAir_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include <utilities/idd/AirTerminal_DualDuct_VAV_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirTerminalDualDuctVAVOutdoorAir( AirTerminalDualDuctVAVOutdoorAir & modelObject )
{

  std::string baseName = modelObject.name().get();

  IdfObject _airDistributionUnit(openstudio::IddObjectType::ZoneHVAC_AirDistributionUnit);
  _airDistributionUnit.setName("ADU " + baseName ); //ADU: Air Distribution Unit

  IdfObject idfObject(openstudio::IddObjectType::AirTerminal_DualDuct_VAV_OutdoorAir);
  idfObject.setName(baseName);

  m_idfObjects.push_back(_airDistributionUnit);
  m_idfObjects.push_back(idfObject);

  {
    Schedule schedule = modelObject.availabilitySchedule();
    if( auto idf = translateAndMapModelObject(schedule) )
    {
      idfObject.setString(AirTerminal_DualDuct_VAV_OutdoorAirFields::AvailabilityScheduleName,idf->name().get());
    }
  }

  if( auto mo = modelObject.outletModelObject() ) {
    idfObject.setString(AirTerminal_DualDuct_VAV_OutdoorAirFields::AirOutletNodeName,mo->name().get());
  }

  // outdoorAirInletNode = 0
  if( auto mo = modelObject.inletModelObject(0) ) {
    idfObject.setString(AirTerminal_DualDuct_VAV_OutdoorAirFields::OutdoorAirInletNodeName,mo->name().get());
  }

  if( auto mo = modelObject.inletModelObject(1) ) {
    idfObject.setString(AirTerminal_DualDuct_VAV_OutdoorAirFields::RecirculatedAirInletNodeName,mo->name().get());
  }

  if( modelObject.isMaximumTerminalAirFlowRateAutosized() ) {
    idfObject.setString(AirTerminal_DualDuct_VAV_OutdoorAirFields::MaximumTerminalAirFlowRate,"Autosize");
  } else if ( auto value = modelObject.maximumTerminalAirFlowRate() ) {
    idfObject.setDouble(AirTerminal_DualDuct_VAV_OutdoorAirFields::MaximumTerminalAirFlowRate,value.get());
  }

  {
    auto value = modelObject.perPersonVentilationRateMode();
    idfObject.setString(AirTerminal_DualDuct_VAV_OutdoorAirFields::PerPersonVentilationRateMode,value);
  }

  // ControlForOutdoorAir: if yes, get the zone's space's DSOA
  {
    bool dsoa_found = false;
    if( modelObject.controlForOutdoorAir() ) {
      if( auto airLoopHVAC = modelObject.airLoopHVAC() ) {
        auto zones = airLoopHVAC->demandComponents(modelObject,airLoopHVAC->demandOutletNode(),model::ThermalZone::iddObjectType());
        if( ! zones.empty() ) {
          auto zone = zones.front();
          auto spaces = zone.cast<model::ThermalZone>().spaces();
          if( ! spaces.empty() ) {
            if( auto designSpecificationOutdoorAir = spaces.front().designSpecificationOutdoorAir() ) {
              idfObject.setString(AirTerminal_DualDuct_VAV_OutdoorAirFields::DesignSpecificationOutdoorAirObjectName,
                designSpecificationOutdoorAir->name().get());
              dsoa_found = true;
            }
          }
        }
      }
    }
    if (!dsoa_found) {
      LOG(Error, "Cannot set the Required 'Design Specification Outdoor Air' (DSOA) object for " << modelObject.briefDescription()
              << ". You should set controlForOutdoorAir to 'true' and ensure the zone's Space/SpaceType has a DSOA attached.");
    }
  }
  // Populate fields for AirDistributionUnit
  if( boost::optional<ModelObject> outletNode = modelObject.outletModelObject() )
  {
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName,outletNode->name().get());
  }
  _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType,idfObject.iddObject().name());
  _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName,idfObject.name().get());

  return _airDistributionUnit;
}

} // energyplus

} // openstudio

