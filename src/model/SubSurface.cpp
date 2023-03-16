/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"
#include "ShadingSurfaceGroup.hpp"
#include "ShadingSurfaceGroup_Impl.hpp"
#include "ShadingControl.hpp"
#include "ShadingControl_Impl.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "Construction.hpp"
#include "Construction_Impl.hpp"
#include "DaylightingDeviceShelf.hpp"
#include "DaylightingDeviceShelf_Impl.hpp"
#include "DaylightingDeviceTubular.hpp"
#include "DaylightingDeviceTubular_Impl.hpp"
#include "DaylightingDeviceLightWell.hpp"
#include "DaylightingDeviceLightWell_Impl.hpp"
#include "WindowPropertyFrameAndDivider.hpp"
#include "WindowPropertyFrameAndDivider_Impl.hpp"
#include "SurfacePropertyOtherSideCoefficients.hpp"
#include "SurfacePropertyOtherSideCoefficients_Impl.hpp"
#include "SurfacePropertyOtherSideConditionsModel.hpp"
#include "SurfacePropertyOtherSideConditionsModel_Impl.hpp"
#include "SurfacePropertyConvectionCoefficients.hpp"
#include "SurfacePropertyConvectionCoefficients_Impl.hpp"
#include "SurfacePropertyLocalEnvironment.hpp"
#include "SurfacePropertyLocalEnvironment_Impl.hpp"
#include "SurfacePropertyIncidentSolarMultiplier.hpp"
#include "SurfacePropertyIncidentSolarMultiplier_Impl.hpp"
#include "AirflowNetworkSurface.hpp"
#include "AirflowNetworkSurface_Impl.hpp"
#include "AirflowNetworkDetailedOpening.hpp"
#include "AirflowNetworkDetailedOpening_Impl.hpp"
#include "AirflowNetworkCrack.hpp"
#include "AirflowNetworkCrack_Impl.hpp"
#include "AirflowNetworkSimpleOpening.hpp"
#include "AirflowNetworkSimpleOpening_Impl.hpp"
#include "AirflowNetworkEffectiveLeakageArea.hpp"
#include "AirflowNetworkEffectiveLeakageArea_Impl.hpp"
#include "AirflowNetworkHorizontalOpening.hpp"
#include "AirflowNetworkHorizontalOpening_Impl.hpp"
#include "AirflowNetworkSpecifiedFlowRate.hpp"
#include "AirflowNetworkSpecifiedFlowRate_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SubSurface_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/sql/SqlFile.hpp"

#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/Intersection.hpp"
#include "../utilities/geometry/BoundingBox.hpp"
#include "../utilities/core/Assert.hpp"

using boost::to_upper_copy;

namespace openstudio {
namespace model {

  namespace detail {

    SubSurface_Impl::SubSurface_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : PlanarSurface_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SubSurface::iddObjectType());
    }

