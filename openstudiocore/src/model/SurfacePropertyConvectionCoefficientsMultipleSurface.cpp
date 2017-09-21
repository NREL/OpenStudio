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

#include "SurfacePropertyConvectionCoefficientsMultipleSurface.hpp"
#include "SurfacePropertyConvectionCoefficientsMultipleSurface_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SurfaceProperty_ConvectionCoefficients_MultipleSurface_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl(const IdfObject& idfObject,
                                                                                                                       Model_Impl* model,
                                                                                                                       bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SurfacePropertyConvectionCoefficientsMultipleSurface::iddObjectType());
  }

  SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                       Model_Impl* model,
                                                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SurfacePropertyConvectionCoefficientsMultipleSurface::iddObjectType());
  }

  SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl(const SurfacePropertyConvectionCoefficientsMultipleSurface_Impl& other,
                                                                                                                       Model_Impl* model,
                                                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::iddObjectType() const {
    return SurfacePropertyConvectionCoefficientsMultipleSurface::iddObjectType();
  }

  std::vector<ScheduleTypeKey> SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SurfacePropertyConvectionCoefficientsMultipleSurface","Convection Coefficient 1"));
    }
    if (std::find(b,e,OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SurfacePropertyConvectionCoefficientsMultipleSurface","Convection Coefficient 2"));
    }
    return result;
  }

  boost::optional<std::string> SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::surfaceType() const {
    return getString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::SurfaceType,true);
  }

  boost::optional<std::string> SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::convectionCoefficient1Location() const {
    return getString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1Location,true);
  }

  boost::optional<std::string> SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::convectionCoefficient1Type() const {
    return getString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1Type,true);
  }

  boost::optional<double> SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::convectionCoefficient1() const {
    return getDouble(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1,true);
  }

  boost::optional<Schedule> SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::convectionCoefficient1Schedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1ScheduleName);
  }

  //boost::optional<UserConvectionModels> SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::convectionCoefficient1UserCurve() const {
  //  return getObject<ModelObject>().getModelObjectTarget<UserConvectionModels>(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1UserCurveName);
  //}

  boost::optional<std::string> SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::convectionCoefficient2Location() const {
    return getString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2Location,true);
  }

  boost::optional<std::string> SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::convectionCoefficient2Type() const {
    return getString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2Type,true);
  }

  boost::optional<double> SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::convectionCoefficient2() const {
    return getDouble(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2,true);
  }

  boost::optional<Schedule> SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::convectionCoefficient2Schedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2ScheduleName);
  }

  //boost::optional<UserConvectionModels> SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::convectionCoefficient2UserCurve() const {
  //  return getObject<ModelObject>().getModelObjectTarget<UserConvectionModels>(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2UserCurveName);
  //}

  bool SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::setSurfaceType(const std::string& surfaceType) {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::SurfaceType, surfaceType);
    return result;
  }

  void SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::resetSurfaceType() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::SurfaceType, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::setConvectionCoefficient1Location(const std::string& convectionCoefficient1Location) {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1Location, convectionCoefficient1Location);
    return result;
  }

  void SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::resetConvectionCoefficient1Location() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1Location, "");
    OS_ASSERT(result);
  }


  bool SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::setConvectionCoefficient1Type(const std::string& convectionCoefficient1Type) {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1Type, convectionCoefficient1Type);
    return result;
  }

  void SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::resetConvectionCoefficient1Type() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1Type, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::setConvectionCoefficient1(double convectionCoefficient1) {
    return setDouble(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1, convectionCoefficient1);
  }

  void SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::resetConvectionCoefficient1() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::setConvectionCoefficient1Schedule(Schedule& schedule) {
    bool result = setSchedule(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1ScheduleName,
                              "SurfacePropertyConvectionCoefficientsMultipleSurface",
                              "Convection Coefficient 1",
                              schedule);
    return result;
  }

  void SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::resetConvectionCoefficient1Schedule() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1ScheduleName, "");
    OS_ASSERT(result);
  }

  //bool SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::setConvectionCoefficient1UserCurve(const boost::optional<UserConvectionModels>& userConvectionModels) {
  //  bool result(false);
  //  if (userConvectionModels) {
  //    result = setPointer(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1UserCurveName, userConvectionModels.get().handle());
  //  }
  //  else {
  //    resetConvectionCoefficient1UserCurve();
  //    result = true;
  //  }
  //  return result;
  //}

  //void SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::resetConvectionCoefficient1UserCurve() {
  //  bool result = setString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1UserCurveName, "");
  //  OS_ASSERT(result);
  //}

  bool SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::setConvectionCoefficient2Location(const std::string& convectionCoefficient2Location) {
    return setString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2Location, convectionCoefficient2Location);
  }

  void SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::resetConvectionCoefficient2Location() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2Location, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::setConvectionCoefficient2Type(const std::string& convectionCoefficient2Type) {
    return setString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2Type, convectionCoefficient2Type);
  }

  void SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::resetConvectionCoefficient2Type() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2Type, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::setConvectionCoefficient2(double convectionCoefficient2) {
    return setDouble(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2, convectionCoefficient2);
  }

  void SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::resetConvectionCoefficient2() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::setConvectionCoefficient2Schedule(Schedule& schedule) {
    bool result = setSchedule(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2ScheduleName,
                              "SurfacePropertyConvectionCoefficientsMultipleSurface",
                              "Convection Coefficient 2",
                              schedule);
    return result;
  }

  void SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::resetConvectionCoefficient2Schedule() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2ScheduleName, "");
    OS_ASSERT(result);
  }

  //bool SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::setConvectionCoefficient2UserCurve(const boost::optional<UserConvectionModels>& userConvectionModels) {
  //  bool result(false);
  //  if (userConvectionModels) {
  //    result = setPointer(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2UserCurveName, userConvectionModels.get().handle());
  //  }
  //  else {
  //    resetConvectionCoefficient2UserCurve();
  //    result = true;
  //  }
  //  return result;
  //}

  //void SurfacePropertyConvectionCoefficientsMultipleSurface_Impl::resetConvectionCoefficient2UserCurve() {
  //  bool result = setString(OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2UserCurveName, "");
  //  OS_ASSERT(result);
  //}

} // detail

