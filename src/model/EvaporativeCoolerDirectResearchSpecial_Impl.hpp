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

#ifndef MODEL_EVAPORATIVECOOLERDIRECTRESEARCHSPECIAL_IMPL_HPP
#define MODEL_EVAPORATIVECOOLERDIRECTRESEARCHSPECIAL_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;

  namespace detail {

    class MODEL_API EvaporativeCoolerDirectResearchSpecial_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      EvaporativeCoolerDirectResearchSpecial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      EvaporativeCoolerDirectResearchSpecial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      EvaporativeCoolerDirectResearchSpecial_Impl(const EvaporativeCoolerDirectResearchSpecial_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~EvaporativeCoolerDirectResearchSpecial_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters and Setters */
      //@{

      Schedule availabilitySchedule() const;

      bool setAvailabilitySchedule(Schedule& schedule);

      double coolerDesignEffectiveness() const;

      bool setCoolerDesignEffectiveness(double value);

      boost::optional<double> recirculatingWaterPumpPowerConsumption() const;

      bool setRecirculatingWaterPumpPowerConsumption(double value);

      void autosizeRecirculatingWaterPumpPowerConsumption();

      bool isRecirculatingWaterPumpPowerConsumptionAutosized() const;

      boost::optional<Node> sensorNode() const;

      bool setSensorNode(const Node& node);

      double driftLossFraction() const;

      bool setDriftLossFraction(double value);

      double blowdownConcentrationRatio() const;

      bool setBlowdownConcentrationRatio(double value);

      boost::optional<Curve> effectivenessFlowRatioModifierCurve() const;

      bool setEffectivenessFlowRatioModifierCurve(const Curve& curve);

      void resetEffectivenessFlowRatioModifierCurve();

      double waterPumpPowerSizingFactor() const;

      bool setWaterPumpPowerSizingFactor(double waterPumpPowerSizingFactor);

      boost::optional<Curve> waterPumpPowerModifierCurve() const;

      bool setWaterPumpPowerModifierCurve(const Curve& curve);

      void resetWaterPumpPowerModifierCurve();

      boost::optional<double> primaryAirDesignFlowRate() const;

      bool setPrimaryAirDesignFlowRate(double value);

      void autosizePrimaryAirDesignFlowRate();

      bool isPrimaryAirDesignFlowRateAutosized() const;

      double evaporativeOperationMinimumDrybulbTemperature() const;
      bool setEvaporativeOperationMinimumDrybulbTemperature(double evaporativeOperationMinimumDrybulbTemperature);

      double evaporativeOperationMaximumLimitWetbulbTemperature() const;
      bool setEvaporativeOperationMaximumLimitWetbulbTemperature(double evaporativeOperationMaximumLimitWetbulbTemperature);

      double evaporativeOperationMaximumLimitDrybulbTemperature() const;
      bool setEvaporativeOperationMaximumLimitDrybulbTemperature(double evaporativeOperationMaximumLimitDrybulbTemperature);

      boost::optional<double> autosizedRecirculatingWaterPumpPowerConsumption() const;

      boost::optional<double> autosizedPrimaryAirDesignFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

     private:
      REGISTER_LOGGER("openstudio.model.EvaporativeCoolerDirectResearchSpecial");

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EVAPORATIVECOOLERDIRECTRESEARCHSPECIAL_IMPL_HPP
