/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "SurfacePropertyConvectionCoefficients.hpp"
#include "SurfacePropertyConvectionCoefficients_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "InternalMass.hpp"
#include "InternalMass_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

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

  ModelObject SurfacePropertyConvectionCoefficients_Impl::clone(Model model) const
  {
    ModelObject result = ModelObject_Impl::clone(model);

    Model m = this->model();
    if (model == m) {
      // cloned into same model, erase reference to parent
      // this object is now invalid but having two objects point to same surface would also be invalid
      result.setString(OS_SurfaceProperty_ConvectionCoefficientsFields::SurfaceName, "");
    }

    return result;
  }

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

  bool SurfacePropertyConvectionCoefficients_Impl::setSurface(const ModelObject &surface) {
    boost::optional<SurfacePropertyConvectionCoefficients> currentCoef;
    OptionalSurface surfaceSurface = surface.optionalCast<Surface>();
    OptionalSubSurface surfaceSubSurface = surface.optionalCast<SubSurface>();
    OptionalInternalMass surfaceInternalMass = surface.optionalCast<InternalMass>();
    if (surfaceSurface) {
      currentCoef = surfaceSurface->surfacePropertyConvectionCoefficients();
    } else if (surfaceSubSurface) {
      currentCoef = surfaceSubSurface->surfacePropertyConvectionCoefficients();
    } else {
      OS_ASSERT(surfaceInternalMass);
      currentCoef = surfaceInternalMass->surfacePropertyConvectionCoefficients();
    }
    if (currentCoef) {
      if (currentCoef->handle() == this->handle()) {
        return true;
      }
      currentCoef->remove();
    }
    return setPointer(OS_SurfaceProperty_ConvectionCoefficientsFields::SurfaceName, surface.handle());
  }

  ModelObject SurfacePropertyConvectionCoefficients_Impl::surfaceAsModelObject() const {
    boost::optional<ModelObject> surfaceObj;
    surfaceObj = getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_SurfaceProperty_ConvectionCoefficientsFields::SurfaceName);
    OS_ASSERT(surfaceObj);
    return *surfaceObj;
  }

  boost::optional<Surface> SurfacePropertyConvectionCoefficients_Impl::surfaceAsSurface() const {
    return getObject<ModelObject>().getModelObjectTarget<Surface>(OS_SurfaceProperty_ConvectionCoefficientsFields::SurfaceName);
  }

  boost::optional<SubSurface> SurfacePropertyConvectionCoefficients_Impl::surfaceAsSubSurface() const {
    return getObject<ModelObject>().getModelObjectTarget<SubSurface>(OS_SurfaceProperty_ConvectionCoefficientsFields::SurfaceName);
  }

  boost::optional<InternalMass> SurfacePropertyConvectionCoefficients_Impl::surfaceAsInternalMass() const {
    return getObject<ModelObject>().getModelObjectTarget<InternalMass>(OS_SurfaceProperty_ConvectionCoefficientsFields::SurfaceName);
  }

  boost::optional<std::string> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient1Location() const {
    return getString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Location,true,true);
  }

  boost::optional<std::string> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient1Type() const {
    return getString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Type,true,true);
  }

  boost::optional<double> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient1() const {
    return getDouble(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1,true);
  }

  boost::optional<Schedule> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient1Schedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1ScheduleName);
  }

  boost::optional<std::string> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient2Location() const {
    return getString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Location,true,true);
  }

  boost::optional<std::string> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient2Type() const {
    return getString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Type,true,true);
  }

  boost::optional<double> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient2() const {
    return getDouble(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2,true);
  }

  boost::optional<Schedule> SurfacePropertyConvectionCoefficients_Impl::convectionCoefficient2Schedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2ScheduleName);
  }

  bool SurfacePropertyConvectionCoefficients_Impl::setConvectionCoefficient1Location(const std::string& convectionCoefficient1Location) {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Location, convectionCoefficient1Location);
    return result;
  }

  void SurfacePropertyConvectionCoefficients_Impl::resetConvectionCoefficient1Location() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Location, "");
    OS_ASSERT(result);
  }


  bool SurfacePropertyConvectionCoefficients_Impl::setConvectionCoefficient1Type(const std::string& convectionCoefficient1Type) {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Type, convectionCoefficient1Type);
    return result;
  }

  void SurfacePropertyConvectionCoefficients_Impl::resetConvectionCoefficient1Type() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Type, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyConvectionCoefficients_Impl::setConvectionCoefficient1(double convectionCoefficient1) {
    return setDouble(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1, convectionCoefficient1);
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

  bool SurfacePropertyConvectionCoefficients_Impl::setConvectionCoefficient2Location(const std::string& convectionCoefficient2Location) {
    return setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Location, convectionCoefficient2Location);
  }

  void SurfacePropertyConvectionCoefficients_Impl::resetConvectionCoefficient2Location() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Location, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyConvectionCoefficients_Impl::setConvectionCoefficient2Type(const std::string& convectionCoefficient2Type) {
    return setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Type, convectionCoefficient2Type);
  }

  void SurfacePropertyConvectionCoefficients_Impl::resetConvectionCoefficient2Type() {
    bool result = setString(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Type, "");
    OS_ASSERT(result);
  }

  bool SurfacePropertyConvectionCoefficients_Impl::setConvectionCoefficient2(double convectionCoefficient2) {
    return setDouble(OS_SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2, convectionCoefficient2);
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

} // detail

