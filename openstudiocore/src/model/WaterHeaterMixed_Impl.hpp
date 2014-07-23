/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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
    Q_OBJECT;

    Q_PROPERTY(boost::optional<double> tankVolume READ tankVolume WRITE setTankVolume RESET resetTankVolume);
    Q_PROPERTY(openstudio::OSOptionalQuantity tankVolume_SI READ tankVolume_SI WRITE setTankVolume RESET resetTankVolume);
    Q_PROPERTY(openstudio::OSOptionalQuantity tankVolume_IP READ tankVolume_IP WRITE setTankVolume RESET resetTankVolume);
    Q_PROPERTY(bool isTankVolumeDefaulted READ isTankVolumeDefaulted);
    Q_PROPERTY(bool isTankVolumeAutosized READ isTankVolumeAutosized);

    Q_PROPERTY(double deadbandTemperatureDifference READ deadbandTemperatureDifference WRITE setDeadbandTemperatureDifference RESET resetDeadbandTemperatureDifference);
    Q_PROPERTY(openstudio::Quantity deadbandTemperatureDifference_SI READ deadbandTemperatureDifference_SI WRITE setDeadbandTemperatureDifference RESET resetDeadbandTemperatureDifference);
    Q_PROPERTY(openstudio::Quantity deadbandTemperatureDifference_IP READ deadbandTemperatureDifference_IP WRITE setDeadbandTemperatureDifference RESET resetDeadbandTemperatureDifference);
    Q_PROPERTY(bool isDeadbandTemperatureDifferenceDefaulted READ isDeadbandTemperatureDifferenceDefaulted);

    Q_PROPERTY(boost::optional<double> maximumTemperatureLimit READ maximumTemperatureLimit WRITE setMaximumTemperatureLimit RESET resetMaximumTemperatureLimit);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumTemperatureLimit_SI READ maximumTemperatureLimit_SI WRITE setMaximumTemperatureLimit RESET resetMaximumTemperatureLimit);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumTemperatureLimit_IP READ maximumTemperatureLimit_IP WRITE setMaximumTemperatureLimit RESET resetMaximumTemperatureLimit);

    Q_PROPERTY(std::string heaterControlType READ heaterControlType WRITE setHeaterControlType RESET resetHeaterControlType);
    Q_PROPERTY(bool isHeaterControlTypeDefaulted READ isHeaterControlTypeDefaulted);
    Q_PROPERTY(std::vector<std::string> heaterControlTypeValues READ heaterControlTypeValues);

    Q_PROPERTY(boost::optional<double> heaterMaximumCapacity READ heaterMaximumCapacity WRITE setHeaterMaximumCapacity RESET resetHeaterMaximumCapacity);
    Q_PROPERTY(openstudio::OSOptionalQuantity heaterMaximumCapacity_SI READ heaterMaximumCapacity_SI WRITE setHeaterMaximumCapacity RESET resetHeaterMaximumCapacity);
    Q_PROPERTY(openstudio::OSOptionalQuantity heaterMaximumCapacity_IP READ heaterMaximumCapacity_IP WRITE setHeaterMaximumCapacity RESET resetHeaterMaximumCapacity);
    Q_PROPERTY(bool isHeaterMaximumCapacityAutosized READ isHeaterMaximumCapacityAutosized);

    Q_PROPERTY(boost::optional<double> heaterMinimumCapacity READ heaterMinimumCapacity WRITE setHeaterMinimumCapacity RESET resetHeaterMinimumCapacity);
    Q_PROPERTY(openstudio::OSOptionalQuantity heaterMinimumCapacity_SI READ heaterMinimumCapacity_SI WRITE setHeaterMinimumCapacity RESET resetHeaterMinimumCapacity);
    Q_PROPERTY(openstudio::OSOptionalQuantity heaterMinimumCapacity_IP READ heaterMinimumCapacity_IP WRITE setHeaterMinimumCapacity RESET resetHeaterMinimumCapacity);

    Q_PROPERTY(double heaterIgnitionMinimumFlowRate READ heaterIgnitionMinimumFlowRate WRITE setHeaterIgnitionMinimumFlowRate RESET resetHeaterIgnitionMinimumFlowRate);
    Q_PROPERTY(openstudio::Quantity heaterIgnitionMinimumFlowRate_SI READ heaterIgnitionMinimumFlowRate_SI WRITE setHeaterIgnitionMinimumFlowRate RESET resetHeaterIgnitionMinimumFlowRate);
    Q_PROPERTY(openstudio::Quantity heaterIgnitionMinimumFlowRate_IP READ heaterIgnitionMinimumFlowRate_IP WRITE setHeaterIgnitionMinimumFlowRate RESET resetHeaterIgnitionMinimumFlowRate);
    Q_PROPERTY(bool isHeaterIgnitionMinimumFlowRateDefaulted READ isHeaterIgnitionMinimumFlowRateDefaulted);

    Q_PROPERTY(double heaterIgnitionDelay READ heaterIgnitionDelay WRITE setHeaterIgnitionDelay RESET resetHeaterIgnitionDelay);
    Q_PROPERTY(openstudio::Quantity heaterIgnitionDelay_SI READ heaterIgnitionDelay_SI WRITE setHeaterIgnitionDelay RESET resetHeaterIgnitionDelay);
    Q_PROPERTY(openstudio::Quantity heaterIgnitionDelay_IP READ heaterIgnitionDelay_IP WRITE setHeaterIgnitionDelay RESET resetHeaterIgnitionDelay);
    Q_PROPERTY(bool isHeaterIgnitionDelayDefaulted READ isHeaterIgnitionDelayDefaulted);

    Q_PROPERTY(std::string heaterFuelType READ heaterFuelType WRITE setHeaterFuelType);
    Q_PROPERTY(std::vector<std::string> heaterFuelTypeValues READ heaterFuelTypeValues);

    Q_PROPERTY(boost::optional<double> heaterThermalEfficiency READ heaterThermalEfficiency WRITE setHeaterThermalEfficiency RESET resetHeaterThermalEfficiency);
    Q_PROPERTY(openstudio::OSOptionalQuantity heaterThermalEfficiency_SI READ heaterThermalEfficiency_SI WRITE setHeaterThermalEfficiency RESET resetHeaterThermalEfficiency);
    Q_PROPERTY(openstudio::OSOptionalQuantity heaterThermalEfficiency_IP READ heaterThermalEfficiency_IP WRITE setHeaterThermalEfficiency RESET resetHeaterThermalEfficiency);

    Q_PROPERTY(double offCycleParasiticFuelConsumptionRate READ offCycleParasiticFuelConsumptionRate WRITE setOffCycleParasiticFuelConsumptionRate RESET resetOffCycleParasiticFuelConsumptionRate);
    Q_PROPERTY(openstudio::Quantity offCycleParasiticFuelConsumptionRate_SI READ offCycleParasiticFuelConsumptionRate_SI WRITE setOffCycleParasiticFuelConsumptionRate RESET resetOffCycleParasiticFuelConsumptionRate);
    Q_PROPERTY(openstudio::Quantity offCycleParasiticFuelConsumptionRate_IP READ offCycleParasiticFuelConsumptionRate_IP WRITE setOffCycleParasiticFuelConsumptionRate RESET resetOffCycleParasiticFuelConsumptionRate);
    Q_PROPERTY(bool isOffCycleParasiticFuelConsumptionRateDefaulted READ isOffCycleParasiticFuelConsumptionRateDefaulted);

    Q_PROPERTY(boost::optional<std::string> offCycleParasiticFuelType READ offCycleParasiticFuelType WRITE setOffCycleParasiticFuelType RESET resetOffCycleParasiticFuelType);
    Q_PROPERTY(std::vector<std::string> offCycleParasiticFuelTypeValues READ offCycleParasiticFuelTypeValues);

    Q_PROPERTY(double offCycleParasiticHeatFractiontoTank READ offCycleParasiticHeatFractiontoTank WRITE setOffCycleParasiticHeatFractiontoTank RESET resetOffCycleParasiticHeatFractiontoTank);
    Q_PROPERTY(openstudio::Quantity offCycleParasiticHeatFractiontoTank_SI READ offCycleParasiticHeatFractiontoTank_SI WRITE setOffCycleParasiticHeatFractiontoTank RESET resetOffCycleParasiticHeatFractiontoTank);
    Q_PROPERTY(openstudio::Quantity offCycleParasiticHeatFractiontoTank_IP READ offCycleParasiticHeatFractiontoTank_IP WRITE setOffCycleParasiticHeatFractiontoTank RESET resetOffCycleParasiticHeatFractiontoTank);
    Q_PROPERTY(bool isOffCycleParasiticHeatFractiontoTankDefaulted READ isOffCycleParasiticHeatFractiontoTankDefaulted);

    Q_PROPERTY(double onCycleParasiticFuelConsumptionRate READ onCycleParasiticFuelConsumptionRate WRITE setOnCycleParasiticFuelConsumptionRate RESET resetOnCycleParasiticFuelConsumptionRate);
    Q_PROPERTY(openstudio::Quantity onCycleParasiticFuelConsumptionRate_SI READ onCycleParasiticFuelConsumptionRate_SI WRITE setOnCycleParasiticFuelConsumptionRate RESET resetOnCycleParasiticFuelConsumptionRate);
    Q_PROPERTY(openstudio::Quantity onCycleParasiticFuelConsumptionRate_IP READ onCycleParasiticFuelConsumptionRate_IP WRITE setOnCycleParasiticFuelConsumptionRate RESET resetOnCycleParasiticFuelConsumptionRate);
    Q_PROPERTY(bool isOnCycleParasiticFuelConsumptionRateDefaulted READ isOnCycleParasiticFuelConsumptionRateDefaulted);

    Q_PROPERTY(boost::optional<std::string> onCycleParasiticFuelType READ onCycleParasiticFuelType WRITE setOnCycleParasiticFuelType RESET resetOnCycleParasiticFuelType);
    Q_PROPERTY(std::vector<std::string> onCycleParasiticFuelTypeValues READ onCycleParasiticFuelTypeValues);

    Q_PROPERTY(double onCycleParasiticHeatFractiontoTank READ onCycleParasiticHeatFractiontoTank WRITE setOnCycleParasiticHeatFractiontoTank RESET resetOnCycleParasiticHeatFractiontoTank);
    Q_PROPERTY(openstudio::Quantity onCycleParasiticHeatFractiontoTank_SI READ onCycleParasiticHeatFractiontoTank_SI WRITE setOnCycleParasiticHeatFractiontoTank RESET resetOnCycleParasiticHeatFractiontoTank);
    Q_PROPERTY(openstudio::Quantity onCycleParasiticHeatFractiontoTank_IP READ onCycleParasiticHeatFractiontoTank_IP WRITE setOnCycleParasiticHeatFractiontoTank RESET resetOnCycleParasiticHeatFractiontoTank);
    Q_PROPERTY(bool isOnCycleParasiticHeatFractiontoTankDefaulted READ isOnCycleParasiticHeatFractiontoTankDefaulted);

    Q_PROPERTY(std::string ambientTemperatureIndicator READ ambientTemperatureIndicator WRITE setAmbientTemperatureIndicator);
    Q_PROPERTY(std::vector<std::string> ambientTemperatureIndicatorValues READ ambientTemperatureIndicatorValues);

    Q_PROPERTY(boost::optional<std::string> ambientTemperatureOutdoorAirNodeName READ ambientTemperatureOutdoorAirNodeName WRITE setAmbientTemperatureOutdoorAirNodeName RESET resetAmbientTemperatureOutdoorAirNodeName);

    Q_PROPERTY(boost::optional<double> offCycleLossCoefficienttoAmbientTemperature READ offCycleLossCoefficienttoAmbientTemperature WRITE setOffCycleLossCoefficienttoAmbientTemperature RESET resetOffCycleLossCoefficienttoAmbientTemperature);
    Q_PROPERTY(openstudio::OSOptionalQuantity offCycleLossCoefficienttoAmbientTemperature_SI READ offCycleLossCoefficienttoAmbientTemperature_SI WRITE setOffCycleLossCoefficienttoAmbientTemperature RESET resetOffCycleLossCoefficienttoAmbientTemperature);
    Q_PROPERTY(openstudio::OSOptionalQuantity offCycleLossCoefficienttoAmbientTemperature_IP READ offCycleLossCoefficienttoAmbientTemperature_IP WRITE setOffCycleLossCoefficienttoAmbientTemperature RESET resetOffCycleLossCoefficienttoAmbientTemperature);

    Q_PROPERTY(double offCycleLossFractiontoThermalZone READ offCycleLossFractiontoThermalZone WRITE setOffCycleLossFractiontoThermalZone RESET resetOffCycleLossFractiontoThermalZone);
    Q_PROPERTY(openstudio::Quantity offCycleLossFractiontoThermalZone_SI READ offCycleLossFractiontoThermalZone_SI WRITE setOffCycleLossFractiontoThermalZone RESET resetOffCycleLossFractiontoThermalZone);
    Q_PROPERTY(openstudio::Quantity offCycleLossFractiontoThermalZone_IP READ offCycleLossFractiontoThermalZone_IP WRITE setOffCycleLossFractiontoThermalZone RESET resetOffCycleLossFractiontoThermalZone);
    Q_PROPERTY(bool isOffCycleLossFractiontoThermalZoneDefaulted READ isOffCycleLossFractiontoThermalZoneDefaulted);

    Q_PROPERTY(boost::optional<double> onCycleLossCoefficienttoAmbientTemperature READ onCycleLossCoefficienttoAmbientTemperature WRITE setOnCycleLossCoefficienttoAmbientTemperature RESET resetOnCycleLossCoefficienttoAmbientTemperature);
    Q_PROPERTY(openstudio::OSOptionalQuantity onCycleLossCoefficienttoAmbientTemperature_SI READ onCycleLossCoefficienttoAmbientTemperature_SI WRITE setOnCycleLossCoefficienttoAmbientTemperature RESET resetOnCycleLossCoefficienttoAmbientTemperature);
    Q_PROPERTY(openstudio::OSOptionalQuantity onCycleLossCoefficienttoAmbientTemperature_IP READ onCycleLossCoefficienttoAmbientTemperature_IP WRITE setOnCycleLossCoefficienttoAmbientTemperature RESET resetOnCycleLossCoefficienttoAmbientTemperature);

    Q_PROPERTY(double onCycleLossFractiontoThermalZone READ onCycleLossFractiontoThermalZone WRITE setOnCycleLossFractiontoThermalZone RESET resetOnCycleLossFractiontoThermalZone);
    Q_PROPERTY(openstudio::Quantity onCycleLossFractiontoThermalZone_SI READ onCycleLossFractiontoThermalZone_SI WRITE setOnCycleLossFractiontoThermalZone RESET resetOnCycleLossFractiontoThermalZone);
    Q_PROPERTY(openstudio::Quantity onCycleLossFractiontoThermalZone_IP READ onCycleLossFractiontoThermalZone_IP WRITE setOnCycleLossFractiontoThermalZone RESET resetOnCycleLossFractiontoThermalZone);
    Q_PROPERTY(bool isOnCycleLossFractiontoThermalZoneDefaulted READ isOnCycleLossFractiontoThermalZoneDefaulted);

    Q_PROPERTY(boost::optional<double> peakUseFlowRate READ peakUseFlowRate WRITE setPeakUseFlowRate RESET resetPeakUseFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity peakUseFlowRate_SI READ peakUseFlowRate_SI WRITE setPeakUseFlowRate RESET resetPeakUseFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity peakUseFlowRate_IP READ peakUseFlowRate_IP WRITE setPeakUseFlowRate RESET resetPeakUseFlowRate);

    Q_PROPERTY(double useSideEffectiveness READ useSideEffectiveness WRITE setUseSideEffectiveness RESET resetUseSideEffectiveness);
    Q_PROPERTY(openstudio::Quantity useSideEffectiveness_SI READ useSideEffectiveness_SI WRITE setUseSideEffectiveness RESET resetUseSideEffectiveness);
    Q_PROPERTY(openstudio::Quantity useSideEffectiveness_IP READ useSideEffectiveness_IP WRITE setUseSideEffectiveness RESET resetUseSideEffectiveness);
    Q_PROPERTY(bool isUseSideEffectivenessDefaulted READ isUseSideEffectivenessDefaulted);

    Q_PROPERTY(double sourceSideEffectiveness READ sourceSideEffectiveness WRITE setSourceSideEffectiveness RESET resetSourceSideEffectiveness);
    Q_PROPERTY(openstudio::Quantity sourceSideEffectiveness_SI READ sourceSideEffectiveness_SI WRITE setSourceSideEffectiveness RESET resetSourceSideEffectiveness);
    Q_PROPERTY(openstudio::Quantity sourceSideEffectiveness_IP READ sourceSideEffectiveness_IP WRITE setSourceSideEffectiveness RESET resetSourceSideEffectiveness);
    Q_PROPERTY(bool isSourceSideEffectivenessDefaulted READ isSourceSideEffectivenessDefaulted);

    Q_PROPERTY(boost::optional<double> useSideDesignFlowRate READ useSideDesignFlowRate WRITE setUseSideDesignFlowRate RESET resetUseSideDesignFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity useSideDesignFlowRate_SI READ useSideDesignFlowRate_SI WRITE setUseSideDesignFlowRate RESET resetUseSideDesignFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity useSideDesignFlowRate_IP READ useSideDesignFlowRate_IP WRITE setUseSideDesignFlowRate RESET resetUseSideDesignFlowRate);
    Q_PROPERTY(bool isUseSideDesignFlowRateDefaulted READ isUseSideDesignFlowRateDefaulted);
    Q_PROPERTY(bool isUseSideDesignFlowRateAutosized READ isUseSideDesignFlowRateAutosized);

    Q_PROPERTY(boost::optional<double> sourceSideDesignFlowRate READ sourceSideDesignFlowRate WRITE setSourceSideDesignFlowRate RESET resetSourceSideDesignFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity sourceSideDesignFlowRate_SI READ sourceSideDesignFlowRate_SI WRITE setSourceSideDesignFlowRate RESET resetSourceSideDesignFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity sourceSideDesignFlowRate_IP READ sourceSideDesignFlowRate_IP WRITE setSourceSideDesignFlowRate RESET resetSourceSideDesignFlowRate);
    Q_PROPERTY(bool isSourceSideDesignFlowRateDefaulted READ isSourceSideDesignFlowRateDefaulted);
    Q_PROPERTY(bool isSourceSideDesignFlowRateAutosized READ isSourceSideDesignFlowRateAutosized);

    Q_PROPERTY(double indirectWaterHeatingRecoveryTime READ indirectWaterHeatingRecoveryTime WRITE setIndirectWaterHeatingRecoveryTime RESET resetIndirectWaterHeatingRecoveryTime);
    Q_PROPERTY(openstudio::Quantity indirectWaterHeatingRecoveryTime_SI READ indirectWaterHeatingRecoveryTime_SI WRITE setIndirectWaterHeatingRecoveryTime RESET resetIndirectWaterHeatingRecoveryTime);
    Q_PROPERTY(openstudio::Quantity indirectWaterHeatingRecoveryTime_IP READ indirectWaterHeatingRecoveryTime_IP WRITE setIndirectWaterHeatingRecoveryTime RESET resetIndirectWaterHeatingRecoveryTime);
    Q_PROPERTY(bool isIndirectWaterHeatingRecoveryTimeDefaulted READ isIndirectWaterHeatingRecoveryTimeDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> setpointTemperatureSchedule READ setpointTemperatureScheduleAsModelObject WRITE setSetpointTemperatureScheduleAsModelObject RESET resetSetpointTemperatureSchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> partLoadFactorCurve READ partLoadFactorCurveAsModelObject WRITE setPartLoadFactorCurveAsModelObject RESET resetPartLoadFactorCurve);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> ambientTemperatureSchedule READ ambientTemperatureScheduleAsModelObject WRITE setAmbientTemperatureScheduleAsModelObject RESET resetAmbientTemperatureSchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> ambientTemperatureThermalZone READ ambientTemperatureThermalZoneAsModelObject WRITE setAmbientTemperatureThermalZoneAsModelObject RESET resetAmbientTemperatureThermalZone);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> useFlowRateFractionSchedule READ useFlowRateFractionScheduleAsModelObject WRITE setUseFlowRateFractionScheduleAsModelObject RESET resetUseFlowRateFractionSchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> coldWaterSupplyTemperatureSchedule READ coldWaterSupplyTemperatureScheduleAsModelObject WRITE setColdWaterSupplyTemperatureScheduleAsModelObject RESET resetColdWaterSupplyTemperatureSchedule);

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

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual unsigned supplyInletPort();

    virtual unsigned supplyOutletPort();

    virtual unsigned demandInletPort();

    virtual unsigned demandOutletPort();

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

    void setMaximumTemperatureLimit(boost::optional<double> maximumTemperatureLimit);

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

    void setAmbientTemperatureOutdoorAirNodeName(boost::optional<std::string> ambientTemperatureOutdoorAirNodeName);

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

    boost::optional<ModelObject> setpointTemperatureScheduleAsModelObject() const;
    boost::optional<ModelObject> partLoadFactorCurveAsModelObject() const;
    boost::optional<ModelObject> ambientTemperatureScheduleAsModelObject() const;
    boost::optional<ModelObject> ambientTemperatureThermalZoneAsModelObject() const;
    boost::optional<ModelObject> useFlowRateFractionScheduleAsModelObject() const;
    boost::optional<ModelObject> coldWaterSupplyTemperatureScheduleAsModelObject() const;

    bool setSetpointTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setPartLoadFactorCurveAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setAmbientTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setAmbientTemperatureThermalZoneAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setUseFlowRateFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setColdWaterSupplyTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_WATERHEATERMIXED_IMPL_HPP

