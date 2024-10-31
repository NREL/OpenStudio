/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERCOLDEST_IMPL_HPP
#define MODEL_SETPOINTMANAGERCOLDEST_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** SetpointManagerColdest_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerColdest.*/
    class MODEL_API SetpointManagerColdest_Impl : public SetpointManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerColdest_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerColdest_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerColdest_Impl(const SetpointManagerColdest_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SetpointManagerColdest_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      virtual std::string controlVariable() const override;

      double minimumSetpointTemperature() const;

      double maximumSetpointTemperature() const;

      std::string strategy() const;

      virtual boost::optional<Node> setpointNode() const override;

      //@}
      /** @name Setters */
      //@{

      virtual bool setControlVariable(const std::string& controlVariable) override;

      bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

      bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

      bool setStrategy(const std::string& strategy);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      virtual bool setSetpointNode(const Node& node) override;

      virtual void resetSetpointNode() override;

      REGISTER_LOGGER("openstudio.model.SetpointManagerColdest");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERCOLDEST_IMPL_HPP
