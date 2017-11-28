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

#include "CoilHeatingDXVariableSpeed.hpp"
#include "CoilHeatingDXVariableSpeed_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CoilHeatingDXVariableSpeedSpeedData.hpp"
#include "CoilHeatingDXVariableSpeedSpeedData_Impl.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"
#include "ZoneHVACPackagedTerminalHeatPump.hpp"
#include "ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "AirLoopHVAC.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Coil_Heating_DX_VariableSpeed_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingDXVariableSpeed_Impl::CoilHeatingDXVariableSpeed_Impl(const IdfObject& idfObject,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingDXVariableSpeed::iddObjectType());
  }

  CoilHeatingDXVariableSpeed_Impl::CoilHeatingDXVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingDXVariableSpeed::iddObjectType());
  }

  CoilHeatingDXVariableSpeed_Impl::CoilHeatingDXVariableSpeed_Impl(const CoilHeatingDXVariableSpeed_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingDXVariableSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingDXVariableSpeed_Impl::iddObjectType() const {
    return CoilHeatingDXVariableSpeed::iddObjectType();
  }

  unsigned CoilHeatingDXVariableSpeed_Impl::inletPort() {
    return OS_Coil_Heating_DX_VariableSpeedFields::IndoorAirInletNodeName;
  }

  unsigned CoilHeatingDXVariableSpeed_Impl::outletPort() {
    return OS_Coil_Heating_DX_VariableSpeedFields::IndoorAirOutletNodeName;
  }

  int CoilHeatingDXVariableSpeed_Impl::nominalSpeedLevel() const {
    boost::optional<int> value = getInt(OS_Coil_Heating_DX_VariableSpeedFields::NominalSpeedLevel,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilHeatingDXVariableSpeed_Impl::ratedHeatingCapacityAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Heating_DX_VariableSpeedFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel,true);
  }

  bool CoilHeatingDXVariableSpeed_Impl::isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_VariableSpeedFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilHeatingDXVariableSpeed_Impl::ratedAirFlowRateAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Heating_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel,true);
  }

  bool CoilHeatingDXVariableSpeed_Impl::isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  Curve CoilHeatingDXVariableSpeed_Impl::energyPartLoadFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyPartLoadFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Part Load Fraction Curve attached.");
    }
    return value.get();
  }

  boost::optional<Curve> CoilHeatingDXVariableSpeed_Impl::defrostEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_DX_VariableSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName);
  }

  double CoilHeatingDXVariableSpeed_Impl::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_VariableSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilHeatingDXVariableSpeed_Impl::outdoorDryBulbTemperaturetoTurnOnCompressor() const {
    return getDouble(OS_Coil_Heating_DX_VariableSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor,true);
  }

  double CoilHeatingDXVariableSpeed_Impl::maximumOutdoorDryBulbTemperatureforDefrostOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingDXVariableSpeed_Impl::crankcaseHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_VariableSpeedFields::CrankcaseHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingDXVariableSpeed_Impl::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string CoilHeatingDXVariableSpeed_Impl::defrostStrategy() const {
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_VariableSpeedFields::DefrostStrategy,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string CoilHeatingDXVariableSpeed_Impl::defrostControl() const {
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_VariableSpeedFields::DefrostControl,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingDXVariableSpeed_Impl::defrostTimePeriodFraction() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_VariableSpeedFields::DefrostTimePeriodFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilHeatingDXVariableSpeed_Impl::resistiveDefrostHeaterCapacity() const {
    return getDouble(OS_Coil_Heating_DX_VariableSpeedFields::ResistiveDefrostHeaterCapacity,true);
  }

  bool CoilHeatingDXVariableSpeed_Impl::isResistiveDefrostHeaterCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_VariableSpeedFields::ResistiveDefrostHeaterCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  void CoilHeatingDXVariableSpeed_Impl::setNominalSpeedLevel(int nominalSpeedLevel) {
    bool result = setInt(OS_Coil_Heating_DX_VariableSpeedFields::NominalSpeedLevel, nominalSpeedLevel);
    OS_ASSERT(result);
  }

  void CoilHeatingDXVariableSpeed_Impl::setRatedHeatingCapacityAtSelectedNominalSpeedLevel(boost::optional<double> ratedHeatingCapacityAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (ratedHeatingCapacityAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel, ratedHeatingCapacityAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilHeatingDXVariableSpeed_Impl::autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Heating_DX_VariableSpeedFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  void CoilHeatingDXVariableSpeed_Impl::setRatedAirFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (ratedAirFlowRateAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, ratedAirFlowRateAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilHeatingDXVariableSpeed_Impl::autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Heating_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXVariableSpeed_Impl::setEnergyPartLoadFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_DX_VariableSpeedFields::EnergyPartLoadFractionCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingDXVariableSpeed_Impl::setDefrostEnergyInputRatioFunctionofTemperatureCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_Coil_Heating_DX_VariableSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName, curve.get().handle());
    }
    else {
      resetDefrostEnergyInputRatioFunctionofTemperatureCurve();
      result = true;
    }
    return result;
  }

  void CoilHeatingDXVariableSpeed_Impl::resetDefrostEnergyInputRatioFunctionofTemperatureCurve() {
    bool result = setString(OS_Coil_Heating_DX_VariableSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXVariableSpeed_Impl::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
    bool result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, minimumOutdoorDryBulbTemperatureforCompressorOperation);
    return result;
  }

  void CoilHeatingDXVariableSpeed_Impl::setOutdoorDryBulbTemperaturetoTurnOnCompressor(boost::optional<double> outdoorDryBulbTemperaturetoTurnOnCompressor) {
    bool result(false);
    if (outdoorDryBulbTemperaturetoTurnOnCompressor) {
      result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor, outdoorDryBulbTemperaturetoTurnOnCompressor.get());
    }
    else {
      resetOutdoorDryBulbTemperaturetoTurnOnCompressor();
      result = true;
    }
    OS_ASSERT(result);
  }

  void CoilHeatingDXVariableSpeed_Impl::resetOutdoorDryBulbTemperaturetoTurnOnCompressor() {
    bool result = setString(OS_Coil_Heating_DX_VariableSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXVariableSpeed_Impl::setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation) {
    bool result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation, maximumOutdoorDryBulbTemperatureforDefrostOperation);
    return result;
  }

  bool CoilHeatingDXVariableSpeed_Impl::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
    bool result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::CrankcaseHeaterCapacity, crankcaseHeaterCapacity);
    return result;
  }

  bool CoilHeatingDXVariableSpeed_Impl::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
    bool result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
    return result;
  }

  bool CoilHeatingDXVariableSpeed_Impl::setDefrostStrategy(std::string defrostStrategy) {
    bool result = setString(OS_Coil_Heating_DX_VariableSpeedFields::DefrostStrategy, defrostStrategy);
    return result;
  }

  bool CoilHeatingDXVariableSpeed_Impl::setDefrostControl(std::string defrostControl) {
    bool result = setString(OS_Coil_Heating_DX_VariableSpeedFields::DefrostControl, defrostControl);
    return result;
  }

  bool CoilHeatingDXVariableSpeed_Impl::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
    bool result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::DefrostTimePeriodFraction, defrostTimePeriodFraction);
    return result;
  }

  bool CoilHeatingDXVariableSpeed_Impl::setResistiveDefrostHeaterCapacity(boost::optional<double> resistiveDefrostHeaterCapacity) {
    bool result(false);
    if (resistiveDefrostHeaterCapacity) {
      result = setDouble(OS_Coil_Heating_DX_VariableSpeedFields::ResistiveDefrostHeaterCapacity, resistiveDefrostHeaterCapacity.get());
    }
    return result;
  }

  void CoilHeatingDXVariableSpeed_Impl::autosizeResistiveDefrostHeaterCapacity() {
    bool result = setString(OS_Coil_Heating_DX_VariableSpeedFields::ResistiveDefrostHeaterCapacity, "autosize");
    OS_ASSERT(result);
  }

  boost::optional<Curve> CoilHeatingDXVariableSpeed_Impl::optionalEnergyPartLoadFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_DX_VariableSpeedFields::EnergyPartLoadFractionCurveName);
  }

  ModelObject CoilHeatingDXVariableSpeed_Impl::clone(Model model) const {
    auto t_clone = StraightComponent_Impl::clone(model).cast<CoilHeatingDXVariableSpeed>();

    if (auto speedDataList = this->speedDataList()) {
      auto speedDataListClone = speedDataList->clone(model).cast<ModelObjectList>();
      t_clone.getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setSpeedDataList(speedDataListClone);
    }

    return t_clone;
  }

  std::vector<ModelObject> CoilHeatingDXVariableSpeed_Impl::children() const {
    std::vector<ModelObject> children;
    if( auto const _stageDataList = speedDataList() ) {
      for( const auto & mo : _stageDataList->modelObjects() ) {
        children.push_back( mo );
      }
    }
    children.push_back( energyPartLoadFractionCurve() );
    if ( auto curve = defrostEnergyInputRatioFunctionofTemperatureCurve() ) {
      children.push_back( curve.get() );
    }
    return children;
  }

  boost::optional<ModelObjectList> CoilHeatingDXVariableSpeed_Impl::speedDataList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Coil_Heating_DX_VariableSpeedFields::SpeedDataList);
  }

  bool CoilHeatingDXVariableSpeed_Impl::addSpeed( const CoilHeatingDXVariableSpeedSpeedData & speed) {
    auto modelObjectList = speedDataList();
    if( modelObjectList ) {
      modelObjectList->addModelObject(speed);
    }
    return false;
  }

  void CoilHeatingDXVariableSpeed_Impl::removeSpeed( const CoilHeatingDXVariableSpeedSpeedData & speed) {
    auto modelObjectList = speedDataList();
    if( modelObjectList ) {
      modelObjectList->removeModelObject(speed);
    }
  }

  void CoilHeatingDXVariableSpeed_Impl::removeAllSpeeds() {
    auto modelObjectList = speedDataList();
    if( modelObjectList ) {
      auto const modelObjects = modelObjectList->modelObjects();

      for(const auto & elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CoilHeatingDXVariableSpeedSpeedData>();
          if (modelObject) {
            modelObjectList->removeModelObject(elem);
          }
      }
    }
  }

  std::vector<CoilHeatingDXVariableSpeedSpeedData> CoilHeatingDXVariableSpeed_Impl::speeds() const {
    std::vector<CoilHeatingDXVariableSpeedSpeedData> result;
    auto const modelObjectList = speedDataList();
    if( modelObjectList ) {
      auto const modelObjects = modelObjectList->modelObjects();

      for(const auto & elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CoilHeatingDXVariableSpeedSpeedData>();
          if (modelObject) {
            result.push_back(modelObject.get());
          }
      }
    }
    return result;
  }

  boost::optional<HVACComponent> CoilHeatingDXVariableSpeed_Impl::containingHVACComponent() const
  {
    // AirLoopHVACUnitarySystem
    {
      auto systems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

      for( auto const & system : systems ) {
        if( auto heatingCoil = system.heatingCoil() ) {
          if( heatingCoil->handle() == this->handle() ) {
            return system;
          }
        }
      }
    }

    // AirLoopHVACUnitaryHeatPumpAirToAir
    {
      auto systems = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

      for( auto const & system : systems ) {
        auto heatingCoil = system.heatingCoil();
        if( heatingCoil.handle() == this->handle() ) {
          return system;
        }
      }
    }

    return boost::none;
  }

  boost::optional<ZoneHVACComponent> CoilHeatingDXVariableSpeed_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACPackagedTerminalHeatPump
    {
      auto systems = this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalHeatPump>();

      for( auto const & system : systems ) {
        auto heatingCoil = system.heatingCoil();
        if( heatingCoil.handle() == this->handle() ) {
          return system;
        }
      }
    }

    return boost::none;
  }

  bool CoilHeatingDXVariableSpeed_Impl::addToNode(Node & node)
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

  bool CoilHeatingDXVariableSpeed_Impl::setSpeedDataList(const boost::optional<ModelObjectList>& modelObjectList) {
    bool result(false);
    if (modelObjectList) {
      result = setPointer(OS_Coil_Heating_DX_VariableSpeedFields::SpeedDataList, modelObjectList.get().handle());
    }
    else {
      resetSpeedDataList();
      result = true;
    }
    return result;
  }

  void CoilHeatingDXVariableSpeed_Impl::resetSpeedDataList() {
    bool result = setString(OS_Coil_Heating_DX_VariableSpeedFields::SpeedDataList, "");
    OS_ASSERT(result);
  }

  std::vector<IdfObject> CoilHeatingDXVariableSpeed_Impl::remove()
  {
    auto _stageDataList = speedDataList();
    auto result = StraightComponent_Impl::remove();
    if( (! result.empty()) && _stageDataList ) {
      _stageDataList->remove();
    }

    return result;
  }

} // detail

