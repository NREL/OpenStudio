/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/SimulationControl.hpp"
#include "../../model/SizingPeriod.hpp"
#include "../../model/SizingPeriod_Impl.hpp"
#include "../../model/SizingPlant.hpp"
#include "../../model/SizingPlant_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include <utilities/idd/SimulationControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSimulationControl(SimulationControl& modelObject) {
    IdfObject simCon(openstudio::IddObjectType::SimulationControl);

    m_idfObjects.push_back(simCon);

    OptionalString s = modelObject.name();
    if (s) {
      simCon.setName(*s);
    }

    unsigned numSizingPeriods = modelObject.model().getModelObjects<SizingPeriod>().size();

    if (modelObject.doZoneSizingCalculation()) {
      simCon.setString(openstudio::SimulationControlFields::DoZoneSizingCalculation, "Yes");
    } else if ((numSizingPeriods > 0) && (!modelObject.model().getConcreteModelObjects<ThermalZone>().empty())) {
      simCon.setString(openstudio::SimulationControlFields::DoZoneSizingCalculation, "Yes");
    } else {
      simCon.setString(openstudio::SimulationControlFields::DoZoneSizingCalculation, "No");
    }

    if (modelObject.doSystemSizingCalculation()) {
      simCon.setString(openstudio::SimulationControlFields::DoSystemSizingCalculation, "Yes");
    } else if ((numSizingPeriods > 0) && (!modelObject.model().getConcreteModelObjects<AirLoopHVAC>().empty())) {
      simCon.setString(openstudio::SimulationControlFields::DoSystemSizingCalculation, "Yes");
    } else {
      simCon.setString(openstudio::SimulationControlFields::DoSystemSizingCalculation, "No");
    }

    if (modelObject.doPlantSizingCalculation()) {
      simCon.setString(openstudio::SimulationControlFields::DoPlantSizingCalculation, "Yes");
    } else if ((numSizingPeriods > 0) && (!modelObject.model().getConcreteModelObjects<PlantLoop>().empty())) {
      simCon.setString(openstudio::SimulationControlFields::DoPlantSizingCalculation, "Yes");
    } else {
      simCon.setString(openstudio::SimulationControlFields::DoPlantSizingCalculation, "No");
    }

    if (modelObject.runSimulationforSizingPeriods()) {
      simCon.setString(openstudio::SimulationControlFields::RunSimulationforSizingPeriods, "Yes");
    } else {
      simCon.setString(openstudio::SimulationControlFields::RunSimulationforSizingPeriods, "No");
    }

    // DLM: might want to check for weather file object?
    if (modelObject.runSimulationforWeatherFileRunPeriods()) {
      simCon.setString(openstudio::SimulationControlFields::RunSimulationforWeatherFileRunPeriods, "Yes");
    } else {
      simCon.setString(openstudio::SimulationControlFields::RunSimulationforWeatherFileRunPeriods, "No");
    }

    if (modelObject.doHVACSizingSimulationforSizingPeriods()) {
      simCon.setString(openstudio::SimulationControlFields::DoHVACSizingSimulationforSizingPeriods, "Yes");
    } else if (numSizingPeriods > 0) {
      // Sizing:Plant I/O: "The use of 'Coincident' sizing option requires that the object
      // be set to YES for the input field called Do HVAC Sizing Simulation for Sizing Periods"
      std::vector<PlantLoop> plantLoops = modelObject.model().getConcreteModelObjects<PlantLoop>();
      if (std::any_of(plantLoops.begin(), plantLoops.end(),
                      [](const PlantLoop& p) { return openstudio::istringEqual("Coincident", p.sizingPlant().sizingOption()); })) {
        simCon.setString(openstudio::SimulationControlFields::DoHVACSizingSimulationforSizingPeriods, "Yes");
      } else {
        if (!modelObject.isDoZoneSizingCalculationDefaulted()) {
          simCon.setString(openstudio::SimulationControlFields::DoHVACSizingSimulationforSizingPeriods, "No");
        }
      }
    } else {
      if (!modelObject.isDoZoneSizingCalculationDefaulted()) {
        simCon.setString(openstudio::SimulationControlFields::DoHVACSizingSimulationforSizingPeriods, "No");
      }
    }

    if (!modelObject.isMaximumNumberofHVACSizingSimulationPassesDefaulted()) {
      simCon.setInt(openstudio::SimulationControlFields::MaximumNumberofHVACSizingSimulationPasses,
                    modelObject.maximumNumberofHVACSizingSimulationPasses());
    }

    // other fields mapped to Building

    return boost::optional<IdfObject>(simCon);
  }

}  // namespace energyplus

}  // namespace openstudio
