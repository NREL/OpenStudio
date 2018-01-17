/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

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

  boost::optional<double> autosizedNominalSupplyAirFlowRate() const ;

  virtual void autosize() override;

  virtual void applySizingValues() override;

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

    bool setSupplyAirOutletTemperatureControl(bool supplyAirOutletTemperatureControl);

    bool setHeatExchangerType(std::string heatExchangerType);

    bool setFrostControlType(std::string frostControlType);

    bool setThresholdTemperature(double thresholdTemperature);

    bool setThresholdTemperature(const Quantity& thresholdTemperature);

    void resetThresholdTemperature();

    bool setInitialDefrostTimeFraction(boost::optional<double> initialDefrostTimeFraction);

    bool setInitialDefrostTimeFraction(const OSOptionalQuantity& initialDefrostTimeFraction);

    void resetInitialDefrostTimeFraction();

    bool setRateofDefrostTimeFractionIncrease(boost::optional<double> rateofDefrostTimeFractionIncrease);

    bool setRateofDefrostTimeFractionIncrease(const OSOptionalQuantity& rateofDefrostTimeFractionIncrease);

    void resetRateofDefrostTimeFractionIncrease();

    bool setEconomizerLockout(bool economizerLockout);

    //@}
    /** @name Other */
    //@{

    unsigned primaryAirInletPort() override;

    unsigned primaryAirOutletPort() override;

    unsigned secondaryAirInletPort() override;

    unsigned secondaryAirOutletPort() override;

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