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
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "../../model/ZoneHVACWaterToAirHeatPump.hpp"
#include "../../model/ZoneHVACWaterToAirHeatPump_Impl.hpp"
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
#include "../../model/CoilHeatingDXVariableSpeed.hpp"
#include "../../model/CoilHeatingDXVariableSpeed_Impl.hpp"
#include "../../model/CoilHeatingDXVariableSpeedSpeedData.hpp"
#include "../../model/CoilHeatingDXVariableSpeedSpeedData_Impl.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl.hpp"
#include "../../model/CoilCoolingDXMultiSpeed.hpp"
#include "../../model/CoilCoolingDXMultiSpeed_Impl.hpp"
#include "../../model/CoilCoolingDXMultiSpeedStageData.hpp"
#include "../../model/CoilCoolingDXMultiSpeedStageData_Impl.hpp"
#include "../../model/CoilCoolingDXVariableSpeed.hpp"
#include "../../model/CoilCoolingDXVariableSpeed_Impl.hpp"
#include "../../model/CoilCoolingDXVariableSpeedSpeedData.hpp"
#include "../../model/CoilCoolingDXVariableSpeedSpeedData_Impl.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl.hpp"
#include <utilities/idd/UnitarySystemPerformance_Multispeed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateUnitarySystemPerformanceMultispeed(UnitarySystemPerformanceMultispeed& modelObject) {

    // Name
    IdfObject sysPerf = createRegisterAndNameIdfObject(openstudio::IddObjectType::UnitarySystemPerformance_Multispeed, modelObject);

    boost::optional<HVACComponent> heatingCoil;
    boost::optional<HVACComponent> coolingCoil;

    // Find the associated AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> unitarySystems = modelObject.getModelObjectSources<AirLoopHVACUnitarySystem>();
    if (unitarySystems.size() == 1) {
      AirLoopHVACUnitarySystem& unitarySystem = unitarySystems[0];
      heatingCoil = unitarySystem.heatingCoil();
      coolingCoil = unitarySystem.coolingCoil();
    }

    // Find the associated ZoneHVACTerminalUnitVariableRefrigerantFlow
    std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> tuVRFs =
      modelObject.getModelObjectSources<ZoneHVACTerminalUnitVariableRefrigerantFlow>();
    if (tuVRFs.size() == 1) {
      ZoneHVACTerminalUnitVariableRefrigerantFlow& tuVRF = tuVRFs[0];
      heatingCoil = tuVRF.heatingCoil();
      coolingCoil = tuVRF.coolingCoil();
    }

    // Find the associated ZoneHVACWaterToAirHeatPump
    std::vector<ZoneHVACWaterToAirHeatPump> hps = modelObject.getModelObjectSources<ZoneHVACWaterToAirHeatPump>();
    if (hps.size() == 1) {
      ZoneHVACWaterToAirHeatPump& hp = hps[0];
      heatingCoil = hp.heatingCoil().optionalCast<HVACComponent>();
      coolingCoil = hp.coolingCoil().optionalCast<HVACComponent>();
    }

    if ((!heatingCoil) && (!coolingCoil)) {
      LOG(Error, "OS:UnitarySystemPerformance:Multispeed should be referenced by one and only one OS:AirLoopHVAC:UnitarySystem or "
                 "OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow or OS:ZoneHVAC:WaterToAirHeatPump, "
                   << modelObject.nameString() << " is referenced by " << unitarySystems.size() << " OS:AirLoopHVAC:UnitarySystem, " << tuVRFs.size()
                   << " OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow, " << hps.size() << " OS:ZoneHVAC:WaterToAirHeatPump.")
      return boost::none;
    }

    // Number of speeds for heating
    if (heatingCoil) {
      if (heatingCoil->iddObjectType() == openstudio::IddObjectType::OS_Coil_Heating_DX_MultiSpeed) {
        auto heatingCoilDXMultispeed = heatingCoil->cast<CoilHeatingDXMultiSpeed>();
        sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, heatingCoilDXMultispeed.stages().size());
      } else if (heatingCoil->iddObjectType() == openstudio::IddObjectType::OS_Coil_Heating_Gas_MultiStage) {
        auto heatingCoilGasMultiStage = heatingCoil->cast<CoilHeatingGasMultiStage>();
        sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, heatingCoilGasMultiStage.stages().size());
      } else if (heatingCoil->iddObjectType() == openstudio::IddObjectType::OS_Coil_Heating_DX_VariableSpeed) {
        auto heatingCoilDXVarspeed = heatingCoil->cast<CoilHeatingDXVariableSpeed>();
        sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, heatingCoilDXVarspeed.speeds().size());
      } else if (heatingCoil->iddObjectType() == openstudio::IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit) {
        auto heatingCoilWaterToAirHeatingVar = heatingCoil->cast<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit>();
        sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, heatingCoilWaterToAirHeatingVar.speeds().size());
      } else {  // e.g., CoilHeatingWaterToAirHeatPumpEquationFit, CoilHeatingDXVariableRefrigerantFlow, CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl
        sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, 1);
      }
    } else {
      sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, 0);
    }

    // Number of speeds for cooling
    if (coolingCoil) {
      if (coolingCoil->iddObjectType() == openstudio::IddObjectType::OS_Coil_Cooling_DX_MultiSpeed) {
        auto coolingCoilDXMultispeed = coolingCoil->cast<CoilCoolingDXMultiSpeed>();
        sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling, coolingCoilDXMultispeed.stages().size());
      } else if (coolingCoil->iddObjectType() == openstudio::IddObjectType::OS_Coil_Cooling_DX_VariableSpeed) {
        auto coolingCoilDXVarspeed = heatingCoil->cast<CoilCoolingDXVariableSpeed>();
        sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling, coolingCoilDXVarspeed.speeds().size());
      } else if (coolingCoil->iddObjectType() == openstudio::IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit) {
        auto coolingCoilWaterToAirHeatingVar = heatingCoil->cast<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit>();
        sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling, coolingCoilWaterToAirHeatingVar.speeds().size());
      } else {  // e.g., CoilCoolingWaterToAirHeatPumpEquationFit, CoilCoolingDXVariableRefrigerantFlow, CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl
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
