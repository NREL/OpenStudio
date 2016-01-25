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

#include "../ReverseTranslator.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../../model/ControllerOutdoorAir.hpp"
#include "../../model/ControllerOutdoorAir_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include <utilities/idd/AirLoopHVAC_OutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include <utilities/idd/EvaporativeCooler_Direct_ResearchSpecial_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateAirLoopHVACOutdoorAirSystem( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::AirLoopHVAC_OutdoorAirSystem )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: AirLoopHVAC_OutdoorAirSystem");
     return boost::none;
  }

  Workspace _workspace = workspaceObject.workspace();

  boost::optional<WorkspaceObject> _controllerList;
  boost::optional<WorkspaceObject> _controllerOutdoorAir;
  boost::optional<std::string> controllerName;
  boost::optional<std::string> controllerType;
  boost::optional<ControllerOutdoorAir> oaController;

  _controllerList = workspaceObject.getTarget(AirLoopHVAC_OutdoorAirSystemFields::ControllerListName);

  if( _controllerList )
  {
    for( int i = 1;
         _controllerList->getString(i);
         i = i + 2 )
    {
      controllerType = _controllerList->getString(i);
      controllerName = _controllerList->getString(i + 1);
      if( controllerType )
      {
        if( istringEqual(controllerType.get(),"Controller:OutdoorAir") )
        {
          break;
        }
      }
    }
  }

  if( controllerName && controllerType )
  {
    boost::optional<WorkspaceObject> wo = _workspace.getObjectByTypeAndName(IddObjectType(controllerType.get()),controllerName.get()); 
    if( wo )
    {
      boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get());
      if( mo )
      {
        oaController = mo->optionalCast<ControllerOutdoorAir>();
      }
    }
  }

  if( oaController )
  {
    AirLoopHVACOutdoorAirSystem mo(m_model,oaController.get());

    boost::optional<std::string> name = workspaceObject.getString(AirLoopHVAC_OutdoorAirSystemFields::Name);
    if( name ) { mo.setName(name.get()); }

    Node outboardOANode = mo.outboardOANode().get();

    boost::optional<WorkspaceObject> _oaEquipmentList;
    boost::optional<WorkspaceObject> _outdoorAirMixer;
    std::vector<WorkspaceObject> equipmentVector;
    _oaEquipmentList = workspaceObject.getTarget(AirLoopHVAC_OutdoorAirSystemFields::OutdoorAirEquipmentListName);

    if( _oaEquipmentList )
    {
      for( int i = 1;
           _oaEquipmentList->getString(i);
           i = i + 2 )
      {
        boost::optional<std::string> equipmentName;
        boost::optional<std::string> equipmentType;

        equipmentType = _oaEquipmentList->getString(i);
        equipmentName = _oaEquipmentList->getString(i + 1);

        if( equipmentName && equipmentType )
        {
          boost::optional<WorkspaceObject> wo = _workspace.getObjectByTypeAndName(IddObjectType(equipmentType.get()),equipmentName.get());
          if( wo )
          {
            equipmentVector.push_back(wo.get());

            if( wo->iddObject().type() == IddObjectType::OutdoorAir_Mixer )
            {
              _outdoorAirMixer = wo;
            }
          }
        }
      }
      if( _outdoorAirMixer )
      {
        boost::optional<std::string> mixerOAInletNodeName = _outdoorAirMixer->getString(OutdoorAir_MixerFields::OutdoorAirStreamNodeName);
        boost::optional<std::string> mixerOAReliefNodeName = _outdoorAirMixer->getString(OutdoorAir_MixerFields::ReliefAirStreamNodeName);
        if( mixerOAInletNodeName ) { mo.outdoorAirModelObject()->cast<Node>().setName(mixerOAInletNodeName.get()); }
        if( mixerOAReliefNodeName ) { mo.reliefAirModelObject()->cast<Node>().setName(mixerOAReliefNodeName.get()); }

        boost::optional<std::string> oaStreamInletNodeName;
        boost::optional<std::string> oaStreamOutletNodeName;

        oaStreamInletNodeName = _outdoorAirMixer->getString(OutdoorAir_MixerFields::OutdoorAirStreamNodeName);

        while( oaStreamInletNodeName )
        {
          boost::optional<ModelObject> oaComponentModelObject;
          boost::optional<std::string> newOAStreamInletNodeName;

          for( const auto & equipmentElem : equipmentVector )
          {
            switch(equipmentElem.iddObject().type().value())
            {
              case openstudio::IddObjectType::EvaporativeCooler_Direct_ResearchSpecial :
              {
                oaStreamOutletNodeName = equipmentElem.getString(EvaporativeCooler_Direct_ResearchSpecialFields::AirOutletNodeName);
                if( oaStreamOutletNodeName )
                {
                  if( istringEqual(oaStreamOutletNodeName.get(),oaStreamInletNodeName.get()) )
                  {
                    newOAStreamInletNodeName = equipmentElem.getString(EvaporativeCooler_Direct_ResearchSpecialFields::AirInletNodeName);

                    oaComponentModelObject = translateAndMapWorkspaceObject(equipmentElem);
                  }
                }
                break;
              }
              default :
              {
                break;
              }
            }
            if( newOAStreamInletNodeName )
            {
              break;
            }
          }

          oaStreamInletNodeName = newOAStreamInletNodeName;

          if( oaComponentModelObject )
          {
            bool success = false;

            if( boost::optional<HVACComponent> hvacComponent = oaComponentModelObject->optionalCast<HVACComponent>() )
            {
              success = hvacComponent->addToNode(outboardOANode);

              if( success )
              {
                if( boost::optional<StraightComponent> straightComponent = hvacComponent->optionalCast<StraightComponent>() )
                {
                  if( oaStreamInletNodeName )
                  {
                    straightComponent->inletModelObject()->cast<Node>().setName(oaStreamInletNodeName.get());
                  }
                  if( oaStreamOutletNodeName )
                  {
                    straightComponent->outletModelObject()->cast<Node>().setName(oaStreamOutletNodeName.get());
                  }
                }
              }
            }

            if( ! success )
            {
              oaComponentModelObject->remove();
            }
          }
        }
      }
    }

    return mo;
  }
  else
  {
    return boost::none;
  }
}

} // energyplus

} // openstudio

