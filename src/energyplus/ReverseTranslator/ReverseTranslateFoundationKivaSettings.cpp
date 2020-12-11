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

#include "../ReverseTranslator.hpp"

#include "../../model/FoundationKivaSettings.hpp"
#include "../../model/FoundationKivaSettings_Impl.hpp"

#include <utilities/idd/Foundation_Kiva_Settings_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateFoundationKivaSettings(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Foundation_Kiva_Settings) {
      LOG(Error, "WorkspaceObject is not IddObjectType: FoundationKivaSettings");
      return boost::none;
    }

    FoundationKivaSettings foundationKivaSettings = m_model.getUniqueModelObject<FoundationKivaSettings>();

    OptionalString s;
    OptionalDouble d;

    s = workspaceObject.name();
    if (s) {
      foundationKivaSettings.setName(*s);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::SoilConductivity);
    if (d) {
      foundationKivaSettings.setSoilConductivity(*d);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::SoilDensity);
    if (d) {
      foundationKivaSettings.setSoilDensity(*d);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::SoilSpecificHeat);
    if (d) {
      foundationKivaSettings.setSoilSpecificHeat(*d);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::GroundSolarAbsorptivity);
    if (d) {
      foundationKivaSettings.setGroundSolarAbsorptivity(*d);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::GroundThermalAbsorptivity);
    if (d) {
      foundationKivaSettings.setGroundThermalAbsorptivity(*d);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::GroundSurfaceRoughness);
    if (d) {
      foundationKivaSettings.setGroundSurfaceRoughness(*d);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::FarFieldWidth);
    if (d) {
      foundationKivaSettings.setFarFieldWidth(*d);
    }

    s = workspaceObject.getString(Foundation_Kiva_SettingsFields::DeepGroundBoundaryCondition);
    if (s) {
      foundationKivaSettings.setDeepGroundBoundaryCondition(*s);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::DeepGroundDepth);
    if (d) {
      foundationKivaSettings.setDeepGroundDepth(*d);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::MinimumCellDimension);
    if (d) {
      foundationKivaSettings.setMinimumCellDimension(*d);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::MaximumCellGrowthCoefficient);
    if (d) {
      foundationKivaSettings.setMaximumCellGrowthCoefficient(*d);
    }

    s = workspaceObject.getString(Foundation_Kiva_SettingsFields::SimulationTimestep);
    if (s) {
      foundationKivaSettings.setSimulationTimestep(*s);
    }

    return foundationKivaSettings;
  }

}  // namespace energyplus

}  // namespace openstudio