SurfacePropertyConvectionCoefficients::SurfacePropertyConvectionCoefficients(const Surface& surface)
  : ModelObject(SurfacePropertyConvectionCoefficients::iddObjectType(),surface.model())
{
  OS_ASSERT(getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>());
  setSurface(surface);
}

SurfacePropertyConvectionCoefficients::SurfacePropertyConvectionCoefficients(const SubSurface& surface)
  : ModelObject(SurfacePropertyConvectionCoefficients::iddObjectType(),surface.model())
{
  OS_ASSERT(getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>());
  setSurface(surface);
}

SurfacePropertyConvectionCoefficients::SurfacePropertyConvectionCoefficients(const InternalMass& surface)
  : ModelObject(SurfacePropertyConvectionCoefficients::iddObjectType(),surface.model())
{
  OS_ASSERT(getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>());
  setSurface(surface);
}

IddObjectType SurfacePropertyConvectionCoefficients::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SurfaceProperty_ConvectionCoefficients);
}

ModelObject SurfacePropertyConvectionCoefficients::surfaceAsModelObject() const {
  return getImpl<ImplType>()->surfaceAsModelObject();
}

boost::optional<Surface> SurfacePropertyConvectionCoefficients::surfaceAsSurface() const {
  return getImpl<ImplType>()->surfaceAsSurface();
}

boost::optional<SubSurface> SurfacePropertyConvectionCoefficients::surfaceAsSubSurface() const {
  return getImpl<ImplType>()->surfaceAsSubSurface();
}

boost::optional<InternalMass> SurfacePropertyConvectionCoefficients::surfaceAsInternalMass() const {
  return getImpl<ImplType>()->surfaceAsInternalMass();
}

bool SurfacePropertyConvectionCoefficients::setSurface(const ModelObject& surface) {
  return getImpl<ImplType>()->setSurface(surface);
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

bool SurfacePropertyConvectionCoefficients::setConvectionCoefficient1Location(const std::string& convectionCoefficient1Location) {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->setConvectionCoefficient1Location(convectionCoefficient1Location);
}

void SurfacePropertyConvectionCoefficients::resetConvectionCoefficient1Location() {
  getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->resetConvectionCoefficient1Location();
}

bool SurfacePropertyConvectionCoefficients::setConvectionCoefficient1Type(const std::string& convectionCoefficient1Type) {
  return getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->setConvectionCoefficient1Type(convectionCoefficient1Type);
}

void SurfacePropertyConvectionCoefficients::resetConvectionCoefficient1Type() {
  getImpl<detail::SurfacePropertyConvectionCoefficients_Impl>()->resetConvectionCoefficient1Type();
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

/// @cond
SurfacePropertyConvectionCoefficients::SurfacePropertyConvectionCoefficients(std::shared_ptr<detail::SurfacePropertyConvectionCoefficients_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

