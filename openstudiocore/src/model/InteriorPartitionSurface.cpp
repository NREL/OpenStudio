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

#include <model/InteriorPartitionSurface.hpp>
#include <model/InteriorPartitionSurface_Impl.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/InteriorPartitionSurfaceGroup.hpp>
#include <model/InteriorPartitionSurfaceGroup_Impl.hpp>
#include <model/ConstructionBase.hpp>
#include <model/ConstructionBase_Impl.hpp>

#include <utilities/idd/OS_InteriorPartitionSurface_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  InteriorPartitionSurface_Impl::InteriorPartitionSurface_Impl(const IdfObject& idfObject,
                                                               Model_Impl* model, 
                                                               bool keepHandle)
    : PlanarSurface_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == InteriorPartitionSurface::iddObjectType());
  }

  InteriorPartitionSurface_Impl::InteriorPartitionSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : PlanarSurface_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == InteriorPartitionSurface::iddObjectType());
  }

  InteriorPartitionSurface_Impl::InteriorPartitionSurface_Impl(const InteriorPartitionSurface_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : PlanarSurface_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> InteriorPartitionSurface_Impl::parent() const
  {
    boost::optional<ParentObject> result;
    result = this->interiorPartitionSurfaceGroup();
    return result;
  }

  std::vector<ModelObject> InteriorPartitionSurface_Impl::children() const
  {
    std::vector<ModelObject> result;
    return result;
  }

  bool InteriorPartitionSurface_Impl::setParent(ParentObject& newParent)
  {
    bool result = false;
    OptionalInteriorPartitionSurfaceGroup interiorPartitionSurfaceGroup = newParent.optionalCast<InteriorPartitionSurfaceGroup>();
    if (interiorPartitionSurfaceGroup){
      result = this->setInteriorPartitionSurfaceGroup(*interiorPartitionSurfaceGroup);
    }
    return result;
  }

  const std::vector<std::string>& InteriorPartitionSurface_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType InteriorPartitionSurface_Impl::iddObjectType() const {
    return InteriorPartitionSurface::iddObjectType();
  }

  boost::optional<std::pair<ConstructionBase, int> > InteriorPartitionSurface_Impl::constructionWithSearchDistance() const
  {
    boost::optional<std::pair<ConstructionBase, int> > result;

    boost::optional<ConstructionBase> construction = getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_InteriorPartitionSurfaceFields::ConstructionName);
    if (construction){
      return std::make_pair<ConstructionBase, int>(*construction, 0);
    }

    boost::optional<InteriorPartitionSurfaceGroup> interiorPartitionSurfaceGroup = this->interiorPartitionSurfaceGroup();
    if (interiorPartitionSurfaceGroup){
      boost::optional<Space> space = interiorPartitionSurfaceGroup->space();
      if (space){
        result = space->getDefaultConstructionWithSearchDistance(this->getObject<InteriorPartitionSurface>());
      }
    }

    return result;
  }

  bool InteriorPartitionSurface_Impl::isConstructionDefaulted() const
  {
    return isEmpty(OS_InteriorPartitionSurfaceFields::ConstructionName);
  }


  bool InteriorPartitionSurface_Impl::setConstruction(const ConstructionBase& construction)
  {
    return this->setPointer(OS_InteriorPartitionSurfaceFields::ConstructionName, construction.handle());
  }

  void InteriorPartitionSurface_Impl::resetConstruction()
  {
    this->setString(OS_InteriorPartitionSurfaceFields::ConstructionName, "");
  }

  bool InteriorPartitionSurface_Impl::subtractFromGrossArea() const
  {
    return false;
  }

  boost::optional<PlanarSurfaceGroup> InteriorPartitionSurface_Impl::planarSurfaceGroup() const
  {
    return boost::optional<PlanarSurfaceGroup>(this->interiorPartitionSurfaceGroup());
  }

  boost::optional<Space> InteriorPartitionSurface_Impl::space() const
  { 
    boost::optional<Space> result;
    boost::optional<InteriorPartitionSurfaceGroup> interiorPartitionSurfaceGroup = this->interiorPartitionSurfaceGroup();
    if (interiorPartitionSurfaceGroup){
      result = interiorPartitionSurfaceGroup->space();
    }
    return result;
  }

  bool InteriorPartitionSurface_Impl::converttoInternalMass() const {
    boost::optional<std::string> value = getString(OS_InteriorPartitionSurfaceFields::ConverttoInternalMass,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool InteriorPartitionSurface_Impl::isConverttoInternalMassDefaulted() const {
    return isEmpty(OS_InteriorPartitionSurfaceFields::ConverttoInternalMass);
  }

  boost::optional<double> InteriorPartitionSurface_Impl::surfaceArea() const {
    return getDouble(OS_InteriorPartitionSurfaceFields::SurfaceArea,true);
  }

  boost::optional<double> InteriorPartitionSurface_Impl::numberofVertices() const {
    return getDouble(OS_InteriorPartitionSurfaceFields::NumberofVertices,true);
  }

  bool InteriorPartitionSurface_Impl::isNumberofVerticesDefaulted() const {
    return isEmpty(OS_InteriorPartitionSurfaceFields::NumberofVertices);
  }

  bool InteriorPartitionSurface_Impl::isNumberofVerticesAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_InteriorPartitionSurfaceFields::NumberofVertices, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "canAutocalculate");
    }
    return result;
  }

  void InteriorPartitionSurface_Impl::setConverttoInternalMass(bool converttoInternalMass) {
    bool result = false;
    if (converttoInternalMass) {
      result = setString(OS_InteriorPartitionSurfaceFields::ConverttoInternalMass, "Yes");
    } else {
      result = setString(OS_InteriorPartitionSurfaceFields::ConverttoInternalMass, "No");
    }
    OS_ASSERT(result);
  }

  void InteriorPartitionSurface_Impl::resetConverttoInternalMass() {
    bool result = setString(OS_InteriorPartitionSurfaceFields::ConverttoInternalMass, "");
    OS_ASSERT(result);
  }

  bool InteriorPartitionSurface_Impl::setSurfaceArea(boost::optional<double> surfaceArea) {
    bool result = false;
    if (surfaceArea) {
      result = setDouble(OS_InteriorPartitionSurfaceFields::SurfaceArea, surfaceArea.get());
    } else {
      result = setString(OS_InteriorPartitionSurfaceFields::SurfaceArea, "");
    }
    return result;
  }

  bool InteriorPartitionSurface_Impl::setSurfaceArea(double surfaceArea) {
    bool result = setDouble(OS_InteriorPartitionSurfaceFields::SurfaceArea, surfaceArea);
    return result;
  }

  void InteriorPartitionSurface_Impl::resetSurfaceArea() {
    bool result = setString(OS_InteriorPartitionSurfaceFields::SurfaceArea, "");
    OS_ASSERT(result);
  }

  bool InteriorPartitionSurface_Impl::setNumberofVertices(boost::optional<double> numberofVertices) {
    bool result = false;
    if (numberofVertices) {
      result = setDouble(OS_InteriorPartitionSurfaceFields::NumberofVertices, numberofVertices.get());
    } else {
      result = setString(OS_InteriorPartitionSurfaceFields::NumberofVertices, "");
    }
    return result;
  }

  bool InteriorPartitionSurface_Impl::setNumberofVertices(double numberofVertices) {
    bool result = setDouble(OS_InteriorPartitionSurfaceFields::NumberofVertices, numberofVertices);
    return result;
  }

  void InteriorPartitionSurface_Impl::resetNumberofVertices() {
    bool result = setString(OS_InteriorPartitionSurfaceFields::NumberofVertices, "");
    OS_ASSERT(result);
  }

  void InteriorPartitionSurface_Impl::autocalculateNumberofVertices() {
    bool result = setString(OS_InteriorPartitionSurfaceFields::NumberofVertices, "Autocalculate");
    OS_ASSERT(result);
  }

  boost::optional<InteriorPartitionSurfaceGroup> InteriorPartitionSurface_Impl::interiorPartitionSurfaceGroup() const
  {
    boost::optional<InteriorPartitionSurfaceGroup> result;
    OptionalWorkspaceObject object = this->getTarget(OS_InteriorPartitionSurfaceFields::InteriorPartitionSurfaceGroupName);
    if (object){
      result = object->optionalCast<InteriorPartitionSurfaceGroup>();
    }
    return result;
  }

  bool InteriorPartitionSurface_Impl::setInteriorPartitionSurfaceGroup(const InteriorPartitionSurfaceGroup& interiorPartitionSurfaceGroup)
  {
    return setPointer(OS_InteriorPartitionSurfaceFields::InteriorPartitionSurfaceGroupName, 
                      interiorPartitionSurfaceGroup.handle());
  }

  void InteriorPartitionSurface_Impl::resetInteriorPartitionSurfaceGroup() {
    bool ok = setString(OS_InteriorPartitionSurfaceFields::InteriorPartitionSurfaceGroupName,
                        "");
    OS_ASSERT(ok);
  }

  boost::optional<ModelObject> InteriorPartitionSurface_Impl::interiorPartitionSurfaceGroupAsModelObject() const {
    OptionalModelObject result;
    OptionalInteriorPartitionSurfaceGroup intermediate = interiorPartitionSurfaceGroup();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool InteriorPartitionSurface_Impl::setInteriorPartitionSurfaceGroupAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalInteriorPartitionSurfaceGroup intermediate = modelObject->optionalCast<InteriorPartitionSurfaceGroup>();
      if (intermediate) {
        return setInteriorPartitionSurfaceGroup(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetInteriorPartitionSurfaceGroup();
    }
    return true;
  }

} // detail

