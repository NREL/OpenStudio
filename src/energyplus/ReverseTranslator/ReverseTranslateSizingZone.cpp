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

#include "../ReverseTranslator.hpp"
#include "../../model/SizingZone.hpp"
#include "../../model/SizingZone_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include <utilities/idd/Sizing_Zone_FieldEnums.hxx>
#include <utilities/idd/DesignSpecification_ZoneAirDistribution_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateSizingZone( const WorkspaceObject & workspaceObject )
{
  boost::optional<WorkspaceObject> target = workspaceObject.getTarget(Sizing_ZoneFields::ZoneorZoneListName);

  std::vector<ThermalZone> thermalZones;

  if( target ){

    // just one thermal zone
    boost::optional<ModelObject> mo;
    if (target->iddObject().type() == IddObjectType::Zone){
      mo = translateAndMapWorkspaceObject(target.get());
      if( mo ) {
        if( boost::optional<Space> space = mo->optionalCast<Space>() ) {
          boost::optional<ThermalZone> thermalZone = space->thermalZone();
          if (thermalZone){
            thermalZones.push_back(*thermalZone);
          }
        }
      }
    }else if (target->iddObject().type() == IddObjectType::ZoneList){

      // get all thermal zones in zone list
      for (const IdfExtensibleGroup& idfGroup : target->extensibleGroups()){
        WorkspaceExtensibleGroup workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();
        OptionalWorkspaceObject owo = workspaceGroup.getTarget(0);
        if (owo){
          mo = translateAndMapWorkspaceObject(owo.get());
          if( mo ) {
            if( boost::optional<Space> space = mo->optionalCast<Space>() ) {
              boost::optional<ThermalZone> thermalZone = space->thermalZone();
              if (thermalZone){
                thermalZones.push_back(*thermalZone);
              }
            }
          }
        }
      }
    }
  }

  if(thermalZones.empty())
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't find associated ThermalZone(s).");

    return boost::none;
  }

  boost::optional<ModelObject> result;
  for (ThermalZone thermalZone : thermalZones){

    // sizing zone is constructed in thermal zone ctor
    openstudio::model::SizingZone sizingZone = thermalZone.sizingZone();

    // return first sizing zone
    if (!result){
      result = sizingZone;
    }

    boost::optional<std::string> s;
    boost::optional<double> value;

    // ZoneCoolingDesignSupplyAirTemperature

    value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperature);
    if( value )
    {
      sizingZone.setZoneCoolingDesignSupplyAirTemperature(value.get());
    }

    // ZoneHeatingDesignSupplyAirTemperature

    value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperature);
    if( value )
    {
      sizingZone.setZoneHeatingDesignSupplyAirTemperature(value.get());
    }

    // ZoneCoolingDesignSupplyAirHumidityRatio

    value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio);
    if( value )
    {
      sizingZone.setZoneHeatingDesignSupplyAirHumidityRatio(value.get());
    }

    // ZoneHeatingDesignSupplyAirHumidityRatio

    value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio);
    if( value )
    {
      sizingZone.setZoneHeatingDesignSupplyAirHumidityRatio(value.get());
    }

    // DesignSpecificationOutdoorAirObjectName

    target = workspaceObject.getTarget(Sizing_ZoneFields::DesignSpecificationOutdoorAirObjectName);
    if (target){
      OptionalModelObject mo = translateDesignSpecificationOutdoorAir(*target);
      if (mo){
        if (mo->optionalCast<DesignSpecificationOutdoorAir>()){
          std::vector<Space> spaces = thermalZone.spaces();
          OS_ASSERT(spaces.size() == 1);
          spaces[0].setDesignSpecificationOutdoorAir(mo->cast<DesignSpecificationOutdoorAir>());
        }
      }
    }

    // ZoneHeatingSizingFactor

    value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneHeatingSizingFactor);
    if( value )
    {
      sizingZone.setZoneHeatingSizingFactor(value.get());
    }

    // ZoneCoolingSizingFactor

    value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneCoolingSizingFactor);
    if( value )
    {
      sizingZone.setZoneCoolingSizingFactor(value.get());
    }

    // CoolingDesignAirFlowMethod

    s = workspaceObject.getString(Sizing_ZoneFields::CoolingDesignAirFlowMethod);
    if( s )
    {
      sizingZone.setCoolingDesignAirFlowMethod(s.get());
    }

    // CoolingDesignAirFlowRate

    value = workspaceObject.getDouble(Sizing_ZoneFields::CoolingDesignAirFlowRate);
    if( value )
    {
      sizingZone.setCoolingDesignAirFlowRate(value.get());
    }

    // CoolingMinimumAirFlowperZoneFloorArea

    value = workspaceObject.getDouble(Sizing_ZoneFields::CoolingMinimumAirFlowperZoneFloorArea);
    if( value )
    {
      sizingZone.setCoolingMinimumAirFlowperZoneFloorArea(value.get());
    }

    // CoolingMinimumAirFlow

    value = workspaceObject.getDouble(Sizing_ZoneFields::CoolingMinimumAirFlow);
    if( value )
    {
      sizingZone.setCoolingMinimumAirFlow(value.get());
    }

    // CoolingMinimumAirFlowFraction

    value = workspaceObject.getDouble(Sizing_ZoneFields::CoolingMinimumAirFlowFraction);
    if( value )
    {
      sizingZone.setCoolingMinimumAirFlowFraction(value.get());
    }

    // HeatingDesignAirFlowMethod

    s = workspaceObject.getString(Sizing_ZoneFields::HeatingDesignAirFlowMethod);
    if( s )
    {
      sizingZone.setHeatingDesignAirFlowMethod(s.get());
    }

    // HeatingDesignAirFlowRate

    value = workspaceObject.getDouble(Sizing_ZoneFields::HeatingDesignAirFlowRate);
    if( value )
    {
      sizingZone.setHeatingDesignAirFlowRate(value.get());
    }

    // HeatingMaximumAirFlowperZoneFloorArea

    value = workspaceObject.getDouble(Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea);
    if( value )
    {
      sizingZone.setHeatingMaximumAirFlowperZoneFloorArea(value.get());
    }

    // HeatingMaximumAirFlow

    value = workspaceObject.getDouble(Sizing_ZoneFields::HeatingMaximumAirFlow);
    if( value )
    {
      sizingZone.setHeatingMaximumAirFlow(value.get());
    }

    // HeatingMaximumAirFlowFraction

    value = workspaceObject.getDouble(Sizing_ZoneFields::HeatingMaximumAirFlowFraction);
    if( value )
    {
      sizingZone.setHeatingMaximumAirFlowFraction(value.get());
    }

    //DesignSpecification_ZoneAirDistribution

    boost::optional<WorkspaceObject> _designSpecification
        = workspaceObject.getTarget(Sizing_ZoneFields::DesignSpecificationZoneAirDistributionObjectName);

    if( _designSpecification )
    {
      // ZoneAirDistributionEffectivenessinCoolingMode

      value = _designSpecification->getDouble(
                DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinCoolingMode);
      if( value )
      {
        sizingZone.setDesignZoneAirDistributionEffectivenessinCoolingMode(value.get());
      }

      // ZoneAirDistributionEffectivenessinHeatingMode

      value = _designSpecification->getDouble(
                DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinHeatingMode);
      if( value )
      {
        sizingZone.setDesignZoneAirDistributionEffectivenessinHeatingMode(value.get());
      }
    }

  }

  return result;
}

} // energyplus

} // openstudio

