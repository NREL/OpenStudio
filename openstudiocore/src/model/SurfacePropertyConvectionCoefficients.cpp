/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "SurfacePropertyConvectionCoefficients.hpp"
#include "SurfacePropertyConvectionCoefficients_Impl.hpp"

#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SurfaceProperty_ConvectionCoefficients_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SurfacePropertyConvectionCoefficients_Impl::SurfacePropertyConvectionCoefficients_Impl(const IdfObject& idfObject,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SurfacePropertyConvectionCoefficients::iddObjectType());
  }

  SurfacePropertyConvectionCoefficients_Impl::SurfacePropertyConvectionCoefficients_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SurfacePropertyConvectionCoefficients::iddObjectType());
  }

  SurfacePropertyConvectionCoefficients_Impl::SurfacePropertyConvectionCoefficients_Impl(const SurfacePropertyConvectionCoefficients_Impl& other,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SurfacePropertyConvectionCoefficients_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SurfacePropertyConvectionCoefficients_Impl::iddObjectType() const {
    return SurfacePropertyConvectionCoefficients::iddObjectType();
  }

  std::vector<ScheduleTypeKey> SurfacePropertyConvectionCoefficients_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SurfacePropertyConvectionCoefficients","Convection Coefficient 1"));
    }
    if (std::find(b,e,OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SurfacePropertyConvectionCoefficients","Convection Coefficient 2"));
    }
    return result;
  }

  boost::optional<PlanarSurface> SurfacePropertyConvectionCoefficients_Impl::surface() const {
    return getObject<ModelObject>().getModelObjectTarget<PlanarSurface>(OS_SurfaceProperty_ConvectionCoefficientsFields::SurfaceName);
  }

  boost::optional<std::string> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient1Location() const {
    return getString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Location,true);
  }

  boost::optional<std::string> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient1Type() const {
    return getString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Type,true);
  }

  boost::optional<double> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient1() const {
    return getDouble(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1,true);
  }

  boost::optional<Schedule> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient1Schedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1ScheduleName);
  }

  //boost::optional<UserConvectionModels> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient1UserCurve() const {
  //  return getObject<ModelObject>().getModelObjectTarget<UserConvectionModels>(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1UserCurveName);
  //}

  boost::optional<std::string> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient2Location() const {
    return getString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Location,true);
  }

  boost::optional<std::string> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient2Type() const {
    return getString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Type,true);
  }

  boost::optional<double> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient2() const {
    return getDouble(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2,true);
  }

  boost::optional<Schedule> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient2Schedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2ScheduleName);
  }

  //boost::optional<UserConvectionModels> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient2UserCurve() const {
  //  return getObject<ModelObject>().getModelObjectTarget<UserConvectionModels>(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2UserCurveName);
  //}

  bool SurfacePropertyConvectionCoefficients_Impl::setSurface(const PlanarSurface& surface) {
    bool result = setPointer(OS_SurfaceProperty_ConvectionCoefficientsFields::SurfaceName, surface.handle());
    return result;
  }

  bool SurfacePropertyConvectionCoefficients_Impl::setConvectionCoefficient1Location(const std::string& convectionCoefficient1Location) {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Location, convectionCoefficient1Location);
    return result;
  }

  bool SurfacePropertyConvectionCoefficients_Impl::setConvectionCoefficient1Type(const std::string& convectionCoefficient1Type) {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Type, convectionCoefficient1Type);
    return result;
  }

  bool SurfacePropertyConvectionCoefficients_Impl::setConvectionCoefficient1(double convectionCoefficient1) {
    bool result = setDouble(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1, convectionCoefficient1);
    return result;
  }

  void SurfacePropertyConvectionCoefficients_Impl::resetConvectionCoefficient1() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyConvectionCoefficients_Impl::setConvectionCoefficient1Schedule(Schedule& schedule) {
    bool result = setSchedule(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1ScheduleName,
                              "SurfacePropertyConvectionCoefficients",
                              "Convection Coefficient 1",
                              schedule);
    return result;
  }

  void SurfacePropertyConvectionCoefficients_Impl::resetConvectionCoefficient1Schedule() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1ScheduleName, "");
    OS_ASSERT(result);
  }
  /*
  bool SurfacePropertyConvectionCoefficients_Impl::setConvectionCoefficient1UserCurve(const boost::optional<UserConvectionModels>& userConvectionModels) {
    bool result(false);
    if (userConvectionModels) {
      result = setPointer(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1UserCurveName, userConvectionModels.get().handle());
    }
    else {
      resetConvectionCoefficient1UserCurve();
      result = true;
    }
    return result;
  }

  void SurfacePropertyConvectionCoefficients_Impl::resetConvectionCoefficient1UserCurve() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1UserCurveName, "");
    OS_ASSERT(result);
  }
  */

  bool SurfacePropertyConvectionCoefficients_Impl::setConvectionCoefficient2Location(const std::string& convectionCoefficient2Location) {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Location, convectionCoefficient2Location);
    return result;
  }

  void SurfacePropertyConvectionCoefficients_Impl::resetConvectionCoefficient2Location() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Location, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyConvectionCoefficients_Impl::setConvectionCoefficient2Type(const std::string& convectionCoefficient2Type) {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Type, convectionCoefficient2Type);
    return result;
  }

  void SurfacePropertyConvectionCoefficients_Impl::resetConvectionCoefficient2Type() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Type, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyConvectionCoefficients_Impl::setConvectionCoefficient2(double convectionCoefficient2) {
    bool result = setDouble(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2, convectionCoefficient2);
    return result;
  }

  void SurfacePropertyConvectionCoefficients_Impl::resetConvectionCoefficient2() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyConvectionCoefficients_Impl::setConvectionCoefficient2Schedule(Schedule& schedule) {
    bool result = setSchedule(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2ScheduleName,
                              "SurfacePropertyConvectionCoefficients",
                              "Convection Coefficient 2",
                              schedule);
    return result;
  }

  void SurfacePropertyConvectionCoefficients_Impl::resetConvectionCoefficient2Schedule() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2ScheduleName, "");
    OS_ASSERT(result);
  }
  /*
  bool SurfacePropertyConvectionCoefficients_Impl::setConvectionCoefficient2UserCurve(const boost::optional<UserConvectionModels>& userConvectionModels) {
    bool result(false);
    if (userConvectionModels) {
      result = setPointer(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2UserCurveName, userConvectionModels.get().handle());
    }
    else {
      resetConvectionCoefficient2UserCurve();
      result = true;
    }
    return result;
  }

  void SurfacePropertyConvectionCoefficients_Impl::resetConvectionCoefficient2UserCurve() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2UserCurveName, "");
    OS_ASSERT(result);
  }
  */

} // detail

SurfacePropertyConvectionCoefficients::SurfacePropertyConvectionCoefficients(const Model& model)
  : ModelObject(SurfacePropertyConvectionCoefficients::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>());
}

IddObjectType SurfacePropertyConvectionCoefficients::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SurfaceProperty_ConvectionCoefficients);
}

std::vector<std::string> SurfacePropertyConvectionCoefficients::convectionCoefficient1LocationValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Location);
}

std::vector<std::string> SurfacePropertyConvectionCoefficients::convectionCoefficient1TypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Type);
}

std::vector<std::string> SurfacePropertyConvectionCoefficients::convectionCoefficient2LocationValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Location);
}

std::vector<std::string> SurfacePropertyConvectionCoefficients::convectionCoefficient2TypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Type);
}

boost::optional<PlanarSurface> SurfacePropertyConvectionCoefficients::surface() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->surface();
}

boost::optional<std::string> SurfacePropertyConvectionCoefficients::convectionCoefficient1Location() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->convectionCoefficient1Location();
}

boost::optional<std::string> SurfacePropertyConvectionCoefficients::convectionCoefficient1Type() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->convectionCoefficient1Type();
}

boost::optional<double> SurfacePropertyConvectionCoefficients::convectionCoefficient1() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->convectionCoefficient1();
}

boost::optional<Schedule> SurfacePropertyConvectionCoefficients::convectionCoefficient1Schedule() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->convectionCoefficient1Schedule();
}

//boost::optional<UserConvectionModels> SurfacePropertyConvectionCoefficients::convectionCoefficient1UserCurve() const {
//  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->convectionCoefficient1UserCurve();
//}

