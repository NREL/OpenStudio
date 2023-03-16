/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
