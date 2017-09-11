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
#include "SiteGroundTemperatureDeep.hpp"
#include "SiteGroundTemperatureDeep_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"

#include <utilities/idd/OS_Site_GroundTemperature_Deep_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SiteGroundTemperatureDeep_Impl::SiteGroundTemperatureDeep_Impl(
      const IdfObject& idfObject,
      Model_Impl* model,
      bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SiteGroundTemperatureDeep::iddObjectType());
  }

  SiteGroundTemperatureDeep_Impl::SiteGroundTemperatureDeep_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SiteGroundTemperatureDeep::iddObjectType());
  }

  SiteGroundTemperatureDeep_Impl::SiteGroundTemperatureDeep_Impl(const SiteGroundTemperatureDeep_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> SiteGroundTemperatureDeep_Impl::parent() const
  {
    boost::optional<Site> result = this->model().getOptionalUniqueModelObject<Site>();
    return boost::optional<ParentObject>(result);
  }

  const std::vector<std::string>& SiteGroundTemperatureDeep_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SiteGroundTemperatureDeep_Impl::iddObjectType() const {
    return SiteGroundTemperatureDeep::iddObjectType();
  }

  double SiteGroundTemperatureDeep_Impl::januaryDeepGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::JanuaryDeepGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isJanuaryDeepGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::JanuaryDeepGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::februaryDeepGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::FebruaryDeepGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isFebruaryDeepGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::FebruaryDeepGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::marchDeepGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::MarchDeepGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isMarchDeepGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::MarchDeepGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::aprilDeepGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::AprilDeepGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isAprilDeepGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::AprilDeepGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::mayDeepGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::MayDeepGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isMayDeepGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::MayDeepGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::juneDeepGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::JuneDeepGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isJuneDeepGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::JuneDeepGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::julyDeepGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::JulyDeepGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isJulyDeepGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::JulyDeepGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::augustDeepGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::AugustDeepGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isAugustDeepGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::AugustDeepGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::septemberDeepGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::SeptemberDeepGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isSeptemberDeepGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::SeptemberDeepGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::octoberDeepGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::OctoberDeepGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isOctoberDeepGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::OctoberDeepGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::novemberDeepGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::NovemberDeepGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isNovemberDeepGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::NovemberDeepGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::decemberDeepGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::DecemberDeepGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isDecemberDeepGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::DecemberDeepGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::getTemperatureByMonth(int month) const {
    double retval;
    switch(month) {
      case 1 : retval = januaryDeepGroundTemperature(); break;
      case 2 : retval = februaryDeepGroundTemperature(); break;
      case 3 : retval = marchDeepGroundTemperature(); break;
      case 4 : retval = aprilDeepGroundTemperature(); break;
      case 5 : retval = mayDeepGroundTemperature(); break;
      case 6 : retval = juneDeepGroundTemperature(); break;
      case 7 : retval = julyDeepGroundTemperature(); break;
      case 8 : retval = augustDeepGroundTemperature(); break;
      case 9 : retval = septemberDeepGroundTemperature(); break;
      case 10 : retval = octoberDeepGroundTemperature(); break;
      case 11 : retval = novemberDeepGroundTemperature(); break;
      case 12 : retval = decemberDeepGroundTemperature(); break;
      default : LOG_AND_THROW("SiteGroundTemperature::getTemperatureByMonth: Invalid Month " + std::to_string(month)); break;
    }
    return retval;
  }

  double SiteGroundTemperatureDeep_Impl::getTemperatureByMonth(const openstudio::MonthOfYear & month) const {
    return getTemperatureByMonth(month.value());
  }

  bool SiteGroundTemperatureDeep_Impl::isMonthDefaulted(int month) const {
    bool is_defaulted;
    switch(month) {
      case 1 : is_defaulted = isJanuaryDeepGroundTemperatureDefaulted(); break;
      case 2 : is_defaulted = isFebruaryDeepGroundTemperatureDefaulted(); break;
      case 3 : is_defaulted = isMarchDeepGroundTemperatureDefaulted(); break;
      case 4 : is_defaulted = isAprilDeepGroundTemperatureDefaulted(); break;
      case 5 : is_defaulted = isMayDeepGroundTemperatureDefaulted(); break;
      case 6 : is_defaulted = isJuneDeepGroundTemperatureDefaulted(); break;
      case 7 : is_defaulted = isJulyDeepGroundTemperatureDefaulted(); break;
      case 8 : is_defaulted = isAugustDeepGroundTemperatureDefaulted(); break;
      case 9 : is_defaulted = isSeptemberDeepGroundTemperatureDefaulted(); break;
      case 10 : is_defaulted = isOctoberDeepGroundTemperatureDefaulted(); break;
      case 11 : is_defaulted = isNovemberDeepGroundTemperatureDefaulted(); break;
      case 12 : is_defaulted = isDecemberDeepGroundTemperatureDefaulted(); break;
      default : LOG_AND_THROW("SiteGroundTemperature::getTemperatureByMonth: Invalid Month " + std::to_string(month)); break;
    }
    return is_defaulted;
  }

  bool SiteGroundTemperatureDeep_Impl::isMonthDefaulted(const openstudio::MonthOfYear & month) const {
    return isMonthDefaulted(month.value());
  }

  std::vector<double> SiteGroundTemperatureDeep_Impl::getAllMonthlyTemperatures() const {
    std::vector<double> monthly_temperatures;
    monthly_temperatures.reserve(12);
    for (int i=1; i <= 12; ++i) {
      monthly_temperatures.push_back(getTemperatureByMonth(i));
    }
    return monthly_temperatures;
  }

  void SiteGroundTemperatureDeep_Impl::setJanuaryDeepGroundTemperature(double januaryDeepGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::JanuaryDeepGroundTemperature, januaryDeepGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetJanuaryDeepGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::JanuaryDeepGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setFebruaryDeepGroundTemperature(double februaryDeepGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::FebruaryDeepGroundTemperature, februaryDeepGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetFebruaryDeepGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::FebruaryDeepGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setMarchDeepGroundTemperature(double marchDeepGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::MarchDeepGroundTemperature, marchDeepGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetMarchDeepGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::MarchDeepGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setAprilDeepGroundTemperature(double aprilDeepGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::AprilDeepGroundTemperature, aprilDeepGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetAprilDeepGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::AprilDeepGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setMayDeepGroundTemperature(double mayDeepGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::MayDeepGroundTemperature, mayDeepGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetMayDeepGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::MayDeepGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setJuneDeepGroundTemperature(double juneDeepGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::JuneDeepGroundTemperature, juneDeepGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetJuneDeepGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::JuneDeepGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setJulyDeepGroundTemperature(double julyDeepGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::JulyDeepGroundTemperature, julyDeepGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetJulyDeepGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::JulyDeepGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setAugustDeepGroundTemperature(double augustDeepGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::AugustDeepGroundTemperature, augustDeepGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetAugustDeepGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::AugustDeepGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setSeptemberDeepGroundTemperature(double septemberDeepGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::SeptemberDeepGroundTemperature, septemberDeepGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetSeptemberDeepGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::SeptemberDeepGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setOctoberDeepGroundTemperature(double octoberDeepGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::OctoberDeepGroundTemperature, octoberDeepGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetOctoberDeepGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::OctoberDeepGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setNovemberDeepGroundTemperature(double novemberDeepGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::NovemberDeepGroundTemperature, novemberDeepGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetNovemberDeepGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::NovemberDeepGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setDecemberDeepGroundTemperature(double decemberDeepGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::DecemberDeepGroundTemperature, decemberDeepGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetDecemberDeepGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::DecemberDeepGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setTemperatureByMonth(int month, double temperature) {
    switch(month) {
      case 1 : setJanuaryDeepGroundTemperature(temperature); break;
      case 2 : setFebruaryDeepGroundTemperature(temperature); break;
      case 3 : setMarchDeepGroundTemperature(temperature); break;
      case 4 : setAprilDeepGroundTemperature(temperature); break;
      case 5 : setMayDeepGroundTemperature(temperature); break;
      case 6 : setJuneDeepGroundTemperature(temperature); break;
      case 7 : setJulyDeepGroundTemperature(temperature); break;
      case 8 : setAugustDeepGroundTemperature(temperature); break;
      case 9 : setSeptemberDeepGroundTemperature(temperature); break;
      case 10 : setOctoberDeepGroundTemperature(temperature); break;
      case 11 : setNovemberDeepGroundTemperature(temperature); break;
      case 12 : setDecemberDeepGroundTemperature(temperature); break;
      default : LOG_AND_THROW("SiteGroundTemperature::getTemperatureByMonth: Invalid Month " + std::to_string(month)); break;
    }
  }

  void SiteGroundTemperatureDeep_Impl::setTemperatureByMonth(const openstudio::MonthOfYear & month, double temperature) {
    return setTemperatureByMonth(month.value(), temperature);
  }

  void SiteGroundTemperatureDeep_Impl::resetTemperatureByMonth(int month) {
    switch(month) {
      case 1 : resetJanuaryDeepGroundTemperature(); break;
      case 2 : resetFebruaryDeepGroundTemperature(); break;
      case 3 : resetMarchDeepGroundTemperature(); break;
      case 4 : resetAprilDeepGroundTemperature(); break;
      case 5 : resetMayDeepGroundTemperature(); break;
      case 6 : resetJuneDeepGroundTemperature(); break;
      case 7 : resetJulyDeepGroundTemperature(); break;
      case 8 : resetAugustDeepGroundTemperature(); break;
      case 9 : resetSeptemberDeepGroundTemperature(); break;
      case 10 : resetOctoberDeepGroundTemperature(); break;
      case 11 : resetNovemberDeepGroundTemperature(); break;
      case 12 : resetDecemberDeepGroundTemperature(); break;
      default : LOG_AND_THROW("SiteGroundTemperature::getTemperatureByMonth: Invalid Month " + std::to_string(month)); break;
    }
  }

  void SiteGroundTemperatureDeep_Impl::resetTemperatureByMonth(const openstudio::MonthOfYear & month) {
    return resetTemperatureByMonth(month.value());
  }

  void SiteGroundTemperatureDeep_Impl::resetAllMonths() {
    for (int i = 1; i <= 12; ++i) {
      resetTemperatureByMonth(i);
    }
  }

  bool SiteGroundTemperatureDeep_Impl::setAllMonthlyTemperatures(const std::vector<double> &monthly_temperatures) {
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

IddObjectType SiteGroundTemperatureDeep::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Site_GroundTemperature_Deep);
  return result;
}

double SiteGroundTemperatureDeep::januaryDeepGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->januaryDeepGroundTemperature();
}

bool SiteGroundTemperatureDeep::isJanuaryDeepGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isJanuaryDeepGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::februaryDeepGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->februaryDeepGroundTemperature();
}

bool SiteGroundTemperatureDeep::isFebruaryDeepGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isFebruaryDeepGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::marchDeepGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->marchDeepGroundTemperature();
}

bool SiteGroundTemperatureDeep::isMarchDeepGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isMarchDeepGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::aprilDeepGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->aprilDeepGroundTemperature();
}

bool SiteGroundTemperatureDeep::isAprilDeepGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isAprilDeepGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::mayDeepGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->mayDeepGroundTemperature();
}

bool SiteGroundTemperatureDeep::isMayDeepGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isMayDeepGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::juneDeepGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->juneDeepGroundTemperature();
}

bool SiteGroundTemperatureDeep::isJuneDeepGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isJuneDeepGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::julyDeepGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->julyDeepGroundTemperature();
}

bool SiteGroundTemperatureDeep::isJulyDeepGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isJulyDeepGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::augustDeepGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->augustDeepGroundTemperature();
}

bool SiteGroundTemperatureDeep::isAugustDeepGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isAugustDeepGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::septemberDeepGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->septemberDeepGroundTemperature();
}

bool SiteGroundTemperatureDeep::isSeptemberDeepGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isSeptemberDeepGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::octoberDeepGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->octoberDeepGroundTemperature();
}

bool SiteGroundTemperatureDeep::isOctoberDeepGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isOctoberDeepGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::novemberDeepGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->novemberDeepGroundTemperature();
}

bool SiteGroundTemperatureDeep::isNovemberDeepGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isNovemberDeepGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::decemberDeepGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->decemberDeepGroundTemperature();
}

