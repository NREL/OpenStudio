/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERWARMESTTEMPERATUREFLOW_HPP
#define MODEL_SETPOINTMANAGERWARMESTTEMPERATUREFLOW_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class SetpointManagerWarmestTemperatureFlow_Impl;

  }  // namespace detail

  /** SetpointManagerWarmestTemperatureFlow is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:WarmestTemperatureFlow'. */
  class MODEL_API SetpointManagerWarmestTemperatureFlow : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerWarmestTemperatureFlow(const Model& model);

    virtual ~SetpointManagerWarmestTemperatureFlow() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerWarmestTemperatureFlow(const SetpointManagerWarmestTemperatureFlow& other) = default;
    SetpointManagerWarmestTemperatureFlow(SetpointManagerWarmestTemperatureFlow&& other) = default;
    SetpointManagerWarmestTemperatureFlow& operator=(const SetpointManagerWarmestTemperatureFlow&) = default;
    SetpointManagerWarmestTemperatureFlow& operator=(SetpointManagerWarmestTemperatureFlow&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    static std::vector<std::string> strategyValues();

    /** @name Getters */
    //@{

    double minimumSetpointTemperature() const;

    double maximumSetpointTemperature() const;

    std::string strategy() const;

    double minimumTurndownRatio() const;

    //@}
    /** @name Setters */
    //@{

    bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

    bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

    bool setStrategy(const std::string& strategy);

    bool setMinimumTurndownRatio(double minimumTurndownRatio);

    //@}
    /** @name Other */
    //@{
    bool setSetpointNode(const Node& node);

    void resetSetpointNode();

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerWarmestTemperatureFlow_Impl;

    explicit SetpointManagerWarmestTemperatureFlow(std::shared_ptr<detail::SetpointManagerWarmestTemperatureFlow_Impl> impl);

    friend class detail::SetpointManagerWarmestTemperatureFlow_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerWarmestTemperatureFlow");
  };

  /** \relates SetpointManagerWarmestTemperatureFlow*/
  using OptionalSetpointManagerWarmestTemperatureFlow = boost::optional<SetpointManagerWarmestTemperatureFlow>;

  /** \relates SetpointManagerWarmestTemperatureFlow*/
  using SetpointManagerWarmestTemperatureFlowVector = std::vector<SetpointManagerWarmestTemperatureFlow>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERWARMESTTEMPERATUREFLOW_HPP
