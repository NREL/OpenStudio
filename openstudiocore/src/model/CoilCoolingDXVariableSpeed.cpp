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

#include "CoilCoolingDXVariableSpeed.hpp"
#include "CoilCoolingDXVariableSpeed_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CoilCoolingDXVariableSpeedSpeedData.hpp"
#include "CoilCoolingDXVariableSpeedSpeedData_Impl.hpp"
// #include "WaterStorageTank.hpp"
// #include "WaterStorageTank_Impl.hpp"
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
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Coil_Cooling_DX_VariableSpeed_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingDXVariableSpeed_Impl::CoilCoolingDXVariableSpeed_Impl(const IdfObject& idfObject,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDXVariableSpeed::iddObjectType());
  }

  CoilCoolingDXVariableSpeed_Impl::CoilCoolingDXVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingDXVariableSpeed::iddObjectType());
  }

  CoilCoolingDXVariableSpeed_Impl::CoilCoolingDXVariableSpeed_Impl(const CoilCoolingDXVariableSpeed_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilCoolingDXVariableSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilCoolingDXVariableSpeed_Impl::iddObjectType() const {
    return CoilCoolingDXVariableSpeed::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilCoolingDXVariableSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Cooling_DX_VariableSpeedFields::BasinHeaterOperatingScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilCoolingDXVariableSpeed","Basin Heater Operating"));
    }
    return result;
  }

  unsigned CoilCoolingDXVariableSpeed_Impl::inletPort() {
    return OS_Coil_Cooling_DX_VariableSpeedFields::IndoorAirInletNodeName;
  }

  unsigned CoilCoolingDXVariableSpeed_Impl::outletPort() {
    return OS_Coil_Cooling_DX_VariableSpeedFields::IndoorAirOutletNodeName;
  }

  int CoilCoolingDXVariableSpeed_Impl::nominalSpeedLevel() const {
    boost::optional<int> value = getInt(OS_Coil_Cooling_DX_VariableSpeedFields::NominalSpeedLevel,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilCoolingDXVariableSpeed_Impl::grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel,true);
  }

  bool CoilCoolingDXVariableSpeed_Impl::isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_VariableSpeedFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilCoolingDXVariableSpeed_Impl::ratedAirFlowRateAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel,true);
  }

  bool CoilCoolingDXVariableSpeed_Impl::isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilCoolingDXVariableSpeed_Impl::nominalTimeforCondensatetoBeginLeavingtheCoil() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::NominalTimeforCondensatetoBeginLeavingtheCoil,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXVariableSpeed_Impl::initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::InitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve CoilCoolingDXVariableSpeed_Impl::energyPartLoadFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyPartLoadFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Part Load Fraction Curve attached.");
    }
    return value.get();
  }

  // boost::optional<std::string> CoilCoolingDXVariableSpeed_Impl::condenserAirInletNodeName() const {
  //   return getString(OS_Coil_Cooling_DX_VariableSpeedFields::CondenserAirInletNodeName,true);
  // }

  std::string CoilCoolingDXVariableSpeed_Impl::condenserType() const {
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_VariableSpeedFields::CondenserType,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilCoolingDXVariableSpeed_Impl::evaporativeCondenserPumpRatedPowerConsumption() const {
    return getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption,true);
  }

  bool CoilCoolingDXVariableSpeed_Impl::isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_VariableSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilCoolingDXVariableSpeed_Impl::crankcaseHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::CrankcaseHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXVariableSpeed_Impl::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  // boost::optional<WaterStorageTank> CoilCoolingDXVariableSpeed_Impl::supplyWaterStorageTank() const {
  //   return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(OS_Coil_Cooling_DX_VariableSpeedFields::SupplyWaterStorageTankName);
  // }

  // boost::optional<WaterStorageTank> CoilCoolingDXVariableSpeed_Impl::condensateCollectionWaterStorageTank() const {
  //   return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(OS_Coil_Cooling_DX_VariableSpeedFields::CondensateCollectionWaterStorageTankName);
  // }

  double CoilCoolingDXVariableSpeed_Impl::basinHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::BasinHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXVariableSpeed_Impl::basinHeaterSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeedFields::BasinHeaterSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> CoilCoolingDXVariableSpeed_Impl::basinHeaterOperatingSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DX_VariableSpeedFields::BasinHeaterOperatingScheduleName);
  }

  void CoilCoolingDXVariableSpeed_Impl::setNominalSpeedLevel(int nominalSpeedLevel) {
    bool result = setInt(OS_Coil_Cooling_DX_VariableSpeedFields::NominalSpeedLevel, nominalSpeedLevel);
    OS_ASSERT(result);
  }

  void CoilCoolingDXVariableSpeed_Impl::setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel, grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilCoolingDXVariableSpeed_Impl::autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  void CoilCoolingDXVariableSpeed_Impl::setRatedAirFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (ratedAirFlowRateAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, ratedAirFlowRateAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilCoolingDXVariableSpeed_Impl::autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXVariableSpeed_Impl::setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil) {
    bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::NominalTimeforCondensatetoBeginLeavingtheCoil, nominalTimeforCondensatetoBeginLeavingtheCoil);
    return result;
  }

  bool CoilCoolingDXVariableSpeed_Impl::setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity) {
    bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::InitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity, initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);
    return result;
  }

  bool CoilCoolingDXVariableSpeed_Impl::setEnergyPartLoadFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_DX_VariableSpeedFields::EnergyPartLoadFractionCurveName, curve.handle());
    return result;
  }

  // void CoilCoolingDXVariableSpeed_Impl::setCondenserAirInletNodeName(boost::optional<std::string> condenserAirInletNodeName) {
  //   bool result(false);
  //   if (condenserAirInletNodeName) {
  //     result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::CondenserAirInletNodeName, condenserAirInletNodeName.get());
  //   }
  //   else {
  //     resetCondenserAirInletNodeName();
  //     result = true;
  //   }
  //   OS_ASSERT(result);
  // }

  // void CoilCoolingDXVariableSpeed_Impl::resetCondenserAirInletNodeName() {
  //   bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::CondenserAirInletNodeName, "");
  //   OS_ASSERT(result);
  // }

  bool CoilCoolingDXVariableSpeed_Impl::setCondenserType(std::string condenserType) {
    bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::CondenserType, condenserType);
    return result;
  }

  bool CoilCoolingDXVariableSpeed_Impl::setEvaporativeCondenserPumpRatedPowerConsumption(boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption) {
    bool result(false);
    if (evaporativeCondenserPumpRatedPowerConsumption) {
      result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption, evaporativeCondenserPumpRatedPowerConsumption.get());
    }
    return result;
  }

  void CoilCoolingDXVariableSpeed_Impl::autosizeEvaporativeCondenserPumpRatedPowerConsumption() {
    bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption, "autosize");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXVariableSpeed_Impl::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
    bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::CrankcaseHeaterCapacity, crankcaseHeaterCapacity);
    return result;
  }

  bool CoilCoolingDXVariableSpeed_Impl::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
    bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
    return result;
  }

  // bool CoilCoolingDXVariableSpeed_Impl::setSupplyWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank) {
  //   bool result(false);
  //   if (waterStorageTank) {
  //     result = setPointer(OS_Coil_Cooling_DX_VariableSpeedFields::SupplyWaterStorageTankName, waterStorageTank.get().handle());
  //   }
  //   else {
  //     resetSupplyWaterStorageTank();
  //     result = true;
  //   }
  //   return result;
  // }

  // void CoilCoolingDXVariableSpeed_Impl::resetSupplyWaterStorageTank() {
  //   bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::SupplyWaterStorageTankName, "");
  //   OS_ASSERT(result);
  // }

  // bool CoilCoolingDXVariableSpeed_Impl::setCondensateCollectionWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank) {
  //   bool result(false);
  //   if (waterStorageTank) {
  //     result = setPointer(OS_Coil_Cooling_DX_VariableSpeedFields::CondensateCollectionWaterStorageTankName, waterStorageTank.get().handle());
  //   }
  //   else {
  //     resetCondensateCollectionWaterStorageTank();
  //     result = true;
  //   }
  //   return result;
  // }

  // void CoilCoolingDXVariableSpeed_Impl::resetCondensateCollectionWaterStorageTank() {
  //   bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::CondensateCollectionWaterStorageTankName, "");
  //   OS_ASSERT(result);
  // }

  bool CoilCoolingDXVariableSpeed_Impl::setBasinHeaterCapacity(double basinHeaterCapacity) {
    bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::BasinHeaterCapacity, basinHeaterCapacity);
    return result;
  }

  bool CoilCoolingDXVariableSpeed_Impl::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
    bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeedFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature);
    return result;
  }

  bool CoilCoolingDXVariableSpeed_Impl::setBasinHeaterOperatingSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Cooling_DX_VariableSpeedFields::BasinHeaterOperatingScheduleName,
                              "CoilCoolingDXVariableSpeed",
                              "Basin Heater Operating",
                              schedule);
    return result;
  }

  void CoilCoolingDXVariableSpeed_Impl::resetBasinHeaterOperatingSchedule() {
    bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::BasinHeaterOperatingScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<Curve> CoilCoolingDXVariableSpeed_Impl::optionalEnergyPartLoadFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_VariableSpeedFields::EnergyPartLoadFractionCurveName);
  }

  ModelObject CoilCoolingDXVariableSpeed_Impl::clone(Model model) const {
    auto t_clone = StraightComponent_Impl::clone(model).cast<CoilCoolingDXVariableSpeed>();

    if (auto speedDataList = this->speedDataList()) {
      auto speedDataListClone = speedDataList->clone(model).cast<ModelObjectList>();
      t_clone.getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setSpeedDataList(speedDataListClone);
    }

    return t_clone;
  }

  std::vector<ModelObject> CoilCoolingDXVariableSpeed_Impl::children() const {
    std::vector<ModelObject> children;
    children.push_back( energyPartLoadFractionCurve() );
    if( auto const _stageDataList = speedDataList() ) {
      for( const auto & mo : _stageDataList->modelObjects() ) {
        children.push_back( mo );
      }
    }
    return children;
  }

  boost::optional<ModelObjectList> CoilCoolingDXVariableSpeed_Impl::speedDataList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Coil_Cooling_DX_VariableSpeedFields::SpeedDataList);
  }

  bool CoilCoolingDXVariableSpeed_Impl::addSpeed( const CoilCoolingDXVariableSpeedSpeedData & speed) {
    auto modelObjectList = speedDataList();
    if( modelObjectList ) {
      modelObjectList->addModelObject(speed);
    }
    return false;
  }

  void CoilCoolingDXVariableSpeed_Impl::removeSpeed( const CoilCoolingDXVariableSpeedSpeedData & speed) {
    auto modelObjectList = speedDataList();
    if( modelObjectList ) {
      modelObjectList->removeModelObject(speed);
    }
  }

  void CoilCoolingDXVariableSpeed_Impl::removeAllSpeeds() {
    auto modelObjectList = speedDataList();
    if( modelObjectList ) {
      auto const modelObjects = modelObjectList->modelObjects();

      for(const auto & elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CoilCoolingDXVariableSpeedSpeedData>();
          if (modelObject) {
            modelObjectList->removeModelObject(elem);
          }
      }
    }
  }

  std::vector<CoilCoolingDXVariableSpeedSpeedData> CoilCoolingDXVariableSpeed_Impl::speeds() const {
    std::vector<CoilCoolingDXVariableSpeedSpeedData> result;
    auto const modelObjectList = speedDataList();
    if( modelObjectList ) {
      auto const modelObjects = modelObjectList->modelObjects();

      for(const auto & elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CoilCoolingDXVariableSpeedSpeedData>();
          if (modelObject) {
            result.push_back(modelObject.get());
          }
      }
    }
    return result;
  }

  boost::optional<HVACComponent> CoilCoolingDXVariableSpeed_Impl::containingHVACComponent() const
  {
    // AirLoopHVACUnitarySystem
    {
      auto systems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

      for( auto const & system : systems ) {
        if( auto coolingCoil = system.coolingCoil() ) {
          if( coolingCoil->handle() == this->handle() ) {
            return system;
          }
        }
      }
    }

    // AirLoopHVACUnitaryHeatPumpAirToAir
    {
      auto systems = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

      for( auto const & system : systems ) {
        auto coolingCoil = system.coolingCoil();
        if( coolingCoil.handle() == this->handle() ) {
          return system;
        }
      }
    }

    return boost::none;
  }

  boost::optional<ZoneHVACComponent> CoilCoolingDXVariableSpeed_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACPackagedTerminalAirConditioner
    {
      auto systems = this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalAirConditioner>();

      for( auto const & system : systems ) {
        auto coolingCoil = system.coolingCoil();
        if( coolingCoil.handle() == this->handle() ) {
          return system;
        }
      }
    }

    // ZoneHVACPackagedTerminalHeatPump
    {
      auto systems = this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalHeatPump>();

      for( auto const & system : systems ) {
        auto coolingCoil = system.coolingCoil();
        if( coolingCoil.handle() == this->handle() ) {
          return system;
        }
      }
    }

    return boost::none;
  }

  bool CoilCoolingDXVariableSpeed_Impl::addToNode(Node & node)
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

  bool CoilCoolingDXVariableSpeed_Impl::setSpeedDataList(const boost::optional<ModelObjectList>& modelObjectList) {
    bool result(false);
    if (modelObjectList) {
      result = setPointer(OS_Coil_Cooling_DX_VariableSpeedFields::SpeedDataList, modelObjectList.get().handle());
    }
    else {
      resetSpeedDataList();
      result = true;
    }
    return result;
  }

  void CoilCoolingDXVariableSpeed_Impl::resetSpeedDataList() {
    bool result = setString(OS_Coil_Cooling_DX_VariableSpeedFields::SpeedDataList, "");
    OS_ASSERT(result);
  }

  std::vector<IdfObject> CoilCoolingDXVariableSpeed_Impl::remove()
  {
    auto _stageDataList = speedDataList();
    auto result = StraightComponent_Impl::remove();
    if( (! result.empty()) && _stageDataList ) {
      _stageDataList->remove();
    }

    return result;
  }

} // detail

