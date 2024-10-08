/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERMULTIZONEHUMIDITYMAXIMUM_HPP
#define MODEL_SETPOINTMANAGERMULTIZONEHUMIDITYMAXIMUM_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class SetpointManagerMultiZoneHumidityMaximum_Impl;

  }  // namespace detail

  /** SetpointManagerMultiZoneHumidityMaximum is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:MultiZone:Humidity:Maximum'. */
  class MODEL_API SetpointManagerMultiZoneHumidityMaximum : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerMultiZoneHumidityMaximum(const Model& model);

    virtual ~SetpointManagerMultiZoneHumidityMaximum() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerMultiZoneHumidityMaximum(const SetpointManagerMultiZoneHumidityMaximum& other) = default;
    SetpointManagerMultiZoneHumidityMaximum(SetpointManagerMultiZoneHumidityMaximum&& other) = default;
    SetpointManagerMultiZoneHumidityMaximum& operator=(const SetpointManagerMultiZoneHumidityMaximum&) = default;
    SetpointManagerMultiZoneHumidityMaximum& operator=(SetpointManagerMultiZoneHumidityMaximum&&) = default;

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
    using ImplType = detail::SetpointManagerMultiZoneHumidityMaximum_Impl;

    explicit SetpointManagerMultiZoneHumidityMaximum(std::shared_ptr<detail::SetpointManagerMultiZoneHumidityMaximum_Impl> impl);

    friend class detail::SetpointManagerMultiZoneHumidityMaximum_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerMultiZoneHumidityMaximum");
  };

  /** \relates SetpointManagerMultiZoneHumidityMaximum*/
  using OptionalSetpointManagerMultiZoneHumidityMaximum = boost::optional<SetpointManagerMultiZoneHumidityMaximum>;

  /** \relates SetpointManagerMultiZoneHumidityMaximum*/
  using SetpointManagerMultiZoneHumidityMaximumVector = std::vector<SetpointManagerMultiZoneHumidityMaximum>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERMULTIZONEHUMIDITYMAXIMUM_HPP
