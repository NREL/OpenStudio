/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/CoilHeatingGasMultiStage.hpp"
#include "../../model/CoilHeatingGasMultiStage_Impl.hpp"
#include "../../model/CoilHeatingGasMultiStageStageData.hpp"
#include "../../model/CoilHeatingGasMultiStageStageData_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include <utilities/idd/Coil_Heating_Gas_MultiStage_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingGasMultiStage(CoilHeatingGasMultiStage& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Heating_Gas_MultiStage, modelObject);

    // AvailabilityScheduleName
    {
      auto schedule = modelObject.availabilitySchedule();
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(Coil_Heating_Gas_MultiStageFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    // AirInletNodeName
    if (auto node = modelObject.inletModelObject()) {
      idfObject.setString(Coil_Heating_Gas_MultiStageFields::AirInletNodeName, node->name().get());
    }

    // AirOutletNodeName
    // TemperatureSetpointNodeName
    if (auto node = modelObject.outletModelObject()) {
      idfObject.setString(Coil_Heating_Gas_MultiStageFields::AirOutletNodeName, node->name().get());
      idfObject.setString(Coil_Heating_Gas_MultiStageFields::TemperatureSetpointNodeName, node->name().get());
    }

    // PartLoadFractionCorrelationCurveName
    if (auto curve = modelObject.partLoadFractionCorrelationCurve()) {
      if (auto _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(Coil_Heating_Gas_MultiStageFields::PartLoadFractionCorrelationCurveName, _curve->name().get());
      }
    }

    // OffCycleParasiticGasLoad
    if ((value = modelObject.parasiticGasLoad())) {
      idfObject.setDouble(Coil_Heating_Gas_MultiStageFields::OffCycleParasiticGasLoad, value.get());
    }

    const auto stages = modelObject.stages();

    // NumberofStages
    if (!stages.empty()) {
      idfObject.setInt(Coil_Heating_Gas_MultiStageFields::NumberofStages, stages.size());
    }

    for (const auto& stage : stages) {
      auto eg = idfObject.pushExtensibleGroup();

      // Stage1GasBurnerEfficiency
      eg.setDouble(Coil_Heating_Gas_MultiStageExtensibleFields::StageGasBurnerEfficiency, stage.gasBurnerEfficiency());

      // Stage1NominalCapacity
      if (stage.isNominalCapacityAutosized()) {
        eg.setString(Coil_Heating_Gas_MultiStageExtensibleFields::StageNominalCapacity, "AutoSize");
      } else if ((value = stage.nominalCapacity())) {
        eg.setDouble(Coil_Heating_Gas_MultiStageExtensibleFields::StageNominalCapacity, value.get());
      }

      // Stage1OnCycleParasiticElectricLoad
      eg.setDouble(Coil_Heating_Gas_MultiStageExtensibleFields::StageOnCycleParasiticElectricLoad, stage.parasiticElectricLoad());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
