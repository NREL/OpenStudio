/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKNODE_HPP
#define MODEL_AIRFLOWNETWORKNODE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class AirflowNetworkNode_Impl;
  }

  /** AirflowNetworkNode is the base class for AirflowNetwork objects that are nodes in the pressure network.
 *
 */
  class MODEL_API AirflowNetworkNode : public ModelObject
  {

   public:
    AirflowNetworkNode(IddObjectType type, const Model& model);

    virtual ~AirflowNetworkNode() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkNode(const AirflowNetworkNode& other) = default;
    AirflowNetworkNode(AirflowNetworkNode&& other) = default;
    AirflowNetworkNode& operator=(const AirflowNetworkNode&) = default;
    AirflowNetworkNode& operator=(AirflowNetworkNode&&) = default;

    //std::vector<openstudio::IdfObject> remove();

    //bool removeFromLoop();

    //** Returns the inlet port. **/
    //unsigned inletPort() const;

    //** Returns the outlet port. **/
    //unsigned outletPort() const;

    //** Returns the optional ModelObject connected to the inlet port. **/
    //boost::optional<ModelObject> inletModelObject() const;

    //** Returns the optional ModelObject connected to the outlet port. **/
    //boost::optional<ModelObject> outletModelObject() const;

    //** Returns the optional AirLoopHVAC object that this AirToAirComponent is attached to.
    // *
    // *  Reimplemented from HVACComponent.
    // */
    //boost::optional<AirLoopHVAC> airLoopHVAC() const;

    //bool addToNode(Node & node);

    //ModelObject clone(Model model) const;

    //void disconnect();

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::AirflowNetworkNode_Impl;

    explicit AirflowNetworkNode(std::shared_ptr<detail::AirflowNetworkNode_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkNode");

    /// @endcond
  };

  using OptionalAirflowNetworkNode = boost::optional<AirflowNetworkNode>;

  using AirflowNetworkNodeVector = std::vector<AirflowNetworkNode>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKNODE_HPP
