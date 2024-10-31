/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKCOMPONENT_HPP
#define MODEL_AIRFLOWNETWORKCOMPONENT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class AirflowNetworkComponent_Impl;
  }

  /** AirflowNetworkComponent is the base class for AirflowNetwork component objects that determine linkage flows in the pressure network.
 *
 */
  class MODEL_API AirflowNetworkComponent : public ModelObject
  {

   public:
    AirflowNetworkComponent(IddObjectType type, const Model& model);

    virtual ~AirflowNetworkComponent() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkComponent(const AirflowNetworkComponent& other) = default;
    AirflowNetworkComponent(AirflowNetworkComponent&& other) = default;
    AirflowNetworkComponent& operator=(const AirflowNetworkComponent&) = default;
    AirflowNetworkComponent& operator=(AirflowNetworkComponent&&) = default;

    //std::vector<openstudio::IdfObject> remove();

    //bool removeFromLoop();

    //** Returns the inlet port. **/
    //unsigned inletPort() const;

    //** Returns the outlet port. **/
    //unsigned outletPort() const;

    //** Returns the optional ModelObject that this component represents. **/
    boost::optional<ModelObject> componentModelObject() const;

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

    using ImplType = detail::AirflowNetworkComponent_Impl;

    explicit AirflowNetworkComponent(std::shared_ptr<detail::AirflowNetworkComponent_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkComponent");

    /// @endcond
  };

  using OptionalAirflowNetworkComponent = boost::optional<AirflowNetworkComponent>;

  using AirflowNetworkComponentVector = std::vector<AirflowNetworkComponent>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKCOMPONENT_HPP
