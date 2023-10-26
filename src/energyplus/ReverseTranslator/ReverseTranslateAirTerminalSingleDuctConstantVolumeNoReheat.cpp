/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeNoReheat.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeNoReheat_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include <utilities/idd/AirTerminal_SingleDuct_ConstantVolume_NoReheat_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateAirTerminalSingleDuctConstantVolumeNoReheat(const WorkspaceObject& workspaceObject) {
    if ((workspaceObject.iddObject().type() != IddObjectType::AirTerminal_SingleDuct_ConstantVolume_NoReheat)) {
      LOG(Error, "WorkspaceObject is not IddObjectType: AirTerminal_SingleDuct_ConstantVolume_NoReheat");
      return boost::none;
    }

    boost::optional<WorkspaceObject> wo = workspaceObject.getTarget(AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::AvailabilityScheduleName);
    boost::optional<Schedule> schedule;
    boost::optional<AirTerminalSingleDuctConstantVolumeNoReheat> airTerminal;
    boost::optional<double> value;

    if (wo) {
      boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get());
      if (mo) {
        if (!(schedule = mo->optionalCast<Schedule>())) {
          LOG(Error, workspaceObject.briefDescription() << " does not have an associated availability schedule");

          return boost::none;
        }
      }
    }

    if (schedule) {
      airTerminal = AirTerminalSingleDuctConstantVolumeNoReheat(m_model, schedule.get());
    }

    if (airTerminal) {
      boost::optional<std::string> s = workspaceObject.getString(AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::Name);
      if (s) {
        airTerminal->setName(s.get());
      }

      s = workspaceObject.getString(AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::MaximumAirFlowRate);
      if (s && istringEqual(s.get(), "AutoSize")) {
        airTerminal->autosizeMaximumAirFlowRate();
      } else if ((value = workspaceObject.getDouble(AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::MaximumAirFlowRate))) {
        airTerminal->setMaximumAirFlowRate(value.get());
      }

      return airTerminal.get();
    } else {
      LOG(Error, "Unknown error translating " << workspaceObject.briefDescription());

      return boost::none;
    }
  }

}  // namespace energyplus

}  // namespace openstudio
