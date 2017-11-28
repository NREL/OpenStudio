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

#include "SiteGroundReflectance.hpp"
#include "SiteGroundReflectance_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"

#include <utilities/idd/OS_Site_GroundReflectance_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SiteGroundReflectance_Impl::SiteGroundReflectance_Impl(const IdfObject& idfObject,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SiteGroundReflectance::iddObjectType());
  }

  SiteGroundReflectance_Impl::SiteGroundReflectance_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SiteGroundReflectance::iddObjectType());
  }

  SiteGroundReflectance_Impl::SiteGroundReflectance_Impl(const SiteGroundReflectance_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> SiteGroundReflectance_Impl::parent() const
  {
    boost::optional<Site> result = this->model().getOptionalUniqueModelObject<Site>();
    return boost::optional<ParentObject>(result);
  }

  const std::vector<std::string>& SiteGroundReflectance_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SiteGroundReflectance_Impl::iddObjectType() const {
    return SiteGroundReflectance::iddObjectType();
  }

  double SiteGroundReflectance_Impl::januaryGroundReflectance() const {
    boost::optional<double> value = getDouble(OS_Site_GroundReflectanceFields::JanuaryGroundReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundReflectance_Impl::isJanuaryGroundReflectanceDefaulted() const {
    return isEmpty(OS_Site_GroundReflectanceFields::JanuaryGroundReflectance);
  }

  double SiteGroundReflectance_Impl::februaryGroundReflectance() const {
    boost::optional<double> value = getDouble(OS_Site_GroundReflectanceFields::FebruaryGroundReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundReflectance_Impl::isFebruaryGroundReflectanceDefaulted() const {
    return isEmpty(OS_Site_GroundReflectanceFields::FebruaryGroundReflectance);
  }

  double SiteGroundReflectance_Impl::marchGroundReflectance() const {
    boost::optional<double> value = getDouble(OS_Site_GroundReflectanceFields::MarchGroundReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundReflectance_Impl::isMarchGroundReflectanceDefaulted() const {
    return isEmpty(OS_Site_GroundReflectanceFields::MarchGroundReflectance);
  }

  double SiteGroundReflectance_Impl::aprilGroundReflectance() const {
    boost::optional<double> value = getDouble(OS_Site_GroundReflectanceFields::AprilGroundReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundReflectance_Impl::isAprilGroundReflectanceDefaulted() const {
    return isEmpty(OS_Site_GroundReflectanceFields::AprilGroundReflectance);
  }

  double SiteGroundReflectance_Impl::mayGroundReflectance() const {
    boost::optional<double> value = getDouble(OS_Site_GroundReflectanceFields::MayGroundReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundReflectance_Impl::isMayGroundReflectanceDefaulted() const {
    return isEmpty(OS_Site_GroundReflectanceFields::MayGroundReflectance);
  }

  double SiteGroundReflectance_Impl::juneGroundReflectance() const {
    boost::optional<double> value = getDouble(OS_Site_GroundReflectanceFields::JuneGroundReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundReflectance_Impl::isJuneGroundReflectanceDefaulted() const {
    return isEmpty(OS_Site_GroundReflectanceFields::JuneGroundReflectance);
  }

  double SiteGroundReflectance_Impl::julyGroundReflectance() const {
    boost::optional<double> value = getDouble(OS_Site_GroundReflectanceFields::JulyGroundReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundReflectance_Impl::isJulyGroundReflectanceDefaulted() const {
    return isEmpty(OS_Site_GroundReflectanceFields::JulyGroundReflectance);
  }

  double SiteGroundReflectance_Impl::augustGroundReflectance() const {
    boost::optional<double> value = getDouble(OS_Site_GroundReflectanceFields::AugustGroundReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundReflectance_Impl::isAugustGroundReflectanceDefaulted() const {
    return isEmpty(OS_Site_GroundReflectanceFields::AugustGroundReflectance);
  }

  double SiteGroundReflectance_Impl::septemberGroundReflectance() const {
    boost::optional<double> value = getDouble(OS_Site_GroundReflectanceFields::SeptemberGroundReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundReflectance_Impl::isSeptemberGroundReflectanceDefaulted() const {
    return isEmpty(OS_Site_GroundReflectanceFields::SeptemberGroundReflectance);
  }

  double SiteGroundReflectance_Impl::octoberGroundReflectance() const {
    boost::optional<double> value = getDouble(OS_Site_GroundReflectanceFields::OctoberGroundReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundReflectance_Impl::isOctoberGroundReflectanceDefaulted() const {
    return isEmpty(OS_Site_GroundReflectanceFields::OctoberGroundReflectance);
  }

  double SiteGroundReflectance_Impl::novemberGroundReflectance() const {
    boost::optional<double> value = getDouble(OS_Site_GroundReflectanceFields::NovemberGroundReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundReflectance_Impl::isNovemberGroundReflectanceDefaulted() const {
    return isEmpty(OS_Site_GroundReflectanceFields::NovemberGroundReflectance);
  }

  double SiteGroundReflectance_Impl::decemberGroundReflectance() const {
    boost::optional<double> value = getDouble(OS_Site_GroundReflectanceFields::DecemberGroundReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundReflectance_Impl::isDecemberGroundReflectanceDefaulted() const {
    return isEmpty(OS_Site_GroundReflectanceFields::DecemberGroundReflectance);
  }

  bool SiteGroundReflectance_Impl::setJanuaryGroundReflectance(double januaryGroundReflectance) {
    bool result = setDouble(OS_Site_GroundReflectanceFields::JanuaryGroundReflectance, januaryGroundReflectance);
    return result;
  }

  void SiteGroundReflectance_Impl::resetJanuaryGroundReflectance() {
    bool result = setString(OS_Site_GroundReflectanceFields::JanuaryGroundReflectance, "");
    OS_ASSERT(result);
  }

  bool SiteGroundReflectance_Impl::setFebruaryGroundReflectance(double februaryGroundReflectance) {
    bool result = setDouble(OS_Site_GroundReflectanceFields::FebruaryGroundReflectance, februaryGroundReflectance);
    return result;
  }

  void SiteGroundReflectance_Impl::resetFebruaryGroundReflectance() {
    bool result = setString(OS_Site_GroundReflectanceFields::FebruaryGroundReflectance, "");
    OS_ASSERT(result);
  }

  bool SiteGroundReflectance_Impl::setMarchGroundReflectance(double marchGroundReflectance) {
    bool result = setDouble(OS_Site_GroundReflectanceFields::MarchGroundReflectance, marchGroundReflectance);
    return result;
  }

  void SiteGroundReflectance_Impl::resetMarchGroundReflectance() {
    bool result = setString(OS_Site_GroundReflectanceFields::MarchGroundReflectance, "");
    OS_ASSERT(result);
  }

  bool SiteGroundReflectance_Impl::setAprilGroundReflectance(double aprilGroundReflectance) {
    bool result = setDouble(OS_Site_GroundReflectanceFields::AprilGroundReflectance, aprilGroundReflectance);
    return result;
  }

  void SiteGroundReflectance_Impl::resetAprilGroundReflectance() {
    bool result = setString(OS_Site_GroundReflectanceFields::AprilGroundReflectance, "");
    OS_ASSERT(result);
  }

  bool SiteGroundReflectance_Impl::setMayGroundReflectance(double mayGroundReflectance) {
    bool result = setDouble(OS_Site_GroundReflectanceFields::MayGroundReflectance, mayGroundReflectance);
    return result;
  }

  void SiteGroundReflectance_Impl::resetMayGroundReflectance() {
    bool result = setString(OS_Site_GroundReflectanceFields::MayGroundReflectance, "");
    OS_ASSERT(result);
  }

  bool SiteGroundReflectance_Impl::setJuneGroundReflectance(double juneGroundReflectance) {
    bool result = setDouble(OS_Site_GroundReflectanceFields::JuneGroundReflectance, juneGroundReflectance);
    return result;
  }

  void SiteGroundReflectance_Impl::resetJuneGroundReflectance() {
    bool result = setString(OS_Site_GroundReflectanceFields::JuneGroundReflectance, "");
    OS_ASSERT(result);
  }

  bool SiteGroundReflectance_Impl::setJulyGroundReflectance(double julyGroundReflectance) {
    bool result = setDouble(OS_Site_GroundReflectanceFields::JulyGroundReflectance, julyGroundReflectance);
    return result;
  }

  void SiteGroundReflectance_Impl::resetJulyGroundReflectance() {
    bool result = setString(OS_Site_GroundReflectanceFields::JulyGroundReflectance, "");
    OS_ASSERT(result);
  }

  bool SiteGroundReflectance_Impl::setAugustGroundReflectance(double augustGroundReflectance) {
    bool result = setDouble(OS_Site_GroundReflectanceFields::AugustGroundReflectance, augustGroundReflectance);
    return result;
  }

  void SiteGroundReflectance_Impl::resetAugustGroundReflectance() {
    bool result = setString(OS_Site_GroundReflectanceFields::AugustGroundReflectance, "");
    OS_ASSERT(result);
  }

  bool SiteGroundReflectance_Impl::setSeptemberGroundReflectance(double septemberGroundReflectance) {
    bool result = setDouble(OS_Site_GroundReflectanceFields::SeptemberGroundReflectance, septemberGroundReflectance);
    return result;
  }

  void SiteGroundReflectance_Impl::resetSeptemberGroundReflectance() {
    bool result = setString(OS_Site_GroundReflectanceFields::SeptemberGroundReflectance, "");
    OS_ASSERT(result);
  }

  bool SiteGroundReflectance_Impl::setOctoberGroundReflectance(double octoberGroundReflectance) {
    bool result = setDouble(OS_Site_GroundReflectanceFields::OctoberGroundReflectance, octoberGroundReflectance);
    return result;
  }

  void SiteGroundReflectance_Impl::resetOctoberGroundReflectance() {
    bool result = setString(OS_Site_GroundReflectanceFields::OctoberGroundReflectance, "");
    OS_ASSERT(result);
  }

  bool SiteGroundReflectance_Impl::setNovemberGroundReflectance(double novemberGroundReflectance) {
    bool result = setDouble(OS_Site_GroundReflectanceFields::NovemberGroundReflectance, novemberGroundReflectance);
    return result;
  }

  void SiteGroundReflectance_Impl::resetNovemberGroundReflectance() {
    bool result = setString(OS_Site_GroundReflectanceFields::NovemberGroundReflectance, "");
    OS_ASSERT(result);
  }

  bool SiteGroundReflectance_Impl::setDecemberGroundReflectance(double decemberGroundReflectance) {
    bool result = setDouble(OS_Site_GroundReflectanceFields::DecemberGroundReflectance, decemberGroundReflectance);
    return result;
  }

  void SiteGroundReflectance_Impl::resetDecemberGroundReflectance() {
    bool result = setString(OS_Site_GroundReflectanceFields::DecemberGroundReflectance, "");
    OS_ASSERT(result);
  }

} // detail

IddObjectType SiteGroundReflectance::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Site_GroundReflectance);
  return result;
}

double SiteGroundReflectance::januaryGroundReflectance() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->januaryGroundReflectance();
}

bool SiteGroundReflectance::isJanuaryGroundReflectanceDefaulted() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->isJanuaryGroundReflectanceDefaulted();
}

double SiteGroundReflectance::februaryGroundReflectance() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->februaryGroundReflectance();
}

bool SiteGroundReflectance::isFebruaryGroundReflectanceDefaulted() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->isFebruaryGroundReflectanceDefaulted();
}

double SiteGroundReflectance::marchGroundReflectance() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->marchGroundReflectance();
}

bool SiteGroundReflectance::isMarchGroundReflectanceDefaulted() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->isMarchGroundReflectanceDefaulted();
}

double SiteGroundReflectance::aprilGroundReflectance() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->aprilGroundReflectance();
}

bool SiteGroundReflectance::isAprilGroundReflectanceDefaulted() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->isAprilGroundReflectanceDefaulted();
}

double SiteGroundReflectance::mayGroundReflectance() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->mayGroundReflectance();
}

bool SiteGroundReflectance::isMayGroundReflectanceDefaulted() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->isMayGroundReflectanceDefaulted();
}

double SiteGroundReflectance::juneGroundReflectance() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->juneGroundReflectance();
}

bool SiteGroundReflectance::isJuneGroundReflectanceDefaulted() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->isJuneGroundReflectanceDefaulted();
}

double SiteGroundReflectance::julyGroundReflectance() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->julyGroundReflectance();
}

bool SiteGroundReflectance::isJulyGroundReflectanceDefaulted() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->isJulyGroundReflectanceDefaulted();
}

double SiteGroundReflectance::augustGroundReflectance() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->augustGroundReflectance();
}

bool SiteGroundReflectance::isAugustGroundReflectanceDefaulted() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->isAugustGroundReflectanceDefaulted();
}

double SiteGroundReflectance::septemberGroundReflectance() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->septemberGroundReflectance();
}

bool SiteGroundReflectance::isSeptemberGroundReflectanceDefaulted() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->isSeptemberGroundReflectanceDefaulted();
}

double SiteGroundReflectance::octoberGroundReflectance() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->octoberGroundReflectance();
}

bool SiteGroundReflectance::isOctoberGroundReflectanceDefaulted() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->isOctoberGroundReflectanceDefaulted();
}

double SiteGroundReflectance::novemberGroundReflectance() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->novemberGroundReflectance();
}

bool SiteGroundReflectance::isNovemberGroundReflectanceDefaulted() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->isNovemberGroundReflectanceDefaulted();
}

