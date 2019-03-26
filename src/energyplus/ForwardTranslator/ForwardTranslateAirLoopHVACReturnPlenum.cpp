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
#include "../../model/AirLoopHVACReturnPlenum.hpp"
#include "../../model/AirLoopHVACReturnPlenum_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ZoneHVACIdealLoadsAirSystem.hpp"
#include "../../model/ZoneHVACIdealLoadsAirSystem_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PortList.hpp"
#include "../../model/PortList_Impl.hpp"
#include <utilities/idd/AirLoopHVAC_ReturnPlenum_FieldEnums.hxx>
#include <utilities/idd/NodeList_FieldEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACReturnPlenum( AirLoopHVACReturnPlenum & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  std::string s;

  IdfObject idfObject(openstudio::IddObjectType::AirLoopHVAC_ReturnPlenum);

  m_idfObjects.push_back(idfObject);

  // Name
  s = modelObject.name().get();
  idfObject.setName(s);

  //((ZoneName)(Zone Name))
  //((ZoneNodeName)(Zone Node Name))

  //  InducedAirOutletNodeorNodeListName
  // This is only the zoneHVAC that meets a very specific criteria,
  // see the method documentation in AirLoopHVACReturnPlenum_Impl
  auto zoneHVAC = modelObject.getImpl<model::detail::AirLoopHVACReturnPlenum_Impl>()->zoneHVACIdealLoadsAirSystems();
  // If zoneHVAC is not empty we do a workaround,
  // and we don't use the inducedAirOutletPortList because of an EnergyPlus bug
  if ( zoneHVAC.empty() ) {
    PortList po = modelObject.getImpl<model::detail::AirLoopHVACReturnPlenum_Impl>()->inducedAirOutletPortList();
    std::vector<ModelObject> inducedNodes = po.modelObjects();
    if( ! inducedNodes.empty() )
    {
      IdfObject nodeList(openstudio::IddObjectType::NodeList);
      m_idfObjects.push_back(nodeList);
      nodeList.setName(s + " Induced Air Node List");
      idfObject.setString(AirLoopHVAC_ReturnPlenumFields::InducedAirOutletNodeorNodeListName,nodeList.name().get());
      for( std::vector<ModelObject>::const_iterator it = inducedNodes.begin();
           it != inducedNodes.end();
           ++it )
      {
        IdfExtensibleGroup eg = nodeList.pushExtensibleGroup();
        eg.setString(NodeListExtensibleFields::NodeName,it->name().get());
      }
    }
  }

  // OutletNodeName
  if ( ! zoneHVAC.empty() ) {
    auto node = zoneHVAC.front().airInletModelObject();
    OS_ASSERT( node );
    idfObject.setString(AirLoopHVAC_ReturnPlenumFields::OutletNodeName,node->nameString());
  } else if( boost::optional<model::ModelObject> node = modelObject.outletModelObject() ) {
    idfObject.setString(AirLoopHVAC_ReturnPlenumFields::OutletNodeName,node->name().get());
  } else {
    idfObject.setString(AirLoopHVAC_ReturnPlenumFields::OutletNodeName,modelObject.nameString() + " Outlet Node");
  }

  //// ZoneName
  //// and
  //// ZoneNodeName
  if ( boost::optional<model::ThermalZone> zone = modelObject.thermalZone() )
  {
    if( boost::optional<IdfObject> _zone = translateAndMapModelObject(zone.get()) )
    {
      idfObject.setString(AirLoopHVAC_ReturnPlenumFields::ZoneName,_zone->name().get());

      model::Node node = zone->zoneAirNode();
      idfObject.setString(AirLoopHVAC_ReturnPlenumFields::ZoneNodeName,node.name().get());
    }
  }

  // InletNodeName
  auto inletModelObjects = modelObject.inletModelObjects();
  for( auto & inletModelObject : inletModelObjects ) {
    IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
    eg.setString(AirLoopHVAC_ReturnPlenumExtensibleFields::InletNodeName,inletModelObject.name().get());
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

