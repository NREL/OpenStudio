/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERMULTIZONEMAXIMUMHUMIDITYAVERAGE_IMPL_HPP
#define MODEL_SETPOINTMANAGERMULTIZONEMAXIMUMHUMIDITYAVERAGE_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** SetpointManagerMultiZoneMaximumHumidityAverage_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerMultiZoneMaximumHumidityAverage.*/
    class MODEL_API SetpointManagerMultiZoneMaximumHumidityAverage_Impl : public SetpointManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerMultiZoneMaximumHumidityAverage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerMultiZoneMaximumHumidityAverage_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerMultiZoneMaximumHumidityAverage_Impl(const SetpointManagerMultiZoneMaximumHumidityAverage_Impl& other, Model_Impl* model,
                                                          bool keepHandle);

      virtual ~SetpointManagerMultiZoneMaximumHumidityAverage_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      virtual std::string controlVariable() const override;

      double minimumSetpointHumidityRatio() const;

      double maximumSetpointHumidityRatio() const;

      virtual boost::optional<Node> setpointNode() const override;

      //@}
      /** @name Setters */
      //@{

      virtual bool setControlVariable(const std::string& controlVariable) override;

      bool setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio);

      bool setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      virtual bool setSetpointNode(const Node& node) override;

      virtual void resetSetpointNode() override;

      REGISTER_LOGGER("openstudio.model.SetpointManagerMultiZoneMaximumHumidityAverage");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERMULTIZONEMAXIMUMHUMIDITYAVERAGE_IMPL_HPP
