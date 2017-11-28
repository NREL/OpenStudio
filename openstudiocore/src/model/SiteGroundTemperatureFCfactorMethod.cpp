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

#include <vector>
#include <string>
#include "SiteGroundTemperatureFCfactorMethod.hpp"
#include "SiteGroundTemperatureFCfactorMethod_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"

#include <utilities/idd/OS_Site_GroundTemperature_FCfactorMethod_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SiteGroundTemperatureFCfactorMethod_Impl::SiteGroundTemperatureFCfactorMethod_Impl(
      const IdfObject& idfObject,
      Model_Impl* model,
      bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SiteGroundTemperatureFCfactorMethod::iddObjectType());
  }

  SiteGroundTemperatureFCfactorMethod_Impl::SiteGroundTemperatureFCfactorMethod_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SiteGroundTemperatureFCfactorMethod::iddObjectType());
  }

  SiteGroundTemperatureFCfactorMethod_Impl::SiteGroundTemperatureFCfactorMethod_Impl(const SiteGroundTemperatureFCfactorMethod_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> SiteGroundTemperatureFCfactorMethod_Impl::parent() const
  {
    boost::optional<Site> result = this->model().getOptionalUniqueModelObject<Site>();
    return boost::optional<ParentObject>(result);
  }

  const std::vector<std::string>& SiteGroundTemperatureFCfactorMethod_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SiteGroundTemperatureFCfactorMethod_Impl::iddObjectType() const {
    return SiteGroundTemperatureFCfactorMethod::iddObjectType();
  }

  double SiteGroundTemperatureFCfactorMethod_Impl::januaryGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_FCfactorMethodFields::JanuaryGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureFCfactorMethod_Impl::isJanuaryGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_FCfactorMethodFields::JanuaryGroundTemperature);
  }

  double SiteGroundTemperatureFCfactorMethod_Impl::februaryGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_FCfactorMethodFields::FebruaryGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureFCfactorMethod_Impl::isFebruaryGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_FCfactorMethodFields::FebruaryGroundTemperature);
  }

  double SiteGroundTemperatureFCfactorMethod_Impl::marchGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_FCfactorMethodFields::MarchGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureFCfactorMethod_Impl::isMarchGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_FCfactorMethodFields::MarchGroundTemperature);
  }

  double SiteGroundTemperatureFCfactorMethod_Impl::aprilGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_FCfactorMethodFields::AprilGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureFCfactorMethod_Impl::isAprilGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_FCfactorMethodFields::AprilGroundTemperature);
  }

  double SiteGroundTemperatureFCfactorMethod_Impl::mayGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_FCfactorMethodFields::MayGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureFCfactorMethod_Impl::isMayGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_FCfactorMethodFields::MayGroundTemperature);
  }

  double SiteGroundTemperatureFCfactorMethod_Impl::juneGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_FCfactorMethodFields::JuneGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureFCfactorMethod_Impl::isJuneGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_FCfactorMethodFields::JuneGroundTemperature);
  }

  double SiteGroundTemperatureFCfactorMethod_Impl::julyGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_FCfactorMethodFields::JulyGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureFCfactorMethod_Impl::isJulyGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_FCfactorMethodFields::JulyGroundTemperature);
  }

  double SiteGroundTemperatureFCfactorMethod_Impl::augustGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_FCfactorMethodFields::AugustGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureFCfactorMethod_Impl::isAugustGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_FCfactorMethodFields::AugustGroundTemperature);
  }

  double SiteGroundTemperatureFCfactorMethod_Impl::septemberGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_FCfactorMethodFields::SeptemberGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureFCfactorMethod_Impl::isSeptemberGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_FCfactorMethodFields::SeptemberGroundTemperature);
  }

  double SiteGroundTemperatureFCfactorMethod_Impl::octoberGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_FCfactorMethodFields::OctoberGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureFCfactorMethod_Impl::isOctoberGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_FCfactorMethodFields::OctoberGroundTemperature);
  }

  double SiteGroundTemperatureFCfactorMethod_Impl::novemberGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_FCfactorMethodFields::NovemberGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureFCfactorMethod_Impl::isNovemberGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_FCfactorMethodFields::NovemberGroundTemperature);
  }

  double SiteGroundTemperatureFCfactorMethod_Impl::decemberGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_FCfactorMethodFields::DecemberGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureFCfactorMethod_Impl::isDecemberGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_FCfactorMethodFields::DecemberGroundTemperature);
  }

  double SiteGroundTemperatureFCfactorMethod_Impl::getTemperatureByMonth(int month) const {
    double retval;
    switch(month) {
      case 1 : retval = januaryGroundTemperature(); break;
      case 2 : retval = februaryGroundTemperature(); break;
      case 3 : retval = marchGroundTemperature(); break;
      case 4 : retval = aprilGroundTemperature(); break;
      case 5 : retval = mayGroundTemperature(); break;
      case 6 : retval = juneGroundTemperature(); break;
      case 7 : retval = julyGroundTemperature(); break;
      case 8 : retval = augustGroundTemperature(); break;
      case 9 : retval = septemberGroundTemperature(); break;
      case 10 : retval = octoberGroundTemperature(); break;
      case 11 : retval = novemberGroundTemperature(); break;
      case 12 : retval = decemberGroundTemperature(); break;
      default : LOG_AND_THROW("SiteGroundTemperature::getTemperatureByMonth: Invalid Month " + std::to_string(month)); break;
    }
    return retval;
  }

  double SiteGroundTemperatureFCfactorMethod_Impl::getTemperatureByMonth(const openstudio::MonthOfYear & month) const {
    return getTemperatureByMonth(month.value());
  }

  bool SiteGroundTemperatureFCfactorMethod_Impl::isMonthDefaulted(int month) const {
    bool is_defaulted;
    switch(month) {
      case 1 : is_defaulted = isJanuaryGroundTemperatureDefaulted(); break;
      case 2 : is_defaulted = isFebruaryGroundTemperatureDefaulted(); break;
      case 3 : is_defaulted = isMarchGroundTemperatureDefaulted(); break;
      case 4 : is_defaulted = isAprilGroundTemperatureDefaulted(); break;
      case 5 : is_defaulted = isMayGroundTemperatureDefaulted(); break;
      case 6 : is_defaulted = isJuneGroundTemperatureDefaulted(); break;
      case 7 : is_defaulted = isJulyGroundTemperatureDefaulted(); break;
      case 8 : is_defaulted = isAugustGroundTemperatureDefaulted(); break;
      case 9 : is_defaulted = isSeptemberGroundTemperatureDefaulted(); break;
      case 10 : is_defaulted = isOctoberGroundTemperatureDefaulted(); break;
      case 11 : is_defaulted = isNovemberGroundTemperatureDefaulted(); break;
      case 12 : is_defaulted = isDecemberGroundTemperatureDefaulted(); break;
      default : LOG_AND_THROW("SiteGroundTemperature::getTemperatureByMonth: Invalid Month " + std::to_string(month)); break;
    }
    return is_defaulted;
  }

  bool SiteGroundTemperatureFCfactorMethod_Impl::isMonthDefaulted(const openstudio::MonthOfYear & month) const {
    return isMonthDefaulted(month.value());
  }

  std::vector<double> SiteGroundTemperatureFCfactorMethod_Impl::getAllMonthlyTemperatures() const {
    std::vector<double> monthly_temperatures;
    monthly_temperatures.reserve(12);
    for (int i=1; i <= 12; ++i) {
      monthly_temperatures.push_back(getTemperatureByMonth(i));
    }
    return monthly_temperatures;
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::setJanuaryGroundTemperature(double januaryGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_FCfactorMethodFields::JanuaryGroundTemperature, januaryGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::resetJanuaryGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_FCfactorMethodFields::JanuaryGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::setFebruaryGroundTemperature(double februaryGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_FCfactorMethodFields::FebruaryGroundTemperature, februaryGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::resetFebruaryGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_FCfactorMethodFields::FebruaryGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::setMarchGroundTemperature(double marchGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_FCfactorMethodFields::MarchGroundTemperature, marchGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::resetMarchGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_FCfactorMethodFields::MarchGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::setAprilGroundTemperature(double aprilGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_FCfactorMethodFields::AprilGroundTemperature, aprilGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::resetAprilGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_FCfactorMethodFields::AprilGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::setMayGroundTemperature(double mayGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_FCfactorMethodFields::MayGroundTemperature, mayGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::resetMayGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_FCfactorMethodFields::MayGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::setJuneGroundTemperature(double juneGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_FCfactorMethodFields::JuneGroundTemperature, juneGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::resetJuneGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_FCfactorMethodFields::JuneGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::setJulyGroundTemperature(double julyGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_FCfactorMethodFields::JulyGroundTemperature, julyGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::resetJulyGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_FCfactorMethodFields::JulyGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::setAugustGroundTemperature(double augustGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_FCfactorMethodFields::AugustGroundTemperature, augustGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::resetAugustGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_FCfactorMethodFields::AugustGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::setSeptemberGroundTemperature(double septemberGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_FCfactorMethodFields::SeptemberGroundTemperature, septemberGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::resetSeptemberGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_FCfactorMethodFields::SeptemberGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::setOctoberGroundTemperature(double octoberGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_FCfactorMethodFields::OctoberGroundTemperature, octoberGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::resetOctoberGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_FCfactorMethodFields::OctoberGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::setNovemberGroundTemperature(double novemberGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_FCfactorMethodFields::NovemberGroundTemperature, novemberGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::resetNovemberGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_FCfactorMethodFields::NovemberGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::setDecemberGroundTemperature(double decemberGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_FCfactorMethodFields::DecemberGroundTemperature, decemberGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::resetDecemberGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_FCfactorMethodFields::DecemberGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::setTemperatureByMonth(int month, double temperature) {
    switch(month) {
      case 1 : setJanuaryGroundTemperature(temperature); break;
      case 2 : setFebruaryGroundTemperature(temperature); break;
      case 3 : setMarchGroundTemperature(temperature); break;
      case 4 : setAprilGroundTemperature(temperature); break;
      case 5 : setMayGroundTemperature(temperature); break;
      case 6 : setJuneGroundTemperature(temperature); break;
      case 7 : setJulyGroundTemperature(temperature); break;
      case 8 : setAugustGroundTemperature(temperature); break;
      case 9 : setSeptemberGroundTemperature(temperature); break;
      case 10 : setOctoberGroundTemperature(temperature); break;
      case 11 : setNovemberGroundTemperature(temperature); break;
      case 12 : setDecemberGroundTemperature(temperature); break;
      default : LOG_AND_THROW("SiteGroundTemperature::getTemperatureByMonth: Invalid Month " + std::to_string(month)); break;
    }
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::setTemperatureByMonth(const openstudio::MonthOfYear & month, double temperature) {
    return setTemperatureByMonth(month.value(), temperature);
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::resetTemperatureByMonth(int month) {
    switch(month) {
      case 1 : resetJanuaryGroundTemperature(); break;
      case 2 : resetFebruaryGroundTemperature(); break;
      case 3 : resetMarchGroundTemperature(); break;
      case 4 : resetAprilGroundTemperature(); break;
      case 5 : resetMayGroundTemperature(); break;
      case 6 : resetJuneGroundTemperature(); break;
      case 7 : resetJulyGroundTemperature(); break;
      case 8 : resetAugustGroundTemperature(); break;
      case 9 : resetSeptemberGroundTemperature(); break;
      case 10 : resetOctoberGroundTemperature(); break;
      case 11 : resetNovemberGroundTemperature(); break;
      case 12 : resetDecemberGroundTemperature(); break;
      default : LOG_AND_THROW("SiteGroundTemperature::getTemperatureByMonth: Invalid Month " + std::to_string(month)); break;
    }
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::resetTemperatureByMonth(const openstudio::MonthOfYear & month) {
    return resetTemperatureByMonth(month.value());
  }

  void SiteGroundTemperatureFCfactorMethod_Impl::resetAllMonths() {
    for (int i = 1; i <= 12; ++i) {
      resetTemperatureByMonth(i);
    }
  }

  bool SiteGroundTemperatureFCfactorMethod_Impl::setAllMonthlyTemperatures(const std::vector<double> &monthly_temperatures) {
    if (monthly_temperatures.size() == 12) {
      for (int i = 1; i <= 12; ++i) {
        setTemperatureByMonth(i, monthly_temperatures[i-1]);
      }
      return true;
    } else {
      return false;
    }
  }

} // detail

IddObjectType SiteGroundTemperatureFCfactorMethod::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Site_GroundTemperature_FCfactorMethod);
  return result;
}

double SiteGroundTemperatureFCfactorMethod::januaryGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->januaryGroundTemperature();
}

bool SiteGroundTemperatureFCfactorMethod::isJanuaryGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->isJanuaryGroundTemperatureDefaulted();
}

double SiteGroundTemperatureFCfactorMethod::februaryGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->februaryGroundTemperature();
}

bool SiteGroundTemperatureFCfactorMethod::isFebruaryGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->isFebruaryGroundTemperatureDefaulted();
}

double SiteGroundTemperatureFCfactorMethod::marchGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->marchGroundTemperature();
}

bool SiteGroundTemperatureFCfactorMethod::isMarchGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->isMarchGroundTemperatureDefaulted();
}

double SiteGroundTemperatureFCfactorMethod::aprilGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->aprilGroundTemperature();
}

bool SiteGroundTemperatureFCfactorMethod::isAprilGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->isAprilGroundTemperatureDefaulted();
}

double SiteGroundTemperatureFCfactorMethod::mayGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->mayGroundTemperature();
}

bool SiteGroundTemperatureFCfactorMethod::isMayGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->isMayGroundTemperatureDefaulted();
}

double SiteGroundTemperatureFCfactorMethod::juneGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->juneGroundTemperature();
}

bool SiteGroundTemperatureFCfactorMethod::isJuneGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->isJuneGroundTemperatureDefaulted();
}

double SiteGroundTemperatureFCfactorMethod::julyGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->julyGroundTemperature();
}

