/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_WATERHEATERSTRATIFIED_IMPL_HPP
#define MODEL_WATERHEATERSTRATIFIED_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  /** WaterHeaterStratified_Impl is a WaterToWaterComponent_Impl that is the implementation class for WaterHeater:Stratified.*/
  class MODEL_API WaterHeaterStratified_Impl : public WaterToWaterComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    WaterHeaterStratified_Impl(const IdfObject& idfObject,
                                Model_Impl* model,
                                bool keepHandle);

    WaterHeaterStratified_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

    WaterHeaterStratified_Impl(const WaterHeaterStratified_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

    virtual ~WaterHeaterStratified_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual unsigned supplyInletPort() override;

    virtual unsigned supplyOutletPort() override;

    virtual unsigned demandInletPort() override;

    virtual unsigned demandOutletPort() override;

    //@}
    /** @name Getters */
    //@{

    std::string endUseSubcategory() const;

    boost::optional<double> tankVolume() const;

    bool isTankVolumeAutosized() const;

    boost::optional<double> tankHeight() const;

    bool isTankHeightAutosized() const;

    std::string tankShape() const;

    boost::optional<double> tankPerimeter() const;

    double maximumTemperatureLimit() const;

    std::string heaterPriorityControl() const;

    Schedule heater1SetpointTemperatureSchedule() const;

    double heater1DeadbandTemperatureDifference() const;

    boost::optional<double> heater1Capacity() const;

    bool isHeater1CapacityAutosized() const;

    double heater1Height() const;

    Schedule heater2SetpointTemperatureSchedule() const;

    double heater2DeadbandTemperatureDifference() const;

    double heater2Capacity() const;

    double heater2Height() const;

    std::string heaterFuelType() const;

    double heaterThermalEfficiency() const;

    double offCycleParasiticFuelConsumptionRate() const;

    std::string offCycleParasiticFuelType() const;

    double offCycleParasiticHeatFractiontoTank() const;

    double offCycleParasiticHeight() const;

    double onCycleParasiticFuelConsumptionRate() const;

    std::string onCycleParasiticFuelType() const;

    double onCycleParasiticHeatFractiontoTank() const;

    double onCycleParasiticHeight() const;

    std::string ambientTemperatureIndicator() const;

    boost::optional<Schedule> ambientTemperatureSchedule() const;

    boost::optional<ThermalZone> ambientTemperatureThermalZone() const;

    boost::optional<std::string> ambientTemperatureOutdoorAirNodeName() const;

    boost::optional<double> uniformSkinLossCoefficientperUnitAreatoAmbientTemperature() const;

    double skinLossFractiontoZone() const;

    boost::optional<double> offCycleFlueLossCoefficienttoAmbientTemperature() const;

    double offCycleFlueLossFractiontoZone() const;

    boost::optional<double> peakUseFlowRate() const;

    boost::optional<Schedule> useFlowRateFractionSchedule() const;

    boost::optional<Schedule> coldWaterSupplyTemperatureSchedule() const;

    double useSideEffectiveness() const;

    double useSideInletHeight() const;

    boost::optional<double> useSideOutletHeight() const;

    bool isUseSideOutletHeightAutocalculated() const;

    double sourceSideEffectiveness() const;

    boost::optional<double> sourceSideInletHeight() const;

    bool isSourceSideInletHeightAutocalculated() const;

    double sourceSideOutletHeight() const;

    std::string inletMode() const;

    boost::optional<double> useSideDesignFlowRate() const;

    bool isUseSideDesignFlowRateAutosized() const;

    boost::optional<double> sourceSideDesignFlowRate() const;

    bool isSourceSideDesignFlowRateAutosized() const;

    double indirectWaterHeatingRecoveryTime() const;

    int numberofNodes() const;

    double additionalDestratificationConductivity() const;

    double node1AdditionalLossCoefficient() const;

    double node2AdditionalLossCoefficient() const;

    double node3AdditionalLossCoefficient() const;

    double node4AdditionalLossCoefficient() const;

    double node5AdditionalLossCoefficient() const;

    double node6AdditionalLossCoefficient() const;

    double node7AdditionalLossCoefficient() const;

    double node8AdditionalLossCoefficient() const;

    double node9AdditionalLossCoefficient() const;

    double node10AdditionalLossCoefficient() const;

    double node11AdditionalLossCoefficient() const;

    double node12AdditionalLossCoefficient() const;

    std::string sourceSideFlowControlMode() const;

    boost::optional<Schedule> indirectAlternateSetpointTemperatureSchedule() const;

  boost::optional<double> autosizedTankVolume() const ;

  boost::optional<double> autosizedTankHeight() const ;

  boost::optional<double> autosizedHeater1Capacity() const ;

  boost::optional<double> autosizedUseSideDesignFlowRate() const ;

  boost::optional<double> autosizedSourceSideDesignFlowRate() const ;

  virtual void autosize() override;

  virtual void applySizingValues() override;

    //@}
    /** @name Setters */
    //@{

    bool setEndUseSubcategory(std::string endUseSubcategory);

    bool setTankVolume(boost::optional<double> tankVolume);

    void autosizeTankVolume();

    bool setTankHeight(boost::optional<double> tankHeight);

    void autosizeTankHeight();

    bool setTankShape(std::string tankShape);

    bool setTankPerimeter(boost::optional<double> tankPerimeter);

    void resetTankPerimeter();

    bool setMaximumTemperatureLimit(double maximumTemperatureLimit);

    bool setHeaterPriorityControl(std::string heaterPriorityControl);

    bool setHeater1SetpointTemperatureSchedule(Schedule& schedule);

    bool setHeater1DeadbandTemperatureDifference(double heater1DeadbandTemperatureDifference);

    bool setHeater1Capacity(boost::optional<double> heater1Capacity);

    void autosizeHeater1Capacity();

    bool setHeater1Height(double heater1Height);

    bool setHeater2SetpointTemperatureSchedule(Schedule& schedule);

    bool setHeater2DeadbandTemperatureDifference(double heater2DeadbandTemperatureDifference);

    bool setHeater2Capacity(double heater2Capacity);

    bool setHeater2Height(double heater2Height);

    bool setHeaterFuelType(std::string heaterFuelType);

    bool setHeaterThermalEfficiency(double heaterThermalEfficiency);

    bool setOffCycleParasiticFuelConsumptionRate(double offCycleParasiticFuelConsumptionRate);

    bool setOffCycleParasiticFuelType(std::string offCycleParasiticFuelType);

    bool setOffCycleParasiticHeatFractiontoTank(double offCycleParasiticHeatFractiontoTank);

    bool setOffCycleParasiticHeight(double offCycleParasiticHeight);

    bool setOnCycleParasiticFuelConsumptionRate(double onCycleParasiticFuelConsumptionRate);

    bool setOnCycleParasiticFuelType(std::string onCycleParasiticFuelType);

    bool setOnCycleParasiticHeatFractiontoTank(double onCycleParasiticHeatFractiontoTank);

    bool setOnCycleParasiticHeight(double onCycleParasiticHeight);

    bool setAmbientTemperatureIndicator(std::string ambientTemperatureIndicator);

    bool setAmbientTemperatureSchedule(Schedule& schedule);

    void resetAmbientTemperatureSchedule();

    bool setAmbientTemperatureThermalZone(const boost::optional<ThermalZone>& thermalZone);

    void resetAmbientTemperatureThermalZone();

    bool setAmbientTemperatureOutdoorAirNodeName(boost::optional<std::string> ambientTemperatureOutdoorAirNodeName);

    void resetAmbientTemperatureOutdoorAirNodeName();

    bool setUniformSkinLossCoefficientperUnitAreatoAmbientTemperature(boost::optional<double> uniformSkinLossCoefficientperUnitAreatoAmbientTemperature);

    void resetUniformSkinLossCoefficientperUnitAreatoAmbientTemperature();

    bool setSkinLossFractiontoZone(double skinLossFractiontoZone);

    bool setOffCycleFlueLossCoefficienttoAmbientTemperature(boost::optional<double> offCycleFlueLossCoefficienttoAmbientTemperature);

    void resetOffCycleFlueLossCoefficienttoAmbientTemperature();

    bool setOffCycleFlueLossFractiontoZone(double offCycleFlueLossFractiontoZone);

    bool setPeakUseFlowRate(boost::optional<double> peakUseFlowRate);

    void resetPeakUseFlowRate();

    bool setUseFlowRateFractionSchedule(Schedule& schedule);

    void resetUseFlowRateFractionSchedule();

    bool setColdWaterSupplyTemperatureSchedule(Schedule& schedule);

    void resetColdWaterSupplyTemperatureSchedule();

    bool setUseSideEffectiveness(double useSideEffectiveness);

    bool setUseSideInletHeight(double useSideInletHeight);

    bool setUseSideOutletHeight(boost::optional<double> useSideOutletHeight);

    void autocalculateUseSideOutletHeight();

    bool setSourceSideEffectiveness(double sourceSideEffectiveness);

    bool setSourceSideInletHeight(boost::optional<double> sourceSideInletHeight);

    void autocalculateSourceSideInletHeight();

    bool setSourceSideOutletHeight(double sourceSideOutletHeight);

    bool setInletMode(std::string inletMode);

    bool setUseSideDesignFlowRate(boost::optional<double> useSideDesignFlowRate);

    void autosizeUseSideDesignFlowRate();

    bool setSourceSideDesignFlowRate(boost::optional<double> sourceSideDesignFlowRate);

    void autosizeSourceSideDesignFlowRate();

    bool setIndirectWaterHeatingRecoveryTime(double indirectWaterHeatingRecoveryTime);

    bool setNumberofNodes(int numberofNodes);

    bool setAdditionalDestratificationConductivity(double additionalDestratificationConductivity);

    bool setNode1AdditionalLossCoefficient(double node1AdditionalLossCoefficient);

    bool setNode2AdditionalLossCoefficient(double node2AdditionalLossCoefficient);

    bool setNode3AdditionalLossCoefficient(double node3AdditionalLossCoefficient);

    bool setNode4AdditionalLossCoefficient(double node4AdditionalLossCoefficient);

    bool setNode5AdditionalLossCoefficient(double node5AdditionalLossCoefficient);

    bool setNode6AdditionalLossCoefficient(double node6AdditionalLossCoefficient);

    bool setNode7AdditionalLossCoefficient(double node7AdditionalLossCoefficient);

    bool setNode8AdditionalLossCoefficient(double node8AdditionalLossCoefficient);

    bool setNode9AdditionalLossCoefficient(double node9AdditionalLossCoefficient);

    bool setNode10AdditionalLossCoefficient(double node10AdditionalLossCoefficient);

    bool setNode11AdditionalLossCoefficient(double node11AdditionalLossCoefficient);

    bool setNode12AdditionalLossCoefficient(double node12AdditionalLossCoefficient);

    bool setSourceSideFlowControlMode(std::string sourceSideFlowControlMode);

    bool setIndirectAlternateSetpointTemperatureSchedule(Schedule& schedule);

    void resetIndirectAlternateSetpointTemperatureSchedule();

    //@}
    /** @name Other */
    //@{

    boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.WaterHeaterStratified");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Schedule> optionalHeater1SetpointTemperatureSchedule() const;
    boost::optional<Schedule> optionalHeater2SetpointTemperatureSchedule() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_WATERHEATERSTRATIFIED_IMPL_HPP
