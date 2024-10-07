/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILSYSTEMCOOLINGWATERHEATEXCHANGERASSISTED_IMPL_HPP
#define MODEL_COILSYSTEMCOOLINGWATERHEATEXCHANGERASSISTED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class AirToAirComponent;
  class WaterToAirComponent;

  namespace detail {

    /** CoilSystemCoolingWaterHeatExchangerAssisted_Impl is a StraightComponent_Impl that is the implementation class for CoilSystemCoolingWaterHeatExchangerAssisted.*/
    class MODEL_API CoilSystemCoolingWaterHeatExchangerAssisted_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilSystemCoolingWaterHeatExchangerAssisted_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilSystemCoolingWaterHeatExchangerAssisted_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilSystemCoolingWaterHeatExchangerAssisted_Impl(const CoilSystemCoolingWaterHeatExchangerAssisted_Impl& other, Model_Impl* model,
                                                       bool keepHandle);

      virtual ~CoilSystemCoolingWaterHeatExchangerAssisted_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      // will return the coolingCoil and heatExchanger
      virtual std::vector<ModelObject> children() const override;

      // Will also clone the coolingCoil and heatExchanger
      virtual ModelObject clone(Model model) const override;

      // This function will connect the underlying Coil:Cooling:Water object
      virtual bool addToNode(Node& node) override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      AirToAirComponent heatExchanger() const;

      WaterToAirComponent coolingCoil() const;

      //@}
      /** @name Setters */
      //@{

      bool setHeatExchanger(const AirToAirComponent& heatExchanger);

      bool setCoolingCoil(const WaterToAirComponent& coolingCoil);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilSystemCoolingWaterHeatExchangerAssisted");

      boost::optional<AirToAirComponent> optionalHeatExchanger() const;
      boost::optional<WaterToAirComponent> optionalCoolingCoil() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILSYSTEMCOOLINGWATERHEATEXCHANGERASSISTED_IMPL_HPP
