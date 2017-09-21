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

#include "CoilHeatingDXSingleSpeed.hpp"
#include "CoilHeatingDXSingleSpeed_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ZoneHVACPackagedTerminalHeatPump.hpp"
#include "ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingDXSingleSpeed_Impl::CoilHeatingDXSingleSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingDXSingleSpeed::iddObjectType());
  }

  CoilHeatingDXSingleSpeed_Impl::CoilHeatingDXSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
                                                                 : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingDXSingleSpeed::iddObjectType());
  }

  CoilHeatingDXSingleSpeed_Impl::CoilHeatingDXSingleSpeed_Impl(const CoilHeatingDXSingleSpeed_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
                                                                 : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingDXSingleSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingDXSingleSpeed_Impl::iddObjectType() const {
    return CoilHeatingDXSingleSpeed::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilHeatingDXSingleSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Heating_DX_SingleSpeedFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilHeatingDXSingleSpeed","Availability"));
    }
    return result;
  }

  boost::optional<double> CoilHeatingDXSingleSpeed_Impl::ratedTotalHeatingCapacity() const {
    return getDouble(OS_Coil_Heating_DX_SingleSpeedFields::RatedTotalHeatingCapacity,true);
  }

  bool CoilHeatingDXSingleSpeed_Impl::isRatedTotalHeatingCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_SingleSpeedFields::RatedTotalHeatingCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  double CoilHeatingDXSingleSpeed_Impl::ratedCOP() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_SingleSpeedFields::RatedCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilHeatingDXSingleSpeed_Impl::ratedAirFlowRate() const {
    return getDouble(OS_Coil_Heating_DX_SingleSpeedFields::RatedAirFlowRate,true);
  }

  bool CoilHeatingDXSingleSpeed_Impl::isRatedAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_SingleSpeedFields::RatedAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  double CoilHeatingDXSingleSpeed_Impl::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_SingleSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingDXSingleSpeed_Impl::isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted() const {
    return isEmpty(OS_Coil_Heating_DX_SingleSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation);
  }

  double CoilHeatingDXSingleSpeed_Impl::maximumOutdoorDryBulbTemperatureforDefrostOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingDXSingleSpeed_Impl::isMaximumOutdoorDryBulbTemperatureforDefrostOperationDefaulted() const {
    return isEmpty(OS_Coil_Heating_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation);
  }

  double CoilHeatingDXSingleSpeed_Impl::crankcaseHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_SingleSpeedFields::CrankcaseHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingDXSingleSpeed_Impl::isCrankcaseHeaterCapacityDefaulted() const {
    return isEmpty(OS_Coil_Heating_DX_SingleSpeedFields::CrankcaseHeaterCapacity);
  }

  double CoilHeatingDXSingleSpeed_Impl::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingDXSingleSpeed_Impl::isMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperationDefaulted() const {
    return isEmpty(OS_Coil_Heating_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
  }

  std::string CoilHeatingDXSingleSpeed_Impl::defrostStrategy() const {
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_SingleSpeedFields::DefrostStrategy,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingDXSingleSpeed_Impl::isDefrostStrategyDefaulted() const {
    return isEmpty(OS_Coil_Heating_DX_SingleSpeedFields::DefrostStrategy);
  }

  std::string CoilHeatingDXSingleSpeed_Impl::defrostControl() const {
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_SingleSpeedFields::DefrostControl,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingDXSingleSpeed_Impl::isDefrostControlDefaulted() const {
    return isEmpty(OS_Coil_Heating_DX_SingleSpeedFields::DefrostControl);
  }

  double CoilHeatingDXSingleSpeed_Impl::defrostTimePeriodFraction() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_SingleSpeedFields::DefrostTimePeriodFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingDXSingleSpeed_Impl::isDefrostTimePeriodFractionDefaulted() const {
    return isEmpty(OS_Coil_Heating_DX_SingleSpeedFields::DefrostTimePeriodFraction);
  }

  boost::optional<double> CoilHeatingDXSingleSpeed_Impl::resistiveDefrostHeaterCapacity() const {
    return getDouble(OS_Coil_Heating_DX_SingleSpeedFields::ResistiveDefrostHeaterCapacity,true);
  }

  bool CoilHeatingDXSingleSpeed_Impl::isResistiveDefrostHeaterCapacityDefaulted() const {
    return isEmpty(OS_Coil_Heating_DX_SingleSpeedFields::ResistiveDefrostHeaterCapacity);
  }

  bool CoilHeatingDXSingleSpeed_Impl::isResistiveDefrostHeaterCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_SingleSpeedFields::ResistiveDefrostHeaterCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  bool CoilHeatingDXSingleSpeed_Impl::setRatedTotalHeatingCapacity(boost::optional<double> ratedTotalHeatingCapacity) {
    bool result = false;
    if (ratedTotalHeatingCapacity) {
      result = setDouble(OS_Coil_Heating_DX_SingleSpeedFields::RatedTotalHeatingCapacity, ratedTotalHeatingCapacity.get());
    } else {
      result = setString(OS_Coil_Heating_DX_SingleSpeedFields::RatedTotalHeatingCapacity, "");
    }
    return result;
  }

  void CoilHeatingDXSingleSpeed_Impl::autosizeRatedTotalHeatingCapacity() {
    bool result = setString(OS_Coil_Heating_DX_SingleSpeedFields::RatedTotalHeatingCapacity, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXSingleSpeed_Impl::setRatedCOP(double ratedCOP) {
    bool result = setDouble(OS_Coil_Heating_DX_SingleSpeedFields::RatedCOP, ratedCOP);
    return result;
  }

  bool CoilHeatingDXSingleSpeed_Impl::setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate) {
    bool result = false;
    if (ratedAirFlowRate) {
      result = setDouble(OS_Coil_Heating_DX_SingleSpeedFields::RatedAirFlowRate, ratedAirFlowRate.get());
    } else {
      result = setString(OS_Coil_Heating_DX_SingleSpeedFields::RatedAirFlowRate, "");
    }
    return result;
  }

  void CoilHeatingDXSingleSpeed_Impl::autosizeRatedAirFlowRate() {
    bool result = setString(OS_Coil_Heating_DX_SingleSpeedFields::RatedAirFlowRate, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXSingleSpeed_Impl::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
    bool result = setDouble(OS_Coil_Heating_DX_SingleSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, minimumOutdoorDryBulbTemperatureforCompressorOperation);
    return result;
  }

  void CoilHeatingDXSingleSpeed_Impl::resetMinimumOutdoorDryBulbTemperatureforCompressorOperation() {
    bool result = setString(OS_Coil_Heating_DX_SingleSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXSingleSpeed_Impl::setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation) {
    bool result = setDouble(OS_Coil_Heating_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation, maximumOutdoorDryBulbTemperatureforDefrostOperation);
    return result;
  }

  void CoilHeatingDXSingleSpeed_Impl::resetMaximumOutdoorDryBulbTemperatureforDefrostOperation() {
    bool result = setString(OS_Coil_Heating_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXSingleSpeed_Impl::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
    bool result = setDouble(OS_Coil_Heating_DX_SingleSpeedFields::CrankcaseHeaterCapacity, crankcaseHeaterCapacity);
    return result;
  }

  void CoilHeatingDXSingleSpeed_Impl::resetCrankcaseHeaterCapacity() {
    bool result = setString(OS_Coil_Heating_DX_SingleSpeedFields::CrankcaseHeaterCapacity, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXSingleSpeed_Impl::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
    bool result = setDouble(OS_Coil_Heating_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
    return result;
  }

  void CoilHeatingDXSingleSpeed_Impl::resetMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() {
    bool result = setString(OS_Coil_Heating_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXSingleSpeed_Impl::setDefrostStrategy(std::string defrostStrategy) {
    bool result = setString(OS_Coil_Heating_DX_SingleSpeedFields::DefrostStrategy, defrostStrategy);
    return result;
  }

  void CoilHeatingDXSingleSpeed_Impl::resetDefrostStrategy() {
    bool result = setString(OS_Coil_Heating_DX_SingleSpeedFields::DefrostStrategy, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXSingleSpeed_Impl::setDefrostControl(std::string defrostControl) {
    bool result = setString(OS_Coil_Heating_DX_SingleSpeedFields::DefrostControl, defrostControl);
    return result;
  }

  void CoilHeatingDXSingleSpeed_Impl::resetDefrostControl() {
    bool result = setString(OS_Coil_Heating_DX_SingleSpeedFields::DefrostControl, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXSingleSpeed_Impl::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
    bool result = setDouble(OS_Coil_Heating_DX_SingleSpeedFields::DefrostTimePeriodFraction, defrostTimePeriodFraction);
    return result;
  }

  void CoilHeatingDXSingleSpeed_Impl::resetDefrostTimePeriodFraction() {
    bool result = setString(OS_Coil_Heating_DX_SingleSpeedFields::DefrostTimePeriodFraction, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXSingleSpeed_Impl::setResistiveDefrostHeaterCapacity(boost::optional<double> resistiveDefrostHeaterCapacity) {
    bool result = false;
    if (resistiveDefrostHeaterCapacity) {
      result = setDouble(OS_Coil_Heating_DX_SingleSpeedFields::ResistiveDefrostHeaterCapacity, resistiveDefrostHeaterCapacity.get());
    } else {
      result = setString(OS_Coil_Heating_DX_SingleSpeedFields::ResistiveDefrostHeaterCapacity, "");
    }
    return result;
  }

  void CoilHeatingDXSingleSpeed_Impl::resetResistiveDefrostHeaterCapacity() {
    bool result = setString(OS_Coil_Heating_DX_SingleSpeedFields::ResistiveDefrostHeaterCapacity, "");
    OS_ASSERT(result);
  }

  void CoilHeatingDXSingleSpeed_Impl::autosizeResistiveDefrostHeaterCapacity() {
    bool result = setString(OS_Coil_Heating_DX_SingleSpeedFields::ResistiveDefrostHeaterCapacity, "Autosize");
    OS_ASSERT(result);
  }

  unsigned CoilHeatingDXSingleSpeed_Impl::inletPort()
  {
    return OS_Coil_Heating_DX_SingleSpeedFields::AirInletNodeName;
  }

  unsigned CoilHeatingDXSingleSpeed_Impl::outletPort()
  {
    return OS_Coil_Heating_DX_SingleSpeedFields::AirOutletNodeName;
  }

  Schedule CoilHeatingDXSingleSpeed_Impl::availabilitySchedule() const
  {
    boost::optional<Schedule> value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_DX_SingleSpeedFields::AvailabilityScheduleName);
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<CoilHeatingDXSingleSpeed_Impl*>(this)->setAvailabilitySchedule(*value);
      value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_DX_SingleSpeedFields::AvailabilityScheduleName);
    }
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingDXSingleSpeed_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Heating_DX_SingleSpeedFields::AvailabilityScheduleName,
                              "CoilHeatingDXSingleSpeed",
                              "Availability",
                              schedule);
    return result;
  }

  // Curves

  Curve CoilHeatingDXSingleSpeed_Impl::totalHeatingCapacityFunctionofTemperatureCurve() const
  {
    boost::optional<Curve> curve;

    curve =
        getObject<ModelObject>().getModelObjectTarget<Curve>(
            OS_Coil_Heating_DX_SingleSpeedFields::TotalHeatingCapacityFunctionofTemperatureCurveName);

    OS_ASSERT(curve);

    return curve.get();
  }

  Curve CoilHeatingDXSingleSpeed_Impl::totalHeatingCapacityFunctionofFlowFractionCurve() const
  {
    boost::optional<Curve> curve;

    curve =
        getObject<ModelObject>().getModelObjectTarget<Curve>(
            OS_Coil_Heating_DX_SingleSpeedFields::TotalHeatingCapacityFunctionofFlowFractionCurveName);

    OS_ASSERT(curve);

    return curve.get();
  }

  Curve CoilHeatingDXSingleSpeed_Impl::energyInputRatioFunctionofTemperatureCurve() const
  {
    boost::optional<Curve> curve;

    curve =
        getObject<ModelObject>().getModelObjectTarget<Curve>(
            OS_Coil_Heating_DX_SingleSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName);

    OS_ASSERT(curve);

    return curve.get();
  }

  Curve CoilHeatingDXSingleSpeed_Impl::energyInputRatioFunctionofFlowFractionCurve() const
  {
    boost::optional<Curve> curve;

    curve =
        getObject<ModelObject>().getModelObjectTarget<Curve>(
            OS_Coil_Heating_DX_SingleSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName);

    OS_ASSERT(curve);

    return curve.get();
  }

  Curve CoilHeatingDXSingleSpeed_Impl::partLoadFractionCorrelationCurve() const
  {
    boost::optional<Curve> curve;

    curve =
        getObject<ModelObject>().getModelObjectTarget<Curve>(
            OS_Coil_Heating_DX_SingleSpeedFields::PartLoadFractionCorrelationCurveName);

    OS_ASSERT(curve);

    return curve.get();
  }

  boost::optional<Curve> CoilHeatingDXSingleSpeed_Impl::defrostEnergyInputRatioFunctionofTemperatureCurve() const
  {
    boost::optional<Curve> curve;

    curve =
        getObject<ModelObject>().getModelObjectTarget<Curve>(
            OS_Coil_Heating_DX_SingleSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName);

    return curve;
  }

  // A5 , \field Total Heating Capacity Function of Temperature Curve Name
  // \object-list BiquadraticQuadraticCubicCurves
  bool CoilHeatingDXSingleSpeed_Impl::setTotalHeatingCapacityFunctionofTemperatureCurve( const Curve& curve )
  {
    bool accepted = false;

    if( model() != curve.model() )
    {
      return accepted;
    }

    if( curve.optionalCast<CurveBiquadratic>() )
    {
      accepted = true;
    }
    else if( curve.optionalCast<CurveQuadratic>() )
    {
      accepted = true;
    }
    else if( curve.optionalCast<CurveCubic>() )
    {
      accepted = true;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Heating_DX_SingleSpeedFields::TotalHeatingCapacityFunctionofTemperatureCurveName,curve.handle()));

    return accepted;
  }

  // A6 , \field Total Heating Capacity Function of Flow Fraction Curve Name
  // \object-list QuadraticCubicCurves
  bool CoilHeatingDXSingleSpeed_Impl::setTotalHeatingCapacityFunctionofFlowFractionCurve( const Curve& curve )
  {
    bool accepted = false;

    if( model() != curve.model() )
    {
      return accepted;
    }

    if( curve.optionalCast<CurveQuadratic>() )
    {
      accepted = true;
    }
    else if( curve.optionalCast<CurveCubic>() )
    {
      accepted = true;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Heating_DX_SingleSpeedFields::TotalHeatingCapacityFunctionofFlowFractionCurveName,curve.handle()));

    return accepted;
  }

  // A7 ,  \field Energy Input Ratio Function of Temperature Curve Name
  // \object-list BiquadraticQuadraticCubicCurves
  bool CoilHeatingDXSingleSpeed_Impl::setEnergyInputRatioFunctionofTemperatureCurve( const Curve& curve )
  {
    bool accepted = false;

    if( model() != curve.model() )
    {
      return accepted;
    }

    if( curve.optionalCast<CurveBiquadratic>() )
    {
      accepted = true;
    }
    else if( curve.optionalCast<CurveQuadratic>() )
    {
      accepted = true;
    }
    else if( curve.optionalCast<CurveCubic>() )
    {
      accepted = true;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Heating_DX_SingleSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName,curve.handle()));

    return accepted;
  }

  // A8 , \field Energy Input Ratio Function of Flow Fraction Curve Name
  // \object-list QuadraticCubicCurves
  bool CoilHeatingDXSingleSpeed_Impl::setEnergyInputRatioFunctionofFlowFractionCurve( const Curve& curve )
  {
    bool accepted = false;

    if( model() != curve.model() )
    {
      return accepted;
    }

    if( curve.optionalCast<CurveQuadratic>() )
    {
      accepted = true;
    }
    else if( curve.optionalCast<CurveCubic>() )
    {
      accepted = true;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Heating_DX_SingleSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName,curve.handle()));

    return accepted;
  }

  // A9 , \field Part Load Fraction Correlation Curve Name
  // \object-list QuadraticCubicCurves
  bool CoilHeatingDXSingleSpeed_Impl::setPartLoadFractionCorrelationCurve( const Curve& curve )
  {
    bool accepted = false;

    if( model() != curve.model() )
    {
      return accepted;
    }

    if( curve.optionalCast<CurveQuadratic>() )
    {
      accepted = true;
    }
    else if( curve.optionalCast<CurveCubic>() )
    {
      accepted = true;
    }

    OS_ASSERT(this->setPointer(OS_Coil_Heating_DX_SingleSpeedFields::PartLoadFractionCorrelationCurveName,curve.handle()));

    return accepted;
  }

  // A10, \field Defrost Energy Input Ratio Function of Temperature Curve Name
  // \object-list BiquadraticCurves
  // not a required curve so it needs a reset
  bool CoilHeatingDXSingleSpeed_Impl::setDefrostEnergyInputRatioFunctionofTemperatureCurve(const boost::optional<Curve> curve )
  {
    bool accepted = false;

    //first, check if a curve was actually passed in
    if (curve)
    {
      //then, check if the curve is of the appropriate type.  if not, accepted stays false
      if( curve->optionalCast<CurveBiquadratic>() )
      {
        accepted = true;
        OS_ASSERT(this->setPointer(OS_Coil_Heating_DX_SingleSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName,curve->handle()));
      }
    }
    //if no curve was passed in, then set it to "" (this is to accomodate the reset method)
    else
    {
      setString(OS_Coil_Heating_DX_SingleSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName,"");
      accepted = true;
    }

    return accepted;
  }

  ModelObject CoilHeatingDXSingleSpeed_Impl::clone(Model model) const
  {
    CoilHeatingDXSingleSpeed newCoil = StraightComponent_Impl::clone(model).cast<CoilHeatingDXSingleSpeed>();

    return newCoil;
  }

  boost::optional<HVACComponent> CoilHeatingDXSingleSpeed_Impl::containingHVACComponent() const
  {
    // AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

    for( const auto & airLoopHVACUnitarySystem : airLoopHVACUnitarySystems )
    {
      if( boost::optional<HVACComponent> heatingCoil = airLoopHVACUnitarySystem.heatingCoil() )
      {
        if( heatingCoil->handle() == this->handle() )
        {
          return airLoopHVACUnitarySystem;
        }
      }
    }

    // AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass
    std::vector<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass> bypassSystems = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>();

    for( const auto & bypassSystem : bypassSystems )
    {
      if( boost::optional<HVACComponent> heatingCoil = bypassSystem.heatingCoil() )
      {
        if( heatingCoil->handle() == this->handle() )
        {
          return bypassSystem;
        }
      }
    }

    // AirLoopHVACUnitaryHeatPumpAirToAir

    std::vector<AirLoopHVACUnitaryHeatPumpAirToAir> airLoopHVACUnitaryHeatPumpAirToAirs;

    airLoopHVACUnitaryHeatPumpAirToAirs = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

    for( const auto & airLoopHVACUnitaryHeatPumpAirToAir : airLoopHVACUnitaryHeatPumpAirToAirs )
    {
      if( boost::optional<HVACComponent> coil = airLoopHVACUnitaryHeatPumpAirToAir.heatingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return airLoopHVACUnitaryHeatPumpAirToAir;
        }
      }
    }

    return boost::none;
  }

  boost::optional<ZoneHVACComponent> CoilHeatingDXSingleSpeed_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACPackagedTerminalHeatPump

    std::vector<ZoneHVACPackagedTerminalHeatPump> zoneHVACPackagedTerminalHeatPumps;

    zoneHVACPackagedTerminalHeatPumps = this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalHeatPump>();

    for( const auto & zoneHVACPackagedTerminalHeatPump : zoneHVACPackagedTerminalHeatPumps )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACPackagedTerminalHeatPump.heatingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return zoneHVACPackagedTerminalHeatPump;
        }
      }
    }

    return boost::none;
  }

  std::vector<ModelObject> CoilHeatingDXSingleSpeed_Impl::children() const
  {
    std::vector<ModelObject> result;

    result.push_back( totalHeatingCapacityFunctionofTemperatureCurve() );
    result.push_back( totalHeatingCapacityFunctionofFlowFractionCurve() );
    result.push_back( energyInputRatioFunctionofTemperatureCurve() );
    result.push_back( energyInputRatioFunctionofFlowFractionCurve() );
    result.push_back( partLoadFractionCorrelationCurve() );

    return result;
  }

  boost::optional<ModelObject> CoilHeatingDXSingleSpeed_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  bool CoilHeatingDXSingleSpeed_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  bool CoilHeatingDXSingleSpeed_Impl::addToNode(Node & node)
  {
    if( boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC() )
    {
      if( ! airLoop->demandComponent(node.handle()) )
      {
        return StraightComponent_Impl::addToNode( node );
      }
    }

    return false;
  }

  double CoilHeatingDXSingleSpeed_Impl::ratedSupplyFanPowerPerVolumeFlowRate() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_SingleSpeedFields::RatedSupplyFanPowerPerVolumeFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingDXSingleSpeed_Impl::setRatedSupplyFanPowerPerVolumeFlowRate(double ratedSupplyFanPowerPerVolumeFlowRate) {
    bool result = setDouble(OS_Coil_Heating_DX_SingleSpeedFields::RatedSupplyFanPowerPerVolumeFlowRate, ratedSupplyFanPowerPerVolumeFlowRate);
    return result;
  }

} // detail

CoilHeatingDXSingleSpeed::CoilHeatingDXSingleSpeed( const Model& model,
                                                    Schedule & availabilitySchedule,
                                                    Curve& totalHeatingCapacityFunctionofTemperatureCurve,
                                                    Curve& totalHeatingCapacityFunctionofFlowFractionCurve,
                                                    Curve& energyInputRatioFunctionofTemperatureCurve,
                                                    Curve& energyInputRatioFunctionofFlowFractionCurve,
                                                    Curve& partLoadFractionCorrelationCurve
                                                  )
  : StraightComponent(CoilHeatingDXSingleSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingDXSingleSpeed_Impl>());

  setAvailabilitySchedule(availabilitySchedule);

  setTotalHeatingCapacityFunctionofTemperatureCurve(totalHeatingCapacityFunctionofTemperatureCurve);

  setTotalHeatingCapacityFunctionofFlowFractionCurve(totalHeatingCapacityFunctionofFlowFractionCurve);

  setEnergyInputRatioFunctionofTemperatureCurve(energyInputRatioFunctionofTemperatureCurve);

  setEnergyInputRatioFunctionofFlowFractionCurve(energyInputRatioFunctionofFlowFractionCurve);

  setPartLoadFractionCorrelationCurve(partLoadFractionCorrelationCurve);

  autosizeRatedTotalHeatingCapacity();

  autosizeRatedAirFlowRate();

  setRatedSupplyFanPowerPerVolumeFlowRate(773.3);

  setRatedCOP(5.0);

  setDefrostStrategy("Resistive");

  setDefrostControl("Timed");

  setDefrostTimePeriodFraction(0.166667);

  setResistiveDefrostHeaterCapacity(2000.0);
}

CoilHeatingDXSingleSpeed::CoilHeatingDXSingleSpeed(const Model& model)
  : StraightComponent(CoilHeatingDXSingleSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingDXSingleSpeed_Impl>());

  auto availabilitySchedule = model.alwaysOnDiscreteSchedule();
  setAvailabilitySchedule(availabilitySchedule);

  CurveCubic totalHeatingCapacityFunctionofTemperatureCurve(model);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient1Constant(0.758746);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient2x(0.027626);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient3xPOW2(0.000148716);
  totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient4xPOW3(0.0000034992);
  totalHeatingCapacityFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
  totalHeatingCapacityFunctionofTemperatureCurve.setMaximumValueofx(20.0);

  CurveCubic totalHeatingCapacityFunctionofFlowFractionCurve(model);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient1Constant(0.84);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient2x(0.16);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0);
  totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient4xPOW3(0.0);
  totalHeatingCapacityFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
  totalHeatingCapacityFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

  CurveCubic energyInputRatioFunctionofTemperatureCurve(model);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient1Constant(1.19248);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient2x(-0.0300438);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient3xPOW2(0.00103745);
  energyInputRatioFunctionofTemperatureCurve.setCoefficient4xPOW3(-0.000023328);
  energyInputRatioFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
  energyInputRatioFunctionofTemperatureCurve.setMaximumValueofx(20.0);

  CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(model);
  energyInputRatioFunctionofFlowFractionCurve.setCoefficient1Constant(1.3824);
  energyInputRatioFunctionofFlowFractionCurve.setCoefficient2x(-0.4336);
  energyInputRatioFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0512);
  energyInputRatioFunctionofFlowFractionCurve.setMinimumValueofx(0.0);
  energyInputRatioFunctionofFlowFractionCurve.setMaximumValueofx(1.0);

  CurveQuadratic partLoadFractionCorrelationCurve(model);
  partLoadFractionCorrelationCurve.setCoefficient1Constant(0.75);
  partLoadFractionCorrelationCurve.setCoefficient2x(0.25);
  partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
  partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
  partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);

  setTotalHeatingCapacityFunctionofTemperatureCurve(totalHeatingCapacityFunctionofTemperatureCurve);

  setTotalHeatingCapacityFunctionofFlowFractionCurve(totalHeatingCapacityFunctionofFlowFractionCurve);

  setEnergyInputRatioFunctionofTemperatureCurve(energyInputRatioFunctionofTemperatureCurve);

  setEnergyInputRatioFunctionofFlowFractionCurve(energyInputRatioFunctionofFlowFractionCurve);

  setPartLoadFractionCorrelationCurve(partLoadFractionCorrelationCurve);

  autosizeRatedTotalHeatingCapacity();

  autosizeRatedAirFlowRate();

  setRatedSupplyFanPowerPerVolumeFlowRate(773.3);

  setRatedCOP(5.0);

  setDefrostStrategy("Resistive");

  setDefrostControl("Timed");

  setDefrostTimePeriodFraction(0.166667);

  setResistiveDefrostHeaterCapacity(2000.0);
}


