/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    Splitter_Impl::Splitter_Impl(IddObjectType type, Model_Impl* model) : HVACComponent_Impl(type, model) {}

    Splitter_Impl::Splitter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : HVACComponent_Impl(idfObject, model, keepHandle) {}

    Splitter_Impl::Splitter_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {}

    Splitter_Impl::Splitter_Impl(const Splitter_Impl& other, Model_Impl* model, bool keepHandles) : HVACComponent_Impl(other, model, keepHandles) {}

    boost::optional<ModelObject> Splitter_Impl::inletModelObject() const {
      return connectedObject(inletPort());
    }

    boost::optional<ModelObject> Splitter_Impl::outletModelObject(unsigned branchIndex) const {
      return connectedObject(outletPort(branchIndex));
    }

    std::vector<HVACComponent> Splitter_Impl::edges(const boost::optional<HVACComponent>& /*prev*/) {
      return castVector<HVACComponent>(outletModelObjects());
    }

    boost::optional<ModelObject> Splitter_Impl::lastOutletModelObject() const {
      std::vector<ModelObject> objects = outletModelObjects();
      if (!objects.empty()) {
        return OptionalModelObject(objects.back());
      } else {
        return {};
      }
    }

    unsigned Splitter_Impl::newOutletPortAfterBranch(unsigned branchIndex) {
      //std::vector<ModelObject> modelObjects = outletModelObjects();
      int stop = nextBranchIndex() - 1;
      for (int i = branchIndex; i < stop; i++) {
        ModelObject mo = outletModelObject(i).get();
        unsigned port = connectedObjectPort(outletPort(i)).get();
        model().connect(getObject<ModelObject>(), outletPort(i + 2), mo, port);
      }

      Model _model = model();
      Node node(_model);

      _model.connect(getObject<ModelObject>(), outletPort(branchIndex + 1), node, node.inletPort());

      return outletPort(branchIndex++);
    }

    unsigned Splitter_Impl::branchIndexForOutletModelObject(ModelObject modelObject) const {
      int stop = nextBranchIndex();
      for (int i = 0; i < stop; i++) {
        if (outletModelObject(i) == modelObject) {
          return i;
        }
      }
      return 0;
    }

    unsigned Splitter_Impl::nextBranchIndex() const {
      unsigned i = 0;
      OptionalModelObject modelObject;
      modelObject = connectedObject(this->outletPort(i));
      while (modelObject) {
        i++;
        modelObject = connectedObject(this->outletPort(i));
      }
      return i;
    }

    void Splitter_Impl::removePortForBranch(unsigned branchIndex) {
      int _nextBranchIndex = nextBranchIndex();
      model().disconnect(getObject<ModelObject>(), outletPort(branchIndex));
      //std::vector<ModelObject> modelObjects = outletModelObjects();
      for (int i = branchIndex + 1; i < _nextBranchIndex; i++) {
        ModelObject mo = outletModelObject(i).get();
        unsigned port = connectedObjectPort(outletPort(i)).get();
        model().disconnect(getObject<ModelObject>(), outletPort(i));
        model().connect(getObject<ModelObject>(), outletPort(i - 1), mo, port);
      }
    }

    std::vector<ModelObject> Splitter_Impl::outletModelObjects() const {
      std::vector<ModelObject> result;
      int stop = nextBranchIndex();
      for (int i = 0; i < stop; i++) {
        result.push_back(this->outletModelObject(i).get());
      }
      return result;
    }

    bool Splitter_Impl::isRemovable() const {
      if (airLoopHVAC() || plantLoop()) {
        return false;
      } else {
        return true;
      }
    }

    void Splitter_Impl::disconnect() {
      auto mo = this->getObject<ModelObject>();
      auto _model = model();

      _model.disconnect(mo, inletPort());
      auto end = nextBranchIndex();
      for (unsigned i = 0; i != end; ++i) {
        _model.disconnect(mo, outletPort(i));
      }
    }

    //bool Splitter_Impl::addToNode(Node & node)
    //{
    //  return false;
    //}

    //ModelObject Splitter_Impl::clone(Model model) const
    //{
    //  return HVACComponent_Impl::clone( model );
    //}

  }  // namespace detail

  Splitter::Splitter(std::shared_ptr<detail::Splitter_Impl> p) : HVACComponent(std::move(p)) {}

  Splitter::Splitter(IddObjectType type, const Model& model) : HVACComponent(type, model) {
    OS_ASSERT(getImpl<detail::Splitter_Impl>());
  }

  unsigned Splitter::inletPort() const {
    return getImpl<detail::Splitter_Impl>()->inletPort();
  }

  unsigned Splitter::outletPort(unsigned branchIndex) const {
    return getImpl<detail::Splitter_Impl>()->outletPort(branchIndex);
  }

  unsigned Splitter::nextOutletPort() const {
    return getImpl<detail::Splitter_Impl>()->nextOutletPort();
  }

  boost::optional<ModelObject> Splitter::inletModelObject() const {
    return getImpl<detail::Splitter_Impl>()->inletModelObject();
  }

  boost::optional<ModelObject> Splitter::outletModelObject(unsigned branchIndex) const {
    return getImpl<detail::Splitter_Impl>()->outletModelObject(branchIndex);
  }

  boost::optional<ModelObject> Splitter::lastOutletModelObject() const {
    return getImpl<detail::Splitter_Impl>()->lastOutletModelObject();
  }

  std::vector<ModelObject> Splitter::outletModelObjects() const {
    return getImpl<detail::Splitter_Impl>()->outletModelObjects();
  }

  unsigned Splitter::newOutletPortAfterBranch(unsigned branchIndex) {
    return getImpl<detail::Splitter_Impl>()->newOutletPortAfterBranch(branchIndex);
  }

  unsigned Splitter::branchIndexForOutletModelObject(ModelObject modelObject) const {
    return getImpl<detail::Splitter_Impl>()->branchIndexForOutletModelObject(modelObject);
  }

  unsigned Splitter::nextBranchIndex() const {
    return getImpl<detail::Splitter_Impl>()->nextBranchIndex();
  }

  void Splitter::removePortForBranch(unsigned branchIndex) {
    return getImpl<detail::Splitter_Impl>()->removePortForBranch(branchIndex);
  }

}  // namespace model

}  // namespace openstudio
