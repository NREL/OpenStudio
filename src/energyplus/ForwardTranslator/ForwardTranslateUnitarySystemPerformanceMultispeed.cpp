/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/UnitarySystemPerformanceMultispeed.hpp"
#include "../../model/UnitarySystemPerformanceMultispeed_Impl.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/AirLoopHVACUnitarySystem_Impl.hpp"
#include "../../model/CoilHeatingDXMultiSpeed.hpp"
#include "../../model/CoilHeatingDXMultiSpeed_Impl.hpp"
#include "../../model/CoilHeatingDXMultiSpeedStageData.hpp"
#include "../../model/CoilHeatingDXMultiSpeedStageData_Impl.hpp"
#include "../../model/CoilHeatingGas.hpp"
#include "../../model/CoilHeatingGas_Impl.hpp"
#include "../../model/CoilHeatingGasMultiStage.hpp"
#include "../../model/CoilHeatingGasMultiStage_Impl.hpp"
#include "../../model/CoilHeatingGasMultiStageStageData.hpp"
#include "../../model/CoilHeatingGasMultiStageStageData_Impl.hpp"
#include "../../model/CoilCoolingDX.hpp"
#include "../../model/CoilCoolingDX_Impl.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance_Impl.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode_Impl.hpp"
#include "../../model/CoilCoolingDXCurveFitSpeed.hpp"
#include "../../model/CoilCoolingDXCurveFitSpeed_Impl.hpp"
#include "../../model/CoilCoolingDXMultiSpeed.hpp"
#include "../../model/CoilCoolingDXMultiSpeed_Impl.hpp"
#include "../../model/CoilCoolingDXMultiSpeedStageData.hpp"
#include "../../model/CoilCoolingDXMultiSpeedStageData_Impl.hpp"
#include <utilities/idd/UnitarySystemPerformance_Multispeed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateUnitarySystemPerformanceMultispeed(UnitarySystemPerformanceMultispeed& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;

    // Name
    IdfObject sysPerf = createRegisterAndNameIdfObject(openstudio::IddObjectType::UnitarySystemPerformance_Multispeed, modelObject);

    // Find the associated AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> unitarySystems = modelObject.getModelObjectSources<AirLoopHVACUnitarySystem>();
    if (unitarySystems.size() != 1) {
      LOG(Error, "OS:UnitarySystemPerformance:Multispeed should be referenced by one and only one OS:AirLoopHVAC:UnitarySystem, "
                   << modelObject.nameString() << " is referenced by " << unitarySystems.size() << ".")
      return boost::none;
    }
    AirLoopHVACUnitarySystem& unitarySystem = unitarySystems[0];

    // Number of speeds for heating
    boost::optional<HVACComponent> heatingCoil = unitarySystem.heatingCoil();
    if (heatingCoil) {
      if (heatingCoil->iddObjectType() == openstudio::IddObjectType::OS_Coil_Heating_DX_MultiSpeed) {
        auto heatingCoilDXMultispeed = heatingCoil->cast<CoilHeatingDXMultiSpeed>();
        sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, heatingCoilDXMultispeed.stages().size());
      } else if (heatingCoil->iddObjectType() == openstudio::IddObjectType::OS_Coil_Heating_Gas_MultiStage) {
        auto heatingCoilGasMultiStage = heatingCoil->cast<CoilHeatingGasMultiStage>();
        sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, heatingCoilGasMultiStage.stages().size());
      } else {
        sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, 1);
      }
    } else {
      sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, 0);
    }

    // Number of speeds for cooling
    boost::optional<HVACComponent> coolingCoil = unitarySystem.coolingCoil();
    if (coolingCoil) {
      if (coolingCoil->iddObjectType() == openstudio::IddObjectType::OS_Coil_Cooling_DX_MultiSpeed) {
        auto coolingCoilDXMultispeed = coolingCoil->cast<CoilCoolingDXMultiSpeed>();
        sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling, coolingCoilDXMultispeed.stages().size());
      } else if (coolingCoil->iddObjectType() == openstudio::IddObjectType::OS_Coil_Cooling_DX) {
        auto coolingCoilDX = coolingCoil->cast<CoilCoolingDX>();
        sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling,
                       coolingCoilDX.performanceObject().baseOperatingMode().speeds().size());
      } else {
        sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling, 1);
      }
    } else {
      sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling, 0);
    }

    // Single mode operation
    sysPerf.setString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation, modelObject.singleModeOperation() ? "Yes" : "No");

    // No Load Supply Air Flow Rate Ratio
    sysPerf.setDouble(UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio, modelObject.noLoadSupplyAirflowRateRatio());

    for (const auto& airflowRatioField : modelObject.supplyAirflowRatioFields()) {
      sysPerf.pushExtensibleGroup(airflowRatioField.getHeatingCoolingRatiosAsStrings());
    }

    return sysPerf;
  }

}  // namespace energyplus
}  // namespace openstudio
