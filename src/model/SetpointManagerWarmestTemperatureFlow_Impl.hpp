/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERWARMESTTEMPERATUREFLOW_IMPL_HPP
#define MODEL_SETPOINTMANAGERWARMESTTEMPERATUREFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Node;

  namespace detail {

    /** SetpointManagerWarmestTemperatureFlow_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerWarmestTemperatureFlow.*/
    class MODEL_API SetpointManagerWarmestTemperatureFlow_Impl : public SetpointManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerWarmestTemperatureFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerWarmestTemperatureFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerWarmestTemperatureFlow_Impl(const SetpointManagerWarmestTemperatureFlow_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SetpointManagerWarmestTemperatureFlow_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string controlVariable() const override;

      double minimumSetpointTemperature() const;

      double maximumSetpointTemperature() const;

      std::string strategy() const;

      double minimumTurndownRatio() const;

      //@}
      /** @name Setters */
      //@{

      bool setControlVariable(const std::string& controlVariable) override;

      bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

      bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

      bool setStrategy(const std::string& strategy);

      bool setMinimumTurndownRatio(double minimumTurndownRatio);

      //@}
      /** @name Other */
      //@{
      boost::optional<Node> setpointNode() const override;

      bool setSetpointNode(const Node& node) override;

      void resetSetpointNode() override;
      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SetpointManagerWarmestTemperatureFlow");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERWARMESTTEMPERATUREFLOW_IMPL_HPP
