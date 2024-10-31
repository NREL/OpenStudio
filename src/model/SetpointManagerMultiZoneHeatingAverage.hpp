/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERMULTIZONEHEATINGAVERAGE_HPP
#define MODEL_SETPOINTMANAGERMULTIZONEHEATINGAVERAGE_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class SetpointManagerMultiZoneHeatingAverage_Impl;

  }  // namespace detail

  /** SetpointManagerMultiZoneHeatingAverage is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:MultiZone:Heating:Average'. */
  class MODEL_API SetpointManagerMultiZoneHeatingAverage : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerMultiZoneHeatingAverage(const Model& model);

    virtual ~SetpointManagerMultiZoneHeatingAverage() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerMultiZoneHeatingAverage(const SetpointManagerMultiZoneHeatingAverage& other) = default;
    SetpointManagerMultiZoneHeatingAverage(SetpointManagerMultiZoneHeatingAverage&& other) = default;
    SetpointManagerMultiZoneHeatingAverage& operator=(const SetpointManagerMultiZoneHeatingAverage&) = default;
    SetpointManagerMultiZoneHeatingAverage& operator=(SetpointManagerMultiZoneHeatingAverage&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    /** @name Getters */
    //@{

    double minimumSetpointTemperature() const;

    double maximumSetpointTemperature() const;

    //@}
    /** @name Setters */
    //@{

    bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

    bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerMultiZoneHeatingAverage_Impl;

    explicit SetpointManagerMultiZoneHeatingAverage(std::shared_ptr<detail::SetpointManagerMultiZoneHeatingAverage_Impl> impl);

    friend class detail::SetpointManagerMultiZoneHeatingAverage_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerMultiZoneHeatingAverage");
  };

  /** \relates SetpointManagerMultiZoneHeatingAverage*/
  using OptionalSetpointManagerMultiZoneHeatingAverage = boost::optional<SetpointManagerMultiZoneHeatingAverage>;

  /** \relates SetpointManagerMultiZoneHeatingAverage*/
  using SetpointManagerMultiZoneHeatingAverageVector = std::vector<SetpointManagerMultiZoneHeatingAverage>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERMULTIZONEHEATINGAVERAGE_HPP
