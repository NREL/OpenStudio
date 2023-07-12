/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/CoilCoolingDXCurveFitOperatingMode.hpp"

#include "../../model/CoilCoolingDXCurveFitSpeed.hpp"
#include "../../model/CoilCoolingDXCurveFitSpeed_Impl.hpp"

#include <utilities/idd/Coil_Cooling_DX_CurveFit_OperatingMode_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXCurveFitOperatingMode(model::CoilCoolingDXCurveFitOperatingMode& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(openstudio::IddObjectType::Coil_Cooling_DX_CurveFit_OperatingMode);

    m_idfObjects.push_back(idfObject);

    // Name
    idfObject.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::Name, modelObject.name().get());

    // RatedGrossTotalCoolingCapacity
    value = modelObject.ratedGrossTotalCoolingCapacity();
    if (value) {
      idfObject.setDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedGrossTotalCoolingCapacity, value.get());
    } else {
      idfObject.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedGrossTotalCoolingCapacity, "Autosize");
    }

    // RatedEvaporatorAirFlowRate
    value = modelObject.ratedEvaporatorAirFlowRate();
    if (value) {
      idfObject.setDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedEvaporatorAirFlowRate, value.get());
    } else {
      idfObject.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedEvaporatorAirFlowRate, "Autosize");
    }

    // RatedCondenserAirFlowRate: Unused if condenserType = "AirCooled"
    value = modelObject.ratedCondenserAirFlowRate();
    if (value) {
      idfObject.setDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedCondenserAirFlowRate, value.get());
    } else {
      idfObject.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedCondenserAirFlowRate, "Autosize");
    }

    // MaximumCyclingRate
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::MaximumCyclingRate, modelObject.maximumCyclingRate());

    // RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity,
                        modelObject.ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity());

    // LatentCapacityTimeConstant
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::LatentCapacityTimeConstant, modelObject.latentCapacityTimeConstant());

    // NominalTimeforCondensateRemovaltoBegin
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalTimeforCondensateRemovaltoBegin,
                        modelObject.nominalTimeforCondensateRemovaltoBegin());

    // ApplyLatentDegradationtoSpeedsGreaterthan1
    if (modelObject.applyLatentDegradationtoSpeedsGreaterthan1()) {
      idfObject.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::ApplyLatentDegradationtoSpeedsGreaterthan1, "Yes");
    } else {
      idfObject.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::ApplyLatentDegradationtoSpeedsGreaterthan1, "No");
    }

    // CondenserType
    idfObject.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::CondenserType, modelObject.condenserType());

    // NominalEvaporativeCondenserPumpPower
    value = modelObject.nominalEvaporativeCondenserPumpPower();
    if (value) {
      idfObject.setDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalEvaporativeCondenserPumpPower, value.get());
    } else {
      idfObject.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalEvaporativeCondenserPumpPower, "Autosize");
    }

    // NominalSpeedNumber
    idfObject.setInt(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalSpeedNumber, modelObject.nominalSpeedNumber());

    // SpeedxName
    for (auto& speed : modelObject.speeds()) {
      if (auto _s = translateAndMapModelObject(speed)) {
        auto eg = idfObject.pushExtensibleGroup();
        eg.setString(Coil_Cooling_DX_CurveFit_OperatingModeExtensibleFields::SpeedName, _s->nameString());
      } else {
        LOG(Warn, modelObject.briefDescription() << " cannot translate speed " << _s->briefDescription());
      }
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
