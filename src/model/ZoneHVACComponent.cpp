/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "AirTerminalSingleDuctInletSideMixer.hpp"
#include "AirTerminalSingleDuctInletSideMixer_Impl.hpp"
#include "AirLoopHVACReturnPlenum.hpp"
#include "AirLoopHVACReturnPlenum_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ZoneHVACComponent_Impl::ZoneHVACComponent_Impl(IddObjectType type, Model_Impl* model) : HVACComponent_Impl(type, model) {}

    ZoneHVACComponent_Impl::ZoneHVACComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(idfObject, model, keepHandle) {}

    ZoneHVACComponent_Impl::ZoneHVACComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {}

    ZoneHVACComponent_Impl::ZoneHVACComponent_Impl(const ZoneHVACComponent_Impl& other, Model_Impl* model, bool keepHandles)
      : HVACComponent_Impl(other, model, keepHandles) {}

    std::vector<ModelObject> ZoneHVACComponent_Impl::children() const {
      return {};
    }

    boost::optional<ParentObject> ZoneHVACComponent_Impl::parent() const {
      return {};
    }

    const std::vector<std::string>& ZoneHVACComponent_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    ModelObject ZoneHVACComponent_Impl::clone(Model model) const {
      auto clone = ModelObject_Impl::clone(model).cast<ZoneHVACComponent>();
      if (clone.inletPort() != 0u) {
        clone.setString(clone.inletPort(), "");
      }
      if (clone.outletPort() != 0u) {
        clone.setString(clone.outletPort(), "");
      }

      return std::move(clone);
    }

    boost::optional<ThermalZone> ZoneHVACComponent_Impl::thermalZone() const {
      auto thisObject = this->getObject<ModelObject>();
      for (const auto& thermalZone : model().getConcreteModelObjects<ThermalZone>()) {
        std::vector<ModelObject> equipment = thermalZone.equipment();

        if (std::find(equipment.begin(), equipment.end(), thisObject) != equipment.end()) {
          return thermalZone;
        }
      }
      return boost::none;
    }

    //boost::optional<Connection> ZoneHVACComponent_Impl::returnPlenumOutletNodeConnection() const {
    //  auto m = model();
    //  auto h = handle();

    //  auto connections = subsetCastVector<Connection>(m.getConcreteModelObjects<Connection>());
    //  auto plenums = subsetCastVector<AirLoopHVACReturnPlenum>(m.getConcreteModelObjects<AirLoopHVACReturnPlenum>());
    //  for ( auto & c : connections ) {
    //    auto target = c.targetObject();
    //    if ( target && ( target->handle() == h ) ) {
    //      auto source = c.sourceObject();
    //      if ( source ) {
    //        auto sourceHandle = source->handle();
    //        for ( auto & p : plenums ) {
    //          auto outlet = p.outletModelObject();
    //          if ( outlet && ( outlet->handle() == sourceHandle ) ) {
    //            return c;
    //          }
    //        }
    //      }
    //    }
    //  }

    //  return boost::none;
    //}

    boost::optional<AirLoopHVACReturnPlenum> ZoneHVACComponent_Impl::returnPlenum() const {
      boost::optional<AirLoopHVACReturnPlenum> plenum;

      auto node = inletNode();
      if (node) {
        auto mo = node->inletModelObject();
        if (mo) {
          auto pl = mo->optionalCast<PortList>();
          if (pl) {
            auto mo2 = pl->getImpl<detail::PortList_Impl>()->hvacComponent();
            plenum = mo2.optionalCast<AirLoopHVACReturnPlenum>();
          }
        }
      }

      return plenum;
    }

    void ZoneHVACComponent_Impl::removeReturnPlenum() {
      Model m = model();
      auto plenum = returnPlenum();
      auto thisObject = getObject<ZoneHVACComponent>();

      if (plenum) {
        auto zone = thermalZone();
        OS_ASSERT(zone);
        auto h = zone->handle();

        auto inlet = inletNode();
        boost::optional<Node> plenumInletNode;

        auto plenumInlets = plenum->inletModelObjects();
        for (auto& plenumInlet : plenumInlets) {
          auto node = plenumInlet.optionalCast<Node>();
          if (node) {
            auto pl = node->inletModelObject();
            if (pl && pl->optionalCast<PortList>()) {
              auto mo = pl->cast<PortList>().getImpl<model::detail::PortList_Impl>()->hvacComponent();
              if (mo.handle() == h) {
                plenumInletNode = node;
                break;
              }
            }
          }
        }

        if (inlet && plenumInletNode) {
          inlet->remove();
          auto branch = plenum->branchIndexForInletModelObject(plenumInletNode.get());
          plenum->removePortForBranch(branch);
          m.connect(plenumInletNode.get(), plenumInletNode->outletPort(), thisObject, thisObject.inletPort());
          plenumInlets = plenum->inletModelObjects();
          if (plenumInlets.empty()) {
            plenum->remove();
          }
        }
      }
    }

    bool ZoneHVACComponent_Impl::setReturnPlenum(const ThermalZone& plenumZone) {

      bool result = true;

      if (!plenumZone.canBePlenum()) {
        result = false;
      }

      auto node = inletNode();
      if (!node) {
        result = false;
      }

      auto m = model();

      boost::optional<AirLoopHVACReturnPlenum> plenum;

      if (result) {
        plenum = plenumZone.getImpl<ThermalZone_Impl>()->airLoopHVACReturnPlenum();

        if (!plenum) {
          plenum = AirLoopHVACReturnPlenum(m);
          plenum->setThermalZone(plenumZone);
        }

        OS_ASSERT(plenum);
        OS_ASSERT(node);
      }

      // don't let a plenum connect to ZoneHVAC and AirLoopHVAC at the same time
      if (plenum->airLoopHVAC()) {
        result = false;
      }

      if (result) {
        removeReturnPlenum();
        result = plenum->addToNode(node.get());
      }

      return result;
    }

    bool ZoneHVACComponent_Impl::addToThermalZone(ThermalZone& thermalZone) {
      Model m = this->model();

      if (thermalZone.model() != m) {
        return false;
      }

      if (thermalZone.isPlenum()) {
        return false;
      }

      removeFromThermalZone();

      thermalZone.setUseIdealAirLoads(false);

      // Connect nodes if this is an air based zone hvac component
      if (inletPort() != 0u && outletPort() != 0u) {
        // Exhaust Node
        Node exhaustNode(m);
        PortList exhaustPortList = thermalZone.exhaustPortList();
        unsigned nextPort = exhaustPortList.nextPort();
        m.connect(exhaustPortList, nextPort, exhaustNode, exhaustNode.inletPort());
        auto mo = this->getObject<ModelObject>();
        m.connect(exhaustNode, exhaustNode.outletPort(), mo, this->inletPort());

        // Air Inlet Node
        Node airInletNode(m);
        PortList inletPortList = thermalZone.inletPortList();
        unsigned nextInletPort = inletPortList.nextPort();
        m.connect(airInletNode, airInletNode.outletPort(), inletPortList, nextInletPort);
        m.connect(mo, this->outletPort(), airInletNode, airInletNode.inletPort());
      }

      thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

      return true;
    }

    void ZoneHVACComponent_Impl::removeFromThermalZone() {
      boost::optional<ThermalZone> thermalZone = this->thermalZone();
      boost::optional<AirLoopHVAC> airLoopHVAC = this->airLoopHVAC();
      Model m = this->model();
      auto mo = getObject<ZoneHVACComponent>();

      bool inletSideMixer = false;
      if (airLoopHVAC) {
        if (airLoopHVAC->demandComponent(mo.handle())) {
          inletSideMixer = true;
        }
      }

      if (inletSideMixer) {
        removeFromInletSideMixer();
      } else if (thermalZone) {

        if (auto t_inletNode = inletNode()) {
          t_inletNode->disconnect();
          t_inletNode->remove();
        }

        if (auto t_outletNode = outletNode()) {
          t_outletNode->disconnect();
          t_outletNode->remove();
        }
      }

      auto thisObject = this->getObject<ModelObject>();
      for (auto& thermalZone : m.getConcreteModelObjects<ThermalZone>()) {
        std::vector<ModelObject> equipment = thermalZone.equipment();

        if (std::find(equipment.begin(), equipment.end(), thisObject) != equipment.end()) {
          thermalZone.removeEquipment(thisObject);
          break;
        }
      }
    }

    boost::optional<Node> ZoneHVACComponent_Impl::inletNode() const {
      boost::optional<ModelObject> mo = connectedObject(inletPort());
      boost::optional<Node> result;

      if (mo) {
        if (boost::optional<Node> node = mo->optionalCast<Node>()) {
          result = node;
        }
      }

      return result;
    }

    boost::optional<Node> ZoneHVACComponent_Impl::outletNode() const {
      boost::optional<ModelObject> mo = connectedObject(outletPort());
      boost::optional<Node> result;

      if (mo) {
        if (boost::optional<Node> node = mo->optionalCast<Node>()) {
          result = node;
        }
      }

      return result;
    }

    std::vector<openstudio::IdfObject> ZoneHVACComponent_Impl::remove() {
      removeFromThermalZone();

      bool result = removeFromAirLoopHVAC();
      OS_ASSERT(result || !airLoopHVAC());

      return HVACComponent_Impl::remove();
    }

    bool ZoneHVACComponent_Impl::addToNode(Node& node) {
      bool result = false;

      boost::optional<ThermalZone> thermalZone;
      boost::optional<AirTerminalSingleDuctInletSideMixer> terminal;

      if (boost::optional<ModelObject> outlet = node.outletModelObject()) {
        if (boost::optional<PortList> pl = outlet->optionalCast<PortList>()) {
          thermalZone = pl->thermalZone();
        }
      }

      if (boost::optional<ModelObject> inlet = node.inletModelObject()) {
        terminal = inlet->optionalCast<AirTerminalSingleDuctInletSideMixer>();
      }

      if (thermalZone && terminal) {
        if (this->thermalZone()) {
          removeFromThermalZone();
        }
        thermalZone->setUseIdealAirLoads(false);
        auto thisObject = getObject<ZoneHVACComponent>();
        thermalZone->addEquipment(thisObject);
        thermalZone->setCoolingPriority(thisObject, 1);
        thermalZone->setHeatingPriority(thisObject, 1);
        Model t_model = model();
        ModelObject thisModelObject = getObject<model::ZoneHVACComponent>();
        unsigned targetPort = node.connectedObjectPort(node.outletPort()).get();
        ModelObject targetModelObject = node.connectedObject(node.outletPort()).get();
        Node newNode(t_model);
        t_model.connect(node, node.outletPort(), thisModelObject, inletPort());
        t_model.connect(thisModelObject, outletPort(), newNode, newNode.inletPort());
        t_model.connect(newNode, newNode.outletPort(), targetModelObject, targetPort);

        Node exhaustNode(t_model);
        PortList exhaustPortList = thermalZone->exhaustPortList();
        unsigned nextPort = exhaustPortList.nextPort();
        t_model.connect(exhaustPortList, nextPort, exhaustNode, exhaustNode.inletPort());
        t_model.connect(exhaustNode, exhaustNode.outletPort(), terminal.get(), terminal->secondaryAirInletPort());

        result = true;
      }

      return result;
    }

    void ZoneHVACComponent_Impl::removeFromInletSideMixer() {
      if (boost::optional<AirLoopHVAC> t_airLoopHVAC = airLoopHVAC()) {
        boost::optional<Node> t_inletNode = inletNode();
        OS_ASSERT(t_inletNode);
        boost::optional<Node> t_outletNode = outletNode();
        OS_ASSERT(t_outletNode);

        if (t_airLoopHVAC->demandComponent(t_inletNode->handle())) {
          unsigned targetPort = t_outletNode->connectedObjectPort(t_outletNode->outletPort()).get();
          ModelObject targetModelObject = t_outletNode->connectedObject(t_outletNode->outletPort()).get();
          t_outletNode->disconnect();
          t_outletNode->remove();

          Model t_model = model();
          t_model.connect(t_inletNode.get(), t_inletNode->outletPort(), targetModelObject, targetPort);

          std::vector<AirTerminalSingleDuctInletSideMixer> terminalMixers = subsetCastVector<AirTerminalSingleDuctInletSideMixer>(
            t_airLoopHVAC->demandComponents(t_airLoopHVAC->demandInletNode(), t_inletNode.get()));
          if (!terminalMixers.empty()) {
            if (boost::optional<Node> secondaryNode = terminalMixers.front().secondaryAirInletNode()) {
              secondaryNode->disconnect();
              secondaryNode->remove();
            }
          }
        }
      }
    }

    boost::optional<AirLoopHVAC> ZoneHVACComponent_Impl::airLoopHVAC() const {
      if (boost::optional<Node> node = inletNode()) {
        return node->airLoopHVAC();
      }

      return boost::none;
    }

    std::vector<HVACComponent> ZoneHVACComponent_Impl::edges(const boost::optional<HVACComponent>& /*prev*/) {
      std::vector<HVACComponent> edges;
      if (boost::optional<Node> t_node = this->outletNode()) {
        edges.push_back(t_node->cast<HVACComponent>());
      }
      return edges;
    }

    bool ZoneHVACComponent_Impl::removeFromAirLoopHVAC() {
      // Note JM 2018-09-22:
      // This calls HVACComponent::airLoopHVACOutdoorAirSystem
      // It returns an AirLoopHVACOutdoorAirSystem that this component is ON
      // Not to be confused with this->airLoopHVAC()->airLoopHVACOutdoorAirSystem() which returns an
      // eventual AirLoopHVACOutdoorAirSystem whether this component is on it or not!
      if (auto t_oaSystem = airLoopHVACOutdoorAirSystem()) {
        if (t_oaSystem->oaComponent(handle())) {
          return HVACComponent_Impl::removeFromLoop(t_oaSystem->outboardOANode().get(), t_oaSystem.get(), inletPort(), outletPort());
        } else if (t_oaSystem->reliefComponent(handle())) {
          return HVACComponent_Impl::removeFromLoop(t_oaSystem.get(), t_oaSystem->outboardReliefNode().get(), inletPort(), outletPort());
        }
      } else if (auto t_loop = loop()) {
        if (t_loop->supplyComponent(handle())) {
          return HVACComponent_Impl::removeFromLoop(t_loop->supplyInletNode(), t_loop->supplyOutletNode(), inletPort(), outletPort());
        }
        /*
       *else if( t_loop->demandComponent(handle()) ) {
       *  return HVACComponent_Impl::removeFromLoop(t_loop->demandInletNode(),
       *         t_loop->demandOutletNode(),
       *         inletPort(),
       *         outletPort());
       *}
       */
      }
      return false;
    }

    boost::optional<ModelObject> ZoneHVACComponent_Impl::airInletModelObject() const {
      auto node = inletNode();
      if (node) {
        return node->cast<ModelObject>();
      }
      return boost::none;
    }

    boost::optional<ModelObject> ZoneHVACComponent_Impl::airOutletModelObject() const {
      auto node = outletNode();
      if (node) {
        return node->cast<ModelObject>();
      }
      return boost::none;
    }

  }  // namespace detail

  ZoneHVACComponent::ZoneHVACComponent(std::shared_ptr<detail::ZoneHVACComponent_Impl> p) : HVACComponent(std::move(p)) {}

  ZoneHVACComponent::ZoneHVACComponent(IddObjectType type, const Model& model) : HVACComponent(type, model) {
    OS_ASSERT(getImpl<detail::ZoneHVACComponent_Impl>());
  }

  unsigned ZoneHVACComponent::inletPort() const {
    return getImpl<detail::ZoneHVACComponent_Impl>()->inletPort();
  }

  unsigned ZoneHVACComponent::outletPort() const {
    return getImpl<detail::ZoneHVACComponent_Impl>()->outletPort();
  }

  boost::optional<ThermalZone> ZoneHVACComponent::thermalZone() const {
    return getImpl<detail::ZoneHVACComponent_Impl>()->thermalZone();
  }

  bool ZoneHVACComponent::addToThermalZone(ThermalZone& thermalZone) {
    return getImpl<detail::ZoneHVACComponent_Impl>()->addToThermalZone(thermalZone);
  }

  void ZoneHVACComponent::removeFromThermalZone() {
    return getImpl<detail::ZoneHVACComponent_Impl>()->removeFromThermalZone();
  }

  boost::optional<Node> ZoneHVACComponent::inletNode() const {
    return getImpl<detail::ZoneHVACComponent_Impl>()->inletNode();
  }

  boost::optional<Node> ZoneHVACComponent::outletNode() const {
    return getImpl<detail::ZoneHVACComponent_Impl>()->outletNode();
  }

  bool ZoneHVACComponent::addToNode(Node& node) {
    return getImpl<detail::ZoneHVACComponent_Impl>()->addToNode(node);
  }

  boost::optional<AirLoopHVAC> ZoneHVACComponent::airLoopHVAC() const {
    return getImpl<detail::ZoneHVACComponent_Impl>()->airLoopHVAC();
  }

  void ZoneHVACComponent::removeReturnPlenum() {
    return getImpl<detail::ZoneHVACComponent_Impl>()->removeReturnPlenum();
  }

  bool ZoneHVACComponent::removeFromAirLoopHVAC() {
    return getImpl<detail::ZoneHVACComponent_Impl>()->removeFromAirLoopHVAC();
  }

  boost::optional<ModelObject> ZoneHVACComponent::airInletModelObject() const {
    return getImpl<detail::ZoneHVACComponent_Impl>()->airInletModelObject();
  }

  boost::optional<ModelObject> ZoneHVACComponent::airOutletModelObject() const {
    return getImpl<detail::ZoneHVACComponent_Impl>()->airOutletModelObject();
  }

  boost::optional<AirLoopHVACReturnPlenum> ZoneHVACComponent::returnPlenum() const {
    return getImpl<detail::ZoneHVACComponent_Impl>()->returnPlenum();
  }

  bool ZoneHVACComponent::setReturnPlenum(const ThermalZone& plenumZone) {
    return getImpl<detail::ZoneHVACComponent_Impl>()->setReturnPlenum(plenumZone);
  }

}  // namespace model

}  // namespace openstudio