CoilCoolingDXVariableSpeed::CoilCoolingDXVariableSpeed(const Model& model)
  : StraightComponent(CoilCoolingDXVariableSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingDXVariableSpeed_Impl>());

  bool ok = true;
  setNominalSpeedLevel(1);
  autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
  autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
  ok = setNominalTimeforCondensatetoBeginLeavingtheCoil(0);
  OS_ASSERT(ok);
  ok = setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(0);
  OS_ASSERT(ok);

  auto partLoadFraction = CurveQuadratic(model);
  partLoadFraction.setCoefficient1Constant(0.85);
  partLoadFraction.setCoefficient2x(0.15);
  partLoadFraction.setCoefficient3xPOW2(0.0);
  partLoadFraction.setMinimumValueofx(0.0);
  partLoadFraction.setMaximumValueofx(1.0);

  ok = setEnergyPartLoadFractionCurve(partLoadFraction);
  OS_ASSERT(ok);
  ok = setCondenserType("AirCooled");
  OS_ASSERT(ok);
  ok = setEvaporativeCondenserPumpRatedPowerConsumption(0.0);
  OS_ASSERT(ok);
  ok = setCrankcaseHeaterCapacity(0.0);
  OS_ASSERT(ok);
  ok = setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(10.0);
  OS_ASSERT(ok);
  // ok = setSupplyWaterStorageTank(WaterStorageTank);
  // OS_ASSERT(ok);
  // ok = setCondensateCollectionWaterStorageTank(WaterStorageTank);
  // OS_ASSERT(ok);
  ok = setBasinHeaterCapacity(0.0);
  OS_ASSERT(ok);
  ok = setBasinHeaterSetpointTemperature(2.0);
  OS_ASSERT(ok);

  auto speedDataList = ModelObjectList(model);
  speedDataList.setName(this->name().get() + " Speed Data List");
  ok = getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setSpeedDataList(speedDataList);
  OS_ASSERT(ok);

}

