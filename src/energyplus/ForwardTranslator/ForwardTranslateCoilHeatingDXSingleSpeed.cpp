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

    if ((value = modelObject.ratedCOP())) {
      idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::GrossRatedHeatingCOP, value.get());
    }

    // RatedAirFlowRate

    if (modelObject.isRatedAirFlowRateAutosized()) {
      idfObject.setString(Coil_Heating_DX_SingleSpeedFields::RatedAirFlowRate, "Autosize");
    } else if ((value = modelObject.ratedAirFlowRate())) {
      idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::RatedAirFlowRate, value.get());
    }

    // Rated Supply Fan Power Per Volume Flow Rate

    if ((value = modelObject.ratedSupplyFanPowerPerVolumeFlowRate())) {
      idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::RatedSupplyFanPowerPerVolumeFlowRate, value.get());
    }

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

    if ((value = modelObject.minimumOutdoorDryBulbTemperatureforCompressorOperation())) {
      idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, value.get());
    }

    // MaximumOutdoorDryBulbTemperatureforDefrostOperation

    if ((value = modelObject.maximumOutdoorDryBulbTemperatureforDefrostOperation())) {
      idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation, value.get());
    }

    // CrankcaseHeaterCapacity

    if ((value = modelObject.crankcaseHeaterCapacity())) {
      idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::CrankcaseHeaterCapacity, value.get());
    }

    // MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation

    if ((value = modelObject.maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation())) {
      idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, value.get());
    }

    // DefrostStrategy

    if ((s = modelObject.defrostStrategy())) {
      idfObject.setString(Coil_Heating_DX_SingleSpeedFields::DefrostStrategy, s.get());
    }

    // DefrostControl

    if ((s = modelObject.defrostControl())) {
      idfObject.setString(Coil_Heating_DX_SingleSpeedFields::DefrostControl, s.get());
    }

    // DefrostTimePeriodFraction

    if ((value = modelObject.defrostTimePeriodFraction())) {
      idfObject.setDouble(Coil_Heating_DX_SingleSpeedFields::DefrostTimePeriodFraction, value.get());
    }

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
