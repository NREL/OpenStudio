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

#include "CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include "CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "HVACComponent.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "ZoneHVACWaterToAirHeatPump.hpp"
#include "ZoneHVACWaterToAirHeatPump_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const IdfObject& idfObject,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : WaterToAirComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType());
  }

  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType());
  }

  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::iddObjectType() const {
    return CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType();
  }

  unsigned CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::airInletPort() {
    return OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirInletNodeName;
  }

  unsigned CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::airOutletPort() {
    return OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirOutletNodeName;
  }

  unsigned CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::waterInletPort() {
    return OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterInletNodeName;
  }

  unsigned CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::waterOutletPort() {
    return OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterOutletNodeName;
  }

  int CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::nominalSpeedLevel() const {
    boost::optional<int> value = getInt(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalSpeedLevel,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel,true);
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::ratedAirFlowRateAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel,true);
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::ratedWaterFlowRateAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel,true);
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::nominalTimeforCondensatetoBeginLeavingtheCoil() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalTimeforCondensatetoBeginLeavingtheCoil,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::InitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::useHotGasReheat() const {
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::UseHotGasReheat,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::energyPartLoadFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyPartLoadFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Part Load Fraction Curve attached.");
    }
    return value.get();
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setNominalSpeedLevel(int nominalSpeedLevel) {
    bool result = setInt(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalSpeedLevel, nominalSpeedLevel);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel, grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setRatedAirFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (ratedAirFlowRateAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, ratedAirFlowRateAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setRatedWaterFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedWaterFlowRateAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (ratedWaterFlowRateAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel, ratedWaterFlowRateAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil) {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalTimeforCondensatetoBeginLeavingtheCoil, nominalTimeforCondensatetoBeginLeavingtheCoil);
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity) {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::InitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity, initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);
    return result;
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setUseHotGasReheat(bool useHotGasReheat) {
    setBooleanFieldValue(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::UseHotGasReheat, useHotGasReheat);
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setEnergyPartLoadFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::EnergyPartLoadFractionCurveName, curve.handle());
    return result;
  }

  boost::optional<Curve> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::optionalEnergyPartLoadFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::EnergyPartLoadFractionCurveName);
  }

  ModelObject CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::clone(Model model) const
  {
    auto newCoil = WaterToAirComponent_Impl::clone(model).cast<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit>();

    if (auto speedDataList = this->speedDataList()) {
      auto speedDataListClone = speedDataList->clone(model).cast<ModelObjectList>();
      newCoil.getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setSpeedDataList(speedDataListClone);
    }

    return newCoil;
  }

  std::vector<ModelObject> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::children() const {
    std::vector<ModelObject> children;

    if( auto const _stageDataList = speedDataList() ) {
      for( const auto & mo : _stageDataList->modelObjects() ) {
        children.push_back( mo );
      }
    }

    children.push_back( energyPartLoadFractionCurve() );
    return children;
  }

  std::vector<openstudio::IdfObject> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::remove()
  {
    if( isRemovable() )
    {
      if( auto _stageDataList = speedDataList() ) {
        _stageDataList->remove();
      }
      return WaterToAirComponent_Impl::remove();
    }

    return std::vector<IdfObject>();
  }

  boost::optional<HVACComponent> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::containingHVACComponent() const
  {
    // AirLoopHVACUnitarySystem
    auto const airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

    for( const auto & airLoopHVACUnitarySystem : airLoopHVACUnitarySystems )
    {
      if( boost::optional<HVACComponent> coolingCoil = airLoopHVACUnitarySystem.coolingCoil() )
      {
        if( coolingCoil->handle() == this->handle() )
        {
          return airLoopHVACUnitarySystem;
        }
      }
    }
    return boost::none;
  }

  boost::optional<ZoneHVACComponent> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACWaterToAirHeatPump
    auto const zoneHVACWaterToAirHeatPumps = this->model().getConcreteModelObjects<ZoneHVACWaterToAirHeatPump>();

    for( const auto & zoneHVACWaterToAirHeatPump : zoneHVACWaterToAirHeatPumps )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACWaterToAirHeatPump.coolingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return zoneHVACWaterToAirHeatPump;
        }
      }
    }

    return boost::none;
  }

  boost::optional<ModelObjectList> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::speedDataList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::SpeedDataList);
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::addSpeed( const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData & speed) {
    auto modelObjectList = speedDataList();
    if( modelObjectList ) {
      modelObjectList->addModelObject(speed);
    }
    return false;
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::removeSpeed( const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData & speed) {
    auto modelObjectList = speedDataList();
    if( modelObjectList ) {
      modelObjectList->removeModelObject(speed);
    }
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::removeAllSpeeds() {
    auto modelObjectList = speedDataList();
    if( modelObjectList ) {
      auto const modelObjects = modelObjectList->modelObjects();

      for(const auto & elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData>();
          if (modelObject) {
            modelObjectList->removeModelObject(elem);
          }
      }
    }
  }

  std::vector<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::speeds() const {
    std::vector<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> result;
    auto const modelObjectList = speedDataList();
    if( modelObjectList ) {
      auto const modelObjects = modelObjectList->modelObjects();

      for(const auto & elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData>();
          if (modelObject) {
            result.push_back(modelObject.get());
          }
      }
    }
    return result;
  }

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setSpeedDataList(const boost::optional<ModelObjectList>& modelObjectList) {
    bool result(false);
    if (modelObjectList) {
      result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::SpeedDataList, modelObjectList.get().handle());
    }
    else {
      resetSpeedDataList();
      result = true;
    }
    return result;
  }

  void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::resetSpeedDataList() {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::SpeedDataList, "");
    OS_ASSERT(result);
  }

} // detail

CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(const Model& model)
  : WaterToAirComponent(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>());

  bool ok = true;
  setNominalSpeedLevel(1);
  autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
  autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
  autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel();
  ok = setNominalTimeforCondensatetoBeginLeavingtheCoil(0);
  OS_ASSERT(ok);
  ok = setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(0);
  OS_ASSERT(ok);
  setUseHotGasReheat(false);

  auto partLoadFraction = CurveQuadratic(model);
  partLoadFraction.setCoefficient1Constant(0.85);
  partLoadFraction.setCoefficient2x(0.15);
  partLoadFraction.setCoefficient3xPOW2(0.0);
  partLoadFraction.setMinimumValueofx(0.0);
  partLoadFraction.setMaximumValueofx(1.0);

  ok = setEnergyPartLoadFractionCurve(partLoadFraction);
  OS_ASSERT(ok);

  auto speedDataList = ModelObjectList(model);
  speedDataList.setName(this->name().get() + " Speed Data List");
  ok = getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setSpeedDataList(speedDataList);
  OS_ASSERT(ok);
}

CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(const Model& model,
                                                                                                             const Curve& partLoadFraction)
  : WaterToAirComponent(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>());

  bool ok = true;
  setNominalSpeedLevel(1);
  autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
  autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
  autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel();
  ok = setNominalTimeforCondensatetoBeginLeavingtheCoil(0);
  OS_ASSERT(ok);
  ok = setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(0);
  OS_ASSERT(ok);
  setUseHotGasReheat(false);
  ok = setEnergyPartLoadFractionCurve(partLoadFraction);
  OS_ASSERT(ok);

  auto speedDataList = ModelObjectList(model);
  speedDataList.setName(this->name().get() + " Speed Data List");
  ok = getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setSpeedDataList(speedDataList);
  OS_ASSERT(ok);
}

IddObjectType CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit);
}

int CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::nominalSpeedLevel() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->nominalSpeedLevel();
}

boost::optional<double> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized();
}

boost::optional<double> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::ratedAirFlowRateAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->ratedAirFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized();
}

boost::optional<double> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::ratedWaterFlowRateAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->ratedWaterFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized();
}

double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::nominalTimeforCondensatetoBeginLeavingtheCoil() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->nominalTimeforCondensatetoBeginLeavingtheCoil();
}

double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity();
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::useHotGasReheat() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->useHotGasReheat();
}

Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::energyPartLoadFractionCurve() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->energyPartLoadFractionCurve();
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::setNominalSpeedLevel(int nominalSpeedLevel) {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setNominalSpeedLevel(nominalSpeedLevel);
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(double grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel);
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setRatedAirFlowRateAtSelectedNominalSpeedLevel(ratedAirFlowRateAtSelectedNominalSpeedLevel);
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::setRatedWaterFlowRateAtSelectedNominalSpeedLevel(double ratedWaterFlowRateAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setRatedWaterFlowRateAtSelectedNominalSpeedLevel(ratedWaterFlowRateAtSelectedNominalSpeedLevel);
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setNominalTimeforCondensatetoBeginLeavingtheCoil(nominalTimeforCondensatetoBeginLeavingtheCoil);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::setUseHotGasReheat(bool useHotGasReheat) {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setUseHotGasReheat(useHotGasReheat);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::setEnergyPartLoadFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setEnergyPartLoadFractionCurve(curve);
}

std::vector<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::speeds() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->speeds();
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::addSpeed(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->addSpeed(speed);
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::removeSpeed(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed) {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->removeSpeed(speed);
}

void CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::removeAllSpeeds() {
  getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->removeAllSpeeds();
}

/// @cond
CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(std::shared_ptr<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl> impl)
  : WaterToAirComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

