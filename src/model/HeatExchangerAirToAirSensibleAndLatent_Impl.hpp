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

#ifndef MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_IMPL_HPP
#define MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_IMPL_HPP

#include "ModelAPI.hpp"
#include "AirToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Connection;

  namespace detail {

    /** HeatExchangerAirToAirSensibleAndLatent_Impl is a AirToAirComponent_Impl that is the implementation class for HeatExchangerAirToAirSensibleAndLatent.*/
    class MODEL_API HeatExchangerAirToAirSensibleAndLatent_Impl : public AirToAirComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      HeatExchangerAirToAirSensibleAndLatent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HeatExchangerAirToAirSensibleAndLatent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HeatExchangerAirToAirSensibleAndLatent_Impl(const HeatExchangerAirToAirSensibleAndLatent_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~HeatExchangerAirToAirSensibleAndLatent_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual std::vector<ModelObject> children() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      // Returns an eventual CoilSystemCooling Water/DX HeatExchangerAssisted
      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      /* Override disallows addToNode when part of a CoilSystemCooling Water/DX HeatExchangerAssisted
     * otherwise calls parent AirToAirComponent_Impl::addToNode
     */
      virtual bool addToNode(Node& node) override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual unsigned primaryAirInletPort() const override;

      virtual unsigned primaryAirOutletPort() const override;

      virtual unsigned secondaryAirInletPort() const override;

      virtual unsigned secondaryAirOutletPort() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      boost::optional<double> nominalSupplyAirFlowRate() const;

      bool isNominalSupplyAirFlowRateAutosized() const;

      double sensibleEffectivenessat100HeatingAirFlow() const;

      double latentEffectivenessat100HeatingAirFlow() const;

      double sensibleEffectivenessat75HeatingAirFlow() const;

      double latentEffectivenessat75HeatingAirFlow() const;

      double sensibleEffectivenessat100CoolingAirFlow() const;

      double latentEffectivenessat100CoolingAirFlow() const;

      double sensibleEffectivenessat75CoolingAirFlow() const;

      double latentEffectivenessat75CoolingAirFlow() const;

      double nominalElectricPower() const;

      bool supplyAirOutletTemperatureControl() const;

      std::string heatExchangerType() const;

      std::string frostControlType() const;

      double thresholdTemperature() const;

      bool isThresholdTemperatureDefaulted() const;

      boost::optional<double> initialDefrostTimeFraction() const;

      boost::optional<double> rateofDefrostTimeFractionIncrease() const;

      bool economizerLockout() const;

      boost::optional<double> autosizedNominalSupplyAirFlowRate() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setNominalSupplyAirFlowRate(boost::optional<double> nominalSupplyAirFlowRate);

      void autosizeNominalSupplyAirFlowRate();

      bool setSensibleEffectivenessat100HeatingAirFlow(double sensibleEffectivenessat100HeatingAirFlow);

      bool setLatentEffectivenessat100HeatingAirFlow(double latentEffectivenessat100HeatingAirFlow);

      bool setSensibleEffectivenessat75HeatingAirFlow(double sensibleEffectivenessat75HeatingAirFlow);

      bool setLatentEffectivenessat75HeatingAirFlow(double latentEffectivenessat75HeatingAirFlow);

      bool setSensibleEffectivenessat100CoolingAirFlow(double sensibleEffectivenessat100CoolingAirFlow);

      bool setLatentEffectivenessat100CoolingAirFlow(double latentEffectivenessat100CoolingAirFlow);

      bool setSensibleEffectivenessat75CoolingAirFlow(double sensibleEffectivenessat75CoolingAirFlow);

      bool setLatentEffectivenessat75CoolingAirFlow(double latentEffectivenessat75CoolingAirFlow);

      bool setNominalElectricPower(double nominalElectricPower);

      bool setSupplyAirOutletTemperatureControl(bool supplyAirOutletTemperatureControl);

      bool setHeatExchangerType(const std::string& heatExchangerType);

      bool setFrostControlType(const std::string& frostControlType);

      bool setThresholdTemperature(double thresholdTemperature);

      void resetThresholdTemperature();

      bool setInitialDefrostTimeFraction(boost::optional<double> initialDefrostTimeFraction);

      void resetInitialDefrostTimeFraction();

      bool setRateofDefrostTimeFractionIncrease(boost::optional<double> rateofDefrostTimeFractionIncrease);

      void resetRateofDefrostTimeFractionIncrease();

      bool setEconomizerLockout(bool economizerLockout);

      //@}
      /** @name Other */
      //@{

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);
      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

      //@}

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HeatExchangerAirToAirSensibleAndLatent");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;

      std::vector<std::string> heatExchangerTypeValues() const;
      std::vector<std::string> frostControlTypeValues() const;
      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_IMPL_HPP
