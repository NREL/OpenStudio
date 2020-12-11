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

#include "../../model/CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode_Impl.hpp"
#include "../../model/CoilCoolingDXCurveFitSpeed.hpp"
#include "../../model/CoilCoolingDXCurveFitSpeed_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Coil_Cooling_DX_CurveFit_OperatingMode_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateCoilCoolingDXCurveFitOperatingMode(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Coil_Cooling_DX_CurveFit_OperatingMode) {
      LOG(Error, "WorkspaceObject is not IddObjectType: CoilCoolingDXCurveFitOperatingMode");
      return boost::none;
    }

    OptionalString s;
    OptionalDouble d;
    OptionalInt i;
    OptionalWorkspaceObject target;

    openstudio::model::CoilCoolingDXCurveFitOperatingMode operatingMode(m_model);

    s = workspaceObject.name();
    if (s) {
      operatingMode.setName(*s);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedGrossTotalCoolingCapacity);
    if (d) {
      operatingMode.setRatedGrossTotalCoolingCapacity(*d);
    } else {
      operatingMode.autosizeRatedGrossTotalCoolingCapacity();
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedEvaporatorAirFlowRate);
    if (d) {
      operatingMode.setRatedEvaporatorAirFlowRate(*d);
    } else {
      operatingMode.autosizeRatedEvaporatorAirFlowRate();
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedCondenserAirFlowRate);
    if (d) {
      operatingMode.setRatedCondenserAirFlowRate(*d);
    } else {
      operatingMode.autosizeRatedCondenserAirFlowRate();
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::MaximumCyclingRate);
    if (d) {
      operatingMode.setMaximumCyclingRate(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
    if (d) {
      operatingMode.setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::LatentCapacityTimeConstant);
    if (d) {
      operatingMode.setLatentCapacityTimeConstant(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalTimeforCondensateRemovaltoBegin);
    if (d) {
      operatingMode.setNominalTimeforCondensateRemovaltoBegin(*d);
    }

    s = workspaceObject.getString(Coil_Cooling_DX_CurveFit_OperatingModeFields::ApplyLatentDegradationtoSpeedsGreaterthan1);
    if (s && istringEqual(s.get(), "Yes")) {
      operatingMode.setApplyLatentDegradationtoSpeedsGreaterthan1(true);
    } else {
      operatingMode.setApplyLatentDegradationtoSpeedsGreaterthan1(false);
    }

    s = workspaceObject.getString(Coil_Cooling_DX_CurveFit_OperatingModeFields::CondenserType);
    if (s) {
      operatingMode.setCondenserType(*s);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalEvaporativeCondenserPumpPower);
    if (d) {
      operatingMode.setNominalEvaporativeCondenserPumpPower(*d);
    } else {
      operatingMode.autosizeNominalEvaporativeCondenserPumpPower();
    }

    i = workspaceObject.getInt(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalSpeedNumber);
    if (i) {
      operatingMode.setNominalSpeedNumber(*i);
    }

    for (const IdfExtensibleGroup& idfGroup : workspaceObject.extensibleGroups()) {
      WorkspaceExtensibleGroup workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();
      OptionalWorkspaceObject target = workspaceGroup.getTarget(Coil_Cooling_DX_CurveFit_OperatingModeExtensibleFields::SpeedName);
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);

      // add the speed
      operatingMode.addSpeed(modelObject->cast<CoilCoolingDXCurveFitSpeed>());
    }

    return operatingMode;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio