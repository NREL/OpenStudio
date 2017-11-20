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

#include "SurfacePropertyOtherSideCoefficients.hpp"
#include "SurfacePropertyOtherSideCoefficients_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SurfaceProperty_OtherSideCoefficients_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SurfacePropertyOtherSideCoefficients_Impl::SurfacePropertyOtherSideCoefficients_Impl(const IdfObject& idfObject,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SurfacePropertyOtherSideCoefficients::iddObjectType());
  }

  SurfacePropertyOtherSideCoefficients_Impl::SurfacePropertyOtherSideCoefficients_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SurfacePropertyOtherSideCoefficients::iddObjectType());
  }

  SurfacePropertyOtherSideCoefficients_Impl::SurfacePropertyOtherSideCoefficients_Impl(const SurfacePropertyOtherSideCoefficients_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SurfacePropertyOtherSideCoefficients_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SurfacePropertyOtherSideCoefficients_Impl::iddObjectType() const {
    return SurfacePropertyOtherSideCoefficients::iddObjectType();
  }

  std::vector<ScheduleTypeKey> SurfacePropertyOtherSideCoefficients_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_SurfaceProperty_OtherSideCoefficientsFields::ConstantTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SurfacePropertyOtherSideCoefficients","Constant Temperature"));
    }
    return result;
  }

  boost::optional<double> SurfacePropertyOtherSideCoefficients_Impl::combinedConvectiveRadiativeFilmCoefficient() const {
    return getDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::CombinedConvective_RadiativeFilmCoefficient,true);
  }

  double SurfacePropertyOtherSideCoefficients_Impl::constantTemperature() const {
    boost::optional<double> value = getDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::ConstantTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::isConstantTemperatureDefaulted() const {
    return isEmpty(OS_SurfaceProperty_OtherSideCoefficientsFields::ConstantTemperature);
  }

  double SurfacePropertyOtherSideCoefficients_Impl::constantTemperatureCoefficient() const {
    boost::optional<double> value = getDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::ConstantTemperatureCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::isConstantTemperatureCoefficientDefaulted() const {
    return isEmpty(OS_SurfaceProperty_OtherSideCoefficientsFields::ConstantTemperatureCoefficient);
  }

  double SurfacePropertyOtherSideCoefficients_Impl::externalDryBulbTemperatureCoefficient() const {
    boost::optional<double> value = getDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::ExternalDryBulbTemperatureCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::isExternalDryBulbTemperatureCoefficientDefaulted() const {
    return isEmpty(OS_SurfaceProperty_OtherSideCoefficientsFields::ExternalDryBulbTemperatureCoefficient);
  }

  double SurfacePropertyOtherSideCoefficients_Impl::groundTemperatureCoefficient() const {
    boost::optional<double> value = getDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::GroundTemperatureCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::isGroundTemperatureCoefficientDefaulted() const {
    return isEmpty(OS_SurfaceProperty_OtherSideCoefficientsFields::GroundTemperatureCoefficient);
  }

  double SurfacePropertyOtherSideCoefficients_Impl::windSpeedCoefficient() const {
    boost::optional<double> value = getDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::WindSpeedCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::isWindSpeedCoefficientDefaulted() const {
    return isEmpty(OS_SurfaceProperty_OtherSideCoefficientsFields::WindSpeedCoefficient);
  }

  double SurfacePropertyOtherSideCoefficients_Impl::zoneAirTemperatureCoefficient() const {
    boost::optional<double> value = getDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::ZoneAirTemperatureCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::isZoneAirTemperatureCoefficientDefaulted() const {
    return isEmpty(OS_SurfaceProperty_OtherSideCoefficientsFields::ZoneAirTemperatureCoefficient);
  }

  boost::optional<Schedule> SurfacePropertyOtherSideCoefficients_Impl::constantTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SurfaceProperty_OtherSideCoefficientsFields::ConstantTemperatureScheduleName);
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::sinusoidalVariationofConstantTemperatureCoefficient() const {
    boost::optional<std::string> value = getString(OS_SurfaceProperty_OtherSideCoefficientsFields::SinusoidalVariationofConstantTemperatureCoefficient,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::isSinusoidalVariationofConstantTemperatureCoefficientDefaulted() const {
    return isEmpty(OS_SurfaceProperty_OtherSideCoefficientsFields::SinusoidalVariationofConstantTemperatureCoefficient);
  }

  double SurfacePropertyOtherSideCoefficients_Impl::periodofSinusoidalVariation() const {
    boost::optional<double> value = getDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::PeriodofSinusoidalVariation,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::isPeriodofSinusoidalVariationDefaulted() const {
    return isEmpty(OS_SurfaceProperty_OtherSideCoefficientsFields::PeriodofSinusoidalVariation);
  }

  double SurfacePropertyOtherSideCoefficients_Impl::previousOtherSideTemperatureCoefficient() const {
    boost::optional<double> value = getDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::PreviousOtherSideTemperatureCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::isPreviousOtherSideTemperatureCoefficientDefaulted() const {
    return isEmpty(OS_SurfaceProperty_OtherSideCoefficientsFields::PreviousOtherSideTemperatureCoefficient);
  }

  boost::optional<double> SurfacePropertyOtherSideCoefficients_Impl::minimumOtherSideTemperatureLimit() const {
    return getDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::MinimumOtherSideTemperatureLimit,true);
  }

  boost::optional<double> SurfacePropertyOtherSideCoefficients_Impl::maximumOtherSideTemperatureLimit() const {
    return getDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::MaximumOtherSideTemperatureLimit,true);
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::setCombinedConvectiveRadiativeFilmCoefficient(double combinedConvectiveRadiativeFilmCoefficient) {
    bool result = setDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::CombinedConvective_RadiativeFilmCoefficient, combinedConvectiveRadiativeFilmCoefficient);
    return result;
  }

  void SurfacePropertyOtherSideCoefficients_Impl::resetCombinedConvectiveRadiativeFilmCoefficient() {
    bool result = setString(OS_SurfaceProperty_OtherSideCoefficientsFields::CombinedConvective_RadiativeFilmCoefficient, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::setConstantTemperature(double constantTemperature) {
    bool result = setDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::ConstantTemperature, constantTemperature);
    return result;
  }

  void SurfacePropertyOtherSideCoefficients_Impl::resetConstantTemperature() {
    bool result = setString(OS_SurfaceProperty_OtherSideCoefficientsFields::ConstantTemperature, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::setConstantTemperatureCoefficient(double constantTemperatureCoefficient) {
    bool result = setDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::ConstantTemperatureCoefficient, constantTemperatureCoefficient);
    return result;
  }

  void SurfacePropertyOtherSideCoefficients_Impl::resetConstantTemperatureCoefficient() {
    bool result = setString(OS_SurfaceProperty_OtherSideCoefficientsFields::ConstantTemperatureCoefficient, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::setExternalDryBulbTemperatureCoefficient(double externalDryBulbTemperatureCoefficient) {
    bool result = setDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::ExternalDryBulbTemperatureCoefficient, externalDryBulbTemperatureCoefficient);
    return result;
  }

  void SurfacePropertyOtherSideCoefficients_Impl::resetExternalDryBulbTemperatureCoefficient() {
    bool result = setString(OS_SurfaceProperty_OtherSideCoefficientsFields::ExternalDryBulbTemperatureCoefficient, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::setGroundTemperatureCoefficient(double groundTemperatureCoefficient) {
    bool result = setDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::GroundTemperatureCoefficient, groundTemperatureCoefficient);
    return result;
  }

  void SurfacePropertyOtherSideCoefficients_Impl::resetGroundTemperatureCoefficient() {
    bool result = setString(OS_SurfaceProperty_OtherSideCoefficientsFields::GroundTemperatureCoefficient, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::setWindSpeedCoefficient(double windSpeedCoefficient) {
    bool result = setDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::WindSpeedCoefficient, windSpeedCoefficient);
    return result;
  }

  void SurfacePropertyOtherSideCoefficients_Impl::resetWindSpeedCoefficient() {
    bool result = setString(OS_SurfaceProperty_OtherSideCoefficientsFields::WindSpeedCoefficient, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::setZoneAirTemperatureCoefficient(double zoneAirTemperatureCoefficient) {
    bool result = setDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::ZoneAirTemperatureCoefficient, zoneAirTemperatureCoefficient);
    return result;
  }

  void SurfacePropertyOtherSideCoefficients_Impl::resetZoneAirTemperatureCoefficient() {
    bool result = setString(OS_SurfaceProperty_OtherSideCoefficientsFields::ZoneAirTemperatureCoefficient, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::setConstantTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_SurfaceProperty_OtherSideCoefficientsFields::ConstantTemperatureScheduleName,
                              "SurfacePropertyOtherSideCoefficients",
                              "Constant Temperature",
                              schedule);
    return result;
  }

  void SurfacePropertyOtherSideCoefficients_Impl::resetConstantTemperatureSchedule() {
    bool result = setString(OS_SurfaceProperty_OtherSideCoefficientsFields::ConstantTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::setSinusoidalVariationofConstantTemperatureCoefficient(bool sinusoidalVariationofConstantTemperatureCoefficient) {
    bool result = false;
    if (sinusoidalVariationofConstantTemperatureCoefficient) {
      result = setString(OS_SurfaceProperty_OtherSideCoefficientsFields::SinusoidalVariationofConstantTemperatureCoefficient, "Yes");
    } else {
      result = setString(OS_SurfaceProperty_OtherSideCoefficientsFields::SinusoidalVariationofConstantTemperatureCoefficient, "No");
    }
    return result;
  }

  void SurfacePropertyOtherSideCoefficients_Impl::resetSinusoidalVariationofConstantTemperatureCoefficient() {
    bool result = setString(OS_SurfaceProperty_OtherSideCoefficientsFields::SinusoidalVariationofConstantTemperatureCoefficient, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::setPeriodofSinusoidalVariation(double periodofSinusoidalVariation) {
    bool result = setDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::PeriodofSinusoidalVariation, periodofSinusoidalVariation);
    return result;
  }

  void SurfacePropertyOtherSideCoefficients_Impl::resetPeriodofSinusoidalVariation() {
    bool result = setString(OS_SurfaceProperty_OtherSideCoefficientsFields::PeriodofSinusoidalVariation, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::setPreviousOtherSideTemperatureCoefficient(double previousOtherSideTemperatureCoefficient) {
    bool result = setDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::PreviousOtherSideTemperatureCoefficient, previousOtherSideTemperatureCoefficient);
    return result;
  }

  void SurfacePropertyOtherSideCoefficients_Impl::resetPreviousOtherSideTemperatureCoefficient() {
    bool result = setString(OS_SurfaceProperty_OtherSideCoefficientsFields::PreviousOtherSideTemperatureCoefficient, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::setMinimumOtherSideTemperatureLimit(double minimumOtherSideTemperatureLimit) {
    bool result = setDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::MinimumOtherSideTemperatureLimit, minimumOtherSideTemperatureLimit);
    return result;
  }

  void SurfacePropertyOtherSideCoefficients_Impl::resetMinimumOtherSideTemperatureLimit() {
    bool result = setString(OS_SurfaceProperty_OtherSideCoefficientsFields::MinimumOtherSideTemperatureLimit, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyOtherSideCoefficients_Impl::setMaximumOtherSideTemperatureLimit(double maximumOtherSideTemperatureLimit) {
    bool result = setDouble(OS_SurfaceProperty_OtherSideCoefficientsFields::MaximumOtherSideTemperatureLimit, maximumOtherSideTemperatureLimit);
    return result;
  }

  void SurfacePropertyOtherSideCoefficients_Impl::resetMaximumOtherSideTemperatureLimit() {
    bool result = setString(OS_SurfaceProperty_OtherSideCoefficientsFields::MaximumOtherSideTemperatureLimit, "");
    OS_ASSERT(result);
  }

} // detail

SurfacePropertyOtherSideCoefficients::SurfacePropertyOtherSideCoefficients(const Model& model)
  : ResourceObject(SurfacePropertyOtherSideCoefficients::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>());
}

IddObjectType SurfacePropertyOtherSideCoefficients::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SurfaceProperty_OtherSideCoefficients);
}

boost::optional<double> SurfacePropertyOtherSideCoefficients::combinedConvectiveRadiativeFilmCoefficient() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->combinedConvectiveRadiativeFilmCoefficient();
}

double SurfacePropertyOtherSideCoefficients::constantTemperature() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->constantTemperature();
}

bool SurfacePropertyOtherSideCoefficients::isConstantTemperatureDefaulted() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->isConstantTemperatureDefaulted();
}

double SurfacePropertyOtherSideCoefficients::constantTemperatureCoefficient() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->constantTemperatureCoefficient();
}

bool SurfacePropertyOtherSideCoefficients::isConstantTemperatureCoefficientDefaulted() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->isConstantTemperatureCoefficientDefaulted();
}

double SurfacePropertyOtherSideCoefficients::externalDryBulbTemperatureCoefficient() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->externalDryBulbTemperatureCoefficient();
}

bool SurfacePropertyOtherSideCoefficients::isExternalDryBulbTemperatureCoefficientDefaulted() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->isExternalDryBulbTemperatureCoefficientDefaulted();
}

double SurfacePropertyOtherSideCoefficients::groundTemperatureCoefficient() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->groundTemperatureCoefficient();
}

bool SurfacePropertyOtherSideCoefficients::isGroundTemperatureCoefficientDefaulted() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->isGroundTemperatureCoefficientDefaulted();
}

double SurfacePropertyOtherSideCoefficients::windSpeedCoefficient() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->windSpeedCoefficient();
}

bool SurfacePropertyOtherSideCoefficients::isWindSpeedCoefficientDefaulted() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->isWindSpeedCoefficientDefaulted();
}

double SurfacePropertyOtherSideCoefficients::zoneAirTemperatureCoefficient() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->zoneAirTemperatureCoefficient();
}

bool SurfacePropertyOtherSideCoefficients::isZoneAirTemperatureCoefficientDefaulted() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->isZoneAirTemperatureCoefficientDefaulted();
}

boost::optional<Schedule> SurfacePropertyOtherSideCoefficients::constantTemperatureSchedule() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->constantTemperatureSchedule();
}

bool SurfacePropertyOtherSideCoefficients::sinusoidalVariationofConstantTemperatureCoefficient() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->sinusoidalVariationofConstantTemperatureCoefficient();
}

bool SurfacePropertyOtherSideCoefficients::isSinusoidalVariationofConstantTemperatureCoefficientDefaulted() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->isSinusoidalVariationofConstantTemperatureCoefficientDefaulted();
}

double SurfacePropertyOtherSideCoefficients::periodofSinusoidalVariation() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->periodofSinusoidalVariation();
}

bool SurfacePropertyOtherSideCoefficients::isPeriodofSinusoidalVariationDefaulted() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->isPeriodofSinusoidalVariationDefaulted();
}

double SurfacePropertyOtherSideCoefficients::previousOtherSideTemperatureCoefficient() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->previousOtherSideTemperatureCoefficient();
}

bool SurfacePropertyOtherSideCoefficients::isPreviousOtherSideTemperatureCoefficientDefaulted() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->isPreviousOtherSideTemperatureCoefficientDefaulted();
}

boost::optional<double> SurfacePropertyOtherSideCoefficients::minimumOtherSideTemperatureLimit() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->minimumOtherSideTemperatureLimit();
}

boost::optional<double> SurfacePropertyOtherSideCoefficients::maximumOtherSideTemperatureLimit() const {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->maximumOtherSideTemperatureLimit();
}

bool SurfacePropertyOtherSideCoefficients::setCombinedConvectiveRadiativeFilmCoefficient(double combinedConvectiveRadiativeFilmCoefficient) {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->setCombinedConvectiveRadiativeFilmCoefficient(combinedConvectiveRadiativeFilmCoefficient);
}

void SurfacePropertyOtherSideCoefficients::resetCombinedConvectiveRadiativeFilmCoefficient() {
  getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->resetCombinedConvectiveRadiativeFilmCoefficient();
}

bool SurfacePropertyOtherSideCoefficients::setConstantTemperature(double constantTemperature) {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->setConstantTemperature(constantTemperature);
}

void SurfacePropertyOtherSideCoefficients::resetConstantTemperature() {
  getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->resetConstantTemperature();
}

bool SurfacePropertyOtherSideCoefficients::setConstantTemperatureCoefficient(double constantTemperatureCoefficient) {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->setConstantTemperatureCoefficient(constantTemperatureCoefficient);
}

void SurfacePropertyOtherSideCoefficients::resetConstantTemperatureCoefficient() {
  getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->resetConstantTemperatureCoefficient();
}

bool SurfacePropertyOtherSideCoefficients::setExternalDryBulbTemperatureCoefficient(double externalDryBulbTemperatureCoefficient) {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->setExternalDryBulbTemperatureCoefficient(externalDryBulbTemperatureCoefficient);
}

void SurfacePropertyOtherSideCoefficients::resetExternalDryBulbTemperatureCoefficient() {
  getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->resetExternalDryBulbTemperatureCoefficient();
}

bool SurfacePropertyOtherSideCoefficients::setGroundTemperatureCoefficient(double groundTemperatureCoefficient) {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->setGroundTemperatureCoefficient(groundTemperatureCoefficient);
}

