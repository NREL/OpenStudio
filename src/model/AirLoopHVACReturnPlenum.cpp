/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirLoopHVACReturnPlenum.hpp"
#include "AirLoopHVACReturnPlenum_Impl.hpp"
#include "AirLoopHVACZoneMixer.hpp"
#include "AirLoopHVACZoneMixer_Impl.hpp"
#include "AirLoopHVACSupplyPlenum.hpp"
#include "AirLoopHVACSupplyPlenum_Impl.hpp"
#include "AirLoopHVACZoneSplitter.hpp"
#include "AirLoopHVACZoneSplitter_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "ZoneHVACIdealLoadsAirSystem.hpp"
#include "ZoneHVACIdealLoadsAirSystem_Impl.hpp"

#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/OS_AirLoopHVAC_ReturnPlenum_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    AirLoopHVACReturnPlenum_Impl::AirLoopHVACReturnPlenum_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Mixer_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACReturnPlenum::iddObjectType());
    }

    AirLoopHVACReturnPlenum_Impl::AirLoopHVACReturnPlenum_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                               bool keepHandle)
      : Mixer_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirLoopHVACReturnPlenum::iddObjectType());
    }

    AirLoopHVACReturnPlenum_Impl::AirLoopHVACReturnPlenum_Impl(const AirLoopHVACReturnPlenum_Impl& other, Model_Impl* model, bool keepHandle)
      : Mixer_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirLoopHVACReturnPlenum_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirLoopHVACReturnPlenum_Impl::iddObjectType() const {
      return AirLoopHVACReturnPlenum::iddObjectType();
    }

    boost::optional<ThermalZone> AirLoopHVACReturnPlenum_Impl::thermalZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_AirLoopHVAC_ReturnPlenumFields::ThermalZone);
    }

    bool AirLoopHVACReturnPlenum_Impl::setThermalZone(const boost::optional<ThermalZone>& thermalZone) {
      bool result(false);

      if (!thermalZone) {
        resetThermalZone();
        result = true;
      } else if ((!thermalZone->getImpl<ThermalZone_Impl>()->airLoopHVACSupplyPlenum())
                 && (!thermalZone->getImpl<ThermalZone_Impl>()->airLoopHVACReturnPlenum()) && (thermalZone->equipment().empty())
                 && (!thermalZone->useIdealAirLoads())) {
        result = setPointer(OS_AirLoopHVAC_ReturnPlenumFields::ThermalZone, thermalZone.get().handle());
      }

      return result;
    }

    void AirLoopHVACReturnPlenum_Impl::resetThermalZone() {
      bool result = setString(OS_AirLoopHVAC_ReturnPlenumFields::ThermalZone, "");
      OS_ASSERT(result);
    }

    unsigned AirLoopHVACReturnPlenum_Impl::outletPort() const {
      return OS_AirLoopHVAC_ReturnPlenumFields::OutletNode;
    }

    PortList AirLoopHVACReturnPlenum_Impl::inducedAirOutletPortList() {
      auto plenum = getObject<model::AirLoopHVACReturnPlenum>();
      boost::optional<PortList> portList = plenum.getModelObjectTarget<PortList>(OS_AirLoopHVAC_ReturnPlenumFields::InducedAirOutletPortList);
      if (!portList) {
        portList = PortList(plenum);
        setPointer(OS_AirLoopHVAC_ReturnPlenumFields::InducedAirOutletPortList, portList->handle());
      }
      return portList.get();
    }

    unsigned AirLoopHVACReturnPlenum_Impl::inletPort(unsigned branchIndex) const {
      unsigned result;
      result = numNonextensibleFields();
      result = result + branchIndex;
      return result;
    }

    unsigned AirLoopHVACReturnPlenum_Impl::nextInletPort() const {
      return inletPort(this->nextBranchIndex());
    }

    bool AirLoopHVACReturnPlenum_Impl::addToNode(Node& node) {
      bool result = true;

      Model _model = model();
      // Is the node in this model
      if (node.model() != _model) {
        return false;
      }

      auto thisObject = getObject<AirLoopHVACReturnPlenum>();

      auto outletObj = node.outletModelObject();
      auto inletObj = node.inletModelObject();

      boost::optional<ZoneHVACComponent> zoneHVAC;
      if (outletObj) {
        zoneHVAC = outletObj->optionalCast<ZoneHVACComponent>();
      }

      // Plenum can be attached to ZoneHVAC OR AirLoopHVAC
      if (zoneHVAC) {
        // don't let a plenum connect to ZoneHVAC and AirLoopHVAC at the same time
        if (airLoopHVAC()) {
          result = false;
        }

        boost::optional<ThermalZone> zone;

        if (inletObj) {
          auto pl = inletObj->optionalCast<PortList>();
          if (pl) {
            zone = pl->thermalZone();
          }
        }

        if (!zone) {
          result = false;
        }

        if (result) {
          _model.connect(node, node.outletPort(), thisObject, thisObject.nextInletPort());

          Node zoneHVACInletNode(_model);
          _model.connect(zoneHVACInletNode, zoneHVACInletNode.outletPort(), zoneHVAC.get(), zoneHVAC->inletPort());
          auto pl = inducedAirOutletPortList();
          _model.connect(pl, pl.nextPort(), zoneHVACInletNode, zoneHVACInletNode.inletPort());
        }
      } else {
        // Is the node part of an air loop
        auto nodeAirLoop = node.airLoopHVAC();

        if (!nodeAirLoop) {
          result = false;
        }

        // Is this plenum already connected to a different air loop
        auto currentAirLoopHVAC = airLoopHVAC();
        if (currentAirLoopHVAC && (currentAirLoopHVAC.get() != nodeAirLoop)) {
          result = false;
        }

        boost::optional<AirLoopHVACZoneMixer> mixer;
        // Is the immediate downstream object to the node a mixer
        if (result) {
          mixer = nodeAirLoop->zoneMixer();

          if (!(outletObj && mixer && (outletObj.get() == mixer.get()))) {
            result = false;
          }
        }

        // Make sure there is not already a return plenum
        if (result) {
          if (inletObj && inletObj->optionalCast<AirLoopHVACReturnPlenum>()) {
            result = false;
          }
        }

        // Is there a zone on this branch
        if (result) {
          Splitter splitter = nodeAirLoop->zoneSplitter();
          if (nodeAirLoop->demandComponents(splitter, node, ThermalZone::iddObjectType()).empty()) {
            result = false;
          }
        }

        if (result) {
          unsigned inletObjectPort;
          unsigned outletObjectPort;
          boost::optional<ModelObject> inletModelObject;
          boost::optional<ModelObject> outletModelObject;

          inletModelObject = node;
          inletObjectPort = node.outletPort();
          outletModelObject = outletObj;
          outletObjectPort = node.connectedObjectPort(node.outletPort()).get();

          if (currentAirLoopHVAC) {
            mixer->removePortForBranch(mixer->branchIndexForInletModelObject(inletModelObject.get()));
            _model.connect(inletModelObject.get(), inletObjectPort, thisObject, thisObject.nextInletPort());
          } else {
            Node plenumOutletNode(_model);
            plenumOutletNode.createName();

            _model.connect(inletModelObject.get(), inletObjectPort, thisObject, thisObject.nextInletPort());
            _model.connect(thisObject, thisObject.outletPort(), plenumOutletNode, plenumOutletNode.inletPort());
            _model.connect(plenumOutletNode, plenumOutletNode.outletPort(), outletModelObject.get(), outletObjectPort);
          }
        }
      }

      return result;
    }

    bool AirLoopHVACReturnPlenum_Impl::addBranchForZone(openstudio::model::ThermalZone& thermalZone) {
      boost::optional<HVACComponent> t_terminal;

      return addBranchForZoneImpl(thermalZone, t_terminal);
    }

    bool AirLoopHVACReturnPlenum_Impl::addBranchForZone(openstudio::model::ThermalZone& thermalZone, HVACComponent& terminal) {
      boost::optional<HVACComponent> t_terminal = terminal;

      return addBranchForZoneImpl(thermalZone, t_terminal);
    }

    bool AirLoopHVACReturnPlenum_Impl::addBranchForZoneImpl(openstudio::model::ThermalZone& thermalZone, boost::optional<HVACComponent>& terminal) {
      boost::optional<Splitter> splitter;
      boost::optional<Mixer> mixer = getObject<AirLoopHVACReturnPlenum>();

      boost::optional<AirLoopHVAC> t_airLoopHVAC = airLoopHVAC();

      if (!t_airLoopHVAC) {
        return false;
      }

      std::vector<ModelObject> supplyPlenums;
      supplyPlenums = t_airLoopHVAC->demandComponents(t_airLoopHVAC->demandInletNode(), mixer.get(), AirLoopHVACSupplyPlenum::iddObjectType());

      if (supplyPlenums.size() == 1u) {
        splitter = supplyPlenums.front().cast<Splitter>();
      } else {
        splitter = t_airLoopHVAC->zoneSplitter();
      }

      OS_ASSERT(splitter);
      OS_ASSERT(mixer);

      return AirLoopHVAC_Impl::addBranchForZoneImpl(thermalZone, t_airLoopHVAC.get(), splitter.get(), mixer.get(), true, terminal);
    }

    std::vector<IdfObject> AirLoopHVACReturnPlenum_Impl::remove() {
      Model t_model = model();

      if (boost::optional<AirLoopHVAC> t_airLoopHVAC = airLoopHVAC()) {
        AirLoopHVACZoneMixer zoneMixer = t_airLoopHVAC->zoneMixer();
        std::vector<ModelObject> t_inletModelObjects = inletModelObjects();

        for (auto it = t_inletModelObjects.rbegin(); it != t_inletModelObjects.rend(); ++it) {
          unsigned branchIndex = branchIndexForInletModelObject(*it);
          unsigned t_inletPort = inletPort(branchIndex);
          unsigned connectedObjectOutletPort = connectedObjectPort(t_inletPort).get();

          t_model.connect(*it, connectedObjectOutletPort, zoneMixer, zoneMixer.nextInletPort());
        }

        boost::optional<ModelObject> mo = outletModelObject();
        OS_ASSERT(mo);
        boost::optional<Node> node = mo->optionalCast<Node>();
        OS_ASSERT(node);
        zoneMixer.removePortForBranch(zoneMixer.branchIndexForInletModelObject(node.get()));
        node->remove();
      } else {
        auto outlet = outletModelObject();
        auto h = handle();

        auto zoneHVAC = t_model.getModelObjects<ZoneHVACComponent>();
        for (auto& hvac : zoneHVAC) {
          auto plenum = hvac.returnPlenum();
          if (plenum && (plenum->handle() == h)) {
            hvac.removeReturnPlenum();
          }
        }

        if (outlet) {
          outlet->remove();
        }
      }

      return Mixer_Impl::remove();
    }

    // If every zone on the plenum has exactly one piece of equipment
    // and that equipment is a ZoneHVACIdealLoadsAirSystem
    // then return a list of all them,
    // This is used privatly in the translator to work around an EnergyPlus bug
    std::vector<model::ZoneHVACIdealLoadsAirSystem> AirLoopHVACReturnPlenum_Impl::zoneHVACIdealLoadsAirSystems() const {
      std::vector<model::ZoneHVACIdealLoadsAirSystem> result;
      bool doworkaround = true;
      auto inlets = inletModelObjects();
      for (const auto& inlet : inlets) {
        auto node = inlet.optionalCast<Node>();
        if (node) {
          auto nodeinlet = node->inletModelObject();
          if (nodeinlet) {
            auto pl = nodeinlet->optionalCast<PortList>();
            if (pl) {
              auto zone = pl->thermalZone();
              auto equipment = zone.equipment();
              if (equipment.size() < 2) {
                if (equipment.size() == 1) {
                  auto ideal = equipment.front().optionalCast<ZoneHVACIdealLoadsAirSystem>();
                  if (ideal) {
                    result.push_back(ideal.get());
                  } else {
                    doworkaround = false;
                    break;
                  }
                }
              } else {
                doworkaround = false;
                break;
              }
            }
          }
        }
      }

      if (!doworkaround) {
        result.clear();
      }

      return result;
    }

    ComponentType AirLoopHVACReturnPlenum_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> AirLoopHVACReturnPlenum_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> AirLoopHVACReturnPlenum_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> AirLoopHVACReturnPlenum_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  AirLoopHVACReturnPlenum::AirLoopHVACReturnPlenum(const Model& model) : Mixer(AirLoopHVACReturnPlenum::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirLoopHVACReturnPlenum_Impl>());
  }

  IddObjectType AirLoopHVACReturnPlenum::iddObjectType() {
    return {IddObjectType::OS_AirLoopHVAC_ReturnPlenum};
  }

  boost::optional<ThermalZone> AirLoopHVACReturnPlenum::thermalZone() const {
    return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->thermalZone();
  }

  bool AirLoopHVACReturnPlenum::setThermalZone(const ThermalZone& thermalZone) {
    return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->setThermalZone(thermalZone);
  }

  void AirLoopHVACReturnPlenum::resetThermalZone() {
    getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->resetThermalZone();
  }

  unsigned AirLoopHVACReturnPlenum::outletPort() const {
    return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->outletPort();
  }

  unsigned AirLoopHVACReturnPlenum::inletPort(unsigned branchIndex) const {
    return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->inletPort(branchIndex);
  }

  unsigned AirLoopHVACReturnPlenum::nextInletPort() const {
    return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->nextInletPort();
  }

  bool AirLoopHVACReturnPlenum::addToNode(Node& node) {
    return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->addToNode(node);
  }

  bool AirLoopHVACReturnPlenum::addBranchForZone(openstudio::model::ThermalZone& thermalZone) {
    return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->addBranchForZone(thermalZone);
  }

  bool AirLoopHVACReturnPlenum::addBranchForZone(openstudio::model::ThermalZone& thermalZone, HVACComponent& terminal) {
    return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->addBranchForZone(thermalZone, terminal);
  }

  /// @cond
  AirLoopHVACReturnPlenum::AirLoopHVACReturnPlenum(std::shared_ptr<detail::AirLoopHVACReturnPlenum_Impl> impl) : Mixer(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
