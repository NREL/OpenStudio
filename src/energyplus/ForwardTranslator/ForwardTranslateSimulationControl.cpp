/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    } else if ((numSizingPeriods > 0) && (modelObject.model().getConcreteModelObjects<ThermalZone>().size() > 0)) {
      simCon.setString(openstudio::SimulationControlFields::DoZoneSizingCalculation, "Yes");
    } else {
      simCon.setString(openstudio::SimulationControlFields::DoZoneSizingCalculation, "No");
    }

    if (modelObject.doSystemSizingCalculation()) {
      simCon.setString(openstudio::SimulationControlFields::DoSystemSizingCalculation, "Yes");
    } else if ((numSizingPeriods > 0) && (modelObject.model().getConcreteModelObjects<AirLoopHVAC>().size() > 0)) {
      simCon.setString(openstudio::SimulationControlFields::DoSystemSizingCalculation, "Yes");
    } else {
      simCon.setString(openstudio::SimulationControlFields::DoSystemSizingCalculation, "No");
    }

    if (modelObject.doPlantSizingCalculation()) {
      simCon.setString(openstudio::SimulationControlFields::DoPlantSizingCalculation, "Yes");
    } else if ((numSizingPeriods > 0) && (modelObject.model().getConcreteModelObjects<PlantLoop>().size() > 0)) {
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
