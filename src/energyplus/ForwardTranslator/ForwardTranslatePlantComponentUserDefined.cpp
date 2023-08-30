/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

  boost::optional<IdfObject> ForwardTranslator::translatePlantComponentUserDefined(PlantComponentUserDefined& modelObject) {
    boost::optional<std::string> s;

    IdfObject idfObject(IddObjectType::PlantComponent_UserDefined);

    m_idfObjects.push_back(idfObject);

    // Name

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // PlantConnection1LoadingMode

    if ((s = modelObject.plantLoadingMode())) {
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

    if (boost::optional<EnergyManagementSystemProgramCallingManager> pcm = modelObject.plantInitializationProgramCallingManager()) {
      idfObject.setString(PlantComponent_UserDefinedFields::PlantConnection1InitializationProgramCallingManagerName, pcm->name().get());
    }

    // PlantConnection1SimulationProgramCallingManagerName

    if (boost::optional<EnergyManagementSystemProgramCallingManager> pcm = modelObject.plantSimulationProgramCallingManager()) {
      idfObject.setString(PlantComponent_UserDefinedFields::PlantConnection1SimulationProgramCallingManagerName, pcm->name().get());
    }

    // PlantConnection1InletNodeName

    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(PlantComponent_UserDefinedFields::PlantConnection1InletNodeName, node->name().get());
      }
    }

    // PlantConnection1OutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(PlantComponent_UserDefinedFields::PlantConnection1OutletNodeName, node->name().get());
      }
    }

    // AmbientZoneName

    if (boost::optional<ThermalZone> tz = modelObject.ambientZone()) {
      idfObject.setString(PlantComponent_UserDefinedFields::AmbientZoneName, tz->name().get());
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