void SurfacePropertyOtherSideCoefficients::resetGroundTemperatureCoefficient() {
  getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->resetGroundTemperatureCoefficient();
}

bool SurfacePropertyOtherSideCoefficients::setWindSpeedCoefficient(double windSpeedCoefficient) {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->setWindSpeedCoefficient(windSpeedCoefficient);
}

void SurfacePropertyOtherSideCoefficients::resetWindSpeedCoefficient() {
  getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->resetWindSpeedCoefficient();
}

bool SurfacePropertyOtherSideCoefficients::setZoneAirTemperatureCoefficient(double zoneAirTemperatureCoefficient) {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->setZoneAirTemperatureCoefficient(zoneAirTemperatureCoefficient);
}

void SurfacePropertyOtherSideCoefficients::resetZoneAirTemperatureCoefficient() {
  getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->resetZoneAirTemperatureCoefficient();
}

bool SurfacePropertyOtherSideCoefficients::setConstantTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->setConstantTemperatureSchedule(schedule);
}

void SurfacePropertyOtherSideCoefficients::resetConstantTemperatureSchedule() {
  getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->resetConstantTemperatureSchedule();
}

bool SurfacePropertyOtherSideCoefficients::setSinusoidalVariationofConstantTemperatureCoefficient(bool sinusoidalVariationofConstantTemperatureCoefficient) {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->setSinusoidalVariationofConstantTemperatureCoefficient(sinusoidalVariationofConstantTemperatureCoefficient);
}

