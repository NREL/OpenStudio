/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "WeatherFileDays.hpp"
#include "WeatherFileDays_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"

#include <utilities/idd/OS_SizingPeriod_WeatherFileDays_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    WeatherFileDays_Impl::WeatherFileDays_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : SizingPeriod_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == WeatherFileDays::iddObjectType());
    }

    WeatherFileDays_Impl::WeatherFileDays_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : SizingPeriod_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == WeatherFileDays::iddObjectType());
    }

    WeatherFileDays_Impl::WeatherFileDays_Impl(const WeatherFileDays_Impl& other, Model_Impl* model, bool keepHandle)
      : SizingPeriod_Impl(other, model, keepHandle) {}

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& WeatherFileDays_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType WeatherFileDays_Impl::iddObjectType() const {
      return WeatherFileDays::iddObjectType();
    }

    void WeatherFileDays_Impl::ensureNoLeapDays() {
      boost::optional<int> month;
      boost::optional<int> day;

      month = getInt(OS_SizingPeriod_WeatherFileDaysFields::BeginMonth);
      if (month && (month.get() == 2)) {
        day = this->getInt(OS_SizingPeriod_WeatherFileDaysFields::BeginDayofMonth);
        if (day && (day.get() == 29)) {
          this->setInt(OS_SizingPeriod_WeatherFileDaysFields::BeginDayofMonth, 28);
        }
      }

      month = getInt(OS_SizingPeriod_WeatherFileDaysFields::EndMonth);
      if (month && (month.get() == 2)) {
        day = this->getInt(OS_SizingPeriod_WeatherFileDaysFields::EndDayofMonth);
        if (day && (day.get() == 29)) {
          this->setInt(OS_SizingPeriod_WeatherFileDaysFields::EndDayofMonth, 28);
        }
      }
    }

  }  // namespace detail

  /// constructor
  WeatherFileDays::WeatherFileDays(const Model& model) : SizingPeriod(WeatherFileDays::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::WeatherFileDays_Impl>());
  }

  // constructor
  WeatherFileDays::WeatherFileDays(std::shared_ptr<detail::WeatherFileDays_Impl> impl) : SizingPeriod(std::move(impl)) {}

  IddObjectType WeatherFileDays::iddObjectType() {
    IddObjectType result(IddObjectType::OS_SizingPeriod_WeatherFileDays);
    return result;
  }

}  // namespace model
}  // namespace openstudio
