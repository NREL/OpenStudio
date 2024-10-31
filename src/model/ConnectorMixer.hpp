/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONNECTORMIXER_HPP
#define MODEL_CONNECTORMIXER_HPP

#include "ModelAPI.hpp"
#include "Mixer.hpp"

namespace openstudio {

namespace model {

  namespace detail {
    class ConnectorMixer_Impl;
  }  // namespace detail

  /** Mixer is an interface to the IDD object named "OS:Connector:Mixer"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of mixer objects in EnergyPlus.  Methods are built around the
 *  acts of getting the inlet and outlet ports to the mixer.  Branch indexes
 *  are used to refer to the many inlet ports of the mixer
 */
  class MODEL_API ConnectorMixer : public Mixer
  {
   public:
    /** Constructs a new Mixer object and places it inside the model. */
    explicit ConnectorMixer(const Model& model);

    virtual ~ConnectorMixer() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ConnectorMixer(const ConnectorMixer& other) = default;
    ConnectorMixer(ConnectorMixer&& other) = default;
    ConnectorMixer& operator=(const ConnectorMixer&) = default;
    ConnectorMixer& operator=(ConnectorMixer&&) = default;

    unsigned outletPort() const override;

    unsigned inletPort(unsigned branchIndex) const override;

    unsigned nextInletPort() const override;

    virtual bool addToNode(Node& node);

    virtual std::vector<openstudio::IdfObject> remove();

    virtual ModelObject clone(Model model) const;

    static IddObjectType iddObjectType();

   protected:
    using ImplType = detail::ConnectorMixer_Impl;

    friend class Model;

    friend class openstudio::IdfObject;

    explicit ConnectorMixer(std::shared_ptr<detail::ConnectorMixer_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ConnectorMixer");

    ConnectorMixer(const Handle& handle, const Model& model);
  };

  /** \relates ConnectorMixer */
  using OptionalConnectorMixer = boost::optional<ConnectorMixer>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_CONNECTORMIXER_HPP
