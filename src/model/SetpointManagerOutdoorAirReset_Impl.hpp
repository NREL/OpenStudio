/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGEROUTDOORAIRRESET_IMPL_HPP
#define MODEL_SETPOINTMANAGEROUTDOORAIRRESET_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;
  class Schedule;

  namespace detail {

    /** SetpointManagerOutdoorAirReset_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerOutdoorAirReset.*/
    class MODEL_API SetpointManagerOutdoorAirReset_Impl : public SetpointManager_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerOutdoorAirReset_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerOutdoorAirReset_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerOutdoorAirReset_Impl(const SetpointManagerOutdoorAirReset_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SetpointManagerOutdoorAirReset_Impl() override = default;

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

      bool isControlVariableDefaulted() const;

      double setpointatOutdoorLowTemperature() const;

      double outdoorLowTemperature() const;

      double setpointatOutdoorHighTemperature() const;

      double outdoorHighTemperature() const;

      virtual boost::optional<Node> setpointNode() const override;

      boost::optional<Schedule> schedule() const;

      boost::optional<double> setpointatOutdoorLowTemperature2() const;

      boost::optional<double> outdoorLowTemperature2() const;

      boost::optional<double> setpointatOutdoorHighTemperature2() const;

      boost::optional<double> outdoorHighTemperature2() const;

      //@}
      /** @name Setters */
      //@{

      virtual bool setControlVariable(const std::string& controlVariable) override;

      void resetControlVariable();

      bool setSetpointatOutdoorLowTemperature(double setpointatOutdoorLowTemperature);

      bool setOutdoorLowTemperature(double outdoorLowTemperature);

      bool setSetpointatOutdoorHighTemperature(double setpointatOutdoorHighTemperature);

      bool setOutdoorHighTemperature(double outdoorHighTemperature);

      bool setSchedule(Schedule& schedule);

      void resetSchedule();

      bool setSetpointatOutdoorLowTemperature2(boost::optional<double> setpointatOutdoorLowTemperature2);

      void resetSetpointatOutdoorLowTemperature2();

      bool setOutdoorLowTemperature2(boost::optional<double> outdoorLowTemperature2);

      void resetOutdoorLowTemperature2();

      bool setSetpointatOutdoorHighTemperature2(boost::optional<double> setpointatOutdoorHighTemperature2);

      void resetSetpointatOutdoorHighTemperature2();

      bool setOutdoorHighTemperature2(boost::optional<double> outdoorHighTemperature2);

      void resetOutdoorHighTemperature2();

      //@}
      /** @name Other */
      //@{

      // bool addToNode(Node & node);

      //@}
     protected:
     private:
      virtual bool setSetpointNode(const Node& node) override;

      virtual void resetSetpointNode() override;

      REGISTER_LOGGER("openstudio.model.SetpointManagerOutdoorAirReset");

      std::vector<std::string> controlVariableValues() const;
      boost::optional<ModelObject> setpointNodeAsModelObject() const;
      boost::optional<ModelObject> scheduleAsModelObject() const;

      bool setSetpointNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGEROUTDOORAIRRESET_IMPL_HPP
