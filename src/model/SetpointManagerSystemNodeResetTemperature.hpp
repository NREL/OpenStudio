/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSYSTEMNODERESETTEMPERATURE_HPP
#define MODEL_SETPOINTMANAGERSYSTEMNODERESETTEMPERATURE_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class SetpointManagerSystemNodeResetTemperature_Impl;

  }  // namespace detail

  /** SetpointManagerSystemNodeResetTemperature is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:SystemNodeReset:Temperature'. */
  class MODEL_API SetpointManagerSystemNodeResetTemperature : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerSystemNodeResetTemperature(const Model& model);

    virtual ~SetpointManagerSystemNodeResetTemperature() override = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    /** @name Getters */
    //@{

    double setpointatLowReferenceTemperature() const;

    double setpointatHighReferenceTemperature() const;

    double lowReferenceTemperature() const;

    double highReferenceTemperature() const;

    boost::optional<Node> referenceNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setSetpointatLowReferenceTemperature(double setpointatLowReferenceTemperature);

    bool setSetpointatHighReferenceTemperature(double setpointatHighReferenceTemperature);

    bool setLowReferenceTemperature(double lowReferenceTemperature);

    bool setHighReferenceTemperature(double highReferenceTemperature);

    bool setReferenceNode(const Node& node);

    void resetReferenceNode();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerSystemNodeResetTemperature_Impl;

    explicit SetpointManagerSystemNodeResetTemperature(std::shared_ptr<detail::SetpointManagerSystemNodeResetTemperature_Impl> impl);

    friend class detail::SetpointManagerSystemNodeResetTemperature_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerSystemNodeResetTemperature");
  };

  /** \relates SetpointManagerSystemNodeResetTemperature*/
  using OptionalSetpointManagerSystemNodeResetTemperature = boost::optional<SetpointManagerSystemNodeResetTemperature>;

  /** \relates SetpointManagerSystemNodeResetTemperature*/
  using SetpointManagerSystemNodeResetTemperatureVector = std::vector<SetpointManagerSystemNodeResetTemperature>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSYSTEMNODERESETTEMPERATURE_HPP
