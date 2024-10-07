/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERFOLLOWSYSTEMNODETEMPERATURE_HPP
#define MODEL_SETPOINTMANAGERFOLLOWSYSTEMNODETEMPERATURE_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class SetpointManagerFollowSystemNodeTemperature_Impl;

  }  // namespace detail

  /** SetpointManagerFollowSystemNodeTemperature is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:FollowSystemNodeTemperature'. */
  class MODEL_API SetpointManagerFollowSystemNodeTemperature : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerFollowSystemNodeTemperature(const Model& model);

    virtual ~SetpointManagerFollowSystemNodeTemperature() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerFollowSystemNodeTemperature(const SetpointManagerFollowSystemNodeTemperature& other) = default;
    SetpointManagerFollowSystemNodeTemperature(SetpointManagerFollowSystemNodeTemperature&& other) = default;
    SetpointManagerFollowSystemNodeTemperature& operator=(const SetpointManagerFollowSystemNodeTemperature&) = default;
    SetpointManagerFollowSystemNodeTemperature& operator=(SetpointManagerFollowSystemNodeTemperature&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    static std::vector<std::string> referenceTemperatureTypeValues();

    /** @name Getters */
    //@{

    std::string controlVariable() const;

    boost::optional<Node> referenceNode() const;

    std::string referenceTemperatureType() const;

    double offsetTemperatureDifference() const;

    double maximumLimitSetpointTemperature() const;

    double minimumLimitSetpointTemperature() const;

    boost::optional<Node> setpointNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlVariable(const std::string& controlVariable);

    bool setReferenceNode(const Node& node);

    void resetReferenceNode();

    bool setReferenceTemperatureType(const std::string& referenceTemperatureType);

    bool setOffsetTemperatureDifference(double offsetTemperatureDifference);

    bool setMaximumLimitSetpointTemperature(double maximumLimitSetpointTemperature);

    bool setMinimumLimitSetpointTemperature(double minimumLimitSetpointTemperature);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerFollowSystemNodeTemperature_Impl;

    explicit SetpointManagerFollowSystemNodeTemperature(std::shared_ptr<detail::SetpointManagerFollowSystemNodeTemperature_Impl> impl);

    friend class detail::SetpointManagerFollowSystemNodeTemperature_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerFollowSystemNodeTemperature");
  };

  /** \relates SetpointManagerFollowSystemNodeTemperature*/
  using OptionalSetpointManagerFollowSystemNodeTemperature = boost::optional<SetpointManagerFollowSystemNodeTemperature>;

  /** \relates SetpointManagerFollowSystemNodeTemperature*/
  using SetpointManagerFollowSystemNodeTemperatureVector = std::vector<SetpointManagerFollowSystemNodeTemperature>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERFOLLOWSYSTEMNODETEMPERATURE_HPP
