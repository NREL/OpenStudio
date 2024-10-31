/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_TEMPERINGVALVE_HPP
#define MODEL_TEMPERINGVALVE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    class TemperingValve_Impl;

  }  // namespace detail

  /** TemperingValve is a StraightComponent that wraps the OpenStudio IDD object 'OS:TemperingValve'. */
  class MODEL_API TemperingValve : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit TemperingValve(const Model& model);

    virtual ~TemperingValve() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    TemperingValve(const TemperingValve& other) = default;
    TemperingValve(TemperingValve&& other) = default;
    TemperingValve& operator=(const TemperingValve&) = default;
    TemperingValve& operator=(TemperingValve&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Node> stream2SourceNode() const;

    boost::optional<Node> temperatureSetpointNode() const;

    boost::optional<Node> pumpOutletNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setStream2SourceNode(const Node& stream2SourceNode);

    void resetStream2SourceNode();

    bool setTemperatureSetpointNode(const Node& temperatureSetpointNode);

    void resetTemperatureSetpointNode();

    bool setPumpOutletNode(const Node& pumpOutletNode);

    void resetPumpOutletNode();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::TemperingValve_Impl;

    explicit TemperingValve(std::shared_ptr<detail::TemperingValve_Impl> impl);

    friend class detail::TemperingValve_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.TemperingValve");
  };

  /** \relates TemperingValve*/
  using OptionalTemperingValve = boost::optional<TemperingValve>;

  /** \relates TemperingValve*/
  using TemperingValveVector = std::vector<TemperingValve>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_TEMPERINGVALVE_HPP
