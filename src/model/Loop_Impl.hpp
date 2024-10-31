/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LOOP_IMPL_HPP
#define MODEL_LOOP_IMPL_HPP

#include "ParentObject_Impl.hpp"

namespace openstudio {

class AppGFuelType;
class ComponentType;
class FuelType;

namespace model {

  class Node;
  class HVACComponent;
  class Splitter;
  class Mixer;
  class AvailabilityManagerAssignmentList;

  namespace detail {

    class Model_Impl;

    class MODEL_API Loop_Impl : public ParentObject_Impl
    {

     public:
      Loop_Impl(IddObjectType type, Model_Impl* model);

      Loop_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Loop_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Loop_Impl(const Loop_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~Loop_Impl() override = default;

      /** This pure virtual method is intended to be overriden by child classes (namely PlantLoop and AirLoopHVAC) to create the basic topology of the
     * loop, that is to create the supply/demand inlet/outlet nodes, splitters and mixers as appropriate */
      virtual void createTopology() = 0;

      virtual Node supplyInletNode() const = 0;

      virtual Node supplyOutletNode() const = 0;

      virtual std::vector<Node> supplyOutletNodes() const = 0;

      virtual Node demandInletNode() const = 0;

      virtual std::vector<Node> demandInletNodes() const = 0;

      virtual Node demandOutletNode() const = 0;

      virtual std::vector<ModelObject> children() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual std::vector<ModelObject> supplyComponents(const HVACComponent& inletComp, const HVACComponent& outletComp,
                                                        openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

      virtual std::vector<ModelObject> demandComponents(const HVACComponent& inletComp, const HVACComponent& outletComp,
                                                        openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

      virtual std::vector<ModelObject> supplyComponents(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

      virtual std::vector<ModelObject> demandComponents(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

      virtual std::vector<ModelObject> components(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

      std::vector<ModelObject> components(const HVACComponent& inletComp, const HVACComponent& outletComp,
                                          openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

      virtual boost::optional<ModelObject> component(openstudio::Handle handle) const;

      virtual boost::optional<ModelObject> demandComponent(openstudio::Handle handle) const;

      virtual boost::optional<ModelObject> supplyComponent(openstudio::Handle handle) const;

      // virtual std::vector<ModelObject> supplyComponents(std::vector<HVACComponent> inletComps, std::vector<HVACComponent> outletComps,
      //                                                   openstudio::IddObjectType type) const;

      // virtual std::vector<ModelObject> demandComponents(std::vector<HVACComponent> inletComps, std::vector<HVACComponent> outletComps,
      //                                                   openstudio::IddObjectType type) const;

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual ModelObject clone(Model model) const override;

      virtual Splitter demandSplitter() const = 0;

      virtual Mixer demandMixer() const = 0;

      virtual void autosize();

      virtual void applySizingValues();

      virtual AvailabilityManagerAssignmentList availabilityManagerAssignmentList() const = 0;

      virtual ComponentType componentType() const;
      virtual std::vector<FuelType> coolingFuelTypes() const;
      virtual std::vector<FuelType> heatingFuelTypes() const;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const;

     private:
      REGISTER_LOGGER("openstudio.model.Loop");

      boost::optional<ModelObject> supplyInletNodeAsModelObject() const;
      boost::optional<ModelObject> supplyOutletNodeAsModelObject() const;
      boost::optional<ModelObject> demandInletNodeAsModelObject() const;
      boost::optional<ModelObject> demandOutletNodeAsModelObject() const;
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif
