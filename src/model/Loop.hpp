/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LOOP_HPP
#define MODEL_LOOP_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

class AppGFuelType;
class ComponentType;
class FuelType;

namespace model {

  namespace detail {
    class Loop_Impl;
  };

  class Node;

  class Splitter;

  class Mixer;

  class HVACComponent;

  /** Loop is the base class for HVAC air and water loops.
 *
 *  This class provides basic functionality to traverse a loop and locate components.
 *  Currently there are two types of loops AirLoopHVAC and PlantLoop.  This interface reflects the
 *  commonality between the two.  Loops in combination with HVACComponents offer a very simple interface to
 *  create new systems in EnergyPlus.
 *
 *  The following is an example of building a new HVAC system composed of an AirLoopHVAC with this c++ interface.
 *
@verbatim

Model model = Model();

AirLoopHVAC airLoop = AirLoopHVAC(model);

AirLoopHVACOutdoorAirSystem oaSystem = AirLoopHVACOutdoorAirSystem(model);

FanConstantVolume fan = FanConstantVolume(model);

CoilCoolingDXSingleSpeed  coolCoil = CoilCoolingDXSingleSpeed(model);

CoilHeatingGas heatingCoil = CoilHeatingGas(model);

oaSystem.addToNode( airLoop.supplyOutletNode() );

fan.addToNode( airLoop.supplyOutletNode() );

coolCoil.addToNode( airLoop.supplyOutletNode() );

heatingCoil.addToNode( airLoop.supplyOutletNode() );

airLoop.addBranchForZone("Zone1");

@endverbatim
 *
 * The interface also allows you to retrieve a ModelObject and clone it into a new model thereby making a new copy.
 * The clone algorithm is intellegent enough to bring along all of the connected pieces as it is appropriate.
 *
@verbatim

Model newModel();
AirLoopHVAC newAirLoop = model.getModelObject(airLoop.handle())->clone(newModel);

@endverbatim
 *
 * The HVACComponent::addToNode function is intellegent. The add routine is context sensitive so for example
 * adding a WaterToAirComponent to an air node will connect the air side ports, adding it to a plant loop node
 * will connect the water side ports.
 *
@verbatim

Model model = Model();

AirLoopHVAC airLoop = AirLoopHVAC(model);

PlantLoop plantLoop = PlantLoop(model);

CoilCoolingWater waterCoolingCoil = CoilCoolingWater(model);

waterCoolingCoil.addToNode( airLoop.supplyOutletNode() );

waterCoolingCoil.addToSplitter( plantLoop.demandSplitter() );

@endverbatim
 */
  class MODEL_API Loop : public ParentObject
  {
   public:
    virtual ~Loop() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Loop(const Loop& other) = default;
    Loop(Loop&& other) = default;
    Loop& operator=(const Loop&) = default;
    Loop& operator=(Loop&&) = default;

    virtual Node supplyInletNode() const;

    virtual Node supplyOutletNode() const;

    virtual std::vector<Node> supplyOutletNodes() const;

    virtual Node demandInletNode() const;

    virtual Node demandOutletNode() const;

    virtual std::vector<Node> demandInletNodes() const;

    /** Returns all of the supply side hvac equipment between
   * inletComps and outletComps.  If type is given then the results will
   * be limited to the given IddObjectType.  Multiple inlet and outlet nodes
   * can be provided.
   */
    // virtual std::vector<ModelObject> supplyComponents(std::vector<HVACComponent> inletComps, std::vector<HVACComponent> outletComps,
    //                                                  openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

    /** Returns all of the supply side hvac equipment between
   * inletComp and outletComp.  If type is given then the results will
   * be limited to the given IddObjectType.  Only one inlet and outlet node
   * can be given.
   */
    virtual std::vector<ModelObject> supplyComponents(const HVACComponent& inletComp, const HVACComponent& outletComp,
                                                      openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

    /** Returns all of the supply side HVAC equipment within the air loop.
   * If type is given then the results will be limited to the given IddObjectType.
   */
    virtual std::vector<ModelObject> supplyComponents(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

    /** Returns all of the demand side hvac equipment between
   * inletComps and outletComps.  If type is given then the results will
   * be limited to the given IddObjectType.  Multiple inlet and outlet nodes
   * can be provided.
   */
    // virtual std::vector<ModelObject> demandComponents(std::vector<HVACComponent> inletComps, std::vector<HVACComponent> outletComps,
    //                                                  openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

    /** Returns all of the demand side hvac equipment between
   * inletComp and outletComp.  If type is given then the results will
   * be limited to the given IddObjectType.  Only one inlet and outlet node
   * can be given.
   */
    virtual std::vector<ModelObject> demandComponents(const HVACComponent& inletComp, const HVACComponent& outletComp,
                                                      openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

    /** Returns all of the demand side HVAC equipment within the air loop.
   * If type is given then the results will be limited to the given IddObjectType.
   */
    virtual std::vector<ModelObject> demandComponents(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

    /** Returns all of the HVAC equipment within the air loop including both
   * the supply and demand sides of the loop.
   * If type is given then the results will be limited to the given IddObjectType.
   */
    virtual std::vector<ModelObject> components(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

    /** Returns supplyComponents() or demandComponents(), depending on if the inlet and
   *  outlet component arugments are on the supply or demand side.
   *  If they are from opposite sides of the loop then an empty vector will be returned.
   */
    std::vector<ModelObject> components(const HVACComponent& inletComp, const HVACComponent& outletComp,
                                        openstudio::IddObjectType type = openstudio::IddObjectType("Catchall"));

    /** Returns an optional ModelObject with the given handle.
   * If the handle is not within the Loop then the optional will be false
   */
    virtual boost::optional<ModelObject> component(openstudio::Handle handle) const;

    /** Returns an optional ModelObject with the given handle.
   * If the handle is not within demand side of the Loop then the optional will be false
   */
    virtual boost::optional<ModelObject> demandComponent(openstudio::Handle handle) const;

    /** Returns an optional ModelObject with the given handle.
   * If the handle is not within supply side of the Loop then the optional will be false
   */
    virtual boost::optional<ModelObject> supplyComponent(openstudio::Handle handle) const;

    virtual std::vector<openstudio::IdfObject> remove();

    Splitter demandSplitter() const;

    Mixer demandMixer() const;

    virtual ModelObject clone(Model model) const;

    virtual std::vector<ModelObject> children() const;

    void autosize();

    void applySizingValues();

    ComponentType componentType() const;
    std::vector<FuelType> coolingFuelTypes() const;
    std::vector<FuelType> heatingFuelTypes() const;
    std::vector<AppGFuelType> appGHeatingFuelTypes() const;

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::Loop_Impl;

    explicit Loop(std::shared_ptr<ImplType> impl);

    Loop(IddObjectType type, const Model& model);

   private:
    REGISTER_LOGGER("openstudio.model.Loop");

    /// @endcond
  };

  using OptionalLoop = boost::optional<Loop>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LOOP_HPP
