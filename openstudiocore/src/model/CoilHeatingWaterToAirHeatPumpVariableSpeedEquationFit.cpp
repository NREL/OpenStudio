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

#include "CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include "CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "HVACComponent.hpp"
#include "ZoneHVACWaterToAirHeatPump.hpp"
#include "ZoneHVACWaterToAirHeatPump_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const IdfObject& idfObject,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : WaterToAirComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType());
  }

  CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType());
  }

  CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl(const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::iddObjectType() const {
    return CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType();
  }

  unsigned CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::airInletPort() {
    return OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirInletNodeName;
  }

  unsigned CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::airOutletPort() {
    return OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirOutletNodeName;
  }

  unsigned CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::waterInletPort() {
    return OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterInletNodeName;
  }

  unsigned CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::waterOutletPort() {
    return OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::WatertoRefrigerantHXWaterOutletNodeName;
  }

  int CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::nominalSpeedLevel() const {
    boost::optional<int> value = getInt(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalSpeedLevel,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::ratedHeatingCapacityAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel,true);
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::ratedAirFlowRateAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel,true);
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::ratedWaterFlowRateAtSelectedNominalSpeedLevel() const {
    return getDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel,true);
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::energyPartLoadFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyPartLoadFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Part Load Fraction Curve attached.");
    }
    return value.get();
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setNominalSpeedLevel(int nominalSpeedLevel) {
    bool result = setInt(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::NominalSpeedLevel, nominalSpeedLevel);
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setRatedHeatingCapacityAtSelectedNominalSpeedLevel(boost::optional<double> ratedHeatingCapacityAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (ratedHeatingCapacityAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel, ratedHeatingCapacityAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setRatedAirFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (ratedAirFlowRateAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, ratedAirFlowRateAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedAirFlowRateAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setRatedWaterFlowRateAtSelectedNominalSpeedLevel(boost::optional<double> ratedWaterFlowRateAtSelectedNominalSpeedLevel) {
    bool result(false);
    if (ratedWaterFlowRateAtSelectedNominalSpeedLevel) {
      result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel, ratedWaterFlowRateAtSelectedNominalSpeedLevel.get());
    }
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel() {
    bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::RatedWaterFlowRateAtSelectedNominalSpeedLevel, "autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setEnergyPartLoadFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::EnergyPartLoadFractionCurveName, curve.handle());
    return result;
  }

  boost::optional<Curve> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::optionalEnergyPartLoadFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::EnergyPartLoadFractionCurveName);
  }

  ModelObject CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::clone(Model model) const
  {
    auto newCoil = WaterToAirComponent_Impl::clone(model).cast<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit>();

    if (auto speedDataList = this->speedDataList()) {
      auto speedDataListClone = speedDataList->clone(model).cast<ModelObjectList>();
      newCoil.getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setSpeedDataList(speedDataListClone);
    }

    return newCoil;
  }

  std::vector<ModelObject> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::children() const {
    std::vector<ModelObject> children;

    if( auto const _stageDataList = speedDataList() ) {
      for( const auto & mo : _stageDataList->modelObjects() ) {
        children.push_back( mo );
      }
    }

    children.push_back( energyPartLoadFractionCurve() );
    return children;
  }

  std::vector<openstudio::IdfObject> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::remove()
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

  boost::optional<HVACComponent> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::containingHVACComponent() const
  {
    // AirLoopHVACUnitarySystem
    auto const airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

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
    return boost::none;
  }

  boost::optional<ZoneHVACComponent> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACWaterToAirHeatPump
    auto const zoneHVACWaterToAirHeatPumps = this->model().getConcreteModelObjects<ZoneHVACWaterToAirHeatPump>();

    for( const auto & zoneHVACWaterToAirHeatPump : zoneHVACWaterToAirHeatPumps )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACWaterToAirHeatPump.heatingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return zoneHVACWaterToAirHeatPump;
        }
      }
    }

    return boost::none;
  }

  boost::optional<ModelObjectList> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::speedDataList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::SpeedDataList);
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::addSpeed( const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData & speed) {
    auto modelObjectList = speedDataList();
    if( modelObjectList ) {
      modelObjectList->addModelObject(speed);
    }
    return false;
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::removeSpeed( const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData & speed) {
    auto modelObjectList = speedDataList();
    if( modelObjectList ) {
      modelObjectList->removeModelObject(speed);
    }
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::removeAllSpeeds() {
    auto modelObjectList = speedDataList();
    if( modelObjectList ) {
      auto const modelObjects = modelObjectList->modelObjects();

      for(const auto & elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData>();
          if (modelObject) {
            modelObjectList->removeModelObject(elem);
          }
      }
    }
  }

  std::vector<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::speeds() const {
    std::vector<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> result;
    auto const modelObjectList = speedDataList();
    if( modelObjectList ) {
      auto const modelObjects = modelObjectList->modelObjects();

      for(const auto & elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData>();
          if (modelObject) {
            result.push_back(modelObject.get());
          }
      }
    }
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::setSpeedDataList(const boost::optional<ModelObjectList>& modelObjectList) {
    bool result(false);
    if (modelObjectList) {
      result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::SpeedDataList, modelObjectList.get().handle());
    }
    else {
      resetSpeedDataList();
      result = true;
    }
    return result;
  }

  void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl::resetSpeedDataList() {
    bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::SpeedDataList, "");
    OS_ASSERT(result);
  }

} // detail

CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit(const Model& model)
  : WaterToAirComponent(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>());

  bool ok = true;
  setNominalSpeedLevel(1);
  autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel();
  autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
  autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel();

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
  ok = getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setSpeedDataList(speedDataList);
  OS_ASSERT(ok);
}

CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit(const Model& model,
                                                                                                             const Curve& partLoadFraction)
  : WaterToAirComponent(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>());

  bool ok = true;
  setNominalSpeedLevel(1);
  autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel();
  autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
  autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel();
  ok = setEnergyPartLoadFractionCurve(partLoadFraction);
  OS_ASSERT(ok);

  auto speedDataList = ModelObjectList(model);
  speedDataList.setName(this->name().get() + " Speed Data List");
  ok = getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setSpeedDataList(speedDataList);
  OS_ASSERT(ok);
}

IddObjectType CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit);
}

int CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::nominalSpeedLevel() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->nominalSpeedLevel();
}

boost::optional<double> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::ratedHeatingCapacityAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->ratedHeatingCapacityAtSelectedNominalSpeedLevel();
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized();
}

boost::optional<double> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::ratedAirFlowRateAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->ratedAirFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized();
}

boost::optional<double> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::ratedWaterFlowRateAtSelectedNominalSpeedLevel() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->ratedWaterFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized();
}

Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::energyPartLoadFractionCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->energyPartLoadFractionCurve();
}

void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::setNominalSpeedLevel(int nominalSpeedLevel) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setNominalSpeedLevel(nominalSpeedLevel);
}

void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::setRatedHeatingCapacityAtSelectedNominalSpeedLevel(double ratedHeatingCapacityAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setRatedHeatingCapacityAtSelectedNominalSpeedLevel(ratedHeatingCapacityAtSelectedNominalSpeedLevel);
}

void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel();
}

void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setRatedAirFlowRateAtSelectedNominalSpeedLevel(ratedAirFlowRateAtSelectedNominalSpeedLevel);
}

void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
}

void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::setRatedWaterFlowRateAtSelectedNominalSpeedLevel(double ratedWaterFlowRateAtSelectedNominalSpeedLevel) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setRatedWaterFlowRateAtSelectedNominalSpeedLevel(ratedWaterFlowRateAtSelectedNominalSpeedLevel);
}

void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel() {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel();
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::setEnergyPartLoadFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->setEnergyPartLoadFractionCurve(curve);
}

std::vector<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::speeds() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->speeds();
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::addSpeed(const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->addSpeed(speed);
}

void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::removeSpeed(const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->removeSpeed(speed);
}

void CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::removeAllSpeeds() {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl>()->removeAllSpeeds();
}

/// @cond
CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit(std::shared_ptr<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl> impl)
  : WaterToAirComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

