/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirLoopHVACZoneSplitter.hpp"
#include "../../model/AirLoopHVACZoneSplitter_Impl.hpp"
#include "../../model/AirLoopHVACZoneMixer.hpp"
#include "../../model/AirLoopHVACZoneMixer_Impl.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeNoReheat.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeNoReheat_Impl.hpp"
#include <utilities/idd/AirLoopHVAC_ZoneSplitter_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACZoneSplitter(AirLoopHVACZoneSplitter& modelObject) {
    OptionalModelObject temp;
    OptionalString optS;
    std::string s;

    IdfObject idfObject(openstudio::IddObjectType::AirLoopHVAC_ZoneSplitter);

    m_idfObjects.push_back(idfObject);

    ///////////////////////////////////////////////////////////////////////////
    // Field: Name ////////////////////////////////////////////////////////////
    s = modelObject.name().get();
    idfObject.setName(s);

    ///////////////////////////////////////////////////////////////////////////
    // Field: Inlet Node Name /////////////////////////////////////////////////
    temp = modelObject.inletModelObject();
    if (temp) {
      optS = temp->name();
      if (optS) {
        idfObject.setString(openstudio::AirLoopHVAC_ZoneSplitterFields::InletNodeName, *optS);
      }
    }
    ///////////////////////////////////////////////////////////////////////////
    // Field: Outlet 1-N Node Name //////////////////////////////////////////////
    std::vector<ModelObject> outletModelObjects = modelObject.outletModelObjects();
    int i = 2;
    for (const auto& outletModelObject : outletModelObjects) {
      if (OptionalAirTerminalSingleDuctConstantVolumeNoReheat airTerminal =
            outletModelObject.optionalCast<AirTerminalSingleDuctConstantVolumeNoReheat>()) {
        temp = airTerminal->outletModelObject();
        if (temp) {
          optS = temp->name().get();
          if (optS) {
            idfObject.setString(i, *optS);
          }
        }
      } else {
        s = outletModelObject.name().get();
        idfObject.setString(i, s);
      }
      i++;
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
