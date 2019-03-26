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
#include "../../model/SetpointManagerSingleZoneReheat.hpp"
#include "../../model/SetpointManagerSingleZoneReheat_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include <utilities/idd/SetpointManager_SingleZone_Reheat_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateSetpointManagerSingleZoneReheat( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::SetpointManager_SingleZone_Reheat )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: SetpointManager_SingleZone_Reheat");
     return boost::none;
  }

  bool nodeFound = false;

  if( boost::optional<std::string> setpointNodeName = workspaceObject.getString(SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName) )
  {
    boost::optional<Node> setpointNode = m_model.getModelObjectByName<Node>(setpointNodeName.get());

    if( setpointNode ) { nodeFound = true; }
  }

  if( ! nodeFound )
  {
    LOG(Error, workspaceObject.briefDescription() << " is not attached to a node in the model");

    return boost::none;
  }

  SetpointManagerSingleZoneReheat mo(m_model);

  boost::optional<std::string> s = workspaceObject.getString(SetpointManager_SingleZone_ReheatFields::Name);
  if( s )
  {
    mo.setName(s.get());
  }

  boost::optional<double> value = workspaceObject.getDouble(SetpointManager_SingleZone_ReheatFields::MinimumSupplyAirTemperature);
  if( value )
  {
    mo.setMinimumSupplyAirTemperature(value.get());
  }

  value = workspaceObject.getDouble(SetpointManager_SingleZone_ReheatFields::MaximumSupplyAirTemperature);
  if( value )
  {
    mo.setMaximumSupplyAirTemperature(value.get());
  }

  s = workspaceObject.getString(SetpointManager_SingleZone_ReheatFields::ControlZoneName);
  if( s )
  {
    boost::optional<ModelObject> modelObject;
    boost::optional<Space> space;

    if( boost::optional<WorkspaceObject> _zone =
          workspaceObject.workspace().getObjectByTypeAndName(IddObjectType::Zone,s.get()) )
    {
      modelObject = translateAndMapWorkspaceObject(_zone.get());
    }

    if( modelObject )
    {
      if( (space = modelObject->optionalCast<Space>()) )
      {
        if( boost::optional<ThermalZone> thermalZone = space->thermalZone() )
        {
          mo.setControlZone(thermalZone.get());
        }
      }
    }
  }

  s = workspaceObject.getString(SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName);
  if( s )
  {
    if( boost::optional<Node> node = m_model.getModelObjectByName<Node>(s.get()) )
    {
      mo.addToNode(node.get());
    }
  }

  if( mo.setpointNode() )
  {
    return mo;
  }
  else
  {
    return boost::none;
  }
}

} // energyplus

} // openstudio

