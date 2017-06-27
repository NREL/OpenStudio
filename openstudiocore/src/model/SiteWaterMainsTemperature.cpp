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
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SiteWaterMainsTemperature::iddObjectType());
  }

  SiteWaterMainsTemperature_Impl::SiteWaterMainsTemperature_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SiteWaterMainsTemperature::iddObjectType());
  }

  SiteWaterMainsTemperature_Impl::SiteWaterMainsTemperature_Impl(const SiteWaterMainsTemperature_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> SiteWaterMainsTemperature_Impl::parent() const
  {
    boost::optional<Site> result = this->model().getOptionalUniqueModelObject<Site>();
    return boost::optional<ParentObject>(result);
  }

  const std::vector<std::string>& SiteWaterMainsTemperature_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SiteWaterMainsTemperature_Impl::iddObjectType() const {
    return SiteWaterMainsTemperature::iddObjectType();
  }

  std::vector<ScheduleTypeKey> SiteWaterMainsTemperature_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Site_WaterMainsTemperatureFields::TemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SiteWaterMainsTemperature","Temperature"));
    }
    return result;
  }

  std::string SiteWaterMainsTemperature_Impl::calculationMethod() const {
    boost::optional<std::string> value = getString(OS_Site_WaterMainsTemperatureFields::CalculationMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> SiteWaterMainsTemperature_Impl::temperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Site_WaterMainsTemperatureFields::TemperatureScheduleName);
  }

  boost::optional<double> SiteWaterMainsTemperature_Impl::annualAverageOutdoorAirTemperature() const {
    return getDouble(OS_Site_WaterMainsTemperatureFields::AnnualAverageOutdoorAirTemperature,true);
  }

  OSOptionalQuantity SiteWaterMainsTemperature_Impl::getAnnualAverageOutdoorAirTemperature(bool returnIP) const {
    OptionalDouble value = annualAverageOutdoorAirTemperature();
    return getQuantityFromDouble(OS_Site_WaterMainsTemperatureFields::AnnualAverageOutdoorAirTemperature, value, returnIP);
  }

  boost::optional<double> SiteWaterMainsTemperature_Impl::maximumDifferenceInMonthlyAverageOutdoorAirTemperatures() const {
    return getDouble(OS_Site_WaterMainsTemperatureFields::MaximumDifferenceInMonthlyAverageOutdoorAirTemperatures,true);
  }

  OSOptionalQuantity SiteWaterMainsTemperature_Impl::getMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(bool returnIP) const {
    OptionalDouble value = maximumDifferenceInMonthlyAverageOutdoorAirTemperatures();
    return getQuantityFromDouble(OS_Site_WaterMainsTemperatureFields::MaximumDifferenceInMonthlyAverageOutdoorAirTemperatures, value, returnIP);
  }

  bool SiteWaterMainsTemperature_Impl::setCalculationMethod(std::string calculationMethod) {
    bool result = setString(OS_Site_WaterMainsTemperatureFields::CalculationMethod, calculationMethod);
    return result;
  }

  bool SiteWaterMainsTemperature_Impl::setTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Site_WaterMainsTemperatureFields::TemperatureScheduleName,
                              "SiteWaterMainsTemperature",
                              "Temperature",
                              schedule);
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

  void SiteWaterMainsTemperature_Impl::setAnnualAverageOutdoorAirTemperature(boost::optional<double> annualAverageOutdoorAirTemperature) {
    bool result(false);
    if (annualAverageOutdoorAirTemperature) {
      result = setDouble(OS_Site_WaterMainsTemperatureFields::AnnualAverageOutdoorAirTemperature, annualAverageOutdoorAirTemperature.get());
      if (result) {
        result = setCalculationMethod("Correlation");
        OS_ASSERT(result);
      }
    }
    else {
      resetAnnualAverageOutdoorAirTemperature();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool SiteWaterMainsTemperature_Impl::setAnnualAverageOutdoorAirTemperature(const OSOptionalQuantity& annualAverageOutdoorAirTemperature) {
    bool result(false);
    OptionalDouble value;
    if (annualAverageOutdoorAirTemperature.isSet()) {
      value = getDoubleFromQuantity(OS_Site_WaterMainsTemperatureFields::AnnualAverageOutdoorAirTemperature,annualAverageOutdoorAirTemperature.get());
      if (value) {
        setAnnualAverageOutdoorAirTemperature(value);
        result = true;
      }
    }
    else {
      setAnnualAverageOutdoorAirTemperature(value);
      result = true;
    }
    return result;
  }

  void SiteWaterMainsTemperature_Impl::resetAnnualAverageOutdoorAirTemperature() {
    bool result = setString(OS_Site_WaterMainsTemperatureFields::AnnualAverageOutdoorAirTemperature, "");
    OS_ASSERT(result);
  }

  bool SiteWaterMainsTemperature_Impl::setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(boost::optional<double> maximumDifferenceInMonthlyAverageOutdoorAirTemperatures) {
    bool result(false);
    if (maximumDifferenceInMonthlyAverageOutdoorAirTemperatures) {
      result = setDouble(OS_Site_WaterMainsTemperatureFields::MaximumDifferenceInMonthlyAverageOutdoorAirTemperatures, maximumDifferenceInMonthlyAverageOutdoorAirTemperatures.get());
      if (result) {
        result = setCalculationMethod("Correlation");
        OS_ASSERT(result);
      }
    }
    else {
      resetMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures();
      result = true;
    }
    return result;
  }

  bool SiteWaterMainsTemperature_Impl::setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(const OSOptionalQuantity& maximumDifferenceInMonthlyAverageOutdoorAirTemperatures) {
    bool result(false);
    OptionalDouble value;
    if (maximumDifferenceInMonthlyAverageOutdoorAirTemperatures.isSet()) {
      value = getDoubleFromQuantity(OS_Site_WaterMainsTemperatureFields::MaximumDifferenceInMonthlyAverageOutdoorAirTemperatures,maximumDifferenceInMonthlyAverageOutdoorAirTemperatures.get());
      if (value) {
        result = setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(value);
      }
    }
    else {
      result = setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(value);
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

  openstudio::OSOptionalQuantity SiteWaterMainsTemperature_Impl::annualAverageOutdoorAirTemperature_SI() const {
    return getAnnualAverageOutdoorAirTemperature(false);
  }

  openstudio::OSOptionalQuantity SiteWaterMainsTemperature_Impl::annualAverageOutdoorAirTemperature_IP() const {
    return getAnnualAverageOutdoorAirTemperature(true);
  }

  openstudio::OSOptionalQuantity SiteWaterMainsTemperature_Impl::maximumDifferenceInMonthlyAverageOutdoorAirTemperatures_SI() const {
    return getMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(false);
  }

  openstudio::OSOptionalQuantity SiteWaterMainsTemperature_Impl::maximumDifferenceInMonthlyAverageOutdoorAirTemperatures_IP() const {
    return getMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(true);
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
      }
      else {
        return false;
      }
    }
    else {
      resetTemperatureSchedule();
    }
    return true;
  }

} // detail

SiteWaterMainsTemperature::SiteWaterMainsTemperature(const Model& model)
  : ModelObject(SiteWaterMainsTemperature::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SiteWaterMainsTemperature_Impl>());
  bool ok = setCalculationMethod("Schedule");
  OS_ASSERT(ok);
}

