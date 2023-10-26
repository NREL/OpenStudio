/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OutputControlTimestamp.hpp"
#include "OutputControlTimestamp_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_OutputControl_Timestamp_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    OutputControlTimestamp_Impl::OutputControlTimestamp_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OutputControlTimestamp::iddObjectType());
    }

    OutputControlTimestamp_Impl::OutputControlTimestamp_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                             bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OutputControlTimestamp::iddObjectType());
    }

    OutputControlTimestamp_Impl::OutputControlTimestamp_Impl(const OutputControlTimestamp_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OutputControlTimestamp_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType OutputControlTimestamp_Impl::iddObjectType() const {
      return OutputControlTimestamp::iddObjectType();
    }

    bool OutputControlTimestamp_Impl::iso8601Format() const {
      return getBooleanFieldValue(OS_OutputControl_TimestampFields::ISO8601Format);
    }

    bool OutputControlTimestamp_Impl::timestampAtBeginningOfInterval() const {
      return getBooleanFieldValue(OS_OutputControl_TimestampFields::TimestampatBeginningofInterval);
    }

    bool OutputControlTimestamp_Impl::setISO8601Format(bool iso8601Format) {
      return setBooleanFieldValue(OS_OutputControl_TimestampFields::ISO8601Format, iso8601Format);
    }

    bool OutputControlTimestamp_Impl::setTimestampAtBeginningOfInterval(bool timestampAtBeginningOfInterval) {
      return setBooleanFieldValue(OS_OutputControl_TimestampFields::TimestampatBeginningofInterval, timestampAtBeginningOfInterval);
    }

  }  // namespace detail

  IddObjectType OutputControlTimestamp::iddObjectType() {
    return IddObjectType(IddObjectType::OS_OutputControl_Timestamp);
  }

  bool OutputControlTimestamp::iso8601Format() const {
    return getImpl<detail::OutputControlTimestamp_Impl>()->iso8601Format();
  }

  bool OutputControlTimestamp::timestampAtBeginningOfInterval() const {
    return getImpl<detail::OutputControlTimestamp_Impl>()->timestampAtBeginningOfInterval();
  }

  bool OutputControlTimestamp::setISO8601Format(bool iso8601Format) {
    return getImpl<detail::OutputControlTimestamp_Impl>()->setISO8601Format(iso8601Format);
  }

  bool OutputControlTimestamp::setTimestampAtBeginningOfInterval(bool timestampAtBeginningOfInterval) {
    return getImpl<detail::OutputControlTimestamp_Impl>()->setTimestampAtBeginningOfInterval(timestampAtBeginningOfInterval);
  }

  /// @cond
  OutputControlTimestamp::OutputControlTimestamp(std::shared_ptr<detail::OutputControlTimestamp_Impl> impl) : ModelObject(std::move(impl)) {}
  OutputControlTimestamp::OutputControlTimestamp(Model& model) : ModelObject(OutputControlTimestamp::iddObjectType(), model) {
    // This is a required-field, so pick one...
    // Same defaults as EnergyPlus IDD
    setISO8601Format(false);
    setTimestampAtBeginningOfInterval(false);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
