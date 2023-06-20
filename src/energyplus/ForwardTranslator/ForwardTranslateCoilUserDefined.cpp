/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CoilUserDefined.hpp"
#include "../../model/CoilUserDefined_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/EnergyManagementSystemProgramCallingManager.hpp"
#include "../../model/EnergyManagementSystemProgram.hpp"
#include "../../model/EnergyManagementSystemActuator.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Coil_UserDefined_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilUserDefined(CoilUserDefined& modelObject) {

    if (modelObject.numberofAirConnections() == 0) {
      LOG(Warn, modelObject.briefDescription() << "will not be translated, cannot find air connection.");
      return boost::none;
    }

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_UserDefined, modelObject);

    //PlantConnectionisUsed default value
    idfObject.setString(Coil_UserDefinedFields::PlantConnectionisUsed, "No");

    // PlantConnectionInletNodeName
    bool plantUsed = false;
    if (boost::optional<ModelObject> mo = modelObject.waterInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_UserDefinedFields::PlantConnectionInletNodeName, node->nameString());
        //PlantConnectionisUsed to Yes since Node is connected
        idfObject.setString(Coil_UserDefinedFields::PlantConnectionisUsed, "Yes");
        plantUsed = true;
      }
    }

    // PlantConnectionOutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.waterOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_UserDefinedFields::PlantConnectionOutletNodeName, node->nameString());
      }
    }
    // remove plant actuators if no plant connections
    if (!plantUsed) {
      modelObject.plantMinimumMassFlowRateActuator().remove();
      modelObject.plantMaximumMassFlowRateActuator().remove();
      modelObject.plantDesignVolumeFlowRateActuator().remove();
      modelObject.plantMassFlowRateActuator().remove();
      modelObject.plantOutletTemperatureActuator().remove();
    }
    // AirConnection1InletNodeName

    if (boost::optional<ModelObject> mo = modelObject.airInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_UserDefinedFields::AirConnection1InletNodeName, node->nameString());
      }
    }

    // AirConnection1OutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.airOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_UserDefinedFields::AirConnection1OutletNodeName, node->nameString());
      }
    }

    // NumberofAirConnections
    idfObject.setInt(Coil_UserDefinedFields::NumberofAirConnections, modelObject.numberofAirConnections());

    // OverallModelSimulationProgramCallingManagerName

    if (boost::optional<EnergyManagementSystemProgramCallingManager> pcm = modelObject.overallModelSimulationProgramCallingManager()) {
      idfObject.setString(Coil_UserDefinedFields::OverallModelSimulationProgramCallingManagerName, pcm->nameString());
    }

    // ModelSetupandSizingProgramCallingManagerName

    if (boost::optional<EnergyManagementSystemProgramCallingManager> pcm = modelObject.modelSetupandSizingProgramCallingManager()) {
      idfObject.setString(Coil_UserDefinedFields::ModelSetupandSizingProgramCallingManagerName, pcm->nameString());
    }

    // AmbientZoneName

    if (boost::optional<ThermalZone> zone_ = modelObject.ambientZone()) {
      idfObject.setString(Coil_UserDefinedFields::AmbientZoneName, zone_->nameString());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
