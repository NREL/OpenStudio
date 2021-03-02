/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ConstructionAirBoundary.hpp"
#include "ConstructionAirBoundary_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Construction_AirBoundary_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ConstructionAirBoundary_Impl::ConstructionAirBoundary_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ConstructionBase_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ConstructionAirBoundary::iddObjectType());
    }

    ConstructionAirBoundary_Impl::ConstructionAirBoundary_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                               bool keepHandle)
      : ConstructionBase_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ConstructionAirBoundary::iddObjectType());
    }

    ConstructionAirBoundary_Impl::ConstructionAirBoundary_Impl(const ConstructionAirBoundary_Impl& other, Model_Impl* model, bool keepHandle)
      : ConstructionBase_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ConstructionAirBoundary_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType ConstructionAirBoundary_Impl::iddObjectType() const {
      return ConstructionAirBoundary::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ConstructionAirBoundary_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_Construction_AirBoundaryFields::SimpleMixingScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ConstructionAirBoundary", "Simple Mixing"));
      }
      return result;
    }

    bool ConstructionAirBoundary_Impl::isOpaque() const {
      return false;
    }

    bool ConstructionAirBoundary_Impl::isFenestration() const {
      return false;
    }

    bool ConstructionAirBoundary_Impl::isSolarDiffusing() const {
      return false;
    }

    bool ConstructionAirBoundary_Impl::isModelPartition() const {
      return true;
    }

    int ConstructionAirBoundary_Impl::renderingColorIndex() const {
      return OS_Construction_AirBoundaryFields::SurfaceRenderingName;
    }

    std::string ConstructionAirBoundary_Impl::solarAndDaylightingMethod() const {
      boost::optional<std::string> value = getString(OS_Construction_AirBoundaryFields::SolarandDaylightingMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ConstructionAirBoundary_Impl::isSolarAndDaylightingMethodDefaulted() const {
      return isEmpty(OS_Construction_AirBoundaryFields::SolarandDaylightingMethod);
    }

    std::string ConstructionAirBoundary_Impl::radiantExchangeMethod() const {
      boost::optional<std::string> value = getString(OS_Construction_AirBoundaryFields::RadiantExchangeMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ConstructionAirBoundary_Impl::isRadiantExchangeMethodDefaulted() const {
      return isEmpty(OS_Construction_AirBoundaryFields::RadiantExchangeMethod);
    }

    std::string ConstructionAirBoundary_Impl::airExchangeMethod() const {
      boost::optional<std::string> value = getString(OS_Construction_AirBoundaryFields::AirExchangeMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ConstructionAirBoundary_Impl::isAirExchangeMethodDefaulted() const {
      return isEmpty(OS_Construction_AirBoundaryFields::AirExchangeMethod);
    }

    double ConstructionAirBoundary_Impl::simpleMixingAirChangesPerHour() const {
      boost::optional<double> value = getDouble(OS_Construction_AirBoundaryFields::SimpleMixingAirChangesperHour, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ConstructionAirBoundary_Impl::isSimpleMixingAirChangesPerHourDefaulted() const {
      return isEmpty(OS_Construction_AirBoundaryFields::SimpleMixingAirChangesperHour);
    }

    boost::optional<Schedule> ConstructionAirBoundary_Impl::simpleMixingSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Construction_AirBoundaryFields::SimpleMixingScheduleName);
    }

    bool ConstructionAirBoundary_Impl::setSolarAndDaylightingMethod(const std::string& solarAndDaylightingMethod) {
      bool result = setString(OS_Construction_AirBoundaryFields::SolarandDaylightingMethod, solarAndDaylightingMethod);
      return result;
    }

    void ConstructionAirBoundary_Impl::resetSolarAndDaylightingMethod() {
      bool result = setString(OS_Construction_AirBoundaryFields::SolarandDaylightingMethod, "");
      OS_ASSERT(result);
    }

    bool ConstructionAirBoundary_Impl::setRadiantExchangeMethod(const std::string& radiantExchangeMethod) {
      bool result = setString(OS_Construction_AirBoundaryFields::RadiantExchangeMethod, radiantExchangeMethod);
      return result;
    }

    void ConstructionAirBoundary_Impl::resetRadiantExchangeMethod() {
      bool result = setString(OS_Construction_AirBoundaryFields::RadiantExchangeMethod, "");
      OS_ASSERT(result);
    }

    bool ConstructionAirBoundary_Impl::setAirExchangeMethod(const std::string& airExchangeMethod) {
      bool result = setString(OS_Construction_AirBoundaryFields::AirExchangeMethod, airExchangeMethod);
      return result;
    }

    void ConstructionAirBoundary_Impl::resetAirExchangeMethod() {
      bool result = setString(OS_Construction_AirBoundaryFields::AirExchangeMethod, "");
      OS_ASSERT(result);
    }

    bool ConstructionAirBoundary_Impl::setSimpleMixingAirChangesPerHour(double simpleMixingAirChangesPerHour) {
      bool result = setDouble(OS_Construction_AirBoundaryFields::SimpleMixingAirChangesperHour, simpleMixingAirChangesPerHour);
      return result;
    }

    void ConstructionAirBoundary_Impl::resetSimpleMixingAirChangesPerHour() {
      bool result = setString(OS_Construction_AirBoundaryFields::SimpleMixingAirChangesperHour, "");
      OS_ASSERT(result);
    }

    bool ConstructionAirBoundary_Impl::setSimpleMixingSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Construction_AirBoundaryFields::SimpleMixingScheduleName, "ConstructionAirBoundary", "Simple Mixing", schedule);
      return result;
    }

    void ConstructionAirBoundary_Impl::resetSimpleMixingSchedule() {
      bool result = setString(OS_Construction_AirBoundaryFields::SimpleMixingScheduleName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  ConstructionAirBoundary::ConstructionAirBoundary(const Model& model) : ConstructionBase(ConstructionAirBoundary::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ConstructionAirBoundary_Impl>());

    setSimpleMixingAirChangesPerHour(0.0);
  }

  IddObjectType ConstructionAirBoundary::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Construction_AirBoundary);
  }

  std::vector<std::string> ConstructionAirBoundary::solarAndDaylightingMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Construction_AirBoundaryFields::SolarandDaylightingMethod);
  }

  std::vector<std::string> ConstructionAirBoundary::radiantExchangeMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Construction_AirBoundaryFields::RadiantExchangeMethod);
  }

  std::vector<std::string> ConstructionAirBoundary::airExchangeMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Construction_AirBoundaryFields::AirExchangeMethod);
  }

  std::string ConstructionAirBoundary::solarAndDaylightingMethod() const {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->solarAndDaylightingMethod();
  }

  bool ConstructionAirBoundary::isSolarAndDaylightingMethodDefaulted() const {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->isSolarAndDaylightingMethodDefaulted();
  }

  std::string ConstructionAirBoundary::radiantExchangeMethod() const {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->radiantExchangeMethod();
  }

  bool ConstructionAirBoundary::isRadiantExchangeMethodDefaulted() const {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->isRadiantExchangeMethodDefaulted();
  }

  std::string ConstructionAirBoundary::airExchangeMethod() const {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->airExchangeMethod();
  }

  bool ConstructionAirBoundary::isAirExchangeMethodDefaulted() const {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->isAirExchangeMethodDefaulted();
  }

  double ConstructionAirBoundary::simpleMixingAirChangesPerHour() const {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->simpleMixingAirChangesPerHour();
  }

  bool ConstructionAirBoundary::isSimpleMixingAirChangesPerHourDefaulted() const {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->isSimpleMixingAirChangesPerHourDefaulted();
  }

  boost::optional<Schedule> ConstructionAirBoundary::simpleMixingSchedule() const {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->simpleMixingSchedule();
  }

  bool ConstructionAirBoundary::setSolarAndDaylightingMethod(const std::string& solarAndDaylightingMethod) {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->setSolarAndDaylightingMethod(solarAndDaylightingMethod);
  }

  void ConstructionAirBoundary::resetSolarAndDaylightingMethod() {
    getImpl<detail::ConstructionAirBoundary_Impl>()->resetSolarAndDaylightingMethod();
  }

  bool ConstructionAirBoundary::setRadiantExchangeMethod(const std::string& radiantExchangeMethod) {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->setRadiantExchangeMethod(radiantExchangeMethod);
  }

  void ConstructionAirBoundary::resetRadiantExchangeMethod() {
    getImpl<detail::ConstructionAirBoundary_Impl>()->resetRadiantExchangeMethod();
  }

  bool ConstructionAirBoundary::setAirExchangeMethod(const std::string& airExchangeMethod) {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->setAirExchangeMethod(airExchangeMethod);
  }

  void ConstructionAirBoundary::resetAirExchangeMethod() {
    getImpl<detail::ConstructionAirBoundary_Impl>()->resetAirExchangeMethod();
  }

  bool ConstructionAirBoundary::setSimpleMixingAirChangesPerHour(double simpleMixingAirChangesPerHour) {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->setSimpleMixingAirChangesPerHour(simpleMixingAirChangesPerHour);
  }

  void ConstructionAirBoundary::resetSimpleMixingAirChangesPerHour() {
    getImpl<detail::ConstructionAirBoundary_Impl>()->resetSimpleMixingAirChangesPerHour();
  }

  bool ConstructionAirBoundary::setSimpleMixingSchedule(Schedule& schedule) {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->setSimpleMixingSchedule(schedule);
  }

  void ConstructionAirBoundary::resetSimpleMixingSchedule() {
    getImpl<detail::ConstructionAirBoundary_Impl>()->resetSimpleMixingSchedule();
  }

  /// @cond
  ConstructionAirBoundary::ConstructionAirBoundary(std::shared_ptr<detail::ConstructionAirBoundary_Impl> impl) : ConstructionBase(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