boost::optional<std::string> SurfacePropertyConvectionCoefficients::convectionCoefficient2Location() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->convectionCoefficient2Location();
}

boost::optional<std::string> SurfacePropertyConvectionCoefficients::convectionCoefficient2Type() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->convectionCoefficient2Type();
}

boost::optional<double> SurfacePropertyConvectionCoefficients::convectionCoefficient2() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->convectionCoefficient2();
}

boost::optional<Schedule> SurfacePropertyConvectionCoefficients::convectionCoefficient2Schedule() const {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->convectionCoefficient2Schedule();
}

//boost::optional<UserConvectionModels> SurfacePropertyConvectionCoefficients::convectionCoefficient2UserCurve() const {
//  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->convectionCoefficient2UserCurve();
//}

bool SurfacePropertyConvectionCoefficients::setSurface(const PlanarSurface& surface) {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->setSurface(surface);
}

bool SurfacePropertyConvectionCoefficients::setConvectionCoefficient1Location(const std::string& convectionCoefficient1Location) {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->setConvectionCoefficient1Location(convectionCoefficient1Location);
}

bool SurfacePropertyConvectionCoefficients::setConvectionCoefficient1Type(const std::string& convectionCoefficient1Type) {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->setConvectionCoefficient1Type(convectionCoefficient1Type);
}

bool SurfacePropertyConvectionCoefficients::setConvectionCoefficient1(double convectionCoefficient1) {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->setConvectionCoefficient1(convectionCoefficient1);
}

void SurfacePropertyConvectionCoefficients::resetConvectionCoefficient1() {
  getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->resetConvectionCoefficient1();
}

bool SurfacePropertyConvectionCoefficients::setConvectionCoefficient1Schedule(Schedule& schedule) {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->setConvectionCoefficient1Schedule(schedule);
}

void SurfacePropertyConvectionCoefficients::resetConvectionCoefficient1Schedule() {
  getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->resetConvectionCoefficient1Schedule();
}

//bool SurfacePropertyConvectionCoefficients::setConvectionCoefficient1UserCurve(const UserConvectionModels& userConvectionModels) {
//  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->setConvectionCoefficient1UserCurve(userConvectionModels);
//}

//void SurfacePropertyConvectionCoefficients::resetConvectionCoefficient1UserCurve() {
//  getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->resetConvectionCoefficient1UserCurve();
//}

bool SurfacePropertyConvectionCoefficients::setConvectionCoefficient2Location(const std::string& convectionCoefficient2Location) {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->setConvectionCoefficient2Location(convectionCoefficient2Location);
}

void SurfacePropertyConvectionCoefficients::resetConvectionCoefficient2Location() {
  getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->resetConvectionCoefficient2Location();
}

bool SurfacePropertyConvectionCoefficients::setConvectionCoefficient2Type(const std::string& convectionCoefficient2Type) {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->setConvectionCoefficient2Type(convectionCoefficient2Type);
}

void SurfacePropertyConvectionCoefficients::resetConvectionCoefficient2Type() {
  getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->resetConvectionCoefficient2Type();
}

bool SurfacePropertyConvectionCoefficients::setConvectionCoefficient2(double convectionCoefficient2) {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->setConvectionCoefficient2(convectionCoefficient2);
}

void SurfacePropertyConvectionCoefficients::resetConvectionCoefficient2() {
  getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->resetConvectionCoefficient2();
}

bool SurfacePropertyConvectionCoefficients::setConvectionCoefficient2Schedule(Schedule& schedule) {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->setConvectionCoefficient2Schedule(schedule);
}

void SurfacePropertyConvectionCoefficients::resetConvectionCoefficient2Schedule() {
  getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->resetConvectionCoefficient2Schedule();
}

//bool SurfacePropertyConvectionCoefficients::setConvectionCoefficient2UserCurve(const UserConvectionModels& userConvectionModels) {
//  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->setConvectionCoefficient2UserCurve(userConvectionModels);
//}

//void SurfacePropertyConvectionCoefficients::resetConvectionCoefficient2UserCurve() {
//  getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->resetConvectionCoefficient2UserCurve();
//}

/// @cond
SurfacePropertyConvectionCoefficients::SurfacePropertyConvectionCoefficients(std::shared_ptr<detail::SurfacePropertyConvectionCoefficients_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

