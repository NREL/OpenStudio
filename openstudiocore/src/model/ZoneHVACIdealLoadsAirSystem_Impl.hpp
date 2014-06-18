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

#ifndef MODEL_ZONEHVACIDEALLOADSAIRSYSTEM_IMPL_HPP
#define MODEL_ZONEHVACIDEALLOADSAIRSYSTEM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Schedule;
class DesignSpecificationOutdoorAir;

namespace detail {

  /** ZoneHVACIdealLoadsAirSystem_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACIdealLoadsAirSystem.*/
  class MODEL_API ZoneHVACIdealLoadsAirSystem_Impl : public ZoneHVACComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(double maximumHeatingSupplyAirTemperature READ maximumHeatingSupplyAirTemperature WRITE setMaximumHeatingSupplyAirTemperature RESET resetMaximumHeatingSupplyAirTemperature);
    Q_PROPERTY(openstudio::Quantity maximumHeatingSupplyAirTemperature_SI READ maximumHeatingSupplyAirTemperature_SI WRITE setMaximumHeatingSupplyAirTemperature RESET resetMaximumHeatingSupplyAirTemperature);
    Q_PROPERTY(openstudio::Quantity maximumHeatingSupplyAirTemperature_IP READ maximumHeatingSupplyAirTemperature_IP WRITE setMaximumHeatingSupplyAirTemperature RESET resetMaximumHeatingSupplyAirTemperature);
    Q_PROPERTY(bool isMaximumHeatingSupplyAirTemperatureDefaulted READ isMaximumHeatingSupplyAirTemperatureDefaulted);

    Q_PROPERTY(double minimumCoolingSupplyAirTemperature READ minimumCoolingSupplyAirTemperature WRITE setMinimumCoolingSupplyAirTemperature RESET resetMinimumCoolingSupplyAirTemperature);
    Q_PROPERTY(openstudio::Quantity minimumCoolingSupplyAirTemperature_SI READ minimumCoolingSupplyAirTemperature_SI WRITE setMinimumCoolingSupplyAirTemperature RESET resetMinimumCoolingSupplyAirTemperature);
    Q_PROPERTY(openstudio::Quantity minimumCoolingSupplyAirTemperature_IP READ minimumCoolingSupplyAirTemperature_IP WRITE setMinimumCoolingSupplyAirTemperature RESET resetMinimumCoolingSupplyAirTemperature);
    Q_PROPERTY(bool isMinimumCoolingSupplyAirTemperatureDefaulted READ isMinimumCoolingSupplyAirTemperatureDefaulted);

    Q_PROPERTY(double maximumHeatingSupplyAirHumidityRatio READ maximumHeatingSupplyAirHumidityRatio WRITE setMaximumHeatingSupplyAirHumidityRatio RESET resetMaximumHeatingSupplyAirHumidityRatio);
    Q_PROPERTY(openstudio::Quantity maximumHeatingSupplyAirHumidityRatio_SI READ maximumHeatingSupplyAirHumidityRatio_SI WRITE setMaximumHeatingSupplyAirHumidityRatio RESET resetMaximumHeatingSupplyAirHumidityRatio);
    Q_PROPERTY(openstudio::Quantity maximumHeatingSupplyAirHumidityRatio_IP READ maximumHeatingSupplyAirHumidityRatio_IP WRITE setMaximumHeatingSupplyAirHumidityRatio RESET resetMaximumHeatingSupplyAirHumidityRatio);
    Q_PROPERTY(bool isMaximumHeatingSupplyAirHumidityRatioDefaulted READ isMaximumHeatingSupplyAirHumidityRatioDefaulted);

    Q_PROPERTY(double minimumCoolingSupplyAirHumidityRatio READ minimumCoolingSupplyAirHumidityRatio WRITE setMinimumCoolingSupplyAirHumidityRatio RESET resetMinimumCoolingSupplyAirHumidityRatio);
    Q_PROPERTY(openstudio::Quantity minimumCoolingSupplyAirHumidityRatio_SI READ minimumCoolingSupplyAirHumidityRatio_SI WRITE setMinimumCoolingSupplyAirHumidityRatio RESET resetMinimumCoolingSupplyAirHumidityRatio);
    Q_PROPERTY(openstudio::Quantity minimumCoolingSupplyAirHumidityRatio_IP READ minimumCoolingSupplyAirHumidityRatio_IP WRITE setMinimumCoolingSupplyAirHumidityRatio RESET resetMinimumCoolingSupplyAirHumidityRatio);
    Q_PROPERTY(bool isMinimumCoolingSupplyAirHumidityRatioDefaulted READ isMinimumCoolingSupplyAirHumidityRatioDefaulted);

