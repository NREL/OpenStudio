/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ProgramControl.hpp"
#include "ProgramControl_Impl.hpp"

#include <utilities/idd/OS_ProgramControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ProgramControl_Impl::ProgramControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ProgramControl::iddObjectType());
    }

    ProgramControl_Impl::ProgramControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ProgramControl::iddObjectType());
    }

    ProgramControl_Impl::ProgramControl_Impl(const ProgramControl_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ProgramControl_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ProgramControl_Impl::iddObjectType() const {
      return ProgramControl::iddObjectType();
    }

    boost::optional<int> ProgramControl_Impl::numberofThreadsAllowed() const {
      return getInt(OS_ProgramControlFields::NumberofThreadsAllowed, true);
    }

    bool ProgramControl_Impl::setNumberofThreadsAllowed(boost::optional<int> numberofThreadsAllowed) {
      bool result(false);
      if (numberofThreadsAllowed) {
        result = setInt(OS_ProgramControlFields::NumberofThreadsAllowed, numberofThreadsAllowed.get());
      } else {
        resetNumberofThreadsAllowed();
        result = true;
      }
      return result;
    }

    void ProgramControl_Impl::resetNumberofThreadsAllowed() {
      bool result = setString(OS_ProgramControlFields::NumberofThreadsAllowed, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  ProgramControl::ProgramControl(const Model& model) : ModelObject(ProgramControl::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ProgramControl_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    bool ok = true;
    // ok = setHandle();
    OS_ASSERT(ok);
  }

  IddObjectType ProgramControl::iddObjectType() {
    return {IddObjectType::OS_ProgramControl};
  }

  boost::optional<int> ProgramControl::numberofThreadsAllowed() const {
    return getImpl<detail::ProgramControl_Impl>()->numberofThreadsAllowed();
  }

  bool ProgramControl::setNumberofThreadsAllowed(int numberofThreadsAllowed) {
    return getImpl<detail::ProgramControl_Impl>()->setNumberofThreadsAllowed(numberofThreadsAllowed);
  }

  void ProgramControl::resetNumberofThreadsAllowed() {
    getImpl<detail::ProgramControl_Impl>()->resetNumberofThreadsAllowed();
  }

  /// @cond
  ProgramControl::ProgramControl(std::shared_ptr<detail::ProgramControl_Impl> impl) : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
