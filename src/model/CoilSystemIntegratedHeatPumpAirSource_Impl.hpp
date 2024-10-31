/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILSYSTEMINTEGRATEDHEATPUMPAIRSOURCE_IMPL_HPP
#define MODEL_COILSYSTEMINTEGRATEDHEATPUMPAIRSOURCE_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class StraightComponent;
  class HVACComponent;
  class Curve;

  namespace detail {

    /** CoilSystemIntegratedHeatPumpAirSource_Impl is a StraightComponent_Impl that is the implementation class for CoilSystemIntegratedHeatPumpAirSource.*/
    class MODEL_API CoilSystemIntegratedHeatPumpAirSource_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilSystemIntegratedHeatPumpAirSource_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilSystemIntegratedHeatPumpAirSource_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilSystemIntegratedHeatPumpAirSource_Impl(const CoilSystemIntegratedHeatPumpAirSource_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilSystemIntegratedHeatPumpAirSource_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      // Override to disallow adding to node. It can only be placed inside another component
      virtual bool addToNode(Node& node) override;

      virtual std::vector<ModelObject> children() const override;

      // Will clone all children coils
      virtual ModelObject clone(Model model) const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      StraightComponent spaceCoolingCoil() const;

      StraightComponent spaceHeatingCoil() const;

      HVACComponent dedicatedWaterHeatingCoil() const;

      HVACComponent scwhCoil() const;

      StraightComponent scdwhCoolingCoil() const;

      HVACComponent scdwhWaterHeatingCoil() const;

      StraightComponent shdwhHeatingCoil() const;

      HVACComponent shdwhWaterHeatingCoil() const;

      double indoorTemperatureLimitForSCWHMode() const;

      double ambientTemperatureLimitForSCWHMode() const;

      double indoorTemperatureAboveWhichWHHasHigherPriority() const;

      double ambientTemperatureAboveWhichWHHasHigherPriority() const;

      int flagtoIndicateLoadControlInSCWHMode() const;

      int minimumSpeedLevelForSCWHMode() const;

      double maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode() const;

      int minimumSpeedLevelForSCDWHMode() const;

      double maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode() const;

      int minimumSpeedLevelForSHDWHMode() const;

      //@}
      /** @name Setters */
      //@{

      bool setSpaceCoolingCoil(const StraightComponent& spaceCoolingCoil);

      bool setSpaceHeatingCoil(const StraightComponent& spaceHeatingCoil);

      bool setDedicatedWaterHeatingCoil(const HVACComponent& dedicatedWaterHeatingCoil);

      bool setSCWHCoil(const HVACComponent& scwhCoil);

      bool setSCDWHCoolingCoil(const StraightComponent& scdwhCoolingCoil);

      bool setSCDWHWaterHeatingCoil(const HVACComponent& scdwhWaterHeatingCoil);

      bool setSHDWHHeatingCoil(const StraightComponent& shdwhHeatingCoil);

      bool setSHDWHWaterHeatingCoil(const HVACComponent& shdwhWaterHeatingCoil);

      bool setIndoorTemperatureLimitForSCWHMode(double indoorTemperatureLimitForSCWHMode);

      bool setAmbientTemperatureLimitForSCWHMode(double ambientTemperatureLimitForSCWHMode);

      bool setIndoorTemperatureAboveWhichWHHasHigherPriority(double indoorTemperatureAboveWhichWHHasHigherPriority);

      bool setAmbientTemperatureAboveWhichWHHasHigherPriority(double ambientTemperatureAboveWhichWHHasHigherPriority);

      bool setFlagtoIndicateLoadControlInSCWHMode(int flagtoIndicateLoadControlInSCWHMode);

      bool setMinimumSpeedLevelForSCWHMode(int minimumSpeedLevelForSCWHMode);

      bool setMaximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode(double maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode);

      bool setMinimumSpeedLevelForSCDWHMode(int minimumSpeedLevelForSCDWHMode);

      bool setMaximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode(
        double maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode);

      bool setMinimumSpeedLevelForSHDWHMode(int minimumSpeedLevelForSHDWHMode);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilSystemIntegratedHeatPumpAirSource");

      boost::optional<StraightComponent> optionalSpaceCoolingCoil() const;
      boost::optional<StraightComponent> optionalSpaceHeatingCoil() const;
      boost::optional<HVACComponent> optionalDedicatedWaterHeatingCoil() const;
      boost::optional<HVACComponent> optionalScwhCoil() const;
      boost::optional<StraightComponent> optionalScdwhCoolingCoil() const;
      boost::optional<HVACComponent> optionalScdwhWaterHeatingCoil() const;
      boost::optional<StraightComponent> optionalShdwhHeatingCoil() const;
      boost::optional<HVACComponent> optionalShdwhWaterHeatingCoil() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILSYSTEMINTEGRATEDHEATPUMPAIRSOURCE_IMPL_HPP
