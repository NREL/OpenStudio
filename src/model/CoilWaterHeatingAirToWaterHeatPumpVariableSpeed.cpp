/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "CoilWaterHeatingAirToWaterHeatPumpVariableSpeed.hpp"
#include "CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl.hpp"

#include "CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData.hpp"
#include "CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "CoilSystemIntegratedHeatPumpAirSource_Impl.hpp"
#include "WaterHeaterHeatPump.hpp"
#include "WaterHeaterHeatPump_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_FieldEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl(const IdfObject& idfObject,
                                                                                                               Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::iddObjectType());
    }

    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::iddObjectType());
    }

    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl(
      const CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl& other, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Cooling Coil Electricity Energy",
        "Cooling Coil Sensible Cooling Energy",
        "Cooling Coil Latent Cooling Energy",
        "Cooling Coil Water Side Heat Transfer Energy",
        "Cooling Coil Cooling Energy",
        "Cooling Coil Water Heating Electricity Rate",
        "Cooling Coil Total Cooling Rate",
        "Cooling Coil Sensible Cooling Rate",
        "Cooling Coil Latent Cooling Rate",
        "Cooling Coil Total Water Heating Rate",
        "Cooling Coil Part Load Ratio",
        "Cooling Coil Runtime Fraction",
        "Cooling Coil Air Mass Flow Rate",
        "Cooling Coil Air Inlet Temperature",
        "Cooling Coil Air Inlet Humidity Ratio",
        "Cooling Coil Air Outlet Temperature",
        "Cooling Coil Air Outlet Humidity Ratio",
        "Cooling Coil Water Mass Flow Rate",
        "Cooling Coil Water Inlet Temperature",
        "Cooling Coil Water Outlet Temperature",
        "Cooling Coil Crankcase Heater Electricity Rate",
        "Cooling Coil Crankcase Heater Electricity Energy",
        "Cooling Coil Upper Speed Level",
        "Cooling Coil Neighboring Speed Levels Ratio",
        "Cooling Coil Water Heating Pump Electricity Rate",
        "Cooling Coil Water Heating Pump Electricity Energy",
      };
      return result;
    }

    IddObjectType CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::iddObjectType() const {
      return CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::iddObjectType();
    }

    std::vector<ModelObject> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::children() const {
      std::vector<ModelObject> result;

      if (OptionalCurve intermediate = optionalPartLoadFractionCorrelationCurve()) {
        result.push_back(*intermediate);
      }

      if (auto const _stageDataList = speedDataList()) {
        for (const auto& mo : _stageDataList->modelObjects()) {
          result.push_back(mo);
        }
      }
      return result;
    }

    ModelObject CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::clone(Model model) const {
      auto t_clone = ModelObject_Impl::clone(model).cast<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>();

      if (auto speedDataList_ = this->speedDataList()) {
        auto speedDataListClone = speedDataList_->clone(model).cast<ModelObjectList>();
        t_clone.getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setSpeedDataList(speedDataListClone);
      }

      return std::move(t_clone);
    }

    std::vector<IdfObject> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::remove() {
      auto speedDataList_ = speedDataList();
      auto result = HVACComponent_Impl::remove();
      if ((!result.empty()) && speedDataList_) {
        speedDataList_->remove();
      }

      return result;
    }

    boost::optional<HVACComponent> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::containingHVACComponent() const {
      // OS:WaterHeater:HeatPump (PumpedCondenser)
      {
        auto systems = this->model().getConcreteModelObjects<WaterHeaterHeatPump>();

        for (auto const& system : systems) {
          if (system.dXCoil().handle() == this->handle()) {
            return system;
          }
        }
      }

      // CoilSystemIntegratedHeatPumpAirSource
      {
        auto coilSystems = this->model().getConcreteModelObjects<CoilSystemIntegratedHeatPumpAirSource>();
        for (const auto& coilSystem : coilSystems) {
          if (coilSystem.dedicatedWaterHeatingCoil().handle() == this->handle()) {
            return coilSystem;
          }
          if (coilSystem.scwhCoil().handle() == this->handle()) {
            return coilSystem;
          }
          if (coilSystem.scdwhWaterHeatingCoil().handle() == this->handle()) {
            return coilSystem;
          }
          if (coilSystem.shdwhWaterHeatingCoil().handle() == this->handle()) {
            return coilSystem;
          }
        }
      }

      return boost::none;
    }

    void CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::autosize() {
      autocalculateRatedEvaporatorAirFlowRate();
      autocalculateRatedCondenserWaterFlowRate();
    }

    void CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autocalculatedRatedEvaporatorAirFlowRate();
      if (val) {
        setRatedEvaporatorAirFlowRate(val.get());
      }

      val = autocalculatedRatedCondenserWaterFlowRate();
      if (val) {
        setRatedCondenserWaterFlowRate(val.get());
      }
    }

    int CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::nominalSpeedLevel() const {
      boost::optional<int> value = getInt(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::NominalSpeedLevel, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::ratedWaterHeatingCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedWaterHeatingCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::ratedEvaporatorInletAirDryBulbTemperature() const {
      boost::optional<double> value =
        getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorInletAirDryBulbTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::ratedEvaporatorInletAirWetBulbTemperature() const {
      boost::optional<double> value =
        getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorInletAirWetBulbTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::ratedCondenserInletWaterTemperature() const {
      boost::optional<double> value =
        getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCondenserInletWaterTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::ratedEvaporatorAirFlowRate() const {
      return getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorAirFlowRate, true);
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::isRatedEvaporatorAirFlowRateAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autocalculate");
      }
      return result;
    }

    boost::optional<double> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::ratedCondenserWaterFlowRate() const {
      return getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCondenserWaterFlowRate, true);
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::isRatedCondenserWaterFlowRateAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCondenserWaterFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autocalculate");
      }
      return result;
    }

    std::string CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::evaporatorFanPowerIncludedinRatedCOP() const {
      boost::optional<std::string> value =
        getString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorFanPowerIncludedinRatedCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::condenserPumpPowerIncludedinRatedCOP() const {
      boost::optional<std::string> value =
        getString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CondenserPumpPowerIncludedinRatedCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP() const {
      boost::optional<std::string> value =
        getString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::fractionofCondenserPumpHeattoWater() const {
      boost::optional<double> value =
        getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::FractionofCondenserPumpHeattoWater, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::crankcaseHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CrankcaseHeaterCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::maximumAmbientTemperatureforCrankcaseHeaterOperation() const {
      boost::optional<double> value =
        getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::evaporatorAirTemperatureTypeforCurveObjects() const {
      boost::optional<std::string> value =
        getString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorAirTemperatureTypeforCurveObjects, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::optionalPartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::PartLoadFractionCorrelationCurveName);
    }

    Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::partLoadFractionCorrelationCurve() const {
      boost::optional<Curve> value = optionalPartLoadFractionCorrelationCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a Part Load Fraction Correlation Curve attached.");
      }
      return value.get();
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setNominalSpeedLevel(int nominalSpeedLevel) {
      bool result = setInt(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::NominalSpeedLevel, nominalSpeedLevel);
      OS_ASSERT(result);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setRatedWaterHeatingCapacity(double ratedWaterHeatingCapacity) {
      bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedWaterHeatingCapacity, ratedWaterHeatingCapacity);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setRatedEvaporatorInletAirDryBulbTemperature(
      double ratedEvaporatorInletAirDryBulbTemperature) {
      bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorInletAirDryBulbTemperature,
                              ratedEvaporatorInletAirDryBulbTemperature);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setRatedEvaporatorInletAirWetBulbTemperature(
      double ratedEvaporatorInletWetDryBulbTemperature) {
      bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorInletAirWetBulbTemperature,
                              ratedEvaporatorInletWetDryBulbTemperature);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setRatedCondenserInletWaterTemperature(double ratedCondenserInletWaterTemperature) {
      bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCondenserInletWaterTemperature,
                              ratedCondenserInletWaterTemperature);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setRatedEvaporatorAirFlowRate(boost::optional<double> ratedEvaporatorAirFlowRate) {
      bool result(false);
      if (ratedEvaporatorAirFlowRate) {
        result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorAirFlowRate, ratedEvaporatorAirFlowRate.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::autocalculateRatedEvaporatorAirFlowRate() {
      bool result = setString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorAirFlowRate, "autocalculate");
      OS_ASSERT(result);
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setRatedCondenserWaterFlowRate(boost::optional<double> ratedCondenserWaterFlowRate) {
      bool result(false);
      if (ratedCondenserWaterFlowRate) {
        result =
          setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCondenserWaterFlowRate, ratedCondenserWaterFlowRate.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::autocalculateRatedCondenserWaterFlowRate() {
      bool result = setString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCondenserWaterFlowRate, "autocalculate");
      OS_ASSERT(result);
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setEvaporatorFanPowerIncludedinRatedCOP(
      const std::string& evaporatorFanPowerIncludedinRatedCOP) {
      bool result = setString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorFanPowerIncludedinRatedCOP,
                              evaporatorFanPowerIncludedinRatedCOP);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setCondenserPumpPowerIncludedinRatedCOP(
      const std::string& condenserPumpPowerIncludedinRatedCOP) {
      bool result = setString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CondenserPumpPowerIncludedinRatedCOP,
                              condenserPumpPowerIncludedinRatedCOP);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(
      const std::string& condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP) {
      bool result = setString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP,
                              condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setFractionofCondenserPumpHeattoWater(double fractionofCondenserPumpHeattoWater) {
      bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::FractionofCondenserPumpHeattoWater,
                              fractionofCondenserPumpHeattoWater);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
      bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CrankcaseHeaterCapacity, crankcaseHeaterCapacity);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setMaximumAmbientTemperatureforCrankcaseHeaterOperation(
      double maximumAmbientTemperatureforCrankcaseHeaterOperation) {
      bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation,
                              maximumAmbientTemperatureforCrankcaseHeaterOperation);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setEvaporatorAirTemperatureTypeforCurveObjects(
      const std::string& evaporatorAirTemperatureTypeforCurveObjects) {
      bool result = setString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorAirTemperatureTypeforCurveObjects,
                              evaporatorAirTemperatureTypeforCurveObjects);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setPartLoadFractionCorrelationCurve(const Curve& partLoadFractionCorrelationCurve) {
      bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::PartLoadFractionCorrelationCurveName,
                               partLoadFractionCorrelationCurve.handle());
      OS_ASSERT(result);
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setSpeedDataList(const boost::optional<ModelObjectList>& modelObjectList) {
      bool result(false);
      if (modelObjectList) {
        result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::SpeedDataList, modelObjectList.get().handle());
      } else {
        resetSpeedDataList();
        result = true;
      }
      return result;
    }

    void CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::resetSpeedDataList() {
      bool result = setString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::SpeedDataList, "");
      OS_ASSERT(result);
    }

    boost::optional<ModelObjectList> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::speedDataList() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(
        OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::SpeedDataList);
    }

    std::vector<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::speeds() const {
      std::vector<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData> result;
      auto const modelObjectList = speedDataList();
      if (modelObjectList) {
        auto const modelObjects = modelObjectList->modelObjects();

        for (const auto& elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData>();
          if (modelObject) {
            result.push_back(modelObject.get());
          }
        }
      }
      return result;
    }

    bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::addSpeed(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData& speed) {
      bool result = false;
      auto modelObjectList = speedDataList();
      if (modelObjectList) {
        result = modelObjectList->addModelObject(speed);
      }
      return result;
    }

    void CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::removeSpeed(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData& speed) {
      auto modelObjectList = speedDataList();
      if (modelObjectList) {
        modelObjectList->removeModelObject(speed);
      }
    }

    void CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::removeAllSpeeds() {
      auto modelObjectList = speedDataList();
      if (modelObjectList) {
        auto const modelObjects = modelObjectList->modelObjects();

        for (const auto& elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData>();
          if (modelObject) {
            modelObjectList->removeModelObject(elem);
          }
        }
      }
    }

    boost::optional<double> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::autocalculatedRatedEvaporatorAirFlowRate() const {
      // TODO: pending NREL/EnergyPlus#8611
      return getAutosizedValue("TODO", "W");
    }

    boost::optional<double> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::autocalculatedRatedCondenserWaterFlowRate() const {
      // TODO: pending NREL/EnergyPlus#8611
      return getAutosizedValue("TODO", "m3/s");
    }

    ComponentType CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::heatingFuelTypes() const {
      return {FuelType::Electricity};
    }

    std::vector<AppGFuelType> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::appGHeatingFuelTypes() const {
      return {AppGFuelType::HeatPump};  // TODO: openstudio-standards uses Electric
    }

  }  // namespace detail

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed(const Model& model)
    : HVACComponent(CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>());

    CurveQuadratic partLoadFraction = CurveQuadratic(model);
    partLoadFraction.setCoefficient1Constant(0.75);
    partLoadFraction.setCoefficient2x(0.25);
    partLoadFraction.setCoefficient3xPOW2(0.0);
    partLoadFraction.setMinimumValueofx(0.0);
    partLoadFraction.setMaximumValueofx(1.0);

    bool ok = true;
    ok = setNominalSpeedLevel(1);
    OS_ASSERT(ok);
    ok = setRatedWaterHeatingCapacity(4000.0);  // ASIHPMixedTank.idf
    OS_ASSERT(ok);
    // Values here are not IDD defaults or the Curve won't evaluate to 1.0 +- 0.1 at rated conditions
    ok = setRatedEvaporatorInletAirDryBulbTemperature(29.44);  // ASIHPMixedTank.idf, not 19.7 idd default
    OS_ASSERT(ok);
    ok = setRatedEvaporatorInletAirWetBulbTemperature(22.22);  // ASIHPMixedTank.idf, not 13.5 idd default
    OS_ASSERT(ok);
    ok = setRatedCondenserInletWaterTemperature(55.72);  // ASIHPMixedTank.idf, not 57.5 idd default
    OS_ASSERT(ok);
    autocalculateRatedEvaporatorAirFlowRate();
    autocalculateRatedCondenserWaterFlowRate();
    ok = setEvaporatorFanPowerIncludedinRatedCOP("Yes");
    OS_ASSERT(ok);
    ok = setCondenserPumpPowerIncludedinRatedCOP("No");
    OS_ASSERT(ok);
    ok = setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP("No");
    OS_ASSERT(ok);
    ok = setFractionofCondenserPumpHeattoWater(0.2);
    OS_ASSERT(ok);
    ok = setCrankcaseHeaterCapacity(0);
    OS_ASSERT(ok);
    ok = setMaximumAmbientTemperatureforCrankcaseHeaterOperation(10);
    OS_ASSERT(ok);
    ok = setEvaporatorAirTemperatureTypeforCurveObjects("WetBulbTemperature");
    OS_ASSERT(ok);
    ok = setPartLoadFractionCorrelationCurve(partLoadFraction);  // ASIHPMixedTank.idf
    OS_ASSERT(ok);

    auto speedDataList = ModelObjectList(model);
    speedDataList.setName(this->name().get() + " Speed Data List");
    ok = getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setSpeedDataList(speedDataList);
    OS_ASSERT(ok);
  }

  IddObjectType CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::iddObjectType() {
    return {IddObjectType::OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed};
  }

  int CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::nominalSpeedLevel() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->nominalSpeedLevel();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::ratedWaterHeatingCapacity() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->ratedWaterHeatingCapacity();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::ratedEvaporatorInletAirDryBulbTemperature() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->ratedEvaporatorInletAirDryBulbTemperature();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::ratedEvaporatorInletAirWetBulbTemperature() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->ratedEvaporatorInletAirWetBulbTemperature();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::ratedCondenserInletWaterTemperature() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->ratedCondenserInletWaterTemperature();
  }

  boost::optional<double> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::ratedEvaporatorAirFlowRate() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->ratedEvaporatorAirFlowRate();
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::isRatedEvaporatorAirFlowRateAutocalculated() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->isRatedEvaporatorAirFlowRateAutocalculated();
  }

  boost::optional<double> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::ratedCondenserWaterFlowRate() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->ratedCondenserWaterFlowRate();
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::isRatedCondenserWaterFlowRateAutocalculated() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->isRatedCondenserWaterFlowRateAutocalculated();
  }

  std::string CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::evaporatorFanPowerIncludedinRatedCOP() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->evaporatorFanPowerIncludedinRatedCOP();
  }

  std::string CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::condenserPumpPowerIncludedinRatedCOP() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->condenserPumpPowerIncludedinRatedCOP();
  }

  std::string CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::fractionofCondenserPumpHeattoWater() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->fractionofCondenserPumpHeattoWater();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::crankcaseHeaterCapacity() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->crankcaseHeaterCapacity();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::maximumAmbientTemperatureforCrankcaseHeaterOperation() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->maximumAmbientTemperatureforCrankcaseHeaterOperation();
  }

  std::string CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::evaporatorAirTemperatureTypeforCurveObjects() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->evaporatorAirTemperatureTypeforCurveObjects();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::partLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->partLoadFractionCorrelationCurve();
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setNominalSpeedLevel(int nominalSpeedLevel) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setNominalSpeedLevel(nominalSpeedLevel);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setRatedWaterHeatingCapacity(double ratedWaterHeatingCapacity) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setRatedWaterHeatingCapacity(ratedWaterHeatingCapacity);
  }

  bool
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setRatedEvaporatorInletAirDryBulbTemperature(
      ratedEvaporatorInletAirDryBulbTemperature);
  }

  bool
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletWetDryBulbTemperature) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setRatedEvaporatorInletAirWetBulbTemperature(
      ratedEvaporatorInletWetDryBulbTemperature);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setRatedCondenserInletWaterTemperature(double ratedCondenserInletWaterTemperature) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setRatedCondenserInletWaterTemperature(
      ratedCondenserInletWaterTemperature);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setRatedEvaporatorAirFlowRate(ratedEvaporatorAirFlowRate);
  }

  void CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::autocalculateRatedEvaporatorAirFlowRate() {
    getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->autocalculateRatedEvaporatorAirFlowRate();
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setRatedCondenserWaterFlowRate(double ratedCondenserWaterFlowRate) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setRatedCondenserWaterFlowRate(ratedCondenserWaterFlowRate);
  }

  void CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::autocalculateRatedCondenserWaterFlowRate() {
    getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->autocalculateRatedCondenserWaterFlowRate();
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setEvaporatorFanPowerIncludedinRatedCOP(
    const std::string& evaporatorFanPowerIncludedinRatedCOP) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setEvaporatorFanPowerIncludedinRatedCOP(
      evaporatorFanPowerIncludedinRatedCOP);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setCondenserPumpPowerIncludedinRatedCOP(
    const std::string& condenserPumpPowerIncludedinRatedCOP) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setCondenserPumpPowerIncludedinRatedCOP(
      condenserPumpPowerIncludedinRatedCOP);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(
    const std::string& condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(
      condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setFractionofCondenserPumpHeattoWater(double fractionofCondenserPumpHeattoWater) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setFractionofCondenserPumpHeattoWater(
      fractionofCondenserPumpHeattoWater);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setCrankcaseHeaterCapacity(crankcaseHeaterCapacity);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setMaximumAmbientTemperatureforCrankcaseHeaterOperation(
    double maximumAmbientTemperatureforCrankcaseHeaterOperation) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setMaximumAmbientTemperatureforCrankcaseHeaterOperation(
      maximumAmbientTemperatureforCrankcaseHeaterOperation);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setEvaporatorAirTemperatureTypeforCurveObjects(
    const std::string& evaporatorAirTemperatureTypeforCurveObjects) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setEvaporatorAirTemperatureTypeforCurveObjects(
      evaporatorAirTemperatureTypeforCurveObjects);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setPartLoadFractionCorrelationCurve(const Curve& partLoadFractionCorrelationCurve) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setPartLoadFractionCorrelationCurve(
      partLoadFractionCorrelationCurve);
  }

  std::vector<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::speeds() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->speeds();
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::addSpeed(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData& speed) {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->addSpeed(speed);
  }

  void CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::removeSpeed(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData& speed) {
    getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->removeSpeed(speed);
  }

  void CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::removeAllSpeeds() {
    getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->removeAllSpeeds();
  }

  boost::optional<double> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::autocalculatedRatedEvaporatorAirFlowRate() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->autocalculatedRatedEvaporatorAirFlowRate();
  }

  boost::optional<double> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::autocalculatedRatedCondenserWaterFlowRate() const {
    return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->autocalculatedRatedCondenserWaterFlowRate();
  }

  /// @cond
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed(
    std::shared_ptr<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl> impl)
    : HVACComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
