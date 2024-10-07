/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSCHEDULED_IMPL_HPP
#define MODEL_SETPOINTMANAGERSCHEDULED_IMPL_HPP

#include "SetpointManager_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class MODEL_API SetpointManagerScheduled_Impl : public SetpointManager_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerScheduled_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerScheduled_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerScheduled_Impl(const SetpointManagerScheduled_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~SetpointManagerScheduled_Impl() override = default;

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

      virtual boost::optional<Node> setpointNode() const override;

      virtual std::string controlVariable() const override;

      Schedule schedule() const;

      bool hasSchedule() const;

      //@}
      /** @name Setters */
      //@{

      virtual bool setControlVariable(const std::string& controlVariable) override;

      bool setSchedule(Schedule& schedule);

      //@}
      /** @name Other */
      //@{

      /** Sets control variable and schedule simultaneously. Use in the case that the
   *  control variable change implies a change in schedule units. */
      bool setControlVariableAndSchedule(const std::string& controlVariable, Schedule& schedule);

      //@}
     private:
      virtual bool setSetpointNode(const Node& node) override;

      virtual void resetSetpointNode() override;

      // converts control variable to the schedule's display name
      std::string scheduleDisplayName() const;

      // as above, but used to check candidate control variables
      std::string scheduleDisplayName(const std::string& candidateControlVariable) const;

      std::vector<std::string> controlVariableValues() const;

      boost::optional<ModelObject> scheduleAsModelObject() const;

      bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSCHEDULED_IMPL_HPP
