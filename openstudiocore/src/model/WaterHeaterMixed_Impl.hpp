/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_WATERHEATERMIXED_IMPL_HPP
#define MODEL_WATERHEATERMIXED_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {

namespace model {

class Schedule;
class CurveCubic;
class ThermalZone;

namespace detail {

  class MODEL_API WaterHeaterMixed_Impl : public WaterToWaterComponent_Impl {

   public:
    /** @name Constructors and Destructors */
    //@{

    WaterHeaterMixed_Impl(const IdfObject& idfObject,
                          Model_Impl* model,
                          bool keepHandle);

    WaterHeaterMixed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    WaterHeaterMixed_Impl(const WaterHeaterMixed_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    virtual ~WaterHeaterMixed_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual unsigned supplyInletPort() const override;

    virtual unsigned supplyOutletPort() const override;

    virtual unsigned demandInletPort() const override;

    virtual unsigned demandOutletPort() const override;

    virtual void autosize() override;

    virtual void applySizingValues() override;

    boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;


    //@}
    /** @name Getters */
    //@{

    boost::optional<double> tankVolume() const;

    OSOptionalQuantity getTankVolume(bool returnIP=false) const;

    bool isTankVolumeDefaulted() const;

    bool isTankVolumeAutosized() const;

    boost::optional<Schedule> setpointTemperatureSchedule() const;

    double deadbandTemperatureDifference() const;

    Quantity getDeadbandTemperatureDifference(bool returnIP=false) const;

    bool isDeadbandTemperatureDifferenceDefaulted() const;

    boost::optional<double> maximumTemperatureLimit() const;

    OSOptionalQuantity getMaximumTemperatureLimit(bool returnIP=false) const;

    std::string heaterControlType() const;

    bool isHeaterControlTypeDefaulted() const;

    boost::optional<double> heaterMaximumCapacity() const;

    OSOptionalQuantity getHeaterMaximumCapacity(bool returnIP=false) const;

    bool isHeaterMaximumCapacityAutosized() const;

    boost::optional<double> heaterMinimumCapacity() const;

    OSOptionalQuantity getHeaterMinimumCapacity(bool returnIP=false) const;

    double heaterIgnitionMinimumFlowRate() const;

    Quantity getHeaterIgnitionMinimumFlowRate(bool returnIP=false) const;

    bool isHeaterIgnitionMinimumFlowRateDefaulted() const;

    double heaterIgnitionDelay() const;

    Quantity getHeaterIgnitionDelay(bool returnIP=false) const;

    bool isHeaterIgnitionDelayDefaulted() const;

    std::string heaterFuelType() const;

    boost::optional<double> heaterThermalEfficiency() const;

    OSOptionalQuantity getHeaterThermalEfficiency(bool returnIP=false) const;

    boost::optional<CurveCubic> partLoadFactorCurve() const;

    double offCycleParasiticFuelConsumptionRate() const;

    Quantity getOffCycleParasiticFuelConsumptionRate(bool returnIP=false) const;

    bool isOffCycleParasiticFuelConsumptionRateDefaulted() const;

    boost::optional<std::string> offCycleParasiticFuelType() const;

    double offCycleParasiticHeatFractiontoTank() const;

    Quantity getOffCycleParasiticHeatFractiontoTank(bool returnIP=false) const;

    bool isOffCycleParasiticHeatFractiontoTankDefaulted() const;

    double onCycleParasiticFuelConsumptionRate() const;

    Quantity getOnCycleParasiticFuelConsumptionRate(bool returnIP=false) const;

    bool isOnCycleParasiticFuelConsumptionRateDefaulted() const;

    boost::optional<std::string> onCycleParasiticFuelType() const;

    double onCycleParasiticHeatFractiontoTank() const;

    Quantity getOnCycleParasiticHeatFractiontoTank(bool returnIP=false) const;

    bool isOnCycleParasiticHeatFractiontoTankDefaulted() const;

    std::string ambientTemperatureIndicator() const;

    boost::optional<Schedule> ambientTemperatureSchedule() const;

    boost::optional<ThermalZone> ambientTemperatureThermalZone() const;

    boost::optional<std::string> ambientTemperatureOutdoorAirNodeName() const;

    boost::optional<double> offCycleLossCoefficienttoAmbientTemperature() const;

    OSOptionalQuantity getOffCycleLossCoefficienttoAmbientTemperature(bool returnIP=false) const;

    double offCycleLossFractiontoThermalZone() const;

    Quantity getOffCycleLossFractiontoThermalZone(bool returnIP=false) const;

    bool isOffCycleLossFractiontoThermalZoneDefaulted() const;

    boost::optional<double> onCycleLossCoefficienttoAmbientTemperature() const;

    OSOptionalQuantity getOnCycleLossCoefficienttoAmbientTemperature(bool returnIP=false) const;

    double onCycleLossFractiontoThermalZone() const;

    Quantity getOnCycleLossFractiontoThermalZone(bool returnIP=false) const;

    bool isOnCycleLossFractiontoThermalZoneDefaulted() const;

    boost::optional<double> peakUseFlowRate() const;

    OSOptionalQuantity getPeakUseFlowRate(bool returnIP=false) const;

    boost::optional<Schedule> useFlowRateFractionSchedule() const;

    boost::optional<Schedule> coldWaterSupplyTemperatureSchedule() const;

    double useSideEffectiveness() const;

    Quantity getUseSideEffectiveness(bool returnIP=false) const;

    bool isUseSideEffectivenessDefaulted() const;

    double sourceSideEffectiveness() const;

    Quantity getSourceSideEffectiveness(bool returnIP=false) const;

    bool isSourceSideEffectivenessDefaulted() const;

    boost::optional<double> useSideDesignFlowRate() const;

    OSOptionalQuantity getUseSideDesignFlowRate(bool returnIP=false) const;

    bool isUseSideDesignFlowRateDefaulted() const;

    bool isUseSideDesignFlowRateAutosized() const;

    boost::optional<double> sourceSideDesignFlowRate() const;

    OSOptionalQuantity getSourceSideDesignFlowRate(bool returnIP=false) const;

    bool isSourceSideDesignFlowRateDefaulted() const;

    bool isSourceSideDesignFlowRateAutosized() const;

    double indirectWaterHeatingRecoveryTime() const;

    Quantity getIndirectWaterHeatingRecoveryTime(bool returnIP=false) const;

    bool isIndirectWaterHeatingRecoveryTimeDefaulted() const;

    boost::optional<double> autosizedTankVolume() const;

    boost::optional<double> autosizedHeaterMaximumCapacity() const;

    boost::optional<double> autosizedUseSideDesignFlowRate() const;

    boost::optional<double> autosizedSourceSideDesignFlowRate() const;

    std::string sourceSideFlowControlMode() const;

    boost::optional<Schedule> indirectAlternateSetpointTemperatureSchedule() const;

    std::string endUseSubcategory() const;

    //@}
    /** @name Setters */
    //@{

    bool setTankVolume(boost::optional<double> tankVolume);

    bool setTankVolume(const OSOptionalQuantity& tankVolume);

    void resetTankVolume();

    void autosizeTankVolume();

    bool setSetpointTemperatureSchedule(Schedule& setpointTemperatureSchedule);

    void resetSetpointTemperatureSchedule();

    bool setDeadbandTemperatureDifference(double deadbandTemperatureDifference);

    bool setDeadbandTemperatureDifference(const Quantity& deadbandTemperatureDifference);

    void resetDeadbandTemperatureDifference();

    bool setMaximumTemperatureLimit(boost::optional<double> maximumTemperatureLimit);

    bool setMaximumTemperatureLimit(const OSOptionalQuantity& maximumTemperatureLimit);

    void resetMaximumTemperatureLimit();

    bool setHeaterControlType(std::string heaterControlType);

    void resetHeaterControlType();

    bool setHeaterMaximumCapacity(boost::optional<double> heaterMaximumCapacity);

    bool setHeaterMaximumCapacity(const OSOptionalQuantity& heaterMaximumCapacity);

    void resetHeaterMaximumCapacity();

    void autosizeHeaterMaximumCapacity();

    bool setHeaterMinimumCapacity(boost::optional<double> heaterMinimumCapacity);

    bool setHeaterMinimumCapacity(const OSOptionalQuantity& heaterMinimumCapacity);

    void resetHeaterMinimumCapacity();

    bool setHeaterIgnitionMinimumFlowRate(double heaterIgnitionMinimumFlowRate);

    bool setHeaterIgnitionMinimumFlowRate(const Quantity& heaterIgnitionMinimumFlowRate);

    void resetHeaterIgnitionMinimumFlowRate();

    bool setHeaterIgnitionDelay(double heaterIgnitionDelay);

    bool setHeaterIgnitionDelay(const Quantity& heaterIgnitionDelay);

    void resetHeaterIgnitionDelay();

    bool setHeaterFuelType(std::string heaterFuelType);

    bool setHeaterThermalEfficiency(boost::optional<double> heaterThermalEfficiency);

    bool setHeaterThermalEfficiency(const OSOptionalQuantity& heaterThermalEfficiency);

    void resetHeaterThermalEfficiency();

    bool setPartLoadFactorCurve(const boost::optional<CurveCubic>& partLoadFactorCurve);

    void resetPartLoadFactorCurve();

    bool setOffCycleParasiticFuelConsumptionRate(double offCycleParasiticFuelConsumptionRate);

    bool setOffCycleParasiticFuelConsumptionRate(const Quantity& offCycleParasiticFuelConsumptionRate);

    void resetOffCycleParasiticFuelConsumptionRate();

    bool setOffCycleParasiticFuelType(boost::optional<std::string> offCycleParasiticFuelType);

    void resetOffCycleParasiticFuelType();

    bool setOffCycleParasiticHeatFractiontoTank(double offCycleParasiticHeatFractiontoTank);

    bool setOffCycleParasiticHeatFractiontoTank(const Quantity& offCycleParasiticHeatFractiontoTank);

    void resetOffCycleParasiticHeatFractiontoTank();

    bool setOnCycleParasiticFuelConsumptionRate(double onCycleParasiticFuelConsumptionRate);

    bool setOnCycleParasiticFuelConsumptionRate(const Quantity& onCycleParasiticFuelConsumptionRate);

    void resetOnCycleParasiticFuelConsumptionRate();

    bool setOnCycleParasiticFuelType(boost::optional<std::string> onCycleParasiticFuelType);

    void resetOnCycleParasiticFuelType();

    bool setOnCycleParasiticHeatFractiontoTank(double onCycleParasiticHeatFractiontoTank);

    bool setOnCycleParasiticHeatFractiontoTank(const Quantity& onCycleParasiticHeatFractiontoTank);

    void resetOnCycleParasiticHeatFractiontoTank();

    bool setAmbientTemperatureIndicator(std::string ambientTemperatureIndicator);

    bool setAmbientTemperatureSchedule(Schedule& ambientTemperatureSchedule);

    void resetAmbientTemperatureSchedule();

    bool setAmbientTemperatureThermalZone(const boost::optional<ThermalZone>& ambientTemperatureThermalZone);

    void resetAmbientTemperatureThermalZone();

    bool setAmbientTemperatureOutdoorAirNodeName(boost::optional<std::string> ambientTemperatureOutdoorAirNodeName);

    void resetAmbientTemperatureOutdoorAirNodeName();

    bool setOffCycleLossCoefficienttoAmbientTemperature(boost::optional<double> offCycleLossCoefficienttoAmbientTemperature);

    bool setOffCycleLossCoefficienttoAmbientTemperature(const OSOptionalQuantity& offCycleLossCoefficienttoAmbientTemperature);

    void resetOffCycleLossCoefficienttoAmbientTemperature();

    bool setOffCycleLossFractiontoThermalZone(double offCycleLossFractiontoThermalZone);

    bool setOffCycleLossFractiontoThermalZone(const Quantity& offCycleLossFractiontoThermalZone);

    void resetOffCycleLossFractiontoThermalZone();

    bool setOnCycleLossCoefficienttoAmbientTemperature(boost::optional<double> onCycleLossCoefficienttoAmbientTemperature);

    bool setOnCycleLossCoefficienttoAmbientTemperature(const OSOptionalQuantity& onCycleLossCoefficienttoAmbientTemperature);

    void resetOnCycleLossCoefficienttoAmbientTemperature();

    bool setOnCycleLossFractiontoThermalZone(double onCycleLossFractiontoThermalZone);

    bool setOnCycleLossFractiontoThermalZone(const Quantity& onCycleLossFractiontoThermalZone);

    void resetOnCycleLossFractiontoThermalZone();

    bool setPeakUseFlowRate(boost::optional<double> peakUseFlowRate);

    bool setPeakUseFlowRate(const OSOptionalQuantity& peakUseFlowRate);

    void resetPeakUseFlowRate();

    bool setUseFlowRateFractionSchedule(Schedule& useFlowRateFractionSchedule);

    void resetUseFlowRateFractionSchedule();

    bool setColdWaterSupplyTemperatureSchedule(Schedule& coldWaterSupplyTemperatureSchedule);

    void resetColdWaterSupplyTemperatureSchedule();

    bool setUseSideEffectiveness(double useSideEffectiveness);

    bool setUseSideEffectiveness(const Quantity& useSideEffectiveness);

    void resetUseSideEffectiveness();

    bool setSourceSideEffectiveness(double sourceSideEffectiveness);

    bool setSourceSideEffectiveness(const Quantity& sourceSideEffectiveness);

    void resetSourceSideEffectiveness();

    bool setUseSideDesignFlowRate(boost::optional<double> useSideDesignFlowRate);

    bool setUseSideDesignFlowRate(const OSOptionalQuantity& useSideDesignFlowRate);

    void resetUseSideDesignFlowRate();

    void autosizeUseSideDesignFlowRate();

    bool setSourceSideDesignFlowRate(boost::optional<double> sourceSideDesignFlowRate);

    bool setSourceSideDesignFlowRate(const OSOptionalQuantity& sourceSideDesignFlowRate);

    void resetSourceSideDesignFlowRate();

    void autosizeSourceSideDesignFlowRate();

    bool setIndirectWaterHeatingRecoveryTime(double indirectWaterHeatingRecoveryTime);

    bool setIndirectWaterHeatingRecoveryTime(const Quantity& indirectWaterHeatingRecoveryTime);

    void resetIndirectWaterHeatingRecoveryTime();

    bool setSourceSideFlowControlMode(const std::string & sourceSideFlowControlMode);

    bool setIndirectAlternateSetpointTemperatureSchedule(Schedule& indirectAlternateSetpointTemperatureSchedule);
    void resetIndirectAlternateSetpointTemperatureSchedule();

    bool setEndUseSubcategory(const std::string & endUseSubcategory);

    //@}
   protected:

   private:

    REGISTER_LOGGER("openstudio.model.WaterHeaterMixed");

    openstudio::OSOptionalQuantity tankVolume_SI() const;
    openstudio::OSOptionalQuantity tankVolume_IP() const;
    openstudio::Quantity deadbandTemperatureDifference_SI() const;
    openstudio::Quantity deadbandTemperatureDifference_IP() const;
    openstudio::OSOptionalQuantity maximumTemperatureLimit_SI() const;
    openstudio::OSOptionalQuantity maximumTemperatureLimit_IP() const;
    std::vector<std::string> heaterControlTypeValues() const;
    openstudio::OSOptionalQuantity heaterMaximumCapacity_SI() const;
    openstudio::OSOptionalQuantity heaterMaximumCapacity_IP() const;
    openstudio::OSOptionalQuantity heaterMinimumCapacity_SI() const;
    openstudio::OSOptionalQuantity heaterMinimumCapacity_IP() const;
    openstudio::Quantity heaterIgnitionMinimumFlowRate_SI() const;
    openstudio::Quantity heaterIgnitionMinimumFlowRate_IP() const;
    openstudio::Quantity heaterIgnitionDelay_SI() const;
    openstudio::Quantity heaterIgnitionDelay_IP() const;
    std::vector<std::string> heaterFuelTypeValues() const;
    openstudio::OSOptionalQuantity heaterThermalEfficiency_SI() const;
    openstudio::OSOptionalQuantity heaterThermalEfficiency_IP() const;
    openstudio::Quantity offCycleParasiticFuelConsumptionRate_SI() const;
    openstudio::Quantity offCycleParasiticFuelConsumptionRate_IP() const;
    std::vector<std::string> offCycleParasiticFuelTypeValues() const;
    openstudio::Quantity offCycleParasiticHeatFractiontoTank_SI() const;
    openstudio::Quantity offCycleParasiticHeatFractiontoTank_IP() const;
    openstudio::Quantity onCycleParasiticFuelConsumptionRate_SI() const;
    openstudio::Quantity onCycleParasiticFuelConsumptionRate_IP() const;
    std::vector<std::string> onCycleParasiticFuelTypeValues() const;
    openstudio::Quantity onCycleParasiticHeatFractiontoTank_SI() const;
    openstudio::Quantity onCycleParasiticHeatFractiontoTank_IP() const;
    std::vector<std::string> ambientTemperatureIndicatorValues() const;
    openstudio::OSOptionalQuantity offCycleLossCoefficienttoAmbientTemperature_SI() const;
    openstudio::OSOptionalQuantity offCycleLossCoefficienttoAmbientTemperature_IP() const;
    openstudio::Quantity offCycleLossFractiontoThermalZone_SI() const;
    openstudio::Quantity offCycleLossFractiontoThermalZone_IP() const;
    openstudio::OSOptionalQuantity onCycleLossCoefficienttoAmbientTemperature_SI() const;
    openstudio::OSOptionalQuantity onCycleLossCoefficienttoAmbientTemperature_IP() const;
    openstudio::Quantity onCycleLossFractiontoThermalZone_SI() const;
    openstudio::Quantity onCycleLossFractiontoThermalZone_IP() const;
    openstudio::OSOptionalQuantity peakUseFlowRate_SI() const;
    openstudio::OSOptionalQuantity peakUseFlowRate_IP() const;
    openstudio::Quantity useSideEffectiveness_SI() const;
    openstudio::Quantity useSideEffectiveness_IP() const;
    openstudio::Quantity sourceSideEffectiveness_SI() const;
    openstudio::Quantity sourceSideEffectiveness_IP() const;
    openstudio::OSOptionalQuantity useSideDesignFlowRate_SI() const;
    openstudio::OSOptionalQuantity useSideDesignFlowRate_IP() const;
    openstudio::OSOptionalQuantity sourceSideDesignFlowRate_SI() const;
    openstudio::OSOptionalQuantity sourceSideDesignFlowRate_IP() const;
    openstudio::Quantity indirectWaterHeatingRecoveryTime_SI() const;
    openstudio::Quantity indirectWaterHeatingRecoveryTime_IP() const;
    std::vector<std::string> sourceSideFlowControlModeValues() const;

    boost::optional<ModelObject> setpointTemperatureScheduleAsModelObject() const;
    boost::optional<ModelObject> partLoadFactorCurveAsModelObject() const;
    boost::optional<ModelObject> ambientTemperatureScheduleAsModelObject() const;
    boost::optional<ModelObject> ambientTemperatureThermalZoneAsModelObject() const;
    boost::optional<ModelObject> useFlowRateFractionScheduleAsModelObject() const;
    boost::optional<ModelObject> coldWaterSupplyTemperatureScheduleAsModelObject() const;

    boost::optional<ModelObject> indirectAlternateSetpointTemperatureScheduleAsModelObject() const;

    bool setSetpointTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setPartLoadFactorCurveAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setAmbientTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setAmbientTemperatureThermalZoneAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setUseFlowRateFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setColdWaterSupplyTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);

    bool setIndirectAlternateSetpointTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);

  };

} // detail

} // model
} // openstudio

#endif // MODEL_WATERHEATERMIXED_IMPL_HPP
