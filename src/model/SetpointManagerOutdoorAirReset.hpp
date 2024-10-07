/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGEROUTDOORAIRRESET_HPP
#define MODEL_SETPOINTMANAGEROUTDOORAIRRESET_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;
  class Schedule;

  namespace detail {

    class SetpointManagerOutdoorAirReset_Impl;

  }  // namespace detail

  /** SetpointManagerOutdoorAirReset is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:OutdoorAirReset'. */
  class MODEL_API SetpointManagerOutdoorAirReset : public SetpointManager
  {
   public:
    explicit SetpointManagerOutdoorAirReset(const Model& model);

    virtual ~SetpointManagerOutdoorAirReset() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerOutdoorAirReset(const SetpointManagerOutdoorAirReset& other) = default;
    SetpointManagerOutdoorAirReset(SetpointManagerOutdoorAirReset&& other) = default;
    SetpointManagerOutdoorAirReset& operator=(const SetpointManagerOutdoorAirReset&) = default;
    SetpointManagerOutdoorAirReset& operator=(SetpointManagerOutdoorAirReset&&) = default;

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    boost::optional<Node> setpointNode() const;

    std::string controlVariable() const;

    bool isControlVariableDefaulted() const;

    bool setControlVariable(const std::string& controlVariable);

    void resetControlVariable();

    double setpointatOutdoorLowTemperature() const;

    bool setSetpointatOutdoorLowTemperature(double setpointatOutdoorLowTemperature);

    double outdoorLowTemperature() const;

    bool setOutdoorLowTemperature(double outdoorLowTemperature);

    double setpointatOutdoorHighTemperature() const;

    bool setSetpointatOutdoorHighTemperature(double setpointatOutdoorHighTemperature);

    double outdoorHighTemperature() const;

    bool setOutdoorHighTemperature(double outdoorHighTemperature);

    boost::optional<Schedule> schedule() const;

    bool setSchedule(Schedule& schedule);

    void resetSchedule();

    boost::optional<double> setpointatOutdoorLowTemperature2() const;

    bool setSetpointatOutdoorLowTemperature2(double setpointatOutdoorLowTemperature2);

    void resetSetpointatOutdoorLowTemperature2();

    boost::optional<double> outdoorLowTemperature2() const;

    bool setOutdoorLowTemperature2(double outdoorLowTemperature2);

    void resetOutdoorLowTemperature2();

    boost::optional<double> setpointatOutdoorHighTemperature2() const;

    bool setSetpointatOutdoorHighTemperature2(double setpointatOutdoorHighTemperature2);

    void resetSetpointatOutdoorHighTemperature2();

    boost::optional<double> outdoorHighTemperature2() const;

    bool setOutdoorHighTemperature2(double outdoorHighTemperature2);

    void resetOutdoorHighTemperature2();

   protected:
    /// @cond
    using ImplType = detail::SetpointManagerOutdoorAirReset_Impl;

    explicit SetpointManagerOutdoorAirReset(std::shared_ptr<detail::SetpointManagerOutdoorAirReset_Impl> impl);

    friend class detail::SetpointManagerOutdoorAirReset_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerOutdoorAirReset");
  };

  /** \relates SetpointManagerOutdoorAirReset*/
  using OptionalSetpointManagerOutdoorAirReset = boost::optional<SetpointManagerOutdoorAirReset>;

  /** \relates SetpointManagerOutdoorAirReset*/
  using SetpointManagerOutdoorAirResetVector = std::vector<SetpointManagerOutdoorAirReset>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGEROUTDOORAIRRESET_HPP
