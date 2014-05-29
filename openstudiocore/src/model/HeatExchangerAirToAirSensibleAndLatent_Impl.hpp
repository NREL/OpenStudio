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

#ifndef MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_IMPL_HPP
#define MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_IMPL_HPP

#include "ModelAPI.hpp"
#include "AirToAirComponent_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Connection;
class Connection;
class Connection;
class Connection;

namespace detail {

  /** HeatExchangerAirToAirSensibleAndLatent_Impl is a AirToAirComponent_Impl that is the implementation class for HeatExchangerAirToAirSensibleAndLatent.*/
  class MODEL_API HeatExchangerAirToAirSensibleAndLatent_Impl : public AirToAirComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<double> nominalSupplyAirFlowRate READ nominalSupplyAirFlowRate WRITE setNominalSupplyAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity nominalSupplyAirFlowRate_SI READ nominalSupplyAirFlowRate_SI WRITE setNominalSupplyAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity nominalSupplyAirFlowRate_IP READ nominalSupplyAirFlowRate_IP WRITE setNominalSupplyAirFlowRate);
    Q_PROPERTY(bool isNominalSupplyAirFlowRateAutosized READ isNominalSupplyAirFlowRateAutosized);

    Q_PROPERTY(double sensibleEffectivenessat100HeatingAirFlow READ sensibleEffectivenessat100HeatingAirFlow WRITE setSensibleEffectivenessat100HeatingAirFlow);
    Q_PROPERTY(openstudio::Quantity sensibleEffectivenessat100HeatingAirFlow_SI READ sensibleEffectivenessat100HeatingAirFlow_SI WRITE setSensibleEffectivenessat100HeatingAirFlow);
    Q_PROPERTY(openstudio::Quantity sensibleEffectivenessat100HeatingAirFlow_IP READ sensibleEffectivenessat100HeatingAirFlow_IP WRITE setSensibleEffectivenessat100HeatingAirFlow);

    Q_PROPERTY(double latentEffectivenessat100HeatingAirFlow READ latentEffectivenessat100HeatingAirFlow WRITE setLatentEffectivenessat100HeatingAirFlow);
    Q_PROPERTY(openstudio::Quantity latentEffectivenessat100HeatingAirFlow_SI READ latentEffectivenessat100HeatingAirFlow_SI WRITE setLatentEffectivenessat100HeatingAirFlow);
    Q_PROPERTY(openstudio::Quantity latentEffectivenessat100HeatingAirFlow_IP READ latentEffectivenessat100HeatingAirFlow_IP WRITE setLatentEffectivenessat100HeatingAirFlow);

    Q_PROPERTY(double sensibleEffectivenessat75HeatingAirFlow READ sensibleEffectivenessat75HeatingAirFlow WRITE setSensibleEffectivenessat75HeatingAirFlow);
    Q_PROPERTY(openstudio::Quantity sensibleEffectivenessat75HeatingAirFlow_SI READ sensibleEffectivenessat75HeatingAirFlow_SI WRITE setSensibleEffectivenessat75HeatingAirFlow);
    Q_PROPERTY(openstudio::Quantity sensibleEffectivenessat75HeatingAirFlow_IP READ sensibleEffectivenessat75HeatingAirFlow_IP WRITE setSensibleEffectivenessat75HeatingAirFlow);

    Q_PROPERTY(double latentEffectivenessat75HeatingAirFlow READ latentEffectivenessat75HeatingAirFlow WRITE setLatentEffectivenessat75HeatingAirFlow);
    Q_PROPERTY(openstudio::Quantity latentEffectivenessat75HeatingAirFlow_SI READ latentEffectivenessat75HeatingAirFlow_SI WRITE setLatentEffectivenessat75HeatingAirFlow);
    Q_PROPERTY(openstudio::Quantity latentEffectivenessat75HeatingAirFlow_IP READ latentEffectivenessat75HeatingAirFlow_IP WRITE setLatentEffectivenessat75HeatingAirFlow);

    Q_PROPERTY(double sensibleEffectivenessat100CoolingAirFlow READ sensibleEffectivenessat100CoolingAirFlow WRITE setSensibleEffectivenessat100CoolingAirFlow);
    Q_PROPERTY(openstudio::Quantity sensibleEffectivenessat100CoolingAirFlow_SI READ sensibleEffectivenessat100CoolingAirFlow_SI WRITE setSensibleEffectivenessat100CoolingAirFlow);
    Q_PROPERTY(openstudio::Quantity sensibleEffectivenessat100CoolingAirFlow_IP READ sensibleEffectivenessat100CoolingAirFlow_IP WRITE setSensibleEffectivenessat100CoolingAirFlow);

    Q_PROPERTY(double latentEffectivenessat100CoolingAirFlow READ latentEffectivenessat100CoolingAirFlow WRITE setLatentEffectivenessat100CoolingAirFlow);
    Q_PROPERTY(openstudio::Quantity latentEffectivenessat100CoolingAirFlow_SI READ latentEffectivenessat100CoolingAirFlow_SI WRITE setLatentEffectivenessat100CoolingAirFlow);
    Q_PROPERTY(openstudio::Quantity latentEffectivenessat100CoolingAirFlow_IP READ latentEffectivenessat100CoolingAirFlow_IP WRITE setLatentEffectivenessat100CoolingAirFlow);

    Q_PROPERTY(double sensibleEffectivenessat75CoolingAirFlow READ sensibleEffectivenessat75CoolingAirFlow WRITE setSensibleEffectivenessat75CoolingAirFlow);
    Q_PROPERTY(openstudio::Quantity sensibleEffectivenessat75CoolingAirFlow_SI READ sensibleEffectivenessat75CoolingAirFlow_SI WRITE setSensibleEffectivenessat75CoolingAirFlow);
    Q_PROPERTY(openstudio::Quantity sensibleEffectivenessat75CoolingAirFlow_IP READ sensibleEffectivenessat75CoolingAirFlow_IP WRITE setSensibleEffectivenessat75CoolingAirFlow);

    Q_PROPERTY(double latentEffectivenessat75CoolingAirFlow READ latentEffectivenessat75CoolingAirFlow WRITE setLatentEffectivenessat75CoolingAirFlow);
    Q_PROPERTY(openstudio::Quantity latentEffectivenessat75CoolingAirFlow_SI READ latentEffectivenessat75CoolingAirFlow_SI WRITE setLatentEffectivenessat75CoolingAirFlow);
    Q_PROPERTY(openstudio::Quantity latentEffectivenessat75CoolingAirFlow_IP READ latentEffectivenessat75CoolingAirFlow_IP WRITE setLatentEffectivenessat75CoolingAirFlow);

    Q_PROPERTY(double nominalElectricPower READ nominalElectricPower WRITE setNominalElectricPower);
    Q_PROPERTY(openstudio::Quantity nominalElectricPower_SI READ nominalElectricPower_SI WRITE setNominalElectricPower);
    Q_PROPERTY(openstudio::Quantity nominalElectricPower_IP READ nominalElectricPower_IP WRITE setNominalElectricPower);

    Q_PROPERTY(bool supplyAirOutletTemperatureControl READ supplyAirOutletTemperatureControl WRITE setSupplyAirOutletTemperatureControl);

    Q_PROPERTY(std::string heatExchangerType READ heatExchangerType WRITE setHeatExchangerType);
    Q_PROPERTY(std::vector<std::string> heatExchangerTypeValues READ heatExchangerTypeValues);

    Q_PROPERTY(std::string frostControlType READ frostControlType WRITE setFrostControlType);
    Q_PROPERTY(std::vector<std::string> frostControlTypeValues READ frostControlTypeValues);

    Q_PROPERTY(double thresholdTemperature READ thresholdTemperature WRITE setThresholdTemperature RESET resetThresholdTemperature);
    Q_PROPERTY(openstudio::Quantity thresholdTemperature_SI READ thresholdTemperature_SI WRITE setThresholdTemperature RESET resetThresholdTemperature);
    Q_PROPERTY(openstudio::Quantity thresholdTemperature_IP READ thresholdTemperature_IP WRITE setThresholdTemperature RESET resetThresholdTemperature);
    Q_PROPERTY(bool isThresholdTemperatureDefaulted READ isThresholdTemperatureDefaulted);

    Q_PROPERTY(boost::optional<double> initialDefrostTimeFraction READ initialDefrostTimeFraction WRITE setInitialDefrostTimeFraction RESET resetInitialDefrostTimeFraction);
    Q_PROPERTY(openstudio::OSOptionalQuantity initialDefrostTimeFraction_SI READ initialDefrostTimeFraction_SI WRITE setInitialDefrostTimeFraction RESET resetInitialDefrostTimeFraction);
    Q_PROPERTY(openstudio::OSOptionalQuantity initialDefrostTimeFraction_IP READ initialDefrostTimeFraction_IP WRITE setInitialDefrostTimeFraction RESET resetInitialDefrostTimeFraction);

    Q_PROPERTY(boost::optional<double> rateofDefrostTimeFractionIncrease READ rateofDefrostTimeFractionIncrease WRITE setRateofDefrostTimeFractionIncrease RESET resetRateofDefrostTimeFractionIncrease);
    Q_PROPERTY(openstudio::OSOptionalQuantity rateofDefrostTimeFractionIncrease_SI READ rateofDefrostTimeFractionIncrease_SI WRITE setRateofDefrostTimeFractionIncrease RESET resetRateofDefrostTimeFractionIncrease);
    Q_PROPERTY(openstudio::OSOptionalQuantity rateofDefrostTimeFractionIncrease_IP READ rateofDefrostTimeFractionIncrease_IP WRITE setRateofDefrostTimeFractionIncrease RESET resetRateofDefrostTimeFractionIncrease);

    Q_PROPERTY(bool economizerLockout READ economizerLockout WRITE setEconomizerLockout);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject);

   public:

    /** @name Constructors and Destructors */
    //@{

    HeatExchangerAirToAirSensibleAndLatent_Impl(const IdfObject& idfObject,
                                                Model_Impl* model,
                                                bool keepHandle);

    HeatExchangerAirToAirSensibleAndLatent_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    HeatExchangerAirToAirSensibleAndLatent_Impl(const HeatExchangerAirToAirSensibleAndLatent_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    virtual ~HeatExchangerAirToAirSensibleAndLatent_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    Schedule availabilitySchedule() const;

    boost::optional<double> nominalSupplyAirFlowRate() const;

    OSOptionalQuantity getNominalSupplyAirFlowRate(bool returnIP=false) const;

    bool isNominalSupplyAirFlowRateAutosized() const;

    double sensibleEffectivenessat100HeatingAirFlow() const;

    Quantity getSensibleEffectivenessat100HeatingAirFlow(bool returnIP=false) const;

    double latentEffectivenessat100HeatingAirFlow() const;

    Quantity getLatentEffectivenessat100HeatingAirFlow(bool returnIP=false) const;

    double sensibleEffectivenessat75HeatingAirFlow() const;

    Quantity getSensibleEffectivenessat75HeatingAirFlow(bool returnIP=false) const;

    double latentEffectivenessat75HeatingAirFlow() const;

    Quantity getLatentEffectivenessat75HeatingAirFlow(bool returnIP=false) const;

    double sensibleEffectivenessat100CoolingAirFlow() const;

    Quantity getSensibleEffectivenessat100CoolingAirFlow(bool returnIP=false) const;

    double latentEffectivenessat100CoolingAirFlow() const;

    Quantity getLatentEffectivenessat100CoolingAirFlow(bool returnIP=false) const;

    double sensibleEffectivenessat75CoolingAirFlow() const;

    Quantity getSensibleEffectivenessat75CoolingAirFlow(bool returnIP=false) const;

    double latentEffectivenessat75CoolingAirFlow() const;

    Quantity getLatentEffectivenessat75CoolingAirFlow(bool returnIP=false) const;

    double nominalElectricPower() const;

    Quantity getNominalElectricPower(bool returnIP=false) const;

    bool supplyAirOutletTemperatureControl() const;

    std::string heatExchangerType() const;

    std::string frostControlType() const;

    double thresholdTemperature() const;

    Quantity getThresholdTemperature(bool returnIP=false) const;

    bool isThresholdTemperatureDefaulted() const;

    boost::optional<double> initialDefrostTimeFraction() const;

    OSOptionalQuantity getInitialDefrostTimeFraction(bool returnIP=false) const;

    boost::optional<double> rateofDefrostTimeFractionIncrease() const;

    OSOptionalQuantity getRateofDefrostTimeFractionIncrease(bool returnIP=false) const;

    bool economizerLockout() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setAvailabilitySchedule(Schedule& schedule);

    bool setNominalSupplyAirFlowRate(boost::optional<double> nominalSupplyAirFlowRate);

    bool setNominalSupplyAirFlowRate(const OSOptionalQuantity& nominalSupplyAirFlowRate);

    void autosizeNominalSupplyAirFlowRate();

    bool setSensibleEffectivenessat100HeatingAirFlow(double sensibleEffectivenessat100HeatingAirFlow);

    bool setSensibleEffectivenessat100HeatingAirFlow(const Quantity& sensibleEffectivenessat100HeatingAirFlow);

    bool setLatentEffectivenessat100HeatingAirFlow(double latentEffectivenessat100HeatingAirFlow);

    bool setLatentEffectivenessat100HeatingAirFlow(const Quantity& latentEffectivenessat100HeatingAirFlow);

    bool setSensibleEffectivenessat75HeatingAirFlow(double sensibleEffectivenessat75HeatingAirFlow);

    bool setSensibleEffectivenessat75HeatingAirFlow(const Quantity& sensibleEffectivenessat75HeatingAirFlow);

    bool setLatentEffectivenessat75HeatingAirFlow(double latentEffectivenessat75HeatingAirFlow);

    bool setLatentEffectivenessat75HeatingAirFlow(const Quantity& latentEffectivenessat75HeatingAirFlow);

    bool setSensibleEffectivenessat100CoolingAirFlow(double sensibleEffectivenessat100CoolingAirFlow);

    bool setSensibleEffectivenessat100CoolingAirFlow(const Quantity& sensibleEffectivenessat100CoolingAirFlow);

    bool setLatentEffectivenessat100CoolingAirFlow(double latentEffectivenessat100CoolingAirFlow);

    bool setLatentEffectivenessat100CoolingAirFlow(const Quantity& latentEffectivenessat100CoolingAirFlow);

    bool setSensibleEffectivenessat75CoolingAirFlow(double sensibleEffectivenessat75CoolingAirFlow);

    bool setSensibleEffectivenessat75CoolingAirFlow(const Quantity& sensibleEffectivenessat75CoolingAirFlow);

    bool setLatentEffectivenessat75CoolingAirFlow(double latentEffectivenessat75CoolingAirFlow);

    bool setLatentEffectivenessat75CoolingAirFlow(const Quantity& latentEffectivenessat75CoolingAirFlow);

    bool setNominalElectricPower(double nominalElectricPower);

    bool setNominalElectricPower(const Quantity& nominalElectricPower);

    void setSupplyAirOutletTemperatureControl(bool supplyAirOutletTemperatureControl);

    bool setHeatExchangerType(std::string heatExchangerType);

    bool setFrostControlType(std::string frostControlType);

    void setThresholdTemperature(double thresholdTemperature);

    bool setThresholdTemperature(const Quantity& thresholdTemperature);

    void resetThresholdTemperature();

    bool setInitialDefrostTimeFraction(boost::optional<double> initialDefrostTimeFraction);

    bool setInitialDefrostTimeFraction(const OSOptionalQuantity& initialDefrostTimeFraction);

    void resetInitialDefrostTimeFraction();

    bool setRateofDefrostTimeFractionIncrease(boost::optional<double> rateofDefrostTimeFractionIncrease);

    bool setRateofDefrostTimeFractionIncrease(const OSOptionalQuantity& rateofDefrostTimeFractionIncrease);

    void resetRateofDefrostTimeFractionIncrease();

    void setEconomizerLockout(bool economizerLockout);

    //@}
    /** @name Other */
    //@{

    unsigned primaryAirInletPort();

    unsigned primaryAirOutletPort();

    unsigned secondaryAirInletPort();

    unsigned secondaryAirOutletPort();

    //@}

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.HeatExchangerAirToAirSensibleAndLatent");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Schedule> optionalAvailabilitySchedule() const;

    openstudio::OSOptionalQuantity nominalSupplyAirFlowRate_SI() const;
    openstudio::OSOptionalQuantity nominalSupplyAirFlowRate_IP() const;
    openstudio::Quantity sensibleEffectivenessat100HeatingAirFlow_SI() const;
    openstudio::Quantity sensibleEffectivenessat100HeatingAirFlow_IP() const;
    openstudio::Quantity latentEffectivenessat100HeatingAirFlow_SI() const;
    openstudio::Quantity latentEffectivenessat100HeatingAirFlow_IP() const;
    openstudio::Quantity sensibleEffectivenessat75HeatingAirFlow_SI() const;
    openstudio::Quantity sensibleEffectivenessat75HeatingAirFlow_IP() const;
    openstudio::Quantity latentEffectivenessat75HeatingAirFlow_SI() const;
    openstudio::Quantity latentEffectivenessat75HeatingAirFlow_IP() const;
    openstudio::Quantity sensibleEffectivenessat100CoolingAirFlow_SI() const;
    openstudio::Quantity sensibleEffectivenessat100CoolingAirFlow_IP() const;
    openstudio::Quantity latentEffectivenessat100CoolingAirFlow_SI() const;
    openstudio::Quantity latentEffectivenessat100CoolingAirFlow_IP() const;
    openstudio::Quantity sensibleEffectivenessat75CoolingAirFlow_SI() const;
    openstudio::Quantity sensibleEffectivenessat75CoolingAirFlow_IP() const;
    openstudio::Quantity latentEffectivenessat75CoolingAirFlow_SI() const;
    openstudio::Quantity latentEffectivenessat75CoolingAirFlow_IP() const;
    openstudio::Quantity nominalElectricPower_SI() const;
    openstudio::Quantity nominalElectricPower_IP() const;
    std::vector<std::string> heatExchangerTypeValues() const;
    std::vector<std::string> frostControlTypeValues() const;
    openstudio::Quantity thresholdTemperature_SI() const;
    openstudio::Quantity thresholdTemperature_IP() const;
    openstudio::OSOptionalQuantity initialDefrostTimeFraction_SI() const;
    openstudio::OSOptionalQuantity initialDefrostTimeFraction_IP() const;
    openstudio::OSOptionalQuantity rateofDefrostTimeFractionIncrease_SI() const;
    openstudio::OSOptionalQuantity rateofDefrostTimeFractionIncrease_IP() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_IMPL_HPP

