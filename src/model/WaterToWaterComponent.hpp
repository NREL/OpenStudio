/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WATERTOWATERCOMPONENT_HPP
#define MODEL_WATERTOWATERCOMPONENT_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

  class AirLoopHVAC;

  namespace detail {
    class WaterToWaterComponent_Impl;
  }

  /** WaterToWaterComponent is the base class for HVACComponent objects which interact with two water loops.  A WaterToWaterComponent
 *  has two inlet nodes and two outlet ports.  One pair of inlet/outlet ports is designated as the supply side and the other pair
 *  is designated as the demand side.
 */
  class MODEL_API WaterToWaterComponent : public HVACComponent
  {
   public:
    WaterToWaterComponent(IddObjectType type, const Model& model);

    virtual ~WaterToWaterComponent() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    WaterToWaterComponent(const WaterToWaterComponent& other) = default;
    WaterToWaterComponent(WaterToWaterComponent&& other) = default;
    WaterToWaterComponent& operator=(const WaterToWaterComponent&) = default;
    WaterToWaterComponent& operator=(WaterToWaterComponent&&) = default;

    /** Returns the supply inlet port. **/
    virtual unsigned supplyInletPort() const;

    /** Returns the supply outlet port. **/
    virtual unsigned supplyOutletPort() const;

    /** Returns the demand inlet port. **/
    virtual unsigned demandInletPort() const;

    /** Returns the demand outlet port. **/
    virtual unsigned demandOutletPort() const;

    /** Returns the optional ModelObject connected to the supply inlet port. **/
    virtual boost::optional<ModelObject> supplyInletModelObject() const;

    /** Returns the optional ModelObject connected to the supply outlet port. **/
    virtual boost::optional<ModelObject> supplyOutletModelObject() const;

    /** Returns the optional ModelObject connected to the demand inlet port. **/
    virtual boost::optional<ModelObject> demandInletModelObject() const;

    /** Returns the optional ModelObject connected to the demand outlet port. **/
    virtual boost::optional<ModelObject> demandOutletModelObject() const;

    virtual bool addToNode(Node& node);

    virtual std::vector<openstudio::IdfObject> remove();

    virtual ModelObject clone(Model model) const;

    /** Returns the optional PlantLoop object that the HVAC component is a supply component on.
   */
    boost::optional<PlantLoop> plantLoop() const;  // cppcheck-suppress [duplInheritedMember] for documentation purposes

    /** Returns the optional PlantLoop object that the HVAC component is a demand component on.
   */
    boost::optional<PlantLoop> secondaryPlantLoop() const;  // cppcheck-suppress [duplInheritedMember] for documentation purposes

    /** Removes the component from the plantLoop if one is attached.
   *  Repairs the plantLoop connections preserving the integrity of the loop.
   */
    bool removeFromPlantLoop();

    /** Removes the component from the secondaryPlantLoop if one is attached.
   *  Repairs the secondaryPlantLoop connections preserving the integrity of the loop.
   */
    bool removeFromSecondaryPlantLoop();

    /** Tertiary plant loop applies to a limited number of WaterToWaterComponent types that
   *  interact with three plant systems.  One example is a water cooled chiller with heat recovery,
   *  which can be connected to a chilled water, condenser, and heat recovery plant simultaneously.
   */
    boost::optional<PlantLoop> tertiaryPlantLoop() const;

    /** Removes the component from the tertiaryPlantLoop if one is attached.
   *  Repairs the tertiaryPlantLoop connections preserving the integrity of the loop.
   */
    bool removeFromTertiaryPlantLoop();

    /** Add this WaterToWaterComponent to a tertiaryPlantLoop by making connections to the
   *  tertiary inlet and outlet.
   */
    virtual bool addToTertiaryNode(Node& node);

    /** Returns the optional ModelObject connected to the tertiary inlet. **/
    boost::optional<ModelObject> tertiaryInletModelObject() const;

    /** Returns the optional ModelObject connected to the tertiary outlet. **/
    boost::optional<ModelObject> tertiaryOutletModelObject() const;

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::WaterToWaterComponent_Impl;

    explicit WaterToWaterComponent(std::shared_ptr<detail::WaterToWaterComponent_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.WaterToWaterComponent");

    /// @endcond
  };

  using OptionalWaterToWaterComponent = boost::optional<WaterToWaterComponent>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_WATERTOWATERCOMPONENT_HPP