IddObjectType CoilHeatingDXSingleSpeed::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Coil_Heating_DX_SingleSpeed);
  return result;
}

std::vector<std::string> CoilHeatingDXSingleSpeed::validDefrostStrategyValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_Heating_DX_SingleSpeedFields::DefrostStrategy);
}

std::vector<std::string> CoilHeatingDXSingleSpeed::validDefrostControlValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_Heating_DX_SingleSpeedFields::DefrostControl);
}

boost::optional<double> CoilHeatingDXSingleSpeed::ratedTotalHeatingCapacity() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->ratedTotalHeatingCapacity();
}

bool CoilHeatingDXSingleSpeed::isRatedTotalHeatingCapacityAutosized() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->isRatedTotalHeatingCapacityAutosized();
}

double CoilHeatingDXSingleSpeed::ratedCOP() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->ratedCOP();
}

boost::optional<double> CoilHeatingDXSingleSpeed::ratedAirFlowRate() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->ratedAirFlowRate();
}

bool CoilHeatingDXSingleSpeed::isRatedAirFlowRateAutosized() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->isRatedAirFlowRateAutosized();
}

double CoilHeatingDXSingleSpeed::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->minimumOutdoorDryBulbTemperatureforCompressorOperation();
}

bool CoilHeatingDXSingleSpeed::isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted();
}

