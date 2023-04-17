/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "SpaceInfiltrationFlowCoefficient.hpp"
#include "SpaceInfiltrationFlowCoefficient_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "DefaultScheduleSet.hpp"
#include "DefaultScheduleSet_Impl.hpp"
#include "LifeCycleCost.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SpaceInfiltration_FlowCoefficient_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SpaceInfiltrationFlowCoefficient_Impl::SpaceInfiltrationFlowCoefficient_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : SpaceLoad_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SpaceInfiltrationFlowCoefficient::iddObjectType());
    }

    SpaceInfiltrationFlowCoefficient_Impl::SpaceInfiltrationFlowCoefficient_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : SpaceLoad_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SpaceInfiltrationFlowCoefficient::iddObjectType());
    }

    SpaceInfiltrationFlowCoefficient_Impl::SpaceInfiltrationFlowCoefficient_Impl(const SpaceInfiltrationFlowCoefficient_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : SpaceLoad_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SpaceInfiltrationFlowCoefficient_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SpaceInfiltrationFlowCoefficient_Impl::iddObjectType() const {
      return SpaceInfiltrationFlowCoefficient::iddObjectType();
    }

    std::vector<ScheduleTypeKey> SpaceInfiltrationFlowCoefficient_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_SpaceInfiltration_FlowCoefficientFields::ScheduleName) != e) {
        result.push_back(ScheduleTypeKey("SpaceInfiltrationFlowCoefficient", "Infiltration"));
      }
      return result;
    }

    bool SpaceInfiltrationFlowCoefficient_Impl::hardSize() {
      boost::optional<Space> space = this->space();
      if (!space) {
        return false;
      }
      return false;
    }

    bool SpaceInfiltrationFlowCoefficient_Impl::hardApplySchedules() {
      bool result = false;
      boost::optional<Schedule> schedule = this->schedule();
      if (schedule) {
        result = this->setSchedule(*schedule);
      }
      return result;
    }

    bool SpaceInfiltrationFlowCoefficient_Impl::isAbsolute() const {
      return true;
    }

    double SpaceInfiltrationFlowCoefficient_Impl::flowCoefficient() const {
      boost::optional<double> value = getDouble(OS_SpaceInfiltration_FlowCoefficientFields::FlowCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SpaceInfiltrationFlowCoefficient_Impl::setFlowCoefficient(double flowCoefficient) {
      bool result = setDouble(OS_SpaceInfiltration_FlowCoefficientFields::FlowCoefficient, flowCoefficient);
      return result;
    }

    double SpaceInfiltrationFlowCoefficient_Impl::stackCoefficient() const {
      boost::optional<double> value = getDouble(OS_SpaceInfiltration_FlowCoefficientFields::StackCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SpaceInfiltrationFlowCoefficient_Impl::setStackCoefficient(double stackCoefficient) {
      bool result = setDouble(OS_SpaceInfiltration_FlowCoefficientFields::StackCoefficient, stackCoefficient);
      return result;
    }

    double SpaceInfiltrationFlowCoefficient_Impl::pressureExponent() const {
      boost::optional<double> value = getDouble(OS_SpaceInfiltration_FlowCoefficientFields::PressureExponent, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SpaceInfiltrationFlowCoefficient_Impl::setPressureExponent(double pressureExponent) {
      bool result = setDouble(OS_SpaceInfiltration_FlowCoefficientFields::PressureExponent, pressureExponent);
      return result;
    }

    double SpaceInfiltrationFlowCoefficient_Impl::windCoefficient() const {
      boost::optional<double> value = getDouble(OS_SpaceInfiltration_FlowCoefficientFields::WindCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SpaceInfiltrationFlowCoefficient_Impl::setWindCoefficient(double windCoefficient) {
      bool result = setDouble(OS_SpaceInfiltration_FlowCoefficientFields::WindCoefficient, windCoefficient);
      return result;
    }

    double SpaceInfiltrationFlowCoefficient_Impl::shelterFactor() const {
      boost::optional<double> value = getDouble(OS_SpaceInfiltration_FlowCoefficientFields::ShelterFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SpaceInfiltrationFlowCoefficient_Impl::setShelterFactor(double shelterFactor) {
      bool result = setDouble(OS_SpaceInfiltration_FlowCoefficientFields::ShelterFactor, shelterFactor);
      return result;
    }

    int SpaceInfiltrationFlowCoefficient_Impl::spaceIndex() const {
      return OS_SpaceInfiltration_FlowCoefficientFields::SpaceorSpaceTypeName;
    }

    boost::optional<Schedule> SpaceInfiltrationFlowCoefficient_Impl::schedule() const {
      boost::optional<Schedule> result =
        getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SpaceInfiltration_FlowCoefficientFields::ScheduleName);
      if (!result) {
        // search upwards
        OptionalSpace space = this->space();
        OptionalSpaceType spaceType = this->spaceType();
        if (space) {
          result = space->getDefaultSchedule(DefaultScheduleType::InfiltrationSchedule);
        } else if (spaceType) {
          result = spaceType->getDefaultSchedule(DefaultScheduleType::InfiltrationSchedule);
        }
      }
      return result;
    }

    bool SpaceInfiltrationFlowCoefficient_Impl::isScheduleDefaulted() const {
      return isEmpty(OS_SpaceInfiltration_FlowCoefficientFields::ScheduleName);
    }

    bool SpaceInfiltrationFlowCoefficient_Impl::setSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_SpaceInfiltration_FlowCoefficientFields::ScheduleName, "SpaceInfiltrationFlowCoefficient",
                                                  "Infiltration", schedule);
      return result;
    }

    void SpaceInfiltrationFlowCoefficient_Impl::resetSchedule() {
      bool result = setString(OS_SpaceInfiltration_FlowCoefficientFields::ScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<ModelObject> SpaceInfiltrationFlowCoefficient_Impl::scheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = schedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool SpaceInfiltrationFlowCoefficient_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetSchedule();
      }
      return true;
    }

  }  // namespace detail

  SpaceInfiltrationFlowCoefficient::SpaceInfiltrationFlowCoefficient(const Model& model)
    : SpaceLoad(SpaceInfiltrationFlowCoefficient::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SpaceInfiltrationFlowCoefficient_Impl>());

    // IDD default
    bool test = setPressureExponent(0.67);
    OS_ASSERT(test);

    // Rest from DirectIndirectEvapCoolers.idf
    test = setFlowCoefficient(0.02);
    OS_ASSERT(test);
    test = setStackCoefficient(0.05);
    OS_ASSERT(test);
    test = setWindCoefficient(0.12);
    OS_ASSERT(test);
    test = setShelterFactor(0.5);
    OS_ASSERT(test);
  }

  IddObjectType SpaceInfiltrationFlowCoefficient::iddObjectType() {
    IddObjectType result(IddObjectType::OS_SpaceInfiltration_FlowCoefficient);
    return result;
  }

  boost::optional<Schedule> SpaceInfiltrationFlowCoefficient::schedule() const {
    return getImpl<detail::SpaceInfiltrationFlowCoefficient_Impl>()->schedule();
  }

  bool SpaceInfiltrationFlowCoefficient::isScheduleDefaulted() const {
    return getImpl<detail::SpaceInfiltrationFlowCoefficient_Impl>()->isScheduleDefaulted();
  }

  double SpaceInfiltrationFlowCoefficient::flowCoefficient() const {
    return getImpl<detail::SpaceInfiltrationFlowCoefficient_Impl>()->flowCoefficient();
  }

  double SpaceInfiltrationFlowCoefficient::stackCoefficient() const {
    return getImpl<detail::SpaceInfiltrationFlowCoefficient_Impl>()->stackCoefficient();
  }

  double SpaceInfiltrationFlowCoefficient::pressureExponent() const {
    return getImpl<detail::SpaceInfiltrationFlowCoefficient_Impl>()->pressureExponent();
  }

  double SpaceInfiltrationFlowCoefficient::windCoefficient() const {
    return getImpl<detail::SpaceInfiltrationFlowCoefficient_Impl>()->windCoefficient();
  }

  double SpaceInfiltrationFlowCoefficient::shelterFactor() const {
    return getImpl<detail::SpaceInfiltrationFlowCoefficient_Impl>()->shelterFactor();
  }

  bool SpaceInfiltrationFlowCoefficient::setSchedule(Schedule& schedule) {
    return getImpl<detail::SpaceInfiltrationFlowCoefficient_Impl>()->setSchedule(schedule);
  }

  void SpaceInfiltrationFlowCoefficient::resetSchedule() {
    getImpl<detail::SpaceInfiltrationFlowCoefficient_Impl>()->resetSchedule();
  }

  bool SpaceInfiltrationFlowCoefficient::setFlowCoefficient(double flowCoefficient) {
    return getImpl<detail::SpaceInfiltrationFlowCoefficient_Impl>()->setFlowCoefficient(flowCoefficient);
  }

  bool SpaceInfiltrationFlowCoefficient::setStackCoefficient(double stackCoefficient) {
    return getImpl<detail::SpaceInfiltrationFlowCoefficient_Impl>()->setStackCoefficient(stackCoefficient);
  }

  bool SpaceInfiltrationFlowCoefficient::setPressureExponent(double pressureExponent) {
    return getImpl<detail::SpaceInfiltrationFlowCoefficient_Impl>()->setPressureExponent(pressureExponent);
  }

  bool SpaceInfiltrationFlowCoefficient::setWindCoefficient(double windCoefficient) {
    return getImpl<detail::SpaceInfiltrationFlowCoefficient_Impl>()->setWindCoefficient(windCoefficient);
  }

  bool SpaceInfiltrationFlowCoefficient::setShelterFactor(double shelterFactor) {
    return getImpl<detail::SpaceInfiltrationFlowCoefficient_Impl>()->setShelterFactor(shelterFactor);
  }

  /// @cond
  SpaceInfiltrationFlowCoefficient::SpaceInfiltrationFlowCoefficient(std::shared_ptr<detail::SpaceInfiltrationFlowCoefficient_Impl> impl)
    : SpaceLoad(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