bool SiteGroundTemperatureFCfactorMethod::isJulyGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->isJulyGroundTemperatureDefaulted();
}

double SiteGroundTemperatureFCfactorMethod::augustGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->augustGroundTemperature();
}

bool SiteGroundTemperatureFCfactorMethod::isAugustGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->isAugustGroundTemperatureDefaulted();
}

double SiteGroundTemperatureFCfactorMethod::septemberGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->septemberGroundTemperature();
}

bool SiteGroundTemperatureFCfactorMethod::isSeptemberGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->isSeptemberGroundTemperatureDefaulted();
}

double SiteGroundTemperatureFCfactorMethod::octoberGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->octoberGroundTemperature();
}

bool SiteGroundTemperatureFCfactorMethod::isOctoberGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->isOctoberGroundTemperatureDefaulted();
}

double SiteGroundTemperatureFCfactorMethod::novemberGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->novemberGroundTemperature();
}

bool SiteGroundTemperatureFCfactorMethod::isNovemberGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->isNovemberGroundTemperatureDefaulted();
}

double SiteGroundTemperatureFCfactorMethod::decemberGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->decemberGroundTemperature();
}

bool SiteGroundTemperatureFCfactorMethod::isDecemberGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->isDecemberGroundTemperatureDefaulted();
}

