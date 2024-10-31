/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKEQUIVALENTDUCT_IMPL_HPP
#define MODEL_AIRFLOWNETWORKEQUIVALENTDUCT_IMPL_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class StraightComponent;

  namespace detail {

    /** AirflowNetworkEquivalentDuct_Impl is a AirflowNetworkComponent_Impl that is the implementation class for AirflowNetworkEquivalentDuct.*/
    class MODEL_API AirflowNetworkEquivalentDuct_Impl : public AirflowNetworkComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkEquivalentDuct_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkEquivalentDuct_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkEquivalentDuct_Impl(const AirflowNetworkEquivalentDuct_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkEquivalentDuct_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual boost::optional<ModelObject> componentModelObject() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<StraightComponent> straightComponent() const;

      /*
    template <typename T> boost::optional<T> component() const
    {
      return getObject<ModelObject>().getModelObjectTarget<T>(OS_AirflowNetworkEquivalentDuctFields::ComponentName);
    }
    */

      boost::optional<std::string> coilObjectType() const;
      boost::optional<std::string> heatExchangerObjectType() const;
      boost::optional<std::string> terminalUnitObjectType() const;

      double airPathLength() const;

      double airPathHydraulicDiameter() const;

      //@}
      /** @name Setters */
      //@{

      void resetComponent();

      bool setAirPathLength(double airPathLength);

      bool setAirPathHydraulicDiameter(double airPathHydraulicDiameter);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkEquivalentDuct");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      //boost::optional<AFNTerminalUnit> optionalTerminalUnit() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKEQUIVALENTDUCT_IMPL_HPP
