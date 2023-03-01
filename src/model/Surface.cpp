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

#include "Surface.hpp"
#include "Surface_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "ShadingSurfaceGroup.hpp"
#include "ShadingSurfaceGroup_Impl.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "LayeredConstruction.hpp"
#include "LayeredConstruction_Impl.hpp"
#include "StandardsInformationConstruction.hpp"
#include "DaylightingDeviceShelf.hpp"
#include "ShadingSurface.hpp"
#include "InteriorPartitionSurfaceGroup.hpp"
#include "InteriorPartitionSurface.hpp"
#include "SurfaceControlMovableInsulation.hpp"
#include "SurfaceControlMovableInsulation_Impl.hpp"
#include "SurfacePropertyOtherSideCoefficients.hpp"
#include "SurfacePropertyOtherSideCoefficients_Impl.hpp"
#include "SurfacePropertyOtherSideConditionsModel.hpp"
#include "SurfacePropertyOtherSideConditionsModel_Impl.hpp"
#include "SurfacePropertyConvectionCoefficients.hpp"
#include "SurfacePropertyConvectionCoefficients_Impl.hpp"
#include "SurfacePropertyLocalEnvironment.hpp"
#include "SurfacePropertyLocalEnvironment_Impl.hpp"
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
#include "FoundationKiva.hpp"
#include "FoundationKiva_Impl.hpp"
#include "FoundationKivaSettings.hpp"
#include "FoundationKivaSettings_Impl.hpp"
#include "SurfacePropertyExposedFoundationPerimeter.hpp"
#include "SurfacePropertyExposedFoundationPerimeter_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Surface_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/geometry/Intersection.hpp"
#include "../utilities/core/Assert.hpp"

#include "../utilities/sql/SqlFile.hpp"

#include <iterator>  // std::make_move_iterator
#include <numeric>   // std::accumulate

using boost::to_upper_copy;

namespace openstudio {
namespace model {

  namespace detail {

    Surface_Impl::Surface_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : PlanarSurface_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == Surface::iddObjectType());
    }