void SurfacePropertyOtherSideCoefficients::resetSinusoidalVariationofConstantTemperatureCoefficient() {
  getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->resetSinusoidalVariationofConstantTemperatureCoefficient();
}

bool SurfacePropertyOtherSideCoefficients::setPeriodofSinusoidalVariation(double periodofSinusoidalVariation) {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->setPeriodofSinusoidalVariation(periodofSinusoidalVariation);
}

void SurfacePropertyOtherSideCoefficients::resetPeriodofSinusoidalVariation() {
  getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->resetPeriodofSinusoidalVariation();
}

bool SurfacePropertyOtherSideCoefficients::setPreviousOtherSideTemperatureCoefficient(double previousOtherSideTemperatureCoefficient) {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->setPreviousOtherSideTemperatureCoefficient(previousOtherSideTemperatureCoefficient);
}

void SurfacePropertyOtherSideCoefficients::resetPreviousOtherSideTemperatureCoefficient() {
  getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->resetPreviousOtherSideTemperatureCoefficient();
}

bool SurfacePropertyOtherSideCoefficients::setMinimumOtherSideTemperatureLimit(double minimumOtherSideTemperatureLimit) {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->setMinimumOtherSideTemperatureLimit(minimumOtherSideTemperatureLimit);
}

void SurfacePropertyOtherSideCoefficients::resetMinimumOtherSideTemperatureLimit() {
  getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->resetMinimumOtherSideTemperatureLimit();
}

bool SurfacePropertyOtherSideCoefficients::setMaximumOtherSideTemperatureLimit(double maximumOtherSideTemperatureLimit) {
  return getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->setMaximumOtherSideTemperatureLimit(maximumOtherSideTemperatureLimit);
}

void SurfacePropertyOtherSideCoefficients::resetMaximumOtherSideTemperatureLimit() {
  getImpl<detail::SurfacePropertyOtherSideCoefficients_Impl>()->resetMaximumOtherSideTemperatureLimit();
}

/// @cond
SurfacePropertyOtherSideCoefficients::SurfacePropertyOtherSideCoefficients(std::shared_ptr<detail::SurfacePropertyOtherSideCoefficients_Impl> impl)
  : ResourceObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