InteriorPartitionSurface::InteriorPartitionSurface(const std::vector<Point3d>& vertices, const Model& model)
  : PlanarSurface(InteriorPartitionSurface::iddObjectType(),vertices,model)
{
  OS_ASSERT(getImpl<detail::InteriorPartitionSurface_Impl>());
}

IddObjectType InteriorPartitionSurface::iddObjectType() {
  IddObjectType result(IddObjectType::OS_InteriorPartitionSurface);
  return result;
}

bool InteriorPartitionSurface::converttoInternalMass() const {
  return getImpl<detail::InteriorPartitionSurface_Impl>()->converttoInternalMass();
}

bool InteriorPartitionSurface::isConverttoInternalMassDefaulted() const {
  return getImpl<detail::InteriorPartitionSurface_Impl>()->isConverttoInternalMassDefaulted();
}

boost::optional<double> InteriorPartitionSurface::surfaceArea() const {
  return getImpl<detail::InteriorPartitionSurface_Impl>()->surfaceArea();
}

boost::optional<double> InteriorPartitionSurface::numberofVertices() const {
  return getImpl<detail::InteriorPartitionSurface_Impl>()->numberofVertices();
}

bool InteriorPartitionSurface::isNumberofVerticesDefaulted() const {
  return getImpl<detail::InteriorPartitionSurface_Impl>()->isNumberofVerticesDefaulted();
}

