/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Loop.hpp"
#include "Loop_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "StraightComponent.hpp"
#include "StraightComponent_Impl.hpp"
#include "WaterToAirComponent.hpp"
#include "WaterToAirComponent_Impl.hpp"
#include "WaterToWaterComponent.hpp"
#include "WaterToWaterComponent_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "ConnectorSplitter.hpp"
#include "ConnectorSplitter_Impl.hpp"
#include "Model.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/ContainersMove.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <algorithm>
#include <functional>

namespace openstudio {

namespace model {

  namespace detail {
    Loop_Impl::Loop_Impl(IddObjectType type, Model_Impl* model) : ParentObject_Impl(type, model) {}

    Loop_Impl::Loop_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : ParentObject_Impl(idfObject, model, keepHandle) {}

    Loop_Impl::Loop_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {}

    Loop_Impl::Loop_Impl(const Loop_Impl& other, Model_Impl* model, bool keepHandles) : ParentObject_Impl(other, model, keepHandles) {}

    const std::vector<std::string>& Loop_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    std::vector<openstudio::IdfObject> Loop_Impl::remove() {
      //ModelObjectVector modelObjects;
      //ModelObjectVector::iterator it;
      //modelObjects = this->supplyComponents();
      //for(it = modelObjects.begin();
      //    it != modelObjects.end();
      //    it++)
      //{
      //  if( it->optionalCast<Node>() ) continue;
      //  it->remove();
      //}
      //modelObjects = this->demandComponents();
      //for(it = modelObjects.begin();
      //    it != modelObjects.end();
      //    it++)
      //{
      //  it->remove();
      //}
      return ParentObject_Impl::remove();
    }

    // Recursive depth first search
    // start algorithm with one source node in the visited vector
    // searches all paths of nodes between the source and sink until a handle match is found
    boost::optional<ModelObject> findModelObject(const openstudio::Handle& handle, const HVACComponent& sink, std::vector<HVACComponent>& visited,
                                                 bool isDemandComponents) {
      HVACComponent hvacComponent = visited.back();
      if (handle == hvacComponent.handle()) {
        return hvacComponent;
      }

      boost::optional<HVACComponent> prev;
      if (visited.size() >= 2u) {
        prev = visited.rbegin()[1];
      }

      std::vector<HVACComponent> nodes = hvacComponent.getImpl<HVACComponent_Impl>()->edges(prev);

      for (auto& node : nodes) {
        // if it node has already been visited or node is sink then continue
        if (std::find(visited.begin(), visited.end(), node) != visited.end() || node == sink) {
          continue;
        }
        visited.push_back(node);
        boost::optional<ModelObject> foundHandle = findModelObject(handle, sink, visited, isDemandComponents);
        if (foundHandle) {
          return *foundHandle;
        }
        visited.pop_back();
      }
      return boost::none;
    }

    OptionalModelObject Loop_Impl::component(openstudio::Handle handle) const {
      boost::optional<ModelObject> supplyComp = this->supplyComponent(handle);
      if (supplyComp) {
        return supplyComp;
      }
      return this->demandComponent(handle);
    }

    boost::optional<ModelObject> Loop_Impl::demandComponent(openstudio::Handle handle) const {
      auto comps = demandComponents();

      auto it = std::find_if(comps.begin(), comps.end(), [&](const ModelObject& comp) { return (comp.handle() == handle); });
      if (it != comps.end()) {
        return *it;
      }

      return boost::none;
    }

    boost::optional<ModelObject> Loop_Impl::supplyComponent(openstudio::Handle handle) const {
      auto comps = supplyComponents();

      auto it = std::find_if(comps.begin(), comps.end(), [&](const ModelObject& comp) { return (comp.handle() == handle); });
      if (it != comps.end()) {
        return *it;
      }

      return boost::none;
    }

    ModelObject Loop_Impl::clone(Model model) const {
      return ParentObject_Impl::clone(model);
    }

    std::vector<ModelObject> Loop_Impl::children() const {
      return {};
    }

    // std::vector<ModelObject> Loop_Impl::supplyComponents(std::vector<HVACComponent> /*inletComps*/, std::vector<HVACComponent> /*outletComps*/,
    //                                                      openstudio::IddObjectType /*type*/) const {
    //   LOG(Warn, "supplyComponents method for " << briefDescription() << " is not implemented to take multiple inlet and outlet components");
    //   return {};
    // }

    // std::vector<ModelObject> Loop_Impl::demandComponents(std::vector<HVACComponent> /*inletComps*/, std::vector<HVACComponent> /*outletComps*/,
    //                                                      openstudio::IddObjectType /*type*/) const {
    //   return {};
    // }

    boost::optional<ModelObject> Loop_Impl::supplyInletNodeAsModelObject() const {
      OptionalModelObject result = supplyInletNode();
      return result;
    }

    boost::optional<ModelObject> Loop_Impl::supplyOutletNodeAsModelObject() const {
      OptionalModelObject result = supplyOutletNode();
      return result;
    }

    boost::optional<ModelObject> Loop_Impl::demandInletNodeAsModelObject() const {
      OptionalModelObject result = demandInletNode();
      return result;
    }

    boost::optional<ModelObject> Loop_Impl::demandOutletNodeAsModelObject() const {
      OptionalModelObject result = demandOutletNode();
      return result;
    }

    // Recursive depth first search
    // start algorithm with one source node in the visited vector
    // when complete, paths will be populated with all nodes between the source node and sink
    void findModelObjects(const HVACComponent& sink, std::vector<HVACComponent>& visited, std::vector<HVACComponent>& paths,
                          bool isDemandComponents) {
      boost::optional<HVACComponent> prev;
      if (visited.size() >= 2u) {
        prev = visited.rbegin()[1];
      }

      std::vector<HVACComponent> nodes = visited.back().getImpl<HVACComponent_Impl>()->edges(prev);

      for (const auto& node : nodes) {
        // if it node has already been visited then continue
        if (std::find(visited.begin(), visited.end(), node) != visited.end()) {
          continue;
        }
        if (node == sink) {
          visited.push_back(node);
          // Avoid pushing duplicate nodes into paths
          if (paths.empty()) {
            paths.insert(paths.end(), visited.begin(), visited.end());
          } else {
            for (const auto& visitedit : visited) {
              if (std::find(paths.begin(), paths.end(), visitedit) == paths.end()) {
                paths.push_back(visitedit);
              }
            }
          }
          visited.pop_back();
        }
      }

      for (const auto& node : nodes) {
        // if it node has already been visited or node is sink then continue
        if (std::find(visited.begin(), visited.end(), node) != visited.end() || node == sink) {
          continue;
        }
        visited.push_back(node);
        findModelObjects(sink, visited, paths, isDemandComponents);
        visited.pop_back();
      }
    }

    std::vector<ModelObject> Loop_Impl::demandComponents(const HVACComponent& inletComp, const HVACComponent& outletComp,
                                                         openstudio::IddObjectType type) const {
      std::vector<HVACComponent> visited;
      visited.push_back(inletComp);
      std::vector<HVACComponent> allPaths;

      if (inletComp == outletComp) {
        allPaths.push_back(inletComp);
      } else {
        findModelObjects(outletComp, visited, allPaths, true);
      }
      std::vector<ModelObject> modelObjects = std::vector<ModelObject>(allPaths.begin(), allPaths.end());

      // Filter modelObjects for type
      if (type != IddObjectType::Catchall) {
        modelObjects.erase(
          std::remove_if(modelObjects.begin(), modelObjects.end(), [&](const auto& mo) -> bool { return mo.iddObjectType() != type; }),
          modelObjects.end());
      }
      return modelObjects;
    }

    template <typename T>
    struct Duplicate
    {
      bool operator()(const T& element) {
        return !s_.insert(element).second;  // true if element already in set
      }

     private:
      std::set<T> s_;
    };

    std::vector<ModelObject> Loop_Impl::supplyComponents(openstudio::IddObjectType type) const {
      std::vector<ModelObject> result;

      auto t_supplyInletNode = supplyInletNode();
      auto t_supplyOutletNodes = supplyOutletNodes();

      for (auto const& t_supplyOutletNode : t_supplyOutletNodes) {
        auto components = supplyComponents(t_supplyInletNode, t_supplyOutletNode, type);
        result.insert(result.end(), components.begin(), components.end());
      }

      // If there is more than one outlet node (dual duct) we might have duplicates
      if (t_supplyOutletNodes.size() > 1u) {
        Duplicate<ModelObject> pred;
        auto it = std::remove_if(result.begin(), result.end(), std::ref(pred));
        return {result.begin(), it};
      } else {
        return result;
      }
    }

    std::vector<ModelObject> Loop_Impl::demandComponents(openstudio::IddObjectType type) const {
      std::vector<ModelObject> result;

      auto t_demandOutletNode = demandOutletNode();
      auto t_demandInletNodes = demandInletNodes();

      for (auto const& t_demandInletNode : t_demandInletNodes) {
        auto components = demandComponents(t_demandInletNode, t_demandOutletNode, type);
        result.insert(result.end(), components.begin(), components.end());
      }

      // If there is more than one inlet node (dual duct) we might have duplicates
      if (t_demandInletNodes.size() > 1u) {
        Duplicate<ModelObject> pred;
        auto it = std::remove_if(result.begin(), result.end(), std::ref(pred));
        return {result.begin(), it};
      } else {
        return result;
      }
    }

    std::vector<ModelObject> Loop_Impl::components(openstudio::IddObjectType type) const {
      return openstudio::concat<ModelObject>(this->supplyComponents(type), this->demandComponents(type));
    }

    std::vector<ModelObject> Loop_Impl::supplyComponents(const HVACComponent& inletComp, const HVACComponent& outletComp,
                                                         openstudio::IddObjectType type) const {
      std::vector<HVACComponent> visited;
      visited.push_back(inletComp);
      std::vector<HVACComponent> allPaths;

      if (inletComp == outletComp) {
        allPaths.push_back(inletComp);
      } else {
        findModelObjects(outletComp, visited, allPaths, false);
      }
      std::vector<ModelObject> modelObjects = std::vector<ModelObject>(allPaths.begin(), allPaths.end());

      // Filter modelObjects for type
      if (type != IddObjectType::Catchall) {
        modelObjects.erase(
          std::remove_if(modelObjects.begin(), modelObjects.end(), [&](const auto& mo) -> bool { return mo.iddObjectType() != type; }),
          modelObjects.end());
      }
      return modelObjects;
    }

    std::vector<ModelObject> Loop_Impl::components(const HVACComponent& inletComp, const HVACComponent& outletComp,
                                                   openstudio::IddObjectType type) const {
      if ((supplyComponent(inletComp.handle()) && supplyComponent(outletComp.handle()))) {
        return supplyComponents(inletComp, outletComp, type);
      } else if ((demandComponent(inletComp.handle()) && demandComponent(outletComp.handle()))) {
        return demandComponents(inletComp, outletComp, type);
      }

      return {};
    }

    // default implementation does nothing.
    // should only be used by objects that have
    // no autosized fields
    void Loop_Impl::autosize() {}

    // default implementation does nothing.
    // should only be used by objects that have
    // no autosized fields
    void Loop_Impl::applySizingValues() {}

    ComponentType Loop_Impl::componentType() const {
      bool has_cooling = false;
      bool has_heating = false;
      for (const auto& comp : subsetCastVector<HVACComponent>(supplyComponents())) {
        auto compType = comp.componentType();
        if (compType == ComponentType::Cooling) {
          has_cooling = true;
        } else if (compType == ComponentType::Heating) {
          has_heating = true;
        } else if (compType == ComponentType::Both) {
          has_cooling = true;
          has_heating = true;
        }
      }

      if (has_cooling && has_heating) {
        return ComponentType::Both;
      } else if (has_cooling) {
        return ComponentType::Cooling;
      } else if (has_heating) {
        return ComponentType::Heating;
      }
      return ComponentType::None;
    }

