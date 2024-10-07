/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERMULTIZONEHEATINGAVERAGE_IMPL_HPP
#define MODEL_SETPOINTMANAGERMULTIZONEHEATINGAVERAGE_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** SetpointManagerMultiZoneHeatingAverage_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerMultiZoneHeatingAverage.*/
    class MODEL_API SetpointManagerMultiZoneHeatingAverage_Impl : public SetpointManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerMultiZoneHeatingAverage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerMultiZoneHeatingAverage_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerMultiZoneHeatingAverage_Impl(const SetpointManagerMultiZoneHeatingAverage_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SetpointManagerMultiZoneHeatingAverage_Impl() override = default;

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

      virtual boost::optional<Node> setpointNode() const override;

      //@}
      /** @name Setters */
      //@{

      virtual bool setControlVariable(const std::string& controlVariable) override;

      bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

      bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      virtual bool setSetpointNode(const Node& node) override;

      virtual void resetSetpointNode() override;

      REGISTER_LOGGER("openstudio.model.SetpointManagerMultiZoneHeatingAverage");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERMULTIZONEHEATINGAVERAGE_IMPL_HPP