double CoilHeatingDXSingleSpeed::maximumOutdoorDryBulbTemperatureforDefrostOperation() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->maximumOutdoorDryBulbTemperatureforDefrostOperation();
}

bool CoilHeatingDXSingleSpeed::isMaximumOutdoorDryBulbTemperatureforDefrostOperationDefaulted() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->isMaximumOutdoorDryBulbTemperatureforDefrostOperationDefaulted();
}

double CoilHeatingDXSingleSpeed::crankcaseHeaterCapacity() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->crankcaseHeaterCapacity();
}

bool CoilHeatingDXSingleSpeed::isCrankcaseHeaterCapacityDefaulted() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->isCrankcaseHeaterCapacityDefaulted();
}

double CoilHeatingDXSingleSpeed::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();
}

bool CoilHeatingDXSingleSpeed::isMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperationDefaulted() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->isMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperationDefaulted();
}

std::string CoilHeatingDXSingleSpeed::defrostStrategy() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->defrostStrategy();
}

bool CoilHeatingDXSingleSpeed::isDefrostStrategyDefaulted() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->isDefrostStrategyDefaulted();
}

std::string CoilHeatingDXSingleSpeed::defrostControl() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->defrostControl();
}

bool CoilHeatingDXSingleSpeed::isDefrostControlDefaulted() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->isDefrostControlDefaulted();
}

