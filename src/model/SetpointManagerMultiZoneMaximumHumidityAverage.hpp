/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERMULTIZONEMAXIMUMHUMIDITYAVERAGE_HPP
#define MODEL_SETPOINTMANAGERMULTIZONEMAXIMUMHUMIDITYAVERAGE_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class SetpointManagerMultiZoneMaximumHumidityAverage_Impl;

  }  // namespace detail

  /** SetpointManagerMultiZoneMaximumHumidityAverage is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:MultiZone:MaximumHumidity:Average'. */
  class MODEL_API SetpointManagerMultiZoneMaximumHumidityAverage : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerMultiZoneMaximumHumidityAverage(const Model& model);

    virtual ~SetpointManagerMultiZoneMaximumHumidityAverage() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerMultiZoneMaximumHumidityAverage(const SetpointManagerMultiZoneMaximumHumidityAverage& other) = default;
    SetpointManagerMultiZoneMaximumHumidityAverage(SetpointManagerMultiZoneMaximumHumidityAverage&& other) = default;
    SetpointManagerMultiZoneMaximumHumidityAverage& operator=(const SetpointManagerMultiZoneMaximumHumidityAverage&) = default;
    SetpointManagerMultiZoneMaximumHumidityAverage& operator=(SetpointManagerMultiZoneMaximumHumidityAverage&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    /** @name Getters */
    //@{

    double minimumSetpointHumidityRatio() const;

    double maximumSetpointHumidityRatio() const;

    //@}
    /** @name Setters */
    //@{

    bool setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio);

    bool setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerMultiZoneMaximumHumidityAverage_Impl;

    explicit SetpointManagerMultiZoneMaximumHumidityAverage(std::shared_ptr<detail::SetpointManagerMultiZoneMaximumHumidityAverage_Impl> impl);

    friend class detail::SetpointManagerMultiZoneMaximumHumidityAverage_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerMultiZoneMaximumHumidityAverage");
  };

  /** \relates SetpointManagerMultiZoneMaximumHumidityAverage*/
  using OptionalSetpointManagerMultiZoneMaximumHumidityAverage = boost::optional<SetpointManagerMultiZoneMaximumHumidityAverage>;

  /** \relates SetpointManagerMultiZoneMaximumHumidityAverage*/
  using SetpointManagerMultiZoneMaximumHumidityAverageVector = std::vector<SetpointManagerMultiZoneMaximumHumidityAverage>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERMULTIZONEMAXIMUMHUMIDITYAVERAGE_HPP