CoilCoolingDXVariableSpeed::CoilCoolingDXVariableSpeed(const Model& model,
                                                       const Curve& partLoadFraction)
  : StraightComponent(CoilCoolingDXVariableSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingDXVariableSpeed_Impl>());

  bool ok = true;
  setNominalSpeedLevel(1);
  autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
  autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
  ok = setNominalTimeforCondensatetoBeginLeavingtheCoil(0);
  OS_ASSERT(ok);
  ok = setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(0);
  OS_ASSERT(ok);
  ok = setEnergyPartLoadFractionCurve(partLoadFraction);
  OS_ASSERT(ok);
  ok = setCondenserType("AirCooled");
  OS_ASSERT(ok);
  ok = setEvaporativeCondenserPumpRatedPowerConsumption(0.0);
  OS_ASSERT(ok);
  ok = setCrankcaseHeaterCapacity(0.0);
  OS_ASSERT(ok);
  ok = setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(10.0);
  OS_ASSERT(ok);
  // ok = setSupplyWaterStorageTank(WaterStorageTank);
  // OS_ASSERT(ok);
  // ok = setCondensateCollectionWaterStorageTank(WaterStorageTank);
  // OS_ASSERT(ok);
  ok = setBasinHeaterCapacity(0.0);
  OS_ASSERT(ok);
  ok = setBasinHeaterSetpointTemperature(2.0);
  OS_ASSERT(ok);

  auto speedDataList = ModelObjectList(model);
  speedDataList.setName(this->name().get() + " Speed Data List");
  ok = getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setSpeedDataList(speedDataList);
  OS_ASSERT(ok);

}

IddObjectType CoilCoolingDXVariableSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Cooling_DX_VariableSpeed);
}

std::vector<std::string> CoilCoolingDXVariableSpeed::condenserTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_Cooling_DX_VariableSpeedFields::CondenserType);
}

int CoilCoolingDXVariableSpeed::nominalSpeedLevel() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->nominalSpeedLevel();
}

boost::optional<double> CoilCoolingDXVariableSpeed::grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
}

bool CoilCoolingDXVariableSpeed::isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized();
}

boost::optional<double> CoilCoolingDXVariableSpeed::ratedAirFlowRateAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->ratedAirFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilCoolingDXVariableSpeed::isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized();
}

double CoilCoolingDXVariableSpeed::nominalTimeforCondensatetoBeginLeavingtheCoil() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->nominalTimeforCondensatetoBeginLeavingtheCoil();
}

double CoilCoolingDXVariableSpeed::initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity();
}

Curve CoilCoolingDXVariableSpeed::energyPartLoadFractionCurve() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->energyPartLoadFractionCurve();
}

// boost::optional<std::string> CoilCoolingDXVariableSpeed::condenserAirInletNodeName() const {
//   return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->condenserAirInletNodeName();
// }

std::string CoilCoolingDXVariableSpeed::condenserType() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->condenserType();
}