bool SiteGroundTemperatureDeep::isDecemberDeepGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isDecemberDeepGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::getTemperatureByMonth(int month) const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->getTemperatureByMonth(month);
}

double SiteGroundTemperatureDeep::getTemperatureByMonth(const openstudio::MonthOfYear & month) const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->getTemperatureByMonth(month);
}

bool SiteGroundTemperatureDeep::isMonthDefaulted(int month) const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isMonthDefaulted(month);
}

bool SiteGroundTemperatureDeep::isMonthDefaulted(const openstudio::MonthOfYear & month) const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isMonthDefaulted(month);
}

std::vector<double> SiteGroundTemperatureDeep::getAllMonthlyTemperatures() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->getAllMonthlyTemperatures();
}

void SiteGroundTemperatureDeep::setJanuaryDeepGroundTemperature(double januaryDeepGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setJanuaryDeepGroundTemperature(januaryDeepGroundTemperature);
}

void SiteGroundTemperatureDeep::resetJanuaryDeepGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetJanuaryDeepGroundTemperature();
}

void SiteGroundTemperatureDeep::setFebruaryDeepGroundTemperature(double februaryDeepGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setFebruaryDeepGroundTemperature(februaryDeepGroundTemperature);
}

void SiteGroundTemperatureDeep::resetFebruaryDeepGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetFebruaryDeepGroundTemperature();
}

