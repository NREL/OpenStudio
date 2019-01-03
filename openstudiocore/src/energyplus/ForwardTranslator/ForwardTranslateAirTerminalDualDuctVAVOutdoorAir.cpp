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

