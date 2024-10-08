/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERMULTIZONEHUMIDITYMINIMUM_HPP
#define MODEL_SETPOINTMANAGERMULTIZONEHUMIDITYMINIMUM_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class SetpointManagerMultiZoneHumidityMinimum_Impl;

  }  // namespace detail

  /** SetpointManagerMultiZoneHumidityMinimum is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:MultiZone:Humidity:Minimum'. */
  class MODEL_API SetpointManagerMultiZoneHumidityMinimum : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerMultiZoneHumidityMinimum(const Model& model);

    virtual ~SetpointManagerMultiZoneHumidityMinimum() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerMultiZoneHumidityMinimum(const SetpointManagerMultiZoneHumidityMinimum& other) = default;
    SetpointManagerMultiZoneHumidityMinimum(SetpointManagerMultiZoneHumidityMinimum&& other) = default;
    SetpointManagerMultiZoneHumidityMinimum& operator=(const SetpointManagerMultiZoneHumidityMinimum&) = default;
    SetpointManagerMultiZoneHumidityMinimum& operator=(SetpointManagerMultiZoneHumidityMinimum&&) = default;

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
    using ImplType = detail::SetpointManagerMultiZoneHumidityMinimum_Impl;

    explicit SetpointManagerMultiZoneHumidityMinimum(std::shared_ptr<detail::SetpointManagerMultiZoneHumidityMinimum_Impl> impl);

    friend class detail::SetpointManagerMultiZoneHumidityMinimum_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerMultiZoneHumidityMinimum");
  };

  /** \relates SetpointManagerMultiZoneHumidityMinimum*/
  using OptionalSetpointManagerMultiZoneHumidityMinimum = boost::optional<SetpointManagerMultiZoneHumidityMinimum>;

  /** \relates SetpointManagerMultiZoneHumidityMinimum*/
  using SetpointManagerMultiZoneHumidityMinimumVector = std::vector<SetpointManagerMultiZoneHumidityMinimum>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERMULTIZONEHUMIDITYMINIMUM_HPP