    Q_PROPERTY(std::string heatingLimit READ heatingLimit WRITE setHeatingLimit RESET resetHeatingLimit);
    Q_PROPERTY(bool isHeatingLimitDefaulted READ isHeatingLimitDefaulted);
    Q_PROPERTY(std::vector<std::string> heatingLimitValues READ heatingLimitValues);

    Q_PROPERTY(boost::optional<double> maximumHeatingAirFlowRate READ maximumHeatingAirFlowRate WRITE setMaximumHeatingAirFlowRate RESET resetMaximumHeatingAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumHeatingAirFlowRate_SI READ maximumHeatingAirFlowRate_SI WRITE setMaximumHeatingAirFlowRate RESET resetMaximumHeatingAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumHeatingAirFlowRate_IP READ maximumHeatingAirFlowRate_IP WRITE setMaximumHeatingAirFlowRate RESET resetMaximumHeatingAirFlowRate);
    Q_PROPERTY(bool isMaximumHeatingAirFlowRateAutosized READ isMaximumHeatingAirFlowRateAutosized);

    Q_PROPERTY(boost::optional<double> maximumSensibleHeatingCapacity READ maximumSensibleHeatingCapacity WRITE setMaximumSensibleHeatingCapacity RESET resetMaximumSensibleHeatingCapacity);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumSensibleHeatingCapacity_SI READ maximumSensibleHeatingCapacity_SI WRITE setMaximumSensibleHeatingCapacity RESET resetMaximumSensibleHeatingCapacity);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumSensibleHeatingCapacity_IP READ maximumSensibleHeatingCapacity_IP WRITE setMaximumSensibleHeatingCapacity RESET resetMaximumSensibleHeatingCapacity);
    Q_PROPERTY(bool isMaximumSensibleHeatingCapacityAutosized READ isMaximumSensibleHeatingCapacityAutosized);

    Q_PROPERTY(std::string coolingLimit READ coolingLimit WRITE setCoolingLimit RESET resetCoolingLimit);
    Q_PROPERTY(bool isCoolingLimitDefaulted READ isCoolingLimitDefaulted);
    Q_PROPERTY(std::vector<std::string> coolingLimitValues READ coolingLimitValues);

    Q_PROPERTY(boost::optional<double> maximumCoolingAirFlowRate READ maximumCoolingAirFlowRate WRITE setMaximumCoolingAirFlowRate RESET resetMaximumCoolingAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumCoolingAirFlowRate_SI READ maximumCoolingAirFlowRate_SI WRITE setMaximumCoolingAirFlowRate RESET resetMaximumCoolingAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumCoolingAirFlowRate_IP READ maximumCoolingAirFlowRate_IP WRITE setMaximumCoolingAirFlowRate RESET resetMaximumCoolingAirFlowRate);
    Q_PROPERTY(bool isMaximumCoolingAirFlowRateAutosized READ isMaximumCoolingAirFlowRateAutosized);

    Q_PROPERTY(boost::optional<double> maximumTotalCoolingCapacity READ maximumTotalCoolingCapacity WRITE setMaximumTotalCoolingCapacity RESET resetMaximumTotalCoolingCapacity);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumTotalCoolingCapacity_SI READ maximumTotalCoolingCapacity_SI WRITE setMaximumTotalCoolingCapacity RESET resetMaximumTotalCoolingCapacity);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumTotalCoolingCapacity_IP READ maximumTotalCoolingCapacity_IP WRITE setMaximumTotalCoolingCapacity RESET resetMaximumTotalCoolingCapacity);
    Q_PROPERTY(bool isMaximumTotalCoolingCapacityAutosized READ isMaximumTotalCoolingCapacityAutosized);

    Q_PROPERTY(std::string dehumidificationControlType READ dehumidificationControlType WRITE setDehumidificationControlType RESET resetDehumidificationControlType);
    Q_PROPERTY(bool isDehumidificationControlTypeDefaulted READ isDehumidificationControlTypeDefaulted);
    Q_PROPERTY(std::vector<std::string> dehumidificationControlTypeValues READ dehumidificationControlTypeValues);

    Q_PROPERTY(double coolingSensibleHeatRatio READ coolingSensibleHeatRatio WRITE setCoolingSensibleHeatRatio RESET resetCoolingSensibleHeatRatio);
    Q_PROPERTY(openstudio::Quantity coolingSensibleHeatRatio_SI READ coolingSensibleHeatRatio_SI WRITE setCoolingSensibleHeatRatio RESET resetCoolingSensibleHeatRatio);
    Q_PROPERTY(openstudio::Quantity coolingSensibleHeatRatio_IP READ coolingSensibleHeatRatio_IP WRITE setCoolingSensibleHeatRatio RESET resetCoolingSensibleHeatRatio);
    Q_PROPERTY(bool isCoolingSensibleHeatRatioDefaulted READ isCoolingSensibleHeatRatioDefaulted);

    Q_PROPERTY(std::string humidificationControlType READ humidificationControlType WRITE setHumidificationControlType RESET resetHumidificationControlType);
    Q_PROPERTY(bool isHumidificationControlTypeDefaulted READ isHumidificationControlTypeDefaulted);
    Q_PROPERTY(std::vector<std::string> humidificationControlTypeValues READ humidificationControlTypeValues);

    Q_PROPERTY(std::string demandControlledVentilationType READ demandControlledVentilationType WRITE setDemandControlledVentilationType RESET resetDemandControlledVentilationType);
    Q_PROPERTY(bool isDemandControlledVentilationTypeDefaulted READ isDemandControlledVentilationTypeDefaulted);
    Q_PROPERTY(std::vector<std::string> demandControlledVentilationTypeValues READ demandControlledVentilationTypeValues);

    Q_PROPERTY(std::string outdoorAirEconomizerType READ outdoorAirEconomizerType WRITE setOutdoorAirEconomizerType RESET resetOutdoorAirEconomizerType);
    Q_PROPERTY(bool isOutdoorAirEconomizerTypeDefaulted READ isOutdoorAirEconomizerTypeDefaulted);
    Q_PROPERTY(std::vector<std::string> outdoorAirEconomizerTypeValues READ outdoorAirEconomizerTypeValues);

    Q_PROPERTY(std::string heatRecoveryType READ heatRecoveryType WRITE setHeatRecoveryType RESET resetHeatRecoveryType);
    Q_PROPERTY(bool isHeatRecoveryTypeDefaulted READ isHeatRecoveryTypeDefaulted);
    Q_PROPERTY(std::vector<std::string> heatRecoveryTypeValues READ heatRecoveryTypeValues);

    Q_PROPERTY(double sensibleHeatRecoveryEffectiveness READ sensibleHeatRecoveryEffectiveness WRITE setSensibleHeatRecoveryEffectiveness RESET resetSensibleHeatRecoveryEffectiveness);
    Q_PROPERTY(openstudio::Quantity sensibleHeatRecoveryEffectiveness_SI READ sensibleHeatRecoveryEffectiveness_SI WRITE setSensibleHeatRecoveryEffectiveness RESET resetSensibleHeatRecoveryEffectiveness);
    Q_PROPERTY(openstudio::Quantity sensibleHeatRecoveryEffectiveness_IP READ sensibleHeatRecoveryEffectiveness_IP WRITE setSensibleHeatRecoveryEffectiveness RESET resetSensibleHeatRecoveryEffectiveness);
    Q_PROPERTY(bool isSensibleHeatRecoveryEffectivenessDefaulted READ isSensibleHeatRecoveryEffectivenessDefaulted);

    Q_PROPERTY(double latentHeatRecoveryEffectiveness READ latentHeatRecoveryEffectiveness WRITE setLatentHeatRecoveryEffectiveness RESET resetLatentHeatRecoveryEffectiveness);
    Q_PROPERTY(openstudio::Quantity latentHeatRecoveryEffectiveness_SI READ latentHeatRecoveryEffectiveness_SI WRITE setLatentHeatRecoveryEffectiveness RESET resetLatentHeatRecoveryEffectiveness);
    Q_PROPERTY(openstudio::Quantity latentHeatRecoveryEffectiveness_IP READ latentHeatRecoveryEffectiveness_IP WRITE setLatentHeatRecoveryEffectiveness RESET resetLatentHeatRecoveryEffectiveness);
    Q_PROPERTY(bool isLatentHeatRecoveryEffectivenessDefaulted READ isLatentHeatRecoveryEffectivenessDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject RESET resetAvailabilitySchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> heatingAvailabilitySchedule READ heatingAvailabilityScheduleAsModelObject WRITE setHeatingAvailabilityScheduleAsModelObject RESET resetHeatingAvailabilitySchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> coolingAvailabilitySchedule READ coolingAvailabilityScheduleAsModelObject WRITE setCoolingAvailabilityScheduleAsModelObject RESET resetCoolingAvailabilitySchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> designSpecificationOutdoorAirObject READ designSpecificationOutdoorAirObjectAsModelObject WRITE setDesignSpecificationOutdoorAirObjectAsModelObject RESET resetDesignSpecificationOutdoorAirObject);
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACIdealLoadsAirSystem_Impl(const IdfObject& idfObject,
                                     Model_Impl* model,
                                     bool keepHandle);

    ZoneHVACIdealLoadsAirSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    ZoneHVACIdealLoadsAirSystem_Impl(const ZoneHVACIdealLoadsAirSystem_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    virtual ~ZoneHVACIdealLoadsAirSystem_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    double maximumHeatingSupplyAirTemperature() const;

    Quantity getMaximumHeatingSupplyAirTemperature(bool returnIP=false) const;

    bool isMaximumHeatingSupplyAirTemperatureDefaulted() const;

    double minimumCoolingSupplyAirTemperature() const;

    Quantity getMinimumCoolingSupplyAirTemperature(bool returnIP=false) const;

    bool isMinimumCoolingSupplyAirTemperatureDefaulted() const;

    double maximumHeatingSupplyAirHumidityRatio() const;

    Quantity getMaximumHeatingSupplyAirHumidityRatio(bool returnIP=false) const;

    bool isMaximumHeatingSupplyAirHumidityRatioDefaulted() const;

    double minimumCoolingSupplyAirHumidityRatio() const;

    Quantity getMinimumCoolingSupplyAirHumidityRatio(bool returnIP=false) const;

    bool isMinimumCoolingSupplyAirHumidityRatioDefaulted() const;

    std::string heatingLimit() const;

    bool isHeatingLimitDefaulted() const;

    boost::optional<double> maximumHeatingAirFlowRate() const;

    OSOptionalQuantity getMaximumHeatingAirFlowRate(bool returnIP=false) const;

    bool isMaximumHeatingAirFlowRateAutosized() const;

    boost::optional<double> maximumSensibleHeatingCapacity() const;

    OSOptionalQuantity getMaximumSensibleHeatingCapacity(bool returnIP=false) const;

    bool isMaximumSensibleHeatingCapacityAutosized() const;

    std::string coolingLimit() const;

    bool isCoolingLimitDefaulted() const;

    boost::optional<double> maximumCoolingAirFlowRate() const;

    OSOptionalQuantity getMaximumCoolingAirFlowRate(bool returnIP=false) const;

    bool isMaximumCoolingAirFlowRateAutosized() const;

    boost::optional<double> maximumTotalCoolingCapacity() const;

    OSOptionalQuantity getMaximumTotalCoolingCapacity(bool returnIP=false) const;

    bool isMaximumTotalCoolingCapacityAutosized() const;

    boost::optional<Schedule> heatingAvailabilitySchedule() const;

    boost::optional<Schedule> coolingAvailabilitySchedule() const;

    std::string dehumidificationControlType() const;

    bool isDehumidificationControlTypeDefaulted() const;

    double coolingSensibleHeatRatio() const;

    Quantity getCoolingSensibleHeatRatio(bool returnIP=false) const;

    bool isCoolingSensibleHeatRatioDefaulted() const;

    std::string humidificationControlType() const;

    bool isHumidificationControlTypeDefaulted() const;

    boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAirObject() const;

    std::string demandControlledVentilationType() const;

    bool isDemandControlledVentilationTypeDefaulted() const;

    std::string outdoorAirEconomizerType() const;

    bool isOutdoorAirEconomizerTypeDefaulted() const;

    std::string heatRecoveryType() const;

    bool isHeatRecoveryTypeDefaulted() const;

    double sensibleHeatRecoveryEffectiveness() const;

    Quantity getSensibleHeatRecoveryEffectiveness(bool returnIP=false) const;

    bool isSensibleHeatRecoveryEffectivenessDefaulted() const;

    double latentHeatRecoveryEffectiveness() const;

    Quantity getLatentHeatRecoveryEffectiveness(bool returnIP=false) const;

    bool isLatentHeatRecoveryEffectivenessDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& availabilitySchedule);

    void resetAvailabilitySchedule();

    bool setMaximumHeatingSupplyAirTemperature(double maximumHeatingSupplyAirTemperature);

    bool setMaximumHeatingSupplyAirTemperature(const Quantity& maximumHeatingSupplyAirTemperature);

    void resetMaximumHeatingSupplyAirTemperature();

    bool setMinimumCoolingSupplyAirTemperature(double minimumCoolingSupplyAirTemperature);

    bool setMinimumCoolingSupplyAirTemperature(const Quantity& minimumCoolingSupplyAirTemperature);

    void resetMinimumCoolingSupplyAirTemperature();

    bool setMaximumHeatingSupplyAirHumidityRatio(double maximumHeatingSupplyAirHumidityRatio);

    bool setMaximumHeatingSupplyAirHumidityRatio(const Quantity& maximumHeatingSupplyAirHumidityRatio);

    void resetMaximumHeatingSupplyAirHumidityRatio();

    bool setMinimumCoolingSupplyAirHumidityRatio(double minimumCoolingSupplyAirHumidityRatio);

    bool setMinimumCoolingSupplyAirHumidityRatio(const Quantity& minimumCoolingSupplyAirHumidityRatio);

    void resetMinimumCoolingSupplyAirHumidityRatio();

    bool setHeatingLimit(std::string heatingLimit);

    void resetHeatingLimit();

    bool setMaximumHeatingAirFlowRate(boost::optional<double> maximumHeatingAirFlowRate);

    bool setMaximumHeatingAirFlowRate(const OSOptionalQuantity& maximumHeatingAirFlowRate);

    void resetMaximumHeatingAirFlowRate();

    void autosizeMaximumHeatingAirFlowRate();

    bool setMaximumSensibleHeatingCapacity(boost::optional<double> maximumSensibleHeatingCapacity);

    bool setMaximumSensibleHeatingCapacity(const OSOptionalQuantity& maximumSensibleHeatingCapacity);

    void resetMaximumSensibleHeatingCapacity();

    void autosizeMaximumSensibleHeatingCapacity();

    bool setCoolingLimit(std::string coolingLimit);

    void resetCoolingLimit();

    bool setMaximumCoolingAirFlowRate(boost::optional<double> maximumCoolingAirFlowRate);

    bool setMaximumCoolingAirFlowRate(const OSOptionalQuantity& maximumCoolingAirFlowRate);

    void resetMaximumCoolingAirFlowRate();

    void autosizeMaximumCoolingAirFlowRate();

    bool setMaximumTotalCoolingCapacity(boost::optional<double> maximumTotalCoolingCapacity);

    bool setMaximumTotalCoolingCapacity(const OSOptionalQuantity& maximumTotalCoolingCapacity);

    void resetMaximumTotalCoolingCapacity();

    void autosizeMaximumTotalCoolingCapacity();

    bool setHeatingAvailabilitySchedule(Schedule& heatingAvailabilitySchedule);

    void resetHeatingAvailabilitySchedule();

    bool setCoolingAvailabilitySchedule(Schedule& coolingAvailabilitySchedule);

    void resetCoolingAvailabilitySchedule();

    bool setDehumidificationControlType(std::string dehumidificationControlType);

    void resetDehumidificationControlType();

    bool setCoolingSensibleHeatRatio(double coolingSensibleHeatRatio);

    bool setCoolingSensibleHeatRatio(const Quantity& coolingSensibleHeatRatio);

    void resetCoolingSensibleHeatRatio();

    bool setHumidificationControlType(std::string humidificationControlType);

    void resetHumidificationControlType();

    bool setDesignSpecificationOutdoorAirObject(const boost::optional<DesignSpecificationOutdoorAir>& designSpecificationOutdoorAirObject);

    void resetDesignSpecificationOutdoorAirObject();

    bool setDemandControlledVentilationType(std::string demandControlledVentilationType);

    void resetDemandControlledVentilationType();

    bool setOutdoorAirEconomizerType(std::string outdoorAirEconomizerType);

    void resetOutdoorAirEconomizerType();

    bool setHeatRecoveryType(std::string heatRecoveryType);

    void resetHeatRecoveryType();

    bool setSensibleHeatRecoveryEffectiveness(double sensibleHeatRecoveryEffectiveness);

    bool setSensibleHeatRecoveryEffectiveness(const Quantity& sensibleHeatRecoveryEffectiveness);

    void resetSensibleHeatRecoveryEffectiveness();

    bool setLatentHeatRecoveryEffectiveness(double latentHeatRecoveryEffectiveness);

    bool setLatentHeatRecoveryEffectiveness(const Quantity& latentHeatRecoveryEffectiveness);

    void resetLatentHeatRecoveryEffectiveness();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACIdealLoadsAirSystem");

    openstudio::Quantity maximumHeatingSupplyAirTemperature_SI() const;
    openstudio::Quantity maximumHeatingSupplyAirTemperature_IP() const;
    openstudio::Quantity minimumCoolingSupplyAirTemperature_SI() const;
    openstudio::Quantity minimumCoolingSupplyAirTemperature_IP() const;
    openstudio::Quantity maximumHeatingSupplyAirHumidityRatio_SI() const;
    openstudio::Quantity maximumHeatingSupplyAirHumidityRatio_IP() const;
    openstudio::Quantity minimumCoolingSupplyAirHumidityRatio_SI() const;
    openstudio::Quantity minimumCoolingSupplyAirHumidityRatio_IP() const;
    std::vector<std::string> heatingLimitValues() const;
    openstudio::OSOptionalQuantity maximumHeatingAirFlowRate_SI() const;
    openstudio::OSOptionalQuantity maximumHeatingAirFlowRate_IP() const;
    openstudio::OSOptionalQuantity maximumSensibleHeatingCapacity_SI() const;
    openstudio::OSOptionalQuantity maximumSensibleHeatingCapacity_IP() const;
    std::vector<std::string> coolingLimitValues() const;
    openstudio::OSOptionalQuantity maximumCoolingAirFlowRate_SI() const;
    openstudio::OSOptionalQuantity maximumCoolingAirFlowRate_IP() const;
    openstudio::OSOptionalQuantity maximumTotalCoolingCapacity_SI() const;
    openstudio::OSOptionalQuantity maximumTotalCoolingCapacity_IP() const;
    std::vector<std::string> dehumidificationControlTypeValues() const;
    openstudio::Quantity coolingSensibleHeatRatio_SI() const;
    openstudio::Quantity coolingSensibleHeatRatio_IP() const;
    std::vector<std::string> humidificationControlTypeValues() const;
    std::vector<std::string> demandControlledVentilationTypeValues() const;
    std::vector<std::string> outdoorAirEconomizerTypeValues() const;
    std::vector<std::string> heatRecoveryTypeValues() const;
    openstudio::Quantity sensibleHeatRecoveryEffectiveness_SI() const;
    openstudio::Quantity sensibleHeatRecoveryEffectiveness_IP() const;
    openstudio::Quantity latentHeatRecoveryEffectiveness_SI() const;
    openstudio::Quantity latentHeatRecoveryEffectiveness_IP() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
    boost::optional<ModelObject> heatingAvailabilityScheduleAsModelObject() const;
    boost::optional<ModelObject> coolingAvailabilityScheduleAsModelObject() const;
    boost::optional<ModelObject> designSpecificationOutdoorAirObjectAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setCoolingAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setDesignSpecificationOutdoorAirObjectAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACIDEALLOADSAIRSYSTEM_IMPL_HPP