CoilHeatingDXVariableSpeed::CoilHeatingDXVariableSpeed(const Model& model)
  : StraightComponent(CoilHeatingDXVariableSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingDXVariableSpeed_Impl>());

  auto partLoadFraction = CurveQuadratic(model);
  partLoadFraction.setCoefficient1Constant(0.85);
  partLoadFraction.setCoefficient2x(0.15);
  partLoadFraction.setCoefficient3xPOW2(0.0);
  partLoadFraction.setMinimumValueofx(0.0);
  partLoadFraction.setMaximumValueofx(1.0);

  bool ok = true;
  setNominalSpeedLevel(1);
  autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel();
  autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
  ok = setEnergyPartLoadFractionCurve(partLoadFraction);
  OS_ASSERT(ok);
  ok = setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-5.0);
  OS_ASSERT(ok);
  ok = setMaximumOutdoorDryBulbTemperatureforDefrostOperation(5.0);
  OS_ASSERT(ok);
  ok = setCrankcaseHeaterCapacity(200.0);
  OS_ASSERT(ok);
  ok = setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(10.0);
  OS_ASSERT(ok);
  ok = setDefrostStrategy("Resistive");
  OS_ASSERT(ok);
  ok = setDefrostControl("OnDemand");
  OS_ASSERT(ok);
  ok = setDefrostTimePeriodFraction(0.166667);
  OS_ASSERT(ok);
  autosizeResistiveDefrostHeaterCapacity();

  auto speedDataList = ModelObjectList(model);
  speedDataList.setName(this->name().get() + " Speed Data List");
  ok = getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setSpeedDataList(speedDataList);
  OS_ASSERT(ok);

}