    Surface_Impl::Surface_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : PlanarSurface_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == Surface::iddObjectType());
    }

    Surface_Impl::Surface_Impl(const Surface_Impl& other, Model_Impl* model, bool keepHandle) : PlanarSurface_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> Surface_Impl::parent() const {
      boost::optional<ParentObject> result;
      result = this->space();
      return result;
    }

    bool Surface_Impl::setParent(ParentObject& newParent) {
      bool result = false;
      if (newParent.optionalCast<Space>()) {
        result = this->setSpace(newParent.cast<Space>());
      }
      return result;
    }

    std::vector<ModelObject> Surface_Impl::children() const {
      std::vector<ModelObject> result;

      // subSurfaces
      SubSurfaceVector subSurfaces = this->subSurfaces();
      result.insert(result.end(), subSurfaces.begin(), subSurfaces.end());

      std::vector<AirflowNetworkSurface> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkSurface>(AirflowNetworkSurface::iddObjectType());
      result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());

      if (boost::optional<SurfacePropertyExposedFoundationPerimeter> prop = this->surfacePropertyExposedFoundationPerimeter()) {
        result.push_back(prop.get());
      }

      return result;
    }

    std::vector<IdfObject> Surface_Impl::remove() {
      boost::optional<Surface> adjacentSurface = this->adjacentSurface();
      if (adjacentSurface) {
        this->resetAdjacentSurface();
      }

      return ParentObject_Impl::remove();
    }

    ModelObject Surface_Impl::clone(Model model) const {
      ModelObject newParentAsModelObject = ModelObject_Impl::clone(model);
      auto newParent = newParentAsModelObject.cast<ParentObject>();

      for (const ModelObject& child : children()) {
        ModelObject newChild = child.clone(model);
        newChild.setParent(newParent);
        if (auto ss_ = child.optionalCast<SubSurface>()) {
          newChild.cast<SubSurface>().setSubSurfaceType(ss_->subSurfaceType());
        }
      }

      auto coefficients = surfacePropertyConvectionCoefficients();
      if (coefficients) {
        auto coefficientsClone = coefficients->clone(model).cast<SurfacePropertyConvectionCoefficients>();
        coefficientsClone.setSurface(newParentAsModelObject);
      }

      // TODO: do we clone the SurfacePropertyLocalEnvironment?

      auto foundation = adjacentFoundation();
      if (foundation) {
        auto foundationClone = foundation->clone(model).cast<FoundationKiva>();
        newParentAsModelObject.cast<Surface>().setAdjacentFoundation(foundationClone);
      }

      return newParentAsModelObject;
    }

    std::vector<IddObjectType> Surface_Impl::allowableChildTypes() const {
      return std::vector<IddObjectType>{IddObjectType::OS_SubSurface};
    }

    const std::vector<std::string>& Surface_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Surface Inside Face Temperature", "Surface Outside Face Temperature"};
      return result;
    }

    IddObjectType Surface_Impl::iddObjectType() const {
      return Surface::iddObjectType();
    }

    bool Surface_Impl::subtractFromGrossArea() const {
      return false;
    }

    boost::optional<ConstructionBase> Surface_Impl::construction() const {
      // DLM: there is duplicate code in ForwardTranslator::resolveMatchedSurfaceConstructionConflicts
      // if you update this code you must update that code

      // DLM: I am not sure we should be doing this here at all, maybe this method should just
      // return the same thing constructionWithSearchDistance does?

      boost::optional<std::pair<ConstructionBase, int>> constructionWithSearchDistance = this->constructionWithSearchDistance();

      model::OptionalSurface adjacentSurface = this->adjacentSurface();
      if (!adjacentSurface) {
        if (constructionWithSearchDistance) {
          return constructionWithSearchDistance->first;
        }
        return boost::none;
      }

      boost::optional<std::pair<model::ConstructionBase, int>> adjacentConstructionWithSearchDistance =
        adjacentSurface->constructionWithSearchDistance();

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

    boost::optional<std::pair<ConstructionBase, int>> Surface_Impl::constructionWithSearchDistance() const {
      boost::optional<std::pair<ConstructionBase, int>> result;

      boost::optional<ConstructionBase> construction =
        getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_SurfaceFields::ConstructionName);
      if (construction) {
        return std::make_pair(*construction, 0);
      }

      boost::optional<Space> space = this->space();
      if (space) {
        result = space->getDefaultConstructionWithSearchDistance(this->getObject<Surface>());
      }
      return result;
    }

    bool Surface_Impl::isConstructionDefaulted() const {
      return isEmpty(OS_SurfaceFields::ConstructionName);
    }

    bool Surface_Impl::setVertices(const std::vector<Point3d>& vertices) {
      bool result = PlanarSurface_Impl::setVertices(vertices);

      if (isEmpty(OS_SurfaceFields::SurfaceType)) {
        if (result) {
          this->assignDefaultSurfaceType(false);
          this->assignDefaultBoundaryCondition(false);
          this->assignDefaultSunExposure(false);
          this->assignDefaultWindExposure(false);
          this->emitChangeSignals();  // emit signals here
        } else {
          LOG(Error, "Cannot compute default Surface properties.");
        }
      }

      return result;
    }

    bool Surface_Impl::setConstruction(const ConstructionBase& construction) {
      return setPointer(OS_SurfaceFields::ConstructionName, construction.handle());
    }

    void Surface_Impl::resetConstruction() {
      setString(OS_SurfaceFields::ConstructionName, "");
    }

    std::string Surface_Impl::surfaceType() const {
      boost::optional<std::string> value = getString(OS_SurfaceFields::SurfaceType, true, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::vector<std::string> Surface_Impl::surfaceTypeValues() const {
      return Surface::validSurfaceTypeValues();
    }

    std::string Surface_Impl::outsideBoundaryCondition() const {
      boost::optional<std::string> value = getString(OS_SurfaceFields::OutsideBoundaryCondition, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::vector<std::string> Surface_Impl::outsideBoundaryConditionValues() const {
      return Surface::validOutsideBoundaryConditionValues();
    }

    bool Surface_Impl::isGroundSurface() const {
      std::string outsideBoundaryCondition = this->outsideBoundaryCondition();

      if (istringEqual("Ground", outsideBoundaryCondition) || istringEqual("GroundFCfactorMethod", outsideBoundaryCondition)
          || istringEqual("GroundSlabPreprocessorAverage", outsideBoundaryCondition)
          || istringEqual("GroundSlabPreprocessorCore", outsideBoundaryCondition)
          || istringEqual("GroundSlabPreprocessorPerimeter", outsideBoundaryCondition)
          || istringEqual("GroundBasementPreprocessorAverageWall", outsideBoundaryCondition)
          || istringEqual("GroundBasementPreprocessorAverageFloor", outsideBoundaryCondition)
          || istringEqual("GroundBasementPreprocessorUpperWall", outsideBoundaryCondition)
          || istringEqual("GroundBasementPreprocessorLowerWall", outsideBoundaryCondition) || istringEqual("Foundation", outsideBoundaryCondition)) {
        return true;
      }

      return false;
    }

    std::string Surface_Impl::sunExposure() const {
      boost::optional<std::string> value = getString(OS_SurfaceFields::SunExposure, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::vector<std::string> Surface_Impl::sunExposureValues() const {
      return Surface::validSunExposureValues();
    }

    bool Surface_Impl::isSunExposureDefaulted() const {
      return isEmpty(OS_SurfaceFields::SunExposure);
    }

    std::string Surface_Impl::windExposure() const {
      boost::optional<std::string> value = getString(OS_SurfaceFields::WindExposure, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::vector<std::string> Surface_Impl::windExposureValues() const {
      return Surface::validWindExposureValues();
    }

    bool Surface_Impl::isWindExposureDefaulted() const {
      return isEmpty(OS_SurfaceFields::WindExposure);
    }

    boost::optional<double> Surface_Impl::viewFactortoGround() const {
      return getDouble(OS_SurfaceFields::ViewFactortoGround, true);
    }

    bool Surface_Impl::isViewFactortoGroundDefaulted() const {
      return isEmpty(OS_SurfaceFields::ViewFactortoGround);
    }

    bool Surface_Impl::isViewFactortoGroundAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_SurfaceFields::ViewFactortoGround, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autocalculate");
      }
      return result;
    }

    boost::optional<double> Surface_Impl::numberofVertices() const {
      return getDouble(OS_SurfaceFields::NumberofVertices, true);
    }

    bool Surface_Impl::isNumberofVerticesDefaulted() const {
      return isEmpty(OS_SurfaceFields::NumberofVertices);
    }

    bool Surface_Impl::isNumberofVerticesAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_SurfaceFields::NumberofVertices, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autocalculate");
      }
      return result;
    }

    bool Surface_Impl::setSurfaceType(const std::string& surfaceType) {
      return setSurfaceType(surfaceType, true);
    }

    bool Surface_Impl::setSurfaceType(const std::string& surfaceType, bool driverMethod) {
      bool result = setString(OS_SurfaceFields::SurfaceType, surfaceType, true);
      if (driverMethod) {
        this->emitChangeSignals();  // emit signals here
      }
      return result;
    }

    bool Surface_Impl::setOutsideBoundaryCondition(const std::string& outsideBoundaryCondition) {
      return setOutsideBoundaryCondition(outsideBoundaryCondition, true);
    }

    bool Surface_Impl::setOutsideBoundaryCondition(const std::string& outsideBoundaryCondition, bool driverMethod) {
      bool result = false;

      boost::optional<Surface> adjacentSurface = this->adjacentSurface();
      boost::optional<SurfacePropertyOtherSideCoefficients> surfacePropertyOtherSideCoefficients = this->surfacePropertyOtherSideCoefficients();
      boost::optional<SurfacePropertyOtherSideConditionsModel> surfacePropertyOtherSideConditionsModel =
        this->surfacePropertyOtherSideConditionsModel();

      if (istringEqual("Surface", outsideBoundaryCondition)) {
        if (adjacentSurface) {
          result = setString(OS_SurfaceFields::OutsideBoundaryCondition, outsideBoundaryCondition, true);
        }
      } else if (istringEqual("OtherSideCoefficients", outsideBoundaryCondition)) {
        if (surfacePropertyOtherSideCoefficients) {
          result = setString(OS_SurfaceFields::OutsideBoundaryCondition, outsideBoundaryCondition, true);
        }
      } else if (istringEqual("OtherSideConditionsModel", outsideBoundaryCondition)) {
        if (surfacePropertyOtherSideConditionsModel) {
          result = setString(OS_SurfaceFields::OutsideBoundaryCondition, outsideBoundaryCondition, true);
        }
      } else {
        this->resetAdjacentSurface();
        this->resetSurfacePropertyOtherSideCoefficients();
        this->resetSurfacePropertyOtherSideConditionsModel();
        result = setString(OS_SurfaceFields::OutsideBoundaryCondition, outsideBoundaryCondition, true);
        if (result) {
          this->assignDefaultSunExposure(false);
          this->assignDefaultWindExposure(false);

          if (istringEqual("Adiabatic", outsideBoundaryCondition)) {
            // remove all subsurfaces
            int n_subsurfaces = 0;
            for (auto& subSurface : subSurfaces()) {
              subSurface.remove();
              ++n_subsurfaces;
            }
            if (n_subsurfaces > 0) {
              // Note JM 2019-03-05: Warn user, it's not obvious that this is happening and they might try to access again
              // one of these subsurfaces which are now disconnected objects
              LOG(Warn, "Setting the Outside Boundary Condition for Surface '" << this->nameString() << "' to 'Adiabatic', removed " << n_subsurfaces
                                                                               << " SubSurfaces.");
            }
          }
        } else if (adjacentSurface) {
          // restore the adjacent surface if set boundary condition fails
          bool test = setAdjacentSurface(*adjacentSurface);
          OS_ASSERT(test);
        } else if (surfacePropertyOtherSideCoefficients) {
          // restore the surfacePropertyOtherSideCoefficientse if set boundary condition fails
          bool test = setSurfacePropertyOtherSideCoefficients(*surfacePropertyOtherSideCoefficients);
          OS_ASSERT(test);
        } else if (surfacePropertyOtherSideConditionsModel) {
          // restore the surfacePropertyOtherSideConditionsModel if set boundary condition fails
          bool test = setSurfacePropertyOtherSideConditionsModel(*surfacePropertyOtherSideConditionsModel);
          OS_ASSERT(test);
        }
      }

      if (driverMethod) {
        this->emitChangeSignals();  // emit signals here
      }

      return result;
    }

    bool Surface_Impl::setSunExposure(const std::string& sunExposure) {
      return setSunExposure(sunExposure, true);
    }

    bool Surface_Impl::setSunExposure(const std::string& sunExposure, bool driverMethod) {
      bool result = setString(OS_SurfaceFields::SunExposure, sunExposure, true);
      if (driverMethod) {
        this->emitChangeSignals();  // emit signals here
      }
      return result;
    }

    void Surface_Impl::resetSunExposure() {
      bool result = setString(OS_SurfaceFields::SunExposure, "");
      OS_ASSERT(result);
    }

    bool Surface_Impl::setWindExposure(const std::string& windExposure) {
      return setWindExposure(windExposure, true);
    }

    bool Surface_Impl::setWindExposure(const std::string& windExposure, bool driverMethod) {
      bool result = setString(OS_SurfaceFields::WindExposure, windExposure, true);
      if (driverMethod) {
        this->emitChangeSignals();  // emit signals here
      }
      return result;
    }

    void Surface_Impl::resetWindExposure() {
      bool result = setString(OS_SurfaceFields::WindExposure, "");
      OS_ASSERT(result);
    }

    bool Surface_Impl::setViewFactortoGround(boost::optional<double> viewFactortoGround) {
      bool result = false;
      if (viewFactortoGround) {
        result = setDouble(OS_SurfaceFields::ViewFactortoGround, viewFactortoGround.get());
      } else {
        result = setString(OS_SurfaceFields::ViewFactortoGround, "");
      }
      return result;
    }

    bool Surface_Impl::setViewFactortoGround(double viewFactortoGround) {
      bool result = setDouble(OS_SurfaceFields::ViewFactortoGround, viewFactortoGround);
      return result;
    }

    void Surface_Impl::resetViewFactortoGround() {
      bool result = setString(OS_SurfaceFields::ViewFactortoGround, "");
      OS_ASSERT(result);
    }

    void Surface_Impl::autocalculateViewFactortoGround() {
      bool result = setString(OS_SurfaceFields::ViewFactortoGround, "Autocalculate");
      OS_ASSERT(result);
    }

    bool Surface_Impl::setNumberofVertices(boost::optional<double> numberofVertices) {
      bool result = false;
      if (numberofVertices) {
        result = setDouble(OS_SurfaceFields::NumberofVertices, numberofVertices.get());
      } else {
        result = setString(OS_SurfaceFields::NumberofVertices, "");
      }
      return result;
    }

    bool Surface_Impl::setNumberofVertices(double numberofVertices) {
      bool result = setDouble(OS_SurfaceFields::NumberofVertices, numberofVertices);
      return result;
    }

    void Surface_Impl::resetNumberofVertices() {
      bool result = setString(OS_SurfaceFields::NumberofVertices, "");
      OS_ASSERT(result);
    }

    void Surface_Impl::autocalculateNumberofVertices() {
      bool result = setString(OS_SurfaceFields::NumberofVertices, "Autocalculate");
      OS_ASSERT(result);
    }

    SubSurfaceVector Surface_Impl::subSurfaces() const {
      SubSurfaceVector result;
      auto object = getObject<ModelObject>();
      WorkspaceObjectVector idfSubSurfaces = object.getSources(IddObjectType(IddObjectType::OS_SubSurface));
      for (const WorkspaceObject& idfSubSurface : idfSubSurfaces) {
        OptionalSubSurface subSurface = this->model().getModelObject<SubSurface>(idfSubSurface.handle());
        if (subSurface) {
          result.push_back(*subSurface);
        }
      }
      return result;
    }

    boost::optional<PlanarSurfaceGroup> Surface_Impl::planarSurfaceGroup() const {
      return boost::optional<PlanarSurfaceGroup>(this->space());
    }

    boost::optional<Space> Surface_Impl::space() const {
      boost::optional<Space> result;
      OptionalWorkspaceObject space = getTarget(OS_SurfaceFields::SpaceName);
      if (space) {
        result = space->optionalCast<Space>();
      }
      return result;
    }

    boost::optional<double> Surface_Impl::uFactor() const {
      OptionalConstructionBase oConstruction = construction();
      OptionalDouble result;

      if (oConstruction) {
        // from input
        OptionalDouble inputResult = oConstruction->uFactor(filmResistance());

        // from output
        OptionalSqlFile sqlFile = model().sqlFile();
        OptionalString constructionName = oConstruction->name();
        OptionalDouble outputResult;
        OptionalString surfaceName = name();

        // opaque exterior
        if (sqlFile && constructionName && oConstruction->isOpaque()) {
          std::string query = R"(SELECT Value from TabularDataWithStrings
                                    WHERE ReportName = 'EnvelopeSummary'
                                      AND ReportForString = 'Entire Facility'
                                      AND TableName = 'Opaque Exterior'
                                      AND ColumnName = 'U-Factor with Film'
                                      AND Units='W/m2-K'
                                      AND RowName = ?;)";
          outputResult = sqlFile->execAndReturnFirstDouble(query, to_upper_copy(*surfaceName));
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
          outputResult = sqlFile->execAndReturnFirstDouble(query, to_upper_copy(*surfaceName));
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

    boost::optional<double> Surface_Impl::thermalConductance() const {
      OptionalConstructionBase oConstruction = construction();
      OptionalDouble result;

      if (oConstruction) {
        // from input
        OptionalDouble inputResult = oConstruction->thermalConductance(filmResistance());

        // from output
        OptionalSqlFile sqlFile = model().sqlFile();
        OptionalString constructionName = oConstruction->name();
        OptionalDouble outputResult;
        OptionalString surfaceName = name();

        // opaque exterior
        if (sqlFile && constructionName && oConstruction->isOpaque()) {
          std::string query = R"(SELECT Value from TabularDataWithStrings
                                    WHERE ReportName = 'EnvelopeSummary'
                                      AND ReportForString = 'Entire Facility'
                                      AND TableName = 'Opaque Exterior'
                                      AND ColumnName = 'U-Factor no Film'
                                      AND Units='W/m2-K'
                                      AND RowName = ?;)";
          outputResult = sqlFile->execAndReturnFirstDouble(query, to_upper_copy(*surfaceName));
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
          outputResult = sqlFile->execAndReturnFirstDouble(query, to_upper_copy(*surfaceName));

          if (outputResult) {
            outputResult = 1.0 / (1.0 / (*outputResult) - filmResistance());
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

    bool Surface_Impl::setUFactor(double value) {
      OptionalConstructionBase oConstruction = this->construction();
      if (oConstruction) {
        return oConstruction->setUFactor(value, filmResistance());
      }
      return false;
    }

    bool Surface_Impl::setThermalConductance(double value) {
      OptionalConstructionBase oConstruction = this->construction();
      if (oConstruction) {
        return oConstruction->setConductance(value, filmResistance());
      }
      return false;
    }

    bool Surface_Impl::setSpace(const Space& space) {
      return setPointer(OS_SurfaceFields::SpaceName, space.handle());
    }

    boost::optional<Surface> Surface_Impl::adjacentSurface() const {
      return getObject<Surface>().getModelObjectTarget<Surface>(OS_SurfaceFields::OutsideBoundaryConditionObject);
    }

    bool Surface_Impl::setAdjacentSurface(Surface& surface) {

      // matching surface with self is ok for stories with multipliers
      bool isSameSurface = false;
      if (surface.handle() == this->handle()) {
        isSameSurface = true;
      }

      // matching surface with other surface in same space is ok for stories with multipliers
      //boost::optional<Space> thisSpace = this->space();
      //boost::optional<Space> otherSpace = surface.space();
      //if (!thisSpace || !otherSpace || (thisSpace->handle() == otherSpace->handle())){
      //  return false;
      //}

      boost::optional<Surface> adjacentSurface = this->adjacentSurface();
      boost::optional<Surface> otherAdjacentSurface = surface.adjacentSurface();

      // if other surface is pointing to this
      bool isNewMatch = true;
      if (otherAdjacentSurface && (otherAdjacentSurface->handle() == this->handle())) {
        // if this is pointing to other surface
        if (adjacentSurface && (adjacentSurface->handle() == surface.handle())) {
          isNewMatch = false;
        }
      }

      // this is basically testing if surface is in same model as this
      bool test = this->setPointer(OS_SurfaceFields::OutsideBoundaryConditionObject, surface.handle());
      if (test) {

        // clean all other surfaces pointing to this (unless it is surface)
        for (const WorkspaceObject& wo : this->getSources(IddObjectType::OS_Surface)) {
          if (wo.handle() == surface.handle()) {
            continue;
          }

          auto otherSurface = wo.cast<Surface>();
          test = otherSurface.setString(OS_SurfaceFields::OutsideBoundaryConditionObject, "");
          OS_ASSERT(test);
          otherSurface.assignDefaultBoundaryCondition();
          otherSurface.assignDefaultSunExposure();
          otherSurface.assignDefaultWindExposure();

          for (SubSurface& subSurface : otherSurface.subSurfaces()) {
            subSurface.resetAdjacentSubSurface();
          }
        }

        if (!isSameSurface) {
          // clean all other surfaces pointing to surface (unless it is this)
          for (const WorkspaceObject& wo : surface.getSources(IddObjectType::OS_Surface)) {
            if (wo.handle() == this->handle()) {
              continue;
            }

            auto otherSurface = wo.cast<Surface>();
            test = otherSurface.setString(OS_SurfaceFields::OutsideBoundaryConditionObject, "");
            OS_ASSERT(test);
            otherSurface.assignDefaultBoundaryCondition();
            otherSurface.assignDefaultSunExposure();
            otherSurface.assignDefaultWindExposure();

            for (SubSurface& subSurface : otherSurface.subSurfaces()) {
              subSurface.resetAdjacentSubSurface();
            }
          }
        }

        // this and surface are newly pointing to each other, clean sub surfaces on both
        if (isNewMatch) {
          for (SubSurface& subSurface : this->subSurfaces()) {
            subSurface.resetAdjacentSubSurface();
          }
          if (!isSameSurface) {
            for (SubSurface& subSurface : surface.subSurfaces()) {
              subSurface.resetAdjacentSubSurface();
            }
          }
        }

        // pointer was set up above, it may be unset during calls to resetAdjacentSubSurface above, just reset it here
        bool test = this->setPointer(OS_SurfaceFields::OutsideBoundaryConditionObject, surface.handle());
        OS_ASSERT(test);
        test = this->setString(OS_SurfaceFields::OutsideBoundaryCondition, "Surface");
        OS_ASSERT(test);
        this->assignDefaultSunExposure();
        this->assignDefaultWindExposure();

        if (!isSameSurface) {
          test = surface.setPointer(OS_SurfaceFields::OutsideBoundaryConditionObject, this->handle());
          OS_ASSERT(test);
          test = surface.setString(OS_SurfaceFields::OutsideBoundaryCondition, "Surface");
          OS_ASSERT(test);
          surface.assignDefaultSunExposure();
          surface.assignDefaultWindExposure();
        }
      }
      return test;
    }

    void Surface_Impl::resetAdjacentSurface() {

      bool test;

      // need to be careful to only call assignDefaultBoundaryCondition if adjacentSurface
      // is set as assignDefaultBoundaryCondition can call resetAdjacentSurface
      boost::optional<Surface> adjacentSurface = this->adjacentSurface();
      if (adjacentSurface) {
        test = setString(OS_SurfaceFields::OutsideBoundaryConditionObject, "");
        OS_ASSERT(test);
        this->assignDefaultBoundaryCondition();
        this->assignDefaultSunExposure();
        this->assignDefaultWindExposure();
      }

      // unset all matched sub surfaces
      for (SubSurface& subSurface : this->subSurfaces()) {
        subSurface.resetAdjacentSubSurface();
      }

      // clean all other surfaces pointing to this
      for (const WorkspaceObject& wo : this->getSources(IddObjectType::OS_Surface)) {

        auto otherSurface = wo.cast<Surface>();
        test = otherSurface.setString(OS_SurfaceFields::OutsideBoundaryConditionObject, "");
        OS_ASSERT(test);
        otherSurface.assignDefaultBoundaryCondition();
        otherSurface.assignDefaultSunExposure();
        otherSurface.assignDefaultWindExposure();

        for (SubSurface& subSurface : otherSurface.subSurfaces()) {
          subSurface.resetAdjacentSubSurface();
        }
      }
    }

    boost::optional<SurfaceControlMovableInsulation> Surface_Impl::surfaceControlMovableInsulation() const {
      auto thisSurface = getObject<Surface>();
      std::vector<SurfaceControlMovableInsulation> movableInsulations =
        thisSurface.getModelObjectSources<SurfaceControlMovableInsulation>(SurfaceControlMovableInsulation::iddObjectType());
      if (movableInsulations.empty()) {
        return boost::none;
      } else if (movableInsulations.size() == 1) {
        return movableInsulations.at(0);
      } else {
        LOG(Error, "More than one SurfaceControlMovableInsulation points to this Surface");
        return boost::none;
      }
    }

    boost::optional<SurfacePropertyConvectionCoefficients> Surface_Impl::surfacePropertyConvectionCoefficients() const {
      std::vector<SurfacePropertyConvectionCoefficients> spccs(model().getConcreteModelObjects<SurfacePropertyConvectionCoefficients>());
      auto thisHandle = this->handle();
      auto isNotPointingToMe = [&thisHandle](const auto& spcc) {
        OptionalSurface surface = spcc.surfaceAsSurface();
        return !surface || !(surface->handle() == thisHandle);
      };

      spccs.erase(std::remove_if(spccs.begin(), spccs.end(), isNotPointingToMe), spccs.end());

      if (spccs.empty()) {
        return boost::none;
      } else if (spccs.size() == 1) {
        return spccs.at(0);
      } else {
        LOG(Error, "More than one SurfacePropertyConvectionCoefficients points to this Surface");
        return boost::none;
      }
    }

    boost::optional<SurfacePropertyLocalEnvironment> Surface_Impl::surfacePropertyLocalEnvironment() const {

      std::vector<SurfacePropertyLocalEnvironment> result;
      for (auto& localEnv : model().getConcreteModelObjects<SurfacePropertyLocalEnvironment>()) {
        if (auto surface_ = localEnv.exteriorSurfaceAsSurface()) {
          if (surface_->handle() == handle()) {
            result.push_back(localEnv);
          }
        }
      }
      if (result.empty()) {
        return boost::none;
      } else if (result.size() == 1) {
        return result.at(0);
      } else {
        LOG(Error, "More than one SurfacePropertyLocalEnvironment points to this Surface");
        return boost::none;
      }
    }

    boost::optional<SurfacePropertyOtherSideCoefficients> Surface_Impl::surfacePropertyOtherSideCoefficients() const {
      return getObject<Surface>().getModelObjectTarget<SurfacePropertyOtherSideCoefficients>(OS_SurfaceFields::OutsideBoundaryConditionObject);
    }

    bool Surface_Impl::setSurfacePropertyOtherSideCoefficients(SurfacePropertyOtherSideCoefficients& otherSideCoefficients) {
      boost::optional<Surface> adjacentSurface = this->adjacentSurface();
      if (adjacentSurface) {
        resetAdjacentSurface();
      }

      // this is basically testing if surface is in same model as this
      bool test = this->setPointer(OS_SurfaceFields::OutsideBoundaryConditionObject, otherSideCoefficients.handle());
      if (test) {
        test = this->setString(OS_SurfaceFields::OutsideBoundaryCondition, "OtherSideCoefficients");
        OS_ASSERT(test);
        this->assignDefaultSunExposure();
        this->assignDefaultWindExposure();
      }
      return test;
    }

    void Surface_Impl::resetSurfacePropertyOtherSideCoefficients() {
      bool test;

      // need to be careful to only call assignDefaultBoundaryCondition if surfacePropertyOtherSideCoefficients
      // is set as assignDefaultBoundaryCondition can call resetSurfacePropertyOtherSideCoefficients
      boost::optional<SurfacePropertyOtherSideCoefficients> surfacePropertyOtherSideCoefficients = this->surfacePropertyOtherSideCoefficients();
      if (surfacePropertyOtherSideCoefficients) {
        test = setString(OS_SurfaceFields::OutsideBoundaryConditionObject, "");
        OS_ASSERT(test);
        this->assignDefaultBoundaryCondition();
        this->assignDefaultSunExposure();
        this->assignDefaultWindExposure();
      }

      // reset all sub surfaces
      for (SubSurface& subSurface : this->subSurfaces()) {
        subSurface.resetSurfacePropertyOtherSideCoefficients();
      }
    }

    boost::optional<SurfacePropertyOtherSideConditionsModel> Surface_Impl::surfacePropertyOtherSideConditionsModel() const {
      return getObject<Surface>().getModelObjectTarget<SurfacePropertyOtherSideConditionsModel>(OS_SurfaceFields::OutsideBoundaryConditionObject);
    }

    bool Surface_Impl::setSurfacePropertyOtherSideConditionsModel(SurfacePropertyOtherSideConditionsModel& otherSideModel) {
      boost::optional<Surface> adjacentSurface = this->adjacentSurface();
      if (adjacentSurface) {
        resetAdjacentSurface();
      }

      // this is basically testing if surface is in same model as this
      bool test = this->setPointer(OS_SurfaceFields::OutsideBoundaryConditionObject, otherSideModel.handle());
      if (test) {
        test = this->setString(OS_SurfaceFields::OutsideBoundaryCondition, "OtherSideConditionsModel");
        OS_ASSERT(test);
        this->assignDefaultSunExposure();
        this->assignDefaultWindExposure();
      }
      return test;
    }

    void Surface_Impl::resetSurfacePropertyOtherSideConditionsModel() {
      bool test;

      // need to be careful to only call assignDefaultBoundaryCondition if surfacePropertyOtherSideConditionsModel
      // is set as assignDefaultBoundaryCondition can call resetSurfacePropertyOtherSideConditionsModel
      boost::optional<SurfacePropertyOtherSideConditionsModel> surfacePropertyOtherSideConditionsModel =
        this->surfacePropertyOtherSideConditionsModel();
      if (surfacePropertyOtherSideConditionsModel) {
        test = setString(OS_SurfaceFields::OutsideBoundaryConditionObject, "");
        OS_ASSERT(test);
        this->assignDefaultBoundaryCondition();
        this->assignDefaultSunExposure();
        this->assignDefaultWindExposure();
      }

      // reset all sub surfaces
      for (SubSurface& subSurface : this->subSurfaces()) {
        subSurface.resetSurfacePropertyOtherSideConditionsModel();
      }
    }

    bool Surface_Impl::intersect(Surface& otherSurface) {
      boost::optional<SurfaceIntersection> intersection = computeIntersection(otherSurface);
      return intersection.has_value();
    }

    boost::optional<SurfaceIntersection> Surface_Impl::computeIntersection(Surface& otherSurface) {
      double tol = 0.01;       //  1 cm tolerance
      double areaTol = 0.001;  // 10 cm2 tolerance

      constexpr bool extraLogging = false;

      boost::optional<Space> space = this->space();
      boost::optional<Space> otherSpace = otherSurface.space();

      if (!space || !otherSpace || space->handle() == otherSpace->handle()) {
        LOG(Error, "Cannot find spaces for each surface in intersection or surfaces in same space.");
        return boost::none;
      }

      if (!this->subSurfaces().empty() || !otherSurface.subSurfaces().empty()) {
        LOG(Error, "Subsurfaces are not allowed in intersection");
        return boost::none;
      }

      if (this->adjacentSurface() || otherSurface.adjacentSurface()) {
        LOG(Error, "Adjacent surfaces are not allowed in intersection");
        return boost::none;
      }

      // goes from local system to building coordinates
      Transformation spaceTransformation = space->transformation();
      Transformation otherSpaceTransformation = otherSpace->transformation();

      // do the intersection in building coordinates

      Plane plane = spaceTransformation * this->plane();
      Plane otherPlane = otherSpaceTransformation * otherSurface.plane();

      if (!plane.reverseEqual(otherPlane)) {
        //LOG(Info, "Planes are not reverse equal, intersection of '" << this->name().get() << "' with '" << otherSurface.name().get() << "' fails");
        return boost::none;
      }

      // get vertices in building coordinates
      std::vector<Point3d> buildingVertices = spaceTransformation * this->vertices();
      std::vector<Point3d> otherBuildingVertices = otherSpaceTransformation * otherSurface.vertices();

      if ((buildingVertices.size() < 3) || (otherBuildingVertices.size() < 3)) {
        LOG(Error, "Fewer than 3 vertices, intersection of '" << this->name().get() << "' with '" << otherSurface.name().get() << "' fails");
        return boost::none;
      }

      // goes from face coordinates of building vertices to building coordinates
      Transformation faceTransformation;
      Transformation faceTransformationInverse;
      try {
        faceTransformation = Transformation::alignFace(buildingVertices);
        faceTransformationInverse = faceTransformation.inverse();
      } catch (const std::exception&) {
        LOG(Error, "Cannot compute face transform, intersection of '" << this->name().get() << "' with '" << otherSurface.name().get() << "' fails");
        return boost::none;
      }

      // put building vertices into face coordinates
      std::vector<Point3d> faceVertices = faceTransformationInverse * buildingVertices;
      std::vector<Point3d> otherFaceVertices = faceTransformationInverse * otherBuildingVertices;

      // boost polygon wants vertices in clockwise order, faceVertices must be reversed, otherFaceVertices already CCW
      std::reverse(faceVertices.begin(), faceVertices.end());
      //std::reverse(otherFaceVertices.begin(), otherFaceVertices.end());

      //LOG(Info, "Trying intersection of '" << this->name().get() << "' with '" << otherSurface.name().get());
      if constexpr (extraLogging) {
        Point3dVectorVector tmp{faceVertices, otherFaceVertices};
        LOG(Debug, tmp);
      }
      boost::optional<IntersectionResult> intersection = openstudio::intersect(faceVertices, otherFaceVertices, tol);
      if (!intersection) {
        //LOG(Info, "No intersection");
        return boost::none;
      }

      if constexpr (extraLogging) {
        Point3dVectorVector tmp;
        Point3dVectorVector newPolys = intersection->newPolygons2();
        tmp.reserve(newPolys.size() + 1);
        tmp.push_back(intersection->polygon2());
        tmp.insert(tmp.end(), std::make_move_iterator(newPolys.begin()), std::make_move_iterator(newPolys.end()));
        LOG(Debug, tmp);
      }

      // DA - Change tolerance. Current tolerance is 0.0001 which is 1cm2 which is unrealistic
      // tolerance could be fixed, say 10cm2 or as a proportion of the area of the polygon. 4cm2
      // on a polygon of area 570m2 is a tiny fraction
      boost::optional<double> area1 = getArea(faceVertices);
      boost::optional<double> area2 = getArea(otherFaceVertices);
      if (area1) {
        if (std::abs(area1.get() - intersection->area1()) > areaTol) {
          LOG(Error, "Initial area of surface '" << this->nameString() << "' " << area1.get() << " does not equal post intersection area "
                                                 << intersection->area1());
          if constexpr (extraLogging) {
            Point3dVectorVector tmp1{faceVertices, otherFaceVertices};
            LOG(Debug, tmp1);
            Point3dVectorVector tmp;
            tmp.push_back(intersection->polygon1());
            for (auto& x : intersection->newPolygons1())
              tmp.push_back(x);
            LOG(Debug, tmp);
          }
        }
      }
      if (area2) {
        if (std::abs(area2.get() - intersection->area2()) > areaTol) {
          LOG(Error, "Initial area of other surface '" << otherSurface.nameString() << "' " << area2.get()
                                                       << " does not equal post intersection area " << intersection->area2());
          if constexpr (extraLogging) {
            Point3dVectorVector tmp1{faceVertices, otherFaceVertices};
            LOG(Debug, tmp1);
            Point3dVectorVector tmp;
            tmp.push_back(intersection->polygon2());
            for (auto& x : intersection->newPolygons2())
              tmp.push_back(x);
            LOG(Debug, tmp);
          }
        }
      }

      // non-zero intersection
      // could match here but will save that for other discrete operation
      Surface surface(std::dynamic_pointer_cast<Surface_Impl>(this->shared_from_this()));
      std::vector<Surface> newSurfaces;
      std::vector<Surface> newOtherSurfaces;

      //LOG(Debug, "Before intersection");
      //LOG(Debug, surface);
      //LOG(Debug, otherSurface);

      // goes from building coordinates to local system
      Transformation spaceTransformationInverse = spaceTransformation.inverse();
      Transformation otherSpaceTransformationInverse = otherSpaceTransformation.inverse();

      std::vector<std::vector<Point3d>> newPolygons1 = intersection->newPolygons1();
      std::vector<std::vector<Point3d>> newPolygons2 = intersection->newPolygons2();

      // modify vertices for surface in this space
      std::vector<Point3d> newBuildingVertices = faceTransformation * intersection->polygon1();
      std::vector<Point3d> newVertices = spaceTransformationInverse * newBuildingVertices;
      std::reverse(newVertices.begin(), newVertices.end());
      newVertices = reorderULC(newVertices);
      this->setVertices(newVertices);
      //this->setAdjacentSurface(otherSurface);

      // modify vertices for surface in other space
      std::vector<Point3d> newOtherBuildingVertices = faceTransformation * intersection->polygon2();
      std::vector<Point3d> newOtherVertices = otherSpaceTransformationInverse * newOtherBuildingVertices;
      newOtherVertices = reorderULC(newOtherVertices);
      otherSurface.setVertices(newOtherVertices);
      //otherSurface.setAdjacentSurface(surface);

      if (newPolygons1.empty() && newPolygons2.empty()) {
        // both surfaces intersect perfectly, no-op
      } else {
        // new surfaces are created
        // create new surfaces in this space
        for (unsigned i = 0; i < newPolygons1.size(); ++i) {

          // new surface in this space
          newBuildingVertices = faceTransformation * newPolygons1[i];
          newVertices = spaceTransformationInverse * newBuildingVertices;
          std::reverse(newVertices.begin(), newVertices.end());
          newVertices = reorderULC(newVertices);
          Surface newSurface(newVertices, this->model());
          newSurface.setSpace(*space);
          newSurfaces.push_back(newSurface);
        }

        // create new surfaces in other space
        for (unsigned i = 0; i < newPolygons2.size(); ++i) {

          // new surface in other space
          newOtherBuildingVertices = faceTransformation * newPolygons2[i];
          newOtherVertices = otherSpaceTransformationInverse * newOtherBuildingVertices;
          newOtherVertices = reorderULC(newOtherVertices);
          Surface newOtherSurface(newOtherVertices, this->model());
          newOtherSurface.setSpace(*otherSpace);
          newOtherSurfaces.push_back(newOtherSurface);
        }
      }

      SurfaceIntersection result(surface, otherSurface, newSurfaces, newOtherSurfaces);

      LOG(Info, "Intersection of '" << this->name().get() << "' with '" << otherSurface.name().get() << "' results in " << result);

      //LOG(Debug, "After intersection");
      //LOG(Debug, surface);
      //LOG(Debug, otherSurface);
      //for (const Surface& s : newSurfaces){
      //  LOG(Debug, s);
      //}
      //for (const Surface& s : newOtherSurfaces){
      //  LOG(Debug, s);
      //}

      return result;
    }

    boost::optional<Surface> Surface_Impl::createAdjacentSurface(const Space& otherSpace) {
      boost::optional<Space> space = this->space();
      if (!space || (space->handle() == otherSpace.handle())) {
        return boost::none;
      }

      Transformation thisSpaceT = space->transformation();
      Transformation otherSpaceT = otherSpace.transformation();
      Transformation transformation = otherSpaceT.inverse() * thisSpaceT;

      std::vector<Point3d> vertices = transformation * this->vertices();
      std::reverse(vertices.begin(), vertices.end());

      Model model = this->model();

      // Be careful not to call clone as you will get duplicate sub surfaces
      Surface otherSurface(vertices, model);
      otherSurface.setName(this->name().get() + " Reversed");
      otherSurface.resetConstruction();  // this will use surface's construction on export, TODO: do something better

      std::string surfaceType = this->surfaceType();
      if (surfaceType == "RoofCeiling") {
        otherSurface.setSurfaceType("Floor");
      } else if (surfaceType == "Floor") {
        otherSurface.setSurfaceType("RoofCeiling");
      } else if (surfaceType == "Wall") {
        otherSurface.setSurfaceType("Wall");
      }

      otherSurface.setSpace(otherSpace);

      this->setAdjacentSurface(otherSurface);

      for (SubSurface& subSurface : this->subSurfaces()) {
        vertices = transformation * subSurface.vertices();
        std::reverse(vertices.begin(), vertices.end());

        SubSurface otherSubSurface(vertices, model);
        otherSubSurface.setName(subSurface.name().get() + " Reversed");
        otherSubSurface.resetConstruction();  // this will use sub surface's construction on export, TODO: do something better
        otherSubSurface.setSubSurfaceType(subSurface.subSurfaceType());
        otherSubSurface.setSurface(otherSurface);
        otherSubSurface.setAdjacentSubSurface(subSurface);
      }

      return otherSurface;
    }

    bool Surface_Impl::isPartOfEnvelope() const {
      std::string bc = this->outsideBoundaryCondition();
      bool result = (istringEqual("Outdoors", bc) || this->isGroundSurface());
      return result;
    }

    void Surface_Impl::assignDefaultSurfaceType() {
      assignDefaultSurfaceType(true);
    }

    void Surface_Impl::assignDefaultSurfaceType(bool driverMethod) {
      double degTilt = radToDeg(this->tilt());
      bool ok = true;
      if (degTilt < 60) {
        ok = setSurfaceType("RoofCeiling", driverMethod);
        OS_ASSERT(ok);
      } else if (degTilt < 179) {
        ok = setSurfaceType("Wall", driverMethod);
        OS_ASSERT(ok);
      } else {
        ok = setSurfaceType("Floor", driverMethod);
        OS_ASSERT(ok);
      }
    }

    void Surface_Impl::assignDefaultBoundaryCondition() {
      assignDefaultBoundaryCondition(true);
    }

    void Surface_Impl::assignDefaultBoundaryCondition(bool driverMethod) {
      if (this->adjacentSurface()) {
        bool test = this->setOutsideBoundaryCondition("Surface", driverMethod);
        OS_ASSERT(test);
      } else if (this->surfacePropertyOtherSideCoefficients()) {
        bool test = this->setOutsideBoundaryCondition("OtherSideCoefficients", driverMethod);
        OS_ASSERT(test);
      } else if (this->surfacePropertyOtherSideConditionsModel()) {
        bool test = this->setOutsideBoundaryCondition("OtherSideConditionsModel", driverMethod);
        OS_ASSERT(test);
      } else if (this->adjacentFoundation()) {
        bool test = this->setOutsideBoundaryCondition("Foundation", driverMethod);
        OS_ASSERT(test);
      } else if (istringEqual("Floor", this->surfaceType())) {
        bool test = this->setOutsideBoundaryCondition("Ground", driverMethod);
        OS_ASSERT(test);
      } else {
        bool test = this->setOutsideBoundaryCondition("Outdoors", driverMethod);
        OS_ASSERT(test);
      }
    }

    void Surface_Impl::assignDefaultSunExposure() {
      assignDefaultSunExposure(true);
    }

    void Surface_Impl::assignDefaultSunExposure(bool driverMethod) {
      std::string outsideBoundaryCondition = this->outsideBoundaryCondition();
      if (istringEqual("Outdoors", outsideBoundaryCondition)) {
        bool test = this->setSunExposure("SunExposed", driverMethod);
        OS_ASSERT(test);
      } else if (istringEqual("Surface", outsideBoundaryCondition) || istringEqual("OtherSideCoefficients", outsideBoundaryCondition)
                 || istringEqual("Adiabatic", outsideBoundaryCondition) || istringEqual("Ground", outsideBoundaryCondition)
                 || istringEqual("GroundFCfactorMethod", outsideBoundaryCondition)
                 || istringEqual("GroundSlabPreprocessorAverage", outsideBoundaryCondition)
                 || istringEqual("GroundSlabPreprocessorCore", outsideBoundaryCondition)
                 || istringEqual("GroundSlabPreprocessorPerimeter", outsideBoundaryCondition)
                 || istringEqual("GroundBasementPreprocessorAverageWall", outsideBoundaryCondition)
                 || istringEqual("GroundBasementPreprocessorAverageFloor", outsideBoundaryCondition)
                 || istringEqual("GroundBasementPreprocessorUpperWall", outsideBoundaryCondition)
                 || istringEqual("GroundBasementPreprocessorLowerWall", outsideBoundaryCondition)
                 || istringEqual("Foundation", outsideBoundaryCondition)) {
        bool test = this->setSunExposure("NoSun", driverMethod);
        OS_ASSERT(test);
      } else {
        std::string surfaceType = this->surfaceType();
        if (istringEqual("Floor", surfaceType)) {
          bool test = this->setSunExposure("NoSun", driverMethod);
          OS_ASSERT(test);
        } else {
          bool test = this->setSunExposure("SunExposed", driverMethod);
          OS_ASSERT(test);
        }
      }
    }

    void Surface_Impl::assignDefaultWindExposure() {
      assignDefaultWindExposure(true);
    }

    void Surface_Impl::assignDefaultWindExposure(bool driverMethod) {
      std::string outsideBoundaryCondition = this->outsideBoundaryCondition();
      if (istringEqual("Outdoors", outsideBoundaryCondition)) {
        bool test = this->setWindExposure("WindExposed", driverMethod);
        OS_ASSERT(test);
      } else if (istringEqual("Surface", outsideBoundaryCondition) || istringEqual("OtherSideCoefficients", outsideBoundaryCondition)
                 || istringEqual("Adiabatic", outsideBoundaryCondition) || istringEqual("Ground", outsideBoundaryCondition)
                 || istringEqual("GroundFCfactorMethod", outsideBoundaryCondition)
                 || istringEqual("GroundSlabPreprocessorAverage", outsideBoundaryCondition)
                 || istringEqual("GroundSlabPreprocessorCore", outsideBoundaryCondition)
                 || istringEqual("GroundSlabPreprocessorPerimeter", outsideBoundaryCondition)
                 || istringEqual("GroundBasementPreprocessorAverageWall", outsideBoundaryCondition)
                 || istringEqual("GroundBasementPreprocessorAverageFloor", outsideBoundaryCondition)
                 || istringEqual("GroundBasementPreprocessorUpperWall", outsideBoundaryCondition)
                 || istringEqual("GroundBasementPreprocessorLowerWall", outsideBoundaryCondition)
                 || istringEqual("Foundation", outsideBoundaryCondition)) {
        bool test = this->setWindExposure("NoWind", driverMethod);
        OS_ASSERT(test);
      } else {
        std::string surfaceType = this->surfaceType();
        if (istringEqual("Floor", surfaceType)) {
          bool test = this->setWindExposure("NoWind", driverMethod);
          OS_ASSERT(test);
        } else {
          bool test = this->setWindExposure("WindExposed", driverMethod);
          OS_ASSERT(test);
        }
      }
    }

    std::vector<ModelObject> Surface_Impl::subSurfacesAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(subSurfaces());
      return result;
    }

    boost::optional<ModelObject> Surface_Impl::adjacentSurfaceAsModelObject() const {
      OptionalModelObject result;
      OptionalSurface intermediate = adjacentSurface();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool Surface_Impl::setSpaceAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSpace intermediate = modelObject->optionalCast<Space>();
        if (intermediate) {
          return setSpace(*intermediate);
        }
      }
      return false;
    }

    bool Surface_Impl::setAdjacentSurfaceAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSurface intermediate = modelObject->optionalCast<Surface>();
        if (intermediate) {
          return setAdjacentSurface(*intermediate);
        } else {
          return false;
        }
      } else {
        resetAdjacentSurface();
      }
      return true;
    }

    double Surface_Impl::filmResistance() const {
      double interiorResistance = PlanarSurface::stillAirFilmResistance(tilt());
      std::string obc = this->outsideBoundaryCondition();
      boost::to_lower(obc);
      if (obc == "outdoors") {
        return PlanarSurface::filmResistance(FilmResistanceType::MovingAir_15mph) + interiorResistance;
      } else if (isPartOfEnvelope()) {
        // ground, underground, or other side coefficients/conditions
        // assume one interiorResistance
        if ((obc == "othersidecoefficients") || (obc == "othersideconditionsmodel")) {
          LOG(Warn, "Returning film resistance for " << briefDescription()
                                                     << " as if it was "
                                                        "underground (one still air film resistance for the inside surface). This may not "
                                                        "be correct as the outside boundary condition is '"
                                                     << obc << "'.");
        }
        return interiorResistance;
      }
      return 2.0 * interiorResistance;
    }

    double Surface_Impl::windowToWallRatio() const {
      double result = 0.0;

      if (!istringEqual(this->surfaceType(), "Wall")) {
        return result;
      }

      double grossArea = this->grossArea();

      if (grossArea == 0) {
        return result;
      }

      double roughOpeningArea = totalAreaOfSubSurfaces();
      double wwr = roughOpeningArea / grossArea;

      return wwr;
    }

    // Calculates and returns the toital area of the subsurfaces
    // If any subsurface extends outside the parent surface or overlaps an adjacent subsurface
    // then that subsurface's vertices rather than rough opening vertices are used to calculate the area
    // NOTE: I'm starting to think a better way to do this would be to use polygon booleans
    // 1 - Intersect the subsurface rough opening with the parent surface to get the area inside the parent surface
    // 2 - Add all the subsurface rough openings together (so overlap areas don't count twice)
    double Surface_Impl::totalAreaOfSubSurfaces() const {
      double tol = 0.01;

      // There must be a simpler way - after all we are only intersetced in areas
      // 1 - Get the surface polygon flattened
      // 2 - Get all the sub-surface polygons with frame offset applied
      // 3 - STart with first poly
      // 4 - Does it overlap any other polys? (A & B)
      // 5 - Y: Get the combined area of the two polygons, remove polygon A & B from the list insert polygon C
      //     N: Remove polygon A form the list add to finished polygon list
      // 6 When no more overlaps are detected we are finished. Get the total area of all the polygons

      // Simple case - no sub-surfaces
      if (this->subSurfaces().size() == 0) return 0;

      // Flattened surface vertices
      Transformation surfaceToXY = Transformation::alignFace(this->vertices()).inverse();
      std::vector<Point3d> surfaceVertices = surfaceToXY * this->vertices();
      // Make sure the surface surface is oriented clockwise
      auto norm = openstudio::getOutwardNormal(surfaceVertices);
      if (norm && norm->z() > 0) {
        std::reverse(surfaceVertices.begin(), surfaceVertices.end());
      }

      Point3dVectorVector subSurfaces;

      // Get the flattened sub surface vertices for windows
      for (const SubSurface& subSurface : this->subSurfaces()) {
        if (istringEqual(subSurface.subSurfaceType(), "FixedWindow") || istringEqual(subSurface.subSurfaceType(), "OperableWindow")) {
          auto roughOpening = surfaceToXY * subSurface.roughOpeningVertices();
          auto norm = openstudio::getOutwardNormal(roughOpening);
          if (norm && norm->z() > 0) {
            std::reverse(roughOpening.begin(), roughOpening.end());
          }
          subSurfaces.push_back(roughOpening);
        }
      }

      // Add all the subsurfaces togther
      auto finalSurfaces = openstudio::joinAll(subSurfaces, tol);

      double area = 0;
      // No overlappingsurfaces so we can add up the surface areas including multipliers
      if (finalSurfaces.size() == subSurfaces.size()) {
        for (const auto& subSurface : this->subSurfaces()) {
          if (istringEqual(subSurface.subSurfaceType(), "FixedWindow") || istringEqual(subSurface.subSurfaceType(), "OperableWindow")) {
            auto roughOpening = surfaceToXY * subSurface.roughOpeningVertices();
            if (!openstudio::polygonInPolygon(roughOpening, surfaceVertices, tol)) {
              roughOpening = surfaceToXY * subSurface.vertices();
            }
            area += openstudio::getArea(roughOpening).value() * subSurface.multiplier();
          }
        }

        return area;
      }

      for (const auto& subSurface : subSurfaces) {
        area += openstudio::getArea(subSurface).value();
      }

      return area;
    }

    double Surface_Impl::skylightToRoofRatio() const {
      double result = 0.0;

      if (!istringEqual(this->surfaceType(), "RoofCeiling")) {
        return result;
      }

      double grossArea = this->grossArea();

      if (grossArea == 0) {
        return result;
      }

      double skylightArea = 0.0;
      for (const SubSurface& subSurface : this->subSurfaces()) {
        if (istringEqual(subSurface.subSurfaceType(), "Skylight")) {
          skylightArea += subSurface.multiplier() * subSurface.netArea();
        }
      }

      result = skylightArea / grossArea;

      return result;
    }

    double Surface_Impl::skylightToProjectedFloorRatio() const {
      double result = 0.0;

      if (!istringEqual(this->surfaceType(), "RoofCeiling")) {
        return result;
      }

      Point3dVector vertices = this->vertices();
      Plane horizontal(Point3d(0, 0, 0), Vector3d(0, 0, 1));
      std::vector<Point3d> projectedVertics = horizontal.project(vertices);

      boost::optional<double> grossArea = getArea(projectedVertics);

      if (!grossArea || grossArea.get() == 0) {
        return result;
      }

      double skylightArea = 0.0;
      for (const SubSurface& subSurface : this->subSurfaces()) {
        if (istringEqual(subSurface.subSurfaceType(), "Skylight")) {
          skylightArea += subSurface.multiplier() * subSurface.netArea();
        }
      }

      result = skylightArea / grossArea.get();

      return result;
    }

    boost::optional<SubSurface> Surface_Impl::setWindowToWallRatio(double wwr) {
      return setWindowToWallRatio(wwr, 0.762, true);
    }

    boost::optional<SubSurface> Surface_Impl::setWindowToWallRatio(double wwr, double desiredHeightOffset, bool heightOffsetFromFloor) {
      boost::optional<SubSurface> result;
      std::vector<SubSurface> tmp;

      double viewGlassToWallRatio = 0;
      double daylightingGlassToWallRatio = 0;
      double desiredViewGlassSillHeight = 0;
      double desiredDaylightingGlassHeaderHeight = 0;
      double exteriorShadingProjectionFactor = 0;
      double interiorShelfProjectionFactor = 0;
      boost::optional<ConstructionBase> viewGlassConstruction;
      boost::optional<ConstructionBase> daylightingGlassConstruction;

      if (heightOffsetFromFloor) {
        viewGlassToWallRatio = wwr;
        desiredViewGlassSillHeight = desiredHeightOffset;
      } else {
        daylightingGlassToWallRatio = wwr;
        desiredDaylightingGlassHeaderHeight = desiredHeightOffset;
      }

      tmp = applyViewAndDaylightingGlassRatios(viewGlassToWallRatio, daylightingGlassToWallRatio, desiredViewGlassSillHeight,
                                               desiredDaylightingGlassHeaderHeight, exteriorShadingProjectionFactor, interiorShelfProjectionFactor,
                                               viewGlassConstruction, daylightingGlassConstruction);

      if (!tmp.empty()) {
        OS_ASSERT(tmp.size() == 1);
        result = tmp[0];
      }

      return result;
    }

    std::vector<SubSurface> Surface_Impl::applyViewAndDaylightingGlassRatios(
      double viewGlassToWallRatio, double daylightingGlassToWallRatio, double desiredViewGlassSillHeight, double desiredDaylightingGlassHeaderHeight,
      double exteriorShadingProjectionFactor, double interiorShelfProjectionFactor, const boost::optional<ConstructionBase>& viewGlassConstruction,
      const boost::optional<ConstructionBase>& daylightingGlassConstruction) {
      std::vector<SubSurface> result;

      // has to be a wall
      if (!istringEqual(this->surfaceType(), "Wall")) {
        return result;
      }

      // surface cannot have any non-window sub surfaces
      for (const SubSurface& subSurface : this->subSurfaces()) {
        if (istringEqual(subSurface.subSurfaceType(), "FixedWindow") || istringEqual(subSurface.subSurfaceType(), "OperableWindow")) {
          continue;
        }
        return result;
      }

      Point3dVector vertices = this->vertices();
      Point3dVector viewVertices;
      Point3dVector daylightingVertices;
      Point3dVector exteriorShadingVertices;
      Point3dVector interiorShelfVertices;
      bool test = openstudio::applyViewAndDaylightingGlassRatios(viewGlassToWallRatio, daylightingGlassToWallRatio, desiredViewGlassSillHeight,
                                                                 desiredDaylightingGlassHeaderHeight, exteriorShadingProjectionFactor,
                                                                 interiorShelfProjectionFactor, vertices, viewVertices, daylightingVertices,
                                                                 exteriorShadingVertices, interiorShelfVertices);

      if (!test) {
        return result;
      }

      // everything ok, remove all windows
      for (SubSurface& subSurface : this->subSurfaces()) {
        if (istringEqual(subSurface.subSurfaceType(), "FixedWindow") || istringEqual(subSurface.subSurfaceType(), "OperableWindow")) {
          subSurface.remove();
        }
      }

      // add a new window
      Model model = this->model();
      auto thisSurface = this->getObject<Surface>();
      boost::optional<Space> space = this->space();

      boost::optional<SubSurface> viewWindow;
      if (!viewVertices.empty()) {
        viewWindow = SubSurface(viewVertices, model);
        result.push_back(*viewWindow);
        viewWindow->setSurface(thisSurface);

        if (viewGlassConstruction) {
          viewWindow->setConstruction(*viewGlassConstruction);
        }
      }

      boost::optional<SubSurface> daylightingWindow;
      if (!daylightingVertices.empty()) {
        daylightingWindow = SubSurface(daylightingVertices, model);
        result.push_back(*daylightingWindow);
        daylightingWindow->setSurface(thisSurface);

        if (daylightingGlassConstruction) {
          daylightingWindow->setConstruction(*daylightingGlassConstruction);
        }
      }

      boost::optional<DaylightingDeviceShelf> interiorShelf;
      if (!interiorShelfVertices.empty()) {
        InteriorPartitionSurfaceGroup interiorGroup(model);
        if (space) {
          interiorGroup.setSpace(*space);
        }

        InteriorPartitionSurface interiorSurface(interiorShelfVertices, model);
        interiorSurface.setInteriorPartitionSurfaceGroup(interiorGroup);

        OS_ASSERT(daylightingWindow);
        interiorShelf = DaylightingDeviceShelf(*daylightingWindow);
        interiorShelf->setInsideShelf(interiorSurface);
      }

      if (!exteriorShadingVertices.empty()) {
        ShadingSurfaceGroup shadingGroup(model);
        if (space) {
          shadingGroup.setSpace(*space);
        }

        OS_ASSERT(viewWindow);
        shadingGroup.setShadedSubSurface(*viewWindow);

        ShadingSurface shadingSurface(exteriorShadingVertices, model);
        shadingSurface.setShadingSurfaceGroup(shadingGroup);

        // EnergyPlus expects outside shelf to be on daylight window, we prefer to shade the view window so do not add this here
        //if (interiorShelf){
        //  interiorShelf->setOutsideShelf(shadingSurface);
        //}
      }

      return result;
    }

    std::vector<ShadingSurfaceGroup> Surface_Impl::shadingSurfaceGroups() const {
      return getObject<ModelObject>().getModelObjectSources<ShadingSurfaceGroup>(ShadingSurfaceGroup::iddObjectType());
    }

    struct PolygonAreaGreater
    {
      bool operator()(const Point3dVector& left, const Point3dVector& right) const {
        boost::optional<double> leftA = getArea(left);
        if (!leftA) {
          leftA = 0;
        }
        boost::optional<double> rightA = getArea(right);
        if (!rightA) {
          rightA = 0;
        }
        return (*leftA > *rightA);
      }
    };

    std::vector<Surface> Surface_Impl::splitSurfaceForSubSurfaces() {
      std::vector<Surface> result;

      double expand = 0.0254;
      double tol = 0.01;  // should be less than expand

      // has to be a wall
      if (!istringEqual(this->surfaceType(), "Wall")) {
        return result;
      }

      // can't have adjacent surface
      if (this->adjacentSurface()) {
        return result;
      }

      std::vector<SubSurface> subSurfaces = this->subSurfaces();
      if (subSurfaces.empty()) {
        // nothing to do
        return result;
      }

      Point3dVector vertices = this->vertices();
      Transformation transformation = Transformation::alignFace(vertices);
      Transformation inverseTransformation = transformation.inverse();
      Point3dVector faceVertices = inverseTransformation * vertices;

      if (faceVertices.size() < 3) {
        return result;
      }

      // boost polygon wants vertices in clockwise order, faceVertices must be reversed
      std::reverse(faceVertices.begin(), faceVertices.end());

      // new coordinate system has z' in direction of outward normal, y' is up
      //double xmin = std::numeric_limits<double>::max();
      //double xmax = std::numeric_limits<double>::min();
      double ymin = std::numeric_limits<double>::max();
      double ymax = std::numeric_limits<double>::min();
      for (const Point3d& faceVertex : faceVertices) {
        //xmin = std::min(xmin, faceVertex.x());
        //xmax = std::max(xmax, faceVertex.x());
        ymin = std::min(ymin, faceVertex.y());
        ymax = std::max(ymax, faceVertex.y());
      }
      if (ymin > ymax) {
        return result;
      }

      // create a mask for each sub surface
      std::vector<Point3dVector> masks;
      std::map<Handle, Point3dVector> handleToFaceVertexMap;
      for (const SubSurface& subSurface : subSurfaces) {

        Point3dVector subSurfaceFaceVertices = inverseTransformation * subSurface.vertices();
        if (subSurfaceFaceVertices.size() < 3) {
          continue;
        }

        // boost polygon wants vertices in clockwise order, subSurfaceFaceVertices must be reversed
        std::reverse(subSurfaceFaceVertices.begin(), subSurfaceFaceVertices.end());

        handleToFaceVertexMap[subSurface.handle()] = subSurfaceFaceVertices;

        double xmin = std::numeric_limits<double>::max();
        double xmax = std::numeric_limits<double>::min();
        //double ymin = std::numeric_limits<double>::max();
        //double ymax = std::numeric_limits<double>::min();
        for (const Point3d& faceVertex : subSurfaceFaceVertices) {
          xmin = std::min(xmin, faceVertex.x());
          xmax = std::max(xmax, faceVertex.x());
          //ymin = std::min(ymin, faceVertex.y());
          //ymax = std::max(ymax, faceVertex.y());
        }
        if (xmin > xmax) {
          continue;
        }

        masks.emplace_back(Point3dVector{Point3d(xmax + expand, ymax + expand, 0), Point3d(xmax + expand, ymin - expand, 0),
                                         Point3d(xmin - expand, ymin - expand, 0), Point3d(xmin - expand, ymax + expand, 0)});
      }

      // join masks
      std::vector<Point3dVector> joinedMasks = joinAll(masks, tol);

      // intersect masks with surface
      std::vector<Point3dVector> newFaces;
      newFaces.push_back(faceVertices);
      for (const Point3dVector& mask : joinedMasks) {

        std::vector<Point3dVector> tmpFaces;
        unsigned numIntersects = 0;
        for (const Point3dVector& newFace : newFaces) {
          // each mask should intersect one and only one newFace
          boost::optional<IntersectionResult> intersection = openstudio::intersect(newFace, mask, tol);

          if (intersection) {
            numIntersects += 1;
            tmpFaces.push_back(intersection->polygon1());
            auto thisTmpFaces = intersection->newPolygons1();
            tmpFaces.insert(tmpFaces.end(), thisTmpFaces.begin(), thisTmpFaces.end());
          } else {
            tmpFaces.push_back(newFace);
          }
        }

        if (numIntersects != 1) {
          LOG(Warn, "Expected each mask to intersect one and only one face");
        }

        newFaces = tmpFaces;
      }

      OS_ASSERT(!newFaces.empty());

      // sort new faces in descending order by area
      std::sort(newFaces.begin(), newFaces.end(), PolygonAreaGreater());

      // loop over all new faces to create new Surfaces
      bool changedThis = false;
      unsigned numReparented = 0;
      Model model = this->model();
      for (const Point3dVector& newFace : newFaces) {

        boost::optional<Surface> surface;
        if (!changedThis) {
          // Re-use the original surface
          changedThis = true;
          surface = getObject<Surface>();
        } else {
          // Create new surfaces
          boost::optional<ModelObject> object = this->clone(model);
          OS_ASSERT(object);
          surface = object->optionalCast<Surface>();
          OS_ASSERT(surface);
          // remove cloned sub surfaces
          for (ModelObject& child : surface->children()) {
            child.remove();
          }
          result.push_back(*surface);
        }
        OS_ASSERT(surface);

        Point3dVector vertices = newFace;
        std::reverse(vertices.begin(), vertices.end());
        vertices = transformation * vertices;
        surface->setVertices(vertices);

        // loop over all sub surfaces and reparent
        for (const auto& p : handleToFaceVertexMap) {
          // if surface includes a single point it will include them all
          if (pointInPolygon(p.second[0], newFace, tol)) {
            boost::optional<SubSurface> subSurface = model.getModelObject<SubSurface>(p.first);
            if (subSurface) {
              numReparented += 1;
              subSurface->setSurface(*surface);
            }
            continue;
          }
        }
      }

      if (numReparented != handleToFaceVertexMap.size()) {
        LOG(Warn, "Expected to reparent " << handleToFaceVertexMap.size() << " sub surfaces in splitSurfaceForSubSurfaces, but only reparented "
                                          << numReparented);
      }

      return result;
    }

    std::vector<SubSurface> Surface_Impl::createSubSurfaces(const std::vector<std::vector<Point3d>>& faces, double inset,
                                                            const boost::optional<ConstructionBase>& construction) {
      std::vector<SubSurface> result;

      double tol = 0.0254;

      if (!this->subSurfaces().empty()) {
        return result;
      }

      if (this->adjacentSurface()) {
        return result;
      }

      boost::optional<Point3d> centroid = this->centroid();
      if (!centroid) {
        return result;
      }

      Point3dVector vertices = this->vertices();
      Point3dVector insetVertices = moveVerticesTowardsPoint(vertices, *centroid, inset);

      Transformation transformation = Transformation::alignFace(vertices);
      Transformation inverseTransformation = transformation.inverse();
      Point3dVector insetFaceVertices = inverseTransformation * insetVertices;

      // boost polygon wants vertices in clockwise order, insetFaceVertices must be reversed
      std::reverse(insetFaceVertices.begin(), insetFaceVertices.end());

      Model model = this->model();
      auto surface = getObject<Surface>();
      for (const std::vector<Point3d>& face : faces) {
        Point3dVector faceVertices = inverseTransformation * face;

        // boost polygon wants vertices in clockwise order, faceVertices must be reversed
        std::reverse(faceVertices.begin(), faceVertices.end());

        boost::optional<IntersectionResult> intersection = openstudio::intersect(faceVertices, insetFaceVertices, tol);
        if (intersection) {
          Point3dVector intersectionVertices = intersection->polygon1();

          std::vector<std::vector<Point3d>> allNewFaceVertices;
          if (intersectionVertices.size() <= 4) {
            allNewFaceVertices.push_back(intersectionVertices);
          } else {
            std::vector<std::vector<Point3d>> holes;
            allNewFaceVertices = computeTriangulation(intersectionVertices, holes, tol);
          }

          // TODO: I think the copy makes sense here
          for (Point3dVector newFaceVertices : allNewFaceVertices) {

            std::reverse(newFaceVertices.begin(), newFaceVertices.end());

            Point3dVector newVertices = transformation * newFaceVertices;
            try {

              SubSurface subSurface(newVertices, model);
              subSurface.setSurface(surface);
              if (construction) {
                subSurface.setConstruction(*construction);
              }
              result.push_back(subSurface);
            } catch (const std::exception&) {
              LOG(Error, "Could not create sub-surface.");
            }
          }
        }
      }

      return result;
    }

    AirflowNetworkSurface Surface_Impl::getAirflowNetworkSurface(const AirflowNetworkComponent& surfaceAirflowLeakage) {
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

    bool Surface_Impl::setAdjacentFoundation(const FoundationKiva& kiva) {
      bool result = this->setPointer(OS_SurfaceFields::OutsideBoundaryConditionObject, kiva.handle());
      OS_ASSERT(result);
      result = this->setString(OS_SurfaceFields::OutsideBoundaryCondition, "Foundation");
      OS_ASSERT(result);
      return result;
    }

    boost::optional<AirflowNetworkSurface> Surface_Impl::airflowNetworkSurface() const {
      std::vector<AirflowNetworkSurface> myAFNSurfs =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkSurface>(AirflowNetworkSurface::iddObjectType());
      boost::optional<Surface> other = adjacentSurface();
      if (other) {
        std::vector<AirflowNetworkSurface> adjAFNSurfs =
          other.get().getImpl<detail::Surface_Impl>()->getObject<ModelObject>().getModelObjectSources<AirflowNetworkSurface>(
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

    boost::optional<FoundationKiva> Surface_Impl::adjacentFoundation() const {
      return getObject<ModelObject>().getModelObjectTarget<FoundationKiva>(OS_SurfaceFields::OutsideBoundaryConditionObject);
    }

    void Surface_Impl::resetAdjacentFoundation() {
      boost::optional<FoundationKiva> adjacentFoundation = this->adjacentFoundation();
      if (adjacentFoundation) {
        bool result = setString(OS_SurfaceFields::OutsideBoundaryConditionObject, "");
        OS_ASSERT(result);
        this->assignDefaultBoundaryCondition();
        this->assignDefaultSunExposure();
        this->assignDefaultWindExposure();
      }
    }

    boost::optional<SurfacePropertyExposedFoundationPerimeter>
      Surface_Impl::createSurfacePropertyExposedFoundationPerimeter(const std::string& exposedPerimeterCalculationMethod, double exposedPerimeter) {
      auto thisSurface = getObject<Surface>();
      std::vector<SurfacePropertyExposedFoundationPerimeter> props =
        thisSurface.getModelObjectSources<SurfacePropertyExposedFoundationPerimeter>(SurfacePropertyExposedFoundationPerimeter::iddObjectType());
      if (!props.empty()) {
        return boost::none;
      }

      SurfacePropertyExposedFoundationPerimeter prop(thisSurface, exposedPerimeterCalculationMethod, exposedPerimeter);
      return prop;
    }

    boost::optional<SurfacePropertyExposedFoundationPerimeter> Surface_Impl::surfacePropertyExposedFoundationPerimeter() const {
      std::vector<SurfacePropertyExposedFoundationPerimeter> props =
        getObject<ModelObject>().getModelObjectSources<SurfacePropertyExposedFoundationPerimeter>(
          SurfacePropertyExposedFoundationPerimeter::iddObjectType());
      if (props.empty()) {
        // no error
      } else if (props.size() == 1) {
        return props[0];
      } else {
        // error
      }
      return boost::none;
    }

    void Surface_Impl::resetSurfacePropertyExposedFoundationPerimeter() {
      boost::optional<SurfacePropertyExposedFoundationPerimeter> prop = this->surfacePropertyExposedFoundationPerimeter();
      if (prop) {
        prop->remove();
      }
    }

    std::vector<EMSActuatorNames> Surface_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Surface", "Interior Surface Convection Heat Transfer Coefficient"},
                                              {"Surface", "Exterior Surface Convection Heat Transfer Coefficient"},
                                              {"Surface", "Outdoor Air Drybulb Temperature"},
                                              {"Surface", "Outdoor Air Wetbulb Temperature"},
                                              {"Surface", "Outdoor Air Wind Speed"},
                                              {"Surface", "Outdoor Air Wind Direction"},
                                              {"Surface", "Construction State"},
                                              {"Window Shading Control", "Control Status"},
                                              {"Window Shading Control", "Slat Angle"}};
      return actuators;
    }

    std::vector<std::string> Surface_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }

    double Surface_Impl::exposedPerimeter(const Polygon3d& buildingPerimeter) const {
      Transformation tr = space()->transformation();

      double perimeter = 0;

      if (surfaceType() == "Floor" && outsideBoundaryCondition() == "Ground") {
        auto vertices = this->vertices();
        if (!vertices.empty() && vertices[0].z() == 0.0) {
          vertices = tr * vertices;
          for (size_t i = 0; i < vertices.size(); i++) {
            Point3dVector line;
            line.push_back(vertices[i]);
            line.push_back(vertices[(i + 1) % vertices.size()]);
            Point3dVectorVector overlaps = buildingPerimeter.overlap(line);
            for (const auto& overlap : overlaps) {
              perimeter += openstudio::getDistance(overlap[0], overlap[1]);
            }
          }
        }
      }

      return perimeter;
    }

  }  // namespace detail

  Surface::Surface(const std::vector<Point3d>& vertices, const Model& model) : PlanarSurface(Surface::iddObjectType(), vertices, model) {
    OS_ASSERT(getImpl<detail::Surface_Impl>());
    getImpl<detail::Surface_Impl>()->assignDefaultSurfaceType(false);
    getImpl<detail::Surface_Impl>()->assignDefaultBoundaryCondition(false);
    getImpl<detail::Surface_Impl>()->assignDefaultSunExposure(false);
    getImpl<detail::Surface_Impl>()->assignDefaultWindExposure(false);
    getImpl<detail::Surface_Impl>()->emitChangeSignals();  // emit signals here
  }

  IddObjectType Surface::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Surface);
    return result;
  }

  std::vector<std::string> Surface::validSurfaceTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SurfaceFields::SurfaceType);
  }

  std::vector<std::string> Surface::validOutsideBoundaryConditionValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SurfaceFields::OutsideBoundaryCondition);
  }

  std::vector<std::string> Surface::validSunExposureValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SurfaceFields::SunExposure);
  }

  std::vector<std::string> Surface::validWindExposureValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SurfaceFields::WindExposure);
  }

  std::string Surface::surfaceType() const {
    return getImpl<detail::Surface_Impl>()->surfaceType();
  }

  std::string Surface::outsideBoundaryCondition() const {
    return getImpl<detail::Surface_Impl>()->outsideBoundaryCondition();
  }

  bool Surface::isGroundSurface() const {
    return getImpl<detail::Surface_Impl>()->isGroundSurface();
  }

  std::string Surface::sunExposure() const {
    return getImpl<detail::Surface_Impl>()->sunExposure();
  }

  bool Surface::isSunExposureDefaulted() const {
    return getImpl<detail::Surface_Impl>()->isSunExposureDefaulted();
  }

  std::string Surface::windExposure() const {
    return getImpl<detail::Surface_Impl>()->windExposure();
  }

  bool Surface::isWindExposureDefaulted() const {
    return getImpl<detail::Surface_Impl>()->isWindExposureDefaulted();
  }

  boost::optional<double> Surface::viewFactortoGround() const {
    return getImpl<detail::Surface_Impl>()->viewFactortoGround();
  }

  bool Surface::isViewFactortoGroundDefaulted() const {
    return getImpl<detail::Surface_Impl>()->isViewFactortoGroundDefaulted();
  }

  bool Surface::isViewFactortoGroundAutocalculated() const {
    return getImpl<detail::Surface_Impl>()->isViewFactortoGroundAutocalculated();
  }

  boost::optional<double> Surface::numberofVertices() const {
    return getImpl<detail::Surface_Impl>()->numberofVertices();
  }

  bool Surface::isNumberofVerticesDefaulted() const {
    return getImpl<detail::Surface_Impl>()->isNumberofVerticesDefaulted();
  }

  bool Surface::isNumberofVerticesAutocalculated() const {
    return getImpl<detail::Surface_Impl>()->isNumberofVerticesAutocalculated();
  }

  bool Surface::setSurfaceType(const std::string& surfaceType) {
    return getImpl<detail::Surface_Impl>()->setSurfaceType(surfaceType);
  }

  bool Surface::setOutsideBoundaryCondition(const std::string& outsideBoundaryCondition) {
    return getImpl<detail::Surface_Impl>()->setOutsideBoundaryCondition(outsideBoundaryCondition);
  }

  bool Surface::setSunExposure(const std::string& sunExposure) {
    return getImpl<detail::Surface_Impl>()->setSunExposure(sunExposure);
  }

  void Surface::resetSunExposure() {
    getImpl<detail::Surface_Impl>()->resetSunExposure();
  }

  bool Surface::setWindExposure(const std::string& windExposure) {
    return getImpl<detail::Surface_Impl>()->setWindExposure(windExposure);
  }

  void Surface::resetWindExposure() {
    getImpl<detail::Surface_Impl>()->resetWindExposure();
  }

  bool Surface::setViewFactortoGround(boost::optional<double> viewFactortoGround) {
    return getImpl<detail::Surface_Impl>()->setViewFactortoGround(viewFactortoGround);
  }

  bool Surface::setViewFactortoGround(double viewFactortoGround) {
    return getImpl<detail::Surface_Impl>()->setViewFactortoGround(viewFactortoGround);
  }

  void Surface::resetViewFactortoGround() {
    getImpl<detail::Surface_Impl>()->resetViewFactortoGround();
  }

  void Surface::autocalculateViewFactortoGround() {
    getImpl<detail::Surface_Impl>()->autocalculateViewFactortoGround();
  }

  bool Surface::setNumberofVertices(boost::optional<double> numberofVertices) {
    return getImpl<detail::Surface_Impl>()->setNumberofVertices(numberofVertices);
  }

  bool Surface::setNumberofVertices(double numberofVertices) {
    return getImpl<detail::Surface_Impl>()->setNumberofVertices(numberofVertices);
  }

  void Surface::resetNumberofVertices() {
    getImpl<detail::Surface_Impl>()->resetNumberofVertices();
  }

  void Surface::autocalculateNumberofVertices() {
    getImpl<detail::Surface_Impl>()->autocalculateNumberofVertices();
  }

  SubSurfaceVector Surface::subSurfaces() const {
    return getImpl<detail::Surface_Impl>()->subSurfaces();
  }

  bool Surface::setSpace(const Space& space) {
    return getImpl<detail::Surface_Impl>()->setSpace(space);
  }

  boost::optional<Surface> Surface::adjacentSurface() const {
    return getImpl<detail::Surface_Impl>()->adjacentSurface();
  }

  bool Surface::setAdjacentSurface(Surface& surface) {
    return getImpl<detail::Surface_Impl>()->setAdjacentSurface(surface);
  }

  void Surface::resetAdjacentSurface() {
    return getImpl<detail::Surface_Impl>()->resetAdjacentSurface();
  }

  boost::optional<SurfaceControlMovableInsulation> Surface::surfaceControlMovableInsulation() const {
    return getImpl<detail::Surface_Impl>()->surfaceControlMovableInsulation();
  }

  boost::optional<SurfacePropertyConvectionCoefficients> Surface::surfacePropertyConvectionCoefficients() const {
    return getImpl<detail::Surface_Impl>()->surfacePropertyConvectionCoefficients();
  }

  boost::optional<SurfacePropertyLocalEnvironment> Surface::surfacePropertyLocalEnvironment() const {
    return getImpl<detail::Surface_Impl>()->surfacePropertyLocalEnvironment();
  }

  boost::optional<SurfacePropertyOtherSideCoefficients> Surface::surfacePropertyOtherSideCoefficients() const {
    return getImpl<detail::Surface_Impl>()->surfacePropertyOtherSideCoefficients();
  }

  bool Surface::setSurfacePropertyOtherSideCoefficients(SurfacePropertyOtherSideCoefficients& otherSideCoefficients) {
    return getImpl<detail::Surface_Impl>()->setSurfacePropertyOtherSideCoefficients(otherSideCoefficients);
  }

  void Surface::resetSurfacePropertyOtherSideCoefficients() {
    return getImpl<detail::Surface_Impl>()->resetSurfacePropertyOtherSideCoefficients();
  }

  boost::optional<SurfacePropertyOtherSideConditionsModel> Surface::surfacePropertyOtherSideConditionsModel() const {
    return getImpl<detail::Surface_Impl>()->surfacePropertyOtherSideConditionsModel();
  }

  bool Surface::setSurfacePropertyOtherSideConditionsModel(SurfacePropertyOtherSideConditionsModel& otherSideModel) {
    return getImpl<detail::Surface_Impl>()->setSurfacePropertyOtherSideConditionsModel(otherSideModel);
  }

  void Surface::resetSurfacePropertyOtherSideConditionsModel() {
    return getImpl<detail::Surface_Impl>()->resetSurfacePropertyOtherSideConditionsModel();
  }

  bool Surface::intersect(Surface& otherSurface) {
    return getImpl<detail::Surface_Impl>()->intersect(otherSurface);
  }

  boost::optional<SurfaceIntersection> Surface::computeIntersection(Surface& otherSurface) {
    return getImpl<detail::Surface_Impl>()->computeIntersection(otherSurface);
  }

  boost::optional<Surface> Surface::createAdjacentSurface(const Space& otherSpace) {
    return getImpl<detail::Surface_Impl>()->createAdjacentSurface(otherSpace);
  }

  bool Surface::isPartOfEnvelope() const {
    return getImpl<detail::Surface_Impl>()->isPartOfEnvelope();
  }

  void Surface::assignDefaultSurfaceType() {
    getImpl<detail::Surface_Impl>()->assignDefaultSurfaceType();
  }

  void Surface::assignDefaultBoundaryCondition() {
    getImpl<detail::Surface_Impl>()->assignDefaultBoundaryCondition();
  }

  void Surface::assignDefaultSunExposure() {
    getImpl<detail::Surface_Impl>()->assignDefaultSunExposure();
  }

  void Surface::assignDefaultWindExposure() {
    getImpl<detail::Surface_Impl>()->assignDefaultWindExposure();
  }

  double Surface::filmResistance() const {
    return getImpl<detail::Surface_Impl>()->filmResistance();
  }

  double Surface::windowToWallRatio() const {
    return getImpl<detail::Surface_Impl>()->windowToWallRatio();
  }

  double Surface::skylightToRoofRatio() const {
    return getImpl<detail::Surface_Impl>()->skylightToRoofRatio();
  }

  double Surface::skylightToProjectedFloorRatio() const {
    return getImpl<detail::Surface_Impl>()->skylightToRoofRatio();
  }

  boost::optional<SubSurface> Surface::setWindowToWallRatio(double wwr) {
    return getImpl<detail::Surface_Impl>()->setWindowToWallRatio(wwr);
  }

  boost::optional<SubSurface> Surface::setWindowToWallRatio(double wwr, double desiredHeightOffset, bool heightOffsetFromFloor) {
    return getImpl<detail::Surface_Impl>()->setWindowToWallRatio(wwr, desiredHeightOffset, heightOffsetFromFloor);
  }

  std::vector<SubSurface> Surface::applyViewAndDaylightingGlassRatios(double viewGlassToWallRatio, double daylightingGlassToWallRatio,
                                                                      double desiredViewGlassSillHeight, double desiredDaylightingGlassHeaderHeight,
                                                                      double exteriorShadingProjectionFactor, double interiorShelfProjectionFactor,
                                                                      const boost::optional<ConstructionBase>& viewGlassConstruction,
                                                                      const boost::optional<ConstructionBase>& daylightingGlassConstruction) {
    return getImpl<detail::Surface_Impl>()->applyViewAndDaylightingGlassRatios(
      viewGlassToWallRatio, daylightingGlassToWallRatio, desiredViewGlassSillHeight, desiredDaylightingGlassHeaderHeight,
      exteriorShadingProjectionFactor, interiorShelfProjectionFactor, viewGlassConstruction, daylightingGlassConstruction);
  }

  std::vector<SubSurface> Surface::createSubSurfaces(const std::vector<std::vector<Point3d>>& faces, double inset,
                                                     const boost::optional<ConstructionBase>& construction) {
    return getImpl<detail::Surface_Impl>()->createSubSurfaces(faces, inset, construction);
  }

  std::vector<ShadingSurfaceGroup> Surface::shadingSurfaceGroups() const {
    return getImpl<detail::Surface_Impl>()->shadingSurfaceGroups();
  }

  std::vector<Surface> Surface::splitSurfaceForSubSurfaces() {
    return getImpl<detail::Surface_Impl>()->splitSurfaceForSubSurfaces();
  }

  bool Surface::setAdjacentFoundation(const FoundationKiva& kiva) {
    return getImpl<detail::Surface_Impl>()->setAdjacentFoundation(kiva);
  }

  boost::optional<FoundationKiva> Surface::adjacentFoundation() const {
    return getImpl<detail::Surface_Impl>()->adjacentFoundation();
  }

  void Surface::resetAdjacentFoundation() {
    return getImpl<detail::Surface_Impl>()->resetAdjacentFoundation();
  }

  boost::optional<SurfacePropertyExposedFoundationPerimeter>
    Surface::createSurfacePropertyExposedFoundationPerimeter(const std::string& exposedPerimeterCalculationMethod, double exposedPerimeter) {
    return getImpl<detail::Surface_Impl>()->createSurfacePropertyExposedFoundationPerimeter(exposedPerimeterCalculationMethod, exposedPerimeter);
  }

  boost::optional<SurfacePropertyExposedFoundationPerimeter> Surface::surfacePropertyExposedFoundationPerimeter() const {
    return getImpl<detail::Surface_Impl>()->surfacePropertyExposedFoundationPerimeter();
  }

  void Surface::resetSurfacePropertyExposedFoundationPerimeter() {
    getImpl<detail::Surface_Impl>()->resetSurfacePropertyExposedFoundationPerimeter();
  }

  /// @cond
  Surface::Surface(std::shared_ptr<detail::Surface_Impl> impl) : PlanarSurface(std::move(impl)) {}
  /// @endcond

  SurfaceIntersection::SurfaceIntersection(const Surface& surface1, const Surface& surface2, const std::vector<Surface>& newSurfaces1,
                                           const std::vector<Surface>& newSurfaces2)
    : m_surface1(surface1), m_surface2(surface2), m_newSurfaces1(newSurfaces1), m_newSurfaces2(newSurfaces2) {}

  Surface SurfaceIntersection::surface1() const {
    return m_surface1;
  }

  Surface SurfaceIntersection::surface2() const {
    return m_surface2;
  }

  std::vector<Surface> SurfaceIntersection::newSurfaces1() const {
    return m_newSurfaces1;
  }

  std::vector<Surface> SurfaceIntersection::newSurfaces2() const {
    return m_newSurfaces2;
  }

  std::ostream& operator<<(std::ostream& os, const SurfaceIntersection& surfaceIntersection) {
    os << "(" << surfaceIntersection.surface1().name().get() << "), ";

    os << "(" << surfaceIntersection.surface2().name().get() << "), ";

    os << "[";
    std::vector<Surface> newSurfaces1 = surfaceIntersection.newSurfaces1();
    for (unsigned i = 0; i < newSurfaces1.size(); ++i) {
      os << newSurfaces1[i].name().get();
      if (i != newSurfaces1.size() - 1) {
        os << ", ";
      }
    }

    os << "], [";
    std::vector<Surface> newSurfaces2 = surfaceIntersection.newSurfaces2();
    for (unsigned i = 0; i < newSurfaces2.size(); ++i) {
      os << newSurfaces2[i].name().get();
      if (i != newSurfaces2.size() - 1) {
        os << ", ";
      }
    }
    os << "]";

    return os;
  }

  AirflowNetworkSurface Surface::getAirflowNetworkSurface(const AirflowNetworkDetailedOpening& surfaceAirflowLeakage) {
    return getImpl<detail::Surface_Impl>()->getAirflowNetworkSurface(surfaceAirflowLeakage);
  }

  AirflowNetworkSurface Surface::getAirflowNetworkSurface(const AirflowNetworkSimpleOpening& surfaceAirflowLeakage) {
    return getImpl<detail::Surface_Impl>()->getAirflowNetworkSurface(surfaceAirflowLeakage);
  }

  AirflowNetworkSurface Surface::getAirflowNetworkSurface(const AirflowNetworkCrack& surfaceAirflowLeakage) {
    return getImpl<detail::Surface_Impl>()->getAirflowNetworkSurface(surfaceAirflowLeakage);
  }

  AirflowNetworkSurface Surface::getAirflowNetworkSurface(const AirflowNetworkEffectiveLeakageArea& surfaceAirflowLeakage) {
    return getImpl<detail::Surface_Impl>()->getAirflowNetworkSurface(surfaceAirflowLeakage);
  }

  AirflowNetworkSurface Surface::getAirflowNetworkSurface(const AirflowNetworkHorizontalOpening& surfaceAirflowLeakage) {
    return getImpl<detail::Surface_Impl>()->getAirflowNetworkSurface(surfaceAirflowLeakage);
  }

  AirflowNetworkSurface Surface::getAirflowNetworkSurface(const AirflowNetworkSpecifiedFlowRate& surfaceAirflowLeakage) {
    return getImpl<detail::Surface_Impl>()->getAirflowNetworkSurface(surfaceAirflowLeakage);
  }

  boost::optional<AirflowNetworkSurface> Surface::airflowNetworkSurface() const {
    return getImpl<detail::Surface_Impl>()->airflowNetworkSurface();
  }

  double Surface::exposedPerimeter(const Polygon3d& buildingPerimeter) const {
    return getImpl<detail::Surface_Impl>()->exposedPerimeter(buildingPerimeter);
  }
}  // namespace model
}  // namespace openstudio
