/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKOUTDOORAIRFLOW_IMPL_HPP
#define MODEL_AIRFLOWNETWORKOUTDOORAIRFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class OutdoorAirController;
  class Crack;

  namespace detail {

    /** AirflowNetworkOutdoorAirflow_Impl is a ModelObject_Impl that is the implementation class for AirflowNetworkOutdoorAirflow.*/
    class MODEL_API AirflowNetworkOutdoorAirflow_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkOutdoorAirflow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkOutdoorAirflow_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkOutdoorAirflow_Impl(const AirflowNetworkOutdoorAirflow_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkOutdoorAirflow_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<ControllerOutdoorAir> controllerOutdoorAir() const;
      boost::optional<AirflowNetworkCrack> crack() const;

      //@}
      /** @name Setters */
      //@{

      bool setCrack(const AirflowNetworkCrack& crack);
      void resetCrack();

      //@}
      /** @name Other */
      //@{

      void resetControllerOutdoorAir();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkOutdoorAirflow");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKOUTDOORAIRFLOW_IMPL_HPP
