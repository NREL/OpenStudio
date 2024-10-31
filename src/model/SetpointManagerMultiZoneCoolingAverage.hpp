/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERMULTIZONECOOLINGAVERAGE_HPP
#define MODEL_SETPOINTMANAGERMULTIZONECOOLINGAVERAGE_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class SetpointManagerMultiZoneCoolingAverage_Impl;

  }  // namespace detail

  /** SetpointManagerMultiZoneCoolingAverage is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:MultiZone:Cooling:Average'. */
  class MODEL_API SetpointManagerMultiZoneCoolingAverage : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerMultiZoneCoolingAverage(const Model& model);

    virtual ~SetpointManagerMultiZoneCoolingAverage() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerMultiZoneCoolingAverage(const SetpointManagerMultiZoneCoolingAverage& other) = default;
    SetpointManagerMultiZoneCoolingAverage(SetpointManagerMultiZoneCoolingAverage&& other) = default;
    SetpointManagerMultiZoneCoolingAverage& operator=(const SetpointManagerMultiZoneCoolingAverage&) = default;
    SetpointManagerMultiZoneCoolingAverage& operator=(SetpointManagerMultiZoneCoolingAverage&&) = default;

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
    using ImplType = detail::SetpointManagerMultiZoneCoolingAverage_Impl;

    explicit SetpointManagerMultiZoneCoolingAverage(std::shared_ptr<detail::SetpointManagerMultiZoneCoolingAverage_Impl> impl);

    friend class detail::SetpointManagerMultiZoneCoolingAverage_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerMultiZoneCoolingAverage");
  };

  /** \relates SetpointManagerMultiZoneCoolingAverage*/
  using OptionalSetpointManagerMultiZoneCoolingAverage = boost::optional<SetpointManagerMultiZoneCoolingAverage>;

  /** \relates SetpointManagerMultiZoneCoolingAverage*/
  using SetpointManagerMultiZoneCoolingAverageVector = std::vector<SetpointManagerMultiZoneCoolingAverage>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERMULTIZONECOOLINGAVERAGE_HPP