double SiteGroundTemperatureFCfactorMethod::getTemperatureByMonth(int month) const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->getTemperatureByMonth(month);
}

double SiteGroundTemperatureFCfactorMethod::getTemperatureByMonth(const openstudio::MonthOfYear & month) const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->getTemperatureByMonth(month);
}

bool SiteGroundTemperatureFCfactorMethod::isMonthDefaulted(int month) const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->isMonthDefaulted(month);
}

bool SiteGroundTemperatureFCfactorMethod::isMonthDefaulted(const openstudio::MonthOfYear & month) const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->isMonthDefaulted(month);
}

std::vector<double> SiteGroundTemperatureFCfactorMethod::getAllMonthlyTemperatures() const {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->getAllMonthlyTemperatures();
}

void SiteGroundTemperatureFCfactorMethod::setJanuaryGroundTemperature(double januaryGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->setJanuaryGroundTemperature(januaryGroundTemperature);
}

void SiteGroundTemperatureFCfactorMethod::resetJanuaryGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->resetJanuaryGroundTemperature();
}

void SiteGroundTemperatureFCfactorMethod::setFebruaryGroundTemperature(double februaryGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->setFebruaryGroundTemperature(februaryGroundTemperature);
}

void SiteGroundTemperatureFCfactorMethod::resetFebruaryGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->resetFebruaryGroundTemperature();
}

