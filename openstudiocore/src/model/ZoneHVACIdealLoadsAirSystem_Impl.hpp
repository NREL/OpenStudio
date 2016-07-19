/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;

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

