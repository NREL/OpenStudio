/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/SimulationControl.hpp"
#include "../../model/SimulationControl_Impl.hpp"

#include <utilities/idd/SimulationControl_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateSimulationControl(const WorkspaceObject& workspaceObject) {
    OptionalModelObject result;
    auto simCon = m_model.getUniqueModelObject<model::SimulationControl>();
    OptionalString optS = workspaceObject.name();
    if (optS) {
      simCon.setName(*optS);
    }

    optS = workspaceObject.getString(SimulationControlFields::DoZoneSizingCalculation);
    if (optS) {
      std::string temp = *optS;
      boost::to_lower(temp);
      if (temp == "no") {
        simCon.setDoZoneSizingCalculation(false);
      } else {
        simCon.setDoZoneSizingCalculation(true);
      }
    }

    optS = workspaceObject.getString(SimulationControlFields::DoSystemSizingCalculation);
    if (optS) {
      std::string temp = *optS;
      boost::to_lower(temp);
      if (temp == "no") {
        simCon.setDoSystemSizingCalculation(false);
      } else {
        simCon.setDoSystemSizingCalculation(true);
      }
    }

    optS = workspaceObject.getString(SimulationControlFields::DoPlantSizingCalculation);
    if (optS) {
      std::string temp = *optS;
      boost::to_lower(temp);
      if (temp == "no") {
        simCon.setDoPlantSizingCalculation(false);
      } else {
        simCon.setDoPlantSizingCalculation(true);
      }
    }

    optS = workspaceObject.getString(SimulationControlFields::RunSimulationforSizingPeriods);
    if (optS) {
      std::string temp = *optS;
      boost::to_lower(temp);
      if (temp == "no") {
        simCon.setRunSimulationforSizingPeriods(false);
      } else {
        simCon.setRunSimulationforSizingPeriods(true);
      }
    }

    optS = workspaceObject.getString(SimulationControlFields::RunSimulationforWeatherFileRunPeriods);
    if (optS) {
      std::string temp = *optS;
      boost::to_lower(temp);
      if (temp == "no") {
        simCon.setRunSimulationforWeatherFileRunPeriods(false);
      } else {
        simCon.setRunSimulationforWeatherFileRunPeriods(true);
      }
    }

    optS = workspaceObject.getString(SimulationControlFields::DoHVACSizingSimulationforSizingPeriods);
    if (optS) {
      if (openstudio::istringEqual("Yes", optS.get())) {
        simCon.setDoHVACSizingSimulationforSizingPeriods(true);
      } else {
        simCon.setDoHVACSizingSimulationforSizingPeriods(false);
      }
    }

    // Don't return default
    if (boost::optional<int> _i = workspaceObject.getInt(SimulationControlFields::MaximumNumberofHVACSizingSimulationPasses, false)) {
      simCon.setMaximumNumberofHVACSizingSimulationPasses(_i.get());
    }

    result = simCon;
    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
