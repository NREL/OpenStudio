/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTINLETSIDEMIXER_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTINLETSIDEMIXER_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** AirTerminalSingleDuctInletSideMixer_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctInletSideMixer.*/
    class MODEL_API AirTerminalSingleDuctInletSideMixer_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirTerminalSingleDuctInletSideMixer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctInletSideMixer_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctInletSideMixer_Impl(const AirTerminalSingleDuctInletSideMixer_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirTerminalSingleDuctInletSideMixer_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;
      virtual unsigned outletPort() const override;
      virtual bool addToNode(Node& node) override;
      virtual std::vector<openstudio::IdfObject> remove() override;
      virtual bool isRemovable() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      bool controlForOutdoorAir() const;

      std::string perPersonVentilationRateMode() const;

      //@}
      /** @name Setters */
      //@{
      //

      bool setControlForOutdoorAir(bool controlForOutdoorAir);

      bool setPerPersonVentilationRateMode(const std::string& perPersonVentilationRateMode);

      //@}
      /** @name Other */
      //@{

      unsigned secondaryAirInletPort() const;
      boost::optional<Node> secondaryAirInletNode() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctInletSideMixer");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTINLETSIDEMIXER_IMPL_HPP
