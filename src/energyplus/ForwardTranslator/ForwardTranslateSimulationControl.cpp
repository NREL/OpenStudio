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

    const unsigned numSizingPeriods = modelObject.model().getModelObjects<SizingPeriod>().size();

    auto hasZoneEquipment = [](const Model& m) {
      auto zones = m.getConcreteModelObjects<ThermalZone>();
      return std::any_of(zones.cbegin(), zones.cend(), [](const auto& z) { return !z.equipment().empty(); });
    };

    if (modelObject.doZoneSizingCalculation()) {
      simCon.setString(openstudio::SimulationControlFields::DoZoneSizingCalculation, "Yes");
    } else if ((numSizingPeriods > 0) && hasZoneEquipment(modelObject.model())) {
      if (modelObject.isDoZoneSizingCalculationDefaulted()) {
        LOG(Info, "You have zonal equipment and design days, and SimulationControl::DoZoneSizingCalculation is defaulted: turning on.");
        simCon.setString(openstudio::SimulationControlFields::DoZoneSizingCalculation, "Yes");
      } else {
        LOG(Warn, "You have zonal equipment and design days, it's possible you should enable SimulationControl::DoZoneSizingCalculation");
        simCon.setString(openstudio::SimulationControlFields::DoZoneSizingCalculation, "No");
      }
    } else {
      simCon.setString(openstudio::SimulationControlFields::DoZoneSizingCalculation, "No");
    }

    if (modelObject.doSystemSizingCalculation()) {
      simCon.setString(openstudio::SimulationControlFields::DoSystemSizingCalculation, "Yes");
    } else if ((numSizingPeriods > 0) && (!modelObject.model().getConcreteModelObjects<AirLoopHVAC>().empty())) {
      if (modelObject.isDoSystemSizingCalculationDefaulted()) {
        LOG(Info, "You have AirLoopHVACs and design days, and SimulationControl::DoSystemSizingCalculation is defaulted: turning it on.");
        simCon.setString(openstudio::SimulationControlFields::DoSystemSizingCalculation, "Yes");
      } else {
        LOG(Warn, "You have AirLoopHVACs and design days, it's possible you should enable SimulationControl::DoSystemSizingCalculation");
        simCon.setString(openstudio::SimulationControlFields::DoSystemSizingCalculation, "No");
      }
    } else {
      simCon.setString(openstudio::SimulationControlFields::DoSystemSizingCalculation, "No");
    }

    if (modelObject.doPlantSizingCalculation()) {
      simCon.setString(openstudio::SimulationControlFields::DoPlantSizingCalculation, "Yes");
    } else if ((numSizingPeriods > 0) && (!modelObject.model().getConcreteModelObjects<PlantLoop>().empty())) {
      if (modelObject.isDoPlantSizingCalculationDefaulted()) {
        LOG(Info, "You have PlantLoops and design days, and SimulationControl::DoPlantSizingCalculation is defaulted: turning it on.");
        simCon.setString(openstudio::SimulationControlFields::DoPlantSizingCalculation, "Yes");
      } else {
        LOG(Warn, "You have PlantLoops and design days, it's possible you should enable SimulationControl::DoPlantSizingCalculation");
        simCon.setString(openstudio::SimulationControlFields::DoPlantSizingCalculation, "No");
      }
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