    SubSurface_Impl::SubSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : PlanarSurface_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SubSurface::iddObjectType());
    }

    SubSurface_Impl::SubSurface_Impl(const SubSurface_Impl& other, Model_Impl* model, bool keepHandle)
      : PlanarSurface_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> SubSurface_Impl::parent() const {
      return boost::optional<ParentObject>(this->surface());
    }

    bool SubSurface_Impl::setParent(ParentObject& newParent) {
      bool result = false;
      if (newParent.optionalCast<Surface>()) {
        result = this->setSurface(newParent.cast<Surface>());
      }
      return result;
    }

    std::vector<ModelObject> SubSurface_Impl::children() const {
      std::vector<ModelObject> result;
      boost::optional<DaylightingDeviceShelf> daylightingDeviceShelf = this->daylightingDeviceShelf();
      if (daylightingDeviceShelf) {
        result.push_back(*daylightingDeviceShelf);
      }
      boost::optional<DaylightingDeviceTubular> daylightingDeviceTubular = this->daylightingDeviceTubular();
      if (daylightingDeviceTubular) {
        result.push_back(*daylightingDeviceTubular);
      }
      boost::optional<DaylightingDeviceLightWell> daylightingDeviceLightWell = this->daylightingDeviceLightWell();
      if (daylightingDeviceLightWell) {
        result.push_back(*daylightingDeviceLightWell);
      }
      std::vector<AirflowNetworkSurface> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkSurface>(AirflowNetworkSurface::iddObjectType());
      result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());

      return result;
    }

    std::vector<IdfObject> SubSurface_Impl::remove() {
      boost::optional<SubSurface> adjacentSubSurface = this->adjacentSubSurface();
      if (adjacentSubSurface) {
        this->resetAdjacentSubSurface();
      }

      // Remove it from the extensible groups in ShadingControl(s)
      for (auto& sc : shadingControls()) {
        sc.removeSubSurface(this->getObject<SubSurface>());
      }

      return ParentObject_Impl::remove();
    }

    ModelObject SubSurface_Impl::clone(Model model) const {
      auto clone = ParentObject_Impl::clone(model).cast<SubSurface>();

      auto coefficients = surfacePropertyConvectionCoefficients();
      if (coefficients) {
        auto coefficientsClone = coefficients->clone(model).cast<SurfacePropertyConvectionCoefficients>();
        coefficientsClone.setSurface(clone);
      }

      auto controls = shadingControls();
      for (const auto& sc : controls) {
        auto shadingConrolClone = sc.clone(model).cast<ShadingControl>();
        clone.addShadingControl(shadingConrolClone);
      }

      return std::move(clone);
    }

    const std::vector<std::string>& SubSurface_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Surface Inside Face Temperature",
                                                   "Surface Outside Face Temperature",
                                                   "Daylighting Window Reference Point 1 View Luminance",
                                                   "Daylighting Window Reference Point 1 Illuminance",
                                                   "Daylighting Window Reference Point 2 View Luminance",
                                                   "Daylighting Window Reference Point 2 Illuminance"};
      return result;
    }

    IddObjectType SubSurface_Impl::iddObjectType() const {
      return SubSurface::iddObjectType();
    }

    boost::optional<ConstructionBase> SubSurface_Impl::construction() const {
      // DLM: there is duplicate code in ForwardTranslator::resolveMatchedSubSurfaceConstructionConflicts
      // if you update this code you must update that code

      // DLM: I am not sure we should be doing this here at all, maybe this method should just
      // return the same thing constructionWithSearchDistance does?

      boost::optional<std::pair<ConstructionBase, int>> constructionWithSearchDistance = this->constructionWithSearchDistance();

      model::OptionalSubSurface adjacentSubSurface = this->adjacentSubSurface();
      if (!adjacentSubSurface) {
        if (constructionWithSearchDistance) {
          return constructionWithSearchDistance->first;
        }
        return boost::none;
      }

      boost::optional<std::pair<model::ConstructionBase, int>> adjacentConstructionWithSearchDistance =
        adjacentSubSurface->constructionWithSearchDistance();

      if (constructionWithSearchDistance && !adjacentConstructionWithSearchDistance) {
        // return this construction
        return constructionWithSearchDistance->first;
      }

      if (!constructionWithSearchDistance && adjacentConstructionWithSearchDistance) {
        // return adjacent construction
        return adjacentConstructionWithSearchDistance->first;
      }

      if (!constructionWithSearchDistance && !adjacentConstructionWithSearchDistance) {
        // no constructions, nothing to be done
        return boost::none;
      }

      // both surfaces return a construction

      if (constructionWithSearchDistance->first.handle() == adjacentConstructionWithSearchDistance->first.handle()) {
        // both surfaces have same construction
        return constructionWithSearchDistance->first;
      }

      // both surfaces return a construction and they are not the same

      if (constructionWithSearchDistance->second < adjacentConstructionWithSearchDistance->second) {
        // lower search distance to construction
        return constructionWithSearchDistance->first;
      }

      if (constructionWithSearchDistance->second > adjacentConstructionWithSearchDistance->second) {
        // lower search distance to adjacent construction
        return adjacentConstructionWithSearchDistance->first;
      }

      // both surfaces return a construction, they are not the same, and both have same search distance

      if (constructionWithSearchDistance->first.optionalCast<model::LayeredConstruction>()
          && adjacentConstructionWithSearchDistance->first.optionalCast<model::LayeredConstruction>()) {
        if (constructionWithSearchDistance->first.cast<model::LayeredConstruction>().reverseEqualLayers(
              adjacentConstructionWithSearchDistance->first.cast<model::LayeredConstruction>())) {
          // these constructions are reverse equal
          return constructionWithSearchDistance->first;
        }
      }

      // give up for now, we can add more later
      // DLM: should we return empty or this construction?
      return constructionWithSearchDistance->first;
    }

    boost::optional<std::pair<ConstructionBase, int>> SubSurface_Impl::constructionWithSearchDistance() const {
      boost::optional<std::pair<ConstructionBase, int>> result;

      boost::optional<ConstructionBase> construction =
        getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_SubSurfaceFields::ConstructionName);
      if (construction) {
        return std::make_pair(*construction, 0);
      }

      boost::optional<Surface> surface = this->surface();
      if (surface) {
        boost::optional<Space> space = surface->space();
        if (space) {
          result = space->getDefaultConstructionWithSearchDistance(this->getObject<SubSurface>());
        }
      }

      return result;
    }

    bool SubSurface_Impl::isConstructionDefaulted() const {
      return isEmpty(OS_SubSurfaceFields::ConstructionName);
    }

    bool SubSurface_Impl::setVertices(const std::vector<Point3d>& vertices) {
      bool result = PlanarSurface_Impl::setVertices(vertices);

      if (isEmpty(OS_SubSurfaceFields::SubSurfaceType)) {
        if (result) {
          this->assignDefaultSubSurfaceType();
        } else {
          LOG(Error, "Cannot compute default SubSurface properties.");
        }
      }

      return result;
    }

    bool SubSurface_Impl::setConstruction(const ConstructionBase& construction) {
      bool result = setPointer(OS_SubSurfaceFields::ConstructionName, construction.handle());
      return result;
    }

    void SubSurface_Impl::resetConstruction() {
      bool result = setString(OS_SubSurfaceFields::ConstructionName, "");
      OS_ASSERT(result);
    }

    boost::optional<PlanarSurfaceGroup> SubSurface_Impl::planarSurfaceGroup() const {
      return boost::optional<PlanarSurfaceGroup>(this->space());
    }

    boost::optional<Space> SubSurface_Impl::space() const {
      boost::optional<Space> result;
      boost::optional<Surface> surface = this->surface();
      if (surface) {
        result = surface->space();
      }
      return result;
    }

    boost::optional<double> SubSurface_Impl::uFactor() const {
      OptionalConstructionBase oConstruction = construction();
      OptionalSurface oSurface = surface();
      OptionalDouble result;

      if (oConstruction) {
        // from input
        OptionalDouble inputResult;
        if (oSurface) {
          inputResult = oConstruction->uFactor(oSurface->filmResistance());
        }

        // from output
        OptionalSqlFile sqlFile = model().sqlFile();
        OptionalString constructionName = oConstruction->name();
        OptionalDouble outputResult;
        OptionalString subSurfaceName = name();

        // opaque exterior
        if (sqlFile && constructionName && oConstruction->isOpaque()) {
          std::string query = R"(SELECT Value from TabularDataWithStrings
                                    WHERE ReportName = 'EnvelopeSummary'
                                      AND ReportForString = 'Entire Facility'
                                      AND TableName = 'Opaque Exterior'
                                      AND ColumnName = 'U-Factor with Film'
                                      AND Units='W/m2-K'
                                      AND RowName = ?;)";
          outputResult = sqlFile->execAndReturnFirstDouble(query, to_upper_copy(*subSurfaceName));
        }

        // fenestration
        if (sqlFile && constructionName && oConstruction->isFenestration()) {
          std::string query = R"(SELECT Value from TabularDataWithStrings
                                    WHERE ReportName = 'EnvelopeSummary'
                                      AND ReportForString = 'Entire Facility'
                                      AND TableName = 'Exterior Fenestration'
                                      AND ColumnName = 'Glass U-Factor'
                                      AND Units='W/m2-K'
                                      AND RowName = ?;)";
          outputResult = sqlFile->execAndReturnFirstDouble(query, to_upper_copy(*subSurfaceName));
        }

        if (inputResult) {
          result = inputResult;
          if (outputResult) {
            compareInputAndOutput(*oConstruction, "uFactor", *inputResult, *outputResult, 1.0E-3);
          }
        } else {
          result = outputResult;
        }
      }

      return result;
    }

    boost::optional<double> SubSurface_Impl::thermalConductance() const {
      OptionalConstructionBase oConstruction = construction();
      OptionalSurface oSurface = surface();
      OptionalDouble result;

      if (oConstruction && oSurface) {
        // from input
        OptionalDouble inputResult = oConstruction->thermalConductance(oSurface->filmResistance());

        // from output
        OptionalSqlFile sqlFile = model().sqlFile();
        OptionalString constructionName = oConstruction->name();
        OptionalDouble outputResult;
        OptionalString subSurfaceName = name();

        // opaque exterior
        if (sqlFile && constructionName && oConstruction->isOpaque()) {
          std::string query = R"(SELECT Value from TabularDataWithStrings
                                    WHERE ReportName = 'EnvelopeSummary'
                                      AND ReportForString = 'Entire Facility'
                                      AND TableName = 'Opaque Exterior'
                                      AND ColumnName = 'U-Factor no Film'
                                      AND Units='W/m2-K'
                                      AND RowName = ?;)";
          outputResult = sqlFile->execAndReturnFirstDouble(query, to_upper_copy(*subSurfaceName));
        }

        // fenestration
        if (sqlFile && constructionName && oConstruction->isFenestration()) {

          // get u-factor, then subtract film coefficients
          std::string query = R"(SELECT Value from TabularDataWithStrings
                                    WHERE ReportName = 'EnvelopeSummary'
                                      AND ReportForString = 'Entire Facility'
                                      AND TableName = 'Exterior Fenestration'
                                      AND ColumnName = 'Glass U-Factor'
                                      AND Units='W/m2-K'
                                      AND RowName = ?;)";
          outputResult = sqlFile->execAndReturnFirstDouble(query, to_upper_copy(*subSurfaceName));

          if (outputResult) {
            outputResult = 1.0 / (1.0 / (*outputResult) - oSurface->filmResistance());
          }
        }

        if (inputResult) {
          result = inputResult;
          if (outputResult) {
            compareInputAndOutput(*oConstruction, "thermal conductance", *inputResult, *outputResult, 1.0E-5);
          }
        } else {
          result = outputResult;
        }
      }

      return result;
    }

    bool SubSurface_Impl::setUFactor(double value) {
      OptionalConstructionBase oConstruction = this->construction();
      OptionalSurface oSurface = this->surface();
      if (oConstruction && oSurface) {
        return oConstruction->setUFactor(value, oSurface->filmResistance());
      }
      return false;
    }

    bool SubSurface_Impl::setThermalConductance(double value) {
      OptionalConstructionBase oConstruction = this->construction();
      OptionalSurface oSurface = this->surface();
      if (oConstruction && oSurface) {
        return oConstruction->setConductance(value, oSurface->filmResistance());
      }
      return false;
    }

    bool SubSurface_Impl::subtractFromGrossArea() const {
      return true;
    }

    std::string SubSurface_Impl::subSurfaceType() const {
      boost::optional<std::string> value = getString(OS_SubSurfaceFields::SubSurfaceType, true, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SubSurface_Impl::isSubSurfaceTypeDefaulted() const {
      return isEmpty(OS_SubSurfaceFields::SubSurfaceType);
    }

    std::vector<std::string> SubSurface_Impl::subSurfaceTypeValues() const {
      return SubSurface::validSubSurfaceTypeValues();
    }

    boost::optional<double> SubSurface_Impl::viewFactortoGround() const {
      return getDouble(OS_SubSurfaceFields::ViewFactortoGround, true);
    }

    bool SubSurface_Impl::isViewFactortoGroundDefaulted() const {
      return isEmpty(OS_SubSurfaceFields::ViewFactortoGround);
    }

    bool SubSurface_Impl::isViewFactortoGroundAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_SubSurfaceFields::ViewFactortoGround, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autocalculate");
      }
      return result;
    }

    bool SubSurface_Impl::allowShadingControl() const {
      bool result = false;

      std::string subSurfaceType = this->subSurfaceType();
      if (istringEqual("FixedWindow", subSurfaceType) || istringEqual("OperableWindow", subSurfaceType) || istringEqual("Skylight", subSurfaceType)
          || istringEqual("GlassDoor", subSurfaceType)) {
        result = true;
      }

      return result;
    }

    std::vector<ShadingControl> SubSurface_Impl::shadingControls() const {
      auto thisSubSurface = getObject<SubSurface>();

      std::vector<ShadingControl> shadingControls;

      for (const auto& object : getObject<ModelObject>().getModelObjectSources<ShadingControl>()) {
        auto modelObject = object.cast<ModelObject>();
        auto shadingControl = modelObject.cast<ShadingControl>();
        for (const SubSurface& subSurface : shadingControl.subSurfaces()) {
          if (subSurface.handle() == thisSubSurface.handle()) {
            shadingControls.push_back(shadingControl);
          }
        }
      }

      return shadingControls;
    }

    unsigned int SubSurface_Impl::numberofShadingControls() const {
      return shadingControls().size();
    }

    bool SubSurface_Impl::allowWindowPropertyFrameAndDivider() const {
      bool result = false;

      std::string subSurfaceType = this->subSurfaceType();
      if (istringEqual("FixedWindow", subSurfaceType) || istringEqual("OperableWindow", subSurfaceType) || istringEqual("Skylight", subSurfaceType)
          || istringEqual("GlassDoor", subSurfaceType)) {
        result = true;
      }

      return result;
    }

    boost::optional<WindowPropertyFrameAndDivider> SubSurface_Impl::windowPropertyFrameAndDivider() const {
      return getObject<SubSurface>().getModelObjectTarget<WindowPropertyFrameAndDivider>(OS_SubSurfaceFields::FrameandDividerName);
    }

    double SubSurface_Impl::multiplier() const {
      boost::optional<double> value = getDouble(OS_SubSurfaceFields::Multiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SubSurface_Impl::isMultiplierDefaulted() const {
      return isEmpty(OS_SubSurfaceFields::Multiplier);
    }

    boost::optional<double> SubSurface_Impl::numberofVertices() const {
      return getDouble(OS_SubSurfaceFields::NumberofVertices, true);
    }

    bool SubSurface_Impl::isNumberofVerticesDefaulted() const {
      return isEmpty(OS_SubSurfaceFields::NumberofVertices);
    }

    bool SubSurface_Impl::isNumberofVerticesAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_SubSurfaceFields::NumberofVertices, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autocalculate");
      }
      return result;
    }

    bool SubSurface_Impl::setSubSurfaceType(const std::string& subSurfaceType) {
      bool result = setString(OS_SubSurfaceFields::SubSurfaceType, subSurfaceType);
      if (result) {

        if (!allowShadingControl() && (numberofShadingControls() > 0)) {
          LOG(Warn, briefDescription() << " new subSurfaceType='" << subSurfaceType
                                       << "' is incompatible with Shading Controls. Removing this SubSurface from the Shading Controls.");
          this->removeAllShadingControls();
        }

        if (!allowWindowPropertyFrameAndDivider() && windowPropertyFrameAndDivider()) {
          LOG(Warn, briefDescription() << " new subSurfaceType=" << subSurfaceType
                                       << "' is incompatible with the Window Property Frame and Divider. Resetting the Frame and Divider object.");
          this->resetWindowPropertyFrameAndDivider();
        }

        if (!allowDaylightingDeviceShelf()) {
          if (boost::optional<DaylightingDeviceShelf> shelf = this->daylightingDeviceShelf()) {
            LOG(Warn, briefDescription() << " new subSurfaceType='" << subSurfaceType
                                         << "' is incompatible with Daylighting Device Shelf. Removing the Shelf object.");
            shelf->remove();
          }
        }

        if (!allowDaylightingDeviceTubularDome()) {
          if (boost::optional<DaylightingDeviceTubular> tubular = this->daylightingDeviceTubular()) {
            if (tubular->subSurfaceDome().handle() == this->handle()) {
              LOG(Warn,
                  briefDescription() << " cannot be used as a dome subsurface on a Daylighting Device Tubular object. Removing the Tubular object.");
              tubular->remove();
            }
          }
        }

        if (!allowDaylightingDeviceTubularDiffuser()) {
          if (boost::optional<DaylightingDeviceTubular> tubular = this->daylightingDeviceTubular()) {
            if (tubular->subSurfaceDiffuser().handle() == this->handle()) {
              LOG(Warn, briefDescription()
                          << " cannot be used as a diffuser subsurface on a Daylighting Device Tubular object. Removing the Tubular object.");
              tubular->remove();
            }
          }
        }

        if (!allowDaylightingDeviceLightWell()) {
          if (boost::optional<DaylightingDeviceLightWell> lightwell = this->daylightingDeviceLightWell()) {
            LOG(Warn, briefDescription() << " cannot be used as a subsurface on a Daylighting Device Light Well object. Removing the Well object.");
            lightwell->remove();
          }
        }

        if (boost::optional<SubSurface> adjacentSubSurface = this->adjacentSubSurface()) {
          adjacentSubSurface->setString(OS_SubSurfaceFields::SubSurfaceType, subSurfaceType);
          adjacentSubSurface->removeAllShadingControls();
        }
      }
      return result;
    }

    void SubSurface_Impl::resetSubSurfaceType() {
      bool result = setString(OS_SubSurfaceFields::SubSurfaceType, "");
      OS_ASSERT(result);
    }

    bool SubSurface_Impl::setViewFactortoGround(boost::optional<double> viewFactortoGround) {
      bool result = false;
      if (viewFactortoGround) {
        result = setDouble(OS_SubSurfaceFields::ViewFactortoGround, viewFactortoGround.get());
      } else {
        result = setString(OS_SubSurfaceFields::ViewFactortoGround, "");
      }
      return result;
    }

    bool SubSurface_Impl::setViewFactortoGround(double viewFactortoGround) {
      bool result = setDouble(OS_SubSurfaceFields::ViewFactortoGround, viewFactortoGround);
      return result;
    }

    void SubSurface_Impl::resetViewFactortoGround() {
      bool result = setString(OS_SubSurfaceFields::ViewFactortoGround, "");
      OS_ASSERT(result);
    }

    void SubSurface_Impl::autocalculateViewFactortoGround() {
      bool result = setString(OS_SubSurfaceFields::ViewFactortoGround, "Autocalculate");
      OS_ASSERT(result);
    }

    bool SubSurface_Impl::addShadingControl(ShadingControl& shadingControl) {
      bool result = false;
      if (allowShadingControl()) {
        auto thisSubSurface = getObject<SubSurface>();
        result = shadingControl.addSubSurface(thisSubSurface);
      }
      return result;
    }

    bool SubSurface_Impl::addShadingControls(std::vector<ShadingControl>& shadingControls) {
      bool ok = true;
      auto thisSubSurface = getObject<SubSurface>();
      for (ShadingControl& shadingControl : shadingControls) {
        ok &= shadingControl.addSubSurface(thisSubSurface);
      }
      return ok;
    }

    void SubSurface_Impl::removeShadingControl(ShadingControl& shadingControl) {
      auto thisSubSurface = getObject<SubSurface>();
      shadingControl.removeSubSurface(thisSubSurface);
    }

    void SubSurface_Impl::removeAllShadingControls() {
      auto thisSubSurface = getObject<SubSurface>();
      for (ShadingControl& shadingControl : shadingControls()) {
        shadingControl.removeSubSurface(thisSubSurface);
      }
    }

    bool SubSurface_Impl::setWindowPropertyFrameAndDivider(const WindowPropertyFrameAndDivider& windowPropertyFrameAndDivider) {
      bool result = false;
      if (allowWindowPropertyFrameAndDivider()) {
        result = setPointer(OS_SubSurfaceFields::FrameandDividerName, windowPropertyFrameAndDivider.handle());
      }
      return result;
    }

    void SubSurface_Impl::resetWindowPropertyFrameAndDivider() {
      bool result = setString(OS_SubSurfaceFields::FrameandDividerName, "");
      OS_ASSERT(result);
    }

    bool SubSurface_Impl::setMultiplier(double multiplier) {
      bool result = setDouble(OS_SubSurfaceFields::Multiplier, multiplier);

      if (result) {
        boost::optional<SubSurface> adjacentSubSurface = this->adjacentSubSurface();
        if (adjacentSubSurface) {
          result = adjacentSubSurface->setDouble(OS_SubSurfaceFields::Multiplier, multiplier);
          OS_ASSERT(result);
        }
      }

      return result;
    }

    void SubSurface_Impl::resetMultiplier() {
      bool result = setString(OS_SubSurfaceFields::Multiplier, "");
      OS_ASSERT(result);

      boost::optional<SubSurface> adjacentSubSurface = this->adjacentSubSurface();
      if (adjacentSubSurface) {
        result = adjacentSubSurface->setString(OS_SubSurfaceFields::Multiplier, "");
        OS_ASSERT(result);
      }
    }

    bool SubSurface_Impl::setNumberofVertices(boost::optional<double> numberofVertices) {
      bool result = false;
      if (numberofVertices) {
        result = setDouble(OS_SubSurfaceFields::NumberofVertices, numberofVertices.get());
      } else {
        result = setString(OS_SubSurfaceFields::NumberofVertices, "");
      }
      return result;
    }

    bool SubSurface_Impl::setNumberofVertices(double numberofVertices) {
      bool result = setDouble(OS_SubSurfaceFields::NumberofVertices, numberofVertices);
      return result;
    }

    void SubSurface_Impl::resetNumberofVertices() {
      bool result = setString(OS_SubSurfaceFields::NumberofVertices, "");
      OS_ASSERT(result);
    }

    void SubSurface_Impl::autocalculateNumberofVertices() {
      bool result = setString(OS_SubSurfaceFields::NumberofVertices, "Autocalculate");
      OS_ASSERT(result);
    }

    boost::optional<Surface> SubSurface_Impl::surface() const {
      return getObject<SubSurface>().getModelObjectTarget<Surface>(OS_SubSurfaceFields::SurfaceName);
    }

    bool SubSurface_Impl::setSurface(const Surface& surface) {
      bool emptySurface = isEmpty(OS_SubSurfaceFields::SurfaceName);
      bool result = setPointer(OS_SubSurfaceFields::SurfaceName, surface.handle());
      if (result && emptySurface && isSubSurfaceTypeDefaulted()) {
        assignDefaultSubSurfaceType();
      }
      return result;
    }

    boost::optional<SubSurface> SubSurface_Impl::adjacentSubSurface() const {
      return getObject<SubSurface>().getModelObjectTarget<SubSurface>(OS_SubSurfaceFields::OutsideBoundaryConditionObject);
    }

    bool SubSurface_Impl::setAdjacentSubSurface(SubSurface& subSurface) {
      // matching sub surface with self is ok for stories with multipliers
      bool isSameSubSurface = false;
      if (subSurface.handle() == this->handle()) {
        isSameSubSurface = true;
      }

      if (this->multiplier() != subSurface.multiplier()) {
        return false;
      }

      bool result = false;

      boost::optional<Surface> thisSurface = this->surface();
      boost::optional<Surface> otherSurface = subSurface.surface();
      if (thisSurface && otherSurface) {

        // if base surfaces are matched
        boost::optional<Surface> adjacentSurface = thisSurface->adjacentSurface();
        if (adjacentSurface && (adjacentSurface->handle() == otherSurface->handle())) {

          // check for unequal subsurfacetypes
          std::string thisSubSurfaceType = this->subSurfaceType();
          std::string adjacentSubSurfaceType = subSurface.subSurfaceType();
          if (thisSubSurfaceType != adjacentSubSurfaceType) {
            // sub surfaces don't match, we could return false here if we wanted to
            // however, David's preference is that we attempt to resolve this
            std::string thisDefaultSubSurfaceType = this->defaultSubSurfaceType();

            // if subSurfaceType is set to something other than defaultSubSurfaceType assume the user wanted it that way
            // resolve conflicts giving preference to this subSurfaceType
            if (thisSubSurfaceType != thisDefaultSubSurfaceType) {
              // set other sub surface to this subSurfaceType
              subSurface.setSubSurfaceType(thisSubSurfaceType);
            } else {
              // must be the other surface that doesn't match its defaultSubSurfaceType
              // set this sub surface to other subSurfaceType
              this->setSubSurfaceType(adjacentSubSurfaceType);
            }
          }

          // reset any sub surfaces pointing to this
          this->resetAdjacentSubSurface();

          // if other subsurface is already matched to another sub surface, could be this one
          if (!isSameSubSurface) {
            subSurface.resetAdjacentSubSurface();
          }

          result = setPointer(OS_SubSurfaceFields::OutsideBoundaryConditionObject, subSurface.handle());
          OS_ASSERT(result);
          this->removeAllShadingControls();

          if (!isSameSubSurface) {
            result = subSurface.setPointer(OS_SubSurfaceFields::OutsideBoundaryConditionObject, this->handle());
            OS_ASSERT(result);
            subSurface.removeAllShadingControls();
          }
        }
      }

      return result;
    }

    void SubSurface_Impl::resetAdjacentSubSurface() {
      bool test = setString(OS_SubSurfaceFields::OutsideBoundaryConditionObject, "");
      OS_ASSERT(test);

      for (WorkspaceObject& wo : this->getSources(IddObjectType::OS_SubSurface)) {
        test = wo.setString(OS_SubSurfaceFields::OutsideBoundaryConditionObject, "");
        OS_ASSERT(test);
      }
    }

    boost::optional<SurfacePropertyConvectionCoefficients> SubSurface_Impl::surfacePropertyConvectionCoefficients() const {
      std::vector<SurfacePropertyConvectionCoefficients> spccs(model().getConcreteModelObjects<SurfacePropertyConvectionCoefficients>());
      auto thisHandle = this->handle();
      auto isNotPointingToMe = [&thisHandle](const auto& spcc) {
        OptionalSubSurface surface = spcc.surfaceAsSubSurface();
        return !surface || !(surface->handle() == thisHandle);
      };

      spccs.erase(std::remove_if(spccs.begin(), spccs.end(), isNotPointingToMe), spccs.end());

      if (spccs.empty()) {
        return boost::none;
      } else if (spccs.size() == 1) {
        return spccs.at(0);
      } else {
        LOG(Error, "More than one SurfacePropertyConvectionCoefficients points to this SubSurface");
        return boost::none;
      }
    }

    boost::optional<SurfacePropertyLocalEnvironment> SubSurface_Impl::surfacePropertyLocalEnvironment() const {

      std::vector<SurfacePropertyLocalEnvironment> result;
      for (auto& localEnv : model().getConcreteModelObjects<SurfacePropertyLocalEnvironment>()) {
        if (auto subSurface_ = localEnv.exteriorSurfaceAsSubSurface()) {
          if (subSurface_->handle() == handle()) {
            result.push_back(localEnv);
          }
        }
      }
      if (result.empty()) {
        return boost::none;
      } else if (result.size() == 1) {
        return result.at(0);
      } else {
        LOG(Error, "More than one SurfacePropertyLocalEnvironment points to this SubSurface");
        return boost::none;
      }
    }

    boost::optional<SurfacePropertyIncidentSolarMultiplier> SubSurface_Impl::surfacePropertyIncidentSolarMultiplier() const {
      std::vector<SurfacePropertyIncidentSolarMultiplier> allspism(model().getConcreteModelObjects<SurfacePropertyIncidentSolarMultiplier>());
      std::vector<SurfacePropertyIncidentSolarMultiplier> spism;
      for (auto& spcc : allspism) {
        if (spcc.subSurface().handle() == handle()) {
          spism.push_back(spcc);
        }
      }
      if (spism.empty()) {
        return boost::none;
      } else if (spism.size() == 1) {
        return spism.at(0);
      } else {
        LOG(Error, "More than one SurfacePropertyIncidentSolarMultiplier points to this SubSurface");
        return boost::none;
      }
    }

    boost::optional<SurfacePropertyOtherSideCoefficients> SubSurface_Impl::surfacePropertyOtherSideCoefficients() const {
      return getObject<SubSurface>().getModelObjectTarget<SurfacePropertyOtherSideCoefficients>(OS_SubSurfaceFields::OutsideBoundaryConditionObject);
    }

    bool SubSurface_Impl::setSurfacePropertyOtherSideCoefficients(const SurfacePropertyOtherSideCoefficients& otherSideCoefficients) {
      boost::optional<SubSurface> adjacentSubSurface = this->adjacentSubSurface();
      if (adjacentSubSurface) {
        resetAdjacentSubSurface();
      }

      // this is basically testing if otherSideCoefficients is in same model as this
      bool test = this->setPointer(OS_SubSurfaceFields::OutsideBoundaryConditionObject, otherSideCoefficients.handle());
      if (!test) {
        if (adjacentSubSurface) {
          setAdjacentSubSurface(*adjacentSubSurface);
        }
      }
      return test;
    }

    void SubSurface_Impl::resetSurfacePropertyOtherSideCoefficients() {
      bool test = setString(OS_SubSurfaceFields::OutsideBoundaryConditionObject, "");
      OS_ASSERT(test);
    }

    boost::optional<SurfacePropertyOtherSideConditionsModel> SubSurface_Impl::surfacePropertyOtherSideConditionsModel() const {
      return getObject<SubSurface>().getModelObjectTarget<SurfacePropertyOtherSideConditionsModel>(
        OS_SubSurfaceFields::OutsideBoundaryConditionObject);
    }

    bool SubSurface_Impl::setSurfacePropertyOtherSideConditionsModel(const SurfacePropertyOtherSideConditionsModel& otherSideModel) {
      boost::optional<SubSurface> adjacentSubSurface = this->adjacentSubSurface();
      if (adjacentSubSurface) {
        resetAdjacentSubSurface();
      }

      // this is basically testing if otherSideModel is in same model as this
      bool test = this->setPointer(OS_SubSurfaceFields::OutsideBoundaryConditionObject, otherSideModel.handle());
      if (!test) {
        if (adjacentSubSurface) {
          setAdjacentSubSurface(*adjacentSubSurface);
        }
      }
      return test;
    }

    void SubSurface_Impl::resetSurfacePropertyOtherSideConditionsModel() {
      bool test = setString(OS_SubSurfaceFields::OutsideBoundaryConditionObject, "");
      OS_ASSERT(test);
    }

    std::string SubSurface_Impl::defaultSubSurfaceType() const {
      std::string result;

      boost::optional<Surface> surface = this->surface();
      if (!surface) {
        double degTilt = radToDeg(this->tilt());
        if (degTilt < 60) {
          result = "Skylight";
        } else if (degTilt < 179) {
          result = "FixedWindow";
        } else {
          result = "Skylight";
        }
      } else {
        std::string surfaceType = surface->surfaceType();
        if (istringEqual("RoofCeiling", surfaceType) || istringEqual("Floor", surfaceType)) {
          result = "Skylight";
        } else {
          double surfaceMinZ = std::numeric_limits<double>::max();
          for (const Point3d& point : surface->vertices()) {
            surfaceMinZ = std::min(surfaceMinZ, point.z());
          }

          double thisMinZ = std::numeric_limits<double>::max();
          for (const Point3d& point : this->vertices()) {
            thisMinZ = std::min(thisMinZ, point.z());
          }

          if (thisMinZ <= surfaceMinZ) {
            bool isGlassDoor = false;

            // DLM: this surface could have been initialized to FixedWindow and get its construction
            // from the default construction set, this was the source of #1924
            boost::optional<ConstructionBase> construction = this->construction();
            if (!this->isConstructionDefaulted() && construction && construction->isFenestration()) {
              isGlassDoor = true;
            }

            boost::optional<std::string> value = getString(OS_SubSurfaceFields::SubSurfaceType);
            if (value && istringEqual("GlassDoor", *value)) {
              isGlassDoor = true;
            }

            if (isGlassDoor) {
              result = "GlassDoor";
            } else {
              result = "Door";
            }
          } else {
            result = "FixedWindow";
          }
        }
      }

      return result;
    }

    void SubSurface_Impl::assignDefaultSubSurfaceType() {
      std::string defaultSubSurfaceType = this->defaultSubSurfaceType();
      bool test = setSubSurfaceType(defaultSubSurfaceType);
      OS_ASSERT(test);
    }

    std::string SubSurface_Impl::outsideBoundaryCondition() const {
      std::string result;
      OptionalSurface oSurface = surface();
      if (oSurface) {
        result = oSurface->outsideBoundaryCondition();
      }
      return result;
    }

    boost::optional<ShadingSurface> SubSurface_Impl::addOverhang(double depth, double offset) {
      std::string subSurfaceType = this->subSurfaceType();
      if (!(istringEqual("FixedWindow", subSurfaceType) || istringEqual("OperableWindow", subSurfaceType)
            || istringEqual("GlassDoor", subSurfaceType))) {
        return boost::none;
      }

      Model model = this->model();
      boost::optional<Space> space = this->space();
      boost::optional<ShadingSurface> shadingSurface;
      if (space) {

        Point3dVector vertices = this->vertices();
        Transformation transformation = Transformation::alignFace(vertices);
        Point3dVector faceVertices = transformation.inverse() * vertices;

        // new coordinate system has z' in direction of outward normal, y' is up
        double xmin = std::numeric_limits<double>::max();
        double xmax = std::numeric_limits<double>::min();
        double ymin = std::numeric_limits<double>::max();
        double ymax = std::numeric_limits<double>::min();
        for (const Point3d& faceVertex : faceVertices) {
          xmin = std::min(xmin, faceVertex.x());
          xmax = std::max(xmax, faceVertex.x());
          ymin = std::min(ymin, faceVertex.y());
          ymax = std::max(ymax, faceVertex.y());
        }
        if ((xmin > xmax) || (ymin > ymax)) {
          return boost::none;
        }

        Point3dVector overhangVertices{
          {xmax + offset, ymax + offset, 0},
          {xmin - offset, ymax + offset, 0},
          {xmin - offset, ymax + offset, depth},
          {xmax + offset, ymax + offset, depth},
        };

        ShadingSurfaceGroup shadingSurfaceGroup(model);
        shadingSurfaceGroup.setName(this->name().get() + " Shading Surfaces");
        shadingSurfaceGroup.setSpace(*space);

        shadingSurface = ShadingSurface(transformation * overhangVertices, model);
        shadingSurface->setShadingSurfaceGroup(shadingSurfaceGroup);

        shadingSurfaceGroup.setShadedSubSurface(getObject<SubSurface>());
      }

      return shadingSurface;
    }

    boost::optional<ShadingSurface> SubSurface_Impl::addOverhangByProjectionFactor(double projectionFactor, double offsetFraction) {
      std::string subSurfaceType = this->subSurfaceType();
      if (!(istringEqual("FixedWindow", subSurfaceType) || istringEqual("OperableWindow", subSurfaceType)
            || istringEqual("GlassDoor", subSurfaceType))) {
        return boost::none;
      }

      Model model = this->model();
      boost::optional<Space> space = this->space();
      boost::optional<ShadingSurface> shadingSurface;
      if (space) {

        Point3dVector vertices = this->vertices();
        Transformation transformation = Transformation::alignFace(vertices);
        Point3dVector faceVertices = transformation.inverse() * vertices;

        // new coordinate system has z' in direction of outward normal, y' is up
        double xmin = std::numeric_limits<double>::max();
        double xmax = std::numeric_limits<double>::min();
        double ymin = std::numeric_limits<double>::max();
        double ymax = std::numeric_limits<double>::min();
        for (const Point3d& faceVertex : faceVertices) {
          xmin = std::min(xmin, faceVertex.x());
          xmax = std::max(xmax, faceVertex.x());
          ymin = std::min(ymin, faceVertex.y());
          ymax = std::max(ymax, faceVertex.y());
        }
        if ((xmin > xmax) || (ymin > ymax)) {
          return boost::none;
        }

        double offset = offsetFraction * (ymax - ymin);
        double depth = projectionFactor * (offset + (ymax - ymin));

        Point3dVector overhangVertices{
          {xmax + offset, ymax + offset, 0},
          {xmin - offset, ymax + offset, 0},
          {xmin - offset, ymax + offset, depth},
          {xmax + offset, ymax + offset, depth},
        };

        ShadingSurfaceGroup shadingSurfaceGroup(model);
        shadingSurfaceGroup.setName(this->name().get() + " Shading Surfaces");
        shadingSurfaceGroup.setSpace(*space);

        shadingSurface = ShadingSurface(transformation * overhangVertices, model);
        shadingSurface->setShadingSurfaceGroup(shadingSurfaceGroup);

        shadingSurfaceGroup.setShadedSubSurface(getObject<SubSurface>());
      }

      return shadingSurface;
    }

    std::vector<ShadingSurfaceGroup> SubSurface_Impl::shadingSurfaceGroups() const {
      return getObject<ModelObject>().getModelObjectSources<ShadingSurfaceGroup>(ShadingSurfaceGroup::iddObjectType());
    }

    bool SubSurface_Impl::allowDaylightingDeviceShelf() const {
      bool result = false;

      std::string subSurfaceType = this->subSurfaceType();
      if (istringEqual(subSurfaceType, "FixedWindow") || istringEqual(subSurfaceType, "OperableWindow")
          || istringEqual(subSurfaceType, "GlassDoor")) {
        result = true;
      }

      return result;
    }

    boost::optional<DaylightingDeviceShelf> SubSurface_Impl::daylightingDeviceShelf() const {
      boost::optional<DaylightingDeviceShelf> result;

      std::vector<DaylightingDeviceShelf> shelves =
        getObject<ModelObject>().getModelObjectSources<DaylightingDeviceShelf>(DaylightingDeviceShelf::iddObjectType());
      if (shelves.size() == 1) {
        result = shelves[0];
      } else if (shelves.size() > 1) {
        result = shelves[0];
      }
      return result;
    }

    boost::optional<DaylightingDeviceShelf> SubSurface_Impl::addDaylightingDeviceShelf() const {
      boost::optional<DaylightingDeviceShelf> result = this->daylightingDeviceShelf();
      if (result) {
        return result;
      }

      if (allowDaylightingDeviceShelf()) {
        try {
          result = DaylightingDeviceShelf(getObject<SubSurface>());
        } catch (const std::exception&) {
        }
      }

      return result;
    }

    bool SubSurface_Impl::allowDaylightingDeviceTubularDome() const {
      bool result = false;

      std::string subSurfaceType = this->subSurfaceType();
      if (istringEqual(subSurfaceType, "TubularDaylightDome")) {
        result = true;
      }

      return result;
    }

    bool SubSurface_Impl::allowDaylightingDeviceTubularDiffuser() const {
      bool result = false;

      std::string subSurfaceType = this->subSurfaceType();
      if (istringEqual(subSurfaceType, "TubularDaylightDiffuser")) {
        result = true;
      }

      return result;
    }

    boost::optional<DaylightingDeviceTubular> SubSurface_Impl::daylightingDeviceTubular() const {
      boost::optional<DaylightingDeviceTubular> result;

      std::vector<DaylightingDeviceTubular> tubulars =
        getObject<ModelObject>().getModelObjectSources<DaylightingDeviceTubular>(DaylightingDeviceTubular::iddObjectType());
      if (tubulars.size() == 1) {
        result = tubulars[0];
      } else if (tubulars.size() > 1) {
        result = tubulars[0];
      }
      return result;
    }

    bool SubSurface_Impl::allowDaylightingDeviceLightWell() const {
      bool result = false;

      std::string subSurfaceType = this->subSurfaceType();
      if (istringEqual(subSurfaceType, "FixedWindow") || istringEqual(subSurfaceType, "OperableWindow")
          || istringEqual(subSurfaceType, "GlassDoor")) {
        result = true;
      }

      return result;
    }

    boost::optional<DaylightingDeviceLightWell> SubSurface_Impl::daylightingDeviceLightWell() const {
      boost::optional<DaylightingDeviceLightWell> result;

      std::vector<DaylightingDeviceLightWell> lightwells =
        getObject<ModelObject>().getModelObjectSources<DaylightingDeviceLightWell>(DaylightingDeviceLightWell::iddObjectType());
      if (lightwells.size() == 1) {
        result = lightwells[0];
      } else if (lightwells.size() > 1) {
        result = lightwells[0];
      }
      return result;
    }

    boost::optional<DaylightingDeviceLightWell> SubSurface_Impl::addDaylightingDeviceLightWell() const {
      boost::optional<DaylightingDeviceLightWell> result = this->daylightingDeviceLightWell();
      if (result) {
        return result;
      }

      if (allowDaylightingDeviceLightWell()) {
        try {
          result = DaylightingDeviceLightWell(getObject<SubSurface>());
        } catch (const std::exception&) {
        }
      }

      return result;
    }

    boost::optional<ModelObject> SubSurface_Impl::surfaceAsModelObject() const {
      OptionalModelObject result;
      OptionalSurface intermediate = surface();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> SubSurface_Impl::adjacentSubSurfaceAsModelObject() const {
      OptionalModelObject result;
      OptionalSubSurface intermediate = adjacentSubSurface();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool SubSurface_Impl::setSurfaceAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSurface intermediate = modelObject->optionalCast<Surface>();
        if (intermediate) {
          return setSurface(*intermediate);
        }
      }
      return false;
    }

    bool SubSurface_Impl::setAdjacentSubSurfaceAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSubSurface intermediate = modelObject->optionalCast<SubSurface>();
        if (intermediate) {
          return setAdjacentSubSurface(*intermediate);
        } else {
          return false;
        }
      } else {
        resetAdjacentSubSurface();
      }
      return true;
    }

    AirflowNetworkSurface SubSurface_Impl::getAirflowNetworkSurface(const AirflowNetworkComponent& surfaceAirflowLeakage) {
      boost::optional<AirflowNetworkSurface> result = airflowNetworkSurface();
      if (result) {
        boost::optional<AirflowNetworkComponent> leakageComponent = result->leakageComponent();
        if (leakageComponent) {
          if (leakageComponent->handle() == surfaceAirflowLeakage.handle()) {
            return result.get();
          } else {
            result->remove();
          }
        } else {
          result->remove();
        }
      }

      return AirflowNetworkSurface(this->model(), surfaceAirflowLeakage.handle(), this->handle());
    }

    boost::optional<AirflowNetworkSurface> SubSurface_Impl::airflowNetworkSurface() const {
      std::vector<AirflowNetworkSurface> myAFNSurfs =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkSurface>(AirflowNetworkSurface::iddObjectType());
      boost::optional<SubSurface> other = adjacentSubSurface();
      if (other) {
        std::vector<AirflowNetworkSurface> adjAFNSurfs =
          other.get().getImpl<detail::SubSurface_Impl>()->getObject<ModelObject>().getModelObjectSources<AirflowNetworkSurface>(
            AirflowNetworkSurface::iddObjectType());
        myAFNSurfs.insert(myAFNSurfs.end(), adjAFNSurfs.begin(), adjAFNSurfs.end());
      }
      auto count = myAFNSurfs.size();
      if (count == 1) {
        return myAFNSurfs[0];
      } else if (count > 1) {
        LOG(Warn, briefDescription() << " has more than one AirflowNetwork Surface attached, returning first.");
        return myAFNSurfs[0];
      }
      return boost::none;
    }

    boost::optional<double> SubSurface_Impl::assemblyUFactor() const {
      return getExteriorFenestrationValue("Assembly U-Factor");
    }

    boost::optional<double> SubSurface_Impl::assemblySHGC() const {
      return getExteriorFenestrationValue("Assembly SHGC");
    }

    boost::optional<double> SubSurface_Impl::assemblyVisibleTransmittance() const {
      return getExteriorFenestrationValue("Assembly Visible Transmittance");
    }

    /** Gets the fenestration value from the sql file **/
    boost::optional<double> SubSurface_Impl::getExteriorFenestrationValue(std::string columnName) const {
      boost::optional<double> result;

      // Get the object name
      if (!name()) {
        LOG(Warn, "This object does not have a name, cannot retrieve the fenestration value '" + columnName + "'.");
        return result;
      }

      // Get the object name and transform to the way it is recorded
      // in the sql file
      std::string rowName = name().get();
      boost::to_upper(rowName);

      // Check that the model has a sql file
      if (!model().sqlFile()) {
        LOG(Warn, "This model has no sql file, cannot retrieve the fenestration value '" + columnName + "'.");
        return result;
      }

      std::string s = R"(SELECT Value FROM TabularDataWithStrings
                            WHERE ReportName='EnvelopeSummary'
                            AND ReportForString='Entire Facility'
                            AND TableName='Exterior Fenestration'
                            AND RowName=?
                            AND ColumnName=?)";

      result = model().sqlFile().get().execAndReturnFirstDouble(s, rowName, columnName);

      return result;
    }

  }  // namespace detail

  SubSurface::SubSurface(const std::vector<Point3d>& vertices, const Model& model) : PlanarSurface(SubSurface::iddObjectType(), vertices, model) {
    OS_ASSERT(getImpl<detail::SubSurface_Impl>());
    resetSubSurfaceType();
  }

  IddObjectType SubSurface::iddObjectType() {
    IddObjectType result(IddObjectType::OS_SubSurface);
    return result;
  }

  std::vector<std::string> SubSurface::validSubSurfaceTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SubSurfaceFields::SubSurfaceType);
  }

  std::string SubSurface::subSurfaceType() const {
    return getImpl<detail::SubSurface_Impl>()->subSurfaceType();
  }

  bool SubSurface::isSubSurfaceTypeDefaulted() const {
    return getImpl<detail::SubSurface_Impl>()->isSubSurfaceTypeDefaulted();
  }

  boost::optional<double> SubSurface::viewFactortoGround() const {
    return getImpl<detail::SubSurface_Impl>()->viewFactortoGround();
  }

  bool SubSurface::isViewFactortoGroundDefaulted() const {
    return getImpl<detail::SubSurface_Impl>()->isViewFactortoGroundDefaulted();
  }

  bool SubSurface::isViewFactortoGroundAutocalculated() const {
    return getImpl<detail::SubSurface_Impl>()->isViewFactortoGroundAutocalculated();
  }

  bool SubSurface::allowShadingControl() const {
    return getImpl<detail::SubSurface_Impl>()->allowShadingControl();
  }

  std::vector<ShadingControl> SubSurface::shadingControls() const {
    return getImpl<detail::SubSurface_Impl>()->shadingControls();
  }

  unsigned int SubSurface::numberofShadingControls() const {
    return getImpl<detail::SubSurface_Impl>()->numberofShadingControls();
  }

  bool SubSurface::allowWindowPropertyFrameAndDivider() const {
    return getImpl<detail::SubSurface_Impl>()->allowWindowPropertyFrameAndDivider();
  }

  boost::optional<WindowPropertyFrameAndDivider> SubSurface::windowPropertyFrameAndDivider() const {
    return getImpl<detail::SubSurface_Impl>()->windowPropertyFrameAndDivider();
  }

  double SubSurface::multiplier() const {
    return getImpl<detail::SubSurface_Impl>()->multiplier();
  }

  bool SubSurface::isMultiplierDefaulted() const {
    return getImpl<detail::SubSurface_Impl>()->isMultiplierDefaulted();
  }

  boost::optional<double> SubSurface::numberofVertices() const {
    return getImpl<detail::SubSurface_Impl>()->numberofVertices();
  }

  bool SubSurface::isNumberofVerticesDefaulted() const {
    return getImpl<detail::SubSurface_Impl>()->isNumberofVerticesDefaulted();
  }

  bool SubSurface::isNumberofVerticesAutocalculated() const {
    return getImpl<detail::SubSurface_Impl>()->isNumberofVerticesAutocalculated();
  }

  bool SubSurface::setSubSurfaceType(const std::string& subSurfaceType) {
    return getImpl<detail::SubSurface_Impl>()->setSubSurfaceType(subSurfaceType);
  }

  void SubSurface::resetSubSurfaceType() {
    getImpl<detail::SubSurface_Impl>()->resetSubSurfaceType();
  }

  bool SubSurface::setViewFactortoGround(boost::optional<double> viewFactortoGround) {
    return getImpl<detail::SubSurface_Impl>()->setViewFactortoGround(viewFactortoGround);
  }

  bool SubSurface::setViewFactortoGround(double viewFactortoGround) {
    return getImpl<detail::SubSurface_Impl>()->setViewFactortoGround(viewFactortoGround);
  }

  void SubSurface::resetViewFactortoGround() {
    getImpl<detail::SubSurface_Impl>()->resetViewFactortoGround();
  }

  void SubSurface::autocalculateViewFactortoGround() {
    getImpl<detail::SubSurface_Impl>()->autocalculateViewFactortoGround();
  }

  bool SubSurface::setMultiplier(double multiplier) {
    return getImpl<detail::SubSurface_Impl>()->setMultiplier(multiplier);
  }

  void SubSurface::resetMultiplier() {
    getImpl<detail::SubSurface_Impl>()->resetMultiplier();
  }

  bool SubSurface::setNumberofVertices(boost::optional<double> numberofVertices) {
    return getImpl<detail::SubSurface_Impl>()->setNumberofVertices(numberofVertices);
  }

  bool SubSurface::setNumberofVertices(double numberofVertices) {
    return getImpl<detail::SubSurface_Impl>()->setNumberofVertices(numberofVertices);
  }

  void SubSurface::resetNumberofVertices() {
    getImpl<detail::SubSurface_Impl>()->resetNumberofVertices();
  }

  void SubSurface::autocalculateNumberofVertices() {
    getImpl<detail::SubSurface_Impl>()->autocalculateNumberofVertices();
  }

  bool SubSurface::addShadingControl(ShadingControl& shadingControl) {
    return getImpl<detail::SubSurface_Impl>()->addShadingControl(shadingControl);
  }

  bool SubSurface::addShadingControls(std::vector<ShadingControl>& shadingControls) {
    return getImpl<detail::SubSurface_Impl>()->addShadingControls(shadingControls);
  }

  void SubSurface::removeShadingControl(ShadingControl& shadingControl) {
    getImpl<detail::SubSurface_Impl>()->removeShadingControl(shadingControl);
  }

  void SubSurface::removeAllShadingControls() {
    getImpl<detail::SubSurface_Impl>()->removeAllShadingControls();
  }

  bool SubSurface::setWindowPropertyFrameAndDivider(const WindowPropertyFrameAndDivider& windowPropertyFrameAndDivider) {
    return getImpl<detail::SubSurface_Impl>()->setWindowPropertyFrameAndDivider(windowPropertyFrameAndDivider);
  }

  void SubSurface::resetWindowPropertyFrameAndDivider() {
    getImpl<detail::SubSurface_Impl>()->resetWindowPropertyFrameAndDivider();
  }

  boost::optional<Surface> SubSurface::surface() const {
    return getImpl<detail::SubSurface_Impl>()->surface();
  }

  bool SubSurface::setSurface(const Surface& surface) {
    return getImpl<detail::SubSurface_Impl>()->setSurface(surface);
  }

  boost::optional<SubSurface> SubSurface::adjacentSubSurface() const {
    return getImpl<detail::SubSurface_Impl>()->adjacentSubSurface();
  }

  bool SubSurface::setAdjacentSubSurface(SubSurface& subSurface) {
    return getImpl<detail::SubSurface_Impl>()->setAdjacentSubSurface(subSurface);
  }

  void SubSurface::resetAdjacentSubSurface() {
    getImpl<detail::SubSurface_Impl>()->resetAdjacentSubSurface();
  }

  boost::optional<SurfacePropertyConvectionCoefficients> SubSurface::surfacePropertyConvectionCoefficients() const {
    return getImpl<detail::SubSurface_Impl>()->surfacePropertyConvectionCoefficients();
  }

  boost::optional<SurfacePropertyLocalEnvironment> SubSurface::surfacePropertyLocalEnvironment() const {
    return getImpl<detail::SubSurface_Impl>()->surfacePropertyLocalEnvironment();
  }

  boost::optional<SurfacePropertyIncidentSolarMultiplier> SubSurface::surfacePropertyIncidentSolarMultiplier() const {
    return getImpl<detail::SubSurface_Impl>()->surfacePropertyIncidentSolarMultiplier();
  }

  boost::optional<SurfacePropertyOtherSideCoefficients> SubSurface::surfacePropertyOtherSideCoefficients() const {
    return getImpl<detail::SubSurface_Impl>()->surfacePropertyOtherSideCoefficients();
  }

  bool SubSurface::setSurfacePropertyOtherSideCoefficients(const SurfacePropertyOtherSideCoefficients& otherSideCoefficients) {
    return getImpl<detail::SubSurface_Impl>()->setSurfacePropertyOtherSideCoefficients(otherSideCoefficients);
  }

  void SubSurface::resetSurfacePropertyOtherSideCoefficients() {
    return getImpl<detail::SubSurface_Impl>()->resetSurfacePropertyOtherSideCoefficients();
  }

  boost::optional<SurfacePropertyOtherSideConditionsModel> SubSurface::surfacePropertyOtherSideConditionsModel() const {
    return getImpl<detail::SubSurface_Impl>()->surfacePropertyOtherSideConditionsModel();
  }

  bool SubSurface::setSurfacePropertyOtherSideConditionsModel(const SurfacePropertyOtherSideConditionsModel& otherSideModel) {
    return getImpl<detail::SubSurface_Impl>()->setSurfacePropertyOtherSideConditionsModel(otherSideModel);
  }

  void SubSurface::resetSurfacePropertyOtherSideConditionsModel() {
    return getImpl<detail::SubSurface_Impl>()->resetSurfacePropertyOtherSideConditionsModel();
  }

  void SubSurface::assignDefaultSubSurfaceType() {
    getImpl<detail::SubSurface_Impl>()->assignDefaultSubSurfaceType();
  }

  std::string SubSurface::outsideBoundaryCondition() const {
    return getImpl<detail::SubSurface_Impl>()->outsideBoundaryCondition();
  }

  boost::optional<ShadingSurface> SubSurface::addOverhang(double depth, double offset) {
    return getImpl<detail::SubSurface_Impl>()->addOverhang(depth, offset);
  }

  boost::optional<ShadingSurface> SubSurface::addOverhangByProjectionFactor(double projectionFactor, double offsetFraction) {
    return getImpl<detail::SubSurface_Impl>()->addOverhangByProjectionFactor(projectionFactor, offsetFraction);
  }

  std::vector<ShadingSurfaceGroup> SubSurface::shadingSurfaceGroups() const {
    return getImpl<detail::SubSurface_Impl>()->shadingSurfaceGroups();
  }

  bool SubSurface::allowDaylightingDeviceShelf() const {
    return getImpl<detail::SubSurface_Impl>()->allowDaylightingDeviceShelf();
  }

  boost::optional<DaylightingDeviceShelf> SubSurface::daylightingDeviceShelf() const {
    return getImpl<detail::SubSurface_Impl>()->daylightingDeviceShelf();
  }

  boost::optional<DaylightingDeviceShelf> SubSurface::addDaylightingDeviceShelf() const {
    return getImpl<detail::SubSurface_Impl>()->addDaylightingDeviceShelf();
  }

  bool SubSurface::allowDaylightingDeviceTubularDome() const {
    return getImpl<detail::SubSurface_Impl>()->allowDaylightingDeviceTubularDome();
  }

  bool SubSurface::allowDaylightingDeviceTubularDiffuser() const {
    return getImpl<detail::SubSurface_Impl>()->allowDaylightingDeviceTubularDiffuser();
  }

  boost::optional<DaylightingDeviceTubular> SubSurface::daylightingDeviceTubular() const {
    return getImpl<detail::SubSurface_Impl>()->daylightingDeviceTubular();
  }

  bool SubSurface::allowDaylightingDeviceLightWell() const {
    return getImpl<detail::SubSurface_Impl>()->allowDaylightingDeviceLightWell();
  }

  boost::optional<DaylightingDeviceLightWell> SubSurface::daylightingDeviceLightWell() const {
    return getImpl<detail::SubSurface_Impl>()->daylightingDeviceLightWell();
  }

  boost::optional<DaylightingDeviceLightWell> SubSurface::addDaylightingDeviceLightWell() const {
    return getImpl<detail::SubSurface_Impl>()->addDaylightingDeviceLightWell();
  }

  /// @cond
  SubSurface::SubSurface(std::shared_ptr<detail::SubSurface_Impl> impl) : PlanarSurface(std::move(impl)) {}

  // DEPRECATED
  boost::optional<ShadingControl> SubSurface::shadingControl() const {
    boost::optional<ShadingControl> result;
    auto scs = shadingControls();
    if (!scs.empty()) {
      if (scs.size() > 1) {
        LOG(Warn, briefDescription() << " has more than one ShadingControl and you're using a deprecated method. Use shadingControls() instead");
      }
      result = scs[0];
    }
    return result;
  }

  // DEPRECATED
  bool SubSurface::setShadingControl(const ShadingControl& shadingControl) {
    removeAllShadingControls();
    return addShadingControl(const_cast<ShadingControl&>(shadingControl));
  }

  // DEPRECATED
  void SubSurface::resetShadingControl() {
    removeAllShadingControls();
  }

  std::vector<Point3d> SubSurface::roughOpeningVertices() const {
    if (auto frameAndDivider = windowPropertyFrameAndDivider()) {
      double fw = frameAndDivider->frameWidth();
      // Get a transform to change the points to x/y
      Transformation faceTransform = Transformation::alignFace(this->vertices());
      std::vector<Point3d> faceVertices = faceTransform.inverse() * this->vertices();
      // Offset the points by the framewidth
      boost::optional<std::vector<Point3d>> offset = openstudio::buffer(faceVertices, fw, 0.01);
      if (!offset) {
        // If offset failed it is because the points are in the wrong order
        // If it fails again then something went awry with boost::buffer
        faceVertices = openstudio::reverse(faceVertices);
        offset = openstudio::buffer(faceVertices, fw, 0.01);
        if (!offset) {
          return this->vertices();
        }
      }

      std::vector<Point3d> roughOpeningVertices = faceTransform * offset.get();
      return roughOpeningVertices;
    }

    return this->vertices();
  }

  double SubSurface::roughOpeningArea() const {
    if (boost::optional<double> area = openstudio::getArea(roughOpeningVertices())) {
      return area.get();
    }

    return grossArea();
  }

  double SubSurface::frameArea() const {
    double roughOpeningArea = this->roughOpeningArea();
    return roughOpeningArea - grossArea();
  }

  double SubSurface::dividerArea() const {
    double divArea = 0;
    if (auto frameAndDivider = windowPropertyFrameAndDivider()) {
      double dividerWidth = frameAndDivider->dividerWidth();
      if (dividerWidth == 0) {
        return divArea;
      }

      Transformation faceTransform = Transformation::alignFace(this->vertices());
      std::vector<Point3d> faceVertices = faceTransform.inverse() * this->vertices();
      BoundingBox bb;
      bb.addPoints(faceVertices);

      double numHorizDividers = frameAndDivider->numberOfHorizontalDividers();
      if (numHorizDividers != 0 && bb.maxX().has_value() && bb.minX().has_value()) {
        divArea += numHorizDividers * dividerWidth * (*bb.maxX() - *bb.minX());
      }
      double numVertDividers = frameAndDivider->numberOfVerticalDividers();
      if (numVertDividers != 0 && bb.maxY().has_value() && bb.minY().has_value()) {
        divArea += numVertDividers * dividerWidth * (*bb.maxY() - *bb.minY());
      }
    }
    return divArea;
  }

  /// @endcond

  std::vector<SubSurface> applySkylightPattern(const std::vector<std::vector<Point3d>>& pattern, const std::vector<Space>& spaces,
                                               const boost::optional<ConstructionBase>& construction) {
    double inset = 0.0254;

    std::vector<SubSurface> result;

    for (const Space& space : spaces) {

      if (space.isPlenum()) {
        LOG_FREE(Warn, "OpenStudio.applySkylightPattern", "Cannot apply skylights to plenum space");
        continue;
      }

      Transformation transformation = space.buildingTransformation();
      Transformation inverseTransformation = transformation.inverse();

      std::vector<std::vector<Point3d>> spacePattern;
      spacePattern.reserve(pattern.size());
      for (const std::vector<Point3d>& face : pattern) {
        spacePattern.push_back(inverseTransformation * face);
      }

      for (Surface& surface : space.surfaces()) {
        if (istringEqual("RoofCeiling", surface.surfaceType()) && istringEqual("Outdoors", surface.outsideBoundaryCondition())) {

          Plane surfacePlane = surface.plane();

          std::vector<std::vector<Point3d>> surfacePattern;
          spacePattern.reserve(pattern.size());
          for (const std::vector<Point3d>& spaceFace : spacePattern) {
            surfacePattern.push_back(surfacePlane.project(spaceFace));
          }

          std::vector<SubSurface> newSkylights = surface.createSubSurfaces(surfacePattern, inset, construction);
          result.insert(result.end(), newSkylights.begin(), newSkylights.end());
        }
      }
    }

    return result;
  }

  AirflowNetworkSurface SubSurface::getAirflowNetworkSurface(const AirflowNetworkDetailedOpening& surfaceAirflowLeakage) {
    return getImpl<detail::SubSurface_Impl>()->getAirflowNetworkSurface(surfaceAirflowLeakage);
  }

  AirflowNetworkSurface SubSurface::getAirflowNetworkSurface(const AirflowNetworkSimpleOpening& surfaceAirflowLeakage) {
    return getImpl<detail::SubSurface_Impl>()->getAirflowNetworkSurface(surfaceAirflowLeakage);
  }

  AirflowNetworkSurface SubSurface::getAirflowNetworkSurface(const AirflowNetworkCrack& surfaceAirflowLeakage) {
    return getImpl<detail::SubSurface_Impl>()->getAirflowNetworkSurface(surfaceAirflowLeakage);
  }

  AirflowNetworkSurface SubSurface::getAirflowNetworkSurface(const AirflowNetworkEffectiveLeakageArea& surfaceAirflowLeakage) {
    return getImpl<detail::SubSurface_Impl>()->getAirflowNetworkSurface(surfaceAirflowLeakage);
  }

  AirflowNetworkSurface SubSurface::getAirflowNetworkSurface(const AirflowNetworkHorizontalOpening& surfaceAirflowLeakage) {
    return getImpl<detail::SubSurface_Impl>()->getAirflowNetworkSurface(surfaceAirflowLeakage);
  }

  AirflowNetworkSurface SubSurface::getAirflowNetworkSurface(const AirflowNetworkSpecifiedFlowRate& surfaceAirflowLeakage) {
    return getImpl<detail::SubSurface_Impl>()->getAirflowNetworkSurface(surfaceAirflowLeakage);
  }

  boost::optional<AirflowNetworkSurface> SubSurface::airflowNetworkSurface() const {
    return getImpl<detail::SubSurface_Impl>()->airflowNetworkSurface();
  }

  boost::optional<double> SubSurface::assemblyUFactor() const {
    return getImpl<detail::SubSurface_Impl>()->assemblyUFactor();
  }

  boost::optional<double> SubSurface::assemblySHGC() const {
    return getImpl<detail::SubSurface_Impl>()->assemblySHGC();
  }

  boost::optional<double> SubSurface::assemblyVisibleTransmittance() const {
    return getImpl<detail::SubSurface_Impl>()->assemblyVisibleTransmittance();
  }

}  // namespace model
}  // namespace openstudio
