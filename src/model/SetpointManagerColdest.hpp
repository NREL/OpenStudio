/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERCOLDEST_HPP
#define MODEL_SETPOINTMANAGERCOLDEST_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class SetpointManagerColdest_Impl;

  }  // namespace detail

  /** SetpointManagerColdest is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:Coldest'. */
  class MODEL_API SetpointManagerColdest : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerColdest(const Model& model);

    virtual ~SetpointManagerColdest() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerColdest(const SetpointManagerColdest& other) = default;
    SetpointManagerColdest(SetpointManagerColdest&& other) = default;
    SetpointManagerColdest& operator=(const SetpointManagerColdest&) = default;
    SetpointManagerColdest& operator=(SetpointManagerColdest&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    static std::vector<std::string> strategyValues();

    /** @name Getters */
    //@{

    double minimumSetpointTemperature() const;

    double maximumSetpointTemperature() const;

    std::string strategy() const;

    //@}
    /** @name Setters */
    //@{

    bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

    bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

    bool setStrategy(const std::string& strategy);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerColdest_Impl;

    explicit SetpointManagerColdest(std::shared_ptr<detail::SetpointManagerColdest_Impl> impl);

    friend class detail::SetpointManagerColdest_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerColdest");
  };

  /** \relates SetpointManagerColdest*/
  using OptionalSetpointManagerColdest = boost::optional<SetpointManagerColdest>;

  /** \relates SetpointManagerColdest*/
  using SetpointManagerColdestVector = std::vector<SetpointManagerColdest>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERCOLDEST_HPP
