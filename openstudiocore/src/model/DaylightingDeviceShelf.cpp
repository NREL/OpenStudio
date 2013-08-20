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

#include <model/DaylightingDeviceShelf.hpp>
#include <model/DaylightingDeviceShelf_Impl.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/SubSurface.hpp>
#include <model/SubSurface_Impl.hpp>

#include <utilities/idd/OS_DaylightingDevice_Shelf_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

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

  boost::optional<double> DaylightingDeviceShelf_Impl::viewFactortoOutsideShelf() const {
    return getDouble(OS_DaylightingDevice_ShelfFields::ViewFactortoOutsideShelf,true);
  }

  bool DaylightingDeviceShelf_Impl::setViewFactortoOutsideShelf(boost::optional<double> viewFactortoOutsideShelf) {
    bool result = false;
    if (viewFactortoOutsideShelf) {
      result = setDouble(OS_DaylightingDevice_ShelfFields::ViewFactortoOutsideShelf, viewFactortoOutsideShelf.get());
    } else {
      result = setString(OS_DaylightingDevice_ShelfFields::ViewFactortoOutsideShelf, "");
    }
    return result;
  }

  bool DaylightingDeviceShelf_Impl::setViewFactortoOutsideShelf(double viewFactortoOutsideShelf) {
    bool result = false;
    result = setDouble(OS_DaylightingDevice_ShelfFields::ViewFactortoOutsideShelf, viewFactortoOutsideShelf);
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

  bool DaylightingDeviceShelf_Impl::setSubSurface(const SubSurface& subSurface)
  {
    return setPointer(OS_DaylightingDevice_ShelfFields::WindowName, subSurface.handle());
  }

} // detail

DaylightingDeviceShelf::DaylightingDeviceShelf(const SubSurface& subSurface)
  : ModelObject(DaylightingDeviceShelf::iddObjectType(), subSurface.model())
{
  OS_ASSERT(getImpl<detail::DaylightingDeviceShelf_Impl>());
  OS_ASSERT(this->setSubSurface(subSurface)); 
}

IddObjectType DaylightingDeviceShelf::iddObjectType() {
  IddObjectType result(IddObjectType::OS_DaylightingDevice_Shelf);
  return result;
}

boost::optional<double> DaylightingDeviceShelf::viewFactortoOutsideShelf() const {
  return getImpl<detail::DaylightingDeviceShelf_Impl>()->viewFactortoOutsideShelf();
}

bool DaylightingDeviceShelf::setViewFactortoOutsideShelf(boost::optional<double> viewFactortoOutsideShelf) {
  return getImpl<detail::DaylightingDeviceShelf_Impl>()->setViewFactortoOutsideShelf(viewFactortoOutsideShelf);
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

bool DaylightingDeviceShelf::setSubSurface(const SubSurface& subSurface)
{
  return getImpl<detail::DaylightingDeviceShelf_Impl>()->setSubSurface(subSurface);
}

/// @cond
DaylightingDeviceShelf::DaylightingDeviceShelf(boost::shared_ptr<detail::DaylightingDeviceShelf_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond


} // model
} // openstudio

