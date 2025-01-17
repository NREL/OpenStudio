/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

      virtual ~EvaporativeCoolerDirectResearchSpecial_Impl() override = default;

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

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EVAPORATIVECOOLERDIRECTRESEARCHSPECIAL_IMPL_HPP
