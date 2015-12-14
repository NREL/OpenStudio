/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "SurfacePropertyOtherSideConditionsModel.hpp"
#include "SurfacePropertyOtherSideConditionsModel_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SurfaceProperty_OtherSideConditionsModel_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SurfacePropertyOtherSideConditionsModel_Impl::SurfacePropertyOtherSideConditionsModel_Impl(const IdfObject& idfObject,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SurfacePropertyOtherSideConditionsModel::iddObjectType());
  }

  SurfacePropertyOtherSideConditionsModel_Impl::SurfacePropertyOtherSideConditionsModel_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SurfacePropertyOtherSideConditionsModel::iddObjectType());
  }

  SurfacePropertyOtherSideConditionsModel_Impl::SurfacePropertyOtherSideConditionsModel_Impl(const SurfacePropertyOtherSideConditionsModel_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SurfacePropertyOtherSideConditionsModel_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SurfacePropertyOtherSideConditionsModel_Impl::iddObjectType() const {
    return SurfacePropertyOtherSideConditionsModel::iddObjectType();
  }

  std::string SurfacePropertyOtherSideConditionsModel_Impl::typeofModeling() const {
    boost::optional<std::string> value = getString(OS_SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SurfacePropertyOtherSideConditionsModel_Impl::isTypeofModelingDefaulted() const {
    return isEmpty(OS_SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling);
  }

  bool SurfacePropertyOtherSideConditionsModel_Impl::setTypeofModeling(std::string typeofModeling) {
    bool result = setString(OS_SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling, typeofModeling);
    return result;
  }

  void SurfacePropertyOtherSideConditionsModel_Impl::resetTypeofModeling() {
    bool result = setString(OS_SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling, "");
    OS_ASSERT(result);
  }

} // detail

SurfacePropertyOtherSideConditionsModel::SurfacePropertyOtherSideConditionsModel(const Model& model)
  : ResourceObject(SurfacePropertyOtherSideConditionsModel::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SurfacePropertyOtherSideConditionsModel_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
}

IddObjectType SurfacePropertyOtherSideConditionsModel::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SurfaceProperty_OtherSideConditionsModel);
}

std::vector<std::string> SurfacePropertyOtherSideConditionsModel::typeofModelingValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling);
}

std::string SurfacePropertyOtherSideConditionsModel::typeofModeling() const {
  return getImpl<detail::SurfacePropertyOtherSideConditionsModel_Impl>()->typeofModeling();
}

bool SurfacePropertyOtherSideConditionsModel::isTypeofModelingDefaulted() const {
  return getImpl<detail::SurfacePropertyOtherSideConditionsModel_Impl>()->isTypeofModelingDefaulted();
}

bool SurfacePropertyOtherSideConditionsModel::setTypeofModeling(std::string typeofModeling) {
  return getImpl<detail::SurfacePropertyOtherSideConditionsModel_Impl>()->setTypeofModeling(typeofModeling);
}

void SurfacePropertyOtherSideConditionsModel::resetTypeofModeling() {
  getImpl<detail::SurfacePropertyOtherSideConditionsModel_Impl>()->resetTypeofModeling();
}

/// @cond
SurfacePropertyOtherSideConditionsModel::SurfacePropertyOtherSideConditionsModel(std::shared_ptr<detail::SurfacePropertyOtherSideConditionsModel_Impl> impl)
  : ResourceObject(impl)
{}
/// @endcond

} // model
} // openstudio

