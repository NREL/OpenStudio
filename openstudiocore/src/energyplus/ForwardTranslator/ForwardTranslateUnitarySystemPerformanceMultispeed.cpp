/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

boost::optional<IdfObject> ForwardTranslator::translateUnitarySystemPerformanceMultispeed( UnitarySystemPerformanceMultispeed & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> d;

  // Name
  IdfObject sysPerf = createRegisterAndNameIdfObject(openstudio::IddObjectType::UnitarySystemPerformance_Multispeed, modelObject);

  // Find the associated AirLoopHVACUnitarySystem
  std::vector<AirLoopHVACUnitarySystem> unitarySystems = modelObject.getModelObjectSources<AirLoopHVACUnitarySystem>();
  if (unitarySystems.size() != 1) {
    LOG(Error, "OS:UnitarySystemPerformance:Multispeed should be referenced by one and only one OS:AirLoopHVAC:UnitarySystem, " << modelObject.name() << " is referenced by " << unitarySystems.size() << ".")
    return boost::none;
  }
  AirLoopHVACUnitarySystem &unitarySystem = unitarySystems[0];

  // Number of speeds for heating
  boost::optional<HVACComponent> heatingCoil = unitarySystem.heatingCoil();
  if (heatingCoil) {
    if (heatingCoil->iddObjectType() == openstudio::IddObjectType::OS_Coil_Heating_DX_MultiSpeed ) {
      CoilHeatingDXMultiSpeed heatingCoilDXMultispeed = heatingCoil->cast<CoilHeatingDXMultiSpeed>();
      sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, heatingCoilDXMultispeed.stages().size());
    } else if (heatingCoil->iddObjectType() == openstudio::IddObjectType::OS_Coil_Heating_Gas_MultiStage) {
      CoilHeatingGasMultiStage heatingCoilGasMultiStage = heatingCoil->cast<CoilHeatingGasMultiStage>();
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
      CoilCoolingDXMultiSpeed coolingCoilDXMultispeed = coolingCoil->cast<CoilCoolingDXMultiSpeed>();
      sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling, coolingCoilDXMultispeed.stages().size());
    } else {
      sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling, 1);
    }
  } else {
    sysPerf.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling, 0);
  }

  // Single mode operation
  sysPerf.setString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation, modelObject.singleModeOperation() ? "Yes" : "No");

  for (const auto & airflowRatioField : modelObject.supplyAirflowRatioFields()) {
    sysPerf.pushExtensibleGroup(airflowRatioField.getHeatingCoolingRatiosAsStrings());
  }

  return sysPerf;

}

} //openstudio
} //energyplus
