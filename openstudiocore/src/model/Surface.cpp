/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <model/ConstructionBase.hpp>
#include <model/ConstructionBase_Impl.hpp>
#include <model/LayeredConstruction.hpp>
#include <model/LayeredConstruction_Impl.hpp>
#include <model/ConstructionBaseStandardsInformation.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Surface_FieldEnums.hxx>

#include <utilities/geometry/Transformation.hpp>
#include <utilities/geometry/Geometry.hpp>
#include <utilities/core/Assert.hpp>

#include <utilities/sql/SqlFile.hpp>

#include <QPolygon>

#undef BOOST_UBLAS_TYPE_CHECK
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>

typedef boost::geometry::model::d2::point_xy<double> BoostPoint;
typedef boost::geometry::model::polygon<BoostPoint> BoostPolygon;
typedef boost::geometry::model::ring<BoostPoint> BoostRing;
typedef boost::geometry::model::multi_polygon<BoostPolygon> BoostMultiPolygon;

using boost::to_upper_copy;

namespace openstudio {
namespace model {

namespace detail {

  Surface_Impl::Surface_Impl(const IdfObject& idfObject,
                             Model_Impl* model, 
                             bool keepHandle)
    : PlanarSurface_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == Surface::iddObjectType());
  }

  Surface_Impl::Surface_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                             Model_Impl* model,
                             bool keepHandle)
    : PlanarSurface_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == Surface::iddObjectType());
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
      this->assignDefaultSurfaceType(false);
      this->assignDefaultBoundaryCondition(false);
      this->assignDefaultSunExposure(false);
      this->assignDefaultWindExposure(false);
      this->emitChangeSignals(); // emit signals here
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
    BOOST_ASSERT(value);
    return value.get();
  }

  std::vector<std::string> Surface_Impl::surfaceTypeValues() const
  {
    return Surface::validSurfaceTypeValues();
  }

  std::string Surface_Impl::outsideBoundaryCondition() const {
    boost::optional<std::string> value = getString(OS_SurfaceFields::OutsideBoundaryCondition,true);
    BOOST_ASSERT(value);
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
    BOOST_ASSERT(value);
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
    BOOST_ASSERT(value);
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
    bool result = false;
    result = setString(OS_SurfaceFields::SurfaceType, surfaceType, true);
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
        BOOST_ASSERT(test);
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
    bool result = false;
    result = setString(OS_SurfaceFields::SunExposure, sunExposure, true);
    if (driverMethod){
      this->emitChangeSignals(); // emit signals here
    }
    return result;
  }

  void Surface_Impl::resetSunExposure() {
    bool result = setString(OS_SurfaceFields::SunExposure, "");
    BOOST_ASSERT(result);
  }

  bool Surface_Impl::setWindExposure(std::string windExposure) {
    return setWindExposure(windExposure, true);
  }

  bool Surface_Impl::setWindExposure(std::string windExposure, bool driverMethod) {
    bool result = false;
    result = setString(OS_SurfaceFields::WindExposure, windExposure, true);
    if (driverMethod){
      this->emitChangeSignals(); // emit signals here
    }
    return result;
  }

  void Surface_Impl::resetWindExposure() {
    bool result = setString(OS_SurfaceFields::WindExposure, "");
    BOOST_ASSERT(result);
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
    bool result = false;
    result = setDouble(OS_SurfaceFields::ViewFactortoGround, viewFactortoGround);
    return result;
  }

  void Surface_Impl::resetViewFactortoGround() {
    bool result = setString(OS_SurfaceFields::ViewFactortoGround, "");
    BOOST_ASSERT(result);
  }

  void Surface_Impl::autocalculateViewFactortoGround() {
    bool result = setString(OS_SurfaceFields::ViewFactortoGround, "Autocalculate");
    BOOST_ASSERT(result);
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
    bool result = false;
    result = setDouble(OS_SurfaceFields::NumberofVertices, numberofVertices);
    return result;
  }

  void Surface_Impl::resetNumberofVertices() {
    bool result = setString(OS_SurfaceFields::NumberofVertices, "");
    BOOST_ASSERT(result);
  }

  void Surface_Impl::autocalculateNumberofVertices() {
    bool result = setString(OS_SurfaceFields::NumberofVertices, "Autocalculate");
    BOOST_ASSERT(result);
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
        BOOST_ASSERT(test);
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
          BOOST_ASSERT(test);
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
      BOOST_ASSERT(test);
      test = this->setString(OS_SurfaceFields::OutsideBoundaryCondition, "Surface");
      BOOST_ASSERT(test);
      this->assignDefaultSunExposure();
      this->assignDefaultWindExposure();

      if (!isSameSurface){
        test = surface.setPointer(OS_SurfaceFields::OutsideBoundaryConditionObject, this->handle());
        BOOST_ASSERT(test);
        test = surface.setString(OS_SurfaceFields::OutsideBoundaryCondition, "Surface");
        BOOST_ASSERT(test);
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
      BOOST_ASSERT(test);
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
      BOOST_ASSERT(test);
      otherSurface.assignDefaultBoundaryCondition();
      otherSurface.assignDefaultSunExposure();
      otherSurface.assignDefaultWindExposure();

      BOOST_FOREACH(SubSurface subSurface, otherSurface.subSurfaces()){
        subSurface.resetAdjacentSubSurface();
      }
    }
  }


  std::vector<Point3d> verticesFromBoostPolygon(const BoostPolygon& polygon)
  {
    std::vector<Point3d> result;

    BoostRing outer = polygon.outer();
    if (outer.empty()){
      return result;
    }

    // add point for each vertex except final vertex
    for(unsigned i = 0; i < outer.size() - 1; ++i){
      result.push_back(Point3d(outer[i].x(), outer[i].y(), 0.0));
    }
    Point3d lastPoint = result.back();

    BOOST_FOREACH(const BoostRing& inner, polygon.inners()){
      if (!inner.empty()){
        // inner loop already in reverse order
        BOOST_FOREACH(const BoostPoint& p, inner){
          result.push_back(Point3d(p.x(), p.y(), 0.0));
        }
        result.push_back(lastPoint);
      }
    }

    result = removeColinear(result);

    return result;
  }

  bool Surface_Impl::intersect(Surface& otherSurface)
  {
    boost::optional<Space> space = this->space();
    boost::optional<Space> otherSpace = otherSurface.space();
    if (!space || !otherSpace || space->handle() == otherSpace->handle()){
      LOG(Error, "Cannot find spaces for each surface in intersection or surfaces in same space.");
      return false;
    }

    if (!this->subSurfaces().empty() || !otherSurface.subSurfaces().empty()){
      LOG(Error, "Subsurfaces are not allowed in intersection");
      return false;
    }

    if (this->adjacentSurface() || otherSurface.adjacentSurface()){
      LOG(Error, "Adjacent surfaces are not allowed in intersection");
      return false;
    }

    // goes from local system to building coordinates
    Transformation spaceTransformation = space->transformation();
    Transformation otherSpaceTransformation = otherSpace->transformation();

    // do the intersection in building coordinates

    Plane plane = spaceTransformation * this->plane();
    Plane otherPlane = otherSpaceTransformation * otherSurface.plane();

    if (!plane.reverseEqual(otherPlane)){
      return false;
    }

    // get vertices in building coordinates
    std::vector<Point3d> buildingVertices = spaceTransformation * this->vertices();
    std::vector<Point3d> otherBuildingVertices = otherSpaceTransformation * otherSurface.vertices();

    if ((buildingVertices.size() < 3) || (otherBuildingVertices.size() < 3)){
      return false;
    }

    // goes from face coordinates of building vertices to building coordinates
    Transformation faceTransformation; 
    Transformation faceTransformationInverse;
    try {
      faceTransformation = Transformation::alignFace(buildingVertices);
      faceTransformationInverse = faceTransformation.inverse();
    }catch(const std::exception&){
      return false;
    }

    // put building vertices into face coordinates
    std::vector<Point3d> faceVertices = faceTransformationInverse * buildingVertices;
    std::vector<Point3d> otherFaceVertices = faceTransformationInverse * otherBuildingVertices;

    // boost polygon wants vertices in clockwise order, faceVertices must be reversed, otherFaceVertices already CCW
    std::reverse(faceVertices.begin(), faceVertices.end());
    //std::reverse(otherFaceVertices.begin(), otherFaceVertices.end());

    // convert vertices to boost rings
    BoostRing facePolygon;
    BOOST_FOREACH(const Point3d& faceVertex, faceVertices){
      boost::geometry::append(facePolygon, boost::make_tuple(faceVertex.x(), faceVertex.y()));

      // should all have zero z coordinate now
      double z = faceVertex.z();
      if (abs(z) > 0.001){
        LOG(Warn, "Not all points on common plane in intersection");
      }
    }
    boost::geometry::append(facePolygon, boost::make_tuple(faceVertices[0].x(), faceVertices[0].y()));

    // convert vertices to boost rings
    BoostRing otherFacePolygon;
    BOOST_FOREACH(const Point3d& otherFaceVertex, otherFaceVertices){
      boost::geometry::append(otherFacePolygon, boost::make_tuple(otherFaceVertex.x(), otherFaceVertex.y()));

      // should all have zero z coordinate now
      double z = otherFaceVertex.z();
      if (abs(z) > 0.001){
        LOG(Warn, "Not all points on common plane in intersection");
      }
    }
    boost::geometry::append(otherFacePolygon, boost::make_tuple(otherFaceVertices[0].x(), otherFaceVertices[0].y()));
    
    // do we need to check if either polygon overlaps itself?

    // intersect the points in face coordinates, 
    std::vector<BoostPolygon> intersectionResult;
    boost::geometry::intersection(facePolygon, otherFacePolygon, intersectionResult);

    // check if intersection is empty
    if (intersectionResult.empty()){
      return false;
    }

    // non-zero intersection

    // could match here but will save that for other discrete operation

    // this surface minus the intersection
    std::vector<BoostPolygon> faceDifferenceResult;
    boost::geometry::difference(facePolygon, otherFacePolygon, faceDifferenceResult);

    // other surface minus the intersection
    std::vector<BoostPolygon> otherFaceDifferenceResult;
    boost::geometry::difference(otherFacePolygon, facePolygon, otherFaceDifferenceResult);

    // if both differences are empty then these were the same polygon
    if (faceDifferenceResult.empty() && otherFaceDifferenceResult.empty()){
      return false;
    }

    // goes from building coordinates to local system 
    Transformation spaceTransformationInverse = spaceTransformation.inverse();
    Transformation otherSpaceTransformationInverse =  otherSpaceTransformation.inverse();

    // first intersection geometry will be set on original surfaces below

    // if more than one intersection, need to make a new surface in each space
    for (unsigned i = 1; i < intersectionResult.size(); ++i){

      // new vertices in face coordinates
      std::vector<Point3d> intersectionVertices = verticesFromBoostPolygon(intersectionResult[i]);

      // new surface in this space
      std::vector<Point3d> newBuildingVertices = faceTransformation * intersectionVertices;
      std::vector<Point3d> newVertices = spaceTransformationInverse * newBuildingVertices;
      std::reverse(newVertices.begin(), newVertices.end());
      newVertices = reorderULC(newVertices);
      Surface newSurface(newVertices, this->model());
      newSurface.setSpace(*space);

      // new surface in other space
      std::vector<Point3d> newOtherBuildingVertices = faceTransformation * intersectionVertices;
      std::vector<Point3d> newOtherVertices = otherSpaceTransformationInverse * newOtherBuildingVertices;
      newOtherVertices = reorderULC(newOtherVertices);
      Surface newOtherSurface(newOtherVertices, this->model());
      newOtherSurface.setSpace(*otherSpace);

      // could match here but will save that for other discrete operation
    }

    if (faceDifferenceResult.empty()){
      // the entire surface was the intersection, no-op
    }else{
      // only part of the surface was the intersection

      // new vertices in face coordinates
      std::vector<Point3d> intersectionVertices = verticesFromBoostPolygon(intersectionResult[0]);

      // new vertices in this space
      std::vector<Point3d> newBuildingVertices = faceTransformation * intersectionVertices;
      std::vector<Point3d> newVertices = spaceTransformationInverse * newBuildingVertices;
      std::reverse(newVertices.begin(), newVertices.end());
      newVertices = reorderULC(newVertices);
      this->setVertices(newVertices);

      // create surface for each difference
      for (unsigned i = 0; i < faceDifferenceResult.size(); ++i){

        // new vertices in face coordinates, do not add last point
        std::vector<Point3d> faceDifferenceVertices = verticesFromBoostPolygon(faceDifferenceResult[i]);

        // new surface in this space
        std::vector<Point3d> newBuildingVertices = faceTransformation * faceDifferenceVertices;
        std::vector<Point3d> newVertices = spaceTransformationInverse * newBuildingVertices;
        std::reverse(newVertices.begin(), newVertices.end());
        newVertices = reorderULC(newVertices);
        Surface newSurface(newVertices, this->model());
        newSurface.setSpace(*space);
      }
    }

    if (otherFaceDifferenceResult.empty()){
      // the entire surface was the intersection, no-op
    }else{
      // only part of the surface was the intersection

      // new vertices in face coordinates, do not add last point
      std::vector<Point3d> intersectionVertices = verticesFromBoostPolygon(intersectionResult[0]);

      // new vertices in other space
      std::vector<Point3d> newOtherBuildingVertices = faceTransformation * intersectionVertices;
      std::vector<Point3d> newOtherVertices = otherSpaceTransformationInverse * newOtherBuildingVertices;
      newOtherVertices = reorderULC(newOtherVertices);
      otherSurface.setVertices(newOtherVertices);

      // create surface for each difference
      for (unsigned i = 0; i < otherFaceDifferenceResult.size(); ++i){

        // new vertices in face coordinates, do not add last point
        std::vector<Point3d> otherFaceDifferenceVertices = verticesFromBoostPolygon(otherFaceDifferenceResult[i]);

        // new surface in other space
        std::vector<Point3d> newOtherBuildingVertices = faceTransformation * otherFaceDifferenceVertices;
        std::vector<Point3d> newOtherVertices = otherSpaceTransformationInverse * newOtherBuildingVertices;
        newOtherVertices = reorderULC(newOtherVertices);
        Surface newOtherSurface(newOtherVertices, this->model());
        newOtherSurface.setSpace(*otherSpace);
      }
    }

    return true;
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
      BOOST_ASSERT(ok);
    }else if(degTilt < 179){
      ok = setSurfaceType("Wall", driverMethod);
      BOOST_ASSERT(ok);
    }else{
      ok = setSurfaceType("Floor", driverMethod);
      BOOST_ASSERT(ok);
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
      BOOST_ASSERT(test);
    }else if (istringEqual("Floor", this->surfaceType())){
      bool test = this->setOutsideBoundaryCondition("Ground", driverMethod);
      BOOST_ASSERT(test);
    }else{
      bool test = this->setOutsideBoundaryCondition("Outdoors", driverMethod);
      BOOST_ASSERT(test);
    }
  }

  void Surface_Impl::assignDefaultSunExposure()
  {
    assignDefaultSunExposure(true);
  }

  void Surface_Impl::assignDefaultSunExposure(bool driverMethod)
  {
    std::string outsideBoundaryCondition = this->outsideBoundaryCondition();
    if (istringEqual("Outdoors", this->outsideBoundaryCondition())){
      bool test = this->setSunExposure("SunExposed", driverMethod);
      BOOST_ASSERT(test);
    }else{
      bool test = this->setSunExposure("NoSun", driverMethod);
      BOOST_ASSERT(test);
    }
  }
     
  void Surface_Impl::assignDefaultWindExposure()
  {
    assignDefaultWindExposure(true);
  }

  void Surface_Impl::assignDefaultWindExposure(bool driverMethod)
  {
    std::string outsideBoundaryCondition = this->outsideBoundaryCondition();
    if (istringEqual("Outdoors", this->outsideBoundaryCondition())){
      bool test = setWindExposure("WindExposed", driverMethod);
      BOOST_ASSERT(test);
    }else{
      bool test = setWindExposure("NoWind", driverMethod);
      BOOST_ASSERT(test);
    }
  }

  std::string Surface_Impl::constructionType() const {
    std::string result;
    OptionalConstructionBase oConstruction = construction();
    if (oConstruction) {
      result = oConstruction->standardsInformation().constructionType();
    }
    return result;
  }

  bool Surface_Impl::setConstructionType(const std::string& type) {
    OptionalConstructionBase oConstruction = construction();
    if (oConstruction) {
      ConstructionBaseStandardsInformation info = oConstruction->standardsInformation();
      info.setConstructionType(type);
      return true;
    }
    return false;
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

  boost::optional<SubSurface> Surface_Impl::setWindowToWallRatio(double wwr)
  {
    return setWindowToWallRatio(wwr, 0.762, true);
  }

  boost::optional<SubSurface> Surface_Impl::setWindowToWallRatio(double wwr, double desiredHeightOffset, bool heightOffsetFromFloor)
  {
    boost::optional<SubSurface> result;

    if (!istringEqual(this->surfaceType(), "Wall")){
      return result;
    }
    
    if (wwr <= 0.0 || wwr >= 1.0){
      return result;
    }

    Point3dVector vertices = this->vertices();
    Transformation transformation = Transformation::alignFace(vertices);
    Point3dVector faceVertices = transformation.inverse() * vertices;

    if (faceVertices.empty()){
      return result;
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

    // w*h = (W-2*x)(H-y-offset) = W*H-W*y-W*offset-2*x*H+2*x*y+2*x*offset = wwr*W*H
    double x, y, h, w, offset;
    double W = xmax - xmin; // W = x + w + x
    double H = ymax - ymin; // H = y + h + offset
    double minx = 0.0254; // 1" 
    double miny = 0.0254; // 1" 
    double minh = 0.3048; // 1'
    double minw = 0.3048; // 1'

    // first try x = minx and offset = desiredHeightOffset, solve for y
    // H*w-offset*w-wwr*W*H = y*w
    x = minx;
    w = W-2*x;
    offset = desiredHeightOffset;
    y = (H*w-offset*w-wwr*W*H)/(w);
    h = H - offset - y;
    
    if (y < miny){
      // window is too big, need to shrink offset
      // w*H-w*y-wwr*W*H = w*offset
      x = minx;
      w = W-2*x;
      y = miny;
      offset = (w*H-w*y-wwr*W*H)/w;
      h = H - offset - y;

      if (offset < miny){
        // window is too big
        return result;
      }

    }else if (h < minh){
      // window is too skinny, need to increase x
      // w*h = wwr*W*H
      offset = desiredHeightOffset;
      h = minh;
      y = H-offset-h;
      w = wwr*W*H/h;
      x = (W-w)/2.0;

      if (w < minw){
        // window is too small
        return result;
      }
    }

    Point3dVector windowVertices;
    if (heightOffsetFromFloor){
      windowVertices.push_back(Point3d(xmin + x, ymin + offset + h, 0));
      windowVertices.push_back(Point3d(xmin + x, ymin + offset, 0));
      windowVertices.push_back(Point3d(xmin + x + w, ymin + offset, 0));
      windowVertices.push_back(Point3d(xmin + x + w, ymin + offset + h, 0));
    }else{
      windowVertices.push_back(Point3d(xmin + x, ymin + y + h, 0));
      windowVertices.push_back(Point3d(xmin + x, ymin + y, 0));
      windowVertices.push_back(Point3d(xmin + x + w, ymin + y, 0));
      windowVertices.push_back(Point3d(xmin + x + w, ymin + y + h, 0));
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

    QPolygonF windowPolygon;
    BOOST_FOREACH(const Point3d& point, windowVertices){
      if (abs(point.z()) > 0.001){
        LOG(Warn, "Surface point z not on plane, z =" << point.z());
      }
      windowPolygon << QPointF(point.x(),point.y());
    }
    // close the polygon
    windowPolygon << QPointF(windowVertices[0].x(), windowVertices[0].y());

    // sub surface must be fully contained by base surface
    BOOST_FOREACH(const QPointF& point, windowPolygon){
      if (!surfacePolygon.containsPoint(point, Qt::OddEvenFill)){
        LOG(Debug, "Surface does not fully contain SubSurface");
        return result;
      }
    }

    // sub surface must not clip other sub surfaces
    BOOST_FOREACH(const SubSurface& subSurface, this->subSurfaces()){
      if (istringEqual(subSurface.subSurfaceType(), "FixedWindow") || 
        istringEqual(subSurface.subSurfaceType(), "OperableWindow")){
        continue;
      }

      QPolygonF testPolygon;
      Point3dVector testVertices = transformation.inverse() * subSurface.vertices();
      BOOST_FOREACH(const Point3d& point, testVertices){
        if (abs(point.z()) > 0.001){
          LOG(Warn, "Surface point z not on plane, z =" << point.z());
        }
        testPolygon << QPointF(point.x(),point.y());
      }
      // close the polygon
      testPolygon << QPointF(testVertices[0].x(), testVertices[0].y());

      QPolygonF intersection = windowPolygon.intersected(testPolygon);
      if (!intersection.empty()){
        LOG(Debug, "SubSurface intersects other SubSurface");
        return result;
      }
    }

    // window is ok, remove all windows
    BOOST_FOREACH(SubSurface subSurface, this->subSurfaces()){
      if (istringEqual(subSurface.subSurfaceType(), "FixedWindow") || 
        istringEqual(subSurface.subSurfaceType(), "OperableWindow")){
        subSurface.remove();
      }
    }

    // add a new window
    Model model = this->model();
    SubSurface window(transformation*windowVertices, model);
    Surface thisSurface = this->getObject<Surface>();
    window.setSurface(thisSurface);

    return window;
  }

} // detail

Surface::Surface(const std::vector<Point3d>& vertices, const Model& model)
  : PlanarSurface(Surface::iddObjectType(), vertices, model)
{
  BOOST_ASSERT(getImpl<detail::Surface_Impl>());
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

std::string Surface::constructionType() const {
  return getImpl<detail::Surface_Impl>()->constructionType();
}

bool Surface::setConstructionType(const std::string& type) {
  return getImpl<detail::Surface_Impl>()->setConstructionType(type);  
}

double Surface::filmResistance() const {
  return getImpl<detail::Surface_Impl>()->filmResistance();
}

double Surface::windowToWallRatio() const
{
  return getImpl<detail::Surface_Impl>()->windowToWallRatio();
}

boost::optional<SubSurface> Surface::setWindowToWallRatio(double wwr)
{
  return getImpl<detail::Surface_Impl>()->setWindowToWallRatio(wwr);
}

boost::optional<SubSurface> Surface::setWindowToWallRatio(double wwr, double desiredHeightOffset, bool heightOffsetFromFloor)
{
  return getImpl<detail::Surface_Impl>()->setWindowToWallRatio(wwr, desiredHeightOffset, heightOffsetFromFloor);
}

/// @cond
Surface::Surface(boost::shared_ptr<detail::Surface_Impl> impl)
  : PlanarSurface(impl)
{}
/// @endcond


} // model
} // openstudio