double CoilHeatingDXSingleSpeed::defrostTimePeriodFraction() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->defrostTimePeriodFraction();
}

bool CoilHeatingDXSingleSpeed::isDefrostTimePeriodFractionDefaulted() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->isDefrostTimePeriodFractionDefaulted();
}

boost::optional<double> CoilHeatingDXSingleSpeed::resistiveDefrostHeaterCapacity() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->resistiveDefrostHeaterCapacity();
}

bool CoilHeatingDXSingleSpeed::isResistiveDefrostHeaterCapacityDefaulted() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->isResistiveDefrostHeaterCapacityDefaulted();
}

bool CoilHeatingDXSingleSpeed::isResistiveDefrostHeaterCapacityAutosized() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->isResistiveDefrostHeaterCapacityAutosized();
}

bool CoilHeatingDXSingleSpeed::setRatedTotalHeatingCapacity(double ratedTotalHeatingCapacity) {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setRatedTotalHeatingCapacity(ratedTotalHeatingCapacity);
}

void CoilHeatingDXSingleSpeed::autosizeRatedTotalHeatingCapacity() {
  getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->autosizeRatedTotalHeatingCapacity();
}

bool CoilHeatingDXSingleSpeed::setRatedCOP(double ratedCOP) {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setRatedCOP(ratedCOP);
}

