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
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PipeOutdoor.hpp"
#include "../../model/PipeOutdoor_Impl.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Pipe_Outdoor_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translatePipeOutdoor(PipeOutdoor & modelObject)
{
  IdfObject idfObject(openstudio::IddObjectType::Pipe_Outdoor);

  m_idfObjects.push_back(idfObject);

  if(auto node = modelObject.inletModelObject()) {
    idfObject.setString(openstudio::Pipe_OutdoorFields::FluidInletNodeName, node->name().get());
  }

  if(auto node = modelObject.outletModelObject()) {
    idfObject.setString(openstudio::Pipe_OutdoorFields::FluidOutletNodeName, node->name().get());
  }

  if(auto value = modelObject.name()) {
    idfObject.setName(value.get());
  }

  if(auto construction = modelObject.construction()) {
    idfObject.setString(openstudio::Pipe_OutdoorFields::ConstructionName, construction->name().get());
  }

  if(auto node = modelObject.ambientTemperatureOutdoorAirNode()) {
    idfObject.setString(openstudio::Pipe_OutdoorFields::AmbientTemperatureOutdoorAirNodeName, node->name().get());
  } else {
    auto name = modelObject.name().get() + " OA Node";
    IdfObject oaNodeListIdf(openstudio::IddObjectType::OutdoorAir_NodeList);
    oaNodeListIdf.setString(0,name);
    m_idfObjects.push_back(oaNodeListIdf);

    idfObject.setString(openstudio::Pipe_OutdoorFields::AmbientTemperatureOutdoorAirNodeName, name);
  }

  idfObject.setDouble(openstudio::Pipe_OutdoorFields::PipeInsideDiameter, modelObject.pipeInsideDiameter());

  idfObject.setDouble(openstudio::Pipe_OutdoorFields::PipeLength, modelObject.pipeLength());

  return idfObject;
}

} // energyplus

} // openstudio

