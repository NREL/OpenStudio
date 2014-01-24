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

#include <model/SubSurface.hpp>
#include <model/SubSurface_Impl.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/ShadingSurface.hpp>
#include <model/ShadingSurface_Impl.hpp>
#include <model/ShadingSurfaceGroup.hpp>
#include <model/ShadingSurfaceGroup_Impl.hpp>
#include <model/ShadingControl.hpp>
#include <model/ShadingControl_Impl.hpp>
#include <model/ConstructionBase.hpp>
#include <model/ConstructionBase_Impl.hpp>
#include <model/Construction.hpp>
#include <model/Construction_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SubSurface_FieldEnums.hxx>

#include <utilities/sql/SqlFile.hpp>

#include <utilities/geometry/Geometry.hpp>
#include <utilities/geometry/Transformation.hpp>
#include <utilities/core/Assert.hpp>

using boost::to_upper_copy;

namespace openstudio {
namespace model {

namespace detail {

  SubSurface_Impl::SubSurface_Impl(const IdfObject& idfObject,
                                   Model_Impl* model, 
                                   bool keepHandle)
    : PlanarSurface_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SubSurface::iddObjectType());
  }

  SubSurface_Impl::SubSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle)
    : PlanarSurface_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SubSurface::iddObjectType());
  }

  SubSurface_Impl::SubSurface_Impl(const SubSurface_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle)
    : PlanarSurface_Impl(other,model,keepHandle)
  {}

  SubSurface_Impl::~SubSurface_Impl()
  {
  }

  boost::optional<ParentObject> SubSurface_Impl::parent() const
  {
    return boost::optional<ParentObject>(this->surface());
  }

  bool SubSurface_Impl::setParent(ParentObject& newParent)
  {
    bool result = false;
    if (newParent.optionalCast<Surface>()){
      result = this->setSurface(newParent.cast<Surface>());
    }
    return result;
  }

  std::vector<ModelObject> SubSurface_Impl::children() const
  {
    std::vector<ModelObject> result;
    return result;
  }

  std::vector<IdfObject> SubSurface_Impl::remove()
  {
    boost::optional<SubSurface> adjacentSubSurface = this->adjacentSubSurface();
    if (adjacentSubSurface){
      this->resetAdjacentSubSurface();
    }

    return ParentObject_Impl::remove();
  }

  const std::vector<std::string>& SubSurface_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
      result.push_back("Surface Inside Face Temperature");
      result.push_back("Surface Outside Face Temperature");
    }
    return result;
  }

  IddObjectType SubSurface_Impl::iddObjectType() const {
    return SubSurface::iddObjectType();
  }

  boost::optional<ConstructionBase> SubSurface_Impl::construction() const
  {
    // DLM: there is duplicate code in ForwardTranslator::resolveMatchedSubSurfaceConstructionConflicts
    // if you update this code you must update that code

    // DLM: I am not sure we should be doing this here at all, maybe this method should just
    // return the same thing constructionWithSearchDistance does?

    boost::optional<std::pair<ConstructionBase, int> > constructionWithSearchDistance = this->constructionWithSearchDistance();

    model::OptionalSubSurface adjacentSubSurface = this->adjacentSubSurface();
    if (!adjacentSubSurface){
      if (constructionWithSearchDistance){
        return constructionWithSearchDistance->first;
      }
      return boost::none;
    }

    boost::optional<std::pair<model::ConstructionBase, int> > adjacentConstructionWithSearchDistance = adjacentSubSurface->constructionWithSearchDistance();

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

  boost::optional<std::pair<ConstructionBase, int> > SubSurface_Impl::constructionWithSearchDistance() const
  {
    boost::optional<std::pair<ConstructionBase, int> > result;

    boost::optional<ConstructionBase> construction = getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_SubSurfaceFields::ConstructionName);
    if (construction){
      return std::make_pair<ConstructionBase, int>(*construction, 0);
    }

    boost::optional<Surface> surface = this->surface();
    if (surface){
      boost::optional<Space> space = surface->space();
      if (space){
        result = space->getDefaultConstructionWithSearchDistance(this->getObject<SubSurface>());
      }
    }

    return result;
  }

  bool SubSurface_Impl::isConstructionDefaulted() const
  {
    return isEmpty(OS_SubSurfaceFields::ConstructionName);
  }

  bool SubSurface_Impl::setVertices(const std::vector<Point3d>& vertices)
  {
    bool result = PlanarSurface_Impl::setVertices(vertices);

    if (isEmpty(OS_SubSurfaceFields::SubSurfaceType)){
      this->assignDefaultSubSurfaceType();
    }

    return result;
  }

  bool SubSurface_Impl::setConstruction(const ConstructionBase& construction)
  {
    bool result = setPointer(OS_SubSurfaceFields::ConstructionName, construction.handle());
    return result;
  }

  void SubSurface_Impl::resetConstruction()
  {
    bool result = setString(OS_SubSurfaceFields::ConstructionName, "");
    OS_ASSERT(result);
  }

  boost::optional<PlanarSurfaceGroup> SubSurface_Impl::planarSurfaceGroup() const
  {
    return boost::optional<PlanarSurfaceGroup>(this->space());
  }

  boost::optional<Space> SubSurface_Impl::space() const
  { 
    boost::optional<Space> result;
    boost::optional<Surface> surface = this->surface();
    if (surface){
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
      // opaque exterior
      if (sqlFile && constructionName && oConstruction->isOpaque()) {
        std::string query = "SELECT RowId FROM tabulardatawithstrings WHERE ReportName='EnvelopeSummary' AND ReportForString='Entire Facility' AND TableName='Opaque Exterior' AND ColumnName='Construction' AND Value='" + to_upper_copy(*constructionName) + "'";
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
          outputResult = 1.0/(1.0/(*outputResult) - oSurface->filmResistance());
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

  bool SubSurface_Impl::setUFactor(double value) {
    OptionalConstructionBase oConstruction = this->construction();
    OptionalSurface oSurface = this->surface();
    if (oConstruction && oSurface) {
      return oConstruction->setUFactor(value,oSurface->filmResistance());
    }
    return false;
  }

  bool SubSurface_Impl::setThermalConductance(double value) {
    OptionalConstructionBase oConstruction = this->construction();
    OptionalSurface oSurface = this->surface();
    if (oConstruction && oSurface) {
      return oConstruction->setConductance(value,oSurface->filmResistance());
    }
    return false;
  }

  bool SubSurface_Impl::subtractFromGrossArea() const
  {
    return true;
  }

  std::string SubSurface_Impl::subSurfaceType() const {
    boost::optional<std::string> value = getString(OS_SubSurfaceFields::SubSurfaceType,true,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::vector<std::string> SubSurface_Impl::subSurfaceTypeValues() const {
    return SubSurface::validSubSurfaceTypeValues();
  }

  boost::optional<double> SubSurface_Impl::viewFactortoGround() const {
    return getDouble(OS_SubSurfaceFields::ViewFactortoGround,true);
  }

  bool SubSurface_Impl::isViewFactortoGroundDefaulted() const {
    return isEmpty(OS_SubSurfaceFields::ViewFactortoGround);
  }

  bool SubSurface_Impl::isViewFactortoGroundAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_SubSurfaceFields::ViewFactortoGround, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "canAutocalculate");
    }
    return result;
  }

  boost::optional<ShadingControl> SubSurface_Impl::shadingControl() const
  {
    return getObject<SubSurface>().getModelObjectTarget<ShadingControl>(OS_SubSurfaceFields::ShadingControlName);
  }

  double SubSurface_Impl::multiplier() const {
    boost::optional<double> value = getDouble(OS_SubSurfaceFields::Multiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SubSurface_Impl::isMultiplierDefaulted() const {
    return isEmpty(OS_SubSurfaceFields::Multiplier);
  }

  boost::optional<double> SubSurface_Impl::numberofVertices() const {
    return getDouble(OS_SubSurfaceFields::NumberofVertices,true);
  }

  bool SubSurface_Impl::isNumberofVerticesDefaulted() const {
    return isEmpty(OS_SubSurfaceFields::NumberofVertices);
  }

  bool SubSurface_Impl::isNumberofVerticesAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_SubSurfaceFields::NumberofVertices, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "canAutocalculate");
    }
    return result;
  }

  bool SubSurface_Impl::setSubSurfaceType(std::string subSurfaceType) {
    bool result = setString(OS_SubSurfaceFields::SubSurfaceType, subSurfaceType);
    if (result){

      if (!(istringEqual("FixedWindow", subSurfaceType) ||
            istringEqual("OperableWindow", subSurfaceType) ||
            istringEqual("GlassDor", subSurfaceType))){
        this->resetShadingControl();
      }

      boost::optional<SubSurface> adjacentSubSurface = this->adjacentSubSurface();
      if (adjacentSubSurface){
        adjacentSubSurface->setString(OS_SubSurfaceFields::SubSurfaceType, subSurfaceType);
        adjacentSubSurface->resetShadingControl();
      }
    }
    return result;
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

  bool SubSurface_Impl::setShadingControl(const ShadingControl& shadingControl)
  {
    bool result = false;
    std::string subSurfaceType = this->subSurfaceType();
    if (istringEqual("FixedWindow", subSurfaceType) ||
        istringEqual("OperableWindow", subSurfaceType) ||
        istringEqual("GlassDor", subSurfaceType)){
      result = setPointer(OS_SubSurfaceFields::ShadingControlName, shadingControl.handle());
    }
    return result;
  }

  void SubSurface_Impl::resetShadingControl()
  {
    bool result = setString(OS_SubSurfaceFields::ShadingControlName, "");
    OS_ASSERT(result);  
  }

  bool SubSurface_Impl::setMultiplier(double multiplier) {
    bool result = setDouble(OS_SubSurfaceFields::Multiplier, multiplier);

    if (result){
      boost::optional<SubSurface> adjacentSubSurface = this->adjacentSubSurface();
      if (adjacentSubSurface){
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
    if (adjacentSubSurface){
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

  boost::optional<Surface> SubSurface_Impl::surface() const
  {
    return getObject<SubSurface>().getModelObjectTarget<Surface>(OS_SubSurfaceFields::SurfaceName);
  }

  bool SubSurface_Impl::setSurface(const Surface& surface)
  {
    bool emptySurface = isEmpty(OS_SubSurfaceFields::SurfaceName);
    bool result = setPointer(OS_SubSurfaceFields::SurfaceName, surface.handle());
    if (result && emptySurface){
      assignDefaultSubSurfaceType();
    }
    return result;
  }
  
  boost::optional<SubSurface> SubSurface_Impl::adjacentSubSurface() const
  {
    return getObject<SubSurface>().getModelObjectTarget<SubSurface>(OS_SubSurfaceFields::OutsideBoundaryConditionObject);
  }
  
  bool SubSurface_Impl::setAdjacentSubSurface(SubSurface& subSurface)
  {
    // matching sub surface with self is ok for stories with multipliers
    bool isSameSubSurface = false;
    if (subSurface.handle() == this->handle()){
      isSameSubSurface = true;
    }

    if (this->multiplier() != subSurface.multiplier()){
      return false;
    }

    bool result = false;

    boost::optional<Surface> thisSurface = this->surface();
    boost::optional<Surface> otherSurface = subSurface.surface();
    if (thisSurface && otherSurface){

      // if base surfaces are matched
      boost::optional<Surface> adjacentSurface = thisSurface->adjacentSurface();
      if (adjacentSurface && (adjacentSurface->handle() == otherSurface->handle())){

        // check for unequal subsurfacetypes
        std::string thisSubSurfaceType = this->subSurfaceType();
        std::string adjacentSubSurfaceType = subSurface.subSurfaceType();
        if (thisSubSurfaceType != adjacentSubSurfaceType){
          // sub surfaces don't match, we could return false here if we wanted to 
          // however, David's preference is that we attempt to resolve this
          std::string thisDefaultSubSurfaceType = this->defaultSubSurfaceType();

          // if subSurfaceType is set to something other than defaultSubSurfaceType assume the user wanted it that way
          // resolve conflicts giving preference to this subSurfaceType
          if (thisSubSurfaceType != thisDefaultSubSurfaceType){
            // set other sub surface to this subSurfaceType
            subSurface.setSubSurfaceType(thisSubSurfaceType);
          }else{
            // must be the other surface that doesn't match its defaultSubSurfaceType
            // set this sub surface to other subSurfaceType
            this->setSubSurfaceType(adjacentSubSurfaceType);
          }
        }

        // reset any sub surfaces pointing to this
        this->resetAdjacentSubSurface();

        // if other subsurface is already matched to another sub surface, could be this one
        if (!isSameSubSurface){
          subSurface.resetAdjacentSubSurface();
        }

        result = setPointer(OS_SubSurfaceFields::OutsideBoundaryConditionObject, subSurface.handle());
        OS_ASSERT(result);
        this->resetShadingControl();

        if (!isSameSubSurface){
          result = subSurface.setPointer(OS_SubSurfaceFields::OutsideBoundaryConditionObject, this->handle());
          OS_ASSERT(result);
          subSurface.resetShadingControl();
        }
      }
    }

    return result;
  }
  
  void SubSurface_Impl::resetAdjacentSubSurface()
  {
    bool test = setString(OS_SubSurfaceFields::OutsideBoundaryConditionObject, "");
    OS_ASSERT(test);

    BOOST_FOREACH(WorkspaceObject wo, this->getSources(IddObjectType::OS_SubSurface)){
      test = wo.setString(OS_SubSurfaceFields::OutsideBoundaryConditionObject, "");
      OS_ASSERT(test);
    }
  }

  std::string SubSurface_Impl::defaultSubSurfaceType() const
  {
    std::string result;

    boost::optional<Surface> surface = this->surface();
    if (!surface){
      double degTilt = radToDeg(this->tilt());
      if (degTilt < 60){
        result = "Skylight";
      }else if(degTilt < 179){
        result = "FixedWindow";
      }else{
        result = "Skylight";
      }
    }else{
      std::string surfaceType = surface->surfaceType();
      if (istringEqual("RoofCeiling", surfaceType) || istringEqual("Floor", surfaceType)){
        result = "Skylight";
      }else{
        double surfaceMinZ = std::numeric_limits<double>::max();
        BOOST_FOREACH(const Point3d& point, surface->vertices()){
          surfaceMinZ = std::min(surfaceMinZ, point.z());
        }

        double thisMinZ = std::numeric_limits<double>::max();
        BOOST_FOREACH(const Point3d& point, this->vertices()){
          thisMinZ = std::min(thisMinZ, point.z());
        }

        if (thisMinZ <= surfaceMinZ){
          result = "Door";
        }else{
          result = "FixedWindow";
        }
      }
    }

    return result;
  }

  void SubSurface_Impl::assignDefaultSubSurfaceType() 
  {
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

  boost::optional<ShadingSurface> SubSurface_Impl::addOverhang(double depth, double offset)
  {
    std::string subSurfaceType = this->subSurfaceType();
    if (!(istringEqual("FixedWindow", subSurfaceType) ||
          istringEqual("OperableWindow", subSurfaceType) ||
          istringEqual("GlassDoor", subSurfaceType))){
      return boost::none;
    }

    Model model = this->model();
    boost::optional<Space> space = this->space();
    std::string shadingSurfaceGroupName = this->name().get() + " Shading Surfaces";
    boost::optional<ShadingSurfaceGroup> shadingSurfaceGroup;
    boost::optional<ShadingSurface> shadingSurface;
    if (space){
      
      Point3dVector vertices = this->vertices();
      Transformation transformation = Transformation::alignFace(vertices);
      Point3dVector faceVertices = transformation.inverse() * vertices;

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

      Point3dVector overhangVertices;
      overhangVertices.push_back(Point3d(xmax + offset, ymax + offset, 0));
      overhangVertices.push_back(Point3d(xmin - offset, ymax + offset, 0));
      overhangVertices.push_back(Point3d(xmin - offset, ymax + offset, depth));
      overhangVertices.push_back(Point3d(xmax + offset, ymax + offset, depth));

      BOOST_FOREACH(ShadingSurfaceGroup group, space->shadingSurfaceGroups()){
        if (group.name() == shadingSurfaceGroupName){
          shadingSurfaceGroup = group;
          break;
        }
      }

      if (!shadingSurfaceGroup){
        shadingSurfaceGroup = ShadingSurfaceGroup(model);
        shadingSurfaceGroup->setSpace(*space);
      }

      shadingSurface = ShadingSurface(transformation*overhangVertices, model);
      shadingSurface->setShadingSurfaceGroup(*shadingSurfaceGroup);
    }

    return shadingSurface;
  }

  boost::optional<ShadingSurface> SubSurface_Impl::addOverhangByProjectionFactor(double projectionFactor, double offsetFraction)
  { 
    std::string subSurfaceType = this->subSurfaceType();
    if (!(istringEqual("FixedWindow", subSurfaceType) ||
          istringEqual("OperableWindow", subSurfaceType) ||
          istringEqual("GlassDoor", subSurfaceType))){
      return boost::none;
    }

    Model model = this->model();
    boost::optional<Space> space = this->space();
    std::string shadingSurfaceGroupName = this->name().get() + " Shading Surfaces";
    boost::optional<ShadingSurfaceGroup> shadingSurfaceGroup;
    boost::optional<ShadingSurface> shadingSurface;
    if (space){
      
      Point3dVector vertices = this->vertices();
      Transformation transformation = Transformation::alignFace(vertices);
      Point3dVector faceVertices = transformation.inverse() * vertices;

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

      double offset = offsetFraction*(ymax-ymin);
      double depth = projectionFactor*(offset + (ymax-ymin));

      Point3dVector overhangVertices;
      overhangVertices.push_back(Point3d(xmax + offset, ymax + offset, 0));
      overhangVertices.push_back(Point3d(xmin - offset, ymax + offset, 0));
      overhangVertices.push_back(Point3d(xmin - offset, ymax + offset, depth));
      overhangVertices.push_back(Point3d(xmax + offset, ymax + offset, depth));

      BOOST_FOREACH(ShadingSurfaceGroup group, space->shadingSurfaceGroups()){
        if (group.name() == shadingSurfaceGroupName){
          shadingSurfaceGroup = group;
          break;
        }
      }

      if (!shadingSurfaceGroup){
        shadingSurfaceGroup = ShadingSurfaceGroup(model);
        shadingSurfaceGroup->setSpace(*space);
      }

      shadingSurface = ShadingSurface(transformation*overhangVertices, model);
      shadingSurface->setShadingSurfaceGroup(*shadingSurfaceGroup);
    }

    return shadingSurface;
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
      }
      else {
        return false;
      }
    }
    else {
      resetAdjacentSubSurface();
    }
    return true;
  }

} // detail

SubSurface::SubSurface(const std::vector<Point3d>& vertices, const Model& model)
  : PlanarSurface(SubSurface::iddObjectType(),vertices,model)
{
  OS_ASSERT(getImpl<detail::SubSurface_Impl>());
  assignDefaultSubSurfaceType();
}

IddObjectType SubSurface::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SubSurface);
  return result;
}

std::vector<std::string> SubSurface::validSubSurfaceTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SubSurfaceFields::SubSurfaceType);
}