bool CoilHeatingDXSingleSpeed::setRatedAirFlowRate(double ratedAirFlowRate) {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setRatedAirFlowRate(ratedAirFlowRate);
}

void CoilHeatingDXSingleSpeed::autosizeRatedAirFlowRate() {
  getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->autosizeRatedAirFlowRate();
}

bool CoilHeatingDXSingleSpeed::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setMinimumOutdoorDryBulbTemperatureforCompressorOperation(minimumOutdoorDryBulbTemperatureforCompressorOperation);
}

void CoilHeatingDXSingleSpeed::resetMinimumOutdoorDryBulbTemperatureforCompressorOperation() {
  getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->resetMinimumOutdoorDryBulbTemperatureforCompressorOperation();
}

bool CoilHeatingDXSingleSpeed::setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation) {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureforDefrostOperation(maximumOutdoorDryBulbTemperatureforDefrostOperation);
}

void CoilHeatingDXSingleSpeed::resetMaximumOutdoorDryBulbTemperatureforDefrostOperation() {
  getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->resetMaximumOutdoorDryBulbTemperatureforDefrostOperation();
}

bool CoilHeatingDXSingleSpeed::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setCrankcaseHeaterCapacity(crankcaseHeaterCapacity);
}

void CoilHeatingDXSingleSpeed::resetCrankcaseHeaterCapacity() {
  getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->resetCrankcaseHeaterCapacity();
}

