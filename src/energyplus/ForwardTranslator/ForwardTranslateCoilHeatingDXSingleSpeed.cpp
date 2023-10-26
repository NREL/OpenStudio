/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/CoilHeatingDXSingleSpeed.hpp"
#include "../../model/CoilHeatingDXSingleSpeed_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"
#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/CoilSystem_Heating_DX_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingDXSingleSpeedWithoutUnitary(CoilHeatingDXSingleSpeed& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::Coil_Heating_DX_SingleSpeed);

    m_idfObjects.push_back(idfObject);

    // Name

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // AvailabilityScheduleName

    if (boost::optional<Schedule> schedule = modelObject.availabilitySchedule()) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(Coil_Heating_DX_SingleSpeedFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    // AirInletNodeName

    boost::optional<std::string> airInletNodeName;

    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        if ((s = node->name())) {
          airInletNodeName = s;

          idfObject.setString(Coil_Heating_DX_SingleSpeedFields::AirInletNodeName, s.get());
        }
      }
    }

    // AirOutletNodeName

    boost::optional<std::string> airOutletNodeName;

    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        if ((s = node->name())) {
          airOutletNodeName = s;

          idfObject.setString(Coil_Heating_DX_SingleSpeedFields::AirOutletNodeName, s.get());
        }
      }
    }

    // RatedTotalHeatingCapacity

    if (modelObject.isRatedTotalHeatingCapacityAutosized()) {
      idfObject.setString(Coil_Heating_DX_SingleSpeedFields::GrossRatedHeatingCapacity, "Autosize");
    } else if ((value = modelObject.ratedTotalHeatingCapacity())) {
      idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::GrossRatedHeatingCapacity, value.get());
    }

    // RatedCOP

    idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::GrossRatedHeatingCOP, modelObject.ratedCOP());

    // RatedAirFlowRate

    if (modelObject.isRatedAirFlowRateAutosized()) {
      idfObject.setString(Coil_Heating_DX_SingleSpeedFields::RatedAirFlowRate, "Autosize");
    } else if ((value = modelObject.ratedAirFlowRate())) {
      idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::RatedAirFlowRate, value.get());
    }

    // Rated Supply Fan Power Per Volume Flow Rate 2017

    idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::RatedSupplyFanPowerPerVolumeFlowRate2017,
                        modelObject.ratedSupplyFanPowerPerVolumeFlowRate2017());

    // Rated Supply Fan Power Per Volume Flow Rate 2023

    idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::RatedSupplyFanPowerPerVolumeFlowRate2023,
                        modelObject.ratedSupplyFanPowerPerVolumeFlowRate2023());

    // TotalHeatingCapacityFunctionofTemperatureCurveName

    ModelObject curve = modelObject.totalHeatingCapacityFunctionofTemperatureCurve();

    if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve)) {
      idfObject.setString(Coil_Heating_DX_SingleSpeedFields::HeatingCapacityFunctionofTemperatureCurveName, _curve->name().get());
    }

    // TotalHeatingCapacityFunctionofFlowFractionCurveName

    curve = modelObject.totalHeatingCapacityFunctionofFlowFractionCurve();

    if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve)) {
      idfObject.setString(Coil_Heating_DX_SingleSpeedFields::HeatingCapacityFunctionofFlowFractionCurveName, _curve->name().get());
    }

    // EnergyInputRatioFunctionofTemperatureCurveName

    curve = modelObject.energyInputRatioFunctionofTemperatureCurve();

    if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve)) {
      idfObject.setString(Coil_Heating_DX_SingleSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName, _curve->name().get());
    }

    // EnergyInputRatioFunctionofFlowFractionCurveName

    curve = modelObject.energyInputRatioFunctionofFlowFractionCurve();

    if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve)) {
      idfObject.setString(Coil_Heating_DX_SingleSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName, _curve->name().get());
    }

    // PartLoadFractionCorrelationCurveName

    curve = modelObject.partLoadFractionCorrelationCurve();

    if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve)) {
      idfObject.setString(Coil_Heating_DX_SingleSpeedFields::PartLoadFractionCorrelationCurveName, _curve->name().get());
    }

    // DefrostEnergyInputRatioFunctionofTemperatureCurveName

    if (boost::optional<Curve> curve = modelObject.defrostEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(Coil_Heating_DX_SingleSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName, _curve->name().get());
      }
    }

    // MinimumOutdoorDryBulbTemperatureforCompressorOperation

    idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,
                        modelObject.minimumOutdoorDryBulbTemperatureforCompressorOperation());

    // MaximumOutdoorDryBulbTemperatureforDefrostOperation

    idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation,
                        modelObject.maximumOutdoorDryBulbTemperatureforDefrostOperation());

    // CrankcaseHeaterCapacity

    idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::CrankcaseHeaterCapacity, modelObject.crankcaseHeaterCapacity());

    // CrankcaseHeaterCapacityFunctionofTemperatureCurveName
    if (auto crankCurve_ = modelObject.crankcaseHeaterCapacityFunctionofTemperatureCurve()) {
      if (auto curve_ = translateAndMapModelObject(*crankCurve_)) {
        idfObject.setString(Coil_Heating_DX_SingleSpeedFields::CrankcaseHeaterCapacityFunctionofTemperatureCurveName, curve_->nameString());
      }
    }

    // MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation

    if ((value = modelObject.maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation())) {
      idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, value.get());
    }

    // DefrostStrategy

    idfObject.setString(Coil_Heating_DX_SingleSpeedFields::DefrostStrategy, modelObject.defrostStrategy());

    // DefrostControl

    idfObject.setString(Coil_Heating_DX_SingleSpeedFields::DefrostControl, modelObject.defrostControl());

    // DefrostTimePeriodFraction

    idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::DefrostTimePeriodFraction, modelObject.defrostTimePeriodFraction());

    // ResistiveDefrostHeaterCapacity

    if (modelObject.isResistiveDefrostHeaterCapacityAutosized()) {
      idfObject.setString(Coil_Heating_DX_SingleSpeedFields::ResistiveDefrostHeaterCapacity, "Autosize");
    } else if ((value = modelObject.resistiveDefrostHeaterCapacity())) {
      idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::ResistiveDefrostHeaterCapacity, value.get());
    }

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingDXSingleSpeed(CoilHeatingDXSingleSpeed& modelObject) {
    IdfObject _coilSystemHeatingDX(IddObjectType::CoilSystem_Heating_DX);

    m_idfObjects.push_back(_coilSystemHeatingDX);

    boost::optional<IdfObject> _coilHeatingDXSingleSpeed = translateCoilHeatingDXSingleSpeedWithoutUnitary(modelObject);

    OS_ASSERT(_coilHeatingDXSingleSpeed);

    OptionalString s;

    s = modelObject.name();

    // Type and Name

    if (s) {
      _coilSystemHeatingDX.setString(CoilSystem_Heating_DXFields::HeatingCoilObjectType, _coilHeatingDXSingleSpeed->iddObject().name());

      _coilSystemHeatingDX.setString(CoilSystem_Heating_DXFields::HeatingCoilName, *s);

      _coilSystemHeatingDX.setName(*s + " CoilSystem");
    }

    // Availability Schedule

    Schedule schedule = modelObject.availabilitySchedule();

    boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule);

    if (_schedule) {
      _coilSystemHeatingDX.setString(CoilSystem_Heating_DXFields::AvailabilityScheduleName, _schedule->name().get());
    }

    return _coilSystemHeatingDX;
  }

}  // namespace energyplus

}  // namespace openstudio
