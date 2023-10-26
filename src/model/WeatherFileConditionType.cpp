/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "WeatherFileConditionType.hpp"
#include "WeatherFileConditionType_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    WeatherFileConditionType_Impl::WeatherFileConditionType_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : SizingPeriod_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == WeatherFileConditionType::iddObjectType());
    }

    WeatherFileConditionType_Impl::WeatherFileConditionType_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : SizingPeriod_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == WeatherFileConditionType::iddObjectType());
    }

    WeatherFileConditionType_Impl::WeatherFileConditionType_Impl(const WeatherFileConditionType_Impl& other, Model_Impl* model, bool keepHandle)
      : SizingPeriod_Impl(other, model, keepHandle) {}

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& WeatherFileConditionType_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType WeatherFileConditionType_Impl::iddObjectType() const {
      return WeatherFileConditionType::iddObjectType();
    }

    void WeatherFileConditionType_Impl::ensureNoLeapDays() {
      // nothing to do
    }

  }  // namespace detail

  /// constructor
  WeatherFileConditionType::WeatherFileConditionType(const Model& model) : SizingPeriod(WeatherFileConditionType::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::WeatherFileConditionType_Impl>());
  }

  // constructor
  WeatherFileConditionType::WeatherFileConditionType(std::shared_ptr<detail::WeatherFileConditionType_Impl> impl) : SizingPeriod(std::move(impl)) {}

  IddObjectType WeatherFileConditionType::iddObjectType() {
    IddObjectType result(IddObjectType::OS_SizingPeriod_WeatherFileConditionType);
    return result;
  }

}  // namespace model
}  // namespace openstudio