CoilHeatingDXVariableSpeed::CoilHeatingDXVariableSpeed(const Model& model,
                                                       const Curve& partLoadFraction)
  : StraightComponent(CoilHeatingDXVariableSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingDXVariableSpeed_Impl>());

  bool ok = true;
  setNominalSpeedLevel(1);
  autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel();
  autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
  ok = setEnergyPartLoadFractionCurve(partLoadFraction);
  OS_ASSERT(ok);
  ok = setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-5.0);
  OS_ASSERT(ok);
  ok = setMaximumOutdoorDryBulbTemperatureforDefrostOperation(5.0);
  OS_ASSERT(ok);
  ok = setCrankcaseHeaterCapacity(200.0);
  OS_ASSERT(ok);
  ok = setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(10.0);
  OS_ASSERT(ok);
  ok = setDefrostStrategy("Resistive");
  OS_ASSERT(ok);
  ok = setDefrostControl("OnDemand");
  OS_ASSERT(ok);
  ok = setDefrostTimePeriodFraction(0.166667);
  OS_ASSERT(ok);
  autosizeResistiveDefrostHeaterCapacity();

  auto speedDataList = ModelObjectList(model);
  speedDataList.setName(this->name().get() + " Speed Data List");
  ok = getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setSpeedDataList(speedDataList);
  OS_ASSERT(ok);

}

IddObjectType CoilHeatingDXVariableSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_DX_VariableSpeed);
}

std::vector<std::string> CoilHeatingDXVariableSpeed::defrostStrategyValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_Heating_DX_VariableSpeedFields::DefrostStrategy);
}

std::vector<std::string> CoilHeatingDXVariableSpeed::defrostControlValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_Heating_DX_VariableSpeedFields::DefrostControl);
}

int CoilHeatingDXVariableSpeed::nominalSpeedLevel() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->nominalSpeedLevel();
}

boost::optional<double> CoilHeatingDXVariableSpeed::ratedHeatingCapacityAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->ratedHeatingCapacityAtSelectedNominalSpeedLevel();
}

bool CoilHeatingDXVariableSpeed::isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized();
}

boost::optional<double> CoilHeatingDXVariableSpeed::ratedAirFlowRateAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->ratedAirFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilHeatingDXVariableSpeed::isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized();
}

Curve CoilHeatingDXVariableSpeed::energyPartLoadFractionCurve() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->energyPartLoadFractionCurve();
}

boost::optional<Curve> CoilHeatingDXVariableSpeed::defrostEnergyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->defrostEnergyInputRatioFunctionofTemperatureCurve();
}

double CoilHeatingDXVariableSpeed::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->minimumOutdoorDryBulbTemperatureforCompressorOperation();
}

boost::optional<double> CoilHeatingDXVariableSpeed::outdoorDryBulbTemperaturetoTurnOnCompressor() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->outdoorDryBulbTemperaturetoTurnOnCompressor();
}

double CoilHeatingDXVariableSpeed::maximumOutdoorDryBulbTemperatureforDefrostOperation() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->maximumOutdoorDryBulbTemperatureforDefrostOperation();
}

double CoilHeatingDXVariableSpeed::crankcaseHeaterCapacity() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->crankcaseHeaterCapacity();
}

double CoilHeatingDXVariableSpeed::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();
}

std::string CoilHeatingDXVariableSpeed::defrostStrategy() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->defrostStrategy();
}

