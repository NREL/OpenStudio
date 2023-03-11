/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/CoilUserDefined.hpp"
#include "../../model/CoilUserDefined_Impl.hpp"
#include "../../model/EnergyManagementSystemProgramCallingManager.hpp"
#include "../../model/EnergyManagementSystemProgramCallingManager_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Coil_UserDefined_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilUserDefined(CoilUserDefined& modelObject) {
    boost::optional<std::string> s;
    //boost::optional<double> value;

    IdfObject idfObject(IddObjectType::Coil_UserDefined);

    m_idfObjects.push_back(idfObject);
    
    // Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // PlantConnectionInletNodeName

    if (boost::optional<ModelObject> mo = modelObject.waterInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_UserDefinedFields::PlantConnectionInletNodeName, node->name().get());
      }
    }

    // PlantConnectionOutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.waterOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_UserDefinedFields::PlantConnectionOutletNodeName, node->name().get());
      }
    }

    // AirConnection1InletNodeName

    if (boost::optional<ModelObject> mo = modelObject.airInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_UserDefinedFields::AirConnection1InletNodeName, node->name().get());
      }
    }

    // AirConnection1OutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.airOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_UserDefinedFields::AirConnection1OutletNodeName, node->name().get());
      }
    }

    // NumberofAirConnections
    // value is 1 since its a WaterToAirComponent
    idfObject.setInt(Coil_UserDefinedFields::NumberofAirConnections, 1);

    // OverallModelSimulationProgramCallingManagerName

    if (boost::optional<EnergyManagementSystemProgramCallingManager> pcm = modelObject.overallModelSimulationProgramCallingManager()) {
        idfObject.setString(Coil_UserDefinedFields::OverallModelSimulationProgramCallingManagerName, pcm->name().get());
    }

    // ModelSetupandSizingProgramCallingManagerName

    if (boost::optional<EnergyManagementSystemProgramCallingManager> pcm = modelObject.modelSetupandSizingProgramCallingManager()) {
        idfObject.setString(Coil_UserDefinedFields::ModelSetupandSizingProgramCallingManagerName, pcm->name().get());
    }

    // AmbientZoneName

    if (boost::optional<ThermalZone> tz = modelObject.ambientZone()) {
        idfObject.setString(Coil_UserDefinedFields::AmbientZoneName, tz->name().get());
    }

    return boost::optional<IdfObject>(idfObject);
  }

  //((Name)(Name))
  //((AvailabilityScheduleName)(Availability Schedule Name))

}  // namespace energyplus

}  // namespace openstudio
