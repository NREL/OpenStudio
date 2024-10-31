/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERMULTIZONEHUMIDITYMINIMUM_IMPL_HPP
#define MODEL_SETPOINTMANAGERMULTIZONEHUMIDITYMINIMUM_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** SetpointManagerMultiZoneHumidityMinimum_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerMultiZoneHumidityMinimum.*/
    class MODEL_API SetpointManagerMultiZoneHumidityMinimum_Impl : public SetpointManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerMultiZoneHumidityMinimum_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerMultiZoneHumidityMinimum_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerMultiZoneHumidityMinimum_Impl(const SetpointManagerMultiZoneHumidityMinimum_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SetpointManagerMultiZoneHumidityMinimum_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double minimumSetpointHumidityRatio() const;

      bool isMinimumSetpointHumidityRatioDefaulted() const;

      double maximumSetpointHumidityRatio() const;

      bool isMaximumSetpointHumidityRatioDefaulted() const;

      virtual boost::optional<Node> setpointNode() const override;

      virtual std::string controlVariable() const override;

      //@}
      /** @name Setters */
      //@{

      bool setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio);

      void resetMinimumSetpointHumidityRatio();

      bool setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio);

      void resetMaximumSetpointHumidityRatio();

      virtual bool setControlVariable(const std::string& controlVariable) override;

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      virtual bool setSetpointNode(const Node& node) override;

      virtual void resetSetpointNode() override;

      REGISTER_LOGGER("openstudio.model.SetpointManagerMultiZoneHumidityMinimum");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERMULTIZONEHUMIDITYMINIMUM_IMPL_HPP
