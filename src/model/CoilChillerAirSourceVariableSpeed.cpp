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

#include "CoilChillerAirSourceVariableSpeed.hpp"
#include "CoilChillerAirSourceVariableSpeed_Impl.hpp"

#include "CoilChillerAirSourceVariableSpeedSpeedData.hpp"
#include "CoilChillerAirSourceVariableSpeedSpeedData_Impl.hpp"
#include "CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "CoilSystemIntegratedHeatPumpAirSource_Impl.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Coil_Chiller_AirSource_VariableSpeed_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilChillerAirSourceVariableSpeed_Impl::CoilChillerAirSourceVariableSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilChillerAirSourceVariableSpeed::iddObjectType());
    }

    CoilChillerAirSourceVariableSpeed_Impl::CoilChillerAirSourceVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilChillerAirSourceVariableSpeed::iddObjectType());
    }

    CoilChillerAirSourceVariableSpeed_Impl::CoilChillerAirSourceVariableSpeed_Impl(const CoilChillerAirSourceVariableSpeed_Impl& other,
                                                                                   Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilChillerAirSourceVariableSpeed_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;

      // TODO

      return result;
    }

    IddObjectType CoilChillerAirSourceVariableSpeed_Impl::iddObjectType() const {
      return CoilChillerAirSourceVariableSpeed::iddObjectType();
    }

    std::vector<ScheduleTypeKey> CoilChillerAirSourceVariableSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_Coil_Chiller_AirSource_VariableSpeedFields::GridSignalScheduleName) != e) {
        result.push_back(ScheduleTypeKey("CoilChillerAirSourceVariableSpeed", "Grid Signal"));
      }
      return result;
    }

    unsigned CoilChillerAirSourceVariableSpeed_Impl::inletPort() const {
      return OS_Coil_Chiller_AirSource_VariableSpeedFields::EvaporatorWaterInletNodeName;
    }

    unsigned CoilChillerAirSourceVariableSpeed_Impl::outletPort() const {
      return OS_Coil_Chiller_AirSource_VariableSpeedFields::EvaporatorWaterOutletNodeName;
    }

    ModelObject CoilChillerAirSourceVariableSpeed_Impl::clone(Model model) const {
      auto t_clone = StraightComponent_Impl::clone(model).cast<CoilChillerAirSourceVariableSpeed>();

      if (auto speedDataList = this->speedDataList()) {
        auto speedDataListClone = speedDataList->clone(model).cast<ModelObjectList>();
        t_clone.getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setSpeedDataList(speedDataListClone);
      }

      return t_clone;
    }

    std::vector<IdfObject> CoilChillerAirSourceVariableSpeed_Impl::remove() {
      auto _stageDataList = speedDataList();
      auto result = StraightComponent_Impl::remove();
      if ((!result.empty()) && _stageDataList) {
        _stageDataList->remove();
      }

      return result;
    }

    std::vector<ModelObject> CoilChillerAirSourceVariableSpeed_Impl::children() const {
      std::vector<ModelObject> result;

      boost::optional<ModelObject> curve;

      curve = partLoadFractionCorrelationCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      if (auto const _stageDataList = speedDataList()) {
        for (const auto& mo : _stageDataList->modelObjects()) {
          result.push_back(mo);
        }
      }
      return result;
    }

    boost::optional<HVACComponent> CoilChillerAirSourceVariableSpeed_Impl::containingHVACComponent() const {
      // CoilSystemIntegratedHeatPumpAirSource
      {
        auto coilSystems = this->model().getConcreteModelObjects<CoilSystemIntegratedHeatPumpAirSource>();
        for (const auto& coilSystem : coilSystems) {
          if (coilSystem.chillingCoil()) {
            if (coilSystem.chillingCoil().get().handle() == this->handle()) {
              return coilSystem;
            }
          }
        }
      }

      return boost::none;
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::addToNode(Node& node) {

      auto t_containingHVACComponent = containingHVACComponent();
      if (t_containingHVACComponent && t_containingHVACComponent->optionalCast<CoilSystemIntegratedHeatPumpAirSource>()) {
        LOG(Warn, this->briefDescription() << " cannot be connected directly when it's part of a parent CoilSystemIntegratedHeatPumpAirSource. "
                                              "Please call CoilSystemIntegratedHeatPumpAirSource::addToNode instead");
      }
      return false;
    }

    void CoilChillerAirSourceVariableSpeed_Impl::autosize() {
      autosizeRatedChilledWaterCapacity();
      autocalculateRatedEvaporatorWaterFlowRate();
    }

    void CoilChillerAirSourceVariableSpeed_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedRatedChilledWaterCapacity();
      if (val) {
        setRatedChilledWaterCapacity(val.get());
      }

      val = autocalculatedRatedEvaporatorWaterFlowRate();
      if (val) {
        setRatedEvaporatorWaterFlowRate(val.get());
      }
    }

    int CoilChillerAirSourceVariableSpeed_Impl::nominalSpeedLevel() const {
      boost::optional<int> value = getInt(OS_Coil_Chiller_AirSource_VariableSpeedFields::NominalSpeedLevel, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilChillerAirSourceVariableSpeed_Impl::ratedChilledWaterCapacity() const {
      return getDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::RatedChilledWaterCapacity, true);
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::isRatedChilledWaterCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Chiller_AirSource_VariableSpeedFields::RatedChilledWaterCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double CoilChillerAirSourceVariableSpeed_Impl::ratedEvaporatorInletWaterTemperature() const {
      boost::optional<double> value = getDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::RatedEvaporatorInletWaterTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilChillerAirSourceVariableSpeed_Impl::ratedCondenserInletAirTemperature() const {
      boost::optional<double> value = getDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::RatedCondenserInletAirTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilChillerAirSourceVariableSpeed_Impl::ratedEvaporatorWaterFlowRate() const {
      return getDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::RatedEvaporatorWaterFlowRate, true);
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::isRatedEvaporatorWaterFlowRateAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Chiller_AirSource_VariableSpeedFields::RatedEvaporatorWaterFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autocalculate");
      }
      return result;
    }

    std::string CoilChillerAirSourceVariableSpeed_Impl::evaporatorPumpPowerIncludedinRatedCOP() const {
      boost::optional<std::string> value = getString(OS_Coil_Chiller_AirSource_VariableSpeedFields::EvaporatorPumpPowerIncludedinRatedCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string CoilChillerAirSourceVariableSpeed_Impl::evaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP() const {
      boost::optional<std::string> value =
        getString(OS_Coil_Chiller_AirSource_VariableSpeedFields::EvaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilChillerAirSourceVariableSpeed_Impl::fractionofEvaporatorPumpHeattoWater() const {
      boost::optional<double> value = getDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::FractionofEvaporatorPumpHeattoWater, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilChillerAirSourceVariableSpeed_Impl::crankcaseHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::CrankcaseHeaterCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilChillerAirSourceVariableSpeed_Impl::maximumAmbientTemperatureforCrankcaseHeaterOperation() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> CoilChillerAirSourceVariableSpeed_Impl::partLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Chiller_AirSource_VariableSpeedFields::PartLoadFractionCorrelationCurveName);
    }

    boost::optional<Schedule> CoilChillerAirSourceVariableSpeed_Impl::gridSignalSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Chiller_AirSource_VariableSpeedFields::GridSignalScheduleName);
    }

    double CoilChillerAirSourceVariableSpeed_Impl::lowerBoundToApplyGridResponsiveControl() const {
      boost::optional<double> value = getDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::LowerBoundToApplyGridResponsiveControl, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::isLowerBoundToApplyGridResponsiveControlDefaulted() const {
      return isEmpty(OS_Coil_Chiller_AirSource_VariableSpeedFields::LowerBoundToApplyGridResponsiveControl);
    }

    double CoilChillerAirSourceVariableSpeed_Impl::upperBoundToApplyGridResponsiveControl() const {
      boost::optional<double> value = getDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::UpperBoundToApplyGridResponsiveControl, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::isUpperBoundToApplyGridResponsiveControlDefaulted() const {
      return isEmpty(OS_Coil_Chiller_AirSource_VariableSpeedFields::UpperBoundToApplyGridResponsiveControl);
    }

    int CoilChillerAirSourceVariableSpeed_Impl::maxSpeedLevelDuringGridResponsiveControl() const {
      boost::optional<int> value = getInt(OS_Coil_Chiller_AirSource_VariableSpeedFields::MaxSpeedLevelDuringGridResponsiveControl, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::isMaxSpeedLevelDuringGridResponsiveControlDefaulted() const {
      return isEmpty(OS_Coil_Chiller_AirSource_VariableSpeedFields::MaxSpeedLevelDuringGridResponsiveControl);
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::setNominalSpeedLevel(int nominalSpeedLevel) {
      bool result = setInt(OS_Coil_Chiller_AirSource_VariableSpeedFields::NominalSpeedLevel, nominalSpeedLevel);
      OS_ASSERT(result);
      return result;
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::setRatedChilledWaterCapacity(boost::optional<double> ratedChilledWaterCapacity) {
      bool result(false);
      if (ratedChilledWaterCapacity) {
        result = setDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::RatedChilledWaterCapacity, ratedChilledWaterCapacity.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void CoilChillerAirSourceVariableSpeed_Impl::autosizeRatedChilledWaterCapacity() {
      bool result = setString(OS_Coil_Chiller_AirSource_VariableSpeedFields::RatedChilledWaterCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::setRatedEvaporatorInletWaterTemperature(double ratedEvaporatorInletWaterTemperature) {
      bool result =
        setDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::RatedEvaporatorInletWaterTemperature, ratedEvaporatorInletWaterTemperature);
      return result;
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::setRatedCondenserInletAirTemperature(double ratedCondenserInletAirTemperature) {
      bool result = setDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::RatedCondenserInletAirTemperature, ratedCondenserInletAirTemperature);
      return result;
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::setRatedEvaporatorWaterFlowRate(boost::optional<double> ratedEvaporatorWaterFlowRate) {
      bool result(false);
      if (ratedEvaporatorWaterFlowRate) {
        result = setDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::RatedEvaporatorWaterFlowRate, ratedEvaporatorWaterFlowRate.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void CoilChillerAirSourceVariableSpeed_Impl::autocalculateRatedEvaporatorWaterFlowRate() {
      bool result = setString(OS_Coil_Chiller_AirSource_VariableSpeedFields::RatedEvaporatorWaterFlowRate, "autocalculate");
      OS_ASSERT(result);
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::setEvaporatorPumpPowerIncludedinRatedCOP(std::string evaporatorPumpPowerIncludedinRatedCOP) {
      bool result =
        setString(OS_Coil_Chiller_AirSource_VariableSpeedFields::EvaporatorPumpPowerIncludedinRatedCOP, evaporatorPumpPowerIncludedinRatedCOP);
      return result;
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::setEvaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP(
      std::string evaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP) {
      bool result = setString(OS_Coil_Chiller_AirSource_VariableSpeedFields::EvaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP,
                              evaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP);
      return result;
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::setFractionofEvaporatorPumpHeattoWater(double fractionofEvaporatorPumpHeattoWater) {
      bool result =
        setDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::FractionofEvaporatorPumpHeattoWater, fractionofEvaporatorPumpHeattoWater);
      return result;
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
      bool result = setDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::CrankcaseHeaterCapacity, crankcaseHeaterCapacity);
      return result;
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::setMaximumAmbientTemperatureforCrankcaseHeaterOperation(
      double maximumAmbientTemperatureforCrankcaseHeaterOperation) {
      bool result = setDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation,
                              maximumAmbientTemperatureforCrankcaseHeaterOperation);
      return result;
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::setPartLoadFractionCorrelationCurve(const boost::optional<Curve>& partLoadFractionCorrelationCurve) {
      bool result(false);
      if (partLoadFractionCorrelationCurve) {
        result = setPointer(OS_Coil_Chiller_AirSource_VariableSpeedFields::PartLoadFractionCorrelationCurveName,
                            partLoadFractionCorrelationCurve.get().handle());
      } else {
        resetPartLoadFractionCorrelationCurve();
        result = true;
      }
      return result;
    }

    void CoilChillerAirSourceVariableSpeed_Impl::resetPartLoadFractionCorrelationCurve() {
      bool result = setString(OS_Coil_Chiller_AirSource_VariableSpeedFields::PartLoadFractionCorrelationCurveName, "");
      OS_ASSERT(result);
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::setGridSignalSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Chiller_AirSource_VariableSpeedFields::GridSignalScheduleName, "CoilChillerAirSourceVariableSpeed",
                                "Grid Signal", schedule);
      OS_ASSERT(result);
      return result;
    }

    void CoilChillerAirSourceVariableSpeed_Impl::resetGridSignalSchedule() {
      bool result = setString(OS_Coil_Chiller_AirSource_VariableSpeedFields::GridSignalScheduleName, "");
      OS_ASSERT(result);
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::setLowerBoundToApplyGridResponsiveControl(double lowerBoundToApplyGridResponsiveControl) {
      bool result =
        setDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::LowerBoundToApplyGridResponsiveControl, lowerBoundToApplyGridResponsiveControl);
      OS_ASSERT(result);
      return result;
    }

    void CoilChillerAirSourceVariableSpeed_Impl::resetLowerBoundToApplyGridResponsiveControl() {
      bool result = setString(OS_Coil_Chiller_AirSource_VariableSpeedFields::LowerBoundToApplyGridResponsiveControl, "");
      OS_ASSERT(result);
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::setUpperBoundToApplyGridResponsiveControl(double upperBoundToApplyGridResponsiveControl) {
      bool result =
        setDouble(OS_Coil_Chiller_AirSource_VariableSpeedFields::UpperBoundToApplyGridResponsiveControl, upperBoundToApplyGridResponsiveControl);
      OS_ASSERT(result);
      return result;
    }

    void CoilChillerAirSourceVariableSpeed_Impl::resetUpperBoundToApplyGridResponsiveControl() {
      bool result = setString(OS_Coil_Chiller_AirSource_VariableSpeedFields::UpperBoundToApplyGridResponsiveControl, "");
      OS_ASSERT(result);
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::setMaxSpeedLevelDuringGridResponsiveControl(int maxSpeedLevelDuringGridResponsiveControl) {
      bool result =
        setInt(OS_Coil_Chiller_AirSource_VariableSpeedFields::MaxSpeedLevelDuringGridResponsiveControl, maxSpeedLevelDuringGridResponsiveControl);
      OS_ASSERT(result);
      return result;
    }

    void CoilChillerAirSourceVariableSpeed_Impl::resetMaxSpeedLevelDuringGridResponsiveControl() {
      bool result = setString(OS_Coil_Chiller_AirSource_VariableSpeedFields::MaxSpeedLevelDuringGridResponsiveControl, "");
      OS_ASSERT(result);
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::setSpeedDataList(const boost::optional<ModelObjectList>& modelObjectList) {
      bool result(false);
      if (modelObjectList) {
        result = setPointer(OS_Coil_Chiller_AirSource_VariableSpeedFields::SpeedDataList, modelObjectList.get().handle());
      } else {
        resetSpeedDataList();
        result = true;
      }
      return result;
    }

    void CoilChillerAirSourceVariableSpeed_Impl::resetSpeedDataList() {
      bool result = setString(OS_Coil_Chiller_AirSource_VariableSpeedFields::SpeedDataList, "");
      OS_ASSERT(result);
    }

    boost::optional<ModelObjectList> CoilChillerAirSourceVariableSpeed_Impl::speedDataList() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Coil_Chiller_AirSource_VariableSpeedFields::SpeedDataList);
    }

    std::vector<CoilChillerAirSourceVariableSpeedSpeedData> CoilChillerAirSourceVariableSpeed_Impl::speeds() const {
      std::vector<CoilChillerAirSourceVariableSpeedSpeedData> result;
      auto const modelObjectList = speedDataList();
      if (modelObjectList) {
        auto const modelObjects = modelObjectList->modelObjects();

        for (const auto& elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CoilChillerAirSourceVariableSpeedSpeedData>();
          if (modelObject) {
            result.push_back(modelObject.get());
          }
        }
      }
      return result;
    }

    bool CoilChillerAirSourceVariableSpeed_Impl::addSpeed(const CoilChillerAirSourceVariableSpeedSpeedData& speed) {
      bool result = false;
      auto modelObjectList = speedDataList();
      if (modelObjectList) {
        result = modelObjectList->addModelObject(speed);
      }
      return result;
    }

    void CoilChillerAirSourceVariableSpeed_Impl::removeSpeed(const CoilChillerAirSourceVariableSpeedSpeedData& speed) {
      auto modelObjectList = speedDataList();
      if (modelObjectList) {
        modelObjectList->removeModelObject(speed);
      }
    }

    void CoilChillerAirSourceVariableSpeed_Impl::removeAllSpeeds() {
      auto modelObjectList = speedDataList();
      if (modelObjectList) {
        auto const modelObjects = modelObjectList->modelObjects();

        for (const auto& elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CoilChillerAirSourceVariableSpeedSpeedData>();
          if (modelObject) {
            modelObjectList->removeModelObject(elem);
          }
        }
      }
    }

    boost::optional<double> CoilChillerAirSourceVariableSpeed_Impl::autosizedRatedChilledWaterCapacity() const {
      return getAutosizedValue("TODO", "W");
    }

    boost::optional<double> CoilChillerAirSourceVariableSpeed_Impl::autocalculatedRatedEvaporatorWaterFlowRate() const {
      return getAutosizedValue("TODO", "m3/s");
    }

  }  // namespace detail

  CoilChillerAirSourceVariableSpeed::CoilChillerAirSourceVariableSpeed(const Model& model)
    : StraightComponent(CoilChillerAirSourceVariableSpeed::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>());

    bool ok = true;
    ok = setNominalSpeedLevel(1);
    OS_ASSERT(ok);
    autosizeRatedChilledWaterCapacity();
    ok = setRatedEvaporatorInletWaterTemperature(8.0);
    OS_ASSERT(ok);
    ok = setRatedCondenserInletAirTemperature(35.0);
    OS_ASSERT(ok);
    autocalculateRatedEvaporatorWaterFlowRate();
    ok = setEvaporatorPumpPowerIncludedinRatedCOP("No");
    OS_ASSERT(ok);
    ok = setEvaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP("No");
    OS_ASSERT(ok);
    ok = setFractionofEvaporatorPumpHeattoWater(0.2);
    OS_ASSERT(ok);
    ok = setCrankcaseHeaterCapacity(0);
    OS_ASSERT(ok);
    ok = setMaximumAmbientTemperatureforCrankcaseHeaterOperation(10);
    OS_ASSERT(ok);

    auto speedDataList = ModelObjectList(model);
    speedDataList.setName(this->name().get() + " Speed Data List");
    ok = getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setSpeedDataList(speedDataList);
    OS_ASSERT(ok);
  }

  IddObjectType CoilChillerAirSourceVariableSpeed::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Coil_Chiller_AirSource_VariableSpeed);
  }

  int CoilChillerAirSourceVariableSpeed::nominalSpeedLevel() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->nominalSpeedLevel();
  }

  boost::optional<double> CoilChillerAirSourceVariableSpeed::ratedChilledWaterCapacity() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->ratedChilledWaterCapacity();
  }

  bool CoilChillerAirSourceVariableSpeed::isRatedChilledWaterCapacityAutosized() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->isRatedChilledWaterCapacityAutosized();
  }

  double CoilChillerAirSourceVariableSpeed::ratedEvaporatorInletWaterTemperature() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->ratedEvaporatorInletWaterTemperature();
  }

  double CoilChillerAirSourceVariableSpeed::ratedCondenserInletAirTemperature() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->ratedCondenserInletAirTemperature();
  }

  boost::optional<double> CoilChillerAirSourceVariableSpeed::ratedEvaporatorWaterFlowRate() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->ratedEvaporatorWaterFlowRate();
  }

  bool CoilChillerAirSourceVariableSpeed::isRatedEvaporatorWaterFlowRateAutocalculated() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->isRatedEvaporatorWaterFlowRateAutocalculated();
  }

  std::string CoilChillerAirSourceVariableSpeed::evaporatorPumpPowerIncludedinRatedCOP() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->evaporatorPumpPowerIncludedinRatedCOP();
  }

  std::string CoilChillerAirSourceVariableSpeed::evaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->evaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP();
  }

  double CoilChillerAirSourceVariableSpeed::fractionofEvaporatorPumpHeattoWater() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->fractionofEvaporatorPumpHeattoWater();
  }

  double CoilChillerAirSourceVariableSpeed::crankcaseHeaterCapacity() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->crankcaseHeaterCapacity();
  }

  double CoilChillerAirSourceVariableSpeed::maximumAmbientTemperatureforCrankcaseHeaterOperation() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->maximumAmbientTemperatureforCrankcaseHeaterOperation();
  }

  boost::optional<Curve> CoilChillerAirSourceVariableSpeed::partLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->partLoadFractionCorrelationCurve();
  }

  boost::optional<Schedule> CoilChillerAirSourceVariableSpeed::gridSignalSchedule() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->gridSignalSchedule();
  }

  double CoilChillerAirSourceVariableSpeed::lowerBoundToApplyGridResponsiveControl() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->lowerBoundToApplyGridResponsiveControl();
  }

  bool CoilChillerAirSourceVariableSpeed::isLowerBoundToApplyGridResponsiveControlDefaulted() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->isLowerBoundToApplyGridResponsiveControlDefaulted();
  }

  double CoilChillerAirSourceVariableSpeed::upperBoundToApplyGridResponsiveControl() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->upperBoundToApplyGridResponsiveControl();
  }

  bool CoilChillerAirSourceVariableSpeed::isUpperBoundToApplyGridResponsiveControlDefaulted() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->isUpperBoundToApplyGridResponsiveControlDefaulted();
  }

  int CoilChillerAirSourceVariableSpeed::maxSpeedLevelDuringGridResponsiveControl() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->maxSpeedLevelDuringGridResponsiveControl();
  }

  bool CoilChillerAirSourceVariableSpeed::isMaxSpeedLevelDuringGridResponsiveControlDefaulted() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->isMaxSpeedLevelDuringGridResponsiveControlDefaulted();
  }

  bool CoilChillerAirSourceVariableSpeed::setNominalSpeedLevel(int nominalSpeedLevel) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setNominalSpeedLevel(nominalSpeedLevel);
  }

  bool CoilChillerAirSourceVariableSpeed::setRatedChilledWaterCapacity(double ratedChilledWaterCapacity) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setRatedChilledWaterCapacity(ratedChilledWaterCapacity);
  }

  void CoilChillerAirSourceVariableSpeed::autosizeRatedChilledWaterCapacity() {
    getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->autosizeRatedChilledWaterCapacity();
  }

  bool CoilChillerAirSourceVariableSpeed::setRatedEvaporatorInletWaterTemperature(double ratedEvaporatorInletWaterTemperature) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setRatedEvaporatorInletWaterTemperature(ratedEvaporatorInletWaterTemperature);
  }

  bool CoilChillerAirSourceVariableSpeed::setRatedCondenserInletAirTemperature(double ratedCondenserInletAirTemperature) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setRatedCondenserInletAirTemperature(ratedCondenserInletAirTemperature);
  }

  bool CoilChillerAirSourceVariableSpeed::setRatedEvaporatorWaterFlowRate(double ratedEvaporatorWaterFlowRate) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setRatedEvaporatorWaterFlowRate(ratedEvaporatorWaterFlowRate);
  }

  void CoilChillerAirSourceVariableSpeed::autocalculateRatedEvaporatorWaterFlowRate() {
    getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->autocalculateRatedEvaporatorWaterFlowRate();
  }

  bool CoilChillerAirSourceVariableSpeed::setEvaporatorPumpPowerIncludedinRatedCOP(std::string evaporatorPumpPowerIncludedinRatedCOP) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setEvaporatorPumpPowerIncludedinRatedCOP(evaporatorPumpPowerIncludedinRatedCOP);
  }

  bool CoilChillerAirSourceVariableSpeed::setEvaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP(
    std::string evaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setEvaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP(
      evaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP);
  }

  bool CoilChillerAirSourceVariableSpeed::setFractionofEvaporatorPumpHeattoWater(double fractionofEvaporatorPumpHeattoWater) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setFractionofEvaporatorPumpHeattoWater(fractionofEvaporatorPumpHeattoWater);
  }

  bool CoilChillerAirSourceVariableSpeed::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setCrankcaseHeaterCapacity(crankcaseHeaterCapacity);
  }

  bool CoilChillerAirSourceVariableSpeed::setMaximumAmbientTemperatureforCrankcaseHeaterOperation(
    double maximumAmbientTemperatureforCrankcaseHeaterOperation) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setMaximumAmbientTemperatureforCrankcaseHeaterOperation(
      maximumAmbientTemperatureforCrankcaseHeaterOperation);
  }

  bool CoilChillerAirSourceVariableSpeed::setPartLoadFractionCorrelationCurve(const Curve& partLoadFractionCorrelationCurve) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setPartLoadFractionCorrelationCurve(partLoadFractionCorrelationCurve);
  }

  void CoilChillerAirSourceVariableSpeed::resetPartLoadFractionCorrelationCurve() {
    getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->resetPartLoadFractionCorrelationCurve();
  }

  bool CoilChillerAirSourceVariableSpeed::setGridSignalSchedule(Schedule& schedule) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setGridSignalSchedule(schedule);
  }

  void CoilChillerAirSourceVariableSpeed::resetGridSignalSchedule() {
    getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->resetGridSignalSchedule();
  }

  bool CoilChillerAirSourceVariableSpeed::setLowerBoundToApplyGridResponsiveControl(double lowerBoundToApplyGridResponsiveControl) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setLowerBoundToApplyGridResponsiveControl(
      lowerBoundToApplyGridResponsiveControl);
  }

  void CoilChillerAirSourceVariableSpeed::resetLowerBoundToApplyGridResponsiveControl() {
    getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->resetLowerBoundToApplyGridResponsiveControl();
  }

  bool CoilChillerAirSourceVariableSpeed::setUpperBoundToApplyGridResponsiveControl(double upperBoundToApplyGridResponsiveControl) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setUpperBoundToApplyGridResponsiveControl(
      upperBoundToApplyGridResponsiveControl);
  }

  void CoilChillerAirSourceVariableSpeed::resetUpperBoundToApplyGridResponsiveControl() {
    getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->resetUpperBoundToApplyGridResponsiveControl();
  }

  bool CoilChillerAirSourceVariableSpeed::setMaxSpeedLevelDuringGridResponsiveControl(int maxSpeedLevelDuringGridResponsiveControl) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->setMaxSpeedLevelDuringGridResponsiveControl(
      maxSpeedLevelDuringGridResponsiveControl);
  }

  void CoilChillerAirSourceVariableSpeed::resetMaxSpeedLevelDuringGridResponsiveControl() {
    getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->resetMaxSpeedLevelDuringGridResponsiveControl();
  }

  std::vector<CoilChillerAirSourceVariableSpeedSpeedData> CoilChillerAirSourceVariableSpeed::speeds() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->speeds();
  }

  bool CoilChillerAirSourceVariableSpeed::addSpeed(const CoilChillerAirSourceVariableSpeedSpeedData& speed) {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->addSpeed(speed);
  }

  void CoilChillerAirSourceVariableSpeed::removeSpeed(const CoilChillerAirSourceVariableSpeedSpeedData& speed) {
    getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->removeSpeed(speed);
  }

  void CoilChillerAirSourceVariableSpeed::removeAllSpeeds() {
    getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->removeAllSpeeds();
  }

  boost::optional<double> CoilChillerAirSourceVariableSpeed::autosizedRatedChilledWaterCapacity() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->autosizedRatedChilledWaterCapacity();
  }

  boost::optional<double> CoilChillerAirSourceVariableSpeed::autocalculatedRatedEvaporatorWaterFlowRate() const {
    return getImpl<detail::CoilChillerAirSourceVariableSpeed_Impl>()->autocalculatedRatedEvaporatorWaterFlowRate();
  }

  /// @cond
  CoilChillerAirSourceVariableSpeed::CoilChillerAirSourceVariableSpeed(std::shared_ptr<detail::CoilChillerAirSourceVariableSpeed_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