double SiteGroundReflectance::decemberGroundReflectance() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->decemberGroundReflectance();
}

bool SiteGroundReflectance::isDecemberGroundReflectanceDefaulted() const {
  return getImpl<detail::SiteGroundReflectance_Impl>()->isDecemberGroundReflectanceDefaulted();
}

bool SiteGroundReflectance::setJanuaryGroundReflectance(double januaryGroundReflectance) {
  return getImpl<detail::SiteGroundReflectance_Impl>()->setJanuaryGroundReflectance(januaryGroundReflectance);
}

void SiteGroundReflectance::resetJanuaryGroundReflectance() {
  getImpl<detail::SiteGroundReflectance_Impl>()->resetJanuaryGroundReflectance();
}

bool SiteGroundReflectance::setFebruaryGroundReflectance(double februaryGroundReflectance) {
  return getImpl<detail::SiteGroundReflectance_Impl>()->setFebruaryGroundReflectance(februaryGroundReflectance);
}

void SiteGroundReflectance::resetFebruaryGroundReflectance() {
  getImpl<detail::SiteGroundReflectance_Impl>()->resetFebruaryGroundReflectance();
}

bool SiteGroundReflectance::setMarchGroundReflectance(double marchGroundReflectance) {
  return getImpl<detail::SiteGroundReflectance_Impl>()->setMarchGroundReflectance(marchGroundReflectance);
}