SurfacePropertyConvectionCoefficientsMultipleSurface::SurfacePropertyConvectionCoefficientsMultipleSurface(const Model& model)
  : ModelObject(SurfacePropertyConvectionCoefficientsMultipleSurface::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>());
}

IddObjectType SurfacePropertyConvectionCoefficientsMultipleSurface::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SurfaceProperty_ConvectionCoefficients_MultipleSurface);
}

std::vector<std::string> SurfacePropertyConvectionCoefficientsMultipleSurface::surfaceTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::SurfaceType);
}

std::vector<std::string> SurfacePropertyConvectionCoefficientsMultipleSurface::convectionCoefficient1LocationValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1Location);
}

std::vector<std::string> SurfacePropertyConvectionCoefficientsMultipleSurface::convectionCoefficient1TypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1Type);
}

std::vector<std::string> SurfacePropertyConvectionCoefficientsMultipleSurface::convectionCoefficient2LocationValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2Location);
}

std::vector<std::string> SurfacePropertyConvectionCoefficientsMultipleSurface::convectionCoefficient2TypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2Type);
}

boost::optional<std::string> SurfacePropertyConvectionCoefficientsMultipleSurface::surfaceType() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->surfaceType();
}

boost::optional<std::string> SurfacePropertyConvectionCoefficientsMultipleSurface::convectionCoefficient1Location() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->convectionCoefficient1Location();
}

boost::optional<std::string> SurfacePropertyConvectionCoefficientsMultipleSurface::convectionCoefficient1Type() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->convectionCoefficient1Type();
}

boost::optional<double> SurfacePropertyConvectionCoefficientsMultipleSurface::convectionCoefficient1() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->convectionCoefficient1();
}

boost::optional<Schedule> SurfacePropertyConvectionCoefficientsMultipleSurface::convectionCoefficient1Schedule() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->convectionCoefficient1Schedule();
}

//boost::optional<UserConvectionModels> SurfacePropertyConvectionCoefficientsMultipleSurface::convectionCoefficient1UserCurve() const {
//  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->convectionCoefficient1UserCurve();
//}

boost::optional<std::string> SurfacePropertyConvectionCoefficientsMultipleSurface::convectionCoefficient2Location() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->convectionCoefficient2Location();
}

boost::optional<std::string> SurfacePropertyConvectionCoefficientsMultipleSurface::convectionCoefficient2Type() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->convectionCoefficient2Type();
}

boost::optional<double> SurfacePropertyConvectionCoefficientsMultipleSurface::convectionCoefficient2() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->convectionCoefficient2();
}

boost::optional<Schedule> SurfacePropertyConvectionCoefficientsMultipleSurface::convectionCoefficient2Schedule() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->convectionCoefficient2Schedule();
}

