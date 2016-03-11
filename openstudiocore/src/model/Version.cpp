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

#include "Version.hpp"
#include "Version_Impl.hpp"

#include "ParentObject.hpp"

#include <utilities/idd/OS_Version_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <OpenStudio.hxx>

namespace openstudio {
namespace model {

namespace detail {

  Version_Impl::Version_Impl(const IdfObject& idfObject,
                             Model_Impl* model,
                             bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Version::iddObjectType());
  }

  Version_Impl::Version_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                             Model_Impl* model,
                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Version::iddObjectType());
  }

  Version_Impl::Version_Impl(const Version_Impl& other,
                             Model_Impl* model,
                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
  }

  IddObjectType Version_Impl::iddObjectType() const {
    return Version::iddObjectType();
  }

  std::string Version_Impl::versionIdentifier() const
  {
    return getString(OS_VersionFields::VersionIdentifier).get();
  }

  void Version_Impl::setVersionIdentifier(const std::string& s)
  {
    setString(OS_VersionFields::VersionIdentifier,s);
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> Version_Impl::parent() const
  {
    boost::optional<ParentObject> result;
    return result;
  }

  bool Version_Impl::setParent(ParentObject& newParent)
  {
    return false;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& Version_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

} //detail

Version::Version(const Model& model)
  : ModelObject(iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::Version_Impl>());
  setVersionIdentifier( openStudioVersion() );
}

// constructor
Version::Version(std::shared_ptr<detail::Version_Impl> impl)
  : ModelObject(impl)
{}

std::string Version::versionIdentifier() const {
  return getImpl<detail::Version_Impl>()->versionIdentifier();
}

void Version::setVersionIdentifier(const std::string& s)
{
  getImpl<detail::Version_Impl>()->setVersionIdentifier(s);
}

IddObjectType Version::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Version);
  return result;
}

} // model
} // openstudio