void SiteGroundReflectance::resetMarchGroundReflectance() {
  getImpl<detail::SiteGroundReflectance_Impl>()->resetMarchGroundReflectance();
}

bool SiteGroundReflectance::setAprilGroundReflectance(double aprilGroundReflectance) {
  return getImpl<detail::SiteGroundReflectance_Impl>()->setAprilGroundReflectance(aprilGroundReflectance);
}

void SiteGroundReflectance::resetAprilGroundReflectance() {
  getImpl<detail::SiteGroundReflectance_Impl>()->resetAprilGroundReflectance();
}

bool SiteGroundReflectance::setMayGroundReflectance(double mayGroundReflectance) {
  return getImpl<detail::SiteGroundReflectance_Impl>()->setMayGroundReflectance(mayGroundReflectance);
}

void SiteGroundReflectance::resetMayGroundReflectance() {
  getImpl<detail::SiteGroundReflectance_Impl>()->resetMayGroundReflectance();
}

bool SiteGroundReflectance::setJuneGroundReflectance(double juneGroundReflectance) {
  return getImpl<detail::SiteGroundReflectance_Impl>()->setJuneGroundReflectance(juneGroundReflectance);
}

void SiteGroundReflectance::resetJuneGroundReflectance() {
  getImpl<detail::SiteGroundReflectance_Impl>()->resetJuneGroundReflectance();
}

