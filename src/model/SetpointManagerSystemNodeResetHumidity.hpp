/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSYSTEMNODERESETHUMIDITY_HPP
#define MODEL_SETPOINTMANAGERSYSTEMNODERESETHUMIDITY_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class SetpointManagerSystemNodeResetHumidity_Impl;

  }  // namespace detail

  /** SetpointManagerSystemNodeResetHumidity is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:SystemNodeReset:Humidity'. */
  class MODEL_API SetpointManagerSystemNodeResetHumidity : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerSystemNodeResetHumidity(const Model& model);

    virtual ~SetpointManagerSystemNodeResetHumidity() override = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    /** @name Getters */
    //@{

    double setpointatLowReferenceHumidityRatio() const;

    double setpointatHighReferenceHumidityRatio() const;

    double lowReferenceHumidityRatio() const;

    double highReferenceHumidityRatio() const;

    boost::optional<Node> referenceNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setSetpointatLowReferenceHumidityRatio(double setpointatLowReferenceHumidityRatio);

    bool setSetpointatHighReferenceHumidityRatio(double setpointatHighReferenceHumidityRatio);

    bool setLowReferenceHumidityRatio(double lowReferenceHumidityRatio);

    bool setHighReferenceHumidityRatio(double highReferenceHumidityRatio);

    bool setReferenceNode(const Node& node);

    void resetReferenceNode();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerSystemNodeResetHumidity_Impl;

    explicit SetpointManagerSystemNodeResetHumidity(std::shared_ptr<detail::SetpointManagerSystemNodeResetHumidity_Impl> impl);

    friend class detail::SetpointManagerSystemNodeResetHumidity_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerSystemNodeResetHumidity");
  };

  /** \relates SetpointManagerSystemNodeResetHumidity*/
  using OptionalSetpointManagerSystemNodeResetHumidity = boost::optional<SetpointManagerSystemNodeResetHumidity>;

  /** \relates SetpointManagerSystemNodeResetHumidity*/
  using SetpointManagerSystemNodeResetHumidityVector = std::vector<SetpointManagerSystemNodeResetHumidity>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSYSTEMNODERESETHUMIDITY_HPP