    std::vector<FuelType> Loop_Impl::coolingFuelTypes() const {
      std::set<FuelType> result;
      for (const auto& comp : subsetCastVector<HVACComponent>(supplyComponents())) {
        for (auto& ft : comp.coolingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

    std::vector<FuelType> Loop_Impl::heatingFuelTypes() const {
      std::set<FuelType> result;
      for (const auto& comp : subsetCastVector<HVACComponent>(supplyComponents())) {
        for (auto& ft : comp.heatingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

    std::vector<AppGFuelType> Loop_Impl::appGHeatingFuelTypes() const {
      std::set<AppGFuelType> result;
      for (const auto& comp : subsetCastVector<HVACComponent>(supplyComponents())) {
        for (auto& ft : comp.appGHeatingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

  }  // namespace detail

  Loop::Loop(IddObjectType type, const Model& model) : ParentObject(type, model) {
    OS_ASSERT(getImpl<detail::Loop_Impl>());
  }

  Loop::Loop(std::shared_ptr<detail::Loop_Impl> impl) : ParentObject(std::move(impl)) {}

  std::vector<IdfObject> Loop::remove() {
    return getImpl<detail::Loop_Impl>()->remove();
  }

  std::vector<ModelObject> Loop::components(openstudio::IddObjectType type) const {
    return getImpl<detail::Loop_Impl>()->components(type);
  }

  std::vector<ModelObject> Loop::components(const HVACComponent& inletComp, const HVACComponent& outletComp, openstudio::IddObjectType type) {
    return getImpl<detail::Loop_Impl>()->components(inletComp, outletComp, type);
  }

  boost::optional<ModelObject> Loop::component(openstudio::Handle handle) const {
    return getImpl<detail::Loop_Impl>()->component(handle);
  }

  // std::vector<ModelObject> Loop::supplyComponents(std::vector<HVACComponent> inletComps, std::vector<HVACComponent> outletComps,
  //                                                 openstudio::IddObjectType type) const {
  //   return getImpl<detail::Loop_Impl>()->supplyComponents(inletComps, outletComps, type);
  // }

  // std::vector<ModelObject> Loop::demandComponents(std::vector<HVACComponent> inletComps, std::vector<HVACComponent> outletComps,
  //                                                 openstudio::IddObjectType type) const {
  //   return getImpl<detail::Loop_Impl>()->demandComponents(inletComps, outletComps, type);
  // }

  std::vector<ModelObject> Loop::supplyComponents(const HVACComponent& inletComp, const HVACComponent& outletComp,
                                                  openstudio::IddObjectType type) const {
    return getImpl<detail::Loop_Impl>()->supplyComponents(inletComp, outletComp, type);
  }

  std::vector<ModelObject> Loop::supplyComponents(openstudio::IddObjectType type) const {
    return getImpl<detail::Loop_Impl>()->supplyComponents(type);
  }

  std::vector<ModelObject> Loop::demandComponents(const HVACComponent& inletComp, const HVACComponent& outletComp,
                                                  openstudio::IddObjectType type) const {
    return getImpl<detail::Loop_Impl>()->demandComponents(inletComp, outletComp, type);
  }

  std::vector<ModelObject> Loop::demandComponents(openstudio::IddObjectType type) const {
    return getImpl<detail::Loop_Impl>()->demandComponents(type);
  }

  ModelObject Loop::clone(Model model) const {
    return getImpl<detail::Loop_Impl>()->clone(model);
  }

  std::vector<ModelObject> Loop::children() const {
    return getImpl<detail::Loop_Impl>()->children();
  }

  boost::optional<ModelObject> Loop::demandComponent(openstudio::Handle handle) const {
    return getImpl<detail::Loop_Impl>()->demandComponent(handle);
  }

  boost::optional<ModelObject> Loop::supplyComponent(openstudio::Handle handle) const {
    return getImpl<detail::Loop_Impl>()->supplyComponent(handle);
  }

  Node Loop::supplyInletNode() const {
    return getImpl<detail::Loop_Impl>()->supplyInletNode();
  }

  Node Loop::supplyOutletNode() const {
    return getImpl<detail::Loop_Impl>()->supplyOutletNode();
  }

  std::vector<Node> Loop::supplyOutletNodes() const {
    return getImpl<detail::Loop_Impl>()->supplyOutletNodes();
  }

  Node Loop::demandInletNode() const {
    return getImpl<detail::Loop_Impl>()->demandInletNode();
  }

  std::vector<Node> Loop::demandInletNodes() const {
    return getImpl<detail::Loop_Impl>()->demandInletNodes();
  }

  Node Loop::demandOutletNode() const {
    return getImpl<detail::Loop_Impl>()->demandOutletNode();
  }

  Splitter Loop::demandSplitter() const {
    return getImpl<detail::Loop_Impl>()->demandSplitter();
  }

  Mixer Loop::demandMixer() const {
    return getImpl<detail::Loop_Impl>()->demandMixer();
  }

  void Loop::autosize() {
    return getImpl<detail::Loop_Impl>()->autosize();
  }

  void Loop::applySizingValues() {
    return getImpl<detail::Loop_Impl>()->applySizingValues();
  }

  ComponentType Loop::componentType() const {
    return getImpl<detail::Loop_Impl>()->componentType();
  }

  std::vector<FuelType> Loop::coolingFuelTypes() const {
    return getImpl<detail::Loop_Impl>()->coolingFuelTypes();
  }

  std::vector<FuelType> Loop::heatingFuelTypes() const {
    return getImpl<detail::Loop_Impl>()->heatingFuelTypes();
  }

  std::vector<AppGFuelType> Loop::appGHeatingFuelTypes() const {
    return getImpl<detail::Loop_Impl>()->appGHeatingFuelTypes();
  }

}  // namespace model

}  // namespace openstudio
