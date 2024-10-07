/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERFOLLOWOUTDOORAIRTEMPERATURE_IMPL_HPP
#define MODEL_SETPOINTMANAGERFOLLOWOUTDOORAIRTEMPERATURE_IMPL_HPP

#include "SetpointManagerFollowOutdoorAirTemperature.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class MODEL_API SetpointManagerFollowOutdoorAirTemperature_Impl : public SetpointManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerFollowOutdoorAirTemperature_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerFollowOutdoorAirTemperature_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerFollowOutdoorAirTemperature_Impl(const SetpointManagerFollowOutdoorAirTemperature_Impl& other, Model_Impl* model,
                                                      bool keepHandles);

      virtual ~SetpointManagerFollowOutdoorAirTemperature_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      // virtual bool addToNode(Node & node) override;

      virtual bool isAllowedOnPlantLoop() const override;

      //@}
      /** @name Getters and Setters */
      //@{

      std::string referenceTemperatureType() const;

      bool setReferenceTemperatureType(const std::string& value);

      double offsetTemperatureDifference() const;

      bool setOffsetTemperatureDifference(double value);

      double maximumSetpointTemperature() const;

      bool setMaximumSetpointTemperature(double value);

      double minimumSetpointTemperature() const;

      bool setMinimumSetpointTemperature(double value);

      virtual boost::optional<Node> setpointNode() const override;

      virtual std::string controlVariable() const override;

      virtual bool setControlVariable(const std::string& value) override;

      //@}
     private:
      virtual bool setSetpointNode(const Node& node) override;

      virtual void resetSetpointNode() override;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERFOLLOWOUTDOORAIRTEMPERATURE_IMPL_HPP
