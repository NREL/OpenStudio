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
#include "../../model/AirToAirComponent.hpp"
#include "../../model/AirToAirComponent_Impl.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../../model/ControllerOutdoorAir.hpp"
#include "../../model/ControllerOutdoorAir_Impl.hpp"
#include "../../model/ControllerWaterCoil.hpp"
#include "../../model/ControllerWaterCoil_Impl.hpp"
#include "../../model/CoilCoolingWater.hpp"
#include "../../model/CoilCoolingWater_Impl.hpp"
#include "../../model/CoilHeatingWater.hpp"
#include "../../model/CoilHeatingWater_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/AirLoopHVAC_ControllerList_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_OutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_SupplyPath_FieldEnums.hxx>
#include <utilities/idd/AvailabilityManagerAssignmentList_FieldEnums.hxx>
#include <utilities/idd/AvailabilityManager_Scheduled_FieldEnums.hxx>
#include <utilities/idd/Controller_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACOutdoorAirSystem( AirLoopHVACOutdoorAirSystem & modelObject )
{
  OptionalString s;
  IdfObject idfObject(IddObjectType::AirLoopHVAC_OutdoorAirSystem);

  m_idfObjects.push_back(idfObject);

  // Name
  std::string name = modelObject.name().get();
  idfObject.setString(openstudio::AirLoopHVAC_OutdoorAirSystemFields::Name,name);


  // Controller List
  IdfObject _controllerList(IddObjectType::AirLoopHVAC_ControllerList);
  _controllerList.setName(name + " Controller List");
  _controllerList.clearExtensibleGroups();
  m_idfObjects.push_back(_controllerList);

  ControllerOutdoorAir controllerOutdoorAir = modelObject.getControllerOutdoorAir();
  boost::optional<IdfObject> _controllerOutdoorAir = translateAndMapModelObject(controllerOutdoorAir);
  OS_ASSERT(_controllerOutdoorAir);

  idfObject.setString(openstudio::AirLoopHVAC_OutdoorAirSystemFields::ControllerListName,_controllerList.name().get());

  IdfExtensibleGroup eg = _controllerList.pushExtensibleGroup();
  eg.setString(AirLoopHVAC_ControllerListExtensibleFields::ControllerObjectType,_controllerOutdoorAir->iddObject().name());
  eg.setString(AirLoopHVAC_ControllerListExtensibleFields::ControllerName,_controllerOutdoorAir->name().get());

  std::vector<ModelObject> controllers;
  auto components = modelObject.components();
  for( const auto & component : components ) {
    boost::optional<ControllerWaterCoil> controller;

    if( auto coil = component.optionalCast<CoilCoolingWater>() ) {
      controller = coil->controllerWaterCoil();
    } else if ( auto coil = component.optionalCast<CoilHeatingWater>() ) {
      controller = coil->controllerWaterCoil();
    }

    if( controller ) {
      controllers.push_back(controller.get());
    }
  }

  for( auto & controller: controllers ) {
    auto _controller = translateAndMapModelObject(controller);
    if( _controller ) {
      IdfExtensibleGroup eg = _controllerList.pushExtensibleGroup();
      eg.setString(AirLoopHVAC_ControllerListExtensibleFields::ControllerObjectType,_controller->iddObject().name());
      eg.setString(AirLoopHVAC_ControllerListExtensibleFields::ControllerName,_controller->name().get());
    }
  }

  // Field: Availability Manager List Name //////////////////////////////////
  IdfObject availabilityManagerListIdf(IddObjectType::AvailabilityManagerAssignmentList);
  availabilityManagerListIdf.setName(name + " Availability Manager List");
  m_idfObjects.push_back(availabilityManagerListIdf);

  IdfObject availabilityManagerScheduledIdf = IdfObject(openstudio::IddObjectType::AvailabilityManager_Scheduled);
  availabilityManagerScheduledIdf.setName(name + " Availability Manager");
  m_idfObjects.push_back(availabilityManagerScheduledIdf);

  Schedule alwaysOn = modelObject.model().alwaysOnDiscreteSchedule();
  IdfObject alwaysOnIdf = translateAndMapModelObject(alwaysOn).get();

  s = availabilityManagerListIdf.getString(openstudio::AvailabilityManagerAssignmentListFields::Name);
  if(s)
  {
    idfObject.setString(openstudio::AirLoopHVAC_OutdoorAirSystemFields::AvailabilityManagerListName,*s);
  }

  availabilityManagerListIdf.setString(1 + openstudio::AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerObjectType,
                                       availabilityManagerScheduledIdf.iddObject().name());
  availabilityManagerListIdf.setString(1 + openstudio::AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerName,
                                       availabilityManagerScheduledIdf.name().get());
  availabilityManagerScheduledIdf.setString(openstudio::AvailabilityManager_ScheduledFields::ScheduleName,alwaysOnIdf.name().get());

  // OA Node List
  s = modelObject.outboardOANode()->name();
  IdfObject oaNodeListIdf(openstudio::IddObjectType::OutdoorAir_NodeList);
  if(s)
  {
    oaNodeListIdf.setString(0,*s);
  }
  m_idfObjects.push_back(oaNodeListIdf);

  ///////////////////////////////////////////////////////////////////////////
  // Field: Outdoor Air Equipment List Name /////////////////////////////////
  IdfObject equipmentListIdf(IddObjectType::AirLoopHVAC_OutdoorAirSystem_EquipmentList);
  equipmentListIdf.setName(name + " Equipment List");

  m_idfObjects.push_back(equipmentListIdf);

  IdfObject outdoorAirMixerIdf(IddObjectType::OutdoorAir_Mixer);
  outdoorAirMixerIdf.setName(name + " Outdoor Air Mixer");
  m_idfObjects.push_back(outdoorAirMixerIdf);

  s = modelObject.mixedAirModelObject()->name();
  if(s)
  {
    outdoorAirMixerIdf.setString(OutdoorAir_MixerFields::MixedAirNodeName,*s);
  }
  s = modelObject.outdoorAirModelObject()->name();
  if(s)
  {
    outdoorAirMixerIdf.setString(OutdoorAir_MixerFields::OutdoorAirStreamNodeName,*s);
  }

  s = modelObject.reliefAirModelObject()->name();
  if(s)
  {
    outdoorAirMixerIdf.setString(OutdoorAir_MixerFields::ReliefAirStreamNodeName,*s);
  }

  s = modelObject.returnAirModelObject()->name();
  if(s)
  {
    outdoorAirMixerIdf.setString(OutdoorAir_MixerFields::ReturnAirStreamNodeName,*s);
  }

  unsigned i = 1;
  ModelObjectVector oaModelObjects = modelObject.oaComponents();
  for( auto oaIt = oaModelObjects.begin();
       oaIt != oaModelObjects.end();
       ++oaIt )
  {
    if( boost::optional<IdfObject> idfObject = translateAndMapModelObject(*oaIt) )
    {
      equipmentListIdf.setString(i,idfObject->iddObject().name());
      i++;
      equipmentListIdf.setString(i,idfObject->name().get());
      i++;
    }
  }

  ModelObjectVector reliefModelObjects = modelObject.reliefComponents();
  for( auto reliefIt = reliefModelObjects.begin();
       reliefIt != reliefModelObjects.end();
       ++reliefIt )
  {
    // Make sure this is not an AirToAirComponent,
    // because those will be added to the equipment list
    // from the oaComponents() side.
    if( ! reliefIt->optionalCast<AirToAirComponent>() ) {
      if( boost::optional<IdfObject> idfObject = translateAndMapModelObject(*reliefIt) )
      {
        equipmentListIdf.setString(i,idfObject->iddObject().name());
        i++;
        equipmentListIdf.setString(i,idfObject->name().get());
        i++;
      }
    }
  }

  s = outdoorAirMixerIdf.iddObject().name();
  equipmentListIdf.setString(i,*s);
  ++i;
  s = outdoorAirMixerIdf.name();
  equipmentListIdf.setString(i,*s);

  s = equipmentListIdf.name();
  if(s)
  {
    idfObject.setString(openstudio::AirLoopHVAC_OutdoorAirSystemFields::OutdoorAirEquipmentListName,*s);
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