bool SiteGroundReflectance::setJulyGroundReflectance(double julyGroundReflectance) {
  return getImpl<detail::SiteGroundReflectance_Impl>()->setJulyGroundReflectance(julyGroundReflectance);
}

void SiteGroundReflectance::resetJulyGroundReflectance() {
  getImpl<detail::SiteGroundReflectance_Impl>()->resetJulyGroundReflectance();
}

bool SiteGroundReflectance::setAugustGroundReflectance(double augustGroundReflectance) {
  return getImpl<detail::SiteGroundReflectance_Impl>()->setAugustGroundReflectance(augustGroundReflectance);
}

void SiteGroundReflectance::resetAugustGroundReflectance() {
  getImpl<detail::SiteGroundReflectance_Impl>()->resetAugustGroundReflectance();
}

bool SiteGroundReflectance::setSeptemberGroundReflectance(double septemberGroundReflectance) {
  return getImpl<detail::SiteGroundReflectance_Impl>()->setSeptemberGroundReflectance(septemberGroundReflectance);
}

void SiteGroundReflectance::resetSeptemberGroundReflectance() {
  getImpl<detail::SiteGroundReflectance_Impl>()->resetSeptemberGroundReflectance();
}

bool SiteGroundReflectance::setOctoberGroundReflectance(double octoberGroundReflectance) {
  return getImpl<detail::SiteGroundReflectance_Impl>()->setOctoberGroundReflectance(octoberGroundReflectance);
}

