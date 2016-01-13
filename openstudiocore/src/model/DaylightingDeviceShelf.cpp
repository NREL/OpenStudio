/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "DaylightingDeviceShelf.hpp"
#include "DaylightingDeviceShelf_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "InteriorPartitionSurface.hpp"
#include "InteriorPartitionSurface_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"

#include <utilities/idd/OS_DaylightingDevice_Shelf_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  DaylightingDeviceShelf_Impl::DaylightingDeviceShelf_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == DaylightingDeviceShelf::iddObjectType());
  }

  DaylightingDeviceShelf_Impl::DaylightingDeviceShelf_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == DaylightingDeviceShelf::iddObjectType());
  }

  DaylightingDeviceShelf_Impl::DaylightingDeviceShelf_Impl(const DaylightingDeviceShelf_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& DaylightingDeviceShelf_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType DaylightingDeviceShelf_Impl::iddObjectType() const {
    return DaylightingDeviceShelf::iddObjectType();
  }

  boost::optional<InteriorPartitionSurface> DaylightingDeviceShelf_Impl::insideShelf() const
  {
    return getObject<DaylightingDeviceShelf>().getModelObjectTarget<InteriorPartitionSurface>(OS_DaylightingDevice_ShelfFields::InsideShelfName);
  }

  boost::optional<ShadingSurface> DaylightingDeviceShelf_Impl::outsideShelf() const
  {
    return getObject<DaylightingDeviceShelf>().getModelObjectTarget<ShadingSurface>(OS_DaylightingDevice_ShelfFields::OutsideShelfName);
  }

  boost::optional<double> DaylightingDeviceShelf_Impl::viewFactortoOutsideShelf() const {
    return getDouble(OS_DaylightingDevice_ShelfFields::ViewFactortoOutsideShelf,true);
  }

  bool DaylightingDeviceShelf_Impl::setInsideShelf(const InteriorPartitionSurface& insideShelf)
  {
    boost::optional<Space> insideShelfSpace = insideShelf.space();
    if (!insideShelfSpace){
      return false;
    }

    SubSurface subSurface = this->subSurface();
    boost::optional<Space> subSurfaceSpace = subSurface.space();
    if (!subSurfaceSpace){
      return false;
    }

    if (insideShelfSpace->handle() != subSurfaceSpace->handle()){
      return false;
    }

    // DLM: check that this insideShelf is not used by any other daylighting light shelf?

    bool result = setPointer(OS_DaylightingDevice_ShelfFields::InsideShelfName, insideShelf.handle());
    OS_ASSERT(result);
    return result;
  }

  bool DaylightingDeviceShelf_Impl::setOutsideShelf(const ShadingSurface& outsideShelf)
  {
    boost::optional<Space> outsideShelfSpace = outsideShelf.space();
    if (!outsideShelfSpace){
      return false;
    }

    SubSurface subSurface = this->subSurface();
    boost::optional<Space> subSurfaceSpace = subSurface.space();
    if (!subSurfaceSpace){
      return false;
    }

    if (outsideShelfSpace->handle() != subSurfaceSpace->handle()){
      return false;
    }

    // DLM: check that this outsideShelf is not used by any other daylighting light shelf?

    bool result = setPointer(OS_DaylightingDevice_ShelfFields::OutsideShelfName, outsideShelf.handle());
    OS_ASSERT(result);
    return result;
  }

  bool DaylightingDeviceShelf_Impl::setViewFactortoOutsideShelf(double viewFactortoOutsideShelf) {
    bool result = setDouble(OS_DaylightingDevice_ShelfFields::ViewFactortoOutsideShelf, viewFactortoOutsideShelf);
    return result;
  }

  void DaylightingDeviceShelf_Impl::resetViewFactortoOutsideShelf() {
    bool result = setString(OS_DaylightingDevice_ShelfFields::ViewFactortoOutsideShelf, "");
    OS_ASSERT(result);
  }

  SubSurface DaylightingDeviceShelf_Impl::subSurface() const
  {
    OptionalSubSurface subSurface = getObject<ModelObject>().getModelObjectTarget<SubSurface>(OS_DaylightingDevice_ShelfFields::WindowName);
    OS_ASSERT(subSurface);
    return subSurface.get();
  }

} // detail

DaylightingDeviceShelf::DaylightingDeviceShelf(const SubSurface& subSurface)
  : ModelObject(DaylightingDeviceShelf::iddObjectType(), subSurface.model())
{
  OS_ASSERT(getImpl<detail::DaylightingDeviceShelf_Impl>());

  bool subSurfaceOk = false;
  if (subSurface.allowDaylightingDeviceShelf()){
    if (!subSurface.daylightingDeviceShelf()){
      subSurfaceOk = true;
    }
  }

  if (!subSurfaceOk){
    this->remove();
    LOG_AND_THROW("Cannot create DaylightingDeviceShelf for SubSurface '" << subSurface.name().get() << "'");
  }

  bool test = setPointer(OS_DaylightingDevice_ShelfFields::WindowName, subSurface.handle());
  OS_ASSERT(test); 
}

IddObjectType DaylightingDeviceShelf::iddObjectType() {
  IddObjectType result(IddObjectType::OS_DaylightingDevice_Shelf);
  return result;
}

boost::optional<InteriorPartitionSurface> DaylightingDeviceShelf::insideShelf() const {
  return getImpl<detail::DaylightingDeviceShelf_Impl>()->insideShelf();
}

boost::optional<ShadingSurface> DaylightingDeviceShelf::outsideShelf() const {
  return getImpl<detail::DaylightingDeviceShelf_Impl>()->outsideShelf();
}

boost::optional<double> DaylightingDeviceShelf::viewFactortoOutsideShelf() const {
  return getImpl<detail::DaylightingDeviceShelf_Impl>()->viewFactortoOutsideShelf();
}

bool DaylightingDeviceShelf::setInsideShelf(const InteriorPartitionSurface& insideShelf){
  return getImpl<detail::DaylightingDeviceShelf_Impl>()->setInsideShelf(insideShelf);
}

bool DaylightingDeviceShelf::setOutsideShelf(const ShadingSurface& outsideShelf){
  return getImpl<detail::DaylightingDeviceShelf_Impl>()->setOutsideShelf(outsideShelf);
}

bool DaylightingDeviceShelf::setViewFactortoOutsideShelf(double viewFactortoOutsideShelf) {
  return getImpl<detail::DaylightingDeviceShelf_Impl>()->setViewFactortoOutsideShelf(viewFactortoOutsideShelf);
}

void DaylightingDeviceShelf::resetViewFactortoOutsideShelf() {
  getImpl<detail::DaylightingDeviceShelf_Impl>()->resetViewFactortoOutsideShelf();
}

SubSurface DaylightingDeviceShelf::subSurface() const
{
  return getImpl<detail::DaylightingDeviceShelf_Impl>()->subSurface();
}

/// @cond
DaylightingDeviceShelf::DaylightingDeviceShelf(std::shared_ptr<detail::DaylightingDeviceShelf_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond


} // model
} // openstudio

