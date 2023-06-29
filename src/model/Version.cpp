/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

    Version_Impl::Version_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == Version::iddObjectType());
    }

    Version_Impl::Version_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == Version::iddObjectType());
    }

    Version_Impl::Version_Impl(const Version_Impl& other, Model_Impl* model, bool keepHandle) : ModelObject_Impl(other, model, keepHandle) {}

    IddObjectType Version_Impl::iddObjectType() const {
      return Version::iddObjectType();
    }

    std::string Version_Impl::versionIdentifier() const {
      return getString(OS_VersionFields::VersionIdentifier).get();
    }

    bool Version_Impl::setVersionIdentifier(const std::string& s) {
      return setString(OS_VersionFields::VersionIdentifier, s);
      ;
    }

    boost::optional<std::string> Version_Impl::prereleaseIdentifier() const {
      return getString(OS_VersionFields::PrereleaseIdentifier, false, true);
    }

    bool Version_Impl::setPrereleaseIdentifier(const std::string& s) {
      return setString(OS_VersionFields::PrereleaseIdentifier, s);
      ;
    }

    // return the parent object in the hierarchy
    boost::optional<ParentObject> Version_Impl::parent() const {
      boost::optional<ParentObject> result;
      return result;
    }

    bool Version_Impl::setParent(ParentObject& /*newParent*/) {
      return false;
    }

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& Version_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

  }  // namespace detail

  Version::Version(const Model& model) : ModelObject(iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::Version_Impl>());
    setVersionIdentifier(openStudioVersion());

    std::string preReleaseTag = openStudioVersionPrerelease();
    if (!preReleaseTag.empty()) {
      setPrereleaseIdentifier(preReleaseTag);
    }
  }

  // constructor
  Version::Version(std::shared_ptr<detail::Version_Impl> impl) : ModelObject(std::move(impl)) {}

  std::string Version::versionIdentifier() const {
    return getImpl<detail::Version_Impl>()->versionIdentifier();
  }

  bool Version::setVersionIdentifier(const std::string& s) {
    return getImpl<detail::Version_Impl>()->setVersionIdentifier(s);
  }

  boost::optional<std::string> Version::prereleaseIdentifier() const {
    return getImpl<detail::Version_Impl>()->prereleaseIdentifier();
  }

  bool Version::setPrereleaseIdentifier(const std::string& s) {
    return getImpl<detail::Version_Impl>()->setPrereleaseIdentifier(s);
  }

  IddObjectType Version::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Version);
    return result;
  }

}  // namespace model
}  // namespace openstudio
