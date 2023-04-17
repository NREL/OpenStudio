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

#include "RefrigerationCompressorRack.hpp"
#include "RefrigerationCompressorRack_Impl.hpp"

#include "RefrigerationAirChiller.hpp"
#include "RefrigerationAirChiller_Impl.hpp"
#include "RefrigerationCase.hpp"
#include "RefrigerationCase_Impl.hpp"
#include "RefrigerationWalkIn.hpp"
#include "RefrigerationWalkIn_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Node.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Refrigeration_CompressorRack_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    RefrigerationCompressorRack_Impl::RefrigerationCompressorRack_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == RefrigerationCompressorRack::iddObjectType());
    }

    RefrigerationCompressorRack_Impl::RefrigerationCompressorRack_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == RefrigerationCompressorRack::iddObjectType());
    }

    RefrigerationCompressorRack_Impl::RefrigerationCompressorRack_Impl(const RefrigerationCompressorRack_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    unsigned RefrigerationCompressorRack_Impl::inletPort() const {
      return OS_Refrigeration_CompressorRackFields::WaterCooledCondenserInletNodeName;
    }

    unsigned RefrigerationCompressorRack_Impl::outletPort() const {
      return OS_Refrigeration_CompressorRackFields::WaterCooledCondenserOutletNodeName;
    }

    const std::vector<std::string>& RefrigerationCompressorRack_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Refrigeration Compressor Rack Electricity Rate",
                                                   "Refrigeration Compressor Rack Electricity Energy",
                                                   "Refrigeration Compressor Rack Condenser Fan Electricity Rate",
                                                   "Refrigeration Compressor Rack Condenser Fan Electricity Energy",
                                                   "Refrigeration Compressor Rack Total Heat Transfer Rate",
                                                   "Refrigeration Compressor Rack Total Heat Transfer Energy",
                                                   "Refrigeration Compressor Rack COP"};
      return result;
    }

    IddObjectType RefrigerationCompressorRack_Impl::iddObjectType() const {
      return RefrigerationCompressorRack::iddObjectType();
    }

    std::vector<ScheduleTypeKey> RefrigerationCompressorRack_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Refrigeration_CompressorRackFields::WaterCooledCondenserOutletTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("RefrigerationCompressorRack", "Water Cooled Condenser Outlet Temperature"));
      }
      if (std::find(b, e, OS_Refrigeration_CompressorRackFields::EvaporativeCondenserAvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("RefrigerationCompressorRack", "Evaporative Condenser Availability"));
      }
      return result;
    }

    bool RefrigerationCompressorRack_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->demandComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    ModelObject RefrigerationCompressorRack_Impl::clone(Model model) const {
      auto modelObjectClone = StraightComponent_Impl::clone(model).cast<RefrigerationCompressorRack>();

      if (boost::optional<ModelObjectList> caseAndWalkinList = this->refrigeratedCaseAndWalkInList()) {
        auto caseAndWalkinListClone = caseAndWalkinList->clone(model).cast<ModelObjectList>();
        modelObjectClone.getImpl<detail::RefrigerationCompressorRack_Impl>()->setRefrigeratedCaseAndWalkInList(caseAndWalkinListClone);
      }

      modelObjectClone.resetHeatRejectionZone();

      return std::move(modelObjectClone);
    }

    std::vector<IddObjectType> RefrigerationCompressorRack_Impl::allowableChildTypes() const {
      std::vector<IddObjectType> result;
      result.push_back(IddObjectType::OS_Curve_Quadratic);
      result.push_back(IddObjectType::OS_Curve_Cubic);
      return result;
    }

    std::vector<ModelObject> RefrigerationCompressorRack_Impl::children() const {
      std::vector<ModelObject> result;
      if (boost::optional<Curve> intermediate = optionalCompressorRackCOPFunctionofTemperatureCurve()) {
        result.push_back(*intermediate);
      }
      if (boost::optional<Curve> intermediate = condenserFanPowerFunctionofTemperatureCurve()) {
        result.push_back(*intermediate);
      }
      return result;
    }

    std::vector<IdfObject> RefrigerationCompressorRack_Impl::remove() {
      std::vector<IdfObject> result;

      if (boost::optional<ModelObjectList> caseAndWalkinList = this->refrigeratedCaseAndWalkInList()) {
        std::vector<IdfObject> removedCasesAndWalkins = caseAndWalkinList->remove();
        result.insert(result.end(), removedCasesAndWalkins.begin(), removedCasesAndWalkins.end());
      }

      std::vector<IdfObject> removedRefrigerationCompressorRack = StraightComponent_Impl::remove();
      result.insert(result.end(), removedRefrigerationCompressorRack.begin(), removedRefrigerationCompressorRack.end());

      return result;
    }

    std::string RefrigerationCompressorRack_Impl::heatRejectionLocation() const {
      boost::optional<std::string> value = getString(OS_Refrigeration_CompressorRackFields::HeatRejectionLocation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double RefrigerationCompressorRack_Impl::designCompressorRackCOP() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_CompressorRackFields::DesignCompressorRackCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    Curve RefrigerationCompressorRack_Impl::compressorRackCOPFunctionofTemperatureCurve() const {
      OptionalCurve result = optionalCompressorRackCOPFunctionofTemperatureCurve();
      OS_ASSERT(result);
      return result.get();
    }

    boost::optional<Curve> RefrigerationCompressorRack_Impl::optionalCompressorRackCOPFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Refrigeration_CompressorRackFields::CompressorRackCOPFunctionofTemperatureCurveName);
    }

    double RefrigerationCompressorRack_Impl::designCondenserFanPower() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_CompressorRackFields::DesignCondenserFanPower, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> RefrigerationCompressorRack_Impl::condenserFanPowerFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Refrigeration_CompressorRackFields::CondenserFanPowerFunctionofTemperatureCurveName);
    }

    std::string RefrigerationCompressorRack_Impl::condenserType() const {
      boost::optional<std::string> value = getString(OS_Refrigeration_CompressorRackFields::CondenserType, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string RefrigerationCompressorRack_Impl::waterCooledLoopFlowType() const {
      boost::optional<std::string> value = getString(OS_Refrigeration_CompressorRackFields::WaterCooledLoopFlowType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> RefrigerationCompressorRack_Impl::waterCooledCondenserOutletTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_Refrigeration_CompressorRackFields::WaterCooledCondenserOutletTemperatureScheduleName);
    }

    boost::optional<double> RefrigerationCompressorRack_Impl::waterCooledCondenserDesignFlowRate() const {
      return getDouble(OS_Refrigeration_CompressorRackFields::WaterCooledCondenserDesignFlowRate, true);
    }

    boost::optional<double> RefrigerationCompressorRack_Impl::waterCooledCondenserMaximumFlowRate() const {
      return getDouble(OS_Refrigeration_CompressorRackFields::WaterCooledCondenserMaximumFlowRate, true);
    }

    double RefrigerationCompressorRack_Impl::waterCooledCondenserMaximumWaterOutletTemperature() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_CompressorRackFields::WaterCooledCondenserMaximumWaterOutletTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double RefrigerationCompressorRack_Impl::waterCooledCondenserMinimumWaterInletTemperature() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_CompressorRackFields::WaterCooledCondenserMinimumWaterInletTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> RefrigerationCompressorRack_Impl::evaporativeCondenserAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_Refrigeration_CompressorRackFields::EvaporativeCondenserAvailabilityScheduleName);
    }

    double RefrigerationCompressorRack_Impl::evaporativeCondenserEffectiveness() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_CompressorRackFields::EvaporativeCondenserEffectiveness, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> RefrigerationCompressorRack_Impl::evaporativeCondenserAirFlowRate() const {
      return getDouble(OS_Refrigeration_CompressorRackFields::EvaporativeCondenserAirFlowRate, true);
    }

    bool RefrigerationCompressorRack_Impl::isEvaporativeCondenserAirFlowRateAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Refrigeration_CompressorRackFields::EvaporativeCondenserAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autocalculate");
      }
      return result;
    }

    double RefrigerationCompressorRack_Impl::basinHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_CompressorRackFields::BasinHeaterCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double RefrigerationCompressorRack_Impl::basinHeaterSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_CompressorRackFields::BasinHeaterSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> RefrigerationCompressorRack_Impl::designEvaporativeCondenserWaterPumpPower() const {
      return getDouble(OS_Refrigeration_CompressorRackFields::DesignEvaporativeCondenserWaterPumpPower, true);
    }

    bool RefrigerationCompressorRack_Impl::isDesignEvaporativeCondenserWaterPumpPowerAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Refrigeration_CompressorRackFields::DesignEvaporativeCondenserWaterPumpPower, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autocalculate");
      }
      return result;
    }

    // boost::optional<HVACComponent> RefrigerationCompressorRack_Impl::evaporativeWaterSupplyTank() const {

    // }

    // boost::optional<std::string> RefrigerationCompressorRack_Impl::condenserAirInletNodeName() const {

    // }

    std::string RefrigerationCompressorRack_Impl::endUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_Refrigeration_CompressorRackFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefrigerationCompressorRack_Impl::isEndUseSubcategoryDefaulted() const {
      return isEmpty(OS_Refrigeration_CompressorRackFields::EndUseSubcategory);
    }

    template <class T>
    std::vector<T> RefrigerationCompressorRack_Impl::listTemplate(const boost::optional<ModelObjectList>& modelObjectList) const {
      std::vector<T> result;

      if (modelObjectList) {
        std::vector<ModelObject> modelObjects = modelObjectList->modelObjects();

        for (const auto& elem : modelObjects) {
          boost::optional<T> modelObject = elem.optionalCast<T>();
          if (modelObject) {
            result.push_back(modelObject.get());
          }
        }
      }

      return result;
    }

    std::vector<RefrigerationCase> RefrigerationCompressorRack_Impl::cases() const {
      return RefrigerationCompressorRack_Impl::listTemplate<RefrigerationCase>(refrigeratedCaseAndWalkInList());
    }

    std::vector<RefrigerationWalkIn> RefrigerationCompressorRack_Impl::walkins() const {
      return RefrigerationCompressorRack_Impl::listTemplate<RefrigerationWalkIn>(refrigeratedCaseAndWalkInList());
    }

    std::vector<RefrigerationAirChiller> RefrigerationCompressorRack_Impl::airChillers() const {
      return RefrigerationCompressorRack_Impl::listTemplate<RefrigerationAirChiller>(refrigeratedCaseAndWalkInList());
    }

    boost::optional<ModelObjectList> RefrigerationCompressorRack_Impl::refrigeratedCaseAndWalkInList() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Refrigeration_CompressorRackFields::RefrigeratedCaseAndWalkInListName);
    }

    boost::optional<ThermalZone> RefrigerationCompressorRack_Impl::heatRejectionZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Refrigeration_CompressorRackFields::HeatRejectionZoneName);
    }

    bool RefrigerationCompressorRack_Impl::setHeatRejectionLocation(const std::string& heatRejectionLocation) {
      bool result = setString(OS_Refrigeration_CompressorRackFields::HeatRejectionLocation, heatRejectionLocation);
      OS_ASSERT(result);
      return result;
    }

    bool RefrigerationCompressorRack_Impl::setDesignCompressorRackCOP(double designCompressorRackCOP) {
      bool result = setDouble(OS_Refrigeration_CompressorRackFields::DesignCompressorRackCOP, designCompressorRackCOP);
      OS_ASSERT(result);
      return result;
    }

    bool RefrigerationCompressorRack_Impl::setCompressorRackCOPFunctionofTemperatureCurve(const Curve& curve) {
      bool result = this->setPointer(OS_Refrigeration_CompressorRackFields::CompressorRackCOPFunctionofTemperatureCurveName, curve.handle());
      OS_ASSERT(result);
      return result;
    }

    bool RefrigerationCompressorRack_Impl::setDesignCondenserFanPower(double designCondenserFanPower) {
      bool result = setDouble(OS_Refrigeration_CompressorRackFields::DesignCondenserFanPower, designCondenserFanPower);
      OS_ASSERT(result);
      return result;
    }

    bool RefrigerationCompressorRack_Impl::setCondenserFanPowerFunctionofTemperatureCurve(const Curve& curve) {
      bool result = this->setPointer(OS_Refrigeration_CompressorRackFields::CondenserFanPowerFunctionofTemperatureCurveName, curve.handle());
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetCondenserFanPowerFunctionofTemperatureCurve() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::CondenserFanPowerFunctionofTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCompressorRack_Impl::setCondenserType(const std::string& condenserType) {
      bool result = setString(OS_Refrigeration_CompressorRackFields::CondenserType, condenserType);
      OS_ASSERT(result);
      return result;
    }

    bool RefrigerationCompressorRack_Impl::setWaterCooledLoopFlowType(const std::string& waterCooledLoopFlowType) {
      bool result = setString(OS_Refrigeration_CompressorRackFields::WaterCooledLoopFlowType, waterCooledLoopFlowType);
      OS_ASSERT(result);
      return result;
    }

    bool RefrigerationCompressorRack_Impl::setWaterCooledCondenserOutletTemperatureSchedule(Schedule& schedule) {
      bool result = this->setPointer(OS_Refrigeration_CompressorRackFields::WaterCooledCondenserOutletTemperatureScheduleName, schedule.handle());
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetWaterCooledCondenserOutletTemperatureSchedule() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::WaterCooledCondenserOutletTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCompressorRack_Impl::setWaterCooledCondenserDesignFlowRate(double waterCooledCondenserDesignFlowRate) {
      bool result = setDouble(OS_Refrigeration_CompressorRackFields::WaterCooledCondenserDesignFlowRate, waterCooledCondenserDesignFlowRate);
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetWaterCooledCondenserDesignFlowRate() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::WaterCooledCondenserDesignFlowRate, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCompressorRack_Impl::setWaterCooledCondenserMaximumFlowRate(double waterCooledCondenserMaximumFlowRate) {
      bool result = setDouble(OS_Refrigeration_CompressorRackFields::WaterCooledCondenserMaximumFlowRate, waterCooledCondenserMaximumFlowRate);
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetWaterCooledCondenserMaximumFlowRate() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::WaterCooledCondenserMaximumFlowRate, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCompressorRack_Impl::setWaterCooledCondenserMaximumWaterOutletTemperature(
      double waterCooledCondenserMaximumWaterOutletTemperature) {
      bool result = setDouble(OS_Refrigeration_CompressorRackFields::WaterCooledCondenserMaximumWaterOutletTemperature,
                              waterCooledCondenserMaximumWaterOutletTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool
      RefrigerationCompressorRack_Impl::setWaterCooledCondenserMinimumWaterInletTemperature(double waterCooledCondenserMinimumWaterInletTemperature) {
      bool result = setDouble(OS_Refrigeration_CompressorRackFields::WaterCooledCondenserMinimumWaterInletTemperature,
                              waterCooledCondenserMinimumWaterInletTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool RefrigerationCompressorRack_Impl::setEvaporativeCondenserAvailabilitySchedule(Schedule& schedule) {
      bool result = this->setPointer(OS_Refrigeration_CompressorRackFields::EvaporativeCondenserAvailabilityScheduleName, schedule.handle());
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetEvaporativeCondenserAvailabilitySchedule() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::EvaporativeCondenserAvailabilityScheduleName, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCompressorRack_Impl::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
      bool result = setDouble(OS_Refrigeration_CompressorRackFields::EvaporativeCondenserEffectiveness, evaporativeCondenserEffectiveness);
      OS_ASSERT(result);
      return result;
    }

    bool RefrigerationCompressorRack_Impl::setEvaporativeCondenserAirFlowRate(double evaporativeCondenserAirFlowRate) {
      bool result = setDouble(OS_Refrigeration_CompressorRackFields::EvaporativeCondenserAirFlowRate, evaporativeCondenserAirFlowRate);
      OS_ASSERT(result);
      return result;
    }

    bool RefrigerationCompressorRack_Impl::autocalculateEvaporativeCondenserAirFlowRate() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::EvaporativeCondenserAirFlowRate, "Autocalculate");
      OS_ASSERT(result);
      return result;
    }

    bool RefrigerationCompressorRack_Impl::setBasinHeaterCapacity(double basinHeaterCapacity) {
      bool result = setDouble(OS_Refrigeration_CompressorRackFields::BasinHeaterCapacity, basinHeaterCapacity);
      OS_ASSERT(result);
      return result;
    }

    bool RefrigerationCompressorRack_Impl::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
      bool result = setDouble(OS_Refrigeration_CompressorRackFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool RefrigerationCompressorRack_Impl::setDesignEvaporativeCondenserWaterPumpPower(double designEvaporativeCondenserWaterPumpPower) {
      bool result =
        setDouble(OS_Refrigeration_CompressorRackFields::DesignEvaporativeCondenserWaterPumpPower, designEvaporativeCondenserWaterPumpPower);
      OS_ASSERT(result);
      return result;
    }

    bool RefrigerationCompressorRack_Impl::autocalculateDesignEvaporativeCondenserWaterPumpPower() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::DesignEvaporativeCondenserWaterPumpPower, "Autocalculate");
      OS_ASSERT(result);
      return result;
    }

    // bool RefrigerationCompressorRack_Impl::setEvaporativeWaterSupplyTank(const HVACComponent& evaporativeWaterSupplyTank) {

    // }

    // void RefrigerationCompressorRack_Impl::resetEvaporativeWaterSupplyTank() {

    // }

    // bool RefrigerationCompressorRack_Impl::setCondenserAirInletNodeName(const boost::optional<std::string>& condenserAirInletNodeName) {

    // }

    // void RefrigerationCompressorRack_Impl::resetCondenserAirInletNodeName() {

    // }

    bool RefrigerationCompressorRack_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      bool result = setString(OS_Refrigeration_CompressorRackFields::EndUseSubcategory, endUseSubcategory);
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetEndUseSubcategory() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::EndUseSubcategory, "");
      OS_ASSERT(result);
    }

    template <class T>
    void RefrigerationCompressorRack_Impl::removeAllTemplate(boost::optional<ModelObjectList>& modelObjectList) {
      if (modelObjectList) {
        std::vector<ModelObject> modelObjects = modelObjectList->modelObjects();

        for (const auto& elem : modelObjects) {
          boost::optional<T> modelObject = elem.optionalCast<T>();
          if (modelObject) {
            modelObjectList->removeModelObject(elem);
          }
        }
      }
    }

    template <class T>
    void RefrigerationCompressorRack_Impl::removeTemplate(const T& modelObject, boost::optional<ModelObjectList>& modelObjectList) {
      if (modelObjectList) {
        modelObjectList->removeModelObject(modelObject);
      }
    }

    template <class T>
    bool RefrigerationCompressorRack_Impl::addTemplate(const T& modelObject, boost::optional<ModelObjectList>& modelObjectList) {
      if (modelObjectList) {
        return modelObjectList->addModelObject(modelObject);
      }
      return false;
    }

    bool RefrigerationCompressorRack_Impl::addCase(const RefrigerationCase& refrigerationCase) {
      // From 9.2 IO ref:
      // > This list may contain a combination of case and walk-in names OR a list of air chiller names.
      // > Air chillers may not be included in any list that also includes cases or walk-ins.
      if (!this->airChillers().empty()) {
        LOG(Warn, "You cannot mix RefigerationCase/RefrigerationWalkins with RefrigerationAirChillers, occurred for " << briefDescription());
        return false;
      }

      // Enforce unicity
      if (boost::optional<RefrigerationCompressorRack> currentCompressorRack = refrigerationCase.compressorRack()) {
        LOG(Warn, refrigerationCase.briefDescription()
                    << " was removed from its existing RefrigerationCompressorRack named '" << currentCompressorRack->nameString() << "'.");
        currentCompressorRack->removeCase(refrigerationCase);
      }

      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      return addTemplate<RefrigerationCase>(refrigerationCase, modelObjectList);
    }

    void RefrigerationCompressorRack_Impl::removeCase(const RefrigerationCase& refrigerationCase) {
      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      removeTemplate<RefrigerationCase>(refrigerationCase, modelObjectList);
    }

    void RefrigerationCompressorRack_Impl::removeAllCases() {
      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      removeAllTemplate<RefrigerationCase>(modelObjectList);
    }

    bool RefrigerationCompressorRack_Impl::addWalkin(const RefrigerationWalkIn& refrigerationWalkin) {
      // From 9.2 IO ref:
      // > This list may contain a combination of case and walk-in names OR a list of air chiller names.
      // > Air chillers may not be included in any list that also includes cases or walk-ins.
      if (!this->airChillers().empty()) {
        LOG(Warn, "You cannot mix RefigerationCase/RefrigerationWalkins with RefrigerationAirChillers, occurred for " << briefDescription());
        return false;
      }

      // Enforce unicity
      if (boost::optional<RefrigerationCompressorRack> currentCompressorRack = refrigerationWalkin.compressorRack()) {
        LOG(Warn, refrigerationWalkin.briefDescription()
                    << " was removed from its existing RefrigerationCompressorRack named '" << currentCompressorRack->nameString() << "'.");
        currentCompressorRack->removeWalkin(refrigerationWalkin);
      }

      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      return addTemplate<RefrigerationWalkIn>(refrigerationWalkin, modelObjectList);
    }

    void RefrigerationCompressorRack_Impl::removeWalkin(const RefrigerationWalkIn& refrigerationWalkin) {
      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      removeTemplate<RefrigerationWalkIn>(refrigerationWalkin, modelObjectList);
    }

    void RefrigerationCompressorRack_Impl::removeAllWalkins() {
      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      removeAllTemplate<RefrigerationWalkIn>(modelObjectList);
    }

    bool RefrigerationCompressorRack_Impl::addAirChiller(const RefrigerationAirChiller& refrigerationAirChiller) {
      // From 9.2 IO ref:
      // > This list may contain a combination of case and walk-in names OR a list of air chiller names.
      // > Air chillers may not be included in any list that also includes cases or walk-ins.
      if (boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList()) {
        std::vector<ModelObject> modelObjects = modelObjectList->modelObjects();

        for (const auto& elem : modelObjects) {
          if (elem.optionalCast<RefrigerationWalkIn>() || elem.optionalCast<RefrigerationCase>()) {
            LOG(Warn, "You cannot mix RefigerationCase/RefrigerationWalkins with RefrigerationAirChillers, occurred for " << briefDescription());
            return false;
          }
        }
      }

      // Enforce unicity
      if (boost::optional<RefrigerationCompressorRack> currentCompressorRack = refrigerationAirChiller.compressorRack()) {
        LOG(Warn, refrigerationAirChiller.briefDescription()
                    << " was removed from its existing RefrigerationCompressorRack named '" << currentCompressorRack->nameString() << "'.");
        currentCompressorRack->removeAirChiller(refrigerationAirChiller);
      }

      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      return addTemplate<RefrigerationAirChiller>(refrigerationAirChiller, modelObjectList);
    }

    void RefrigerationCompressorRack_Impl::removeAirChiller(const RefrigerationAirChiller& refrigerationAirChiller) {
      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      removeTemplate<RefrigerationAirChiller>(refrigerationAirChiller, modelObjectList);
    }

    void RefrigerationCompressorRack_Impl::removeAllAirChillers() {
      boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
      removeAllTemplate<RefrigerationAirChiller>(modelObjectList);
    }

    bool RefrigerationCompressorRack_Impl::setRefrigeratedCaseAndWalkInList(const boost::optional<ModelObjectList>& modelObjectList) {
      bool result(false);
      if (modelObjectList) {
        result = setPointer(OS_Refrigeration_CompressorRackFields::RefrigeratedCaseAndWalkInListName, modelObjectList.get().handle());
      } else {
        resetRefrigeratedCaseAndWalkInList();
        result = true;
      }
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetRefrigeratedCaseAndWalkInList() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::RefrigeratedCaseAndWalkInListName, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCompressorRack_Impl::setHeatRejectionZone(const boost::optional<ThermalZone>& thermalZone) {
      bool result(false);
      if (thermalZone) {
        result = setPointer(OS_Refrigeration_CompressorRackFields::HeatRejectionZoneName, thermalZone.get().handle());
      } else {
        resetHeatRejectionZone();
        result = true;
      }
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetHeatRejectionZone() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::HeatRejectionZoneName, "");
      OS_ASSERT(result);
    }

    ComponentType RefrigerationCompressorRack_Impl::componentType() const {
      return ComponentType::Cooling;
    }

    std::vector<FuelType> RefrigerationCompressorRack_Impl::coolingFuelTypes() const {
      return {FuelType::Electricity};
    }

    std::vector<FuelType> RefrigerationCompressorRack_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> RefrigerationCompressorRack_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  RefrigerationCompressorRack::RefrigerationCompressorRack(const Model& model)
    : StraightComponent(RefrigerationCompressorRack::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::RefrigerationCompressorRack_Impl>());

    CurveQuadratic compressorRackCOPFunctionofTemperatureCurve = CurveQuadratic(model);
    compressorRackCOPFunctionofTemperatureCurve.setCoefficient1Constant(1.7603);
    compressorRackCOPFunctionofTemperatureCurve.setCoefficient2x(-0.0377);
    compressorRackCOPFunctionofTemperatureCurve.setCoefficient3xPOW2(0.0004);
    compressorRackCOPFunctionofTemperatureCurve.setMinimumValueofx(10.0000);
    compressorRackCOPFunctionofTemperatureCurve.setMaximumValueofx(35.0000);

    setHeatRejectionLocation("Outdoors");
    setDesignCompressorRackCOP(2.0);
    setCompressorRackCOPFunctionofTemperatureCurve(compressorRackCOPFunctionofTemperatureCurve);
    setDesignCondenserFanPower(250.0);
    setCondenserType("AirCooled");
    setWaterCooledLoopFlowType("VariableFlow");
    setWaterCooledCondenserMaximumWaterOutletTemperature(55.0);
    setWaterCooledCondenserMinimumWaterInletTemperature(10.0);
    setEvaporativeCondenserEffectiveness(0.9);
    autocalculateEvaporativeCondenserAirFlowRate();
    setBasinHeaterCapacity(200.0);
    setBasinHeaterSetpointTemperature(2.0);
    setDesignEvaporativeCondenserWaterPumpPower(1000.0);
    ModelObjectList caseAndWalkinList = ModelObjectList(model);
    caseAndWalkinList.setName(this->name().get() + " Case and Walkin List");
    bool ok = getImpl<detail::RefrigerationCompressorRack_Impl>()->setRefrigeratedCaseAndWalkInList(caseAndWalkinList);
    OS_ASSERT(ok);
  }

  IddObjectType RefrigerationCompressorRack::iddObjectType() {
    return {IddObjectType::OS_Refrigeration_CompressorRack};
  }

  std::vector<std::string> RefrigerationCompressorRack::condenserTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Refrigeration_CompressorRackFields::CondenserType);
  }

  std::vector<std::string> RefrigerationCompressorRack::waterCooledLoopFlowTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Refrigeration_CompressorRackFields::WaterCooledLoopFlowType);
  }

  std::string RefrigerationCompressorRack::heatRejectionLocation() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->heatRejectionLocation();
  }

  double RefrigerationCompressorRack::designCompressorRackCOP() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->designCompressorRackCOP();
  }

  Curve RefrigerationCompressorRack::compressorRackCOPFunctionofTemperatureCurve() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->compressorRackCOPFunctionofTemperatureCurve();
  }

  double RefrigerationCompressorRack::designCondenserFanPower() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->designCondenserFanPower();
  }

  boost::optional<Curve> RefrigerationCompressorRack::condenserFanPowerFunctionofTemperatureCurve() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->condenserFanPowerFunctionofTemperatureCurve();
  }

  std::string RefrigerationCompressorRack::condenserType() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->condenserType();
  }

  std::string RefrigerationCompressorRack::waterCooledLoopFlowType() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->waterCooledLoopFlowType();
  }

  boost::optional<Schedule> RefrigerationCompressorRack::waterCooledCondenserOutletTemperatureSchedule() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->waterCooledCondenserOutletTemperatureSchedule();
  }

  boost::optional<double> RefrigerationCompressorRack::waterCooledCondenserDesignFlowRate() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->waterCooledCondenserDesignFlowRate();
  }

  boost::optional<double> RefrigerationCompressorRack::waterCooledCondenserMaximumFlowRate() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->waterCooledCondenserMaximumFlowRate();
  }

  double RefrigerationCompressorRack::waterCooledCondenserMaximumWaterOutletTemperature() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->waterCooledCondenserMaximumWaterOutletTemperature();
  }

  double RefrigerationCompressorRack::waterCooledCondenserMinimumWaterInletTemperature() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->waterCooledCondenserMinimumWaterInletTemperature();
  }

  boost::optional<Schedule> RefrigerationCompressorRack::evaporativeCondenserAvailabilitySchedule() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->evaporativeCondenserAvailabilitySchedule();
  }

  double RefrigerationCompressorRack::evaporativeCondenserEffectiveness() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->evaporativeCondenserEffectiveness();
  }

  boost::optional<double> RefrigerationCompressorRack::evaporativeCondenserAirFlowRate() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->evaporativeCondenserAirFlowRate();
  }

  bool RefrigerationCompressorRack::isEvaporativeCondenserAirFlowRateAutocalculated() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->isEvaporativeCondenserAirFlowRateAutocalculated();
  }

  double RefrigerationCompressorRack::basinHeaterCapacity() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->basinHeaterCapacity();
  }

  double RefrigerationCompressorRack::basinHeaterSetpointTemperature() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->basinHeaterSetpointTemperature();
  }

  boost::optional<double> RefrigerationCompressorRack::designEvaporativeCondenserWaterPumpPower() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->designEvaporativeCondenserWaterPumpPower();
  }

  bool RefrigerationCompressorRack::isDesignEvaporativeCondenserWaterPumpPowerAutocalculated() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->isDesignEvaporativeCondenserWaterPumpPowerAutocalculated();
  }

  // boost::optional<HVACComponent> RefrigerationCompressorRack::evaporativeWaterSupplyTank() const {
  //   return getImpl<detail::RefrigerationCompressorRack_Impl>()->evaporativeWaterSupplyTank();
  // }

  // boost::optional<std::string> RefrigerationCompressorRack::condenserAirInletNodeName() const {
  //   return getImpl<detail::RefrigerationCompressorRack_Impl>()->condenserAirInletNodeName();
  // }

  std::string RefrigerationCompressorRack::endUseSubcategory() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->endUseSubcategory();
  }

  bool RefrigerationCompressorRack::isEndUseSubcategoryDefaulted() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->isEndUseSubcategoryDefaulted();
  }

  std::vector<RefrigerationCase> RefrigerationCompressorRack::cases() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->cases();
  }

  std::vector<RefrigerationWalkIn> RefrigerationCompressorRack::walkins() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->walkins();
  }

  std::vector<RefrigerationAirChiller> RefrigerationCompressorRack::airChillers() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->airChillers();
  }

  boost::optional<ThermalZone> RefrigerationCompressorRack::heatRejectionZone() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->heatRejectionZone();
  }

  bool RefrigerationCompressorRack::setHeatRejectionLocation(const std::string& heatRejectionLocation) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setHeatRejectionLocation(heatRejectionLocation);
  }

  bool RefrigerationCompressorRack::setDesignCompressorRackCOP(double designCompressorRackCOP) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setDesignCompressorRackCOP(designCompressorRackCOP);
  }

  bool RefrigerationCompressorRack::setCompressorRackCOPFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setCompressorRackCOPFunctionofTemperatureCurve(curve);
  }

  bool RefrigerationCompressorRack::setDesignCondenserFanPower(double designCondenserFanPower) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setDesignCondenserFanPower(designCondenserFanPower);
  }

  bool RefrigerationCompressorRack::setCondenserFanPowerFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setCondenserFanPowerFunctionofTemperatureCurve(curve);
  }

  void RefrigerationCompressorRack::resetCondenserFanPowerFunctionofTemperatureCurve() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetCondenserFanPowerFunctionofTemperatureCurve();
  }

  bool RefrigerationCompressorRack::setCondenserType(const std::string& condenserType) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setCondenserType(condenserType);
  }

  bool RefrigerationCompressorRack::setWaterCooledLoopFlowType(const std::string& waterCooledLoopFlowType) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setWaterCooledLoopFlowType(waterCooledLoopFlowType);
  }

  bool RefrigerationCompressorRack::setWaterCooledCondenserOutletTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setWaterCooledCondenserOutletTemperatureSchedule(schedule);
  }

  void RefrigerationCompressorRack::resetWaterCooledCondenserOutletTemperatureSchedule() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetWaterCooledCondenserOutletTemperatureSchedule();
  }

  bool RefrigerationCompressorRack::setWaterCooledCondenserDesignFlowRate(double waterCooledCondenserDesignFlowRate) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setWaterCooledCondenserDesignFlowRate(waterCooledCondenserDesignFlowRate);
  }

  void RefrigerationCompressorRack::resetWaterCooledCondenserDesignFlowRate() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetWaterCooledCondenserDesignFlowRate();
  }

  bool RefrigerationCompressorRack::setWaterCooledCondenserMaximumFlowRate(double waterCooledCondenserMaximumFlowRate) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setWaterCooledCondenserMaximumFlowRate(waterCooledCondenserMaximumFlowRate);
  }

  void RefrigerationCompressorRack::resetWaterCooledCondenserMaximumFlowRate() {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->resetWaterCooledCondenserMaximumFlowRate();
  }

  bool RefrigerationCompressorRack::setWaterCooledCondenserMaximumWaterOutletTemperature(double waterCooledCondenserMaximumWaterOutletTemperature) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setWaterCooledCondenserMaximumWaterOutletTemperature(
      waterCooledCondenserMaximumWaterOutletTemperature);
  }

  bool RefrigerationCompressorRack::setWaterCooledCondenserMinimumWaterInletTemperature(double waterCooledCondenserMinimumWaterInletTemperature) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setWaterCooledCondenserMinimumWaterInletTemperature(
      waterCooledCondenserMinimumWaterInletTemperature);
  }

  bool RefrigerationCompressorRack::setEvaporativeCondenserAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setEvaporativeCondenserAvailabilitySchedule(schedule);
  }

  void RefrigerationCompressorRack::resetEvaporativeCondenserAvailabilitySchedule() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetEvaporativeCondenserAvailabilitySchedule();
  }

  bool RefrigerationCompressorRack::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setEvaporativeCondenserEffectiveness(evaporativeCondenserEffectiveness);
  }

  bool RefrigerationCompressorRack::setEvaporativeCondenserAirFlowRate(double evaporativeCondenserAirFlowRate) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setEvaporativeCondenserAirFlowRate(evaporativeCondenserAirFlowRate);
  }

  bool RefrigerationCompressorRack::autocalculateEvaporativeCondenserAirFlowRate() {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->autocalculateEvaporativeCondenserAirFlowRate();
  }

  bool RefrigerationCompressorRack::setBasinHeaterCapacity(double basinHeaterCapacity) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
  }

  bool RefrigerationCompressorRack::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setBasinHeaterSetpointTemperature(basinHeaterSetpointTemperature);
  }

  bool RefrigerationCompressorRack::setDesignEvaporativeCondenserWaterPumpPower(double designEvaporativeCondenserWaterPumpPower) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setDesignEvaporativeCondenserWaterPumpPower(designEvaporativeCondenserWaterPumpPower);
  }

  bool RefrigerationCompressorRack::autocalculateDesignEvaporativeCondenserWaterPumpPower() {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->autocalculateDesignEvaporativeCondenserWaterPumpPower();
  }

  // bool RefrigerationCompressorRack::setEvaporativeWaterSupplyTank(const HVACComponent& evaporativeWaterSupplyTank) {
  //   return getImpl<detail::RefrigerationCompressorRack_Impl>()->setEvaporativeWaterSupplyTank(evaporativeWaterSupplyTank);
  // }

  // void RefrigerationCompressorRack::resetEvaporativeWaterSupplyTank() {
  //  return getImpl<detail::RefrigerationCompressorRack_Impl>()->resetEvaporativeWaterSupplyTank();
  // }

  // bool RefrigerationCompressorRack::setCondenserAirInletNodeName(std::string condenserAirInletNodeName) {
  //   return getImpl<detail::RefrigerationCompressorRack_Impl>()->setCondenserAirInletNodeName(condenserAirInletNodeName);
  // }

  // void RefrigerationCompressorRack::resetCondenserAirInletNodeName() {
  ///  return getImpl<detail::RefrigerationCompressorRack_Impl>()->resetCondenserAirInletNodeName();
  // }

  bool RefrigerationCompressorRack::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  void RefrigerationCompressorRack::resetEndUseSubcategory() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetEndUseSubcategory();
  }

  bool RefrigerationCompressorRack::addCase(const RefrigerationCase& refrigerationCase) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->addCase(refrigerationCase);
  }

  void RefrigerationCompressorRack::removeCase(const RefrigerationCase& refrigerationCase) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->removeCase(refrigerationCase);
  }

  void RefrigerationCompressorRack::removeAllCases() {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->removeAllCases();
  }

  bool RefrigerationCompressorRack::addWalkin(const RefrigerationWalkIn& refrigerationWalkin) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->addWalkin(refrigerationWalkin);
  }

  void RefrigerationCompressorRack::removeWalkin(const RefrigerationWalkIn& refrigerationWalkin) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->removeWalkin(refrigerationWalkin);
  }

  void RefrigerationCompressorRack::removeAllWalkins() {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->removeAllWalkins();
  }

  bool RefrigerationCompressorRack::addAirChiller(const RefrigerationAirChiller& refrigerationAirChiller) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->addAirChiller(refrigerationAirChiller);
  }

  void RefrigerationCompressorRack::removeAirChiller(const RefrigerationAirChiller& refrigerationAirChiller) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->removeAirChiller(refrigerationAirChiller);
  }

  void RefrigerationCompressorRack::removeAllAirChillers() {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->removeAllAirChillers();
  }

  void RefrigerationCompressorRack::resetRefrigeratedCaseAndWalkInList() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetRefrigeratedCaseAndWalkInList();
  }

  bool RefrigerationCompressorRack::setHeatRejectionZone(const ThermalZone& thermalZone) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setHeatRejectionZone(thermalZone);
  }

  void RefrigerationCompressorRack::resetHeatRejectionZone() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetHeatRejectionZone();
  }

  /// @cond
  RefrigerationCompressorRack::RefrigerationCompressorRack(std::shared_ptr<detail::RefrigerationCompressorRack_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
