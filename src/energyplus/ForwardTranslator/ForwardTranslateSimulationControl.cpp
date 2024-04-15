/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/SimulationControl.hpp"

#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/SizingPeriod.hpp"
#include "../../model/SizingPeriod_Impl.hpp"
#include "../../model/SizingPlant.hpp"

#include <utilities/idd/SimulationControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSimulationControl(SimulationControl& modelObject) {

    IdfObject simCon = createAndRegisterIdfObject(openstudio::IddObjectType::SimulationControl, modelObject);

    auto boolToString = [](bool b) { return b ? "Yes" : "No"; };

    // NOTE: the logic for these three where it's defaulted is inside the model object:
    // it checks if you have Design Days + (Zonal equipment / AirLoopHVAC / PlantLoop)
    simCon.setString(openstudio::SimulationControlFields::DoZoneSizingCalculation, boolToString(modelObject.doZoneSizingCalculation()));
    simCon.setString(openstudio::SimulationControlFields::DoSystemSizingCalculation, boolToString(modelObject.doSystemSizingCalculation()));
    simCon.setString(openstudio::SimulationControlFields::DoPlantSizingCalculation, boolToString(modelObject.doPlantSizingCalculation()));

    simCon.setString(openstudio::SimulationControlFields::RunSimulationforSizingPeriods, boolToString(modelObject.runSimulationforSizingPeriods()));

    // DLM: might want to check for weather file object?
    simCon.setString(openstudio::SimulationControlFields::RunSimulationforWeatherFileRunPeriods,
                     boolToString(modelObject.runSimulationforWeatherFileRunPeriods()));

    // The logic when defaulted is inside the model object
    if (!modelObject.isDoHVACSizingSimulationforSizingPeriodsDefaulted()) {
      simCon.setString(openstudio::SimulationControlFields::DoHVACSizingSimulationforSizingPeriods,
                       boolToString(modelObject.doHVACSizingSimulationforSizingPeriods()));
    } else if (modelObject.doHVACSizingSimulationforSizingPeriods()) {
      // We only write it if it's not the IDD default of "No", so we don't want to pollute the IDF with advanced fields that people aren't used to.
      simCon.setString(openstudio::SimulationControlFields::DoHVACSizingSimulationforSizingPeriods, "Yes");
    }

    if (!modelObject.isMaximumNumberofHVACSizingSimulationPassesDefaulted()) {
      simCon.setInt(openstudio::SimulationControlFields::MaximumNumberofHVACSizingSimulationPasses,
                    modelObject.maximumNumberofHVACSizingSimulationPasses());
    }

    // other fields mapped to Building

    return simCon;
  }

}  // namespace energyplus

}  // namespace openstudio