void SiteGroundReflectance::resetOctoberGroundReflectance() {
  getImpl<detail::SiteGroundReflectance_Impl>()->resetOctoberGroundReflectance();
}

bool SiteGroundReflectance::setNovemberGroundReflectance(double novemberGroundReflectance) {
  return getImpl<detail::SiteGroundReflectance_Impl>()->setNovemberGroundReflectance(novemberGroundReflectance);
}

void SiteGroundReflectance::resetNovemberGroundReflectance() {
  getImpl<detail::SiteGroundReflectance_Impl>()->resetNovemberGroundReflectance();
}

bool SiteGroundReflectance::setDecemberGroundReflectance(double decemberGroundReflectance) {
  return getImpl<detail::SiteGroundReflectance_Impl>()->setDecemberGroundReflectance(decemberGroundReflectance);
}

void SiteGroundReflectance::resetDecemberGroundReflectance() {
  getImpl<detail::SiteGroundReflectance_Impl>()->resetDecemberGroundReflectance();
}

/// @cond
SiteGroundReflectance::SiteGroundReflectance(std::shared_ptr<detail::SiteGroundReflectance_Impl> impl)
  : ModelObject(std::move(impl))
{}
SiteGroundReflectance::SiteGroundReflectance(Model& model)
  : ModelObject(SiteGroundReflectance::iddObjectType(),model)
{}

/// @endcond

} // model
} // openstudio

