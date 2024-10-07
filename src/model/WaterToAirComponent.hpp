/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WATERTOAIRCOMPONENT_HPP
#define MODEL_WATERTOAIRCOMPONENT_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {
    class WaterToAirComponent_Impl;
  }

  class AirLoopHVAC;

  class PlantLoop;

  /** WaterToAirComponent is the base class for model objects which interact with
 *  an air stream and a water stream.
 *
 *  These objects are typically water coils such as the EnergyPlus IDD objects
 *  named Coil:Heating:Water and Coil:Cooling:Water.
 */
  class MODEL_API WaterToAirComponent : public HVACComponent
  {
   public:
    virtual ~WaterToAirComponent() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    WaterToAirComponent(const WaterToAirComponent& other) = default;
    WaterToAirComponent(WaterToAirComponent&& other) = default;
    WaterToAirComponent& operator=(const WaterToAirComponent&) = default;
    WaterToAirComponent& operator=(WaterToAirComponent&&) = default;

    /** Returns the air inlet port. **/
    virtual unsigned airInletPort() const;

    /** Returns the air outlet port. **/
    virtual unsigned airOutletPort() const;

    /** Returns the optional ModelObject connected to the air inlet port. **/
    virtual boost::optional<ModelObject> airInletModelObject() const;

    /** Returns the optional ModelObject connected to the air outlet port. **/
    virtual boost::optional<ModelObject> airOutletModelObject() const;

    /** Returns the water inlet port. **/
    virtual unsigned waterInletPort() const;

    /** Returns the water outlet port. **/
    virtual unsigned waterOutletPort() const;

    /** Returns the optional ModelObject connected to the water inlet port. **/
    virtual boost::optional<ModelObject> waterInletModelObject() const;

    /** Returns the optional ModelObject connected to the water outlet port. **/
    virtual boost::optional<ModelObject> waterOutletModelObject() const;

    virtual boost::optional<AirLoopHVAC> airLoopHVAC() const;

    virtual boost::optional<PlantLoop> plantLoop() const;

    virtual bool addToNode(Node& node);

    virtual bool addToSplitter(Splitter& splitter);

    virtual std::vector<openstudio::IdfObject> remove();

    virtual ModelObject clone(Model model) const;

    void disconnectWaterSide();

    void disconnectAirSide();

    /** Removes the component from the airLoopHVAC if one is attached.
   *  Repairs the airLoopHVAC connections preserving the integrity of the loop.
   *  Does not remove the component from the model
   */
    bool removeFromAirLoopHVAC();

    /** Removes the component from the plantLoop if one is attached.
   *  Repairs the plantLoop connections preserving the integrity of the loop.
   *  Does not remove the component from the model
   */
    bool removeFromPlantLoop();

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::WaterToAirComponent_Impl;

    explicit WaterToAirComponent(std::shared_ptr<detail::WaterToAirComponent_Impl> impl);

    WaterToAirComponent(IddObjectType type, const Model& model);

   private:
    REGISTER_LOGGER("openstudio.model.WaterToAirComponent");

    /// @endcond
  };

  using OptionalWaterToAirComponent = boost::optional<WaterToAirComponent>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_WATERTOAIRCOMPONENT_HPP
