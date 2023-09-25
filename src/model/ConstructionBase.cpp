/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"

#include "Model.hpp"
#include "StandardsInformationConstruction.hpp"
#include "StandardsInformationConstruction_Impl.hpp"
#include "PlanarSurface.hpp"
#include "PlanarSurface_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "InteriorPartitionSurface.hpp"
#include "InteriorPartitionSurface_Impl.hpp"
#include "InteriorPartitionSurfaceGroup.hpp"
#include "InteriorPartitionSurfaceGroup_Impl.hpp"
#include "RenderingColor.hpp"
#include "RenderingColor_Impl.hpp"
#include "Space.hpp"

#include "../utilities/core/Assert.hpp"

using std::vector;

namespace openstudio {
namespace model {

  namespace detail {

    ConstructionBase_Impl::ConstructionBase_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {}

    ConstructionBase_Impl::ConstructionBase_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    ConstructionBase_Impl::ConstructionBase_Impl(const ConstructionBase_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    StandardsInformationConstruction ConstructionBase_Impl::standardsInformation() const {
      StandardsInformationConstructionVector candidates = getObject<ConstructionBase>().getModelObjectSources<StandardsInformationConstruction>();
      if (candidates.size() > 1) {
        for (unsigned i = 1, n = candidates.size(); i < n; ++i) {
          candidates[i].remove();
        }
        LOG(Warn, "Removed extraneous ConstructionBaseStandardsInformation objects pointing to " << briefDescription() << ".");
      }
      if (candidates.size() == 1) {
        return candidates[0];
      }
      return StandardsInformationConstruction(getObject<ConstructionBase>());
    }

    // not in the public api
    double getNetAreaHelper(const PlanarSurface& planarSurface) {
      boost::optional<Space> space = planarSurface.space();

      double multiplier = 1.0;
      if (space) {
        multiplier = space->multiplier();
      }

      if (planarSurface.optionalCast<SubSurface>()) {
        multiplier = multiplier * planarSurface.cast<SubSurface>().multiplier();
      }

      if (planarSurface.optionalCast<InteriorPartitionSurface>()) {
        boost::optional<InteriorPartitionSurfaceGroup> interiorPartitionSurfaceGroup =
          planarSurface.cast<InteriorPartitionSurface>().interiorPartitionSurfaceGroup();
        if (interiorPartitionSurfaceGroup) {
          multiplier = multiplier * interiorPartitionSurfaceGroup->multiplier();
        }
      }

      return multiplier * planarSurface.netArea();
    }

    double ConstructionBase_Impl::getNetArea() const {
      Handle handle = this->handle();
      std::set<Handle> adjacentPlanarSurfacesToSkip;

      double result = 0.0;
      for (const PlanarSurface& planarSurface : model().getModelObjects<PlanarSurface>()) {

        // if we have already processed this surface as an adjacent surface
        if (std::find(adjacentPlanarSurfacesToSkip.begin(), adjacentPlanarSurfacesToSkip.end(), planarSurface.handle())
            != adjacentPlanarSurfacesToSkip.end()) {
          continue;
        }

        // PlanarSurface::construction now attempts to resolve surface matching mismatches
        boost::optional<ConstructionBase> constructionBase = planarSurface.construction();
        if (!constructionBase) {
          continue;
        }

        if (constructionBase->handle() == handle) {
          double netArea = getNetAreaHelper(planarSurface);

          // avoid double counting construction for adjacent surface
          if (planarSurface.optionalCast<Surface>()) {
            boost::optional<Surface> adjacentSurface = planarSurface.cast<Surface>().adjacentSurface();
            if (adjacentSurface) {
              boost::optional<ConstructionBase> adjacentConstructionBase = adjacentSurface->construction();
              if (adjacentConstructionBase && (constructionBase->handle() == adjacentConstructionBase->handle())) {
                adjacentPlanarSurfacesToSkip.insert(adjacentSurface->handle());
                netArea = std::max(netArea, getNetAreaHelper(*adjacentSurface));
              }
            }
          }

          if (planarSurface.optionalCast<SubSurface>()) {
            boost::optional<SubSurface> adjacentSubSurface = planarSurface.cast<SubSurface>().adjacentSubSurface();
            if (adjacentSubSurface) {
              boost::optional<ConstructionBase> adjacentConstructionBase = adjacentSubSurface->construction();
              if (adjacentConstructionBase && (constructionBase->handle() == adjacentConstructionBase->handle())) {
                adjacentPlanarSurfacesToSkip.insert(adjacentSubSurface->handle());
                netArea = std::max(netArea, getNetAreaHelper(*adjacentSubSurface));
              }
            }
          }

          result += netArea;
        }
      }
      return result;
    }

    std::vector<ModelObject> ConstructionBase_Impl::children() const {

      vector<ModelObject> results(castVector<ModelObject>(getObject<ConstructionBase>().getModelObjectSources<StandardsInformationConstruction>()));

      return results;
    }

    std::vector<IddObjectType> ConstructionBase_Impl::allowableChildTypes() const {
      IddObjectTypeVector result;
      result.push_back(StandardsInformationConstruction::iddObjectType());
      return result;
    }

    bool ConstructionBase_Impl::setUFactor(double /*value*/) {
      return false;
    }

    bool ConstructionBase_Impl::setUFactor(double /*value*/, double /*filmResistance*/) {
      return false;
    }

    bool ConstructionBase_Impl::setConductance(double /*value*/) {
      return false;
    }

    bool ConstructionBase_Impl::setConductance(double /*value*/, double /*filmResistance*/) {
      return false;
    }

    bool ConstructionBase_Impl::isGreenRoof() const {
      return false;
    }

    boost::optional<double> ConstructionBase_Impl::uFactor() const {
      return boost::none;
    }

    boost::optional<double> ConstructionBase_Impl::uFactor(double /*filmResistance*/) const {
      return boost::none;
    }

    boost::optional<double> ConstructionBase_Impl::thermalConductance() const {
      return boost::none;
    }

    boost::optional<double> ConstructionBase_Impl::thermalConductance(double /*filmResistance*/) const {
      return boost::none;
    }

    boost::optional<double> ConstructionBase_Impl::heatCapacity() const {
      return boost::none;
    }

    boost::optional<double> ConstructionBase_Impl::interiorVisibleAbsorptance() const {
      return boost::none;
    }

    boost::optional<double> ConstructionBase_Impl::exteriorVisibleAbsorptance() const {
      return boost::none;
    }

    boost::optional<double> ConstructionBase_Impl::visibleTransmittance() const {
      return boost::none;
    }

    boost::optional<RenderingColor> ConstructionBase_Impl::renderingColor() const {
      return getObject<ModelObject>().getModelObjectTarget<RenderingColor>(this->renderingColorIndex());
    }

    bool ConstructionBase_Impl::setRenderingColor(const RenderingColor& renderingColor) {
      return setPointer(this->renderingColorIndex(), renderingColor.handle());
    }

    void ConstructionBase_Impl::resetRenderingColor() {
      bool test = setString(this->renderingColorIndex(), "");
      OS_ASSERT(test);
    }

  }  // namespace detail

