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
  : ModelObject(std::move(impl))
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
