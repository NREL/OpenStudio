/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSCHEDULEDDUALSETPOINT_IMPL_HPP
#define MODEL_SETPOINTMANAGERSCHEDULEDDUALSETPOINT_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Node;

  namespace detail {

    /** SetpointManagerScheduledDualSetpoint_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerScheduledDualSetpoint.*/
    class MODEL_API SetpointManagerScheduledDualSetpoint_Impl : public SetpointManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerScheduledDualSetpoint_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerScheduledDualSetpoint_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerScheduledDualSetpoint_Impl(const SetpointManagerScheduledDualSetpoint_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SetpointManagerScheduledDualSetpoint_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      // virtual bool addToNode(Node & node) override;

      virtual bool isAllowedOnPlantLoop() const override;

      //@}
      /** @name Getters */
      //@{

      virtual std::string controlVariable() const override;

      boost::optional<Schedule> highSetpointSchedule() const;

      boost::optional<Schedule> lowSetpointSchedule() const;

      virtual boost::optional<Node> setpointNode() const override;

      //@}
      /** @name Setters */
      //@{

      virtual bool setControlVariable(const std::string& controlVariable) override;

      bool setHighSetpointSchedule(Schedule& schedule);

      void resetHighSetpointSchedule();

      bool setLowSetpointSchedule(Schedule& schedule);

      void resetLowSetpointSchedule();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      virtual bool setSetpointNode(const Node& node) override;

      virtual void resetSetpointNode() override;

      REGISTER_LOGGER("openstudio.model.SetpointManagerScheduledDualSetpoint");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSCHEDULEDDUALSETPOINT_IMPL_HPP
