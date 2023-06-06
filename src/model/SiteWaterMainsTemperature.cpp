/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SiteWaterMainsTemperature.hpp"
#include "SiteWaterMainsTemperature_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"

#include "../utilities/idf/ValidityReport.hpp"

#include "../utilities/idd/IddKey.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Site_WaterMainsTemperature_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SiteWaterMainsTemperature_Impl::SiteWaterMainsTemperature_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SiteWaterMainsTemperature::iddObjectType());
    }

    SiteWaterMainsTemperature_Impl::SiteWaterMainsTemperature_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                   bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SiteWaterMainsTemperature::iddObjectType());
    }

    SiteWaterMainsTemperature_Impl::SiteWaterMainsTemperature_Impl(const SiteWaterMainsTemperature_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> SiteWaterMainsTemperature_Impl::parent() const {
      boost::optional<Site> result = this->model().getOptionalUniqueModelObject<Site>();
      return boost::optional<ParentObject>(result);
    }

    const std::vector<std::string>& SiteWaterMainsTemperature_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // Not appropriate
      //SetupOutputVariable( "Site Mains Water Temperature [C]", WaterMainsTemp, "Zone", "Average", "Environment" );
      return result;
    }

    IddObjectType SiteWaterMainsTemperature_Impl::iddObjectType() const {
      return SiteWaterMainsTemperature::iddObjectType();
    }

    std::vector<ScheduleTypeKey> SiteWaterMainsTemperature_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Site_WaterMainsTemperatureFields::TemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("SiteWaterMainsTemperature", "Temperature"));
      }
      return result;
    }

    std::string SiteWaterMainsTemperature_Impl::calculationMethod() const {
      boost::optional<std::string> value = getString(OS_Site_WaterMainsTemperatureFields::CalculationMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> SiteWaterMainsTemperature_Impl::temperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Site_WaterMainsTemperatureFields::TemperatureScheduleName);
    }

    boost::optional<double> SiteWaterMainsTemperature_Impl::annualAverageOutdoorAirTemperature() const {
      return getDouble(OS_Site_WaterMainsTemperatureFields::AnnualAverageOutdoorAirTemperature, true);
    }

    boost::optional<double> SiteWaterMainsTemperature_Impl::maximumDifferenceInMonthlyAverageOutdoorAirTemperatures() const {
      return getDouble(OS_Site_WaterMainsTemperatureFields::MaximumDifferenceInMonthlyAverageOutdoorAirTemperatures, true);
    }

    bool SiteWaterMainsTemperature_Impl::setCalculationMethod(const std::string& calculationMethod) {
      bool result = setString(OS_Site_WaterMainsTemperatureFields::CalculationMethod, calculationMethod);
      return result;
    }

    bool SiteWaterMainsTemperature_Impl::setTemperatureSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Site_WaterMainsTemperatureFields::TemperatureScheduleName, "SiteWaterMainsTemperature", "Temperature", schedule);
      if (result) {
        result = setCalculationMethod("Schedule");
        OS_ASSERT(result);
      }
      return result;
    }

    void SiteWaterMainsTemperature_Impl::resetTemperatureSchedule() {
      bool result = setString(OS_Site_WaterMainsTemperatureFields::TemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool SiteWaterMainsTemperature_Impl::setAnnualAverageOutdoorAirTemperature(boost::optional<double> annualAverageOutdoorAirTemperature) {
      bool result(false);
      if (annualAverageOutdoorAirTemperature) {
        result = setDouble(OS_Site_WaterMainsTemperatureFields::AnnualAverageOutdoorAirTemperature, annualAverageOutdoorAirTemperature.get());
        if (result) {
          result = setCalculationMethod("Correlation");
          OS_ASSERT(result);
        }
      } else {
        resetAnnualAverageOutdoorAirTemperature();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void SiteWaterMainsTemperature_Impl::resetAnnualAverageOutdoorAirTemperature() {
      bool result = setString(OS_Site_WaterMainsTemperatureFields::AnnualAverageOutdoorAirTemperature, "");
      OS_ASSERT(result);
    }

    bool SiteWaterMainsTemperature_Impl::setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(
      boost::optional<double> maximumDifferenceInMonthlyAverageOutdoorAirTemperatures) {
      bool result(false);
      if (maximumDifferenceInMonthlyAverageOutdoorAirTemperatures) {
        result = setDouble(OS_Site_WaterMainsTemperatureFields::MaximumDifferenceInMonthlyAverageOutdoorAirTemperatures,
                           maximumDifferenceInMonthlyAverageOutdoorAirTemperatures.get());
        if (result) {
          result = setCalculationMethod("Correlation");
          OS_ASSERT(result);
        }
      } else {
        resetMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures();
        result = true;
      }
      return result;
    }

    void SiteWaterMainsTemperature_Impl::resetMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures() {
      bool result = setString(OS_Site_WaterMainsTemperatureFields::MaximumDifferenceInMonthlyAverageOutdoorAirTemperatures, "");
      OS_ASSERT(result);
    }

    std::vector<std::string> SiteWaterMainsTemperature_Impl::calculationMethodValues() const {
      return SiteWaterMainsTemperature::calculationMethodValues();
    }

    boost::optional<ModelObject> SiteWaterMainsTemperature_Impl::temperatureScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = temperatureSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool SiteWaterMainsTemperature_Impl::setTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setTemperatureSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetTemperatureSchedule();
      }
      return true;
    }

  }  // namespace detail

  SiteWaterMainsTemperature::SiteWaterMainsTemperature(const Model& model) : ModelObject(SiteWaterMainsTemperature::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SiteWaterMainsTemperature_Impl>());
    bool ok = setCalculationMethod("CorrelationFromWeatherFile");
    OS_ASSERT(ok);
  }

  IddObjectType SiteWaterMainsTemperature::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Site_WaterMainsTemperature);
    return result;
  }

  std::vector<std::string> SiteWaterMainsTemperature::calculationMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Site_WaterMainsTemperatureFields::CalculationMethod);
  }

  std::vector<std::string> SiteWaterMainsTemperature::validCalculationMethodValues() {
    return SiteWaterMainsTemperature::calculationMethodValues();
  }

  std::string SiteWaterMainsTemperature::calculationMethod() const {
    return getImpl<detail::SiteWaterMainsTemperature_Impl>()->calculationMethod();
  }

  boost::optional<Schedule> SiteWaterMainsTemperature::temperatureSchedule() const {
    return getImpl<detail::SiteWaterMainsTemperature_Impl>()->temperatureSchedule();
  }

  boost::optional<double> SiteWaterMainsTemperature::annualAverageOutdoorAirTemperature() const {
    return getImpl<detail::SiteWaterMainsTemperature_Impl>()->annualAverageOutdoorAirTemperature();
  }

  boost::optional<double> SiteWaterMainsTemperature::maximumDifferenceInMonthlyAverageOutdoorAirTemperatures() const {
    return getImpl<detail::SiteWaterMainsTemperature_Impl>()->maximumDifferenceInMonthlyAverageOutdoorAirTemperatures();
  }

  bool SiteWaterMainsTemperature::setCalculationMethod(const std::string& calculationMethod) {
    return getImpl<detail::SiteWaterMainsTemperature_Impl>()->setCalculationMethod(calculationMethod);
  }

  bool SiteWaterMainsTemperature::setTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::SiteWaterMainsTemperature_Impl>()->setTemperatureSchedule(schedule);
  }

  void SiteWaterMainsTemperature::resetTemperatureSchedule() {
    getImpl<detail::SiteWaterMainsTemperature_Impl>()->resetTemperatureSchedule();
  }

  bool SiteWaterMainsTemperature::setAnnualAverageOutdoorAirTemperature(double annualAverageOutdoorAirTemperature) {
    return getImpl<detail::SiteWaterMainsTemperature_Impl>()->setAnnualAverageOutdoorAirTemperature(annualAverageOutdoorAirTemperature);
  }

  void SiteWaterMainsTemperature::resetAnnualAverageOutdoorAirTemperature() {
    getImpl<detail::SiteWaterMainsTemperature_Impl>()->resetAnnualAverageOutdoorAirTemperature();
  }

  bool SiteWaterMainsTemperature::setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(
    double maximumDifferenceInMonthlyAverageOutdoorAirTemperatures) {
    return getImpl<detail::SiteWaterMainsTemperature_Impl>()->setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(
      maximumDifferenceInMonthlyAverageOutdoorAirTemperatures);
  }

  void SiteWaterMainsTemperature::resetMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures() {
    getImpl<detail::SiteWaterMainsTemperature_Impl>()->resetMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures();
  }

  /// @cond
  SiteWaterMainsTemperature::SiteWaterMainsTemperature(std::shared_ptr<detail::SiteWaterMainsTemperature_Impl> impl) : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
