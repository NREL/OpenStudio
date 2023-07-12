/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
      auto workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();
      OptionalWorkspaceObject target = workspaceGroup.getTarget(Coil_Cooling_DX_CurveFit_OperatingModeExtensibleFields::SpeedName);
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);

      // add the speed
      operatingMode.addSpeed(modelObject->cast<CoilCoolingDXCurveFitSpeed>());
    }

    return operatingMode;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
