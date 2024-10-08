/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERWARMEST_HPP
#define MODEL_SETPOINTMANAGERWARMEST_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class SetpointManagerWarmest_Impl;

  }  // namespace detail

  /** SetpointManagerWarmest is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:Warmest'. */
  class MODEL_API SetpointManagerWarmest : public SetpointManager
  {
   public:
    explicit SetpointManagerWarmest(const Model& model);

    virtual ~SetpointManagerWarmest() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerWarmest(const SetpointManagerWarmest& other) = default;
    SetpointManagerWarmest(SetpointManagerWarmest&& other) = default;
    SetpointManagerWarmest& operator=(const SetpointManagerWarmest&) = default;
    SetpointManagerWarmest& operator=(SetpointManagerWarmest&&) = default;

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    static std::vector<std::string> strategyValues();

    double minimumSetpointTemperature() const;

    bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

    double maximumSetpointTemperature() const;

    bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

    std::string strategy() const;

    bool setStrategy(const std::string& strategy);

   protected:
    /// @cond
    using ImplType = detail::SetpointManagerWarmest_Impl;

    explicit SetpointManagerWarmest(std::shared_ptr<detail::SetpointManagerWarmest_Impl> impl);

    friend class detail::SetpointManagerWarmest_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerWarmest");
  };

  /** \relates SetpointManagerWarmest*/
  using OptionalSetpointManagerWarmest = boost::optional<SetpointManagerWarmest>;

  /** \relates SetpointManagerWarmest*/
  using SetpointManagerWarmestVector = std::vector<SetpointManagerWarmest>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERWARMEST_HPP
