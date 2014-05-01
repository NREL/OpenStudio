/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/SubSurface.hpp>
#include <model/SubSurface_Impl.hpp>
#include <model/ShadingSurfaceGroup.hpp>
#include <model/ShadingSurfaceGroup_Impl.hpp>
#include <model/ConstructionBase.hpp>
#include <model/ConstructionBase_Impl.hpp>
#include <model/LayeredConstruction.hpp>
#include <model/LayeredConstruction_Impl.hpp>
#include <model/StandardsInformationConstruction.hpp>
#include <model/DaylightingDeviceShelf.hpp>
#include <model/ShadingSurface.hpp>
#include <model/InteriorPartitionSurfaceGroup.hpp>
#include <model/InteriorPartitionSurface.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Surface_FieldEnums.hxx>

#include <utilities/geometry/Transformation.hpp>
#include <utilities/geometry/Geometry.hpp>
#include <utilities/geometry/Intersection.hpp>
#include <utilities/core/Assert.hpp>

#include <utilities/sql/SqlFile.hpp>

#include <QPolygon>

using boost::to_upper_copy;

namespace openstudio {
namespace model {

namespace detail {

  Surface_Impl::Surface_Impl(const IdfObject& idfObject,
                             Model_Impl* model, 
                             bool keepHandle)
    : PlanarSurface_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Surface::iddObjectType());
  }

