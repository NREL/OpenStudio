/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTOAIRCOMPONENT_HPP
#define MODEL_AIRTOAIRCOMPONENT_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class AirToAirComponent_Impl;
  }

  class AirLoopHVAC;

  /** AirToAirComponent is the base class for model objects which interact with
 *  two air streams.
 *
 *  These objects are typically heat recovery devices such as the EnergyPlus IDD object
 *  named HeatExchanger:AirToAir:SensibleAndLatent.
 */
  class MODEL_API AirToAirComponent : public HVACComponent
  {
   public:
    AirToAirComponent(IddObjectType type, const Model& model);

    virtual ~AirToAirComponent() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirToAirComponent(const AirToAirComponent& other) = default;
    AirToAirComponent(AirToAirComponent&& other) = default;
    AirToAirComponent& operator=(const AirToAirComponent&) = default;
    AirToAirComponent& operator=(AirToAirComponent&&) = default;

    /** Returns the primary air stream inlet port.
   *
   *  In the conext of air to air heat recovery devices in EnergyPlus,
   *  the primary air stream is often thought of as the supply side air stream
   *  which is the supply of outdoor air into the system.
   */
    unsigned primaryAirInletPort() const;

    /** Returns the primary air stream outlet port. **/
    unsigned primaryAirOutletPort() const;

    /** Returns the optional ModelObject connected to the primary air stream inlet port. **/
    boost::optional<ModelObject> primaryAirInletModelObject() const;

    /** Returns the optional ModelObject connected to the primary air stream outlet port. **/
    boost::optional<ModelObject> primaryAirOutletModelObject() const;

    /** Returns the secondary air stream inlet port.
   *
   *  In the context of air to air heat recovery devices in EnergyPlus,
   *  the secondary air stream is often the exhaust air stream leaving the system.
   */
    unsigned secondaryAirInletPort() const;

    /** Returns the secondary air stream outlet port. **/
    unsigned secondaryAirOutletPort() const;

    /** Returns the optional ModelObject connected to the secondary air stream inlet port. **/
    boost::optional<ModelObject> secondaryAirInletModelObject() const;

    /** Returns the optional ModelObject connected to the secondary air stream outlet port. **/
    boost::optional<ModelObject> secondaryAirOutletModelObject() const;

    bool addToNode(Node& node);

    std::vector<openstudio::IdfObject> remove();

    ModelObject clone(Model model) const;

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::AirToAirComponent_Impl;

    explicit AirToAirComponent(std::shared_ptr<detail::AirToAirComponent_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.AirToAirComponent");

    /// @endcond
  };

  using OptionalAirToAirComponent = boost::optional<AirToAirComponent>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTOAIRCOMPONENT_HPP
