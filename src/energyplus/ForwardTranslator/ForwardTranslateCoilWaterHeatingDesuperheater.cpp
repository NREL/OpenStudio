/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/CoilWaterHeatingDesuperheater.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Coil_WaterHeating_Desuperheater_FieldEnums.hxx>
#include <utilities/idd/CoilSystem_Cooling_DX_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilWaterHeatingDesuperheater(CoilWaterHeatingDesuperheater& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_WaterHeating_Desuperheater, modelObject);

    // AvailabilityScheduleName
    boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule();

    if (availabilitySchedule) {
      boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

      if (_availabilitySchedule && _availabilitySchedule->name()) {
        idfObject.setString(Coil_WaterHeating_DesuperheaterFields::AvailabilityScheduleName, _availabilitySchedule->name().get());
      }
    }

    // SetpointTemperatureScheduleName
    boost::optional<Schedule> setpointTemperatureSchedule = modelObject.setpointTemperatureSchedule();

    if (setpointTemperatureSchedule) {
      boost::optional<IdfObject> _setpointTemperatureSchedule = translateAndMapModelObject(setpointTemperatureSchedule.get());

      if (_setpointTemperatureSchedule && _setpointTemperatureSchedule->name()) {
        idfObject.setString(Coil_WaterHeating_DesuperheaterFields::SetpointTemperatureScheduleName, _setpointTemperatureSchedule->name().get());
      }
    }

    // DeadBandTemperatureDifference
    d = modelObject.deadBandTemperatureDifference();
    if (d) {
      idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::DeadBandTemperatureDifference, d.get());
    }

    // RatedHeatReclaimRecoveryEfficiency
    d = modelObject.ratedHeatReclaimRecoveryEfficiency();
    if (d) {
      idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::RatedHeatReclaimRecoveryEfficiency, d.get());
    }

    // RatedInletWaterTemperature
    d = modelObject.ratedInletWaterTemperature();
    if (d) {
      idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::RatedInletWaterTemperature, d.get());
    }

    // RatedOutdoorAirTemperature
    d = modelObject.ratedOutdoorAirTemperature();
    if (d) {
      idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::RatedOutdoorAirTemperature, d.get());
    }

    // MaximumInletWaterTemperatureforHeatReclaim
    d = modelObject.maximumInletWaterTemperatureforHeatReclaim();
    if (d) {
      idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::MaximumInletWaterTemperatureforHeatReclaim, d.get());
    }

    // HeatReclaimEfficiencyFunctionofTemperatureCurveName
    boost::optional<CurveBiquadratic> heatReclaimCurve = modelObject.heatReclaimEfficiencyFunctionofTemperatureCurve();

    if (heatReclaimCurve) {
      boost::optional<IdfObject> _heatReclaimCurve = translateAndMapModelObject(heatReclaimCurve.get());

      if (_heatReclaimCurve && _heatReclaimCurve->name()) {
        idfObject.setString(Coil_WaterHeating_DesuperheaterFields::HeatReclaimEfficiencyFunctionofTemperatureCurveName,
                            _heatReclaimCurve->name().get());
      }
    }

    // WaterInletNodeName
    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_WaterHeating_DesuperheaterFields::WaterInletNodeName, node->name().get());
      }
    }

    // WaterOutletNodeName
    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_WaterHeating_DesuperheaterFields::WaterOutletNodeName, node->name().get());
      }
    }

    // TankObjectType
    // TankName
    if (boost::optional<ModelObject> heatRejectionTarget = modelObject.heatRejectionTarget()) {
      boost::optional<IdfObject> _heatRejectionTarget = translateAndMapModelObject(heatRejectionTarget.get());

      if (_heatRejectionTarget && _heatRejectionTarget->name()) {
        idfObject.setString(Coil_WaterHeating_DesuperheaterFields::TankObjectType, _heatRejectionTarget->iddObject().name());
        idfObject.setString(Coil_WaterHeating_DesuperheaterFields::TankName, _heatRejectionTarget->name().get());
      }
    }

    // HeatingSourceObjectType
    // HeatingSourceName
    if (boost::optional<ModelObject> heatingSource = modelObject.heatingSource()) {

      boost::optional<IdfObject> _heatingSource = translateAndMapModelObject(*heatingSource);

      if (_heatingSource) {

        // Note JM 2019-03-14:
        // If the coil in question is a DX coil (CoilCoolingDXSingleSpeed, CoilCoolingDXTwoSpeed, CoilCoolingDXTwoStageWithHumidityControlMode,
        // CoilCoolingDXVariableSpeed, CoilCoolingDXMultiSpeed, CoilCoolingDX)
        // and this DX coil isn't already wrapped in a Unitary, then the FT will wrap it into a CoilSystem:Cooling:DX object and return that, but we
        // need the DX coil here and not the wrapper.
        //
        // Note: Other accepted types are objects and don't suffer the same problem
        // (Refrigeration:Condenser:AirCooled, Refrigeration:Condenser:EvaporativeCooled, Refrigeration:Condenser:WaterCooled,
        // Coil:Cooling:WaterToAirHeatPump:EquationFit, Refrigeration:CompressorRack)

        std::string objectType;
        std::string objectName;

        if (_heatingSource->iddObject().type() == IddObjectType::CoilSystem_Cooling_DX) {
          // We must retrieve the coil itself, not the wrapper
          objectType = _heatingSource->getString(CoilSystem_Cooling_DXFields::CoolingCoilObjectType).get();
          objectName = _heatingSource->getString(CoilSystem_Cooling_DXFields::CoolingCoilName).get();
        } else {
          objectType = _heatingSource->iddObject().name();
          objectName = _heatingSource->name().get();
        }

        if (objectType.empty() || objectName.empty()) {
          // We log an error but let E+ fail eventually
          LOG(Error, "Something went wrong in the translation of " << modelObject.briefDescription() << ", couldn't retrieve the coil");
          // OS_ASSERT(false);
        } else {
          idfObject.setString(Coil_WaterHeating_DesuperheaterFields::HeatingSourceObjectType, objectType);
          idfObject.setString(Coil_WaterHeating_DesuperheaterFields::HeatingSourceName, objectName);
        }
      }
    }

    // WaterFlowRate
    d = modelObject.waterFlowRate();
    if (d) {
      idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::WaterFlowRate, d.get());
    }

    // WaterPumpPower
    d = modelObject.waterPumpPower();
    if (d) {
      idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::WaterPumpPower, d.get());
    }

    // FractionofPumpHeattoWater
    d = modelObject.fractionofPumpHeattoWater();
    if (d) {
      idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::FractionofPumpHeattoWater, d.get());
    }

    // OnCycleParasiticElectricLoad
    d = modelObject.onCycleParasiticElectricLoad();
    if (d) {
      idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::OnCycleParasiticElectricLoad, d.get());
    }

    // OffCycleParasiticElectricLoad
    d = modelObject.offCycleParasiticElectricLoad();
    if (d) {
      idfObject.setDouble(Coil_WaterHeating_DesuperheaterFields::OffCycleParasiticElectricLoad, d.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
