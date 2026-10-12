/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "DefaultConstructionSet.hpp"
#include "DefaultConstructionSet_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "PlanarSurface.hpp"
#include "PlanarSurface_Impl.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "DefaultSubSurfaceConstructions.hpp"
#include "DefaultSubSurfaceConstructions_Impl.hpp"
#include "DefaultSurfaceConstructions.hpp"
#include "DefaultSurfaceConstructions_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "InteriorPartitionSurface.hpp"
#include "InteriorPartitionSurface_Impl.hpp"
#include "InteriorPartitionSurfaceGroup.hpp"
#include "InteriorPartitionSurfaceGroup_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"
#include "ShadingSurfaceGroup.hpp"
#include "ShadingSurfaceGroup_Impl.hpp"
#include "Building.hpp"
#include "Building_Impl.hpp"
#include "BuildingStory.hpp"
#include "BuildingStory_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"

#include <utilities/idd/OS_DefaultConstructionSet_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Logger.hpp"

#include <bitset>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

using openstudio::istringEqual;

namespace openstudio {
namespace model {

  namespace detail {

    DefaultConstructionSet_Impl::DefaultConstructionSet_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == DefaultConstructionSet::iddObjectType());
    }

    DefaultConstructionSet_Impl::DefaultConstructionSet_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                             bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == DefaultConstructionSet::iddObjectType());
    }

    DefaultConstructionSet_Impl::DefaultConstructionSet_Impl(const DefaultConstructionSet_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& DefaultConstructionSet_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType DefaultConstructionSet_Impl::iddObjectType() const {
      return DefaultConstructionSet::iddObjectType();
    }

    boost::optional<DefaultSurfaceConstructions> DefaultConstructionSet_Impl::defaultExteriorSurfaceConstructions() const {
      return getObject<ModelObject>().getModelObjectTarget<DefaultSurfaceConstructions>(
        OS_DefaultConstructionSetFields::DefaultExteriorSurfaceConstructionsName);
    }

    boost::optional<DefaultSurfaceConstructions> DefaultConstructionSet_Impl::defaultInteriorSurfaceConstructions() const {
      return getObject<ModelObject>().getModelObjectTarget<DefaultSurfaceConstructions>(
        OS_DefaultConstructionSetFields::DefaultInteriorSurfaceConstructionsName);
    }

    boost::optional<DefaultSurfaceConstructions> DefaultConstructionSet_Impl::defaultGroundContactSurfaceConstructions() const {
      return getObject<ModelObject>().getModelObjectTarget<DefaultSurfaceConstructions>(
        OS_DefaultConstructionSetFields::DefaultGroundContactSurfaceConstructionsName);
    }

    boost::optional<DefaultSubSurfaceConstructions> DefaultConstructionSet_Impl::defaultExteriorSubSurfaceConstructions() const {
      return getObject<ModelObject>().getModelObjectTarget<DefaultSubSurfaceConstructions>(
        OS_DefaultConstructionSetFields::DefaultExteriorSubSurfaceConstructionsName);
    }

    boost::optional<DefaultSubSurfaceConstructions> DefaultConstructionSet_Impl::defaultInteriorSubSurfaceConstructions() const {
      return getObject<ModelObject>().getModelObjectTarget<DefaultSubSurfaceConstructions>(
        OS_DefaultConstructionSetFields::DefaultInteriorSubSurfaceConstructionsName);
    }

    boost::optional<ConstructionBase> DefaultConstructionSet_Impl::interiorPartitionConstruction() const {
      return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DefaultConstructionSetFields::InteriorPartitionConstructionName);
    }

    boost::optional<ConstructionBase> DefaultConstructionSet_Impl::spaceShadingConstruction() const {
      return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DefaultConstructionSetFields::SpaceShadingConstructionName);
    }

    boost::optional<ConstructionBase> DefaultConstructionSet_Impl::buildingShadingConstruction() const {
      return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DefaultConstructionSetFields::BuildingShadingConstructionName);
    }

    boost::optional<ConstructionBase> DefaultConstructionSet_Impl::siteShadingConstruction() const {
      return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DefaultConstructionSetFields::SiteShadingConstructionName);
    }

    bool DefaultConstructionSet_Impl::setDefaultExteriorSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions) {
      return setPointer(OS_DefaultConstructionSetFields::DefaultExteriorSurfaceConstructionsName, defaultSurfaceConstructions.handle());
    }

    void DefaultConstructionSet_Impl::resetDefaultExteriorSurfaceConstructions() {
      setString(OS_DefaultConstructionSetFields::DefaultExteriorSurfaceConstructionsName, "");
    }

    bool DefaultConstructionSet_Impl::setDefaultInteriorSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions) {
      return setPointer(OS_DefaultConstructionSetFields::DefaultInteriorSurfaceConstructionsName, defaultSurfaceConstructions.handle());
    }

    void DefaultConstructionSet_Impl::resetDefaultInteriorSurfaceConstructions() {
      setString(OS_DefaultConstructionSetFields::DefaultInteriorSurfaceConstructionsName, "");
    }

    bool DefaultConstructionSet_Impl::setDefaultGroundContactSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions) {
      return setPointer(OS_DefaultConstructionSetFields::DefaultGroundContactSurfaceConstructionsName, defaultSurfaceConstructions.handle());
    }

    void DefaultConstructionSet_Impl::resetDefaultGroundContactSurfaceConstructions() {
      setString(OS_DefaultConstructionSetFields::DefaultGroundContactSurfaceConstructionsName, "");
    }

    bool
      DefaultConstructionSet_Impl::setDefaultExteriorSubSurfaceConstructions(const DefaultSubSurfaceConstructions& defaultSubSurfaceConstructions) {
      return setPointer(OS_DefaultConstructionSetFields::DefaultExteriorSubSurfaceConstructionsName, defaultSubSurfaceConstructions.handle());
    }

    void DefaultConstructionSet_Impl::resetDefaultExteriorSubSurfaceConstructions() {
      setString(OS_DefaultConstructionSetFields::DefaultExteriorSubSurfaceConstructionsName, "");
    }

    bool
      DefaultConstructionSet_Impl::setDefaultInteriorSubSurfaceConstructions(const DefaultSubSurfaceConstructions& defaultSubSurfaceConstructions) {
      return setPointer(OS_DefaultConstructionSetFields::DefaultInteriorSubSurfaceConstructionsName, defaultSubSurfaceConstructions.handle());
    }

    void DefaultConstructionSet_Impl::resetDefaultInteriorSubSurfaceConstructions() {
      setString(OS_DefaultConstructionSetFields::DefaultInteriorSubSurfaceConstructionsName, "");
    }

    bool DefaultConstructionSet_Impl::setInteriorPartitionConstruction(const ConstructionBase& construction) {
      return setPointer(OS_DefaultConstructionSetFields::InteriorPartitionConstructionName, construction.handle());
    }

    void DefaultConstructionSet_Impl::resetInteriorPartitionConstruction() {
      setString(OS_DefaultConstructionSetFields::InteriorPartitionConstructionName, "");
    }

    bool DefaultConstructionSet_Impl::setSpaceShadingConstruction(const ConstructionBase& construction) {
      return setPointer(OS_DefaultConstructionSetFields::SpaceShadingConstructionName, construction.handle());
    }

    void DefaultConstructionSet_Impl::resetSpaceShadingConstruction() {
      setString(OS_DefaultConstructionSetFields::SpaceShadingConstructionName, "");
    }

    bool DefaultConstructionSet_Impl::setBuildingShadingConstruction(const ConstructionBase& construction) {
      return setPointer(OS_DefaultConstructionSetFields::BuildingShadingConstructionName, construction.handle());
    }

    void DefaultConstructionSet_Impl::resetBuildingShadingConstruction() {
      setString(OS_DefaultConstructionSetFields::BuildingShadingConstructionName, "");
    }

    bool DefaultConstructionSet_Impl::setSiteShadingConstruction(const ConstructionBase& construction) {
      return setPointer(OS_DefaultConstructionSetFields::SiteShadingConstructionName, construction.handle());
    }

    void DefaultConstructionSet_Impl::resetSiteShadingConstruction() {
      setString(OS_DefaultConstructionSetFields::SiteShadingConstructionName, "");
    }

    boost::optional<ConstructionBase> DefaultConstructionSet_Impl::getDefaultConstruction(const PlanarSurface& planarSurface) const {
      boost::optional<ConstructionBase> result;
      if (planarSurface.optionalCast<Surface>()) {

        boost::optional<DefaultSurfaceConstructions> defaultSurfaceConstructions;
        std::string outsideBoundaryCondition = planarSurface.cast<Surface>().outsideBoundaryCondition();
        if (istringEqual("Outdoors", outsideBoundaryCondition)) {
          defaultSurfaceConstructions = this->defaultExteriorSurfaceConstructions();
        } else if (istringEqual("Ground", outsideBoundaryCondition) || istringEqual("GroundSlabPreprocessorAverage", outsideBoundaryCondition)
                   || istringEqual("GroundSlabPreprocessorCore", outsideBoundaryCondition)
                   || istringEqual("GroundSlabPreprocessorPerimeter", outsideBoundaryCondition)
                   || istringEqual("GroundBasementPreprocessorAverageWall", outsideBoundaryCondition)
                   || istringEqual("GroundBasementPreprocessorAverageFloor", outsideBoundaryCondition)
                   || istringEqual("GroundBasementPreprocessorUpperWall", outsideBoundaryCondition)
                   || istringEqual("GroundBasementPreprocessorLowerWall", outsideBoundaryCondition)) {
          defaultSurfaceConstructions = this->defaultGroundContactSurfaceConstructions();
        } else if (istringEqual("Surface", outsideBoundaryCondition)) {
          defaultSurfaceConstructions = this->defaultInteriorSurfaceConstructions();
        } else if (istringEqual("Adiabatic", outsideBoundaryCondition)) {
          // Adiabatic is special - doesn't has multiple choices by orientation - so return directly
          result = this->adiabaticSurfaceConstruction();
          return result;
        } else {
          //GroundFCfactorMethod
          //OtherSideCoefficients
          //OtherSideConditionsModel
        }

        if (defaultSurfaceConstructions) {
          std::string surfaceType = planarSurface.cast<Surface>().surfaceType();
          if (istringEqual("Floor", surfaceType)) {
            result = defaultSurfaceConstructions->floorConstruction();
          } else if (istringEqual("Wall", surfaceType)) {
            result = defaultSurfaceConstructions->wallConstruction();
          } else if (istringEqual("RoofCeiling", surfaceType)) {
            result = defaultSurfaceConstructions->roofCeilingConstruction();
          }
        }

      } else if (planarSurface.optionalCast<SubSurface>()) {

        boost::optional<DefaultSubSurfaceConstructions> defaultSubSurfaceConstructions;
        boost::optional<Surface> surface = planarSurface.cast<SubSurface>().surface();
        if (surface) {
          std::string outsideBoundaryCondition = surface->outsideBoundaryCondition();
          if (istringEqual("Outdoors", outsideBoundaryCondition)) {
            defaultSubSurfaceConstructions = this->defaultExteriorSubSurfaceConstructions();
          } else if (istringEqual("Surface", outsideBoundaryCondition)) {
            defaultSubSurfaceConstructions = this->defaultInteriorSubSurfaceConstructions();
          } else {
            //Adiabatic
            //Ground
            //GroundFCfactorMethod
            //OtherSideCoefficients
            //OtherSideConditionsModel
            //GroundSlabPreprocessorAverage
            //GroundSlabPreprocessorCore
            //GroundSlabPreprocessorPerimeter
            //GroundBasementPreprocessorAverageWall
            //GroundBasementPreprocessorAverageFloor
            //GroundBasementPreprocessorUpperWall
            //GroundBasementPreprocessorLowerWall
          }
        }

        if (defaultSubSurfaceConstructions) {

          std::string subSurfaceType = planarSurface.cast<SubSurface>().subSurfaceType();
          if (istringEqual("FixedWindow", subSurfaceType)) {
            result = defaultSubSurfaceConstructions->fixedWindowConstruction();
          } else if (istringEqual("OperableWindow", subSurfaceType)) {
            result = defaultSubSurfaceConstructions->operableWindowConstruction();
          } else if (istringEqual("Door", subSurfaceType)) {
            result = defaultSubSurfaceConstructions->doorConstruction();
          } else if (istringEqual("GlassDoor", subSurfaceType)) {
            result = defaultSubSurfaceConstructions->glassDoorConstruction();
          } else if (istringEqual("OverheadDoor", subSurfaceType)) {
            result = defaultSubSurfaceConstructions->overheadDoorConstruction();
          } else if (istringEqual("Skylight", subSurfaceType)) {
            result = defaultSubSurfaceConstructions->skylightConstruction();
          } else if (istringEqual("TubularDaylightDome", subSurfaceType)) {
            result = defaultSubSurfaceConstructions->tubularDaylightDomeConstruction();
          } else if (istringEqual("TubularDaylightDiffuser", subSurfaceType)) {
            result = defaultSubSurfaceConstructions->tubularDaylightDiffuserConstruction();
          }
        }

      } else if (planarSurface.optionalCast<InteriorPartitionSurface>()) {

        result = this->interiorPartitionConstruction();

      } else if (planarSurface.optionalCast<ShadingSurface>()) {

        boost::optional<ShadingSurfaceGroup> shadingSurfaceGroup = planarSurface.cast<ShadingSurface>().shadingSurfaceGroup();
        if (shadingSurfaceGroup) {
          std::string shadingSurfaceType = shadingSurfaceGroup->shadingSurfaceType();
          if (istringEqual("Space", shadingSurfaceType)) {
            result = this->spaceShadingConstruction();
          } else if (istringEqual("Building", shadingSurfaceType)) {
            result = this->buildingShadingConstruction();
          } else if (istringEqual("Site", shadingSurfaceType)) {
            result = this->siteShadingConstruction();
          }
        }

      } else {
        LOG_AND_THROW("Unknown planar surface: " << planarSurface.briefDescription());
      }

      return result;
    }

    void DefaultConstructionSet_Impl::merge(const DefaultConstructionSet& other) {
      boost::optional<DefaultSurfaceConstructions> thisSurfaceConstructions;
      boost::optional<DefaultSurfaceConstructions> otherSurfaceConstructions;
      boost::optional<DefaultSubSurfaceConstructions> thisSubSurfaceConstructions;
      boost::optional<DefaultSubSurfaceConstructions> otherSubSurfaceConstructions;

      otherSurfaceConstructions = other.defaultExteriorSurfaceConstructions();
      if (otherSurfaceConstructions) {
        thisSurfaceConstructions = this->defaultExteriorSurfaceConstructions();
        if (!thisSurfaceConstructions) {
          this->setDefaultExteriorSurfaceConstructions(*otherSurfaceConstructions);
        } else {
          if (thisSurfaceConstructions->handle() == otherSurfaceConstructions->handle()) {
            // no op
          } else {
            if (thisSurfaceConstructions->numSources() > 1) {
              thisSurfaceConstructions = thisSurfaceConstructions->clone(this->model()).cast<DefaultSurfaceConstructions>();
              this->setDefaultExteriorSurfaceConstructions(*thisSurfaceConstructions);
            }
            thisSurfaceConstructions->merge(*otherSurfaceConstructions);
          }
        }
      }

      otherSurfaceConstructions = other.defaultInteriorSurfaceConstructions();
      if (otherSurfaceConstructions) {
        thisSurfaceConstructions = this->defaultInteriorSurfaceConstructions();
        if (!thisSurfaceConstructions) {
          this->setDefaultInteriorSurfaceConstructions(*otherSurfaceConstructions);
        } else {
          if (thisSurfaceConstructions->handle() == otherSurfaceConstructions->handle()) {
            // no op
          } else {
            if (thisSurfaceConstructions->numSources() > 1) {
              thisSurfaceConstructions = thisSurfaceConstructions->clone(this->model()).cast<DefaultSurfaceConstructions>();
              this->setDefaultInteriorSurfaceConstructions(*thisSurfaceConstructions);
            }
            thisSurfaceConstructions->merge(*otherSurfaceConstructions);
          }
        }
      }

      otherSurfaceConstructions = other.defaultGroundContactSurfaceConstructions();
      if (otherSurfaceConstructions) {
        thisSurfaceConstructions = this->defaultGroundContactSurfaceConstructions();
        if (!thisSurfaceConstructions) {
          this->setDefaultGroundContactSurfaceConstructions(*otherSurfaceConstructions);
        } else {
          if (thisSurfaceConstructions->handle() == otherSurfaceConstructions->handle()) {
            // no op
          } else {
            if (thisSurfaceConstructions->numSources() > 1) {
              thisSurfaceConstructions = thisSurfaceConstructions->clone(this->model()).cast<DefaultSurfaceConstructions>();
              this->setDefaultGroundContactSurfaceConstructions(*thisSurfaceConstructions);
            }
            thisSurfaceConstructions->merge(*otherSurfaceConstructions);
          }
        }
      }

      otherSubSurfaceConstructions = other.defaultExteriorSubSurfaceConstructions();
      if (otherSubSurfaceConstructions) {
        thisSubSurfaceConstructions = this->defaultExteriorSubSurfaceConstructions();
        if (!thisSubSurfaceConstructions) {
          this->setDefaultExteriorSubSurfaceConstructions(*otherSubSurfaceConstructions);
        } else {
          if (thisSubSurfaceConstructions->handle() == otherSubSurfaceConstructions->handle()) {
            // no op
          } else {
            if (thisSubSurfaceConstructions->numSources() > 1) {
              thisSubSurfaceConstructions = thisSubSurfaceConstructions->clone(this->model()).cast<DefaultSubSurfaceConstructions>();
              this->setDefaultExteriorSubSurfaceConstructions(*thisSubSurfaceConstructions);
            }
            thisSubSurfaceConstructions->merge(*otherSubSurfaceConstructions);
          }
        }
      }

      otherSubSurfaceConstructions = other.defaultInteriorSubSurfaceConstructions();
      if (otherSubSurfaceConstructions) {
        thisSubSurfaceConstructions = this->defaultInteriorSubSurfaceConstructions();
        if (!thisSubSurfaceConstructions) {
          this->setDefaultInteriorSubSurfaceConstructions(*otherSubSurfaceConstructions);
        } else {
          if (thisSubSurfaceConstructions->handle() == otherSubSurfaceConstructions->handle()) {
            // no op
          } else {
            if (thisSubSurfaceConstructions->numSources() > 1) {
              thisSubSurfaceConstructions = thisSubSurfaceConstructions->clone(this->model()).cast<DefaultSubSurfaceConstructions>();
              this->setDefaultInteriorSubSurfaceConstructions(*thisSubSurfaceConstructions);
            }
            thisSubSurfaceConstructions->merge(*otherSubSurfaceConstructions);
          }
        }
      }

      boost::optional<ConstructionBase> construction;

      if (!(this->interiorPartitionConstruction())) {
        construction = other.interiorPartitionConstruction();
        if (construction) {
          this->setInteriorPartitionConstruction(*construction);
        }
      }

      if (!(this->spaceShadingConstruction())) {
        construction = other.spaceShadingConstruction();
        if (construction) {
          this->setSpaceShadingConstruction(*construction);
        }
      }

      if (!(this->buildingShadingConstruction())) {
        construction = other.buildingShadingConstruction();
        if (construction) {
          this->setBuildingShadingConstruction(*construction);
        }
      }

      if (!(this->siteShadingConstruction())) {
        construction = other.siteShadingConstruction();
        if (construction) {
          this->setSiteShadingConstruction(*construction);
        }
      }

      if (!(this->adiabaticSurfaceConstruction())) {
        construction = other.adiabaticSurfaceConstruction();
        if (construction) {
          this->setAdiabaticSurfaceConstruction(*construction);
        }
      }
    }

    boost::optional<ModelObject> DefaultConstructionSet_Impl::defaultExteriorSurfaceConstructionsAsModelObject() const {
      OptionalModelObject result;
      OptionalDefaultSurfaceConstructions intermediate = defaultExteriorSurfaceConstructions();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> DefaultConstructionSet_Impl::defaultInteriorSurfaceConstructionsAsModelObject() const {
      OptionalModelObject result;
      OptionalDefaultSurfaceConstructions intermediate = defaultInteriorSurfaceConstructions();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> DefaultConstructionSet_Impl::defaultGroundContactSurfaceConstructionsAsModelObject() const {
      OptionalModelObject result;
      OptionalDefaultSurfaceConstructions intermediate = defaultGroundContactSurfaceConstructions();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> DefaultConstructionSet_Impl::defaultExteriorSubSurfaceConstructionsAsModelObject() const {
      OptionalModelObject result;
      OptionalDefaultSubSurfaceConstructions intermediate = defaultExteriorSubSurfaceConstructions();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> DefaultConstructionSet_Impl::defaultInteriorSubSurfaceConstructionsAsModelObject() const {
      OptionalModelObject result;
      OptionalDefaultSubSurfaceConstructions intermediate = defaultInteriorSubSurfaceConstructions();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> DefaultConstructionSet_Impl::interiorPartitionConstructionAsModelObject() const {
      OptionalModelObject result;
      OptionalConstructionBase intermediate = interiorPartitionConstruction();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> DefaultConstructionSet_Impl::spaceShadingConstructionAsModelObject() const {
      OptionalModelObject result;
      OptionalConstructionBase intermediate = spaceShadingConstruction();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> DefaultConstructionSet_Impl::buildingShadingConstructionAsModelObject() const {
      OptionalModelObject result;
      OptionalConstructionBase intermediate = buildingShadingConstruction();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> DefaultConstructionSet_Impl::siteShadingConstructionAsModelObject() const {
      OptionalModelObject result;
      OptionalConstructionBase intermediate = siteShadingConstruction();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool DefaultConstructionSet_Impl::setDefaultExteriorSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalDefaultSurfaceConstructions intermediate = modelObject->optionalCast<DefaultSurfaceConstructions>();
        if (intermediate) {
          return setDefaultExteriorSurfaceConstructions(*intermediate);
        } else {
          return false;
        }
      } else {
        resetDefaultExteriorSurfaceConstructions();
      }
      return true;
    }

    bool DefaultConstructionSet_Impl::setDefaultInteriorSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalDefaultSurfaceConstructions intermediate = modelObject->optionalCast<DefaultSurfaceConstructions>();
        if (intermediate) {
          return setDefaultInteriorSurfaceConstructions(*intermediate);
        } else {
          return false;
        }
      } else {
        resetDefaultInteriorSurfaceConstructions();
      }
      return true;
    }

    bool DefaultConstructionSet_Impl::setDefaultGroundContactSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalDefaultSurfaceConstructions intermediate = modelObject->optionalCast<DefaultSurfaceConstructions>();
        if (intermediate) {
          return setDefaultGroundContactSurfaceConstructions(*intermediate);
        } else {
          return false;
        }
      } else {
        resetDefaultGroundContactSurfaceConstructions();
      }
      return true;
    }

    bool DefaultConstructionSet_Impl::setDefaultExteriorSubSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalDefaultSubSurfaceConstructions intermediate = modelObject->optionalCast<DefaultSubSurfaceConstructions>();
        if (intermediate) {
          return setDefaultExteriorSubSurfaceConstructions(*intermediate);
        } else {
          return false;
        }
      } else {
        resetDefaultExteriorSubSurfaceConstructions();
      }
      return true;
    }

    bool DefaultConstructionSet_Impl::setDefaultInteriorSubSurfaceConstructionsAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalDefaultSubSurfaceConstructions intermediate = modelObject->optionalCast<DefaultSubSurfaceConstructions>();
        if (intermediate) {
          return setDefaultInteriorSubSurfaceConstructions(*intermediate);
        } else {
          return false;
        }
      } else {
        resetDefaultInteriorSubSurfaceConstructions();
      }
      return true;
    }

    bool DefaultConstructionSet_Impl::setInteriorPartitionConstructionAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
        if (intermediate) {
          return setInteriorPartitionConstruction(*intermediate);
        } else {
          return false;
        }
      } else {
        resetInteriorPartitionConstruction();
      }
      return true;
    }

    bool DefaultConstructionSet_Impl::setSpaceShadingConstructionAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
        if (intermediate) {
          return setSpaceShadingConstruction(*intermediate);
        } else {
          return false;
        }
      } else {
        resetSpaceShadingConstruction();
      }
      return true;
    }

    bool DefaultConstructionSet_Impl::setBuildingShadingConstructionAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
        if (intermediate) {
          return setBuildingShadingConstruction(*intermediate);
        } else {
          return false;
        }
      } else {
        resetBuildingShadingConstruction();
      }
      return true;
    }

    bool DefaultConstructionSet_Impl::setSiteShadingConstructionAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
        if (intermediate) {
          return setSiteShadingConstruction(*intermediate);
        } else {
          return false;
        }
      } else {
        resetSiteShadingConstruction();
      }
      return true;
    }

    // Adiabatic Surface Construction Methods
    boost::optional<ConstructionBase> DefaultConstructionSet_Impl::adiabaticSurfaceConstruction() const {
      return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_DefaultConstructionSetFields::AdiabaticSurfaceConstructionName);
    }

    bool DefaultConstructionSet_Impl::setAdiabaticSurfaceConstruction(const ConstructionBase& construction) {
      return setPointer(OS_DefaultConstructionSetFields::AdiabaticSurfaceConstructionName, construction.handle());
    }

    void DefaultConstructionSet_Impl::resetAdiabaticSurfaceConstruction() {
      setString(OS_DefaultConstructionSetFields::AdiabaticSurfaceConstructionName, "");
    }

    boost::optional<ModelObject> DefaultConstructionSet_Impl::adiabaticSurfaceConstructionAsModelObject() const {
      OptionalModelObject result;
      OptionalConstructionBase intermediate = adiabaticSurfaceConstruction();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool DefaultConstructionSet_Impl::setAdiabaticSurfaceConstructionAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
        if (intermediate) {
          return setAdiabaticSurfaceConstruction(*intermediate);
        } else {
          return false;
        }
      } else {
        resetAdiabaticSurfaceConstruction();
      }
      return true;
    }

  }  // namespace detail

  DefaultConstructionSet::DefaultConstructionSet(const Model& model) : ResourceObject(DefaultConstructionSet::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::DefaultConstructionSet_Impl>());
  }

  IddObjectType DefaultConstructionSet::iddObjectType() {
    IddObjectType result(IddObjectType::OS_DefaultConstructionSet);
    return result;
  }

  boost::optional<DefaultSurfaceConstructions> DefaultConstructionSet::defaultExteriorSurfaceConstructions() const {
    return getImpl<detail::DefaultConstructionSet_Impl>()->defaultExteriorSurfaceConstructions();
  }

  boost::optional<DefaultSurfaceConstructions> DefaultConstructionSet::defaultInteriorSurfaceConstructions() const {
    return getImpl<detail::DefaultConstructionSet_Impl>()->defaultInteriorSurfaceConstructions();
  }

  boost::optional<DefaultSurfaceConstructions> DefaultConstructionSet::defaultGroundContactSurfaceConstructions() const {
    return getImpl<detail::DefaultConstructionSet_Impl>()->defaultGroundContactSurfaceConstructions();
  }

  boost::optional<DefaultSubSurfaceConstructions> DefaultConstructionSet::defaultExteriorSubSurfaceConstructions() const {
    return getImpl<detail::DefaultConstructionSet_Impl>()->defaultExteriorSubSurfaceConstructions();
  }

  boost::optional<DefaultSubSurfaceConstructions> DefaultConstructionSet::defaultInteriorSubSurfaceConstructions() const {
    return getImpl<detail::DefaultConstructionSet_Impl>()->defaultInteriorSubSurfaceConstructions();
  }

  boost::optional<ConstructionBase> DefaultConstructionSet::interiorPartitionConstruction() const {
    return getImpl<detail::DefaultConstructionSet_Impl>()->interiorPartitionConstruction();
  }

  boost::optional<ConstructionBase> DefaultConstructionSet::spaceShadingConstruction() const {
    return getImpl<detail::DefaultConstructionSet_Impl>()->spaceShadingConstruction();
  }

  boost::optional<ConstructionBase> DefaultConstructionSet::buildingShadingConstruction() const {
    return getImpl<detail::DefaultConstructionSet_Impl>()->buildingShadingConstruction();
  }

  boost::optional<ConstructionBase> DefaultConstructionSet::siteShadingConstruction() const {
    return getImpl<detail::DefaultConstructionSet_Impl>()->siteShadingConstruction();
  }

  bool DefaultConstructionSet::setDefaultExteriorSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions) {
    return getImpl<detail::DefaultConstructionSet_Impl>()->setDefaultExteriorSurfaceConstructions(defaultSurfaceConstructions);
  }

  void DefaultConstructionSet::resetDefaultExteriorSurfaceConstructions() {
    getImpl<detail::DefaultConstructionSet_Impl>()->resetDefaultExteriorSurfaceConstructions();
  }

  bool DefaultConstructionSet::setDefaultInteriorSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions) {
    return getImpl<detail::DefaultConstructionSet_Impl>()->setDefaultInteriorSurfaceConstructions(defaultSurfaceConstructions);
  }

  void DefaultConstructionSet::resetDefaultInteriorSurfaceConstructions() {
    getImpl<detail::DefaultConstructionSet_Impl>()->resetDefaultInteriorSurfaceConstructions();
  }

  bool DefaultConstructionSet::setDefaultGroundContactSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions) {
    return getImpl<detail::DefaultConstructionSet_Impl>()->setDefaultGroundContactSurfaceConstructions(defaultSurfaceConstructions);
  }

  void DefaultConstructionSet::resetDefaultGroundContactSurfaceConstructions() {
    getImpl<detail::DefaultConstructionSet_Impl>()->resetDefaultGroundContactSurfaceConstructions();
  }

  bool DefaultConstructionSet::setDefaultExteriorSubSurfaceConstructions(const DefaultSubSurfaceConstructions& defaultSubSurfaceConstructions) {
    return getImpl<detail::DefaultConstructionSet_Impl>()->setDefaultExteriorSubSurfaceConstructions(defaultSubSurfaceConstructions);
  }

  void DefaultConstructionSet::resetDefaultExteriorSubSurfaceConstructions() {
    getImpl<detail::DefaultConstructionSet_Impl>()->resetDefaultExteriorSubSurfaceConstructions();
  }

  bool DefaultConstructionSet::setDefaultInteriorSubSurfaceConstructions(const DefaultSubSurfaceConstructions& defaultSubSurfaceConstructions) {
    return getImpl<detail::DefaultConstructionSet_Impl>()->setDefaultInteriorSubSurfaceConstructions(defaultSubSurfaceConstructions);
  }

  void DefaultConstructionSet::resetDefaultInteriorSubSurfaceConstructions() {
    getImpl<detail::DefaultConstructionSet_Impl>()->resetDefaultInteriorSubSurfaceConstructions();
  }

  bool DefaultConstructionSet::setInteriorPartitionConstruction(const ConstructionBase& construction) {
    return getImpl<detail::DefaultConstructionSet_Impl>()->setInteriorPartitionConstruction(construction);
  }

  void DefaultConstructionSet::resetInteriorPartitionConstruction() {
    getImpl<detail::DefaultConstructionSet_Impl>()->resetInteriorPartitionConstruction();
  }

  bool DefaultConstructionSet::setSpaceShadingConstruction(const ConstructionBase& construction) {
    return getImpl<detail::DefaultConstructionSet_Impl>()->setSpaceShadingConstruction(construction);
  }

  void DefaultConstructionSet::resetSpaceShadingConstruction() {
    getImpl<detail::DefaultConstructionSet_Impl>()->resetSpaceShadingConstruction();
  }

  bool DefaultConstructionSet::setBuildingShadingConstruction(const ConstructionBase& construction) {
    return getImpl<detail::DefaultConstructionSet_Impl>()->setBuildingShadingConstruction(construction);
  }

  void DefaultConstructionSet::resetBuildingShadingConstruction() {
    getImpl<detail::DefaultConstructionSet_Impl>()->resetBuildingShadingConstruction();
  }

  bool DefaultConstructionSet::setSiteShadingConstruction(const ConstructionBase& construction) {
    return getImpl<detail::DefaultConstructionSet_Impl>()->setSiteShadingConstruction(construction);
  }

  void DefaultConstructionSet::resetSiteShadingConstruction() {
    getImpl<detail::DefaultConstructionSet_Impl>()->resetSiteShadingConstruction();
  }

  boost::optional<ConstructionBase> DefaultConstructionSet::getDefaultConstruction(const PlanarSurface& planarSurface) const {
    return getImpl<detail::DefaultConstructionSet_Impl>()->getDefaultConstruction(planarSurface);
  }

  void DefaultConstructionSet::merge(const DefaultConstructionSet& other) {
    getImpl<detail::DefaultConstructionSet_Impl>()->merge(other);
  }

  // Helpers for DefaultConstructionSet::collapseToSpaceAndBuilding
  namespace {

    constexpr auto collapseLogChannel = "openstudio.model.DefaultConstructionSet";

    // Set to false once shading constructions are hard-assigned before collapsing: E+ has no shading slot, so a set that only
    // contributes shading constructions shouldn't keep a level in the chain
    constexpr bool countShadingSlots = true;

    // 3 surface categories x 3 types, 2 sub-surface categories x 8 types, interior partition, [space, building, site shading], adiabatic
    constexpr size_t nSlots = (3 * 3) + (2 * 8) + 1 + (countShadingSlots ? 3 : 0) + 1;

    using SlotMask = std::bitset<nSlots>;

    SlotMask filledSlots(const DefaultConstructionSet& dcs) {
      SlotMask mask;
      size_t i = 0;

      for (const auto& d_ :
           {dcs.defaultExteriorSurfaceConstructions(), dcs.defaultInteriorSurfaceConstructions(), dcs.defaultGroundContactSurfaceConstructions()}) {
        if (d_) {
          mask[i] = d_->floorConstruction().has_value();
          mask[i + 1] = d_->wallConstruction().has_value();
          mask[i + 2] = d_->roofCeilingConstruction().has_value();
        }
        i += 3;
      }

      for (const auto& d_ : {dcs.defaultExteriorSubSurfaceConstructions(), dcs.defaultInteriorSubSurfaceConstructions()}) {
        if (d_) {
          mask[i] = d_->fixedWindowConstruction().has_value();
          mask[i + 1] = d_->operableWindowConstruction().has_value();
          mask[i + 2] = d_->doorConstruction().has_value();
          mask[i + 3] = d_->glassDoorConstruction().has_value();
          mask[i + 4] = d_->overheadDoorConstruction().has_value();
          mask[i + 5] = d_->skylightConstruction().has_value();
          mask[i + 6] = d_->tubularDaylightDomeConstruction().has_value();
          mask[i + 7] = d_->tubularDaylightDiffuserConstruction().has_value();
        }
        i += 8;
      }

      mask[i++] = dcs.interiorPartitionConstruction().has_value();
      if constexpr (countShadingSlots) {
        mask[i++] = dcs.spaceShadingConstruction().has_value();
        mask[i++] = dcs.buildingShadingConstruction().has_value();
        mask[i++] = dcs.siteShadingConstruction().has_value();
      }
      mask[i++] = dcs.adiabaticSurfaceConstruction().has_value();

      return mask;
    }

    struct ChainItem
    {
      DefaultConstructionSet dcs;
      Handle ownerHandle;  // the Space, SpaceType, BuildingStory, or Building that points at dcs
      std::string owner;   // e.g. "SpaceType Office", used for the merged set name and logging
    };

    template <typename T>
    constexpr std::string_view ownerKind() {
      if constexpr (std::is_same_v<T, Space>) {
        return "Space";
      } else if constexpr (std::is_same_v<T, SpaceType>) {
        return "SpaceType";
      } else if constexpr (std::is_same_v<T, BuildingStory>) {
        return "BuildingStory";
      } else {
        static_assert(std::is_same_v<T, Building>, "Only Space, SpaceType, BuildingStory and Building own a DefaultConstructionSet");
        return "Building";
      }
    }

    /// Append the owner's DefaultConstructionSet, if any, to the chain
    template <typename T>
    void push(std::vector<ChainItem>& chain, const T& owner) {
      if (auto dcs_ = owner.defaultConstructionSet()) {
        chain.push_back({*dcs_, owner.handle(), std::string{ownerKind<T>()} + " " + owner.nameString()});
      }
    }

    template <typename T>
    void push(std::vector<ChainItem>& chain, const boost::optional<T>& owner_) {
      if (owner_) {
        push(chain, *owner_);
      }
    }

    class Resolver
    {
     public:
      explicit Resolver(Model model) : m_model(std::move(model)) {}

      /// Prunes the chain, then returns: none if empty, the single remaining set, or the merged set for these owners. If the
      /// first remaining set belongs to inPlaceOwner and nobody else uses it, the lower levels are merged into it in place instead
      boost::optional<DefaultConstructionSet> resolve(const std::string& groupName, std::vector<ChainItem> chain,
                                                      const Handle& inPlaceOwner = Handle()) {
        prune(groupName, chain);

        if (chain.empty()) {
          return boost::none;
        }
        if (chain.size() == 1) {
          return chain.front().dcs;
        }

        // The owner's own set (Space or Building), used by nobody else: merge the lower levels into it in place.
        // Only valid when the owner is a Space or the Building: each stands for itself alone, so directUseCount() == 1 really
        // means a single user. A SpaceType or BuildingStory is only an intermediate level that fans out to all its Spaces (and
        // the Building's SpaceType to the Building), so a count of 1 there could hide many Spaces, which is why inPlaceOwner is
        // only ever a Space or the Building.
        auto& front = chain.front();
        if (front.ownerHandle == inPlaceOwner && front.dcs.directUseCount() == 1) {
          for (auto it = std::next(chain.cbegin()); it != chain.cend(); ++it) {
            front.dcs.merge(it->dcs);
          }
          LOG_FREE(Info, collapseLogChannel, groupName << ": merged lower levels into '" << front.dcs.nameString() << "' in place");
          return front.dcs;
        }

        std::vector<Handle> key;
        key.reserve(chain.size());
        for (const auto& item : chain) {
          key.push_back(item.ownerHandle);
        }
        if (auto it = m_byOwners.find(key); it != m_byOwners.end()) {
          return it->second;
        }

        // Miss: create it. The clone still points at the original Default(Sub)SurfaceConstructions; merge() only clones one of
        // those when it has to fill it from another partial one, so sub-object cloning is already minimal.
        auto merged = chain.front().dcs.clone(m_model).cast<DefaultConstructionSet>();
        std::string name = chain.front().owner;
        for (auto it = std::next(chain.cbegin()); it != chain.cend(); ++it) {
          merged.merge(it->dcs);
          name += " @ " + it->owner;
        }
        merged.setName(name);
        LOG_FREE(Info, collapseLogChannel, groupName << ": created merged DefaultConstructionSet '" << merged.nameString() << "'");

        m_byOwners.emplace(std::move(key), merged);
        m_created.push_back(merged);
        return merged;
      }

      std::vector<DefaultConstructionSet> created() const {
        return m_created;
      }

     private:
      /// Drops every set that fills no slot left empty by the sets kept above it
      static void prune(const std::string& groupName, std::vector<ChainItem>& chain) {
        SlotMask covered;
        std::vector<ChainItem> kept;
        kept.reserve(chain.size());
        for (auto& item : chain) {
          const SlotMask filled = filledSlots(item.dcs);
          if ((filled & ~covered).none()) {
            LOG_FREE(Trace, collapseLogChannel,
                     groupName << ": ignoring " << item.owner << "'s DefaultConstructionSet '" << item.dcs.nameString()
                               << "', every slot it fills is already filled higher up");
            continue;
          }
          covered |= filled;
          kept.push_back(std::move(item));
        }
        chain = std::move(kept);
      }

      Model m_model;
      std::map<std::vector<Handle>, DefaultConstructionSet> m_byOwners;
      std::vector<DefaultConstructionSet> m_created;
    };

    /// Building group: Building (search distance 4), then Building SpaceType (5), collapsed onto the Building. Returns the created sets
    std::vector<DefaultConstructionSet> collapseToBuilding(Model& model) {
      // model.building(), not getUniqueModelObject, so we don't instantiate a Building
      auto building_ = model.building();
      if (!building_) {
        return {};
      }

      Resolver resolver(model);
      std::vector<ChainItem> chain;
      push(chain, *building_);
      push(chain, building_->spaceType());
      if (auto dcs_ = resolver.resolve("Building", std::move(chain), building_->handle())) {
        building_->setDefaultConstructionSet(*dcs_);
      } else {
        building_->resetDefaultConstructionSet();
      }

      return resolver.created();
    }

    /// Space group, per Space: Space (1), then its SpaceType if explicit (2), then its BuildingStory (3), collapsed onto the Space.
    /// Returns the created sets
    std::vector<DefaultConstructionSet> collapseToSpaces(Model& model) {
      Resolver resolver(model);

      for (auto& space : model.getConcreteModelObjects<Space>()) {
        const std::string groupName = "Space '" + space.nameString() + "'";
        std::vector<ChainItem> chain;
        push(chain, space);
        if (!space.isSpaceTypeDefaulted()) {  // defaulted == Building's SpaceType, which is in the Building group
          push(chain, space.spaceType());
        }
        push(chain, space.buildingStory());

        if (auto dcs_ = resolver.resolve(groupName, std::move(chain), space.handle())) {
          space.setDefaultConstructionSet(*dcs_);  // no-op when it's the Space's own
        } else {
          space.resetDefaultConstructionSet();  // no set in the group, or only empty ones
        }
      }

      return resolver.created();
    }

  }  // namespace

  // OS inheritance order (cf Space_Impl::getDefaultConstructionWithSearchDistance):
  //
  //   Space -> SpaceType (explicit only) -> BuildingStory -> Building -> Building SpaceType
  //   \__________________ ___________________________/     \_______________ _____________/
  //                      V                                                 V
  //                 Space Group                                     Building Group
  //
  // Each group only reads its own levels' sets (the Space group never reads the Building's), and SpaceType / BuildingStory sets
  // are only reset at the very end (if purging), so assigning as we go is safe. Original sets are only mutated when merged into
  // in place: a Space's or the Building's own set, with no other user.
  //
  // Collapsing preserves every surface's construction, but not its search distance: what came from a SpaceType / BuildingStory
  // (2, 3) now comes from the Space (1), and what came from the Building's SpaceType (5) now comes from the Building (4).
  // Purging or not makes no difference to either: a collapsed set's slots are the union of its group's slots, so a slot that
  // falls through the Space's set also falls through its SpaceType's and BuildingStory's, and is never found there.
  std::vector<DefaultConstructionSet> DefaultConstructionSet::collapseToSpaceAndBuilding(Model& model, bool purge) {

    // Sets in use before the call: the only ones a purge may remove
    std::vector<DefaultConstructionSet> usedBefore;
    if (purge) {
      for (auto& dcs : model.getConcreteModelObjects<DefaultConstructionSet>()) {
        if (dcs.directUseCount() > 0) {
          usedBefore.push_back(std::move(dcs));
        }
      }
    }

    // Building first: it can point the Building to an existing set (e.g. its SpaceType's), which the Space group's in place check
    // must see as another user of that set
    std::vector<DefaultConstructionSet> created = collapseToBuilding(model);
    const std::vector<DefaultConstructionSet> spaceCreated = collapseToSpaces(model);
    created.insert(created.end(), spaceCreated.begin(), spaceCreated.end());

    if (!purge) {
      return created;
    }

    // Purge:

    // 1. Drop the intermediate levels so the model looks like E+: sets only on Spaces and the Building (includes the Building's SpaceType)
    for (auto& spaceType : model.getConcreteModelObjects<SpaceType>()) {
      spaceType.resetDefaultConstructionSet();
    }
    for (auto& story : model.getConcreteModelObjects<BuildingStory>()) {
      story.resetDefaultConstructionSet();
    }

    // 2. Remove the sets this left unused, but capture their sub-objects to see if they are orphaned after

    // A set, since two removed sets can share a sub-object
    std::set<ResourceObject> subObjects;
    for (auto& dcs : usedBefore) {
      if (dcs.directUseCount() > 0) {
        continue;
      }
      for (const auto& d_ :
           {dcs.defaultExteriorSurfaceConstructions(), dcs.defaultInteriorSurfaceConstructions(), dcs.defaultGroundContactSurfaceConstructions()}) {
        if (d_) {
          subObjects.insert(*d_);
        }
      }
      for (const auto& d_ : {dcs.defaultExteriorSubSurfaceConstructions(), dcs.defaultInteriorSubSurfaceConstructions()}) {
        if (d_) {
          subObjects.insert(*d_);
        }
      }
      LOG_FREE(Trace, collapseLogChannel, "Removing DefaultConstructionSet '" << dcs.nameString() << "', no longer used");
      dcs.remove();
    }

    // 3. Remove the sub-objects that are no longer used

    // Copy (cheap handle wrapper): set elements are const, and remove() isn't
    for (auto subObject : subObjects) {  // NOLINT(performance-for-range-copy)
      if (subObject.directUseCount() == 0) {
        subObject.remove();
      }
    }

    return created;
  }

  boost::optional<ConstructionBase> DefaultConstructionSet::adiabaticSurfaceConstruction() const {
    return getImpl<detail::DefaultConstructionSet_Impl>()->adiabaticSurfaceConstruction();
  }

  bool DefaultConstructionSet::setAdiabaticSurfaceConstruction(const ConstructionBase& construction) {
    return getImpl<detail::DefaultConstructionSet_Impl>()->setAdiabaticSurfaceConstruction(construction);
  }

  void DefaultConstructionSet::resetAdiabaticSurfaceConstruction() {
    getImpl<detail::DefaultConstructionSet_Impl>()->resetAdiabaticSurfaceConstruction();
  }

  /// @cond
  DefaultConstructionSet::DefaultConstructionSet(std::shared_ptr<detail::DefaultConstructionSet_Impl> impl) : ResourceObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