std::string SubSurface::subSurfaceType() const {
  return getImpl<detail::SubSurface_Impl>()->subSurfaceType();
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

boost::optional<ShadingControl> SubSurface::shadingControl() const
{
  return getImpl<detail::SubSurface_Impl>()->shadingControl();
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

bool SubSurface::setSubSurfaceType(std::string subSurfaceType) {
  return getImpl<detail::SubSurface_Impl>()->setSubSurfaceType(subSurfaceType);
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

bool SubSurface::setShadingControl(const ShadingControl& shadingControl) {
  return getImpl<detail::SubSurface_Impl>()->setShadingControl(shadingControl);
}

void SubSurface::resetShadingControl() {
  getImpl<detail::SubSurface_Impl>()->resetShadingControl();
}

boost::optional<Surface> SubSurface::surface() const {
  return getImpl<detail::SubSurface_Impl>()->surface();
}

bool SubSurface::setSurface(const Surface& surface) {
  return getImpl<detail::SubSurface_Impl>()->setSurface(surface);
}

boost::optional<SubSurface> SubSurface::adjacentSubSurface() const
{
  return getImpl<detail::SubSurface_Impl>()->adjacentSubSurface();
}

bool SubSurface::setAdjacentSubSurface(SubSurface& subSurface)
{
  return getImpl<detail::SubSurface_Impl>()->setAdjacentSubSurface(subSurface);
}

void SubSurface::resetAdjacentSubSurface()
{
  getImpl<detail::SubSurface_Impl>()->resetAdjacentSubSurface();
}

void SubSurface::assignDefaultSubSurfaceType() {
  getImpl<detail::SubSurface_Impl>()->assignDefaultSubSurfaceType();
}

std::string SubSurface::outsideBoundaryCondition() const {
  return getImpl<detail::SubSurface_Impl>()->outsideBoundaryCondition();
}

boost::optional<ShadingSurface> SubSurface::addOverhang(double depth, double offset)
{
  return getImpl<detail::SubSurface_Impl>()->addOverhang(depth, offset);
}

boost::optional<ShadingSurface> SubSurface::addOverhangByProjectionFactor(double projectionFactor, double offsetFraction)
{
  return getImpl<detail::SubSurface_Impl>()->addOverhangByProjectionFactor(projectionFactor, offsetFraction);
}

/// @cond
SubSurface::SubSurface(boost::shared_ptr<detail::SubSurface_Impl> impl)
  : PlanarSurface(impl)
{}
/// @endcond


} // model
} // openstudio