void SiteGroundTemperatureFCfactorMethod::setMarchGroundTemperature(double marchGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->setMarchGroundTemperature(marchGroundTemperature);
}

void SiteGroundTemperatureFCfactorMethod::resetMarchGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->resetMarchGroundTemperature();
}

void SiteGroundTemperatureFCfactorMethod::setAprilGroundTemperature(double aprilGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->setAprilGroundTemperature(aprilGroundTemperature);
}

void SiteGroundTemperatureFCfactorMethod::resetAprilGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->resetAprilGroundTemperature();
}

void SiteGroundTemperatureFCfactorMethod::setMayGroundTemperature(double mayGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->setMayGroundTemperature(mayGroundTemperature);
}

void SiteGroundTemperatureFCfactorMethod::resetMayGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->resetMayGroundTemperature();
}

void SiteGroundTemperatureFCfactorMethod::setJuneGroundTemperature(double juneGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->setJuneGroundTemperature(juneGroundTemperature);
}

void SiteGroundTemperatureFCfactorMethod::resetJuneGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->resetJuneGroundTemperature();
}

void SiteGroundTemperatureFCfactorMethod::setJulyGroundTemperature(double julyGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->setJulyGroundTemperature(julyGroundTemperature);
}

void SiteGroundTemperatureFCfactorMethod::resetJulyGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->resetJulyGroundTemperature();
}