boost::optional<double> CoilCoolingDXVariableSpeed::evaporativeCondenserPumpRatedPowerConsumption() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->evaporativeCondenserPumpRatedPowerConsumption();
}

bool CoilCoolingDXVariableSpeed::isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->isEvaporativeCondenserPumpRatedPowerConsumptionAutosized();
}

double CoilCoolingDXVariableSpeed::crankcaseHeaterCapacity() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->crankcaseHeaterCapacity();
}

double CoilCoolingDXVariableSpeed::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();
}

// boost::optional<WaterStorageTank> CoilCoolingDXVariableSpeed::supplyWaterStorageTank() const {
//   return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->supplyWaterStorageTank();
// }

// boost::optional<WaterStorageTank> CoilCoolingDXVariableSpeed::condensateCollectionWaterStorageTank() const {
//   return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->condensateCollectionWaterStorageTank();
// }

double CoilCoolingDXVariableSpeed::basinHeaterCapacity() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->basinHeaterCapacity();
}

double CoilCoolingDXVariableSpeed::basinHeaterSetpointTemperature() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->basinHeaterSetpointTemperature();
}

boost::optional<Schedule> CoilCoolingDXVariableSpeed::basinHeaterOperatingSchedule() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->basinHeaterOperatingSchedule();
}

void CoilCoolingDXVariableSpeed::setNominalSpeedLevel(int nominalSpeedLevel) {
  getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setNominalSpeedLevel(nominalSpeedLevel);
}