bool CoilHeatingDXSingleSpeed::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
}

void CoilHeatingDXSingleSpeed::resetMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() {
  getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->resetMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();
}

bool CoilHeatingDXSingleSpeed::setDefrostStrategy(std::string defrostStrategy) {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setDefrostStrategy(defrostStrategy);
}

void CoilHeatingDXSingleSpeed::resetDefrostStrategy() {
  getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->resetDefrostStrategy();
}

bool CoilHeatingDXSingleSpeed::setDefrostControl(std::string defrostControl) {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setDefrostControl(defrostControl);
}

void CoilHeatingDXSingleSpeed::resetDefrostControl() {
  getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->resetDefrostControl();
}

bool CoilHeatingDXSingleSpeed::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setDefrostTimePeriodFraction(defrostTimePeriodFraction);
}

void CoilHeatingDXSingleSpeed::resetDefrostTimePeriodFraction() {
  getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->resetDefrostTimePeriodFraction();
}

bool CoilHeatingDXSingleSpeed::setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity) {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setResistiveDefrostHeaterCapacity(resistiveDefrostHeaterCapacity);
}

void CoilHeatingDXSingleSpeed::resetResistiveDefrostHeaterCapacity() {
  getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->resetResistiveDefrostHeaterCapacity();
}

void CoilHeatingDXSingleSpeed::autosizeResistiveDefrostHeaterCapacity() {
  getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->autosizeResistiveDefrostHeaterCapacity();
}

