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

#include "ExternalFile.hpp"
#include "ExternalFile_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"

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
      static std::vector<std::string> result{"Chiller Part Load Ratio",
                                             "Chiller Cycling Ratio",
                                             "Minimum Part Load Ratio",
                                             "Chiller Electricity Rate",
                                             "Chiller Electricity Energy",
                                             "Chiller Evaporator Cooling Rate",
                                             "Chiller Evaporator Cooling Energy",
                                             "Chiller Evaporator Inlet Temperature",
                                             "Chiller Evaporator Outlet Temperature",
                                             "Chiller Evaporator Mass Flow Rate",
                                             "Chiller Condenser Heat Transfer Rate",
                                             "Chiller Condenser Heat Transfer Energy",
                                             "Chiller COP",
                                             "Chiller Condenser Inlet Temperature",
                                             "Chiller Condenser Outlet Temperature",
                                             "Chiller Condenser Mass Flow Rate",
                                             "Chiller Effective Heat Rejection Temperature",
                                             "Chiller Zone Heat Gain Rate",
                                             "Chiller Zone Heat Gain Energy",
                                             "Oil Cooler Heat Transfer Rate",
                                             "Oil Cooler Heat Transfer Energy",
                                             "Auxiliary Heat Transfer Rate",
                                             "Auxiliary Heat Transfer Energy"};
      return result;
    }

    IddObjectType ChillerElectricASHRAE205_Impl::iddObjectType() const {
      return ChillerElectricASHRAE205::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ChillerElectricASHRAE205_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureScheduleName) != e) {
        result.emplace_back("ChillerElectricASHRAE205", "Ambient Temperature");
      }
      return result;
    }

    boost::optional<ExternalFile> ChillerElectricASHRAE205_Impl::optionalRepresentationFile() const {
      return getObject<ModelObject>().getModelObjectTarget<ExternalFile>(OS_Chiller_Electric_ASHRAE205Fields::RepresentationFileName);
    }

    ExternalFile ChillerElectricASHRAE205_Impl::representationFile() const {
      boost::optional<ExternalFile> value = optionalRepresentationFile();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a Representation File attached.");
      }
      return value.get();
    }

    bool ChillerElectricASHRAE205_Impl::setRepresentationFile(const ExternalFile& externalFile) {
      bool result = setPointer(OS_Chiller_Electric_ASHRAE205Fields::RepresentationFileName, externalFile.handle());
      return result;
    }

    std::string ChillerElectricASHRAE205_Impl::performanceInterpolationMethod() const {
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ASHRAE205Fields::PerformanceInterpolationMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricASHRAE205_Impl::setPerformanceInterpolationMethod(const std::string& performanceInterpolationMethod) {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::PerformanceInterpolationMethod, performanceInterpolationMethod);
      return result;
    }

    boost::optional<double> ChillerElectricASHRAE205_Impl::ratedCapacity() const {
      return getDouble(OS_Chiller_Electric_ASHRAE205Fields::RatedCapacity, true);
    }

    bool ChillerElectricASHRAE205_Impl::isRatedCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ASHRAE205Fields::RatedCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "AutoSize");
      }
      return result;
    }

    bool ChillerElectricASHRAE205_Impl::setRatedCapacity(double ratedCapacity) {
      bool result = setDouble(OS_Chiller_Electric_ASHRAE205Fields::RatedCapacity, ratedCapacity);
      return result;
    }

    void ChillerElectricASHRAE205_Impl::autosizeRatedCapacity() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::RatedCapacity, "AutoSize");
      OS_ASSERT(result);
    }

    double ChillerElectricASHRAE205_Impl::sizingFactor() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ASHRAE205Fields::SizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricASHRAE205_Impl::setSizingFactor(double sizingFactor) {
      bool result = setDouble(OS_Chiller_Electric_ASHRAE205Fields::SizingFactor, sizingFactor);
      return result;
    }

    std::string ChillerElectricASHRAE205_Impl::ambientTemperatureIndicator() const {
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureIndicator, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricASHRAE205_Impl::setAmbientTemperatureIndicator(const std::string& ambientTemperatureIndicator) {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureIndicator, ambientTemperatureIndicator);
      return result;
    }

    boost::optional<Schedule> ChillerElectricASHRAE205_Impl::ambientTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureScheduleName);
    }

    bool ChillerElectricASHRAE205_Impl::setAmbientTemperatureSchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureScheduleName, "ChillerElectricASHRAE205", "Ambient Temperature", schedule);
      if (result) {
        bool ok = setAmbientTemperatureIndicator("Schedule");
        OS_ASSERT(ok);
      }
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetAmbientTemperatureSchedule() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureScheduleName, "");
      OS_ASSERT(result);
      if (openstudio::istringEqual("Schedule", ambientTemperatureIndicator())) {
        result = setAmbientTemperatureIndicator("Outdoors");
        OS_ASSERT(result);
      }
    }

    boost::optional<ThermalZone> ChillerElectricASHRAE205_Impl::ambientTemperatureZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureZoneName);
    }

    bool ChillerElectricASHRAE205_Impl::setAmbientTemperatureZone(const ThermalZone& thermalZone) {
      bool result = setPointer(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureZoneName, thermalZone.handle());
      if (result) {
        bool ok = setAmbientTemperatureIndicator("Zone");
        OS_ASSERT(ok);
      }
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetAmbientTemperatureZone() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureZoneName, "");
      OS_ASSERT(result);
      if (openstudio::istringEqual("Zone", ambientTemperatureIndicator())) {
        result = setAmbientTemperatureIndicator("Outdoors");
        OS_ASSERT(result);
      }
    }

    boost::optional<std::string> ChillerElectricASHRAE205_Impl::ambientTemperatureOutdoorAirNodeName() const {
      return getString(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureOutdoorAirNodeName, false, true);
    }

    bool ChillerElectricASHRAE205_Impl::setAmbientTemperatureOutdoorAirNodeName(const std::string& ambientTemperatureOutdoorAirNodeName) {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureOutdoorAirNodeName, ambientTemperatureOutdoorAirNodeName);
      if (result) {
        bool ok = setAmbientTemperatureIndicator("Outdoors");
        OS_ASSERT(ok);
      }
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetAmbientTemperatureOutdoorAirNodeName() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::AmbientTemperatureOutdoorAirNodeName, "");
      OS_ASSERT(result);
    }

    boost::optional<double> ChillerElectricASHRAE205_Impl::chilledWaterMaximumRequestedFlowRate() const {
      return getDouble(OS_Chiller_Electric_ASHRAE205Fields::ChilledWaterMaximumRequestedFlowRate, true);
    }

    bool ChillerElectricASHRAE205_Impl::isChilledWaterMaximumRequestedFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ASHRAE205Fields::ChilledWaterMaximumRequestedFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "AutoSize");
      }
      return result;
    }

    bool ChillerElectricASHRAE205_Impl::setChilledWaterMaximumRequestedFlowRate(double chilledWaterMaximumRequestedFlowRate) {
      bool result = setDouble(OS_Chiller_Electric_ASHRAE205Fields::ChilledWaterMaximumRequestedFlowRate, chilledWaterMaximumRequestedFlowRate);
      return result;
    }

    void ChillerElectricASHRAE205_Impl::autosizeChilledWaterMaximumRequestedFlowRate() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::ChilledWaterMaximumRequestedFlowRate, "AutoSize");
      OS_ASSERT(result);
    }

    boost::optional<double> ChillerElectricASHRAE205_Impl::condenserMaximumRequestedFlowRate() const {
      return getDouble(OS_Chiller_Electric_ASHRAE205Fields::CondenserMaximumRequestedFlowRate, true);
    }

    bool ChillerElectricASHRAE205_Impl::isCondenserMaximumRequestedFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ASHRAE205Fields::CondenserMaximumRequestedFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "AutoSize");
      }
      return result;
    }

    bool ChillerElectricASHRAE205_Impl::setCondenserMaximumRequestedFlowRate(double condenserMaximumRequestedFlowRate) {
      bool result = setDouble(OS_Chiller_Electric_ASHRAE205Fields::CondenserMaximumRequestedFlowRate, condenserMaximumRequestedFlowRate);
      return result;
    }

    void ChillerElectricASHRAE205_Impl::autosizeCondenserMaximumRequestedFlowRate() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::CondenserMaximumRequestedFlowRate, "AutoSize");
      OS_ASSERT(result);
    }

    std::string ChillerElectricASHRAE205_Impl::chillerFlowMode() const {
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ASHRAE205Fields::ChillerFlowMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricASHRAE205_Impl::setChillerFlowMode(const std::string& chillerFlowMode) {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::ChillerFlowMode, chillerFlowMode);
      return result;
    }

    boost::optional<double> ChillerElectricASHRAE205_Impl::oilCoolerDesignFlowRate() const {
      return getDouble(OS_Chiller_Electric_ASHRAE205Fields::OilCoolerDesignFlowRate, true);
    }

    bool ChillerElectricASHRAE205_Impl::setOilCoolerDesignFlowRate(double oilCoolerDesignFlowRate) {
      bool result = setDouble(OS_Chiller_Electric_ASHRAE205Fields::OilCoolerDesignFlowRate, oilCoolerDesignFlowRate);
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetOilCoolerDesignFlowRate() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::OilCoolerDesignFlowRate, "");
      OS_ASSERT(result);
    }

    boost::optional<double> ChillerElectricASHRAE205_Impl::auxiliaryCoolingDesignFlowRate() const {
      return getDouble(OS_Chiller_Electric_ASHRAE205Fields::AuxiliaryCoolingDesignFlowRate, true);
    }

    bool ChillerElectricASHRAE205_Impl::setAuxiliaryCoolingDesignFlowRate(double auxiliaryCoolingDesignFlowRate) {
      bool result = setDouble(OS_Chiller_Electric_ASHRAE205Fields::AuxiliaryCoolingDesignFlowRate, auxiliaryCoolingDesignFlowRate);
      return result;
    }

    void ChillerElectricASHRAE205_Impl::resetAuxiliaryCoolingDesignFlowRate() {
      bool result = setString(OS_Chiller_Electric_ASHRAE205Fields::AuxiliaryCoolingDesignFlowRate, "");
      OS_ASSERT(result);
    }

    std::string ChillerElectricASHRAE205_Impl::endUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ASHRAE205Fields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricASHRAE205_Impl::isEndUseSubcategoryDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ASHRAE205Fields::EndUseSubcategory);
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

    boost::optional<double> ChillerElectricASHRAE205_Impl::autosizedRatedCapacity() {
      return getAutosizedValue("Design Size Rated Capacity", "W");
    }

    boost::optional<double> ChillerElectricASHRAE205_Impl::autosizedChilledWaterMaximumRequestedFlowRate() {
      return getAutosizedValue("Design Size Chilled Water Maximum Requested Flow Rate", "m3/s");
    }

    boost::optional<double> ChillerElectricASHRAE205_Impl::autosizedCondenserMaximumRequestedFlowRate() {
      return getAutosizedValue("Design Size Condenser Maximum Requested Flow Rate", "m3/s");
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

    // Primary: Chiller Water
    unsigned ChillerElectricASHRAE205_Impl::supplyInletPort() const {
      return OS_Chiller_Electric_ASHRAE205Fields::ChilledWaterInletNodeName;
    }

    unsigned ChillerElectricASHRAE205_Impl::supplyOutletPort() const {
      return OS_Chiller_Electric_ASHRAE205Fields::ChilledWaterOutletNodeName;
    }

    boost::optional<PlantLoop> ChillerElectricASHRAE205_Impl::chilledWaterLoop() const {
      return WaterToWaterComponent_Impl::plantLoop();
    }

    boost::optional<Node> ChillerElectricASHRAE205_Impl::chilledWaterInletNode() const {
      if (auto mo_ = supplyInletModelObject()) {
        return mo_->optionalCast<Node>();
      }
      return boost::none;
    }

    boost::optional<Node> ChillerElectricASHRAE205_Impl::chilledWaterOutletNode() const {
      if (auto mo_ = supplyOutletModelObject()) {
        return mo_->optionalCast<Node>();
      }
      return boost::none;
    }

    // Secondary: Condenser Loop

    unsigned ChillerElectricASHRAE205_Impl::demandInletPort() const {
      return OS_Chiller_Electric_ASHRAE205Fields::CondenserInletNodeName;
    }

    unsigned ChillerElectricASHRAE205_Impl::demandOutletPort() const {
      return OS_Chiller_Electric_ASHRAE205Fields::CondenserOutletNodeName;
    }

    boost::optional<PlantLoop> ChillerElectricASHRAE205_Impl::condenserWaterLoop() const {
      return WaterToWaterComponent_Impl::secondaryPlantLoop();
    }

    boost::optional<Node> ChillerElectricASHRAE205_Impl::condenserInletNode() const {
      if (auto mo_ = demandInletModelObject()) {
        return mo_->optionalCast<Node>();
      }
      return boost::none;
    }

    boost::optional<Node> ChillerElectricASHRAE205_Impl::condenserOutletNode() const {
      if (auto mo_ = demandOutletModelObject()) {
        return mo_->optionalCast<Node>();
      }
      return boost::none;
    }

    // Tertiary: Heat Recovery
    unsigned ChillerElectricASHRAE205_Impl::tertiaryInletPort() const {
      return OS_Chiller_Electric_ASHRAE205Fields::HeatRecoveryInletNodeName;
    }

    unsigned ChillerElectricASHRAE205_Impl::tertiaryOutletPort() const {
      return OS_Chiller_Electric_ASHRAE205Fields::HeatRecoveryOutletNodeName;
    }

    boost::optional<PlantLoop> ChillerElectricASHRAE205_Impl::heatRecoveryLoop() const {
      return WaterToWaterComponent_Impl::tertiaryPlantLoop();
    }

    boost::optional<Node> ChillerElectricASHRAE205_Impl::heatRecoveryInletNode() const {
      if (auto mo_ = tertiaryInletModelObject()) {
        return mo_->optionalCast<Node>();
      }
      return boost::none;
    }

    boost::optional<Node> ChillerElectricASHRAE205_Impl::heatRecoveryOutletNode() const {
      if (auto mo_ = tertiaryOutletModelObject()) {
        return mo_->optionalCast<Node>();
      }
      return boost::none;
    }

    bool ChillerElectricASHRAE205_Impl::addToNode(Node& node) {
      boost::optional<PlantLoop> t_plantLoop = node.plantLoop();

      // If trying to add to a node that is on the demand side of a plant loop
      if (t_plantLoop) {
        if (t_plantLoop->demandComponent(node.handle())) {
          // If there is already a another loop where it's on the demand side (condenser Plant Loop)
          boost::optional<PlantLoop> cndLoop_ = this->condenserWaterLoop();
          if (cndLoop_) {
            // And it's not the same as the node's loop
            if (t_plantLoop.get() != cndLoop_.get()) {
              // And if there is no Heat Recovery (tertiary)
              if (!this->heatRecoveryLoop().is_initialized()) {
                ;
                // Then try to add it to the tertiary one
                LOG(Warn, "Calling addToTertiaryNode to connect it to the tertiary (=Heat Recovery) loop for " << briefDescription());
                return this->addToTertiaryNode(node);
              }
            }
          }
        }
      }

      // All other cases, call the base class implementation
      return WaterToWaterComponent_Impl::addToNode(node);
    }

    bool ChillerElectricASHRAE205_Impl::addToTertiaryNode(Node& node) {
      auto t_plantLoop = node.plantLoop();

      // Only accept adding to a node that is on a demand side of a plant loop
      // Since tertiary here = heat recovery loop (heating)
      if (t_plantLoop) {
        if (t_plantLoop->demandComponent(node.handle())) {
          // Call base class method which accepts both supply and demand
          return WaterToWaterComponent_Impl::addToTertiaryNode(node);
        } else {
          LOG(Info,
              "Tertiary Loop (Heat Recovery Loop) connections can only be placed on the Demand side (of a Heating Loop), for " << briefDescription());
        }
      }
      return false;
    }

    boost::optional<PlantLoop> ChillerElectricASHRAE205_Impl::plantLoop() const {
      if (boost::optional<ModelObject> mo_ = supplyOutletModelObject()) {
        if (boost::optional<Node> n_ = mo_->optionalCast<Node>()) {
          return n_->plantLoop();
        }
      }
      return boost::none;
    }

    boost::optional<PlantLoop> ChillerElectricASHRAE205_Impl::secondaryPlantLoop() const {
      if (boost::optional<ModelObject> mo_ = demandOutletModelObject()) {
        if (boost::optional<Node> n_ = mo_->optionalCast<Node>()) {
          return n_->plantLoop();
        }
      }
      return boost::none;
    }

    boost::optional<PlantLoop> ChillerElectricASHRAE205_Impl::tertiaryPlantLoop() const {
      if (boost::optional<ModelObject> mo_ = tertiaryOutletModelObject()) {
        if (boost::optional<Node> n_ = mo_->optionalCast<Node>()) {
          return n_->plantLoop();
        }
      }
      return boost::none;
    }

    // Extra loops

    /// Oil Cooler Loop

    unsigned ChillerElectricASHRAE205_Impl::oilCoolerInletPort() const {
      return OS_Chiller_Electric_ASHRAE205Fields::OilCoolerInletNodeName;
    }

    OptionalModelObject ChillerElectricASHRAE205_Impl::oilCoolerInletModelObject() const {
      return connectedObject(ChillerElectricASHRAE205_Impl::oilCoolerInletPort());
    }

    boost::optional<Node> ChillerElectricASHRAE205_Impl::oilCoolerInletNode() const {
      if (auto mo_ = oilCoolerInletModelObject()) {
        return mo_->optionalCast<Node>();
      }
      return boost::none;
    }

    unsigned ChillerElectricASHRAE205_Impl::oilCoolerOutletPort() const {
      return OS_Chiller_Electric_ASHRAE205Fields::OilCoolerOutletNodeName;
    }

    OptionalModelObject ChillerElectricASHRAE205_Impl::oilCoolerOutletModelObject() const {
      return connectedObject(ChillerElectricASHRAE205_Impl::oilCoolerOutletPort());
    }

    boost::optional<Node> ChillerElectricASHRAE205_Impl::oilCoolerOutletNode() const {
      if (auto mo_ = oilCoolerOutletModelObject()) {
        return mo_->optionalCast<Node>();
      }
      return boost::none;
    }

    boost::optional<PlantLoop> ChillerElectricASHRAE205_Impl::oilCoolerLoop() const {
      if (boost::optional<Node> n_ = oilCoolerOutletNode()) {
        return n_->plantLoop();
      }
      return boost::none;
    }

    bool ChillerElectricASHRAE205_Impl::removeFromOilCoolerLoop() {
      if (auto plant = oilCoolerLoop()) {
        return HVACComponent_Impl::removeFromLoop(plant->demandInletNode(), plant->demandOutletNode(), oilCoolerInletPort(), oilCoolerOutletPort());
      }

      return false;
    }

    bool ChillerElectricASHRAE205_Impl::addToOilCoolerLoopNode(Node& node) {

      if (node.getImpl<Node_Impl>()->isConnected(getObject<ModelObject>())) {
        return false;
      }

      auto plantLoop_ = node.plantLoop();
      if (!plantLoop_) {
        LOG(Warn, "For " << briefDescription() << ", Oil Cooler Loop can only be connected to a PlantLoop");
        return false;
      }

      if (!plantLoop_->demandComponent(node.handle())) {
        LOG(Warn, "For " << briefDescription() << ", Oil Cooler Loop can only be connected to a PlantLoop on the demand side");
        return false;
      }

      HVACComponent systemStartComponent = plantLoop_->demandInletNode();
      HVACComponent systemEndComponent = plantLoop_->demandOutletNode();

      removeFromOilCoolerLoop();

      return HVACComponent_Impl::addToNode(node, systemStartComponent, systemEndComponent, oilCoolerInletPort(), oilCoolerOutletPort());
    }

    /// Auxiliary Loop

    unsigned ChillerElectricASHRAE205_Impl::auxiliaryInletPort() const {
      return OS_Chiller_Electric_ASHRAE205Fields::AuxiliaryInletNodeName;
    }

    OptionalModelObject ChillerElectricASHRAE205_Impl::auxiliaryInletModelObject() const {
      return connectedObject(ChillerElectricASHRAE205_Impl::auxiliaryInletPort());
    }

    boost::optional<Node> ChillerElectricASHRAE205_Impl::auxiliaryInletNode() const {
      if (auto mo_ = auxiliaryInletModelObject()) {
        return mo_->optionalCast<Node>();
      }
      return boost::none;
    }

    unsigned ChillerElectricASHRAE205_Impl::auxiliaryOutletPort() const {
      return OS_Chiller_Electric_ASHRAE205Fields::AuxiliaryOutletNodeName;
    }

    OptionalModelObject ChillerElectricASHRAE205_Impl::auxiliaryOutletModelObject() const {
      return connectedObject(ChillerElectricASHRAE205_Impl::auxiliaryOutletPort());
    }

    boost::optional<Node> ChillerElectricASHRAE205_Impl::auxiliaryOutletNode() const {
      if (auto mo_ = auxiliaryOutletModelObject()) {
        return mo_->optionalCast<Node>();
      }
      return boost::none;
    }

    boost::optional<PlantLoop> ChillerElectricASHRAE205_Impl::auxiliaryLoop() const {
      if (boost::optional<Node> n_ = auxiliaryOutletNode()) {
        return n_->plantLoop();
      }
      return boost::none;
    }

    bool ChillerElectricASHRAE205_Impl::removeFromAuxiliaryLoop() {
      if (auto plant = auxiliaryLoop()) {
        return HVACComponent_Impl::removeFromLoop(plant->demandInletNode(), plant->demandOutletNode(), auxiliaryInletPort(), auxiliaryOutletPort());
      }

      return false;
    }

    bool ChillerElectricASHRAE205_Impl::addToAuxiliaryLoopNode(Node& node) {

      if (node.getImpl<Node_Impl>()->isConnected(getObject<ModelObject>())) {
        return false;
      }

      auto plantLoop_ = node.plantLoop();
      if (!plantLoop_) {
        LOG(Warn, "For " << briefDescription() << ", Oil Cooler Loop can only be connected to a PlantLoop");
        return false;
      }

      if (!plantLoop_->demandComponent(node.handle())) {
        LOG(Warn, "For " << briefDescription() << ", Oil Cooler Loop can only be connected to a PlantLoop on the demand side");
        return false;
      }

      HVACComponent systemStartComponent = plantLoop_->demandInletNode();
      HVACComponent systemEndComponent = plantLoop_->demandOutletNode();

      removeFromAuxiliaryLoop();

      return HVACComponent_Impl::addToNode(node, systemStartComponent, systemEndComponent, auxiliaryInletPort(), auxiliaryOutletPort());
    }

  }  // namespace detail

  ChillerElectricASHRAE205::ChillerElectricASHRAE205(const ExternalFile& representationFile)
    : WaterToWaterComponent(ChillerElectricASHRAE205::iddObjectType(), representationFile.model()) {
    OS_ASSERT(getImpl<detail::ChillerElectricASHRAE205_Impl>());

    auto filePath = representationFile.filePath();
    bool ok = (toString(filePath.extension()) == ".cbor");
    if (!ok) {
      remove();
      LOG_AND_THROW("External file must have a .cbor extension, got externalfile='" << filePath << "'.");
    }
    ok = setPointer(OS_Chiller_Electric_ASHRAE205Fields::RepresentationFileName, representationFile.handle());
    OS_ASSERT(ok);

    setPerformanceInterpolationMethod("Linear");
    autosizeRatedCapacity();
    setSizingFactor(1.0);
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->setAmbientTemperatureIndicator("Outdoors");
    autosizeChilledWaterMaximumRequestedFlowRate();
    autosizeCondenserMaximumRequestedFlowRate();
    setChillerFlowMode("NotModulated");
    setEndUseSubcategory("General");
  }

  IddObjectType ChillerElectricASHRAE205::iddObjectType() {
    return {IddObjectType::OS_Chiller_Electric_ASHRAE205};
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

  boost::optional<std::string> ChillerElectricASHRAE205::ambientTemperatureOutdoorAirNodeName() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->ambientTemperatureOutdoorAirNodeName();
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

  boost::optional<double> ChillerElectricASHRAE205::oilCoolerDesignFlowRate() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->oilCoolerDesignFlowRate();
  }

  boost::optional<double> ChillerElectricASHRAE205::auxiliaryCoolingDesignFlowRate() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->auxiliaryCoolingDesignFlowRate();
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

  // Done via the other setAmbientXXX
  // bool ChillerElectricASHRAE205::setAmbientTemperatureIndicator(const std::string& ambientTemperatureIndicator) {
  //   return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setAmbientTemperatureIndicator(ambientTemperatureIndicator);
  // }

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

  bool ChillerElectricASHRAE205::setAmbientTemperatureOutdoorAirNodeName(const std::string& ambientTemperatureOutdoorAirNodeName) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setAmbientTemperatureOutdoorAirNodeName(ambientTemperatureOutdoorAirNodeName);
  }

  void ChillerElectricASHRAE205::resetAmbientTemperatureOutdoorAirNodeName() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetAmbientTemperatureOutdoorAirNodeName();
  }

  bool ChillerElectricASHRAE205::setChilledWaterMaximumRequestedFlowRate(double chilledWaterMaximumRequestedFlowRate) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setChilledWaterMaximumRequestedFlowRate(chilledWaterMaximumRequestedFlowRate);
  }

  void ChillerElectricASHRAE205::autosizeChilledWaterMaximumRequestedFlowRate() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->autosizeChilledWaterMaximumRequestedFlowRate();
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

  bool ChillerElectricASHRAE205::setOilCoolerDesignFlowRate(double oilCoolerDesignFlowRate) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setOilCoolerDesignFlowRate(oilCoolerDesignFlowRate);
  }

  void ChillerElectricASHRAE205::resetOilCoolerDesignFlowRate() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetOilCoolerDesignFlowRate();
  }

  bool ChillerElectricASHRAE205::setAuxiliaryCoolingDesignFlowRate(double auxiliaryCoolingDesignFlowRate) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setAuxiliaryCoolingDesignFlowRate(auxiliaryCoolingDesignFlowRate);
  }

  void ChillerElectricASHRAE205::resetAuxiliaryCoolingDesignFlowRate() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetAuxiliaryCoolingDesignFlowRate();
  }

  bool ChillerElectricASHRAE205::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  void ChillerElectricASHRAE205::resetEndUseSubcategory() {
    getImpl<detail::ChillerElectricASHRAE205_Impl>()->resetEndUseSubcategory();
  }

  // Convenience getters

  boost::optional<PlantLoop> ChillerElectricASHRAE205::chilledWaterLoop() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->chilledWaterLoop();
  }

  boost::optional<Node> ChillerElectricASHRAE205::chilledWaterInletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->chilledWaterInletNode();
  }

  boost::optional<Node> ChillerElectricASHRAE205::chilledWaterOutletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->chilledWaterOutletNode();
  }

  boost::optional<PlantLoop> ChillerElectricASHRAE205::condenserWaterLoop() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->condenserWaterLoop();
  }

  boost::optional<Node> ChillerElectricASHRAE205::condenserInletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->condenserInletNode();
  }

  boost::optional<Node> ChillerElectricASHRAE205::condenserOutletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->condenserOutletNode();
  }

  boost::optional<PlantLoop> ChillerElectricASHRAE205::heatRecoveryLoop() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->heatRecoveryLoop();
  }

  boost::optional<Node> ChillerElectricASHRAE205::heatRecoveryInletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->heatRecoveryInletNode();
  }

  boost::optional<Node> ChillerElectricASHRAE205::heatRecoveryOutletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->heatRecoveryOutletNode();
  }

  // Oil Cooler Loop

  unsigned ChillerElectricASHRAE205::oilCoolerInletPort() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->oilCoolerInletPort();
  }
  boost::optional<ModelObject> ChillerElectricASHRAE205::oilCoolerInletModelObject() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->oilCoolerInletModelObject();
  }
  boost::optional<Node> ChillerElectricASHRAE205::oilCoolerInletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->oilCoolerInletNode();
  }

  unsigned ChillerElectricASHRAE205::oilCoolerOutletPort() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->oilCoolerOutletPort();
  }
  boost::optional<ModelObject> ChillerElectricASHRAE205::oilCoolerOutletModelObject() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->oilCoolerOutletModelObject();
  }
  boost::optional<Node> ChillerElectricASHRAE205::oilCoolerOutletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->oilCoolerOutletNode();
  }

  boost::optional<PlantLoop> ChillerElectricASHRAE205::oilCoolerLoop() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->oilCoolerLoop();
  }

  bool ChillerElectricASHRAE205::addToOilCoolerLoopNode(Node& node) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->addToOilCoolerLoopNode(node);
  }

  bool ChillerElectricASHRAE205::removeFromOilCoolerLoop() {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->removeFromOilCoolerLoop();
  }

  // Auxiliary Loop

  unsigned ChillerElectricASHRAE205::auxiliaryInletPort() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->auxiliaryInletPort();
  }
  boost::optional<ModelObject> ChillerElectricASHRAE205::auxiliaryInletModelObject() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->auxiliaryInletModelObject();
  }
  boost::optional<Node> ChillerElectricASHRAE205::auxiliaryInletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->auxiliaryInletNode();
  }

  unsigned ChillerElectricASHRAE205::auxiliaryOutletPort() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->auxiliaryOutletPort();
  }
  boost::optional<ModelObject> ChillerElectricASHRAE205::auxiliaryOutletModelObject() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->auxiliaryOutletModelObject();
  }
  boost::optional<Node> ChillerElectricASHRAE205::auxiliaryOutletNode() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->auxiliaryOutletNode();
  }

  boost::optional<PlantLoop> ChillerElectricASHRAE205::auxiliaryLoop() const {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->auxiliaryLoop();
  }

  bool ChillerElectricASHRAE205::addToAuxiliaryLoopNode(Node& node) {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->addToAuxiliaryLoopNode(node);
  }

  bool ChillerElectricASHRAE205::removeFromAuxiliaryLoop() {
    return getImpl<detail::ChillerElectricASHRAE205_Impl>()->removeFromAuxiliaryLoop();
  }

  /// @cond
  ChillerElectricASHRAE205::ChillerElectricASHRAE205(std::shared_ptr<detail::ChillerElectricASHRAE205_Impl> impl)
    : WaterToWaterComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
