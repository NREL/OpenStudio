/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERFOLLOWGROUNDTEMPERATURE_HPP
#define MODEL_SETPOINTMANAGERFOLLOWGROUNDTEMPERATURE_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class SetpointManagerFollowGroundTemperature_Impl;

  }  // namespace detail

  /** SetpointManagerFollowGroundTemperature is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:FollowGroundTemperature'. */
  class MODEL_API SetpointManagerFollowGroundTemperature : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerFollowGroundTemperature(const Model& model);

    virtual ~SetpointManagerFollowGroundTemperature() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerFollowGroundTemperature(const SetpointManagerFollowGroundTemperature& other) = default;
    SetpointManagerFollowGroundTemperature(SetpointManagerFollowGroundTemperature&& other) = default;
    SetpointManagerFollowGroundTemperature& operator=(const SetpointManagerFollowGroundTemperature&) = default;
    SetpointManagerFollowGroundTemperature& operator=(SetpointManagerFollowGroundTemperature&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    static std::vector<std::string> referenceGroundTemperatureObjectTypeValues();

    /** @name Getters */
    //@{

    std::string referenceGroundTemperatureObjectType() const;

    double offsetTemperatureDifference() const;

    double maximumSetpointTemperature() const;

    double minimumSetpointTemperature() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceGroundTemperatureObjectType(const std::string& groundTemperatureObjType);

    bool setOffsetTemperatureDifference(double offsetTemperatureDifference);

    bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

    bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerFollowGroundTemperature_Impl;

    explicit SetpointManagerFollowGroundTemperature(std::shared_ptr<detail::SetpointManagerFollowGroundTemperature_Impl> impl);

    friend class detail::SetpointManagerFollowGroundTemperature_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerFollowGroundTemperature");
  };

  /** \relates SetpointManagerFollowGroundTemperature*/
  using OptionalSetpointManagerFollowGroundTemperature = boost::optional<SetpointManagerFollowGroundTemperature>;

  /** \relates SetpointManagerFollowGroundTemperature*/
  using SetpointManagerFollowGroundTemperatureVector = std::vector<SetpointManagerFollowGroundTemperature>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERFOLLOWGROUNDTEMPERATURE_HPP
