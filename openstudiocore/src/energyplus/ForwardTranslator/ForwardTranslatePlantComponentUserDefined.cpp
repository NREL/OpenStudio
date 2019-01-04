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
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/PlantComponentUserDefined.hpp"
#include "../../model/PlantComponentUserDefined_Impl.hpp"
#include "../../model/EnergyManagementSystemProgramCallingManager.hpp"
#include "../../model/EnergyManagementSystemProgramCallingManager_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/PlantComponent_UserDefined_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translatePlantComponentUserDefined( PlantComponentUserDefined & modelObject )
{
  boost::optional<std::string> s;

  IdfObject idfObject(IddObjectType::PlantComponent_UserDefined);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // PlantConnection1LoadingMode

  if ((s = modelObject.plantLoadingMode()))
  {
    idfObject.setString(PlantComponent_UserDefinedFields::PlantConnection1LoadingMode, s.get());
  }

  // PlantConnection1LoopFlowRequestMode

  if ((s = modelObject.plantLoopFlowRequestMode())) {
    idfObject.setString(PlantComponent_UserDefinedFields::PlantConnection1LoopFlowRequestMode, s.get());
  }

  // NumberofPlantLoopConnections
  // value is 1 since its a StraightComponent
  idfObject.setInt(PlantComponent_UserDefinedFields::NumberofPlantLoopConnections, 1);


  // MainModelProgramCallingManagerName

  if (boost::optional<EnergyManagementSystemProgramCallingManager> pcm = modelObject.mainModelProgramCallingManager()) {
    idfObject.setString(PlantComponent_UserDefinedFields::MainModelProgramCallingManagerName, pcm->name().get());
  }

  // PlantConnection1InitializationProgramCallingManagerName
  
  if (boost::optional<EnergyManagementSystemProgramCallingManager> pcm = modelObject.plantInitializationProgramCallingManager())
  {
    idfObject.setString(PlantComponent_UserDefinedFields::PlantConnection1InitializationProgramCallingManagerName, pcm->name().get());
  }

  // PlantConnection1SimulationProgramCallingManagerName

  if (boost::optional<EnergyManagementSystemProgramCallingManager> pcm = modelObject.plantSimulationProgramCallingManager()) {
    idfObject.setString(PlantComponent_UserDefinedFields::PlantConnection1SimulationProgramCallingManagerName, pcm->name().get());
  }

  // PlantConnection1InletNodeName

  if( boost::optional<ModelObject> mo = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(PlantComponent_UserDefinedFields::PlantConnection1InletNodeName, node->name().get());
    }
  }

  // PlantConnection1OutletNodeName

  if( boost::optional<ModelObject> mo = modelObject.outletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(PlantComponent_UserDefinedFields::PlantConnection1OutletNodeName, node->name().get());
    }
  }

  // AmbientZoneName

  if (boost::optional<ThermalZone> tz = modelObject.ambientZone()) {
    idfObject.setString(PlantComponent_UserDefinedFields::AmbientZoneName, tz->name().get());
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

