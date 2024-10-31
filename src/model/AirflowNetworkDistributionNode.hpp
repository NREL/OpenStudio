/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKDISTRIBUTIONNODE_HPP
#define MODEL_AIRFLOWNETWORKDISTRIBUTIONNODE_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkNode.hpp"

namespace openstudio {

namespace model {

  class AirLoopHVACZoneMixer;
  class AirLoopHVACZoneSplitter;
  class AirLoopHVACOutdoorAirSystem;
  //OAMixerOutdoorAirStreamNode
  //OutdoorAir : NodeList
  //OutdoorAir : Node
  class Node;

  namespace detail {

    class Node_Impl;
    class AirLoopHVACZoneMixer_Impl;
    class AirLoopHVACZoneSplitter_Impl;
    class AirLoopHVACOutdoorAirSystem_Impl;
    class AirflowNetworkDistributionNode_Impl;

  }  // namespace detail

  /** AirflowNetworkDistributionNode is a AirflowNetworkNode that wraps the OpenStudio IDD object 'OS:AirflowNetworkDistributionNode'. */
  class MODEL_API AirflowNetworkDistributionNode : public AirflowNetworkNode
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirflowNetworkDistributionNode(const Model& model);

    virtual ~AirflowNetworkDistributionNode() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkDistributionNode(const AirflowNetworkDistributionNode& other) = default;
    AirflowNetworkDistributionNode(AirflowNetworkDistributionNode&& other) = default;
    AirflowNetworkDistributionNode& operator=(const AirflowNetworkDistributionNode&) = default;
    AirflowNetworkDistributionNode& operator=(AirflowNetworkDistributionNode&&) = default;

    //@}

    static IddObjectType iddObjectType();

    //static std::vector<std::string> componentObjectTypeorNodeTypeValues();

    /** @name Getters */
    //@{
    boost::optional<Node> node() const;
    boost::optional<AirLoopHVACZoneMixer> airLoopHVACZoneMixer() const;
    boost::optional<AirLoopHVACZoneSplitter> airLoopHVACZoneSplitter() const;
    boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const;
    //OAMixerOutdoorAirStreamNode
    //OutdoorAir : NodeList
    //OutdoorAir : Node

    double nodeHeight() const;

    bool isNodeHeightDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    void setNodeHeight(double nodeHeight);

    void resetNodeHeight();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkDistributionNode_Impl;

    explicit AirflowNetworkDistributionNode(std::shared_ptr<detail::AirflowNetworkDistributionNode_Impl> impl);

    AirflowNetworkDistributionNode(const Model& model, const Handle& handle);

    void resetComponent();

    friend class detail::Node_Impl;
    friend class detail::AirLoopHVACZoneMixer_Impl;
    friend class detail::AirLoopHVACZoneSplitter_Impl;
    friend class detail::AirLoopHVACOutdoorAirSystem_Impl;
    friend class detail::AirflowNetworkDistributionNode_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkDistributionNode");
  };

  /** \relates AirflowNetworkDistributionNode*/
  using OptionalAirflowNetworkDistributionNode = boost::optional<AirflowNetworkDistributionNode>;

  /** \relates AirflowNetworkDistributionNode*/
  using AirflowNetworkDistributionNodeVector = std::vector<AirflowNetworkDistributionNode>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKDISTRIBUTIONNODE_HPP
