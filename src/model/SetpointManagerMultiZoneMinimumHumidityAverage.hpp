/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERMULTIZONEMINIMUMHUMIDITYAVERAGE_HPP
#define MODEL_SETPOINTMANAGERMULTIZONEMINIMUMHUMIDITYAVERAGE_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class SetpointManagerMultiZoneMinimumHumidityAverage_Impl;

  }  // namespace detail

  /** SetpointManagerMultiZoneMinimumHumidityAverage is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:MultiZone:MinimumHumidity:Average'. */
  class MODEL_API SetpointManagerMultiZoneMinimumHumidityAverage : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerMultiZoneMinimumHumidityAverage(const Model& model);

    virtual ~SetpointManagerMultiZoneMinimumHumidityAverage() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerMultiZoneMinimumHumidityAverage(const SetpointManagerMultiZoneMinimumHumidityAverage& other) = default;
    SetpointManagerMultiZoneMinimumHumidityAverage(SetpointManagerMultiZoneMinimumHumidityAverage&& other) = default;
    SetpointManagerMultiZoneMinimumHumidityAverage& operator=(const SetpointManagerMultiZoneMinimumHumidityAverage&) = default;
    SetpointManagerMultiZoneMinimumHumidityAverage& operator=(SetpointManagerMultiZoneMinimumHumidityAverage&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    /** @name Getters */
    //@{

    double minimumSetpointHumidityRatio() const;

    bool isMinimumSetpointHumidityRatioDefaulted() const;

    double maximumSetpointHumidityRatio() const;

    bool isMaximumSetpointHumidityRatioDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio);

    void resetMinimumSetpointHumidityRatio();

    bool setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio);

    void resetMaximumSetpointHumidityRatio();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl;

    explicit SetpointManagerMultiZoneMinimumHumidityAverage(std::shared_ptr<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl> impl);

    friend class detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerMultiZoneMinimumHumidityAverage");
  };

  /** \relates SetpointManagerMultiZoneMinimumHumidityAverage*/
  using OptionalSetpointManagerMultiZoneMinimumHumidityAverage = boost::optional<SetpointManagerMultiZoneMinimumHumidityAverage>;

  /** \relates SetpointManagerMultiZoneMinimumHumidityAverage*/
  using SetpointManagerMultiZoneMinimumHumidityAverageVector = std::vector<SetpointManagerMultiZoneMinimumHumidityAverage>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERMULTIZONEMINIMUMHUMIDITYAVERAGE_HPP