//boost::optional<UserConvectionModels> SurfacePropertyConvectionCoefficientsMultipleSurface::convectionCoefficient2UserCurve() const {
//  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->convectionCoefficient2UserCurve();
//}

bool SurfacePropertyConvectionCoefficientsMultipleSurface::setSurfaceType(const std::string& surfaceType) {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->setSurfaceType(surfaceType);
}

void SurfacePropertyConvectionCoefficientsMultipleSurface::resetSurfaceType() {
  getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->resetSurfaceType();
}

bool SurfacePropertyConvectionCoefficientsMultipleSurface::setConvectionCoefficient1Location(const std::string& convectionCoefficient1Location) {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->setConvectionCoefficient1Location(convectionCoefficient1Location);
}

void SurfacePropertyConvectionCoefficientsMultipleSurface::resetConvectionCoefficient1Location() {
  getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->resetConvectionCoefficient1Location();
}

bool SurfacePropertyConvectionCoefficientsMultipleSurface::setConvectionCoefficient1Type(const std::string& convectionCoefficient1Type) {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->setConvectionCoefficient1Type(convectionCoefficient1Type);
}

void SurfacePropertyConvectionCoefficientsMultipleSurface::resetConvectionCoefficient1Type() {
  getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->resetConvectionCoefficient1Type();
}

bool SurfacePropertyConvectionCoefficientsMultipleSurface::setConvectionCoefficient1(double convectionCoefficient1) {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->setConvectionCoefficient1(convectionCoefficient1);
}

void SurfacePropertyConvectionCoefficientsMultipleSurface::resetConvectionCoefficient1() {
  getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->resetConvectionCoefficient1();
}

bool SurfacePropertyConvectionCoefficientsMultipleSurface::setConvectionCoefficient1Schedule(Schedule& schedule) {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->setConvectionCoefficient1Schedule(schedule);
}

void SurfacePropertyConvectionCoefficientsMultipleSurface::resetConvectionCoefficient1Schedule() {
  getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->resetConvectionCoefficient1Schedule();
}

//bool SurfacePropertyConvectionCoefficientsMultipleSurface::setConvectionCoefficient1UserCurve(const UserConvectionModels& userConvectionModels) {
//  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->setConvectionCoefficient1UserCurve(userConvectionModels);
//}

//void SurfacePropertyConvectionCoefficientsMultipleSurface::resetConvectionCoefficient1UserCurve() {
//  getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->resetConvectionCoefficient1UserCurve();
//}

bool SurfacePropertyConvectionCoefficientsMultipleSurface::setConvectionCoefficient2Location(const std::string& convectionCoefficient2Location) {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->setConvectionCoefficient2Location(convectionCoefficient2Location);
}

void SurfacePropertyConvectionCoefficientsMultipleSurface::resetConvectionCoefficient2Location() {
  getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->resetConvectionCoefficient2Location();
}

bool SurfacePropertyConvectionCoefficientsMultipleSurface::setConvectionCoefficient2Type(const std::string& convectionCoefficient2Type) {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->setConvectionCoefficient2Type(convectionCoefficient2Type);
}

void SurfacePropertyConvectionCoefficientsMultipleSurface::resetConvectionCoefficient2Type() {
  getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->resetConvectionCoefficient2Type();
}

bool SurfacePropertyConvectionCoefficientsMultipleSurface::setConvectionCoefficient2(double convectionCoefficient2) {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->setConvectionCoefficient2(convectionCoefficient2);
}

void SurfacePropertyConvectionCoefficientsMultipleSurface::resetConvectionCoefficient2() {
  getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->resetConvectionCoefficient2();
}

bool SurfacePropertyConvectionCoefficientsMultipleSurface::setConvectionCoefficient2Schedule(Schedule& schedule) {
  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->setConvectionCoefficient2Schedule(schedule);
}

void SurfacePropertyConvectionCoefficientsMultipleSurface::resetConvectionCoefficient2Schedule() {
  getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->resetConvectionCoefficient2Schedule();
}

//bool SurfacePropertyConvectionCoefficientsMultipleSurface::setConvectionCoefficient2UserCurve(const UserConvectionModels& userConvectionModels) {
//  return getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->setConvectionCoefficient2UserCurve(userConvectionModels);
//}

//void SurfacePropertyConvectionCoefficientsMultipleSurface::resetConvectionCoefficient2UserCurve() {
//  getImpl<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl>()->resetConvectionCoefficient2UserCurve();
//}

/// @cond
SurfacePropertyConvectionCoefficientsMultipleSurface::SurfacePropertyConvectionCoefficientsMultipleSurface(std::shared_ptr<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