  Surface_Impl::Surface_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                             Model_Impl* model,
                             bool keepHandle)
    : PlanarSurface_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Surface::iddObjectType());
  }

  Surface_Impl::Surface_Impl(const Surface_Impl& other,
                             Model_Impl* model,
                             bool keepHandle)
    : PlanarSurface_Impl(other,model,keepHandle)
  {
  }

  Surface_Impl::~Surface_Impl()
  {
  }

 boost::optional<ParentObject> Surface_Impl::parent() const
 {
   boost::optional<ParentObject> result;
   result = this->space();
   return result;
 }

 bool Surface_Impl::setParent(ParentObject& newParent)
 {
   bool result = false;
   if (newParent.optionalCast<Space>()){
     result = this->setSpace(newParent.cast<Space>());
   }
   return result;
 }

 std::vector<ModelObject> Surface_Impl::children() const
 {
   std::vector<ModelObject> result;

   // subSurfaces
   SubSurfaceVector subSurfaces = this->subSurfaces();
   result.insert(result.end(), subSurfaces.begin(), subSurfaces.end());

   return result;
 }

  std::vector<IdfObject> Surface_Impl::remove()
  {
    boost::optional<Surface> adjacentSurface = this->adjacentSurface();
    if (adjacentSurface){
      this->resetAdjacentSurface();
    }

    return ParentObject_Impl::remove();
  }

  std::vector<IddObjectType> Surface_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_SubSurface);

    return result;
  }

  const std::vector<std::string>& Surface_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
      result.push_back("Surface Inside Face Temperature");
      result.push_back("Surface Outside Face Temperature");
    }
    return result;
  }

  IddObjectType Surface_Impl::iddObjectType() const {
    return Surface::iddObjectType();
  }

  bool Surface_Impl::subtractFromGrossArea() const
  {
    return false;
  }

  boost::optional<ConstructionBase> Surface_Impl::construction() const
  {
    // DLM: there is duplicate code in ForwardTranslator::resolveMatchedSurfaceConstructionConflicts
    // if you update this code you must update that code

    // DLM: I am not sure we should be doing this here at all, maybe this method should just
    // return the same thing constructionWithSearchDistance does?

    boost::optional<std::pair<ConstructionBase, int> > constructionWithSearchDistance = this->constructionWithSearchDistance();

    model::OptionalSurface adjacentSurface = this->adjacentSurface();
    if (!adjacentSurface){
      if (constructionWithSearchDistance){
        return constructionWithSearchDistance->first;
      }
      return boost::none;
    }

    boost::optional<std::pair<model::ConstructionBase, int> > adjacentConstructionWithSearchDistance = adjacentSurface->constructionWithSearchDistance();

    if (constructionWithSearchDistance && !adjacentConstructionWithSearchDistance){
      // return this construction
      return constructionWithSearchDistance->first;
    }

    if (!constructionWithSearchDistance && adjacentConstructionWithSearchDistance){
      // return adjacent construction
      return adjacentConstructionWithSearchDistance->first;
    }

    if (!constructionWithSearchDistance && !adjacentConstructionWithSearchDistance){
      // no constructions, nothing to be done
      return boost::none;
    }

    // both surfaces return a construction

    if (constructionWithSearchDistance->first.handle() == adjacentConstructionWithSearchDistance->first.handle()){
      // both surfaces have same construction
      return constructionWithSearchDistance->first;
    }

    // both surfaces return a construction and they are not the same

    if (constructionWithSearchDistance->second < adjacentConstructionWithSearchDistance->second){
      // lower search distance to construction
      return constructionWithSearchDistance->first;
    }

    if (constructionWithSearchDistance->second > adjacentConstructionWithSearchDistance->second){
      // lower search distance to adjacent construction
      return adjacentConstructionWithSearchDistance->first;
    }

    // both surfaces return a construction, they are not the same, and both have same search distance

    if (constructionWithSearchDistance->first.optionalCast<model::LayeredConstruction>() && 
        adjacentConstructionWithSearchDistance->first.optionalCast<model::LayeredConstruction>()){
      if (constructionWithSearchDistance->first.cast<model::LayeredConstruction>().reverseEqualLayers(adjacentConstructionWithSearchDistance->first.cast<model::LayeredConstruction>())){
        // these constructions are reverse equal
        return constructionWithSearchDistance->first;
      }
    }

    // give up for now, we can add more later
    // DLM: should we return empty or this construction?
    return constructionWithSearchDistance->first;
  }

  boost::optional<std::pair<ConstructionBase, int> > Surface_Impl::constructionWithSearchDistance() const
  {
    boost::optional<std::pair<ConstructionBase, int> > result;

    boost::optional<ConstructionBase> construction = getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_SurfaceFields::ConstructionName);
    if (construction){
      return std::make_pair<ConstructionBase, int>(*construction, 0);
    }
    
    boost::optional<Space> space = this->space();
    if (space){
      result = space->getDefaultConstructionWithSearchDistance(this->getObject<Surface>());
    }
    return result;
  }

  bool Surface_Impl::isConstructionDefaulted() const
  {
    return isEmpty(OS_SurfaceFields::ConstructionName);
  }

  bool Surface_Impl::setVertices(const std::vector<Point3d>& vertices)
  {
    bool result = PlanarSurface_Impl::setVertices(vertices);

    if (isEmpty(OS_SurfaceFields::SurfaceType)){
      if (result){
        this->assignDefaultSurfaceType(false);
        this->assignDefaultBoundaryCondition(false);
        this->assignDefaultSunExposure(false);
        this->assignDefaultWindExposure(false);
        this->emitChangeSignals(); // emit signals here
      }else{
        LOG(Error, "Cannot compute default Surface properties.");
      }
    }

    return result;
  }

  bool Surface_Impl::setConstruction(const ConstructionBase& construction)
  {
    return setPointer(OS_SurfaceFields::ConstructionName, construction.handle());
  }

  void Surface_Impl::resetConstruction()
  {
    setString(OS_SurfaceFields::ConstructionName, "");
  }

  std::string Surface_Impl::surfaceType() const {
    boost::optional<std::string> value = getString(OS_SurfaceFields::SurfaceType,true,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::vector<std::string> Surface_Impl::surfaceTypeValues() const
  {
    return Surface::validSurfaceTypeValues();
  }

  std::string Surface_Impl::outsideBoundaryCondition() const {
    boost::optional<std::string> value = getString(OS_SurfaceFields::OutsideBoundaryCondition,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::vector<std::string> Surface_Impl::outsideBoundaryConditionValues() const
  {
    return Surface::validOutsideBoundaryConditionValues();
  }

  bool Surface_Impl::isGroundSurface() const
  {
    std::string outsideBoundaryCondition = this->outsideBoundaryCondition();
   
    if (istringEqual("Ground", outsideBoundaryCondition) || 
        istringEqual("GroundFCfactorMethod", outsideBoundaryCondition) ||
        istringEqual("GroundSlabPreprocessorAverage", outsideBoundaryCondition) ||
        istringEqual("GroundSlabPreprocessorCore", outsideBoundaryCondition) ||
        istringEqual("GroundSlabPreprocessorPerimeter", outsideBoundaryCondition) ||
        istringEqual("GroundBasementPreprocessorAverageWall", outsideBoundaryCondition) ||
        istringEqual("GroundBasementPreprocessorAverageFloor", outsideBoundaryCondition) ||
        istringEqual("GroundBasementPreprocessorUpperWall", outsideBoundaryCondition) ||
        istringEqual("GroundBasementPreprocessorLowerWall", outsideBoundaryCondition)){
          return true;
    }
    
    return false;
  }

  std::string Surface_Impl::sunExposure() const {
    boost::optional<std::string> value = getString(OS_SurfaceFields::SunExposure,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::vector<std::string> Surface_Impl::sunExposureValues() const
  {
    return Surface::validSunExposureValues();
  }

  bool Surface_Impl::isSunExposureDefaulted() const {
    return isEmpty(OS_SurfaceFields::SunExposure);
  }

  std::string Surface_Impl::windExposure() const {
    boost::optional<std::string> value = getString(OS_SurfaceFields::WindExposure,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::vector<std::string> Surface_Impl::windExposureValues() const
  {
    return Surface::validWindExposureValues();
  }

  bool Surface_Impl::isWindExposureDefaulted() const {
    return isEmpty(OS_SurfaceFields::WindExposure);
  }

  boost::optional<double> Surface_Impl::viewFactortoGround() const {
    return getDouble(OS_SurfaceFields::ViewFactortoGround,true);
  }

  bool Surface_Impl::isViewFactortoGroundDefaulted() const {
    return isEmpty(OS_SurfaceFields::ViewFactortoGround);
  }

  bool Surface_Impl::isViewFactortoGroundAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_SurfaceFields::ViewFactortoGround, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "canAutocalculate");
    }
    return result;
  }

  boost::optional<double> Surface_Impl::numberofVertices() const {
    return getDouble(OS_SurfaceFields::NumberofVertices,true);
  }

  bool Surface_Impl::isNumberofVerticesDefaulted() const {
    return isEmpty(OS_SurfaceFields::NumberofVertices);
  }

  bool Surface_Impl::isNumberofVerticesAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_SurfaceFields::NumberofVertices, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "canAutocalculate");
    }
    return result;
  }

  bool Surface_Impl::setSurfaceType(std::string surfaceType) {
    return setSurfaceType(surfaceType, true);
  }

  bool Surface_Impl::setSurfaceType(std::string surfaceType, bool driverMethod) {
    bool result = setString(OS_SurfaceFields::SurfaceType, surfaceType, true);
    if (driverMethod){
      this->emitChangeSignals(); // emit signals here
    }
    return result;
  }

  bool Surface_Impl::setOutsideBoundaryCondition(std::string outsideBoundaryCondition) {
    return setOutsideBoundaryCondition(outsideBoundaryCondition, true);
  }

  bool Surface_Impl::setOutsideBoundaryCondition(std::string outsideBoundaryCondition, bool driverMethod) {
    bool result = false;
    
    boost::optional<Surface> adjacentSurface = this->adjacentSurface();

    if (istringEqual("Surface", outsideBoundaryCondition)){
      if (adjacentSurface){
        result = setString(OS_SurfaceFields::OutsideBoundaryCondition, outsideBoundaryCondition, true);
      }
    }else{
      this->resetAdjacentSurface();
      result = setString(OS_SurfaceFields::OutsideBoundaryCondition, outsideBoundaryCondition, true);
      if (result){
        this->assignDefaultSunExposure(false);
        this->assignDefaultWindExposure(false);
      }else if(adjacentSurface){
        // restore the adjacent surface if set boundary condition fails
        bool test = setAdjacentSurface(*adjacentSurface);
        OS_ASSERT(test);
      }
    }

    if (driverMethod){
      this->emitChangeSignals(); // emit signals here
    }

    return result;
  }

  bool Surface_Impl::setSunExposure(std::string sunExposure) {
    return setSunExposure(sunExposure, true);
  }

  bool Surface_Impl::setSunExposure(std::string sunExposure, bool driverMethod) {
    bool result = setString(OS_SurfaceFields::SunExposure, sunExposure, true);
    if (driverMethod){
      this->emitChangeSignals(); // emit signals here
    }
    return result;
  }

  void Surface_Impl::resetSunExposure() {
    bool result = setString(OS_SurfaceFields::SunExposure, "");
    OS_ASSERT(result);
  }

  bool Surface_Impl::setWindExposure(std::string windExposure) {
    return setWindExposure(windExposure, true);
  }

  bool Surface_Impl::setWindExposure(std::string windExposure, bool driverMethod) {
    bool result = setString(OS_SurfaceFields::WindExposure, windExposure, true);
    if (driverMethod){
      this->emitChangeSignals(); // emit signals here
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

  SubSurfaceVector Surface_Impl::subSurfaces() const
  {
    SubSurfaceVector result;
    ModelObject object = getObject<ModelObject>();
    WorkspaceObjectVector idfSubSurfaces = object.getSources(IddObjectType(IddObjectType::OS_SubSurface));
    BOOST_FOREACH(const WorkspaceObject& idfSubSurface, idfSubSurfaces){
      OptionalSubSurface subSurface = this->model().getModelObject<SubSurface>(idfSubSurface.handle());
      if (subSurface){
        result.push_back(*subSurface);
      }
    }
    return result;
  }

  boost::optional<PlanarSurfaceGroup> Surface_Impl::planarSurfaceGroup() const
  {
    return boost::optional<PlanarSurfaceGroup>(this->space());
  }

  boost::optional<Space> Surface_Impl::space() const
  {
    boost::optional<Space> result;
    OptionalWorkspaceObject space = getTarget(OS_SurfaceFields::SpaceName);
    if (space){
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
      // opaque exterior
      if (sqlFile && constructionName && oConstruction->isOpaque()) {
        std::string query = "SELECT RowId FROM tabulardatawithstrings WHERE ReportName='EnvelopeSummary' AND ReportForString='Entire Facility' AND TableName='Opaque Exterior' AND ColumnName='Construction' AND Value='" + 
            to_upper_copy(*constructionName) + "'";
        OptionalInt rowId = sqlFile->execAndReturnFirstInt(query);
        if (rowId) {
          std::stringstream ss;
          ss << "SELECT Value FROM tabulardatawithstrings WHERE ReportName='EnvelopeSummary' AND ReportForString='Entire Facility' AND TableName='Opaque Exterior' AND RowId='";
          ss << *rowId << "' AND ColumnName='U-Factor with Film' AND Units='W/m2-K'";
          query = ss.str();
          outputResult = sqlFile->execAndReturnFirstDouble(query);
        }
      }
      // fenestration
      if (sqlFile && constructionName && oConstruction->isFenestration()) {
        std::string query = "SELECT RowId FROM tabulardatawithstrings WHERE ReportName='EnvelopeSummary' and ReportForString = 'Entire Facility' AND TableName = 'Exterior Fenestration' AND ColumnName='Construction' AND Value='" + to_upper_copy(*constructionName) + "'";
        OptionalInt rowId = sqlFile->execAndReturnFirstInt(query);
        if (rowId) {
          std::stringstream ss;
          ss << "SELECT Value FROM tabulardatawithstrings WHERE ReportName='EnvelopeSummary' and ReportForString = 'Entire Facility' AND TableName = 'Exterior Fenestration' AND RowId='";
          ss << *rowId << "' AND ColumnName ='Glass U-Factor' AND Units = 'W/m2-K'";
          query = ss.str();
          outputResult = sqlFile->execAndReturnFirstDouble(query);
        }
      }

      if (inputResult) {
        result = inputResult;
        if (outputResult) {
          compareInputAndOutput(*oConstruction,"uFactor",*inputResult,*outputResult,1.0E-3);
        }
      }
      else { result = outputResult; }
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
      // opaque exterior
      if (sqlFile && constructionName && oConstruction->isOpaque()) {
        std::string query = "SELECT RowId FROM tabulardatawithstrings WHERE ReportName='EnvelopeSummary' AND ReportForString='Entire Facility' AND TableName='Opaque Exterior' AND ColumnName='Construction' AND Value='" + to_upper_copy(*constructionName) + "'";
        OptionalInt rowId = sqlFile->execAndReturnFirstInt(query);
        if (rowId) {
          std::stringstream ss;
          ss << "SELECT Value FROM tabulardatawithstrings WHERE ReportName='EnvelopeSummary' AND ReportForString='Entire Facility' AND TableName='Opaque Exterior' AND RowId='";
          ss << *rowId << "' AND ColumnName='U-Factor no Film' AND Units='W/m2-K'";
          query = ss.str();
          outputResult = sqlFile->execAndReturnFirstDouble(query);
        }
      }
      // fenestration
      if (sqlFile && constructionName && oConstruction->isFenestration()) {
        // get u-factor, then subtract film coefficients
        std::string query = "SELECT RowId FROM tabulardatawithstrings WHERE ReportName='EnvelopeSummary' and ReportForString = 'Entire Facility' AND TableName = 'Exterior Fenestration' AND ColumnName='Construction' AND Value='" + to_upper_copy(*constructionName) + "'";
        OptionalInt rowId = sqlFile->execAndReturnFirstInt(query);
        if (rowId) {
          std::stringstream ss;
          ss << "SELECT Value FROM tabulardatawithstrings WHERE ReportName='EnvelopeSummary' and ReportForString = 'Entire Facility' AND TableName = 'Exterior Fenestration' AND RowId='";
          ss << *rowId << "' AND ColumnName ='Glass U-Factor' AND Units = 'W/m2-K'";
          query = ss.str();
          outputResult = sqlFile->execAndReturnFirstDouble(query);
        }
        if (outputResult) {
          outputResult = 1.0/(1.0/(*outputResult) - filmResistance());
        }
      }

      if (inputResult) {
        result = inputResult;
        if (outputResult) {
          compareInputAndOutput(*oConstruction,"thermal conductance",*inputResult,*outputResult,1.0E-5);
        }
      }
      else { result = outputResult; }
    }

    return result;
  }

  bool Surface_Impl::setUFactor(double value) {
    OptionalConstructionBase oConstruction = this->construction();
    if (oConstruction) {
      return oConstruction->setUFactor(value,filmResistance());
    }
    return false;
  }

  bool Surface_Impl::setThermalConductance(double value) {
    OptionalConstructionBase oConstruction = this->construction();
    if (oConstruction) {
      return oConstruction->setConductance(value,filmResistance());
    }
    return false;
  }

  bool Surface_Impl::setSpace(const Space& space)
  {
    return setPointer(OS_SurfaceFields::SpaceName, space.handle());
  }

  boost::optional<Surface> Surface_Impl::adjacentSurface() const {
    return getObject<Surface>().getModelObjectTarget<Surface>(OS_SurfaceFields::OutsideBoundaryConditionObject);
  }

  bool Surface_Impl::setAdjacentSurface(Surface& surface) {

    // matching surface with self is ok for stories with multipliers
    bool isSameSurface = false;
    if (surface.handle() == this->handle()){
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
    if (otherAdjacentSurface && (otherAdjacentSurface->handle() == this->handle())){
      // if this is pointing to other surface
      if (adjacentSurface && (adjacentSurface->handle() == surface.handle())){
        isNewMatch = false;
      }
    }

    // this is basically testing if surface is in same model as this
    bool test = this->setPointer(OS_SurfaceFields::OutsideBoundaryConditionObject, surface.handle());
    if (test){

      // clean all other surfaces pointing to this (unless it is surface)
      BOOST_FOREACH(WorkspaceObject wo, this->getSources(IddObjectType::OS_Surface)){
        if (wo.handle() == surface.handle()){
          continue;
        }

        Surface otherSurface = wo.cast<Surface>();
        test = otherSurface.setString(OS_SurfaceFields::OutsideBoundaryConditionObject, "");
        OS_ASSERT(test);
        otherSurface.assignDefaultBoundaryCondition();
        otherSurface.assignDefaultSunExposure();
        otherSurface.assignDefaultWindExposure();

        BOOST_FOREACH(SubSurface subSurface, otherSurface.subSurfaces()){
          subSurface.resetAdjacentSubSurface();
        }
      }

      if (!isSameSurface){
        // clean all other surfaces pointing to surface (unless it is this)
        BOOST_FOREACH(WorkspaceObject wo, surface.getSources(IddObjectType::OS_Surface)){
          if (wo.handle() == this->handle()){
            continue;
          }

          Surface otherSurface = wo.cast<Surface>();
          test = otherSurface.setString(OS_SurfaceFields::OutsideBoundaryConditionObject, "");
          OS_ASSERT(test);
          otherSurface.assignDefaultBoundaryCondition();
          otherSurface.assignDefaultSunExposure();
          otherSurface.assignDefaultWindExposure();

          BOOST_FOREACH(SubSurface subSurface, otherSurface.subSurfaces()){
            subSurface.resetAdjacentSubSurface();
          }
        }
      }

      // this and surface are newly pointing to each other, clean sub surfaces on both
      if (isNewMatch){
        BOOST_FOREACH(SubSurface subSurface, this->subSurfaces()){
          subSurface.resetAdjacentSubSurface();
        }
        if (!isSameSurface){
          BOOST_FOREACH(SubSurface subSurface, surface.subSurfaces()){
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

      if (!isSameSurface){
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
    if (adjacentSurface){
      test = setString(OS_SurfaceFields::OutsideBoundaryConditionObject, "");
      OS_ASSERT(test);
      this->assignDefaultBoundaryCondition();
      this->assignDefaultSunExposure();
      this->assignDefaultWindExposure();
    }

    // unset all matched sub surfaces
    BOOST_FOREACH(SubSurface subSurface, this->subSurfaces()){
      subSurface.resetAdjacentSubSurface();
    }

    // clean all other surfaces pointing to this
    BOOST_FOREACH(WorkspaceObject wo, this->getSources(IddObjectType::OS_Surface)){

      Surface otherSurface = wo.cast<Surface>();
      test = otherSurface.setString(OS_SurfaceFields::OutsideBoundaryConditionObject, "");
      OS_ASSERT(test);
      otherSurface.assignDefaultBoundaryCondition();
      otherSurface.assignDefaultSunExposure();
      otherSurface.assignDefaultWindExposure();

      BOOST_FOREACH(SubSurface subSurface, otherSurface.subSurfaces()){
        subSurface.resetAdjacentSubSurface();
      }
    }
  }

  bool Surface_Impl::intersect(Surface& otherSurface){
    boost::optional<SurfaceIntersection> intersection = computeIntersection(otherSurface);
    if (intersection){
      return true;
    }
    return false;
  }

  boost::optional<SurfaceIntersection> Surface_Impl::computeIntersection(Surface& otherSurface)
  {
    double tol = 0.01; // 1 cm tolerance

    boost::optional<Space> space = this->space();
    boost::optional<Space> otherSpace = otherSurface.space();
    if (!space || !otherSpace || space->handle() == otherSpace->handle()){
      LOG(Error, "Cannot find spaces for each surface in intersection or surfaces in same space.");
      return boost::none;
    }

    if (!this->subSurfaces().empty() || !otherSurface.subSurfaces().empty()){
      LOG(Error, "Subsurfaces are not allowed in intersection");
      return boost::none;
    }

    if (this->adjacentSurface() || otherSurface.adjacentSurface()){
      LOG(Error, "Adjacent surfaces are not allowed in intersection");
      return boost::none;
    }

    // goes from local system to building coordinates
    Transformation spaceTransformation = space->transformation();
    Transformation otherSpaceTransformation = otherSpace->transformation();

    // do the intersection in building coordinates

    Plane plane = spaceTransformation * this->plane();
    Plane otherPlane = otherSpaceTransformation * otherSurface.plane();

    if (!plane.reverseEqual(otherPlane)){
      //LOG(Info, "Planes are not reverse equal, intersection of '" << this->name().get() << "' with '" << otherSurface.name().get() << "' fails");
      return boost::none;
    }

    // get vertices in building coordinates
    std::vector<Point3d> buildingVertices = spaceTransformation * this->vertices();
    std::vector<Point3d> otherBuildingVertices = otherSpaceTransformation * otherSurface.vertices();

    if ((buildingVertices.size() < 3) || (otherBuildingVertices.size() < 3)){
      LOG(Error, "Fewer than 3 vertices, intersection of '" << this->name().get() << "' with '" << otherSurface.name().get() << "' fails");
      return boost::none;
    }

    // goes from face coordinates of building vertices to building coordinates
    Transformation faceTransformation; 
    Transformation faceTransformationInverse;
    try {
      faceTransformation = Transformation::alignFace(buildingVertices);
      faceTransformationInverse = faceTransformation.inverse();
    }catch(const std::exception&){
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

    boost::optional<IntersectionResult> intersection = openstudio::intersect(faceVertices, otherFaceVertices, tol);
    if (!intersection){
      //LOG(Info, "No intersection");
      return boost::none;
    }

    // non-zero intersection
    // could match here but will save that for other discrete operation
    Surface surface(boost::dynamic_pointer_cast<Surface_Impl>(this->shared_from_this()));
    std::vector<Surface> newSurfaces;
    std::vector<Surface> newOtherSurfaces;

    //LOG(Debug, "Before intersection");
    //LOG(Debug, surface);
    //LOG(Debug, otherSurface);

    // goes from building coordinates to local system 
    Transformation spaceTransformationInverse = spaceTransformation.inverse();
    Transformation otherSpaceTransformationInverse = otherSpaceTransformation.inverse();

    std::vector< std::vector<Point3d> > newPolygons1 = intersection->newPolygons1();
    std::vector< std::vector<Point3d> > newPolygons2 = intersection->newPolygons2();
    if (newPolygons1.empty() && newPolygons2.empty()){
      // both surfaces intersect perfectly, no-op

    }else{
      // new surfaces are created

      // modify vertices for surface in this space
      std::vector<Point3d> newBuildingVertices = faceTransformation * intersection->polygon1();
      std::vector<Point3d> newVertices = spaceTransformationInverse * newBuildingVertices;
      std::reverse(newVertices.begin(), newVertices.end());
      newVertices = reorderULC(newVertices);
      this->setVertices(newVertices);

      // modify vertices for surface in other space
      std::vector<Point3d> newOtherBuildingVertices = faceTransformation * intersection->polygon2();
      std::vector<Point3d> newOtherVertices = otherSpaceTransformationInverse * newOtherBuildingVertices;
      newOtherVertices = reorderULC(newOtherVertices);
      otherSurface.setVertices(newOtherVertices);

      // create new surfaces in this space
      for (unsigned i = 0; i < newPolygons1.size(); ++i){

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
      for (unsigned i = 0; i < newPolygons2.size(); ++i){

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
    //BOOST_FOREACH(const Surface& s, newSurfaces){
    //  LOG(Debug, s);
    //}
    //BOOST_FOREACH(const Surface& s, newOtherSurfaces){
    //  LOG(Debug, s);
    //}

    return result;
  }

  boost::optional<Surface> Surface_Impl::createAdjacentSurface(const Space& otherSpace)
  {
    boost::optional<Space> space = this->space();
    if (!space || (space->handle() == otherSpace.handle())){
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
    otherSurface.resetConstruction(); // this will use surface's construction on export, TODO: do something better

    std::string surfaceType = this->surfaceType();
    if (surfaceType == "RoofCeiling"){
      otherSurface.setSurfaceType("Floor");
    }else if (surfaceType == "Floor"){
      otherSurface.setSurfaceType("RoofCeiling");
    }else if (surfaceType == "Wall"){
      otherSurface.setSurfaceType("Wall");
    }

    otherSurface.setSpace(otherSpace);

    this->setAdjacentSurface(otherSurface);

    BOOST_FOREACH(SubSurface subSurface, this->subSurfaces()){
      vertices = transformation * subSurface.vertices();
      std::reverse(vertices.begin(), vertices.end());

      SubSurface otherSubSurface(vertices, model);
      otherSubSurface.setName(subSurface.name().get() + " Reversed");
      otherSubSurface.resetConstruction(); // this will use sub surface's construction on export, TODO: do something better
      otherSubSurface.setSubSurfaceType(subSurface.subSurfaceType());
      otherSubSurface.setSurface(otherSurface);
      otherSubSurface.setAdjacentSubSurface(subSurface);
    } 

    return otherSurface;
  }

  bool Surface_Impl::isPartOfEnvelope() const
  {
    std::string bc = this->outsideBoundaryCondition();
    bool result = (istringEqual("Outdoors", bc) || this->isGroundSurface());
    return result;
  }

  void Surface_Impl::assignDefaultSurfaceType()
  {
    assignDefaultSurfaceType(true);
  }

  void Surface_Impl::assignDefaultSurfaceType(bool driverMethod)
  {
    double degTilt = radToDeg(this->tilt());
    bool ok = true;
    if (degTilt < 60){
      ok = setSurfaceType("RoofCeiling", driverMethod);
      OS_ASSERT(ok);
    }else if(degTilt < 179){
      ok = setSurfaceType("Wall", driverMethod);
      OS_ASSERT(ok);
    }else{
      ok = setSurfaceType("Floor", driverMethod);
      OS_ASSERT(ok);
    }
  }

  void Surface_Impl::assignDefaultBoundaryCondition()
  {
    assignDefaultBoundaryCondition(true);
  }

  void Surface_Impl::assignDefaultBoundaryCondition(bool driverMethod)
  {
    if (this->adjacentSurface()){
      bool test = this->setOutsideBoundaryCondition("Surface", driverMethod);
      OS_ASSERT(test);
    }else if (istringEqual("Floor", this->surfaceType())){
      bool test = this->setOutsideBoundaryCondition("Ground", driverMethod);
      OS_ASSERT(test);
    }else{
      bool test = this->setOutsideBoundaryCondition("Outdoors", driverMethod);
      OS_ASSERT(test);
    }
  }

  void Surface_Impl::assignDefaultSunExposure()
  {
    assignDefaultSunExposure(true);
  }

  void Surface_Impl::assignDefaultSunExposure(bool driverMethod)
  {
    //std::string outsideBoundaryCondition = this->outsideBoundaryCondition();
    if (istringEqual("Outdoors", this->outsideBoundaryCondition())){
      bool test = this->setSunExposure("SunExposed", driverMethod);
      OS_ASSERT(test);
    }else{
      bool test = this->setSunExposure("NoSun", driverMethod);
      OS_ASSERT(test);
    }
  }
     
  void Surface_Impl::assignDefaultWindExposure()
  {
    assignDefaultWindExposure(true);
  }

  void Surface_Impl::assignDefaultWindExposure(bool driverMethod)
  {
    //std::string outsideBoundaryCondition = this->outsideBoundaryCondition();
    if (istringEqual("Outdoors", this->outsideBoundaryCondition())){
      bool test = setWindExposure("WindExposed", driverMethod);
      OS_ASSERT(test);
    }else{
      bool test = setWindExposure("NoWind", driverMethod);
      OS_ASSERT(test);
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
      }
      else {
        return false;
      }
    }
    else {
      resetAdjacentSurface();
    }
    return true;
  }

    double Surface_Impl::filmResistance() const {
      double interiorResistance = PlanarSurface::stillAirFilmResistance(tilt());
      std::string obc = this->outsideBoundaryCondition();
      boost::to_lower(obc);
      if (obc == "outdoors") {
        return PlanarSurface::filmResistance(FilmResistanceType::MovingAir_15mph) +
               interiorResistance;
      }
      else if (isPartOfEnvelope()) {
        // ground, underground, or other side coefficients/conditions
        // assume one interiorResistance
        if ((obc == "othersidecoefficients") || (obc == "othersideconditionsmodel")) {
          LOG(Warn,"Returning film resistance for " << briefDescription() << " as if it was "
              "underground (one still air film resistance for the inside surface). This may not "
              "be correct as the outside boundary condition is '" << obc << "'.");
        }
        return interiorResistance;
      }
      return 2.0*interiorResistance;
    }

  double Surface_Impl::windowToWallRatio() const
  {
    double result = 0.0;

    if (!istringEqual(this->surfaceType(), "Wall")){
      return result;
    }

    double grossArea = this->grossArea();
    
    if (grossArea == 0){
      return result;
    }

    double windowArea = 0.0;
    BOOST_FOREACH(const SubSurface& subSurface, this->subSurfaces()){
      if (istringEqual(subSurface.subSurfaceType(), "FixedWindow") || 
        istringEqual(subSurface.subSurfaceType(), "OperableWindow")){
          windowArea += subSurface.multiplier() * subSurface.netArea();
      }
    }
    
    double wwr = windowArea / grossArea;
    
    return wwr;
  }

  double Surface_Impl::skylightToRoofRatio() const
  {
    double result = 0.0;

    if (!istringEqual(this->surfaceType(), "RoofCeiling")){
      return result;
    }

    double grossArea = this->grossArea();
    
    if (grossArea == 0){
      return result;
    }

    double skylightArea = 0.0;
    BOOST_FOREACH(const SubSurface& subSurface, this->subSurfaces()){
      if (istringEqual(subSurface.subSurfaceType(), "Skylight")){
          skylightArea += subSurface.multiplier() * subSurface.netArea();
      }
    }
    
    result = skylightArea / grossArea;
    
    return result;
  }

  double Surface_Impl::skylightToProjectedFloorRatio() const
  {
    double result = 0.0;

    if (!istringEqual(this->surfaceType(), "RoofCeiling")){
      return result;
    }

    Point3dVector vertices = this->vertices();
    Plane horizontal(Point3d(0,0,0), Vector3d(0,0,1));
    std::vector<Point3d> projectedVertics = horizontal.project(vertices);

    boost::optional<double> grossArea = getArea(projectedVertics);
    
    if (!grossArea || grossArea.get() == 0){
      return result;
    }

    double skylightArea = 0.0;
    BOOST_FOREACH(const SubSurface& subSurface, this->subSurfaces()){
      if (istringEqual(subSurface.subSurfaceType(), "Skylight")){
          skylightArea += subSurface.multiplier() * subSurface.netArea();
      }
    }
    
    result = skylightArea / grossArea.get();
    
    return result;
  }

  boost::optional<SubSurface> Surface_Impl::setWindowToWallRatio(double wwr)
  {
    return setWindowToWallRatio(wwr, 0.762, true);
  }

  boost::optional<SubSurface> Surface_Impl::setWindowToWallRatio(double wwr, double desiredHeightOffset, bool heightOffsetFromFloor)
  {
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

    if (heightOffsetFromFloor){
      viewGlassToWallRatio = wwr;
      desiredViewGlassSillHeight = desiredHeightOffset;
    }else{
      daylightingGlassToWallRatio = wwr;
      desiredDaylightingGlassHeaderHeight = desiredHeightOffset;
    }

    tmp = applyViewAndDaylightingGlassRatios(viewGlassToWallRatio, daylightingGlassToWallRatio, 
                                             desiredViewGlassSillHeight, desiredDaylightingGlassHeaderHeight,
                                             exteriorShadingProjectionFactor, interiorShelfProjectionFactor, 
                                             viewGlassConstruction, daylightingGlassConstruction);

    if (!tmp.empty()){
      OS_ASSERT(tmp.size() == 1);
      result = tmp[0];
    }

    return result;
  }

  std::vector<SubSurface> Surface_Impl::applyViewAndDaylightingGlassRatios(double viewGlassToWallRatio, double daylightingGlassToWallRatio, 
                                                                           double desiredViewGlassSillHeight, double desiredDaylightingGlassHeaderHeight,
                                                                           double exteriorShadingProjectionFactor, double interiorShelfProjectionFactor, 
                                                                           const boost::optional<ConstructionBase>& viewGlassConstruction, 
                                                                           const boost::optional<ConstructionBase>& daylightingGlassConstruction)
  {
    std::vector<SubSurface> result;

    // has to be a wall
    if (!istringEqual(this->surfaceType(), "Wall")){
      return result;
    }

    // surface cannot have any non-window sub surfaces
    BOOST_FOREACH(const SubSurface& subSurface, this->subSurfaces()){
      if (istringEqual(subSurface.subSurfaceType(), "FixedWindow") || 
          istringEqual(subSurface.subSurfaceType(), "OperableWindow"))
      {
        continue;
      }
      return result;
    }

    // check inputs for reasonableness
    double totalWWR = viewGlassToWallRatio + daylightingGlassToWallRatio;
    if (totalWWR == 0){
      // requesting no glass? remove existing windows?
      return result;
    }else if (totalWWR < 0.0 || totalWWR >= 1.0){
      return result;
    }

    Point3dVector vertices = this->vertices();
    Transformation transformation = Transformation::alignFace(vertices);
    Point3dVector faceVertices = transformation.inverse() * vertices;

    if (faceVertices.empty()){
      return result;
    }

    bool doViewGlass = (viewGlassToWallRatio > 0);
    bool doDaylightGlass = (daylightingGlassToWallRatio > 0);
    bool doExteriorShading = (doViewGlass && (exteriorShadingProjectionFactor > 0));
    bool doInteriorShelf = (doDaylightGlass && (interiorShelfProjectionFactor > 0));
    bool doViewAndDaylightGlass = (doViewGlass && doDaylightGlass);

    // ignore these user arguments?
    if (!doViewGlass){
      desiredViewGlassSillHeight = 0.0;
    }
    if (!doDaylightGlass){
      desiredDaylightingGlassHeaderHeight = 0.0;
    }
    
    // new coordinate system has z' in direction of outward normal, y' is up
    double xmin = std::numeric_limits<double>::max();
    double xmax = std::numeric_limits<double>::min();
    double ymin = std::numeric_limits<double>::max();
    double ymax = std::numeric_limits<double>::min();
    BOOST_FOREACH(const Point3d& faceVertex, faceVertices){
      xmin = std::min(xmin, faceVertex.x());
      xmax = std::max(xmax, faceVertex.x());
      ymin = std::min(ymin, faceVertex.y());
      ymax = std::max(ymax, faceVertex.y());
    }
    if ((xmin > xmax) || (ymin > ymax)){
      return result;
    }

    double oneInch = 0.0254;

    // DLM: preserve a 1" gap between window and edge to keep SketchUp happy
    double minGlassToEdgeDistance = oneInch;
    double minViewToDaylightDistance = 0;
    if (doViewAndDaylightGlass){
      minViewToDaylightDistance = oneInch;
    }

    // wall parameters
    double wallWidth = xmax - xmin; 
    double wallHeight = ymax - ymin;
    double wallArea = wallWidth*wallHeight;

    if (wallWidth < 2*minGlassToEdgeDistance){
      return result;
    }

    if (wallHeight < 2*minGlassToEdgeDistance + minViewToDaylightDistance){
      return result;
    }

    // check against actual surface area to ensure this is a rectangle?
    if (std::abs(wallArea - this->grossArea()) > oneInch*oneInch){
      return result;
    }

    double maxWindowArea = wallArea - 2*wallHeight*minGlassToEdgeDistance - (wallWidth-2*minGlassToEdgeDistance)*(2*minGlassToEdgeDistance + minViewToDaylightDistance);
    double requestedViewArea = viewGlassToWallRatio*wallArea;
    double requestedDaylightingArea = daylightingGlassToWallRatio*wallArea;
    double requestedTotalWindowArea = totalWWR*wallArea;

    if (requestedTotalWindowArea > maxWindowArea){
      return result;
    }

    // view glass parameters 
    double viewMinX = 0;
    double viewMinY = 0;
    double viewWidth = 0;
    double viewHeight = 0;  

    // daylighting glass parameters
    double daylightingWidth = 0;
    double daylightingHeight = 0;
    double daylightingMinX = 0;
    double daylightingMinY = 0;

    // initial free parameters
    double viewWidthInset = minGlassToEdgeDistance;
    double viewSillHeight = std::max(desiredViewGlassSillHeight, minGlassToEdgeDistance);
    double daylightingWidthInset = minGlassToEdgeDistance;
    double daylightingHeaderHeight = std::max(desiredDaylightingGlassHeaderHeight, minGlassToEdgeDistance);

    bool converged = false;
    for (unsigned i = 0; i < 100; ++i){

      // view glass parameters 
      viewMinX = viewWidthInset;
      viewMinY = viewSillHeight;
      viewWidth = wallWidth - 2*viewWidthInset;
      viewHeight = requestedViewArea/viewWidth;  

      // daylighting glass parameters
      daylightingWidth = wallWidth - 2*daylightingWidthInset;
      daylightingHeight = requestedDaylightingArea/daylightingWidth;
      daylightingMinX = viewWidthInset;
      daylightingMinY = wallHeight - daylightingHeaderHeight - daylightingHeight;

      if (viewMinY + viewHeight + minViewToDaylightDistance > daylightingMinY){
        // windows overlap or exceed maximum size

        if (doViewAndDaylightGlass){

          // try shrinking vertical offsets 
          viewSillHeight = std::max(viewSillHeight - oneInch, minGlassToEdgeDistance);
          daylightingHeaderHeight = std::max(daylightingHeaderHeight - oneInch, minGlassToEdgeDistance);

        }else if (doViewGlass){

          // solve directly
          viewSillHeight = wallHeight - minGlassToEdgeDistance - viewHeight;
          
          if (viewSillHeight < minGlassToEdgeDistance){
            // cannot make window this large
            return result;
          }

        }else if (doDaylightGlass){

          // solve directly
          daylightingHeaderHeight = wallHeight - minGlassToEdgeDistance - daylightingHeight;

          if (daylightingHeaderHeight < minGlassToEdgeDistance){
            // cannot make window this large
            return result;
          }

        }

      }else{

        converged = true;
        break;

      }
    }

    if (!converged){
      return result;
    }

    QPolygonF surfacePolygon;
    BOOST_FOREACH(const Point3d& point, faceVertices){
      if (abs(point.z()) > 0.001){
        LOG(Warn, "Surface point z not on plane, z =" << point.z());
      }
      surfacePolygon << QPointF(point.x(),point.y());
    }
    // close the polygon
    surfacePolygon << QPointF(faceVertices[0].x(), faceVertices[0].y());

    Point3dVector viewVertices;
    if (doViewGlass){
      viewVertices.push_back(Point3d(viewMinX, viewMinY + viewHeight, 0));
      viewVertices.push_back(Point3d(viewMinX, viewMinY, 0));
      viewVertices.push_back(Point3d(viewMinX + viewWidth, viewMinY, 0));
      viewVertices.push_back(Point3d(viewMinX + viewWidth, viewMinY + viewHeight, 0)); 

      QPolygonF windowPolygon;
      BOOST_FOREACH(const Point3d& point, viewVertices){
        if (abs(point.z()) > 0.001){
          LOG(Warn, "Surface point z not on plane, z =" << point.z());
        }
        windowPolygon << QPointF(point.x(),point.y());
      }
      // close the polygon
      windowPolygon << QPointF(viewVertices[0].x(), viewVertices[0].y());

      // sub surface must be fully contained by base surface
      BOOST_FOREACH(const QPointF& point, windowPolygon){
        if (!surfacePolygon.containsPoint(point, Qt::OddEvenFill)){
          LOG(Debug, "Surface does not fully contain SubSurface");
          return result;
        }
      }
    }

    Point3dVector daylightingVertices;
    if (doDaylightGlass){
      daylightingVertices.push_back(Point3d(daylightingMinX, daylightingMinY + daylightingHeight, 0));
      daylightingVertices.push_back(Point3d(daylightingMinX, daylightingMinY, 0));
      daylightingVertices.push_back(Point3d(daylightingMinX + daylightingWidth, daylightingMinY, 0));
      daylightingVertices.push_back(Point3d(daylightingMinX + daylightingWidth, daylightingMinY + daylightingHeight, 0)); 
    
      QPolygonF windowPolygon;
      BOOST_FOREACH(const Point3d& point, daylightingVertices){
        if (abs(point.z()) > 0.001){
          LOG(Warn, "Surface point z not on plane, z =" << point.z());
        }
        windowPolygon << QPointF(point.x(),point.y());
      }
      // close the polygon
      windowPolygon << QPointF(daylightingVertices[0].x(), daylightingVertices[0].y());

      // sub surface must be fully contained by base surface
      BOOST_FOREACH(const QPointF& point, windowPolygon){
        if (!surfacePolygon.containsPoint(point, Qt::OddEvenFill)){
          LOG(Debug, "Surface does not fully contain SubSurface");
          return result;
        }
      }    
    }

    Point3dVector exteriorShadingVertices;
    if (doExteriorShading) {
      exteriorShadingVertices.push_back(Point3d(viewMinX, viewMinY + viewHeight, 0));
      exteriorShadingVertices.push_back(Point3d(viewMinX, viewMinY + viewHeight, exteriorShadingProjectionFactor*viewHeight));
      exteriorShadingVertices.push_back(Point3d(viewMinX + viewWidth, viewMinY + viewHeight, exteriorShadingProjectionFactor*viewHeight));
      exteriorShadingVertices.push_back(Point3d(viewMinX + viewWidth, viewMinY + viewHeight, 0)); 
    }

    Point3dVector interiorShelfVertices;
    if (doInteriorShelf) {
      interiorShelfVertices.push_back(Point3d(daylightingMinX + daylightingWidth, daylightingMinY, 0));
      interiorShelfVertices.push_back(Point3d(daylightingMinX + daylightingWidth, daylightingMinY, -interiorShelfProjectionFactor*daylightingHeight)); 
      interiorShelfVertices.push_back(Point3d(daylightingMinX, daylightingMinY, -interiorShelfProjectionFactor*daylightingHeight));
      interiorShelfVertices.push_back(Point3d(daylightingMinX, daylightingMinY, 0));
    }

    // everything ok, remove all windows
    BOOST_FOREACH(SubSurface subSurface, this->subSurfaces()){
      if (istringEqual(subSurface.subSurfaceType(), "FixedWindow") || 
        istringEqual(subSurface.subSurfaceType(), "OperableWindow")){
        subSurface.remove();
      }
    }

    // add a new window
    Model model = this->model();
    Surface thisSurface = this->getObject<Surface>();
    boost::optional<Space> space = this->space();

    boost::optional<SubSurface> viewWindow;
    if (doViewGlass){
      viewWindow = SubSurface(transformation*viewVertices, model);
      result.push_back(*viewWindow);
      viewWindow->setSurface(thisSurface);

      if (viewGlassConstruction){
        viewWindow->setConstruction(*viewGlassConstruction);
      }
    }

    boost::optional<SubSurface> daylightingWindow;
    if (doDaylightGlass){
      daylightingWindow = SubSurface(transformation*daylightingVertices, model);
      result.push_back(*daylightingWindow);
      daylightingWindow->setSurface(thisSurface);

      if (daylightingGlassConstruction){
        daylightingWindow->setConstruction(*daylightingGlassConstruction);
      }
    }

    boost::optional<DaylightingDeviceShelf> interiorShelf;
    if (doInteriorShelf){
      InteriorPartitionSurfaceGroup interiorGroup(model);
      if (space){
        interiorGroup.setSpace(*space);
      }

      InteriorPartitionSurface interiorSurface(transformation*interiorShelfVertices, model);
      interiorSurface.setInteriorPartitionSurfaceGroup(interiorGroup);

      OS_ASSERT(daylightingWindow);
      interiorShelf = DaylightingDeviceShelf(*daylightingWindow);
      interiorShelf->setInsideShelf(interiorSurface);
    }

    if (doExteriorShading){
      ShadingSurfaceGroup shadingGroup(model);
      if (space){
        shadingGroup.setSpace(*space);
      }

      OS_ASSERT(viewWindow);
      shadingGroup.setShadedSubSurface(*viewWindow);

      ShadingSurface shadingSurface(transformation*exteriorShadingVertices, model);
      shadingSurface.setShadingSurfaceGroup(shadingGroup);

      // EnergyPlus expects outside shelf to be on daylight window, we prefer to shade the view window so do not add this here
       //if (interiorShelf){
      //  interiorShelf->setOutsideShelf(shadingSurface);
      //}
    }

    return result;
  }

  std::vector<ShadingSurfaceGroup> Surface_Impl::shadingSurfaceGroups() const
  {
    return getObject<ModelObject>().getModelObjectSources<ShadingSurfaceGroup>(ShadingSurfaceGroup::iddObjectType());
  }

  struct PolygonAreaGreater{
    bool operator()(const Point3dVector& left, const Point3dVector& right){
      boost::optional<double> leftA = getArea(left);
      if (!leftA){
        leftA = 0;
      }
      boost::optional<double> rightA = getArea(right);
      if (!rightA){
        rightA = 0;
      }
      return (*leftA > *rightA);
    }
  };

  std::vector<Surface> Surface_Impl::splitSurfaceForSubSurfaces()
  {
    std::vector<Surface> result;

    double expand = 0.0254;
    double tol = 0.01; // should be less than expand

    // has to be a wall
    if (!istringEqual(this->surfaceType(), "Wall")){
      return result;
    }

    // can't have adjacent surface
    if (this->adjacentSurface()){
      return result;
    }

    std::vector<SubSurface> subSurfaces = this->subSurfaces();
    if (subSurfaces.empty()){
      // nothing to do
      return result;
    }

    Point3dVector vertices = this->vertices();
    Transformation transformation = Transformation::alignFace(vertices);
    Transformation inverseTransformation = transformation.inverse();
    Point3dVector faceVertices = inverseTransformation * vertices;

    if (faceVertices.size() < 3){
      return result;
    }

    // boost polygon wants vertices in clockwise order, faceVertices must be reversed
    std::reverse(faceVertices.begin(), faceVertices.end());

    // new coordinate system has z' in direction of outward normal, y' is up
    //double xmin = std::numeric_limits<double>::max();
    //double xmax = std::numeric_limits<double>::min();
    double ymin = std::numeric_limits<double>::max();
    double ymax = std::numeric_limits<double>::min();
    BOOST_FOREACH(const Point3d& faceVertex, faceVertices){
      //xmin = std::min(xmin, faceVertex.x());
      //xmax = std::max(xmax, faceVertex.x());
      ymin = std::min(ymin, faceVertex.y());
      ymax = std::max(ymax, faceVertex.y());
    }
    if (ymin > ymax){
      return result;
    }

    // create a mask for each sub surface
    std::vector<Point3dVector> masks;
    std::map<Handle, Point3dVector> handleToFaceVertexMap;
    BOOST_FOREACH(const SubSurface& subSurface, subSurfaces){
      
      Point3dVector subSurfaceFaceVertices = inverseTransformation * subSurface.vertices();
      if (subSurfaceFaceVertices.size() < 3){
        continue;
      }

      // boost polygon wants vertices in clockwise order, subSurfaceFaceVertices must be reversed
      std::reverse(subSurfaceFaceVertices.begin(), subSurfaceFaceVertices.end());

      handleToFaceVertexMap[subSurface.handle()] = subSurfaceFaceVertices;

      double xmin = std::numeric_limits<double>::max();
      double xmax = std::numeric_limits<double>::min();
      //double ymin = std::numeric_limits<double>::max();
      //double ymax = std::numeric_limits<double>::min();
      BOOST_FOREACH(const Point3d& faceVertex, subSurfaceFaceVertices){
        xmin = std::min(xmin, faceVertex.x());
        xmax = std::max(xmax, faceVertex.x());
        //ymin = std::min(ymin, faceVertex.y());
        //ymax = std::max(ymax, faceVertex.y());
      }
      if (xmin > xmax){
        continue;
      }

      Point3dVector mask;
      mask.push_back(Point3d(xmax+expand, ymax+expand, 0));
      mask.push_back(Point3d(xmax+expand, ymin-expand, 0));
      mask.push_back(Point3d(xmin-expand, ymin-expand, 0));
      mask.push_back(Point3d(xmin-expand, ymax+expand, 0));
      masks.push_back(mask);
    }

    // join masks
    std::vector<Point3dVector> joinedMasks = joinAll(masks, tol);

    // intersect masks with surface
    std::vector<Point3dVector> newFaces;
    newFaces.push_back(faceVertices);
    BOOST_FOREACH(const Point3dVector& mask, joinedMasks){

      std::vector<Point3dVector> tmpFaces;
      unsigned numIntersects = 0;
      BOOST_FOREACH(const Point3dVector& face, newFaces){
        // each mask should intersect one and only one newFace
        boost::optional<IntersectionResult> intersection = openstudio::intersect(faceVertices, mask, tol);
        if (intersection){
          numIntersects += 1;
          tmpFaces.push_back(intersection->polygon1());
          BOOST_FOREACH(const Point3dVector& tmpFace, intersection->newPolygons1()){
            tmpFaces.push_back(tmpFace);
          }
        }else{
          tmpFaces.push_back(face);
        }
      }

      if (numIntersects != 1){
        LOG(Warn, "Expected each mask to intersect one and only one face");
      }

      newFaces = tmpFaces;
    }

    OS_ASSERT(!newFaces.empty());

    // sort new faces in descending order by area
    std::sort(newFaces.begin(), newFaces.end(), PolygonAreaGreater());

    // loop over all new faces
    bool changedThis = false;
    unsigned numReparented = 0;
    Model model = this->model();
    BOOST_FOREACH(const Point3dVector& newFace, newFaces){
      
      boost::optional<Surface> surface;
      if (!changedThis){
        changedThis = true;
        surface = getObject<Surface>();
      }else{
        boost::optional<ModelObject> object = this->clone(model);
        OS_ASSERT(object);
        surface = object->optionalCast<Surface>();
        OS_ASSERT(surface);
        // remove cloned sub surfaces
        BOOST_FOREACH(ModelObject child, surface->children()){
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
      typedef std::pair<Handle, Point3dVector> MapType;
      BOOST_FOREACH(const MapType& p, handleToFaceVertexMap){
        // if surface includes a single point it will include them all
        if (pointInPolygon(p.second[0], newFace, tol)){
          boost::optional<SubSurface> subSurface = model.getModelObject<SubSurface>(p.first);
          if (subSurface){
            numReparented += 1;
            subSurface->setSurface(*surface);
          }
          continue;
        }
      }
    }

    if (numReparented != handleToFaceVertexMap.size()){
      LOG(Warn, "Expected to reparent " << handleToFaceVertexMap.size() << " sub surfaces in splitSurfaceForSubSurfaces, but only reparented " << numReparented);
    }
  

    return result;
  }

  std::vector<SubSurface> Surface_Impl::createSubSurfaces(const std::vector<std::vector<Point3d> >& faces, double inset, const boost::optional<ConstructionBase>& construction)
  {
    std::vector<SubSurface> result;
    
    double tol = 0.0254;
    
    if (!this->subSurfaces().empty()){
      return result;
    }

    if (this->adjacentSurface()){
      return result;
    }

    boost::optional<Point3d> centroid = this->centroid();
    if (!centroid){
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
    Surface surface = getObject<Surface>();
    BOOST_FOREACH(const std::vector<Point3d>& face, faces){
      Point3dVector faceVertices = inverseTransformation*face;

      // boost polygon wants vertices in clockwise order, faceVertices must be reversed
      std::reverse(faceVertices.begin(), faceVertices.end());

      boost::optional<IntersectionResult> intersection = openstudio::intersect(faceVertices, insetFaceVertices, tol);
      if (intersection){
        Point3dVector intersectionVertices = intersection->polygon1();

        std::vector<std::vector<Point3d> > allNewFaceVertices;
        if (intersectionVertices.size() <= 4){
          allNewFaceVertices.push_back(intersectionVertices);
        }else{
          std::vector<std::vector<Point3d> > holes;
          allNewFaceVertices = computeTriangulation(intersectionVertices, holes, tol);
        } 

        BOOST_FOREACH(Point3dVector newFaceVertices, allNewFaceVertices){

          std::reverse(newFaceVertices.begin(), newFaceVertices.end());

          Point3dVector newVertices = transformation * newFaceVertices;
          SubSurface subSurface(newVertices, model);
          subSurface.setSurface(surface);
          if (construction){
            subSurface.setConstruction(*construction);
          }
          result.push_back(subSurface);
        }
      }
    }

    return result;
  }


} // detail

Surface::Surface(const std::vector<Point3d>& vertices, const Model& model)
  : PlanarSurface(Surface::iddObjectType(), vertices, model)
{
  OS_ASSERT(getImpl<detail::Surface_Impl>());
  getImpl<detail::Surface_Impl>()->assignDefaultSurfaceType(false);
  getImpl<detail::Surface_Impl>()->assignDefaultBoundaryCondition(false);
  getImpl<detail::Surface_Impl>()->assignDefaultSunExposure(false);
  getImpl<detail::Surface_Impl>()->assignDefaultWindExposure(false); 
  getImpl<detail::Surface_Impl>()->emitChangeSignals(); // emit signals here
}

IddObjectType Surface::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Surface);
  return result;
}

std::vector<std::string> Surface::validSurfaceTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceFields::SurfaceType);
}

std::vector<std::string> Surface::validOutsideBoundaryConditionValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceFields::OutsideBoundaryCondition);
}

std::vector<std::string> Surface::validSunExposureValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceFields::SunExposure);
}

std::vector<std::string> Surface::validWindExposureValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceFields::WindExposure);
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

bool Surface::setSurfaceType(std::string surfaceType) {
  return getImpl<detail::Surface_Impl>()->setSurfaceType(surfaceType);
}

bool Surface::setOutsideBoundaryCondition(std::string outsideBoundaryCondition) {
  return getImpl<detail::Surface_Impl>()->setOutsideBoundaryCondition(outsideBoundaryCondition);
}

bool Surface::setSunExposure(std::string sunExposure) {
  return getImpl<detail::Surface_Impl>()->setSunExposure(sunExposure);
}

void Surface::resetSunExposure() {
  getImpl<detail::Surface_Impl>()->resetSunExposure();
}

bool Surface::setWindExposure(std::string windExposure) {
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

SubSurfaceVector Surface::subSurfaces() const
{
  return getImpl<detail::Surface_Impl>()->subSurfaces();
}

bool Surface::setSpace(const Space& space)
{
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

bool Surface::intersect(Surface& otherSurface) {
  return getImpl<detail::Surface_Impl>()->intersect(otherSurface);
}

boost::optional<SurfaceIntersection> Surface::computeIntersection(Surface& otherSurface) {
  return getImpl<detail::Surface_Impl>()->computeIntersection(otherSurface);
}

boost::optional<Surface> Surface::createAdjacentSurface(const Space& otherSpace) 
{
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

double Surface::windowToWallRatio() const
{
  return getImpl<detail::Surface_Impl>()->windowToWallRatio();
}

double Surface::skylightToRoofRatio() const
{
  return getImpl<detail::Surface_Impl>()->skylightToRoofRatio();
}

double Surface::skylightToProjectedFloorRatio() const
{
  return getImpl<detail::Surface_Impl>()->skylightToRoofRatio();
}

boost::optional<SubSurface> Surface::setWindowToWallRatio(double wwr)
{
  return getImpl<detail::Surface_Impl>()->setWindowToWallRatio(wwr);
}

boost::optional<SubSurface> Surface::setWindowToWallRatio(double wwr, double desiredHeightOffset, bool heightOffsetFromFloor)
{
  return getImpl<detail::Surface_Impl>()->setWindowToWallRatio(wwr, desiredHeightOffset, heightOffsetFromFloor);
}

std::vector<SubSurface> Surface::applyViewAndDaylightingGlassRatios(double viewGlassToWallRatio, double daylightingGlassToWallRatio, 
                                                                    double desiredViewGlassSillHeight, double desiredDaylightingGlassHeaderHeight,
                                                                    double exteriorShadingProjectionFactor, double interiorShelfProjectionFactor, 
                                                                    const boost::optional<ConstructionBase>& viewGlassConstruction, 
                                                                    const boost::optional<ConstructionBase>& daylightingGlassConstruction)
{
  return getImpl<detail::Surface_Impl>()->applyViewAndDaylightingGlassRatios(viewGlassToWallRatio, daylightingGlassToWallRatio, 
                                                                             desiredViewGlassSillHeight, desiredDaylightingGlassHeaderHeight,
                                                                             exteriorShadingProjectionFactor, interiorShelfProjectionFactor,
                                                                             viewGlassConstruction, daylightingGlassConstruction);
}

std::vector<SubSurface> Surface::createSubSurfaces(const std::vector<std::vector<Point3d> >& faces, double inset, const boost::optional<ConstructionBase>& construction)
{
  return getImpl<detail::Surface_Impl>()->createSubSurfaces(faces, inset, construction);
}

std::vector<ShadingSurfaceGroup> Surface::shadingSurfaceGroups() const
{
  return getImpl<detail::Surface_Impl>()->shadingSurfaceGroups();
}

std::vector<Surface> Surface::splitSurfaceForSubSurfaces()
{
  return getImpl<detail::Surface_Impl>()->splitSurfaceForSubSurfaces();
}

/// @cond
Surface::Surface(boost::shared_ptr<detail::Surface_Impl> impl)
  : PlanarSurface(impl)
{}
/// @endcond

SurfaceIntersection::SurfaceIntersection(const Surface& surface1, 
                                         const Surface& surface2,
                                         const std::vector<Surface>& newSurfaces1, 
                                         const std::vector<Surface>& newSurfaces2)
  : m_surface1(surface1), m_surface2(surface2), m_newSurfaces1(newSurfaces1), m_newSurfaces2(newSurfaces2)
{
}

Surface SurfaceIntersection::surface1() const
{
  return m_surface1;
}

Surface SurfaceIntersection::surface2() const
{
  return m_surface2;
}

std::vector<Surface> SurfaceIntersection::newSurfaces1() const
{
  return m_newSurfaces1;
}

std::vector<Surface> SurfaceIntersection::newSurfaces2() const
{
  return m_newSurfaces2;
}

std::ostream& operator<<(std::ostream& os, const SurfaceIntersection& surfaceIntersection)
{
  os << "(" << surfaceIntersection.surface1().name().get() << "), ";

  os << "(" << surfaceIntersection.surface2().name().get() << "), ";

  os << "[";
  std::vector<Surface> newSurfaces1 = surfaceIntersection.newSurfaces1();
  for (unsigned i = 0; i < newSurfaces1.size(); ++i){
    os << newSurfaces1[i].name().get();
    if (i != newSurfaces1.size()-1){
      os << ", ";
    }
  }

  os << "], [";
  std::vector<Surface> newSurfaces2 = surfaceIntersection.newSurfaces2();
  for (unsigned i = 0; i < newSurfaces2.size(); ++i){
    os << newSurfaces2[i].name().get();
    if (i != newSurfaces2.size()-1){
      os << ", ";
    }
  }
  os << "]";

  return os;
}


} // model
} // openstudio

