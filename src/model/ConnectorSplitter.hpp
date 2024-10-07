/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONNECTORSPLITTER_HPP
#define MODEL_CONNECTORSPLITTER_HPP

#include "ModelAPI.hpp"
#include "Splitter.hpp"

namespace openstudio {

namespace model {

  namespace detail {
    class ConnectorSplitter_Impl;
  }  // namespace detail

  /** ConnectorSplitter is an interface to the EnergyPlus IDD object named "OS:Connector:Splitter"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of splitter objects in EnergyPlus.  Methods are built around the
 *  acts of getting the inlet and outlet ports to the splitter.  Branch indexes
 *  are used to refer to the many outlet ports of the mixer
 */
  class MODEL_API ConnectorSplitter : public Splitter
  {
   public:
    /** Constructs a new Splitter object and places it inside the model. */
    explicit ConnectorSplitter(const Model& model);

    virtual ~ConnectorSplitter() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ConnectorSplitter(const ConnectorSplitter& other) = default;
    ConnectorSplitter(ConnectorSplitter&& other) = default;
    ConnectorSplitter& operator=(const ConnectorSplitter&) = default;
    ConnectorSplitter& operator=(ConnectorSplitter&&) = default;

    /** Returns the inlet port to the splitter. */
    unsigned inletPort() const override;

    /** Returns the outlet port for branchIndex.  Branches consequtively
   *  indexed starting from 0.
   */
    unsigned outletPort(unsigned branchIndex) const override;

    /** Returns the next available outlet port.  This will be the first port
   *  with no connected objects */
    unsigned nextOutletPort() const override;

    virtual bool addToNode(Node& node);

    virtual std::vector<openstudio::IdfObject> remove();

    virtual ModelObject clone(Model model) const;

    static IddObjectType iddObjectType();

   protected:
    using ImplType = detail::ConnectorSplitter_Impl;

    friend class Model;

    friend class openstudio::IdfObject;

    explicit ConnectorSplitter(std::shared_ptr<detail::ConnectorSplitter_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ConnectorSplitter");
  };

  /** \relates ConnectorSplitter */
  using OptionalConnectorSplitter = boost::optional<ConnectorSplitter>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONNECTORSPLITTER_HPP