IddObjectType SiteWaterMainsTemperature::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Site_WaterMainsTemperature);
  return result;
}

std::vector<std::string> SiteWaterMainsTemperature::calculationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Site_WaterMainsTemperatureFields::CalculationMethod);
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

OSOptionalQuantity SiteWaterMainsTemperature::getAnnualAverageOutdoorAirTemperature(bool returnIP) const {
  return getImpl<detail::SiteWaterMainsTemperature_Impl>()->getAnnualAverageOutdoorAirTemperature(returnIP);
}

boost::optional<double> SiteWaterMainsTemperature::maximumDifferenceInMonthlyAverageOutdoorAirTemperatures() const {
  return getImpl<detail::SiteWaterMainsTemperature_Impl>()->maximumDifferenceInMonthlyAverageOutdoorAirTemperatures();
}

OSOptionalQuantity SiteWaterMainsTemperature::getMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(bool returnIP) const {
  return getImpl<detail::SiteWaterMainsTemperature_Impl>()->getMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(returnIP);
}

bool SiteWaterMainsTemperature::setCalculationMethod(std::string calculationMethod) {
  return getImpl<detail::SiteWaterMainsTemperature_Impl>()->setCalculationMethod(calculationMethod);
}

bool SiteWaterMainsTemperature::setTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::SiteWaterMainsTemperature_Impl>()->setTemperatureSchedule(schedule);
}

void SiteWaterMainsTemperature::resetTemperatureSchedule() {
  getImpl<detail::SiteWaterMainsTemperature_Impl>()->resetTemperatureSchedule();
}

void SiteWaterMainsTemperature::setAnnualAverageOutdoorAirTemperature(double annualAverageOutdoorAirTemperature) {
  getImpl<detail::SiteWaterMainsTemperature_Impl>()->setAnnualAverageOutdoorAirTemperature(annualAverageOutdoorAirTemperature);
}

bool SiteWaterMainsTemperature::setAnnualAverageOutdoorAirTemperature(const Quantity& annualAverageOutdoorAirTemperature) {
  return getImpl<detail::SiteWaterMainsTemperature_Impl>()->setAnnualAverageOutdoorAirTemperature(annualAverageOutdoorAirTemperature);
}

void SiteWaterMainsTemperature::resetAnnualAverageOutdoorAirTemperature() {
  getImpl<detail::SiteWaterMainsTemperature_Impl>()->resetAnnualAverageOutdoorAirTemperature();
}

bool SiteWaterMainsTemperature::setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(double maximumDifferenceInMonthlyAverageOutdoorAirTemperatures) {
  return getImpl<detail::SiteWaterMainsTemperature_Impl>()->setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(maximumDifferenceInMonthlyAverageOutdoorAirTemperatures);
}

bool SiteWaterMainsTemperature::setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(const Quantity& maximumDifferenceInMonthlyAverageOutdoorAirTemperatures) {
  return getImpl<detail::SiteWaterMainsTemperature_Impl>()->setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(maximumDifferenceInMonthlyAverageOutdoorAirTemperatures);
}

void SiteWaterMainsTemperature::resetMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures() {
  getImpl<detail::SiteWaterMainsTemperature_Impl>()->resetMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures();
}

/// @cond
SiteWaterMainsTemperature::SiteWaterMainsTemperature(std::shared_ptr<detail::SiteWaterMainsTemperature_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