std::string CoilHeatingDXVariableSpeed::defrostControl() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->defrostControl();
}

double CoilHeatingDXVariableSpeed::defrostTimePeriodFraction() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->defrostTimePeriodFraction();
}

boost::optional<double> CoilHeatingDXVariableSpeed::resistiveDefrostHeaterCapacity() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->resistiveDefrostHeaterCapacity();
}

bool CoilHeatingDXVariableSpeed::isResistiveDefrostHeaterCapacityAutosized() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->isResistiveDefrostHeaterCapacityAutosized();
}

void CoilHeatingDXVariableSpeed::setNominalSpeedLevel(int nominalSpeedLevel) {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setNominalSpeedLevel(nominalSpeedLevel);
}

void CoilHeatingDXVariableSpeed::setRatedHeatingCapacityAtSelectedNominalSpeedLevel(double ratedHeatingCapacityAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setRatedHeatingCapacityAtSelectedNominalSpeedLevel(ratedHeatingCapacityAtSelectedNominalSpeedLevel);
}

void CoilHeatingDXVariableSpeed::autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel();
}

void CoilHeatingDXVariableSpeed::setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setRatedAirFlowRateAtSelectedNominalSpeedLevel(ratedAirFlowRateAtSelectedNominalSpeedLevel);
}

void CoilHeatingDXVariableSpeed::autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilHeatingDXVariableSpeed::setEnergyPartLoadFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setEnergyPartLoadFractionCurve(curve);
}

bool CoilHeatingDXVariableSpeed::setDefrostEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setDefrostEnergyInputRatioFunctionofTemperatureCurve(curve);
}

void CoilHeatingDXVariableSpeed::resetDefrostEnergyInputRatioFunctionofTemperatureCurve() {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->resetDefrostEnergyInputRatioFunctionofTemperatureCurve();
}

bool CoilHeatingDXVariableSpeed::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setMinimumOutdoorDryBulbTemperatureforCompressorOperation(minimumOutdoorDryBulbTemperatureforCompressorOperation);
}

void CoilHeatingDXVariableSpeed::setOutdoorDryBulbTemperaturetoTurnOnCompressor(double outdoorDryBulbTemperaturetoTurnOnCompressor) {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setOutdoorDryBulbTemperaturetoTurnOnCompressor(outdoorDryBulbTemperaturetoTurnOnCompressor);
}

void CoilHeatingDXVariableSpeed::resetOutdoorDryBulbTemperaturetoTurnOnCompressor() {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->resetOutdoorDryBulbTemperaturetoTurnOnCompressor();
}

bool CoilHeatingDXVariableSpeed::setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureforDefrostOperation(maximumOutdoorDryBulbTemperatureforDefrostOperation);
}

bool CoilHeatingDXVariableSpeed::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setCrankcaseHeaterCapacity(crankcaseHeaterCapacity);
}

bool CoilHeatingDXVariableSpeed::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
}

bool CoilHeatingDXVariableSpeed::setDefrostStrategy(std::string defrostStrategy) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setDefrostStrategy(defrostStrategy);
}

bool CoilHeatingDXVariableSpeed::setDefrostControl(std::string defrostControl) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setDefrostControl(defrostControl);
}

bool CoilHeatingDXVariableSpeed::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setDefrostTimePeriodFraction(defrostTimePeriodFraction);
}

bool CoilHeatingDXVariableSpeed::setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->setResistiveDefrostHeaterCapacity(resistiveDefrostHeaterCapacity);
}

void CoilHeatingDXVariableSpeed::autosizeResistiveDefrostHeaterCapacity() {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->autosizeResistiveDefrostHeaterCapacity();
}

std::vector<CoilHeatingDXVariableSpeedSpeedData> CoilHeatingDXVariableSpeed::speeds() const {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->speeds();
}

bool CoilHeatingDXVariableSpeed::addSpeed(const CoilHeatingDXVariableSpeedSpeedData& speed) {
  return getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->addSpeed(speed);
}

void CoilHeatingDXVariableSpeed::removeSpeed(const CoilHeatingDXVariableSpeedSpeedData& speed) {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->removeSpeed(speed);
}

void CoilHeatingDXVariableSpeed::removeAllSpeeds() {
  getImpl<detail::CoilHeatingDXVariableSpeed_Impl>()->removeAllSpeeds();
}

/// @cond
CoilHeatingDXVariableSpeed::CoilHeatingDXVariableSpeed(std::shared_ptr<detail::CoilHeatingDXVariableSpeed_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