  StandardsInformationConstruction ConstructionBase::standardsInformation() const {
    return getImpl<detail::ConstructionBase_Impl>()->standardsInformation();
  }

  double ConstructionBase::getNetArea() const {
    return getImpl<detail::ConstructionBase_Impl>()->getNetArea();
  }
  bool ConstructionBase::setUFactor(double value) {
    return getImpl<detail::ConstructionBase_Impl>()->setUFactor(value);
  }

  bool ConstructionBase::setUFactor(double value, double filmResistance) {
    return getImpl<detail::ConstructionBase_Impl>()->setUFactor(value, filmResistance);
  }

  bool ConstructionBase::setConductance(double value) {
    return getImpl<detail::ConstructionBase_Impl>()->setConductance(value);
  }

  bool ConstructionBase::setConductance(double value, double filmResistance) {
    return getImpl<detail::ConstructionBase_Impl>()->setConductance(value, filmResistance);
  }

  bool ConstructionBase::isOpaque() const {
    return getImpl<detail::ConstructionBase_Impl>()->isOpaque();
  }

  bool ConstructionBase::isFenestration() const {
    return getImpl<detail::ConstructionBase_Impl>()->isFenestration();
  }

  bool ConstructionBase::isSolarDiffusing() const {
    return getImpl<detail::ConstructionBase_Impl>()->isSolarDiffusing();
  }

  bool ConstructionBase::isModelPartition() const {
    return getImpl<detail::ConstructionBase_Impl>()->isModelPartition();
  }

  bool ConstructionBase::isGreenRoof() const {
    return getImpl<detail::ConstructionBase_Impl>()->isGreenRoof();
  }

  boost::optional<double> ConstructionBase::uFactor() const {
    return getImpl<detail::ConstructionBase_Impl>()->uFactor();
  }

  boost::optional<double> ConstructionBase::uFactor(double filmResistance) const {
    return getImpl<detail::ConstructionBase_Impl>()->uFactor(filmResistance);
  }

  boost::optional<double> ConstructionBase::thermalConductance() const {
    return getImpl<detail::ConstructionBase_Impl>()->thermalConductance();
  }

  boost::optional<double> ConstructionBase::thermalConductance(double filmResistance) const {
    return getImpl<detail::ConstructionBase_Impl>()->thermalConductance(filmResistance);
  }

  boost::optional<double> ConstructionBase::heatCapacity() const {
    return getImpl<detail::ConstructionBase_Impl>()->heatCapacity();
  }

  boost::optional<double> ConstructionBase::interiorVisibleAbsorptance() const {
    return getImpl<detail::ConstructionBase_Impl>()->interiorVisibleAbsorptance();
  }

  boost::optional<double> ConstructionBase::exteriorVisibleAbsorptance() const {
    return getImpl<detail::ConstructionBase_Impl>()->exteriorVisibleAbsorptance();
  }

  boost::optional<double> ConstructionBase::visibleTransmittance() const {
    return getImpl<detail::ConstructionBase_Impl>()->visibleTransmittance();
  }

  boost::optional<RenderingColor> ConstructionBase::renderingColor() const {
    return getImpl<detail::ConstructionBase_Impl>()->renderingColor();
  }

  bool ConstructionBase::setRenderingColor(const RenderingColor& renderingColor) {
    return getImpl<detail::ConstructionBase_Impl>()->setRenderingColor(renderingColor);
  }

  void ConstructionBase::resetRenderingColor() {
    getImpl<detail::ConstructionBase_Impl>()->resetRenderingColor();
  }

  /// @cond
  ConstructionBase::ConstructionBase(IddObjectType type, const Model& model) : ResourceObject(type, model) {
    OS_ASSERT(getImpl<detail::ConstructionBase_Impl>());
  }

  ConstructionBase::ConstructionBase(std::shared_ptr<detail::ConstructionBase_Impl> impl) : ResourceObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