Schedule CoilHeatingDXSingleSpeed::availabilitySchedule() const
{
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->availabilitySchedule();
}

bool CoilHeatingDXSingleSpeed::setAvailabilitySchedule(Schedule & schedule )
{
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setAvailabilitySchedule( schedule );
}

// Get Curves

Curve CoilHeatingDXSingleSpeed::totalHeatingCapacityFunctionofTemperatureCurve() const
{
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->totalHeatingCapacityFunctionofTemperatureCurve();
}

Curve CoilHeatingDXSingleSpeed::totalHeatingCapacityFunctionofFlowFractionCurve() const
{
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->totalHeatingCapacityFunctionofFlowFractionCurve();
}

Curve CoilHeatingDXSingleSpeed::energyInputRatioFunctionofTemperatureCurve() const
{
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->energyInputRatioFunctionofTemperatureCurve();
}

Curve CoilHeatingDXSingleSpeed::energyInputRatioFunctionofFlowFractionCurve() const
{
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->energyInputRatioFunctionofFlowFractionCurve();
}

Curve CoilHeatingDXSingleSpeed::partLoadFractionCorrelationCurve() const
{
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->partLoadFractionCorrelationCurve();
}

boost::optional<Curve> CoilHeatingDXSingleSpeed::defrostEnergyInputRatioFunctionofTemperatureCurve() const
{
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->defrostEnergyInputRatioFunctionofTemperatureCurve();
}

