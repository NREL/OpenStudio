/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILSYSTEMCOOLINGDXHEATEXCHANGERASSISTED_IMPL_HPP
#define MODEL_COILSYSTEMCOOLINGDXHEATEXCHANGERASSISTED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class AirToAirComponent;

  namespace detail {

    /** CoilSystemCoolingDXHeatExchangerAssisted_Impl is a StraightComponent_Impl that is the implementation class for CoilSystemCoolingDXHeatExchangerAssisted.*/
    class MODEL_API CoilSystemCoolingDXHeatExchangerAssisted_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilSystemCoolingDXHeatExchangerAssisted_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilSystemCoolingDXHeatExchangerAssisted_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilSystemCoolingDXHeatExchangerAssisted_Impl(const CoilSystemCoolingDXHeatExchangerAssisted_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilSystemCoolingDXHeatExchangerAssisted_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;
      virtual unsigned outletPort() const override;

      /**
     * Note JM 2019-03-13: At this point in time
     * CoilSystemCoolingDXHeatExchangerAssisted is **NOT** allowed on a Branch directly and should be placed inside one of the Unitary systems
     * cf https://github.com/NREL/EnergyPlus/issues/7222
     * This method returns false and does nothing as a result
     */
      virtual bool addToNode(Node& node) override;

      // will return the coolingCoil and heatExchanger
      virtual std::vector<ModelObject> children() const override;

      // Will also clone the coolingCoil and heatExchanger
      virtual ModelObject clone(Model model) const override;

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

      StraightComponent coolingCoil() const;

      //@}
      /** @name Setters */
      //@{

      bool setHeatExchanger(const AirToAirComponent& heatExchanger);

      bool setCoolingCoil(const StraightComponent& coolingCoil);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilSystemCoolingDXHeatExchangerAssisted");

      boost::optional<AirToAirComponent> optionalHeatExchanger() const;
      boost::optional<StraightComponent> optionalCoolingCoil() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILSYSTEMCOOLINGDXHEATEXCHANGERASSISTED_IMPL_HPP
