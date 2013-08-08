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
#include <model/AirLoopHVACOutdoorAirSystem.hpp>
#include <model/AirLoopHVACOutdoorAirSystem_Impl.hpp>
#include <model/ControllerOutdoorAir.hpp>
#include <model/ControllerOutdoorAir_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>

#include <utilities/idf/IdfExtensibleGroup.hpp>

#include <utilities/idd/AirLoopHVAC_ControllerList_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_OutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_SupplyPath_FieldEnums.hxx>
#include <utilities/idd/AvailabilityManagerAssignmentList_FieldEnums.hxx>
#include <utilities/idd/AvailabilityManager_Scheduled_FieldEnums.hxx>
#include <utilities/idd/Controller_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <utilities/core/Assert.hpp>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACOutdoorAirSystem( AirLoopHVACOutdoorAirSystem & modelObject )
{
  OptionalString s;
  IdfObject idfObject(IddObjectType::AirLoopHVAC_OutdoorAirSystem);

  m_idfObjects.push_back(idfObject);

  // Name
  s = modelObject.name();
  if(s)
  {
    idfObject.setString(openstudio::AirLoopHVAC_OutdoorAirSystemFields::Name,*s);
  }

  // Controller List
  IdfObject controllerListIdf(IddObjectType::AirLoopHVAC_ControllerList);
  controllerListIdf.createName();
  controllerListIdf.clearExtensibleGroups();

  m_idfObjects.push_back(controllerListIdf);

  ControllerOutdoorAir controllerOutdoorAir = modelObject.getControllerOutdoorAir();
  boost::optional<IdfObject> temp = translateAndMapModelObject(controllerOutdoorAir);

  s = controllerListIdf.name();
  if(s)
  {
    idfObject.setString(openstudio::AirLoopHVAC_OutdoorAirSystemFields::ControllerListName,*s);
  }

  s = temp->iddObject().name();
  StringVector groupFields(2u);
  bool addGroup(false);
  if(s)
  {
    groupFields[0] = *s;
    addGroup = true;
  }

  s = temp->name();
  if(s)
  {
    groupFields[1] = *s;
    addGroup = true;
  }

  if (addGroup) {
    IdfExtensibleGroup eg = controllerListIdf.pushExtensibleGroup(groupFields);
    OS_ASSERT(!eg.empty());
  }

  // Field: Availability Manager List Name //////////////////////////////////
  IdfObject availabilityManagerListIdf(IddObjectType::AvailabilityManagerAssignmentList);
  availabilityManagerListIdf.createName();
  m_idfObjects.push_back(availabilityManagerListIdf);

  IdfObject availabilityManagerScheduledIdf = IdfObject(openstudio::IddObjectType::AvailabilityManager_Scheduled);
  availabilityManagerScheduledIdf.createName();
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
  equipmentListIdf.createName();

  m_idfObjects.push_back(equipmentListIdf);

  IdfObject outdoorAirMixerIdf(IddObjectType::OutdoorAir_Mixer);
  outdoorAirMixerIdf.createName();
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

  s = outdoorAirMixerIdf.iddObject().name();
  equipmentListIdf.setString(1,*s);
  s = outdoorAirMixerIdf.name();
  equipmentListIdf.setString(2,*s);

  ModelObjectVector oaModelObjects = modelObject.oaComponents();
  ModelObjectVector::iterator oaIt;
  unsigned i = 3;
  for( oaIt = oaModelObjects.begin();
       oaIt != oaModelObjects.end();
       oaIt++ )
  {
    if( ! oaIt->optionalCast<Node>() )
    {
      s = stripOS2(oaIt->iddObject().name());
      if(s)
        equipmentListIdf.setString(i,*s);
      i++;
      s = oaIt->name();
      if(s)
        equipmentListIdf.setString(i,*s);
      i++;
    }
  }

  s = equipmentListIdf.name();
  if(s)
  {
    idfObject.setString(openstudio::AirLoopHVAC_OutdoorAirSystemFields::OutdoorAirEquipmentListName,*s);
  }

  for( oaIt = oaModelObjects.begin();
       oaIt != oaModelObjects.end();
       oaIt++ )
  {
    translateAndMapModelObject(*oaIt);
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

