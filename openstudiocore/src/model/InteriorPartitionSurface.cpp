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

#include "InteriorPartitionSurface.hpp"
#include "InteriorPartitionSurface_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "InteriorPartitionSurfaceGroup.hpp"
#include "InteriorPartitionSurfaceGroup_Impl.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "DaylightingDeviceShelf.hpp"
#include "DaylightingDeviceShelf_Impl.hpp"

#include <utilities/idd/OS_InteriorPartitionSurface_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

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

    // solar collectors?


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
      return std::make_pair(*construction, 0);
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

  boost::optional<DaylightingDeviceShelf> InteriorPartitionSurface_Impl::daylightingDeviceShelf() const
  {
    std::vector<DaylightingDeviceShelf> sources = getObject<ModelObject>().getModelObjectSources<DaylightingDeviceShelf>();
    if (sources.empty()){
      return boost::none;
    }
    if (sources.size() > 1){
      LOG(Error, "InteriorPartitionSurface is referenced by more than one DaylightingDeviceShelf, returning first");
    }
    return sources[0];
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

boost::optional<DaylightingDeviceShelf> InteriorPartitionSurface::daylightingDeviceShelf() const
{
  return getImpl<detail::InteriorPartitionSurface_Impl>()->daylightingDeviceShelf();
}

/// @cond
InteriorPartitionSurface::InteriorPartitionSurface(std::shared_ptr<detail::InteriorPartitionSurface_Impl> impl)
  : PlanarSurface(std::move(impl))
{}
/// @endcond


} // model
} // openstudio