void SiteGroundTemperatureDeep::setMarchDeepGroundTemperature(double marchDeepGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setMarchDeepGroundTemperature(marchDeepGroundTemperature);
}

void SiteGroundTemperatureDeep::resetMarchDeepGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetMarchDeepGroundTemperature();
}

void SiteGroundTemperatureDeep::setAprilDeepGroundTemperature(double aprilDeepGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setAprilDeepGroundTemperature(aprilDeepGroundTemperature);
}

void SiteGroundTemperatureDeep::resetAprilDeepGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetAprilDeepGroundTemperature();
}

void SiteGroundTemperatureDeep::setMayDeepGroundTemperature(double mayDeepGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setMayDeepGroundTemperature(mayDeepGroundTemperature);
}

void SiteGroundTemperatureDeep::resetMayDeepGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetMayDeepGroundTemperature();
}

void SiteGroundTemperatureDeep::setJuneDeepGroundTemperature(double juneDeepGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setJuneDeepGroundTemperature(juneDeepGroundTemperature);
}

void SiteGroundTemperatureDeep::resetJuneDeepGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetJuneDeepGroundTemperature();
}

void SiteGroundTemperatureDeep::setJulyDeepGroundTemperature(double julyDeepGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setJulyDeepGroundTemperature(julyDeepGroundTemperature);
}

