/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
#include "WindowPropertyFrameAndDivider.hpp"
#include "WindowPropertyFrameAndDivider_Impl.hpp"
#include "SurfacePropertyConvectionCoefficients.hpp"
#include "SurfacePropertyOtherSideCoefficients.hpp"
#include "SurfacePropertyOtherSideCoefficients_Impl.hpp"
#include "SurfacePropertyOtherSideConditionsModel.hpp"
#include "SurfacePropertyOtherSideConditionsModel_Impl.hpp"
#include "SurfacePropertyConvectionCoefficients.hpp"
#include "SurfacePropertyConvectionCoefficients_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SubSurface_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/sql/SqlFile.hpp"

#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/core/Assert.hpp"

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
    boost::optional<DaylightingDeviceShelf> daylightingDeviceShelf = this->daylightingDeviceShelf();
    if (daylightingDeviceShelf){
      result.push_back(*daylightingDeviceShelf);
    }
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
      return std::make_pair(*construction, 0);
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
      if (result){
        this->assignDefaultSubSurfaceType();
      }else{
        LOG(Error, "Cannot compute default SubSurface properties.");
      }
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

  bool SubSurface_Impl::allowShadingControl() const
  {
    bool result = false;

    std::string subSurfaceType = this->subSurfaceType();
    if (istringEqual("FixedWindow", subSurfaceType) ||
        istringEqual("OperableWindow", subSurfaceType) ||
        istringEqual("GlassDoor", subSurfaceType))
    {
      result = true;
    }

    return result;
  }

  boost::optional<ShadingControl> SubSurface_Impl::shadingControl() const
  {
    return getObject<SubSurface>().getModelObjectTarget<ShadingControl>(OS_SubSurfaceFields::ShadingControlName);
  }

  bool SubSurface_Impl::allowWindowPropertyFrameAndDivider() const
  {
    bool result = false;

    std::string subSurfaceType = this->subSurfaceType();
    if (istringEqual("FixedWindow", subSurfaceType) ||
        istringEqual("OperableWindow", subSurfaceType) ||
        istringEqual("GlassDoor", subSurfaceType))
    {
      result = true;
    }

    return result;
  }

  boost::optional<WindowPropertyFrameAndDivider> SubSurface_Impl::windowPropertyFrameAndDivider() const
  {
    return getObject<SubSurface>().getModelObjectTarget<WindowPropertyFrameAndDivider>(OS_SubSurfaceFields::FrameandDividerName);
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

      if (!allowShadingControl()){
        this->resetShadingControl();
      }

      if (!allowWindowPropertyFrameAndDivider()){
        this->resetWindowPropertyFrameAndDivider();
      }

      if (!allowDaylightingDeviceShelf()){
        boost::optional<DaylightingDeviceShelf> shelf = this->daylightingDeviceShelf();
        if (shelf){
          shelf->remove();
        }
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
    if (allowShadingControl()){
      result = setPointer(OS_SubSurfaceFields::ShadingControlName, shadingControl.handle());
    }
    return result;
  }

  void SubSurface_Impl::resetShadingControl()
  {
    bool result = setString(OS_SubSurfaceFields::ShadingControlName, "");
    OS_ASSERT(result);
  }

  bool SubSurface_Impl::setWindowPropertyFrameAndDivider(const WindowPropertyFrameAndDivider& windowPropertyFrameAndDivider)
  {
    bool result = false;
    if (allowWindowPropertyFrameAndDivider()){
      result = setPointer(OS_SubSurfaceFields::FrameandDividerName, windowPropertyFrameAndDivider.handle());
    }
    return result;
  }

  void SubSurface_Impl::resetWindowPropertyFrameAndDivider()
  {
    bool result = setString(OS_SubSurfaceFields::FrameandDividerName, "");
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

    for (WorkspaceObject wo : this->getSources(IddObjectType::OS_SubSurface)){
      test = wo.setString(OS_SubSurfaceFields::OutsideBoundaryConditionObject, "");
      OS_ASSERT(test);
    }
  }

  boost::optional<SurfacePropertyConvectionCoefficients> SubSurface_Impl::surfacePropertyConvectionCoefficients() const {
    std::vector<SurfacePropertyConvectionCoefficients> allspccs(model().getConcreteModelObjects<SurfacePropertyConvectionCoefficients>());
    std::vector<SurfacePropertyConvectionCoefficients> spccs;
    for (auto& spcc : allspccs) {
        OptionalSubSurface surface = spcc.surfaceAsSubSurface();
        if (surface) {
            if (surface->handle() == handle()) {
                spccs.push_back(spcc);
            }
        }
    }
    if (spccs.empty()) {
      return boost::none;
    } else if (spccs.size() == 1) {
      return spccs.at(0);
    } else {
      LOG(Error, "More than one SurfacePropertyConvectionCoefficients points to this SubSurface");
      return boost::none;
    }
  }

  boost::optional<SurfacePropertyOtherSideCoefficients> SubSurface_Impl::surfacePropertyOtherSideCoefficients() const
  {
    return getObject<SubSurface>().getModelObjectTarget<SurfacePropertyOtherSideCoefficients>(OS_SubSurfaceFields::OutsideBoundaryConditionObject);
  }

  bool SubSurface_Impl::setSurfacePropertyOtherSideCoefficients(SurfacePropertyOtherSideCoefficients& otherSideCoefficients)
  {
    boost::optional<SubSurface> adjacentSubSurface = this->adjacentSubSurface();
    if (adjacentSubSurface){
      resetAdjacentSubSurface();
    }

    // this is basically testing if otherSideCoefficients is in same model as this
    bool test = this->setPointer(OS_SubSurfaceFields::OutsideBoundaryConditionObject, otherSideCoefficients.handle());
    if (!test){
      if (adjacentSubSurface){
        setAdjacentSubSurface(*adjacentSubSurface);
      }
    }
    return test;
  }

  void SubSurface_Impl::resetSurfacePropertyOtherSideCoefficients()
  {
    bool test = setString(OS_SubSurfaceFields::OutsideBoundaryConditionObject, "");
    OS_ASSERT(test);
  }

  boost::optional<SurfacePropertyOtherSideConditionsModel> SubSurface_Impl::surfacePropertyOtherSideConditionsModel() const
  {
    return getObject<SubSurface>().getModelObjectTarget<SurfacePropertyOtherSideConditionsModel>(OS_SubSurfaceFields::OutsideBoundaryConditionObject);
  }

  bool SubSurface_Impl::setSurfacePropertyOtherSideConditionsModel(SurfacePropertyOtherSideConditionsModel& otherSideModel)
  {
    boost::optional<SubSurface> adjacentSubSurface = this->adjacentSubSurface();
    if (adjacentSubSurface){
      resetAdjacentSubSurface();
    }

    // this is basically testing if otherSideModel is in same model as this
    bool test = this->setPointer(OS_SubSurfaceFields::OutsideBoundaryConditionObject, otherSideModel.handle());
    if (!test){
      if (adjacentSubSurface){
        setAdjacentSubSurface(*adjacentSubSurface);
      }
    }
    return test;
  }

  void SubSurface_Impl::resetSurfacePropertyOtherSideConditionsModel()
  {
    bool test = setString(OS_SubSurfaceFields::OutsideBoundaryConditionObject, "");
    OS_ASSERT(test);
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
        for (const Point3d& point : surface->vertices()){
          surfaceMinZ = std::min(surfaceMinZ, point.z());
        }

        double thisMinZ = std::numeric_limits<double>::max();
        for (const Point3d& point : this->vertices()){
          thisMinZ = std::min(thisMinZ, point.z());
        }

        if (thisMinZ <= surfaceMinZ){
          bool isGlassDoor = false;

          // DLM: this surface could have been initialized to FixedWindow and get its construction
          // from the default construction set, this was the source of #1924
          boost::optional<ConstructionBase> construction = this->construction();
          if (!this->isConstructionDefaulted() && construction && construction->isFenestration()){
            isGlassDoor = true;
          }

          boost::optional<std::string> value = getString(OS_SubSurfaceFields::SubSurfaceType);
          if (value && istringEqual("GlassDoor", *value)){
            isGlassDoor = true;
          }


          if (isGlassDoor){
            result = "GlassDoor";
          }else{
            result = "Door";
          }
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
      for (const Point3d& faceVertex : faceVertices){
        xmin = std::min(xmin, faceVertex.x());
        xmax = std::max(xmax, faceVertex.x());
        ymin = std::min(ymin, faceVertex.y());
        ymax = std::max(ymax, faceVertex.y());
      }
      if ((xmin > xmax) || (ymin > ymax)){
        return boost::none;
      }

      Point3dVector overhangVertices;
      overhangVertices.push_back(Point3d(xmax + offset, ymax + offset, 0));
      overhangVertices.push_back(Point3d(xmin - offset, ymax + offset, 0));
      overhangVertices.push_back(Point3d(xmin - offset, ymax + offset, depth));
      overhangVertices.push_back(Point3d(xmax + offset, ymax + offset, depth));

      ShadingSurfaceGroup shadingSurfaceGroup(model);
      shadingSurfaceGroup.setName(this->name().get() + " Shading Surfaces");
      shadingSurfaceGroup.setSpace(*space);

      shadingSurface = ShadingSurface(transformation*overhangVertices, model);
      shadingSurface->setShadingSurfaceGroup(shadingSurfaceGroup);

      shadingSurfaceGroup.setShadedSubSurface(getObject<SubSurface>());
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
      for (const Point3d& faceVertex : faceVertices){
        xmin = std::min(xmin, faceVertex.x());
        xmax = std::max(xmax, faceVertex.x());
        ymin = std::min(ymin, faceVertex.y());
        ymax = std::max(ymax, faceVertex.y());
      }
      if ((xmin > xmax) || (ymin > ymax)){
        return boost::none;
      }

      double offset = offsetFraction*(ymax-ymin);
      double depth = projectionFactor*(offset + (ymax-ymin));

      Point3dVector overhangVertices;
      overhangVertices.push_back(Point3d(xmax + offset, ymax + offset, 0));
      overhangVertices.push_back(Point3d(xmin - offset, ymax + offset, 0));
      overhangVertices.push_back(Point3d(xmin - offset, ymax + offset, depth));
      overhangVertices.push_back(Point3d(xmax + offset, ymax + offset, depth));

      ShadingSurfaceGroup shadingSurfaceGroup(model);
      shadingSurfaceGroup.setName(this->name().get() + " Shading Surfaces");
      shadingSurfaceGroup.setSpace(*space);

      shadingSurface = ShadingSurface(transformation*overhangVertices, model);
      shadingSurface->setShadingSurfaceGroup(shadingSurfaceGroup);

      shadingSurfaceGroup.setShadedSubSurface(getObject<SubSurface>());
    }

    return shadingSurface;
  }

  std::vector<ShadingSurfaceGroup> SubSurface_Impl::shadingSurfaceGroups() const
  {
    return getObject<ModelObject>().getModelObjectSources<ShadingSurfaceGroup>(ShadingSurfaceGroup::iddObjectType());
  }

  bool SubSurface_Impl::allowDaylightingDeviceShelf() const
  {
    bool result = false;

    std::string subSurfaceType = this->subSurfaceType();
    if (istringEqual(subSurfaceType, "FixedWindow") ||
        istringEqual(subSurfaceType, "OperableWindow") ||
        istringEqual(subSurfaceType, "GlassDoor"))
    {
      result = true;
    }

    return result;
  }

  boost::optional<DaylightingDeviceShelf> SubSurface_Impl::daylightingDeviceShelf() const
  {
    boost::optional<DaylightingDeviceShelf> result;

    std::vector<DaylightingDeviceShelf> shelves = getObject<ModelObject>().getModelObjectSources<DaylightingDeviceShelf>(DaylightingDeviceShelf::iddObjectType());
    if (shelves.size() == 1){
      result = shelves[0];
    }else if (shelves.size() > 1){
      result = shelves[0];
    }
    return result;
  }

  boost::optional<DaylightingDeviceShelf> SubSurface_Impl::addDaylightingDeviceShelf() const
  {
    boost::optional<DaylightingDeviceShelf> result = this->daylightingDeviceShelf();
    if (result){
      return result;
    }

    if (allowDaylightingDeviceShelf())
    {
      try{
        result = DaylightingDeviceShelf(getObject<SubSurface>());
      }catch(const std::exception&){
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

bool SubSurface::allowShadingControl() const
{
  return getImpl<detail::SubSurface_Impl>()->allowShadingControl();
}

boost::optional<ShadingControl> SubSurface::shadingControl() const
{
  return getImpl<detail::SubSurface_Impl>()->shadingControl();
}

bool SubSurface::allowWindowPropertyFrameAndDivider() const
{
  return getImpl<detail::SubSurface_Impl>()->allowWindowPropertyFrameAndDivider();
}

boost::optional<WindowPropertyFrameAndDivider> SubSurface::windowPropertyFrameAndDivider() const
{
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

bool SubSurface::setWindowPropertyFrameAndDivider(const WindowPropertyFrameAndDivider& windowPropertyFrameAndDivider)
{
  return getImpl<detail::SubSurface_Impl>()->setWindowPropertyFrameAndDivider(windowPropertyFrameAndDivider);
}

void SubSurface::resetWindowPropertyFrameAndDivider()
{
  getImpl<detail::SubSurface_Impl>()->resetWindowPropertyFrameAndDivider();
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

boost::optional<SurfacePropertyConvectionCoefficients> SubSurface::surfacePropertyConvectionCoefficients() const {
  return getImpl<detail::SubSurface_Impl>()->surfacePropertyConvectionCoefficients();
}

boost::optional<SurfacePropertyOtherSideCoefficients> SubSurface::surfacePropertyOtherSideCoefficients() const {
  return getImpl<detail::SubSurface_Impl>()->surfacePropertyOtherSideCoefficients();
}

bool SubSurface::setSurfacePropertyOtherSideCoefficients(SurfacePropertyOtherSideCoefficients& otherSideCoefficients) {
  return getImpl<detail::SubSurface_Impl>()->setSurfacePropertyOtherSideCoefficients(otherSideCoefficients);
}

void SubSurface::resetSurfacePropertyOtherSideCoefficients() {
  return getImpl<detail::SubSurface_Impl>()->resetSurfacePropertyOtherSideCoefficients();
}

boost::optional<SurfacePropertyOtherSideConditionsModel> SubSurface::surfacePropertyOtherSideConditionsModel() const {
  return getImpl<detail::SubSurface_Impl>()->surfacePropertyOtherSideConditionsModel();
}

bool SubSurface::setSurfacePropertyOtherSideConditionsModel(SurfacePropertyOtherSideConditionsModel& otherSideModel) {
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

boost::optional<ShadingSurface> SubSurface::addOverhang(double depth, double offset)
{
  return getImpl<detail::SubSurface_Impl>()->addOverhang(depth, offset);
}

boost::optional<ShadingSurface> SubSurface::addOverhangByProjectionFactor(double projectionFactor, double offsetFraction)
{
  return getImpl<detail::SubSurface_Impl>()->addOverhangByProjectionFactor(projectionFactor, offsetFraction);
}

std::vector<ShadingSurfaceGroup> SubSurface::shadingSurfaceGroups() const
{
  return getImpl<detail::SubSurface_Impl>()->shadingSurfaceGroups();
}

bool SubSurface::allowDaylightingDeviceShelf() const
{
  return getImpl<detail::SubSurface_Impl>()->allowDaylightingDeviceShelf();
}

boost::optional<DaylightingDeviceShelf> SubSurface::daylightingDeviceShelf() const
{
  return getImpl<detail::SubSurface_Impl>()->daylightingDeviceShelf();
}

boost::optional<DaylightingDeviceShelf> SubSurface::addDaylightingDeviceShelf() const
{
  return getImpl<detail::SubSurface_Impl>()->addDaylightingDeviceShelf();
}

/// @cond
SubSurface::SubSurface(std::shared_ptr<detail::SubSurface_Impl> impl)
  : PlanarSurface(std::move(impl))
{}
/// @endcond

std::vector<SubSurface> applySkylightPattern(const std::vector<std::vector<Point3d> >& pattern, const std::vector<Space>& spaces, const boost::optional<ConstructionBase>& construction)
{
  double inset = 0.0254;

  std::vector<SubSurface> result;

  for (const Space& space : spaces){

    if (space.isPlenum()){
      LOG_FREE(Warn, "OpenStudio.applySkylightPattern", "Cannot apply skylights to plenum space");
      continue;
    }

    Transformation transformation = space.buildingTransformation();
    Transformation inverseTransformation = transformation.inverse();

    std::vector<std::vector<Point3d> > spacePattern;
    spacePattern.reserve(pattern.size());
    for (const std::vector<Point3d>& face : pattern){
      spacePattern.push_back(inverseTransformation*face);
    }

    for (Surface surface : space.surfaces()){
      if (istringEqual("RoofCeiling", surface.surfaceType()) &&
          istringEqual("Outdoors", surface.outsideBoundaryCondition())){

        Plane surfacePlane = surface.plane();

        std::vector<std::vector<Point3d> > surfacePattern;
        spacePattern.reserve(pattern.size());
        for (const std::vector<Point3d>& spaceFace : spacePattern){
          surfacePattern.push_back(surfacePlane.project(spaceFace));
        }

        std::vector<SubSurface> newSkylights = surface.createSubSurfaces(surfacePattern, inset, construction);
        result.insert(result.end(), newSkylights.begin(), newSkylights.end());
      }
    }
  }

  return result;
}


} // model
} // openstudio