// Set Curves

bool CoilHeatingDXSingleSpeed::setTotalHeatingCapacityFunctionofTemperatureCurve( const Curve& curve )
{
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setTotalHeatingCapacityFunctionofTemperatureCurve( curve );
}

bool CoilHeatingDXSingleSpeed::setTotalHeatingCapacityFunctionofFlowFractionCurve( const Curve& curve )
{
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setTotalHeatingCapacityFunctionofFlowFractionCurve( curve );
}

bool CoilHeatingDXSingleSpeed::setEnergyInputRatioFunctionofTemperatureCurve( const Curve& curve )
{
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setEnergyInputRatioFunctionofTemperatureCurve( curve );
}

bool CoilHeatingDXSingleSpeed::setEnergyInputRatioFunctionofFlowFractionCurve( const Curve& curve )
{
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setEnergyInputRatioFunctionofFlowFractionCurve( curve );
}

bool CoilHeatingDXSingleSpeed::setPartLoadFractionCorrelationCurve( const Curve& curve )
{
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setPartLoadFractionCorrelationCurve( curve );
}

bool CoilHeatingDXSingleSpeed::setDefrostEnergyInputRatioFunctionofTemperatureCurve( const Curve& curve )
{
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setDefrostEnergyInputRatioFunctionofTemperatureCurve( curve );
}

void CoilHeatingDXSingleSpeed::resetDefrostEnergyInputRatioFunctionofTemperatureCurve()
{
  getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setDefrostEnergyInputRatioFunctionofTemperatureCurve(boost::none);
}

double CoilHeatingDXSingleSpeed::ratedSupplyFanPowerPerVolumeFlowRate() const {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->ratedSupplyFanPowerPerVolumeFlowRate();
}

bool CoilHeatingDXSingleSpeed::setRatedSupplyFanPowerPerVolumeFlowRate(double ratedSupplyFanPowerPerVolumeFlowRate) {
  return getImpl<detail::CoilHeatingDXSingleSpeed_Impl>()->setRatedSupplyFanPowerPerVolumeFlowRate(ratedSupplyFanPowerPerVolumeFlowRate);
}

/// @cond
CoilHeatingDXSingleSpeed::CoilHeatingDXSingleSpeed(std::shared_ptr<detail::CoilHeatingDXSingleSpeed_Impl> impl)
  : StraightComponent(std::move(impl))
{}

/// @endcond

} // model

} // openstudio

