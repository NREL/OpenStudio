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
// copy from AirTerminalSingleDuctVAVReheat
#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirTerminalSingleDuctInletSideMixer.hpp"
#include "../../model/AirTerminalSingleDuctInletSideMixer_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ZoneHVACComponent.hpp"
#include "../../model/ZoneHVACComponent_Impl.hpp"
#include <utilities/idd/AirTerminal_SingleDuct_InletSideMixer_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirTerminalSingleDuctInletSideMixer( AirTerminalSingleDuctInletSideMixer & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<std::string> s;

  std::string baseName = modelObject.name().get();

  IdfObject _airDistributionUnit(openstudio::IddObjectType::ZoneHVAC_AirDistributionUnit);
  _airDistributionUnit.setName("ADU " + baseName ); //ADU: Air Distribution Unit

  IdfObject idfObject(openstudio::IddObjectType::AirTerminal_SingleDuct_InletSideMixer);

  idfObject.setName(baseName);

  m_idfObjects.push_back(_airDistributionUnit);

  m_idfObjects.push_back(idfObject);

  boost::optional<std::string> inletNodeName;
  boost::optional<std::string> outletNodeName;

  if( boost::optional<ModelObject> inletModelObject = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> inletNode = inletModelObject->optionalCast<Node>() )
    {
      inletNodeName = inletNode->name().get();
    }
  }

  if( boost::optional<ModelObject> outletModelObject = modelObject.outletModelObject() )
  {
    if( boost::optional<Node> outletNode = outletModelObject->optionalCast<Node>() )
    {
      outletNodeName = outletNode->name().get();
    }
  }

  if( outletNodeName && inletNodeName )
  {
    idfObject.setString(AirTerminal_SingleDuct_InletSideMixerFields::TerminalUnitOutletNodeName,outletNodeName.get());
    idfObject.setString(AirTerminal_SingleDuct_InletSideMixerFields::TerminalUnitPrimaryAirInletNodeName,inletNodeName.get());
  }

  if( boost::optional<AirLoopHVAC> airLoopHVAC = modelObject.airLoopHVAC() ) {
    std::vector<ZoneHVACComponent> zoneHVACs = subsetCastVector<ZoneHVACComponent>(airLoopHVAC->demandComponents(modelObject,airLoopHVAC->demandOutletNode()));
    if( ! zoneHVACs.empty() ) {
      ZoneHVACComponent zoneHVAC = zoneHVACs.front();
      if( boost::optional<IdfObject> _zoneHVAC= translateAndMapModelObject(zoneHVAC) ) {
        idfObject.setString(AirTerminal_SingleDuct_InletSideMixerFields::ZoneHVACTerminalUnitObjectType,_zoneHVAC->iddObject().name());
        idfObject.setString(AirTerminal_SingleDuct_InletSideMixerFields::ZoneHVACTerminalUnitName,_zoneHVAC->name().get());
      }
    }
  }

  // TerminalUnitSecondaryAirInletNodeName
  if( boost::optional<Node> secondaryInletNode = modelObject.secondaryAirInletNode() ) {
    idfObject.setString(AirTerminal_SingleDuct_InletSideMixerFields::TerminalUnitSecondaryAirInletNodeName,secondaryInletNode->name().get());
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

