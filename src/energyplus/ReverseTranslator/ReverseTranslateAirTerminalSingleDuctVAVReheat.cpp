/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/AirTerminalSingleDuctVAVReheat.hpp"
#include "../../model/AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/CoilHeatingGas.hpp"
#include "../../model/CoilHeatingGas_Impl.hpp"
#include <utilities/idd/AirTerminal_SingleDuct_VAV_Reheat_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateAirTerminalSingleDuctVAVReheat(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::AirTerminal_SingleDuct_VAV_Reheat) {
      LOG(Error, "WorkspaceObject is not IddObjectType: AirTerminal_SingleDuct_VAV_Reheat");
      return boost::none;
    }

    boost::optional<WorkspaceObject> wo = workspaceObject.getTarget(AirTerminal_SingleDuct_VAV_ReheatFields::AvailabilityScheduleName);
    boost::optional<Schedule> schedule;
    boost::optional<HVACComponent> coil;
    boost::optional<AirTerminalSingleDuctVAVReheat> airTerminal;

    if (wo) {
      boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get());
      if (mo) {
        if (!(schedule = mo->optionalCast<Schedule>())) {
          LOG(Error, workspaceObject.briefDescription() << " does not have an associated availability schedule");

          return boost::none;
        }
      }
    }

    wo = workspaceObject.getTarget(AirTerminal_SingleDuct_VAV_ReheatFields::ReheatCoilName);
    if (wo) {
      boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get());
      if (mo) {
        if (!coil) {
          coil = mo->optionalCast<CoilHeatingGas>();
        }
      }
    }

    if (schedule && coil) {
      airTerminal = AirTerminalSingleDuctVAVReheat(m_model, schedule.get(), coil.get());
    }

    if (airTerminal) {
      boost::optional<double> value;
      boost::optional<std::string> s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::Name);

      if (s) {
        airTerminal->setName(s.get());
      }

      // MaximumAirFlowRate
      value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate);
      s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate);
      if (value) {
        airTerminal->setMaximumAirFlowRate(value.get());
      } else if (s && istringEqual(s.get(), "Autosize")) {
        airTerminal->autosizeMaximumAirFlowRate();
      } else if (s && istringEqual(s.get(), "Autocalculate")) {
        airTerminal->autosizeMaximumAirFlowRate();
      }

      // ZoneMinimumAirFlowInputMethod
      s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::ZoneMinimumAirFlowInputMethod);
      if (s) {
        airTerminal->setZoneMinimumAirFlowInputMethod(s.get());
      }

      // ConstantMinimumAirFlowFraction
      value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::ConstantMinimumAirFlowFraction);
      if (value) {
        airTerminal->setConstantMinimumAirFlowFraction(value.get());
      } else {
        s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::ConstantMinimumAirFlowFraction);
        if (s && (istringEqual(s.get(), "Autosize") || istringEqual(s.get(), "Autocalculate"))) {
          airTerminal->autosizeMaximumAirFlowRate();
        }
      }

      // FixedMinimumAirFlowRate
      value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::FixedMinimumAirFlowRate);
      if (value) {
        airTerminal->setFixedMinimumAirFlowRate(value.get());
      } else {
        s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::FixedMinimumAirFlowRate);
        if (s && (istringEqual(s.get(), "Autosize") || istringEqual(s.get(), "Autocalculate"))) {
          airTerminal->autosizeFixedMinimumAirFlowRate();
        }
      }

      boost::optional<WorkspaceObject> _schedule;

      // MinimumAirFlowFractionScheduleName
      _schedule = workspaceObject.getTarget(AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowFractionScheduleName);
      if (_schedule) {
        boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(_schedule.get());
        if (mo) {
          if (boost::optional<Schedule> schedule = mo->optionalCast<Schedule>()) {
            airTerminal->setMinimumAirFlowFractionSchedule(schedule.get());
          }
        }
      }

      // MaximumHotWaterorSteamFlowRate
      value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate);
      s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate);
      if (value) {
        airTerminal->setMaximumHotWaterOrSteamFlowRate(value.get());
      } else if (s && istringEqual(s.get(), "Autosize")) {
        airTerminal->autosizeMaximumHotWaterOrSteamFlowRate();
      } else if (s && istringEqual(s.get(), "Autocalculate")) {
        airTerminal->autosizeMaximumHotWaterOrSteamFlowRate();
      }

      // MinimumHotWaterorSteamFlowRate
      value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MinimumHotWaterorSteamFlowRate);
      if (value) {
        airTerminal->setMinimumHotWaterOrStreamFlowRate(value.get());
      }

      // ConvergenceTolerance
      value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::ConvergenceTolerance);
      if (value) {
        airTerminal->setConvergenceTolerance(value.get());
      }

      // DamperHeatingAction
      s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::DamperHeatingAction);
      if (s) {
        airTerminal->setDamperHeatingAction(s.get());
      }

      // MaximumFlowPerZoneFloorAreaDuringReheat
      value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat);
      s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat);
      if (value) {
        airTerminal->setMaximumFlowPerZoneFloorAreaDuringReheat(value.get());
      } else if (s && istringEqual(s.get(), "Autosize")) {
        airTerminal->autosizeMaximumFlowPerZoneFloorAreaDuringReheat();
      } else if (s && istringEqual(s.get(), "Autocalculate")) {
        airTerminal->autosizeMaximumFlowPerZoneFloorAreaDuringReheat();
      }

      // MaximumFlowFractionDuringReheat
      value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat);
      s = workspaceObject.getString(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat);
      if (value) {
        airTerminal->setMaximumFlowFractionDuringReheat(value.get());
      } else if (s && istringEqual(s.get(), "Autosize")) {
        airTerminal->autosizeMaximumFlowFractionDuringReheat();
      } else if (s && istringEqual(s.get(), "Autocalculate")) {
        airTerminal->autosizeMaximumFlowFractionDuringReheat();
      }

      // MaximumReheatAirTemperature
      value = workspaceObject.getDouble(AirTerminal_SingleDuct_VAV_ReheatFields::MaximumReheatAirTemperature);
      if (value) {
        airTerminal->setMaximumReheatAirTemperature(value.get());
      }

      // MinimumAirFlowTurndownScheduleName
      _schedule = workspaceObject.getTarget(AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowTurndownScheduleName);
      if (_schedule) {
        boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(_schedule.get());
        if (mo) {
          if (boost::optional<Schedule> schedule = mo->optionalCast<Schedule>()) {
            airTerminal->setMinimumAirFlowTurndownSchedule(schedule.get());
          }
        }
      }

      return airTerminal.get();
    } else {
      LOG(Error, "Unknown error translating " << workspaceObject.briefDescription());

      return boost::none;
    }
  }

}  // namespace energyplus

}  // namespace openstudio