void SiteGroundTemperatureDeep::resetJulyDeepGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetJulyDeepGroundTemperature();
}

void SiteGroundTemperatureDeep::setAugustDeepGroundTemperature(double augustDeepGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setAugustDeepGroundTemperature(augustDeepGroundTemperature);
}

void SiteGroundTemperatureDeep::resetAugustDeepGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetAugustDeepGroundTemperature();
}

void SiteGroundTemperatureDeep::setSeptemberDeepGroundTemperature(double septemberDeepGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setSeptemberDeepGroundTemperature(septemberDeepGroundTemperature);
}

void SiteGroundTemperatureDeep::resetSeptemberDeepGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetSeptemberDeepGroundTemperature();
}

void SiteGroundTemperatureDeep::setOctoberDeepGroundTemperature(double octoberDeepGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setOctoberDeepGroundTemperature(octoberDeepGroundTemperature);
}

void SiteGroundTemperatureDeep::resetOctoberDeepGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetOctoberDeepGroundTemperature();
}

void SiteGroundTemperatureDeep::setNovemberDeepGroundTemperature(double novemberDeepGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setNovemberDeepGroundTemperature(novemberDeepGroundTemperature);
}

void SiteGroundTemperatureDeep::resetNovemberDeepGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetNovemberDeepGroundTemperature();
}

void SiteGroundTemperatureDeep::setDecemberDeepGroundTemperature(double decemberDeepGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setDecemberDeepGroundTemperature(decemberDeepGroundTemperature);
}

void SiteGroundTemperatureDeep::resetDecemberDeepGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetDecemberDeepGroundTemperature();
}

void SiteGroundTemperatureDeep::setTemperatureByMonth(int month, double temperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setTemperatureByMonth(month, temperature);
}

void SiteGroundTemperatureDeep::setTemperatureByMonth(const openstudio::MonthOfYear & month, double temperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setTemperatureByMonth(month, temperature);
}

void SiteGroundTemperatureDeep::resetTemperatureByMonth(int month) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetTemperatureByMonth(month);
}

void SiteGroundTemperatureDeep::resetTemperatureByMonth(const openstudio::MonthOfYear & month) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetTemperatureByMonth(month);
}

void SiteGroundTemperatureDeep::resetAllMonths() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetAllMonths();
}

bool SiteGroundTemperatureDeep::setAllMonthlyTemperatures(const std::vector<double> &monthly_temperatures) {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setAllMonthlyTemperatures(monthly_temperatures);
}

/// @cond
SiteGroundTemperatureDeep::SiteGroundTemperatureDeep(std::shared_ptr<detail::SiteGroundTemperatureDeep_Impl> impl)
  : ModelObject(std::move(impl))
{}
SiteGroundTemperatureDeep::SiteGroundTemperatureDeep(Model& model)
  : ModelObject(SiteGroundTemperatureDeep::iddObjectType(),model)
{}

/// @endcond


} // model
} // openstudio
