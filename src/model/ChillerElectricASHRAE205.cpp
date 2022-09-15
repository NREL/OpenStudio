/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ChillerElectricASHRAE205.hpp"
#include "ChillerElectricASHRAE205_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "ExternalFile.hpp"
#include "ExternalFile_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Chiller_Electric_ASHRAE205_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ChillerElectricASHRAE205_Impl::ChillerElectricASHRAE205_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ChillerElectricASHRAE205::iddObjectType());
    }

    ChillerElectricASHRAE205_Impl::ChillerElectricASHRAE205_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ChillerElectricASHRAE205::iddObjectType());
    }

    ChillerElectricASHRAE205_Impl::ChillerElectricASHRAE205_Impl(const ChillerElectricASHRAE205_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ChillerElectricASHRAE205_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType ChillerElectricASHRAE205_Impl::iddObjectType() const {
      return ChillerElectricASHRAE205::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ChillerElectricASHRAE205_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ChillerElectricASHRAE205", "Ambient Temperature"));
      }
      return result;
    }

    ExternalFile ChillerElectricASHRAE205_Impl::representationFile() const {
      boost::optional<ExternalFile> value = optionalRepresentationFile();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Representation File attached.");
      }
      return value.get();
    }

    std::string ChillerElectricASHRAE205_Impl::performanceInterpolationMethod() const {
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ASHRAE205Fields::PerformanceInterpolationMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> ChillerElectricASHRAE205_Impl::ratedCapacity() const {
      return getDouble(OS_Chiller_Electric_ASHRAE205Fields::RatedCapacity, true);
    }

    bool ChillerElectricASHRAE205_Impl::isRatedCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ASHRAE205Fields::RatedCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> ChillerElectricASHRAE205_Impl::autosizedRatedCapacity() {
      return getAutosizedValue("TODO_CHECK_SQL Rated Capacity", "W");
    }

    double ChillerElectricASHRAE205_Impl::sizingFactor() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ASHRAE205Fields::SizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string ChillerElectricASHRAE205_Impl::ambientTemperatureIndicator() const {
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureIndicator, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> ChillerElectricASHRAE205_Impl::ambientTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureScheduleName);
    }

    boost::optional<ThermalZone> ChillerElectricASHRAE205_Impl::ambientTemperatureZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureZoneName);
    }

    boost::optional<Connection> ChillerElectricASHRAE205_Impl::ambientTemperatureOutdoorAirNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureOutdoorAirNodeName);
    }

    Connection ChillerElectricASHRAE205_Impl::chilledWaterInletNode() const {
      boost::optional<Connection> value = optionalChilledWaterInletNode();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Chilled Water Inlet Node attached.");
      }
      return value.get();
    }

    Connection ChillerElectricASHRAE205_Impl::chilledWaterOutletNode() const {
      boost::optional<Connection> value = optionalChilledWaterOutletNode();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Chilled Water Outlet Node attached.");
      }
      return value.get();
    }

    boost::optional<double> ChillerElectricASHRAE205_Impl::chilledWaterMaximumRequestedFlowRate() const {
      return getDouble(OS_Chiller_Electric_ASHRAE205Fields::ChilledWaterMaximumRequestedFlowRate, true);
    }

    bool ChillerElectricASHRAE205_Impl::isChilledWaterMaximumRequestedFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ASHRAE205Fields::ChilledWaterMaximumRequestedFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> ChillerElectricASHRAE205_Impl::autosizedChilledWaterMaximumRequestedFlowRate() {
      return getAutosizedValue("TODO_CHECK_SQL Chilled Water Maximum Requested Flow Rate", "m3/s");
    }

    boost::optional<Connection> ChillerElectricASHRAE205_Impl::condenserInletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Chiller_Electric_ASHRAE205Fields::CondenserInletNodeName);
    }

    boost::optional<Connection> ChillerElectricASHRAE205_Impl::condenserOutletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Chiller_Electric_ASHRAE205Fields::CondenserOutletNodeName);
    }

    boost::optional<double> ChillerElectricASHRAE205_Impl::condenserMaximumRequestedFlowRate() const {
      return getDouble(OS_Chiller_Electric_ASHRAE205Fields::CondenserMaximumRequestedFlowRate, true);
    }

    bool ChillerElectricASHRAE205_Impl::isCondenserMaximumRequestedFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ASHRAE205Fields::CondenserMaximumRequestedFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> ChillerElectricASHRAE205_Impl::autosizedCondenserMaximumRequestedFlowRate() {
      return getAutosizedValue("TODO_CHECK_SQL Condenser Maximum Requested Flow Rate", "m3/s");
    }

    std::string ChillerElectricASHRAE205_Impl::chillerFlowMode() const {
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ASHRAE205Fields::ChillerFlowMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Connection> ChillerElectricASHRAE205_Impl::oilCoolerInletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Chiller_Electric_ASHRAE205Fields::OilCoolerInletNodeName);
    }

    boost::optional<Connection> ChillerElectricASHRAE205_Impl::oilCoolerOutletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Chiller_Electric_ASHRAE205Fields::OilCoolerOutletNodeName);
    }

    boost::optional<double> ChillerElectricASHRAE205_Impl::oilCoolerDesignFlowRate() const {
      return getDouble(OS_Chiller_Electric_ASHRAE205Fields::OilCoolerDesignFlowRate, true);
    }

    boost::optional<Connection> ChillerElectricASHRAE205_Impl::auxiliaryInletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Chiller_Electric_ASHRAE205Fields::AuxiliaryInletNodeName);
    }

    boost::optional<Connection> ChillerElectricASHRAE205_Impl::auxiliaryOutletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Chiller_Electric_ASHRAE205Fields::AuxiliaryOutletNodeName);
    }

    boost::optional<double> ChillerElectricASHRAE205_Impl::auxiliaryCoolingDesignFlowRate() const {
      return getDouble(OS_Chiller_Electric_ASHRAE205Fields::AuxiliaryCoolingDesignFlowRate, true);
    }

    boost::optional<Connection> ChillerElectricASHRAE205_Impl::heatRecoveryInletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Chiller_Electric_ASHRAE205Fields::HeatRecoveryInletNodeName);
    }

    boost::optional<Connection> ChillerElectricASHRAE205_Impl::heatRecoveryOutletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Chiller_Electric_ASHRAE205Fields::HeatRecoveryOutletNodeName);
    }

    std::string ChillerElectricASHRAE205_Impl::endUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ASHRAE205Fields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricASHRAE205_Impl::isEndUseSubcategoryDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ASHRAE205Fields::EndUseSubcategory);
    }

    bool ChillerElectricASHRAE205_Impl::setRepresentationFile(const ExternalFile& externalFile) {
      bool result = setPointer(OS_Chiller_Electric_ASHRAE205Fields::RepresentationFileName, externalFile.handle());
      return result;
    }

    bool ChillerElectricASHRAE205_Impl::setPerformanceInterpolationMethod(const std::string& performanceInterpolationMethod) {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::PerformanceInterpolationMethod, performanceInterpolationMethod);
      return result;
    }

    bool ChillerElectricASHRAE205_Impl::setRatedCapacity(double ratedCapacity) {
      bool result = setDouble(OS_Chiller_Electric_ASHRAE205Fields::RatedCapacity, ratedCapacity);
      return result;
    }

    void ChillerElectricASHRAE205_Impl::autosizeRatedCapacity() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::RatedCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool ChillerElectricASHRAE205_Impl::setSizingFactor(double sizingFactor) {
      bool result = setDouble(OS_Chiller_Electric_ASHRAE205Fields::SizingFactor, sizingFactor);
      return result;
    }

    bool ChillerElectricASHRAE205_Impl::setAmbientTemperatureIndicator(const std::string& ambientTemperatureIndicator) {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureIndicator, ambientTemperatureIndicator);
      return result;
    }

    bool ChillerElectricASHRAE205_Impl::setAmbientTemperatureSchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureScheduleName, "ChillerElectricASHRAE205", "Ambient Temperature", schedule);
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetAmbientTemperatureSchedule() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricASHRAE205_Impl::setAmbientTemperatureZone(const ThermalZone& thermalZone) {
      bool result = setPointer(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureZoneName, thermalZone.handle());
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetAmbientTemperatureZone() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureZoneName, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricASHRAE205_Impl::setAmbientTemperatureOutdoorAirNode(const Connection& connection) {
      bool result = setPointer(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureOutdoorAirNodeName, connection.handle());
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetAmbientTemperatureOutdoorAirNode() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureOutdoorAirNodeName, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricASHRAE205_Impl::setChilledWaterInletNode(const Connection& connection) {
      bool result = setPointer(OS_Chiller_Electric_ASHRAE205Fields::ChilledWaterInletNodeName, connection.handle());
      return result;
    }

    bool ChillerElectricASHRAE205_Impl::setChilledWaterOutletNode(const Connection& connection) {
      bool result = setPointer(OS_Chiller_Electric_ASHRAE205Fields::ChilledWaterOutletNodeName, connection.handle());
      return result;
    }

    bool ChillerElectricASHRAE205_Impl::setChilledWaterMaximumRequestedFlowRate(double chilledWaterMaximumRequestedFlowRate) {
      bool result = setDouble(OS_Chiller_Electric_ASHRAE205Fields::ChilledWaterMaximumRequestedFlowRate, chilledWaterMaximumRequestedFlowRate);
      return result;
    }

    void ChillerElectricASHRAE205_Impl::autosizeChilledWaterMaximumRequestedFlowRate() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::ChilledWaterMaximumRequestedFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool ChillerElectricASHRAE205_Impl::setCondenserInletNode(const Connection& connection) {
      bool result = setPointer(OS_Chiller_Electric_ASHRAE205Fields::CondenserInletNodeName, connection.handle());
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetCondenserInletNode() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::CondenserInletNodeName, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricASHRAE205_Impl::setCondenserOutletNode(const Connection& connection) {
      bool result = setPointer(OS_Chiller_Electric_ASHRAE205Fields::CondenserOutletNodeName, connection.handle());
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetCondenserOutletNode() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::CondenserOutletNodeName, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricASHRAE205_Impl::setCondenserMaximumRequestedFlowRate(double condenserMaximumRequestedFlowRate) {
      bool result = setDouble(OS_Chiller_Electric_ASHRAE205Fields::CondenserMaximumRequestedFlowRate, condenserMaximumRequestedFlowRate);
      return result;
    }

    void ChillerElectricASHRAE205_Impl::autosizeCondenserMaximumRequestedFlowRate() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::CondenserMaximumRequestedFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool ChillerElectricASHRAE205_Impl::setChillerFlowMode(const std::string& chillerFlowMode) {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::ChillerFlowMode, chillerFlowMode);
      return result;
    }

    bool ChillerElectricASHRAE205_Impl::setOilCoolerInletNode(const Connection& connection) {
      bool result = setPointer(OS_Chiller_Electric_ASHRAE205Fields::OilCoolerInletNodeName, connection.handle());
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetOilCoolerInletNode() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::OilCoolerInletNodeName, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricASHRAE205_Impl::setOilCoolerOutletNode(const Connection& connection) {
      bool result = setPointer(OS_Chiller_Electric_ASHRAE205Fields::OilCoolerOutletNodeName, connection.handle());
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetOilCoolerOutletNode() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::OilCoolerOutletNodeName, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricASHRAE205_Impl::setOilCoolerDesignFlowRate(double oilCoolerDesignFlowRate) {
      bool result = setDouble(OS_Chiller_Electric_ASHRAE205Fields::OilCoolerDesignFlowRate, oilCoolerDesignFlowRate);
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetOilCoolerDesignFlowRate() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::OilCoolerDesignFlowRate, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricASHRAE205_Impl::setAuxiliaryInletNode(const Connection& connection) {
      bool result = setPointer(OS_Chiller_Electric_ASHRAE205Fields::AuxiliaryInletNodeName, connection.handle());
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetAuxiliaryInletNode() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::AuxiliaryInletNodeName, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricASHRAE205_Impl::setAuxiliaryOutletNode(const Connection& connection) {
      bool result = setPointer(OS_Chiller_Electric_ASHRAE205Fields::AuxiliaryOutletNodeName, connection.handle());
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetAuxiliaryOutletNode() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::AuxiliaryOutletNodeName, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricASHRAE205_Impl::setAuxiliaryCoolingDesignFlowRate(double auxiliaryCoolingDesignFlowRate) {
      bool result = setDouble(OS_Chiller_Electric_ASHRAE205Fields::AuxiliaryCoolingDesignFlowRate, auxiliaryCoolingDesignFlowRate);
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetAuxiliaryCoolingDesignFlowRate() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::AuxiliaryCoolingDesignFlowRate, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricASHRAE205_Impl::setHeatRecoveryInletNode(const Connection& connection) {
      bool result = setPointer(OS_Chiller_Electric_ASHRAE205Fields::HeatRecoveryInletNodeName, connection.handle());
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetHeatRecoveryInletNode() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::HeatRecoveryInletNodeName, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricASHRAE205_Impl::setHeatRecoveryOutletNode(const Connection& connection) {
      bool result = setPointer(OS_Chiller_Electric_ASHRAE205Fields::HeatRecoveryOutletNodeName, connection.handle());
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetHeatRecoveryOutletNode() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::HeatRecoveryOutletNodeName, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricASHRAE205_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::EndUseSubcategory, endUseSubcategory);
      OS_ASSERT(result);
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetEndUseSubcategory() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::EndUseSubcategory, "");
      OS_ASSERT(result);
    }

    void ChillerElectricASHRAE205_Impl::autosize() {
      autosizeRatedCapacity();
      autosizeChilledWaterMaximumRequestedFlowRate();
      autosizeCondenserMaximumRequestedFlowRate();
    }

    void ChillerElectricASHRAE205_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedRatedCapacity();
      if (val) {
        setRatedCapacity(val.get());
      }

      val = autosizedChilledWaterMaximumRequestedFlowRate();
      if (val) {
        setChilledWaterMaximumRequestedFlowRate(val.get());
      }

      val = autosizedCondenserMaximumRequestedFlowRate();
      if (val) {
        setCondenserMaximumRequestedFlowRate(val.get());
      }
    }

    boost::optional<ExternalFile> ChillerElectricASHRAE205_Impl::optionalRepresentationFile() const {
      return getObject<ModelObject>().getModelObjectTarget<ExternalFile>(OS_Chiller_Electric_ASHRAE205Fields::RepresentationFileName);
    }

    boost::optional<Connection> ChillerElectricASHRAE205_Impl::optionalChilledWaterInletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Chiller_Electric_ASHRAE205Fields::ChilledWaterInletNodeName);
    }

    boost::optional<Connection> ChillerElectricASHRAE205_Impl::optionalChilledWaterOutletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Chiller_Electric_ASHRAE205Fields::ChilledWaterOutletNodeName);
    }

  }  // namespace detail

  ChillerElectricASHRAE205::ChillerElectricASHRAE205(const Model& model) : WaterToWaterComponent(ChillerElectricASHRAE205::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ChillerElectricASHRAE205_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    //     OS_Chiller_Electric_ASHRAE205Fields::RepresentationFileName
    //     OS_Chiller_Electric_ASHRAE205Fields::ChilledWaterInletNodeName
    //     OS_Chiller_Electric_ASHRAE205Fields::ChilledWaterOutletNodeName
    bool ok = true;
    // ok = setRepresentationFile();
    OS_ASSERT(ok);
    // ok = setPerformanceInterpolationMethod();
    OS_ASSERT(ok);
    // ok = setRatedCapacity();
    OS_ASSERT(ok);
    // ok = setSizingFactor();
    OS_ASSERT(ok);
    // ok = setAmbientTemperatureIndicator();
    OS_ASSERT(ok);
    // ok = setChilledWaterInletNode();
    OS_ASSERT(ok);
    // ok = setChilledWaterOutletNode();
    OS_ASSERT(ok);
    // ok = setChilledWaterMaximumRequestedFlowRate();
    OS_ASSERT(ok);
    // ok = setCondenserMaximumRequestedFlowRate();
    OS_ASSERT(ok);
    // ok = setChillerFlowMode();
    OS_ASSERT(ok);
  }

  IddObjectType ChillerElectricASHRAE205::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Chiller_Electric_ASHRAE205);
  }

  std::vector<std::string> ChillerElectricASHRAE205::performanceInterpolationMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Chiller_Electric_ASHRAE205Fields::PerformanceInterpolationMethod);
  }

  std::vector<std::string> ChillerElectricASHRAE205::ambientTemperatureIndicatorValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureIndicator);
  }

  std::vector<std::string> ChillerElectricASHRAE205::chillerFlowModeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Chiller_Electric_ASHRAE205Fields::ChillerFlowMode);
  }

  ExternalFile ChillerElectricASHRAE205::representationFile() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->representationFile();
  }

  std::string ChillerElectricASHRAE205::performanceInterpolationMethod() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->performanceInterpolationMethod();
  }

  boost::optional<double> ChillerElectricASHRAE205::ratedCapacity() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->ratedCapacity();
  }

  bool ChillerElectricASHRAE205::isRatedCapacityAutosized() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->isRatedCapacityAutosized();
  }

  boost::optional<double> ChillerElectricASHRAE205::autosizedRatedCapacity() {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->autosizedRatedCapacity();
  }

  double ChillerElectricASHRAE205::sizingFactor() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->sizingFactor();
  }

  std::string ChillerElectricASHRAE205::ambientTemperatureIndicator() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->ambientTemperatureIndicator();
  }

  boost::optional<Schedule> ChillerElectricASHRAE205::ambientTemperatureSchedule() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->ambientTemperatureSchedule();
  }

  boost::optional<ThermalZone> ChillerElectricASHRAE205::ambientTemperatureZone() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->ambientTemperatureZone();
  }

  boost::optional<Connection> ChillerElectricASHRAE205::ambientTemperatureOutdoorAirNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->ambientTemperatureOutdoorAirNode();
  }

  Connection ChillerElectricASHRAE205::chilledWaterInletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->chilledWaterInletNode();
  }

  Connection ChillerElectricASHRAE205::chilledWaterOutletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->chilledWaterOutletNode();
  }

  boost::optional<double> ChillerElectricASHRAE205::chilledWaterMaximumRequestedFlowRate() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->chilledWaterMaximumRequestedFlowRate();
  }

  bool ChillerElectricASHRAE205::isChilledWaterMaximumRequestedFlowRateAutosized() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->isChilledWaterMaximumRequestedFlowRateAutosized();
  }

  boost::optional<double> ChillerElectricASHRAE205::autosizedChilledWaterMaximumRequestedFlowRate() {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->autosizedChilledWaterMaximumRequestedFlowRate();
  }

  boost::optional<Connection> ChillerElectricASHRAE205::condenserInletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->condenserInletNode();
  }

  boost::optional<Connection> ChillerElectricASHRAE205::condenserOutletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->condenserOutletNode();
  }

  boost::optional<double> ChillerElectricASHRAE205::condenserMaximumRequestedFlowRate() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->condenserMaximumRequestedFlowRate();
  }

  bool ChillerElectricASHRAE205::isCondenserMaximumRequestedFlowRateAutosized() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->isCondenserMaximumRequestedFlowRateAutosized();
  }

  boost::optional<double> ChillerElectricASHRAE205::autosizedCondenserMaximumRequestedFlowRate() {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->autosizedCondenserMaximumRequestedFlowRate();
  }

  std::string ChillerElectricASHRAE205::chillerFlowMode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->chillerFlowMode();
  }

  boost::optional<Connection> ChillerElectricASHRAE205::oilCoolerInletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->oilCoolerInletNode();
  }

  boost::optional<Connection> ChillerElectricASHRAE205::oilCoolerOutletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->oilCoolerOutletNode();
  }

  boost::optional<double> ChillerElectricASHRAE205::oilCoolerDesignFlowRate() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->oilCoolerDesignFlowRate();
  }

  boost::optional<Connection> ChillerElectricASHRAE205::auxiliaryInletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->auxiliaryInletNode();
  }

  boost::optional<Connection> ChillerElectricASHRAE205::auxiliaryOutletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->auxiliaryOutletNode();
  }

  boost::optional<double> ChillerElectricASHRAE205::auxiliaryCoolingDesignFlowRate() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->auxiliaryCoolingDesignFlowRate();
  }

  boost::optional<Connection> ChillerElectricASHRAE205::heatRecoveryInletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->heatRecoveryInletNode();
  }

  boost::optional<Connection> ChillerElectricASHRAE205::heatRecoveryOutletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->heatRecoveryOutletNode();
  }

  std::string ChillerElectricASHRAE205::endUseSubcategory() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->endUseSubcategory();
  }

  bool ChillerElectricASHRAE205::isEndUseSubcategoryDefaulted() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->isEndUseSubcategoryDefaulted();
  }

  bool ChillerElectricASHRAE205::setRepresentationFile(const ExternalFile& externalFile) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setRepresentationFile(externalFile);
  }

  bool ChillerElectricASHRAE205::setPerformanceInterpolationMethod(const std::string& performanceInterpolationMethod) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setPerformanceInterpolationMethod(performanceInterpolationMethod);
  }

  bool ChillerElectricASHRAE205::setRatedCapacity(double ratedCapacity) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setRatedCapacity(ratedCapacity);
  }

  void ChillerElectricASHRAE205::autosizeRatedCapacity() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->autosizeRatedCapacity();
  }

  bool ChillerElectricASHRAE205::setSizingFactor(double sizingFactor) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setSizingFactor(sizingFactor);
  }

  bool ChillerElectricASHRAE205::setAmbientTemperatureIndicator(const std::string& ambientTemperatureIndicator) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setAmbientTemperatureIndicator(ambientTemperatureIndicator);
  }

  bool ChillerElectricASHRAE205::setAmbientTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setAmbientTemperatureSchedule(schedule);
  }

  void ChillerElectricASHRAE205::resetAmbientTemperatureSchedule() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetAmbientTemperatureSchedule();
  }

  bool ChillerElectricASHRAE205::setAmbientTemperatureZone(const ThermalZone& thermalZone) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setAmbientTemperatureZone(thermalZone);
  }

  void ChillerElectricASHRAE205::resetAmbientTemperatureZone() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetAmbientTemperatureZone();
  }

  bool ChillerElectricASHRAE205::setAmbientTemperatureOutdoorAirNode(const Connection& connection) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setAmbientTemperatureOutdoorAirNode(connection);
  }

  void ChillerElectricASHRAE205::resetAmbientTemperatureOutdoorAirNode() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetAmbientTemperatureOutdoorAirNode();
  }

  bool ChillerElectricASHRAE205::setChilledWaterInletNode(const Connection& connection) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setChilledWaterInletNode(connection);
  }

  bool ChillerElectricASHRAE205::setChilledWaterOutletNode(const Connection& connection) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setChilledWaterOutletNode(connection);
  }

  bool ChillerElectricASHRAE205::setChilledWaterMaximumRequestedFlowRate(double chilledWaterMaximumRequestedFlowRate) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setChilledWaterMaximumRequestedFlowRate(chilledWaterMaximumRequestedFlowRate);
  }

  void ChillerElectricASHRAE205::autosizeChilledWaterMaximumRequestedFlowRate() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->autosizeChilledWaterMaximumRequestedFlowRate();
  }

  bool ChillerElectricASHRAE205::setCondenserInletNode(const Connection& connection) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setCondenserInletNode(connection);
  }

  void ChillerElectricASHRAE205::resetCondenserInletNode() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetCondenserInletNode();
  }

  bool ChillerElectricASHRAE205::setCondenserOutletNode(const Connection& connection) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setCondenserOutletNode(connection);
  }

  void ChillerElectricASHRAE205::resetCondenserOutletNode() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetCondenserOutletNode();
  }

  bool ChillerElectricASHRAE205::setCondenserMaximumRequestedFlowRate(double condenserMaximumRequestedFlowRate) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setCondenserMaximumRequestedFlowRate(condenserMaximumRequestedFlowRate);
  }

  void ChillerElectricASHRAE205::autosizeCondenserMaximumRequestedFlowRate() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->autosizeCondenserMaximumRequestedFlowRate();
  }

  bool ChillerElectricASHRAE205::setChillerFlowMode(const std::string& chillerFlowMode) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setChillerFlowMode(chillerFlowMode);
  }

  bool ChillerElectricASHRAE205::setOilCoolerInletNode(const Connection& connection) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setOilCoolerInletNode(connection);
  }

  void ChillerElectricASHRAE205::resetOilCoolerInletNode() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetOilCoolerInletNode();
  }

  bool ChillerElectricASHRAE205::setOilCoolerOutletNode(const Connection& connection) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setOilCoolerOutletNode(connection);
  }

  void ChillerElectricASHRAE205::resetOilCoolerOutletNode() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetOilCoolerOutletNode();
  }

  bool ChillerElectricASHRAE205::setOilCoolerDesignFlowRate(double oilCoolerDesignFlowRate) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setOilCoolerDesignFlowRate(oilCoolerDesignFlowRate);
  }

  void ChillerElectricASHRAE205::resetOilCoolerDesignFlowRate() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetOilCoolerDesignFlowRate();
  }

  bool ChillerElectricASHRAE205::setAuxiliaryInletNode(const Connection& connection) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setAuxiliaryInletNode(connection);
  }

  void ChillerElectricASHRAE205::resetAuxiliaryInletNode() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetAuxiliaryInletNode();
  }

  bool ChillerElectricASHRAE205::setAuxiliaryOutletNode(const Connection& connection) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setAuxiliaryOutletNode(connection);
  }

  void ChillerElectricASHRAE205::resetAuxiliaryOutletNode() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetAuxiliaryOutletNode();
  }

  bool ChillerElectricASHRAE205::setAuxiliaryCoolingDesignFlowRate(double auxiliaryCoolingDesignFlowRate) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setAuxiliaryCoolingDesignFlowRate(auxiliaryCoolingDesignFlowRate);
  }

  void ChillerElectricASHRAE205::resetAuxiliaryCoolingDesignFlowRate() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetAuxiliaryCoolingDesignFlowRate();
  }

  bool ChillerElectricASHRAE205::setHeatRecoveryInletNode(const Connection& connection) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setHeatRecoveryInletNode(connection);
  }

  void ChillerElectricASHRAE205::resetHeatRecoveryInletNode() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetHeatRecoveryInletNode();
  }

  bool ChillerElectricASHRAE205::setHeatRecoveryOutletNode(const Connection& connection) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setHeatRecoveryOutletNode(connection);
  }

  void ChillerElectricASHRAE205::resetHeatRecoveryOutletNode() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetHeatRecoveryOutletNode();
  }

  bool ChillerElectricASHRAE205::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  void ChillerElectricASHRAE205::resetEndUseSubcategory() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetEndUseSubcategory();
  }

  /// @cond
  ChillerElectricASHRAE205::ChillerElectricASHRAE205(std::shared_ptr<detail::ChillerElectricASHRAE205_Impl> impl)
    : WaterToWaterComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