void SiteGroundTemperatureFCfactorMethod::setAugustGroundTemperature(double augustGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->setAugustGroundTemperature(augustGroundTemperature);
}

void SiteGroundTemperatureFCfactorMethod::resetAugustGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->resetAugustGroundTemperature();
}

void SiteGroundTemperatureFCfactorMethod::setSeptemberGroundTemperature(double septemberGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->setSeptemberGroundTemperature(septemberGroundTemperature);
}

void SiteGroundTemperatureFCfactorMethod::resetSeptemberGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->resetSeptemberGroundTemperature();
}

void SiteGroundTemperatureFCfactorMethod::setOctoberGroundTemperature(double octoberGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->setOctoberGroundTemperature(octoberGroundTemperature);
}

void SiteGroundTemperatureFCfactorMethod::resetOctoberGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->resetOctoberGroundTemperature();
}

void SiteGroundTemperatureFCfactorMethod::setNovemberGroundTemperature(double novemberGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->setNovemberGroundTemperature(novemberGroundTemperature);
}

void SiteGroundTemperatureFCfactorMethod::resetNovemberGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->resetNovemberGroundTemperature();
}

void SiteGroundTemperatureFCfactorMethod::setDecemberGroundTemperature(double decemberGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->setDecemberGroundTemperature(decemberGroundTemperature);
}

void SiteGroundTemperatureFCfactorMethod::resetDecemberGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->resetDecemberGroundTemperature();
}

void SiteGroundTemperatureFCfactorMethod::setTemperatureByMonth(int month, double temperature) {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->setTemperatureByMonth(month, temperature);
}

void SiteGroundTemperatureFCfactorMethod::setTemperatureByMonth(const openstudio::MonthOfYear & month, double temperature) {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->setTemperatureByMonth(month, temperature);
}

void SiteGroundTemperatureFCfactorMethod::resetTemperatureByMonth(int month) {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->resetTemperatureByMonth(month);
}

void SiteGroundTemperatureFCfactorMethod::resetTemperatureByMonth(const openstudio::MonthOfYear & month) {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->resetTemperatureByMonth(month);
}

void SiteGroundTemperatureFCfactorMethod::resetAllMonths() {
  getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->resetAllMonths();
}

bool SiteGroundTemperatureFCfactorMethod::setAllMonthlyTemperatures(const std::vector<double> &monthly_temperatures) {
  return getImpl<detail::SiteGroundTemperatureFCfactorMethod_Impl>()->setAllMonthlyTemperatures(monthly_temperatures);
}

/// @cond
SiteGroundTemperatureFCfactorMethod::SiteGroundTemperatureFCfactorMethod(std::shared_ptr<detail::SiteGroundTemperatureFCfactorMethod_Impl> impl)
  : ModelObject(std::move(impl))
{}
SiteGroundTemperatureFCfactorMethod::SiteGroundTemperatureFCfactorMethod(Model& model)
  : ModelObject(SiteGroundTemperatureFCfactorMethod::iddObjectType(),model)
{}

/// @endcond


} // model
} // openstudio
