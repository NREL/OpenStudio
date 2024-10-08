/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGEROUTDOORAIRPRETREAT_HPP
#define MODEL_SETPOINTMANAGEROUTDOORAIRPRETREAT_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class SetpointManagerOutdoorAirPretreat_Impl;

  }  // namespace detail

  /** SetpointManagerOutdoorAirPretreat is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:OutdoorAirPretreat'. */
  class MODEL_API SetpointManagerOutdoorAirPretreat : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerOutdoorAirPretreat(const Model& model);

    virtual ~SetpointManagerOutdoorAirPretreat() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerOutdoorAirPretreat(const SetpointManagerOutdoorAirPretreat& other) = default;
    SetpointManagerOutdoorAirPretreat(SetpointManagerOutdoorAirPretreat&& other) = default;
    SetpointManagerOutdoorAirPretreat& operator=(const SetpointManagerOutdoorAirPretreat&) = default;
    SetpointManagerOutdoorAirPretreat& operator=(SetpointManagerOutdoorAirPretreat&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    /** @name Getters */
    //@{

    double minimumSetpointTemperature() const;

    bool isMinimumSetpointTemperatureDefaulted() const;

    double maximumSetpointTemperature() const;

    bool isMaximumSetpointTemperatureDefaulted() const;

    double minimumSetpointHumidityRatio() const;

    bool isMinimumSetpointHumidityRatioDefaulted() const;

    double maximumSetpointHumidityRatio() const;

    bool isMaximumSetpointHumidityRatioDefaulted() const;

    boost::optional<Node> referenceSetpointNode() const;

    boost::optional<Node> mixedAirStreamNode() const;

    boost::optional<Node> outdoorAirStreamNode() const;

    boost::optional<Node> returnAirStreamNode() const;

    //@}
    /** @name Setters */
    //@{

    void resetControlVariable();

    bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

    void resetMinimumSetpointTemperature();

    bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

    void resetMaximumSetpointTemperature();

    bool setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio);

    void resetMinimumSetpointHumidityRatio();

    bool setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio);

    void resetMaximumSetpointHumidityRatio();

    bool setReferenceSetpointNode(const Node& node);

    void resetReferenceSetpointNode();

    bool setMixedAirStreamNode(const Node& node);

    void resetMixedAirStreamNode();

    bool setOutdoorAirStreamNode(const Node& node);

    void resetOutdoorAirStreamNode();

    bool setReturnAirStreamNode(const Node& node);

    void resetReturnAirStreamNode();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerOutdoorAirPretreat_Impl;

    explicit SetpointManagerOutdoorAirPretreat(std::shared_ptr<detail::SetpointManagerOutdoorAirPretreat_Impl> impl);

    friend class detail::SetpointManagerOutdoorAirPretreat_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerOutdoorAirPretreat");
  };

  /** \relates SetpointManagerOutdoorAirPretreat*/
  using OptionalSetpointManagerOutdoorAirPretreat = boost::optional<SetpointManagerOutdoorAirPretreat>;

  /** \relates SetpointManagerOutdoorAirPretreat*/
  using SetpointManagerOutdoorAirPretreatVector = std::vector<SetpointManagerOutdoorAirPretreat>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGEROUTDOORAIRPRETREAT_HPP