void CoilCoolingDXVariableSpeed::setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(double grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel);
}

void CoilCoolingDXVariableSpeed::autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
}

void CoilCoolingDXVariableSpeed::setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setRatedAirFlowRateAtSelectedNominalSpeedLevel(ratedAirFlowRateAtSelectedNominalSpeedLevel);
}

void CoilCoolingDXVariableSpeed::autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilCoolingDXVariableSpeed::setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil) {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setNominalTimeforCondensatetoBeginLeavingtheCoil(nominalTimeforCondensatetoBeginLeavingtheCoil);
}

bool CoilCoolingDXVariableSpeed::setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity) {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);
}

bool CoilCoolingDXVariableSpeed::setEnergyPartLoadFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setEnergyPartLoadFractionCurve(curve);
}

// void CoilCoolingDXVariableSpeed::setCondenserAirInletNodeName(std::string condenserAirInletNodeName) {
//   getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setCondenserAirInletNodeName(condenserAirInletNodeName);
// }

// void CoilCoolingDXVariableSpeed::resetCondenserAirInletNodeName() {
//   getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->resetCondenserAirInletNodeName();
// }

bool CoilCoolingDXVariableSpeed::setCondenserType(std::string condenserType) {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setCondenserType(condenserType);
}

bool CoilCoolingDXVariableSpeed::setEvaporativeCondenserPumpRatedPowerConsumption(double evaporativeCondenserPumpRatedPowerConsumption) {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setEvaporativeCondenserPumpRatedPowerConsumption(evaporativeCondenserPumpRatedPowerConsumption);
}

void CoilCoolingDXVariableSpeed::autosizeEvaporativeCondenserPumpRatedPowerConsumption() {
  getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->autosizeEvaporativeCondenserPumpRatedPowerConsumption();
}

bool CoilCoolingDXVariableSpeed::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setCrankcaseHeaterCapacity(crankcaseHeaterCapacity);
}

bool CoilCoolingDXVariableSpeed::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
}

// bool CoilCoolingDXVariableSpeed::setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank) {
//   return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setSupplyWaterStorageTank(waterStorageTank);
// }

// void CoilCoolingDXVariableSpeed::resetSupplyWaterStorageTank() {
//   getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->resetSupplyWaterStorageTank();
// }

// bool CoilCoolingDXVariableSpeed::setCondensateCollectionWaterStorageTank(const WaterStorageTank& waterStorageTank) {
//   return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setCondensateCollectionWaterStorageTank(waterStorageTank);
// }

// void CoilCoolingDXVariableSpeed::resetCondensateCollectionWaterStorageTank() {
//   getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->resetCondensateCollectionWaterStorageTank();
// }

bool CoilCoolingDXVariableSpeed::setBasinHeaterCapacity(double basinHeaterCapacity) {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
}

bool CoilCoolingDXVariableSpeed::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setBasinHeaterSetpointTemperature(basinHeaterSetpointTemperature);
}

bool CoilCoolingDXVariableSpeed::setBasinHeaterOperatingSchedule(Schedule& schedule) {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->setBasinHeaterOperatingSchedule(schedule);
}

void CoilCoolingDXVariableSpeed::resetBasinHeaterOperatingSchedule() {
  getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->resetBasinHeaterOperatingSchedule();
}

std::vector<CoilCoolingDXVariableSpeedSpeedData> CoilCoolingDXVariableSpeed::speeds() const {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->speeds();
}

bool CoilCoolingDXVariableSpeed::addSpeed(const CoilCoolingDXVariableSpeedSpeedData& speed) {
  return getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->addSpeed(speed);
}

void CoilCoolingDXVariableSpeed::removeSpeed(const CoilCoolingDXVariableSpeedSpeedData& speed) {
  getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->removeSpeed(speed);
}

void CoilCoolingDXVariableSpeed::removeAllSpeeds() {
  getImpl<detail::CoilCoolingDXVariableSpeed_Impl>()->removeAllSpeeds();
}

/// @cond
CoilCoolingDXVariableSpeed::CoilCoolingDXVariableSpeed(std::shared_ptr<detail::CoilCoolingDXVariableSpeed_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

