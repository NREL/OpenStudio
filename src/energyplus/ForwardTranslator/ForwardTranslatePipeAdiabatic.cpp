/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/PipeAdiabatic.hpp"
#include "../../model/PipeAdiabatic_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/Pipe_Adiabatic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../model/BoilerSteam.hpp"
#include "../../model/BoilerSteam_Impl.hpp"
#include "../../model/DistrictHeatingSteam.hpp"
#include "../../model/DistrictHeatingSteam_Impl.hpp"
#include "../../model/PumpVariableSpeedCondensate.hpp"
#include "../../model/PumpVariableSpeedCondensate_Impl.hpp"
#include "../../model/CoilHeatingSteam.hpp"
#include "../../model/CoilHeatingSteam_Impl.hpp"
#include "../../model/CoilHeatingSteamBaseboardRadiant.hpp"
#include "../../model/CoilHeatingSteamBaseboardRadiant_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Splitter.hpp"
#include "../../model/Splitter_Impl.hpp"
#include "../../model/Mixer.hpp"
#include "../../model/Mixer_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::createPipeAdiabatic(bool hasSteam) {
    // PipeAdiabatic or PipeAdiabaticSteam

    boost::optional<IdfObject> idfObject;
    idfObject = IdfObject(IddObjectType::Pipe_Adiabatic);
    if (hasSteam) {
      idfObject = IdfObject(IddObjectType::Pipe_Adiabatic_Steam);
    } else {
      idfObject = IdfObject(IddObjectType::Pipe_Adiabatic);
    }

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translatePipeAdiabatic(PipeAdiabatic& modelObject) {
    OptionalString s;
    OptionalDouble d;
    OptionalModelObject temp;

    boost::optional<PlantLoop> plantLoop = modelObject.plantLoop();
    auto [hasWater, hasSteam] = hasWaterAndSteam(*plantLoop);
    boost::optional<IdfObject> idfObject = createPipeAdiabatic(hasSteam);

    m_idfObjects.push_back(*idfObject);

    ///////////////////////////////////////////////////////////////////////////
    // Field: Name ////////////////////////////////////////////////////////////
    s = modelObject.name();
    if (s) {
      idfObject->setName(*s);
    }
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Inlet Node Name ////////////////////////////////////////////////////
    temp = modelObject.inletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject->setString(openstudio::Pipe_AdiabaticFields::InletNodeName, *s);
      }
    }
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Outlet Node Name ///////////////////////////////////////////////////
    temp = modelObject.outletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject->setString(openstudio::Pipe_AdiabaticFields::OutletNodeName, *s);
      }
    }
    ///
    ////////////////////////////////////////////////////////////////////////

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
