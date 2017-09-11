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

#include "RadianceParameters.hpp"
#include "RadianceParameters_Impl.hpp"

#include <utilities/idd/OS_RadianceParameters_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RadianceParameters_Impl::RadianceParameters_Impl(const IdfObject& idfObject,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RadianceParameters::iddObjectType());
  }

  RadianceParameters_Impl::RadianceParameters_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RadianceParameters::iddObjectType());
  }

  RadianceParameters_Impl::RadianceParameters_Impl(const RadianceParameters_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  RadianceParameters_Impl::~RadianceParameters_Impl() {
    // Empty implementation for debugging
  }

  const std::vector<std::string>& RadianceParameters_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RadianceParameters_Impl::iddObjectType() const {
    return RadianceParameters::iddObjectType();
  }

  int RadianceParameters_Impl::accumulatedRaysperRecord() const {
    boost::optional<int> value = getInt(OS_RadianceParametersFields::AccumulatedRaysperRecord,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RadianceParameters_Impl::isAccumulatedRaysperRecordDefaulted() const {
    return isEmpty(OS_RadianceParametersFields::AccumulatedRaysperRecord);
  }

  double RadianceParameters_Impl::directThreshold() const {
    boost::optional<double> value = getDouble(OS_RadianceParametersFields::DirectThreshold,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RadianceParameters_Impl::getDirectThreshold(bool returnIP) const {
    OptionalDouble value = directThreshold();
    OSOptionalQuantity result = getQuantityFromDouble(OS_RadianceParametersFields::DirectThreshold, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RadianceParameters_Impl::isDirectThresholdDefaulted() const {
    return isEmpty(OS_RadianceParametersFields::DirectThreshold);
  }

  double RadianceParameters_Impl::directCertainty() const {
    boost::optional<double> value = getDouble(OS_RadianceParametersFields::DirectCertainty,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RadianceParameters_Impl::getDirectCertainty(bool returnIP) const {
    OptionalDouble value = directCertainty();
    OSOptionalQuantity result = getQuantityFromDouble(OS_RadianceParametersFields::DirectCertainty, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RadianceParameters_Impl::isDirectCertaintyDefaulted() const {
    return isEmpty(OS_RadianceParametersFields::DirectCertainty);
  }

  double RadianceParameters_Impl::directJitter() const {
    boost::optional<double> value = getDouble(OS_RadianceParametersFields::DirectJitter,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RadianceParameters_Impl::getDirectJitter(bool returnIP) const {
    OptionalDouble value = directJitter();
    OSOptionalQuantity result = getQuantityFromDouble(OS_RadianceParametersFields::DirectJitter, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RadianceParameters_Impl::isDirectJitterDefaulted() const {
    return isEmpty(OS_RadianceParametersFields::DirectJitter);
  }

  double RadianceParameters_Impl::directPretest() const {
    boost::optional<double> value = getDouble(OS_RadianceParametersFields::DirectPretest,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RadianceParameters_Impl::getDirectPretest(bool returnIP) const {
    OptionalDouble value = directPretest();
    OSOptionalQuantity result = getQuantityFromDouble(OS_RadianceParametersFields::DirectPretest, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RadianceParameters_Impl::isDirectPretestDefaulted() const {
    return isEmpty(OS_RadianceParametersFields::DirectPretest);
  }

  int RadianceParameters_Impl::ambientBouncesVMX() const {
    boost::optional<int> value = getInt(OS_RadianceParametersFields::AmbientBouncesVMX,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RadianceParameters_Impl::isAmbientBouncesVMXDefaulted() const {
    return isEmpty(OS_RadianceParametersFields::AmbientBouncesVMX);
  }

  int RadianceParameters_Impl::ambientBouncesDMX() const {
    boost::optional<int> value = getInt(OS_RadianceParametersFields::AmbientBouncesDMX,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RadianceParameters_Impl::isAmbientBouncesDMXDefaulted() const {
    return isEmpty(OS_RadianceParametersFields::AmbientBouncesDMX);
  }

  int RadianceParameters_Impl::ambientDivisionsVMX() const {
    boost::optional<int> value = getInt(OS_RadianceParametersFields::AmbientDivisionsVMX,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RadianceParameters_Impl::isAmbientDivisionsVMXDefaulted() const {
    return isEmpty(OS_RadianceParametersFields::AmbientDivisionsVMX);
  }

  int RadianceParameters_Impl::ambientDivisionsDMX() const {
    boost::optional<int> value = getInt(OS_RadianceParametersFields::AmbientDivisionsDMX,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RadianceParameters_Impl::isAmbientDivisionsDMXDefaulted() const {
    return isEmpty(OS_RadianceParametersFields::AmbientDivisionsDMX);
  }

  int RadianceParameters_Impl::ambientSupersamples() const {
    boost::optional<int> value = getInt(OS_RadianceParametersFields::AmbientSupersamples,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RadianceParameters_Impl::isAmbientSupersamplesDefaulted() const {
    return isEmpty(OS_RadianceParametersFields::AmbientSupersamples);
  }

  double RadianceParameters_Impl::limitWeightVMX() const {
    boost::optional<double> value = getDouble(OS_RadianceParametersFields::LimitWeightVMX,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RadianceParameters_Impl::getLimitWeightVMX(bool returnIP) const {
    OptionalDouble value = limitWeightVMX();
    OSOptionalQuantity result = getQuantityFromDouble(OS_RadianceParametersFields::LimitWeightVMX, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RadianceParameters_Impl::isLimitWeightVMXDefaulted() const {
    return isEmpty(OS_RadianceParametersFields::LimitWeightVMX);
  }

  double RadianceParameters_Impl::limitWeightDMX() const {
    boost::optional<double> value = getDouble(OS_RadianceParametersFields::LimitWeightDMX,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RadianceParameters_Impl::getLimitWeightDMX(bool returnIP) const {
    OptionalDouble value = limitWeightDMX();
    OSOptionalQuantity result = getQuantityFromDouble(OS_RadianceParametersFields::LimitWeightDMX, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RadianceParameters_Impl::isLimitWeightDMXDefaulted() const {
    return isEmpty(OS_RadianceParametersFields::LimitWeightDMX);
  }

  int RadianceParameters_Impl::klemsSamplingDensity() const {
    boost::optional<int> value = getInt(OS_RadianceParametersFields::KlemsSamplingDensity,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RadianceParameters_Impl::isKlemsSamplingDensityDefaulted() const {
    return isEmpty(OS_RadianceParametersFields::KlemsSamplingDensity);
  }

  std::string RadianceParameters_Impl::skyDiscretizationResolution() const {
    boost::optional<std::string> value = getString(OS_RadianceParametersFields::SkyDiscretizationResolution,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RadianceParameters_Impl::setSkyDiscretizationResolution(std::string skyDiscretizationResolution) {
    bool result = setString(OS_RadianceParametersFields::SkyDiscretizationResolution, skyDiscretizationResolution);
    return result;
  }

  void RadianceParameters_Impl::resetSkyDiscretizationResolution() {
    bool result = setString(OS_RadianceParametersFields::SkyDiscretizationResolution, "");
    OS_ASSERT(result);
  }

  std::vector<std::string> RadianceParameters_Impl::skyDiscretizationResolutionValues() const {
    return RadianceParameters::skyDiscretizationResolutionValues();
  }

  bool RadianceParameters_Impl::setAccumulatedRaysperRecord(int accumulatedRaysperRecord) {
    bool result = setInt(OS_RadianceParametersFields::AccumulatedRaysperRecord, accumulatedRaysperRecord);
    return result;
  }

  void RadianceParameters_Impl::resetAccumulatedRaysperRecord() {
    bool result = setString(OS_RadianceParametersFields::AccumulatedRaysperRecord, "");
    OS_ASSERT(result);
  }

  bool RadianceParameters_Impl::setDirectThreshold(double directThreshold) {
    bool result = setDouble(OS_RadianceParametersFields::DirectThreshold, directThreshold);
    return result;
  }

  bool RadianceParameters_Impl::setDirectThreshold(const Quantity& directThreshold) {
    OptionalDouble value = getDoubleFromQuantity(OS_RadianceParametersFields::DirectThreshold,directThreshold);
    if (!value) {
      return false;
    }
    return setDirectThreshold(value.get());
  }

  void RadianceParameters_Impl::resetDirectThreshold() {
    bool result = setString(OS_RadianceParametersFields::DirectThreshold, "");
    OS_ASSERT(result);
  }

  bool RadianceParameters_Impl::setDirectCertainty(double directCertainty) {
    bool result = setDouble(OS_RadianceParametersFields::DirectCertainty, directCertainty);
    return result;
  }

  bool RadianceParameters_Impl::setDirectCertainty(const Quantity& directCertainty) {
    OptionalDouble value = getDoubleFromQuantity(OS_RadianceParametersFields::DirectCertainty,directCertainty);
    if (!value) {
      return false;
    }
    return setDirectCertainty(value.get());
  }

  void RadianceParameters_Impl::resetDirectCertainty() {
    bool result = setString(OS_RadianceParametersFields::DirectCertainty, "");
    OS_ASSERT(result);
  }

  bool RadianceParameters_Impl::setDirectJitter(double directJitter) {
    bool result = setDouble(OS_RadianceParametersFields::DirectJitter, directJitter);
    return result;
  }

  bool RadianceParameters_Impl::setDirectJitter(const Quantity& directJitter) {
    OptionalDouble value = getDoubleFromQuantity(OS_RadianceParametersFields::DirectJitter,directJitter);
    if (!value) {
      return false;
    }
    return setDirectJitter(value.get());
  }

  void RadianceParameters_Impl::resetDirectJitter() {
    bool result = setString(OS_RadianceParametersFields::DirectJitter, "");
    OS_ASSERT(result);
  }

  bool RadianceParameters_Impl::setDirectPretest(double directPretest) {
    bool result = setDouble(OS_RadianceParametersFields::DirectPretest, directPretest);
    return result;
  }

  bool RadianceParameters_Impl::setDirectPretest(const Quantity& directPretest) {
    OptionalDouble value = getDoubleFromQuantity(OS_RadianceParametersFields::DirectPretest,directPretest);
    if (!value) {
      return false;
    }
    return setDirectPretest(value.get());
  }

  void RadianceParameters_Impl::resetDirectPretest() {
    bool result = setString(OS_RadianceParametersFields::DirectPretest, "");
    OS_ASSERT(result);
  }

  bool RadianceParameters_Impl::setAmbientBouncesVMX(int ambientBouncesVMX) {
    bool result = setInt(OS_RadianceParametersFields::AmbientBouncesVMX, ambientBouncesVMX);
    return result;
  }

  void RadianceParameters_Impl::resetAmbientBouncesVMX() {
    bool result = setString(OS_RadianceParametersFields::AmbientBouncesVMX, "");
    OS_ASSERT(result);
  }

  bool RadianceParameters_Impl::setAmbientBouncesDMX(int ambientBouncesDMX) {
    bool result = setInt(OS_RadianceParametersFields::AmbientBouncesDMX, ambientBouncesDMX);
    return result;
  }

  void RadianceParameters_Impl::resetAmbientBouncesDMX() {
    bool result = setString(OS_RadianceParametersFields::AmbientBouncesDMX, "");
    OS_ASSERT(result);
  }

  bool RadianceParameters_Impl::setAmbientDivisionsVMX(int ambientDivisionsVMX) {
    bool result = setInt(OS_RadianceParametersFields::AmbientDivisionsVMX, ambientDivisionsVMX);
    return result;
  }


  void RadianceParameters_Impl::resetAmbientDivisionsVMX() {
    bool result = setString(OS_RadianceParametersFields::AmbientDivisionsVMX, "");
    OS_ASSERT(result);
  }

  bool RadianceParameters_Impl::setAmbientDivisionsDMX(int ambientDivisionsDMX) {
    bool result = setInt(OS_RadianceParametersFields::AmbientDivisionsDMX, ambientDivisionsDMX);
    return result;
  }

  void RadianceParameters_Impl::resetAmbientDivisionsDMX() {
    bool result = setString(OS_RadianceParametersFields::AmbientDivisionsDMX, "");
    OS_ASSERT(result);
  }

  bool RadianceParameters_Impl::setAmbientSupersamples(int ambientSupersamples) {
    bool result = setInt(OS_RadianceParametersFields::AmbientSupersamples, ambientSupersamples);
    return result;
  }

  void RadianceParameters_Impl::resetAmbientSupersamples() {
    bool result = setString(OS_RadianceParametersFields::AmbientSupersamples, "");
    OS_ASSERT(result);
  }

  bool RadianceParameters_Impl::setLimitWeightVMX(double LimitWeightVMX) {
    bool result = setDouble(OS_RadianceParametersFields::LimitWeightVMX, LimitWeightVMX);
    return result;
  }

  bool RadianceParameters_Impl::setLimitWeightVMX(const Quantity& LimitWeightVMX) {
    OptionalDouble value = getDoubleFromQuantity(OS_RadianceParametersFields::LimitWeightVMX,LimitWeightVMX);
    if (!value) {
      return false;
    }
    return setLimitWeightVMX(value.get());
  }

  void RadianceParameters_Impl::resetLimitWeightVMX() {
    bool result = setString(OS_RadianceParametersFields::LimitWeightVMX, "");
    OS_ASSERT(result);
  }

  void RadianceParameters_Impl::setLimitWeightDMX(double LimitWeightDMX) {
    bool result = setDouble(OS_RadianceParametersFields::LimitWeightDMX, LimitWeightDMX);
    OS_ASSERT(result);
  }

  bool RadianceParameters_Impl::setLimitWeightDMX(const Quantity& LimitWeightDMX) {
    OptionalDouble value = getDoubleFromQuantity(OS_RadianceParametersFields::LimitWeightDMX,LimitWeightDMX);
    if (!value) {
      return false;
    }
    setLimitWeightDMX(value.get());
    return true;
  }

  void RadianceParameters_Impl::resetLimitWeightDMX() {
    bool result = setString(OS_RadianceParametersFields::LimitWeightDMX, "");
    OS_ASSERT(result);
  }

  bool RadianceParameters_Impl::setKlemsSamplingDensity(int klemsSamplingDensity) {
    bool result = setInt(OS_RadianceParametersFields::KlemsSamplingDensity, klemsSamplingDensity);
    return result;
  }

  void RadianceParameters_Impl::resetKlemsSamplingDensity() {
    bool result = setString(OS_RadianceParametersFields::KlemsSamplingDensity, "");
    OS_ASSERT(result);
  }

  openstudio::Quantity RadianceParameters_Impl::directThreshold_SI() const {
    return getDirectThreshold(false);
  }

  openstudio::Quantity RadianceParameters_Impl::directThreshold_IP() const {
    return getDirectThreshold(true);
  }

  openstudio::Quantity RadianceParameters_Impl::directCertainty_SI() const {
    return getDirectCertainty(false);
  }

  openstudio::Quantity RadianceParameters_Impl::directCertainty_IP() const {
    return getDirectCertainty(true);
  }

  openstudio::Quantity RadianceParameters_Impl::directJitter_SI() const {
    return getDirectJitter(false);
  }

  openstudio::Quantity RadianceParameters_Impl::directJitter_IP() const {
    return getDirectJitter(true);
  }

  openstudio::Quantity RadianceParameters_Impl::directPretest_SI() const {
    return getDirectPretest(false);
  }

  openstudio::Quantity RadianceParameters_Impl::directPretest_IP() const {
    return getDirectPretest(true);
  }

  openstudio::Quantity RadianceParameters_Impl::limitWeightVMX_SI() const {
    return getLimitWeightVMX(false);
  }

  openstudio::Quantity RadianceParameters_Impl::limitWeightVMX_IP() const {
    return getLimitWeightVMX(true);
  }

  openstudio::Quantity RadianceParameters_Impl::limitWeightDMX_SI() const {
    return getLimitWeightDMX(false);
  }

  openstudio::Quantity RadianceParameters_Impl::limitWeightDMX_IP() const {
    return getLimitWeightDMX(true);
  }

  void RadianceParameters_Impl::applyCoarseSettings()
  {
    setAccumulatedRaysperRecord(1);
    setDirectThreshold(0.0);
    setDirectCertainty(1.0);
    setDirectJitter(1.0);
    setDirectPretest(1.0);
    setAmbientBouncesVMX(6);
    setAmbientBouncesDMX(2);
    setAmbientDivisionsVMX(4050);
    setAmbientDivisionsDMX(512);
    setAmbientSupersamples(256);
    setLimitWeightVMX(0.001);
    setLimitWeightDMX(0.001);
    setKlemsSamplingDensity(500);
    setSkyDiscretizationResolution("146");
  }

  bool RadianceParameters_Impl::isCoarseSettings()
  {
    bool result = false;

    if (accumulatedRaysperRecord() == 1 &&
      directThreshold() == 0.0 &&
      directCertainty() == 1.0 &&
      directJitter() == 1.0 &&
      directPretest() == 1.0 &&
      ambientBouncesVMX() == 6 &&
      ambientBouncesDMX() == 2 &&
      ambientDivisionsVMX() == 4050 &&
      ambientDivisionsDMX() == 512 &&
      ambientSupersamples() == 256 &&
      limitWeightVMX() == 0.001 &&
      limitWeightDMX() == 0.001 &&
      klemsSamplingDensity() == 500 &&
      skyDiscretizationResolution() == "146")
    {
      result = true;
    }

    return result;
  }

  void RadianceParameters_Impl::applyFineSettings()
  {
    setAccumulatedRaysperRecord(1);
    setDirectThreshold(0.0);
    setDirectCertainty(1.0);
    setDirectJitter(1.0);
    setDirectPretest(1.0);
    setAmbientBouncesVMX(10);
    setAmbientBouncesDMX(3);
    setAmbientDivisionsVMX(65536);
    setAmbientDivisionsDMX(1024);
    setAmbientSupersamples(512);
    setLimitWeightVMX(0.0000152);
    setLimitWeightDMX(0.0001);
    setKlemsSamplingDensity(1000);
    setSkyDiscretizationResolution("2306");
  }

  bool RadianceParameters_Impl::isFineSettings()
  {
    bool result = false;

    if (accumulatedRaysperRecord() == 1 &&
      directThreshold() == 0.0 &&
      directCertainty() == 1.0 &&
      directJitter() == 1.0 &&
      directPretest() == 1.0 &&
      ambientBouncesVMX() == 10 &&
      ambientBouncesDMX() == 3 &&
      ambientDivisionsVMX() == 65536 &&
      ambientDivisionsDMX() == 1024 &&
      ambientSupersamples() == 512 &&
      limitWeightVMX() == 0.0000152 &&
      limitWeightDMX() == 0.0001 &&
      klemsSamplingDensity() == 1000 &&
      skyDiscretizationResolution() == "2306")
    {
      result = true;
    }

    return result;
  }

} // detail

IddObjectType RadianceParameters::iddObjectType() {
  return IddObjectType(IddObjectType::OS_RadianceParameters);
}

int RadianceParameters::accumulatedRaysperRecord() const {
  return getImpl<detail::RadianceParameters_Impl>()->accumulatedRaysperRecord();
}

bool RadianceParameters::isAccumulatedRaysperRecordDefaulted() const {
  return getImpl<detail::RadianceParameters_Impl>()->isAccumulatedRaysperRecordDefaulted();
}

double RadianceParameters::directThreshold() const {
  return getImpl<detail::RadianceParameters_Impl>()->directThreshold();
}

Quantity RadianceParameters::getDirectThreshold(bool returnIP) const {
  return getImpl<detail::RadianceParameters_Impl>()->getDirectThreshold(returnIP);
}

bool RadianceParameters::isDirectThresholdDefaulted() const {
  return getImpl<detail::RadianceParameters_Impl>()->isDirectThresholdDefaulted();
}

double RadianceParameters::directCertainty() const {
  return getImpl<detail::RadianceParameters_Impl>()->directCertainty();
}

Quantity RadianceParameters::getDirectCertainty(bool returnIP) const {
  return getImpl<detail::RadianceParameters_Impl>()->getDirectCertainty(returnIP);
}

bool RadianceParameters::isDirectCertaintyDefaulted() const {
  return getImpl<detail::RadianceParameters_Impl>()->isDirectCertaintyDefaulted();
}

double RadianceParameters::directJitter() const {
  return getImpl<detail::RadianceParameters_Impl>()->directJitter();
}

Quantity RadianceParameters::getDirectJitter(bool returnIP) const {
  return getImpl<detail::RadianceParameters_Impl>()->getDirectJitter(returnIP);
}

bool RadianceParameters::isDirectJitterDefaulted() const {
  return getImpl<detail::RadianceParameters_Impl>()->isDirectJitterDefaulted();
}

double RadianceParameters::directPretest() const {
  return getImpl<detail::RadianceParameters_Impl>()->directPretest();
}

Quantity RadianceParameters::getDirectPretest(bool returnIP) const {
  return getImpl<detail::RadianceParameters_Impl>()->getDirectPretest(returnIP);
}

bool RadianceParameters::isDirectPretestDefaulted() const {
  return getImpl<detail::RadianceParameters_Impl>()->isDirectPretestDefaulted();
}

int RadianceParameters::ambientBouncesVMX() const {
  return getImpl<detail::RadianceParameters_Impl>()->ambientBouncesVMX();
}

bool RadianceParameters::isAmbientBouncesVMXDefaulted() const {
  return getImpl<detail::RadianceParameters_Impl>()->isAmbientBouncesVMXDefaulted();
}

int RadianceParameters::ambientBouncesDMX() const {
  return getImpl<detail::RadianceParameters_Impl>()->ambientBouncesDMX();
}

bool RadianceParameters::isAmbientBouncesDMXDefaulted() const {
  return getImpl<detail::RadianceParameters_Impl>()->isAmbientBouncesDMXDefaulted();
}

int RadianceParameters::ambientDivisionsVMX() const {
  return getImpl<detail::RadianceParameters_Impl>()->ambientDivisionsVMX();
}

bool RadianceParameters::isAmbientDivisionsVMXDefaulted() const {
  return getImpl<detail::RadianceParameters_Impl>()->isAmbientDivisionsVMXDefaulted();
}

int RadianceParameters::ambientDivisionsDMX() const {
  return getImpl<detail::RadianceParameters_Impl>()->ambientDivisionsDMX();
}

bool RadianceParameters::isAmbientDivisionsDMXDefaulted() const {
  return getImpl<detail::RadianceParameters_Impl>()->isAmbientDivisionsDMXDefaulted();
}

int RadianceParameters::ambientSupersamples() const {
  return getImpl<detail::RadianceParameters_Impl>()->ambientSupersamples();
}

bool RadianceParameters::isAmbientSupersamplesDefaulted() const {
  return getImpl<detail::RadianceParameters_Impl>()->isAmbientSupersamplesDefaulted();
}

double RadianceParameters::limitWeightVMX() const {
  return getImpl<detail::RadianceParameters_Impl>()->limitWeightVMX();
}

Quantity RadianceParameters::getLimitWeightVMX(bool returnIP) const {
  return getImpl<detail::RadianceParameters_Impl>()->getLimitWeightVMX(returnIP);
}

bool RadianceParameters::isLimitWeightVMXDefaulted() const {
  return getImpl<detail::RadianceParameters_Impl>()->isLimitWeightVMXDefaulted();
}

double RadianceParameters::limitWeightDMX() const {
  return getImpl<detail::RadianceParameters_Impl>()->limitWeightDMX();
}

Quantity RadianceParameters::getLimitWeightDMX(bool returnIP) const {
  return getImpl<detail::RadianceParameters_Impl>()->getLimitWeightDMX(returnIP);
}

bool RadianceParameters::isLimitWeightDMXDefaulted() const {
  return getImpl<detail::RadianceParameters_Impl>()->isLimitWeightDMXDefaulted();
}

int RadianceParameters::klemsSamplingDensity() const {
  return getImpl<detail::RadianceParameters_Impl>()->klemsSamplingDensity();
}

bool RadianceParameters::isKlemsSamplingDensityDefaulted() const {
  return getImpl<detail::RadianceParameters_Impl>()->isKlemsSamplingDensityDefaulted();
}

std::vector<std::string> RadianceParameters::skyDiscretizationResolutionValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_RadianceParametersFields::SkyDiscretizationResolution);
}

std::string RadianceParameters::skyDiscretizationResolution() const {
  auto impl = getImpl<detail::RadianceParameters_Impl>();
  return impl->skyDiscretizationResolution();
}

bool RadianceParameters::setSkyDiscretizationResolution(std::string skyDiscretizationResolution) {
  auto impl = getImpl<detail::RadianceParameters_Impl>();
  return impl->setSkyDiscretizationResolution(skyDiscretizationResolution);
}

void RadianceParameters::resetSkyDiscretizationResolution() {
  getImpl<detail::RadianceParameters_Impl>()->resetSkyDiscretizationResolution();
}

bool RadianceParameters::setAccumulatedRaysperRecord(int accumulatedRaysperRecord) {
  return getImpl<detail::RadianceParameters_Impl>()->setAccumulatedRaysperRecord(accumulatedRaysperRecord);
}

void RadianceParameters::resetAccumulatedRaysperRecord() {
  getImpl<detail::RadianceParameters_Impl>()->resetAccumulatedRaysperRecord();
}

bool RadianceParameters::setDirectThreshold(double directThreshold) {
  return getImpl<detail::RadianceParameters_Impl>()->setDirectThreshold(directThreshold);
}

bool RadianceParameters::setDirectThreshold(const Quantity& directThreshold) {
  return getImpl<detail::RadianceParameters_Impl>()->setDirectThreshold(directThreshold);
}

void RadianceParameters::resetDirectThreshold() {
  getImpl<detail::RadianceParameters_Impl>()->resetDirectThreshold();
}

bool RadianceParameters::setDirectCertainty(double directCertainty) {
  return getImpl<detail::RadianceParameters_Impl>()->setDirectCertainty(directCertainty);
}

bool RadianceParameters::setDirectCertainty(const Quantity& directCertainty) {
  return getImpl<detail::RadianceParameters_Impl>()->setDirectCertainty(directCertainty);
}

void RadianceParameters::resetDirectCertainty() {
  getImpl<detail::RadianceParameters_Impl>()->resetDirectCertainty();
}

bool RadianceParameters::setDirectJitter(double directJitter) {
  return getImpl<detail::RadianceParameters_Impl>()->setDirectJitter(directJitter);
}

bool RadianceParameters::setDirectJitter(const Quantity& directJitter) {
  return getImpl<detail::RadianceParameters_Impl>()->setDirectJitter(directJitter);
}

void RadianceParameters::resetDirectJitter() {
  getImpl<detail::RadianceParameters_Impl>()->resetDirectJitter();
}

bool RadianceParameters::setDirectPretest(double directPretest) {
  return getImpl<detail::RadianceParameters_Impl>()->setDirectPretest(directPretest);
}

bool RadianceParameters::setDirectPretest(const Quantity& directPretest) {
  return getImpl<detail::RadianceParameters_Impl>()->setDirectPretest(directPretest);
}

void RadianceParameters::resetDirectPretest() {
  getImpl<detail::RadianceParameters_Impl>()->resetDirectPretest();
}

bool RadianceParameters::setAmbientBouncesVMX(int ambientBouncesVMX) {
  return getImpl<detail::RadianceParameters_Impl>()->setAmbientBouncesVMX(ambientBouncesVMX);
}

void RadianceParameters::resetAmbientBouncesVMX() {
  getImpl<detail::RadianceParameters_Impl>()->resetAmbientBouncesVMX();
}

bool RadianceParameters::setAmbientBouncesDMX(int ambientBouncesDMX) {
  return getImpl<detail::RadianceParameters_Impl>()->setAmbientBouncesDMX(ambientBouncesDMX);
}

void RadianceParameters::resetAmbientBouncesDMX() {
  getImpl<detail::RadianceParameters_Impl>()->resetAmbientBouncesDMX();
}

bool RadianceParameters::setAmbientDivisionsVMX(int ambientDivisionsVMX) {
  return getImpl<detail::RadianceParameters_Impl>()->setAmbientDivisionsVMX(ambientDivisionsVMX);
}

void RadianceParameters::resetAmbientDivisionsVMX() {
  getImpl<detail::RadianceParameters_Impl>()->resetAmbientDivisionsVMX();
}

bool RadianceParameters::setAmbientDivisionsDMX(int ambientDivisionsDMX) {
  return getImpl<detail::RadianceParameters_Impl>()->setAmbientDivisionsDMX(ambientDivisionsDMX);
}

void RadianceParameters::resetAmbientDivisionsDMX() {
  getImpl<detail::RadianceParameters_Impl>()->resetAmbientDivisionsDMX();
}

bool RadianceParameters::setAmbientSupersamples(int ambientSupersamples) {
  return getImpl<detail::RadianceParameters_Impl>()->setAmbientSupersamples(ambientSupersamples);
}

void RadianceParameters::resetAmbientSupersamples() {
  getImpl<detail::RadianceParameters_Impl>()->resetAmbientSupersamples();
}

bool RadianceParameters::setLimitWeightVMX(double LimitWeightVMX) {
  return getImpl<detail::RadianceParameters_Impl>()->setLimitWeightVMX(LimitWeightVMX);
}

bool RadianceParameters::setLimitWeightVMX(const Quantity& LimitWeightVMX) {
  return getImpl<detail::RadianceParameters_Impl>()->setLimitWeightVMX(LimitWeightVMX);
}

void RadianceParameters::resetLimitWeightVMX() {
  getImpl<detail::RadianceParameters_Impl>()->resetLimitWeightVMX();
}

void RadianceParameters::setLimitWeightDMX(double LimitWeightDMX) {
  getImpl<detail::RadianceParameters_Impl>()->setLimitWeightDMX(LimitWeightDMX);
}

bool RadianceParameters::setLimitWeightDMX(const Quantity& LimitWeightDMX) {
  return getImpl<detail::RadianceParameters_Impl>()->setLimitWeightDMX(LimitWeightDMX);
}

void RadianceParameters::resetLimitWeightDMX() {
  getImpl<detail::RadianceParameters_Impl>()->resetLimitWeightDMX();
}

bool RadianceParameters::setKlemsSamplingDensity(int klemsSamplingDensity) {
  return getImpl<detail::RadianceParameters_Impl>()->setKlemsSamplingDensity(klemsSamplingDensity);
}

void RadianceParameters::resetKlemsSamplingDensity() {
  getImpl<detail::RadianceParameters_Impl>()->resetKlemsSamplingDensity();
}

void RadianceParameters::applyCoarseSettings()
{
  getImpl<detail::RadianceParameters_Impl>()->applyCoarseSettings();
}

bool RadianceParameters::isCoarseSettings()
{
  return getImpl<detail::RadianceParameters_Impl>()->isCoarseSettings();
}

void RadianceParameters::applyFineSettings()
{
  getImpl<detail::RadianceParameters_Impl>()->applyFineSettings();
}

bool RadianceParameters::isFineSettings()
{
  return getImpl<detail::RadianceParameters_Impl>()->isFineSettings();
}

/// @cond
RadianceParameters::RadianceParameters(std::shared_ptr<detail::RadianceParameters_Impl> impl)
  : ModelObject(std::move(impl))
{
}

RadianceParameters::RadianceParameters(Model& model)
  : ModelObject(RadianceParameters::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RadianceParameters_Impl>());

  applyCoarseSettings();
}

/// @endcond

} // model
} // openstudio