bool InteriorPartitionSurface::isNumberofVerticesAutocalculated() const {
  return getImpl<detail::InteriorPartitionSurface_Impl>()->isNumberofVerticesAutocalculated();
}

void InteriorPartitionSurface::setConverttoInternalMass(bool converttoInternalMass) {
  getImpl<detail::InteriorPartitionSurface_Impl>()->setConverttoInternalMass(converttoInternalMass);
}

void InteriorPartitionSurface::resetConverttoInternalMass() {
  getImpl<detail::InteriorPartitionSurface_Impl>()->resetConverttoInternalMass();
}

bool InteriorPartitionSurface::setSurfaceArea(boost::optional<double> surfaceArea) {
  return getImpl<detail::InteriorPartitionSurface_Impl>()->setSurfaceArea(surfaceArea);
}

bool InteriorPartitionSurface::setSurfaceArea(double surfaceArea) {
  return getImpl<detail::InteriorPartitionSurface_Impl>()->setSurfaceArea(surfaceArea);
}

void InteriorPartitionSurface::resetSurfaceArea() {
  getImpl<detail::InteriorPartitionSurface_Impl>()->resetSurfaceArea();
}

bool InteriorPartitionSurface::setNumberofVertices(boost::optional<double> numberofVertices) {
  return getImpl<detail::InteriorPartitionSurface_Impl>()->setNumberofVertices(numberofVertices);
}

bool InteriorPartitionSurface::setNumberofVertices(double numberofVertices) {
  return getImpl<detail::InteriorPartitionSurface_Impl>()->setNumberofVertices(numberofVertices);
}

void InteriorPartitionSurface::resetNumberofVertices() {
  getImpl<detail::InteriorPartitionSurface_Impl>()->resetNumberofVertices();
}

void InteriorPartitionSurface::autocalculateNumberofVertices() {
  getImpl<detail::InteriorPartitionSurface_Impl>()->autocalculateNumberofVertices();
}

boost::optional<InteriorPartitionSurfaceGroup> InteriorPartitionSurface::interiorPartitionSurfaceGroup() const
{
  return getImpl<detail::InteriorPartitionSurface_Impl>()->interiorPartitionSurfaceGroup();
}

bool InteriorPartitionSurface::setInteriorPartitionSurfaceGroup(const InteriorPartitionSurfaceGroup& interiorPartitionSurfaceGroup)
{
  return getImpl<detail::InteriorPartitionSurface_Impl>()->setInteriorPartitionSurfaceGroup(interiorPartitionSurfaceGroup);
}

void InteriorPartitionSurface::resetInteriorPartitionSurfaceGroup() {
  getImpl<detail::InteriorPartitionSurface_Impl>()->resetInteriorPartitionSurfaceGroup();
}

/// @cond
InteriorPartitionSurface::InteriorPartitionSurface(boost::shared_ptr<detail::InteriorPartitionSurface_Impl> impl)
  : PlanarSurface(impl)
{}
/// @endcond


} // model
} // openstudio

