/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_THERMALSTORAGECHILLEDWATERSTRATIFIED_IMPL_HPP
#define MODEL_THERMALSTORAGECHILLEDWATERSTRATIFIED_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    /** ThermalStorageChilledWaterStratified_Impl is a WaterToWaterComponent_Impl that is the implementation class for ThermalStorageChilledWaterStratified.*/
    class MODEL_API ThermalStorageChilledWaterStratified_Impl : public WaterToWaterComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ThermalStorageChilledWaterStratified_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ThermalStorageChilledWaterStratified_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ThermalStorageChilledWaterStratified_Impl(const ThermalStorageChilledWaterStratified_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ThermalStorageChilledWaterStratified_Impl() override = default;

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

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      double tankVolume() const;

      double tankHeight() const;

      std::string tankShape() const;

      boost::optional<double> tankPerimeter() const;

      boost::optional<Schedule> setpointTemperatureSchedule() const;

      double deadbandTemperatureDifference() const;

      boost::optional<double> temperatureSensorHeight() const;

      boost::optional<double> minimumTemperatureLimit() const;

      boost::optional<double> nominalCoolingCapacity() const;

      std::string ambientTemperatureIndicator() const;

      boost::optional<Schedule> ambientTemperatureSchedule() const;

      boost::optional<ThermalZone> ambientTemperatureThermalZone() const;

      boost::optional<std::string> ambientTemperatureOutdoorAirNodeName() const;

      boost::optional<double> uniformSkinLossCoefficientperUnitAreatoAmbientTemperature() const;

      double useSideHeatTransferEffectiveness() const;

      boost::optional<Schedule> useSideAvailabilitySchedule() const;

      boost::optional<double> useSideInletHeight() const;

      bool isUseSideInletHeightAutocalculated() const;

      double useSideOutletHeight() const;

      boost::optional<double> useSideDesignFlowRate() const;

      bool isUseSideDesignFlowRateAutosized() const;

      double sourceSideHeatTransferEffectiveness() const;

      boost::optional<Schedule> sourceSideAvailabilitySchedule() const;

      double sourceSideInletHeight() const;

      boost::optional<double> sourceSideOutletHeight() const;

      bool isSourceSideOutletHeightAutocalculated() const;

      boost::optional<double> sourceSideDesignFlowRate() const;

      bool isSourceSideDesignFlowRateAutosized() const;

      double tankRecoveryTime() const;

      std::string inletMode() const;

      int numberofNodes() const;

      double additionalDestratificationConductivity() const;

      double node1AdditionalLossCoefficient() const;

      double node2AdditionalLossCoefficient() const;

      double node3AdditionalLossCoefficient() const;

      double node4AdditionalLossCoefficient() const;

      double node5AdditionalLossCoefficient() const;

      double node6AdditionalLossCoefficient() const;

      double node7AdditionalLossCoefficient() const;

      double node8AdditionalLossCoefficient() const;

      double node9AdditionalLossCoefficient() const;

      double node10AdditionalLossCoefficient() const;

      boost::optional<double> autosizedUseSideDesignFlowRate() const;

      boost::optional<double> autosizedSourceSideDesignFlowRate() const;

      //@}
      /** @name Setters */
      //@{

      bool setTankVolume(double tankVolume);

      bool setTankHeight(double tankHeight);

      bool setTankShape(const std::string& tankShape);

      bool setTankPerimeter(boost::optional<double> tankPerimeter);

      void resetTankPerimeter();

      bool setSetpointTemperatureSchedule(Schedule& schedule);

      void resetSetpointTemperatureSchedule();

      bool setDeadbandTemperatureDifference(double deadbandTemperatureDifference);

      bool setTemperatureSensorHeight(boost::optional<double> temperatureSensorHeight);

      void resetTemperatureSensorHeight();

      bool setMinimumTemperatureLimit(boost::optional<double> minimumTemperatureLimit);

      void resetMinimumTemperatureLimit();

      bool setNominalCoolingCapacity(boost::optional<double> nominalCoolingCapacity);

      void resetNominalCoolingCapacity();

      bool setAmbientTemperatureIndicator(const std::string& ambientTemperatureIndicator);

      bool setAmbientTemperatureSchedule(Schedule& schedule);

      void resetAmbientTemperatureSchedule();

      bool setAmbientTemperatureThermalZone(const boost::optional<ThermalZone>& thermalZone);

      void resetAmbientTemperatureThermalZone();

      bool setAmbientTemperatureOutdoorAirNodeName(boost::optional<std::string> ambientTemperatureOutdoorAirNodeName);

      void resetAmbientTemperatureOutdoorAirNodeName();

      bool setUniformSkinLossCoefficientperUnitAreatoAmbientTemperature(
        boost::optional<double> uniformSkinLossCoefficientperUnitAreatoAmbientTemperature);

      void resetUniformSkinLossCoefficientperUnitAreatoAmbientTemperature();

      bool setUseSideHeatTransferEffectiveness(double useSideHeatTransferEffectiveness);

      bool setUseSideAvailabilitySchedule(Schedule& schedule);

      void resetUseSideAvailabilitySchedule();

      bool setUseSideInletHeight(boost::optional<double> useSideInletHeight);

      void autocalculateUseSideInletHeight();

      bool setUseSideOutletHeight(double useSideOutletHeight);

      bool setUseSideDesignFlowRate(boost::optional<double> useSideDesignFlowRate);

      void autosizeUseSideDesignFlowRate();

      bool setSourceSideHeatTransferEffectiveness(double sourceSideHeatTransferEffectiveness);

      bool setSourceSideAvailabilitySchedule(Schedule& schedule);

      void resetSourceSideAvailabilitySchedule();

      bool setSourceSideInletHeight(double sourceSideInletHeight);

      bool setSourceSideOutletHeight(boost::optional<double> sourceSideOutletHeight);

      void autocalculateSourceSideOutletHeight();

      bool setSourceSideDesignFlowRate(boost::optional<double> sourceSideDesignFlowRate);

      void autosizeSourceSideDesignFlowRate();

      bool setTankRecoveryTime(double tankRecoveryTime);

      bool setInletMode(const std::string& inletMode);

      bool setNumberofNodes(int numberofNodes);

      bool setAdditionalDestratificationConductivity(double additionalDestratificationConductivity);

      bool setNode1AdditionalLossCoefficient(double node1AdditionalLossCoefficient);

      bool setNode2AdditionalLossCoefficient(double node2AdditionalLossCoefficient);

      bool setNode3AdditionalLossCoefficient(double node3AdditionalLossCoefficient);

      bool setNode4AdditionalLossCoefficient(double node4AdditionalLossCoefficient);

      bool setNode5AdditionalLossCoefficient(double node5AdditionalLossCoefficient);

      bool setNode6AdditionalLossCoefficient(double node6AdditionalLossCoefficient);

      bool setNode7AdditionalLossCoefficient(double node7AdditionalLossCoefficient);

      bool setNode8AdditionalLossCoefficient(double node8AdditionalLossCoefficient);

      bool setNode9AdditionalLossCoefficient(double node9AdditionalLossCoefficient);

      bool setNode10AdditionalLossCoefficient(double node10AdditionalLossCoefficient);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ThermalStorageChilledWaterStratified");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_THERMALSTORAGECHILLEDWATERSTRATIFIED_IMPL_HPP
