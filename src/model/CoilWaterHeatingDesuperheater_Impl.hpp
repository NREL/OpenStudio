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

#ifndef MODEL_COILWATERHEATINGDESUPERHEATER_IMPL_HPP
#define MODEL_COILWATERHEATINGDESUPERHEATER_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class CurveBiquadratic;
  class HVACComponent;

  namespace detail {

    /** CoilWaterHeatingDesuperheater_Impl is a StraightComponent_Impl that is the implementation class for CoilWaterHeatingDesuperheater.*/
    class MODEL_API CoilWaterHeatingDesuperheater_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilWaterHeatingDesuperheater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilWaterHeatingDesuperheater_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilWaterHeatingDesuperheater_Impl(const CoilWaterHeatingDesuperheater_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilWaterHeatingDesuperheater_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      Schedule setpointTemperatureSchedule() const;

      double deadBandTemperatureDifference() const;

      bool isDeadBandTemperatureDifferenceDefaulted() const;

      boost::optional<double> ratedHeatReclaimRecoveryEfficiency() const;

      double ratedInletWaterTemperature() const;

      double ratedOutdoorAirTemperature() const;

      double maximumInletWaterTemperatureforHeatReclaim() const;

      boost::optional<CurveBiquadratic> heatReclaimEfficiencyFunctionofTemperatureCurve() const;

      // boost::optional<HVACComponent> tank() const;

      boost::optional<ModelObject> heatingSource() const;

      double waterFlowRate() const;

      double waterPumpPower() const;

      bool isWaterPumpPowerDefaulted() const;

      double fractionofPumpHeattoWater() const;

      bool isFractionofPumpHeattoWaterDefaulted() const;

      double onCycleParasiticElectricLoad() const;

      bool isOnCycleParasiticElectricLoadDefaulted() const;

      double offCycleParasiticElectricLoad() const;

      bool isOffCycleParasiticElectricLoadDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setSetpointTemperatureSchedule(Schedule& schedule);

      bool setDeadBandTemperatureDifference(double deadBandTemperatureDifference);

      void resetDeadBandTemperatureDifference();

      bool setRatedHeatReclaimRecoveryEfficiency(double ratedHeatReclaimRecoveryEfficiency);

      void resetRatedHeatReclaimRecoveryEfficiency();

      bool setRatedInletWaterTemperature(double ratedInletWaterTemperature);

      bool setRatedOutdoorAirTemperature(double ratedOutdoorAirTemperature);

      bool setMaximumInletWaterTemperatureforHeatReclaim(double maximumInletWaterTemperatureforHeatReclaim);

      bool setHeatReclaimEfficiencyFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic);

      void resetHeatReclaimEfficiencyFunctionofTemperatureCurve();

      // bool setTank(const HVACComponent& waterHeater);

      bool setHeatingSource(const ModelObject& heatingSource);

      void resetHeatingSource();

      bool setWaterFlowRate(double waterFlowRate);

      bool setWaterPumpPower(double waterPumpPower);

      void resetWaterPumpPower();

      bool setFractionofPumpHeattoWater(double fractionofPumpHeattoWater);

      void resetFractionofPumpHeattoWater();

      bool setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad);

      void resetOnCycleParasiticElectricLoad();

      bool setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad);

      void resetOffCycleParasiticElectricLoad();

      //@}
      /** @name Other */
      //@{

      boost::optional<ModelObject> heatRejectionTarget() const;

      bool addToHeatRejectionTarget(const ModelObject& heatRejectionTarget);

      void removeFromHeatRejectionTarget();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilWaterHeatingDesuperheater");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalSetpointTemperatureSchedule() const;
      // boost::optional<HVACComponent> optionalTank() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILWATERHEATINGDESUPERHEATER_IMPL_HPP
