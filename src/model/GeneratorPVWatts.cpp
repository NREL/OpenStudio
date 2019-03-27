/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "GeneratorPVWatts.hpp"
#include "GeneratorPVWatts_Impl.hpp"
#include "PlanarSurface.hpp"
#include "PlanarSurface_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"
#include "ElectricLoadCenterDistribution.hpp"
#include "ElectricLoadCenterDistribution_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_PVWatts_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  GeneratorPVWatts_Impl::GeneratorPVWatts_Impl(const IdfObject& idfObject,
                                               Model_Impl* model,
                                               bool keepHandle)
    : Generator_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GeneratorPVWatts::iddObjectType());
  }

  GeneratorPVWatts_Impl::GeneratorPVWatts_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle)
    : Generator_Impl(other, model, keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GeneratorPVWatts::iddObjectType());
  }

  GeneratorPVWatts_Impl::GeneratorPVWatts_Impl(const GeneratorPVWatts_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle)
    : Generator_Impl(other, model, keepHandle)
  {}

  const std::vector<std::string>& GeneratorPVWatts_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
      result.push_back("Generator Produced DC Electric Power");
      result.push_back("Generator Produced DC Electric Energy");
      result.push_back("Generator PV Cell Temperature");
      result.push_back("Generator PV Short Circuit Current");
      result.push_back("Generator PV Open Circuit Voltage");
      result.push_back("Generator PV Array Efficiency");
    }
    return result;
  }

  IddObjectType GeneratorPVWatts_Impl::iddObjectType() const {
    return GeneratorPVWatts::iddObjectType();
  }

  std::vector<ScheduleTypeKey> GeneratorPVWatts_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
    std::vector<ScheduleTypeKey> result;
    return result;
  }

  std::string GeneratorPVWatts_Impl::generatorObjectType() const {
    return "Generator:PVWatts";
  }

  boost::optional<Schedule> GeneratorPVWatts_Impl::availabilitySchedule() const {
    boost::optional<Schedule> schedule;
    return schedule;
  }

  boost::optional<double> GeneratorPVWatts_Impl::ratedElectricPowerOutput() const {
    return getDouble(OS_Generator_PVWattsFields::DCSystemCapacity);
  }

  boost::optional<double> GeneratorPVWatts_Impl::ratedThermaltoElectricalPowerRatio() const {
    return boost::none;
  }

  std::string GeneratorPVWatts_Impl::pvWattsVersion() const {
    boost::optional<std::string> value = getString(OS_Generator_PVWattsFields::PVWattsVersion,true);
    OS_ASSERT(value);
    return value.get();
  }

  double GeneratorPVWatts_Impl::dcSystemCapacity() const {
    boost::optional<double> value = getDouble(OS_Generator_PVWattsFields::DCSystemCapacity);
    OS_ASSERT(value);
    return value.get();
  }

  std::string GeneratorPVWatts_Impl::moduleType() const {
    boost::optional<std::string> value = getString(OS_Generator_PVWattsFields::ModuleType, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorPVWatts_Impl::isModuleTypeDefaulted() const {
    return isEmpty(OS_Generator_PVWattsFields::ModuleType);
  }

  std::string GeneratorPVWatts_Impl::arrayType() const {
    boost::optional<std::string> value = getString(OS_Generator_PVWattsFields::ArrayType, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorPVWatts_Impl::isArrayTypeDefaulted() const {
    return isEmpty(OS_Generator_PVWattsFields::ArrayType);
  }

  double GeneratorPVWatts_Impl::systemLosses() const {
    boost::optional<double> value = getDouble(OS_Generator_PVWattsFields::SystemLosses, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorPVWatts_Impl::isSystemLossesDefaulted() const {
    return isEmpty(OS_Generator_PVWattsFields::SystemLosses);
  }

  double GeneratorPVWatts_Impl::tiltAngle() const {
    boost::optional<double> value = getDouble(OS_Generator_PVWattsFields::TiltAngle, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorPVWatts_Impl::isTiltAngleDefaulted() const {
    return isEmpty(OS_Generator_PVWattsFields::TiltAngle);
  }

  double GeneratorPVWatts_Impl::azimuthAngle() const {
    boost::optional<double> value = getDouble(OS_Generator_PVWattsFields::AzimuthAngle, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorPVWatts_Impl::isAzimuthAngleDefaulted() const {
    return isEmpty(OS_Generator_PVWattsFields::AzimuthAngle);
  }

  boost::optional<PlanarSurface> GeneratorPVWatts_Impl::surface() const {
    return getObject<ModelObject>().getModelObjectTarget<PlanarSurface>(OS_Generator_PVWattsFields::SurfaceName);
  }

  double GeneratorPVWatts_Impl::groundCoverageRatio() const {
    boost::optional<double> value = getDouble(OS_Generator_PVWattsFields::GroundCoverageRatio, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorPVWatts_Impl::isGroundCoverageRatioDefaulted() const {
    return isEmpty(OS_Generator_PVWattsFields::GroundCoverageRatio);
  }

  bool GeneratorPVWatts_Impl::setDCSystemCapacity(double dcSystemCapacity) {
    bool result = setDouble(OS_Generator_PVWattsFields::DCSystemCapacity, dcSystemCapacity);
    return result;
  }

  bool GeneratorPVWatts_Impl::setModuleType(std::string moduleType) {
    bool result = setString(OS_Generator_PVWattsFields::ModuleType, moduleType);
    OS_ASSERT(result);
    return result;
  }

  void GeneratorPVWatts_Impl::resetModuleType() {
    bool result = setString(OS_Generator_PVWattsFields::ModuleType, "");
    OS_ASSERT(result);
  }

  bool GeneratorPVWatts_Impl::setArrayType(std::string arrayType) {
    bool result = setString(OS_Generator_PVWattsFields::ArrayType, arrayType);
    OS_ASSERT(result);
    return result;
  }

  void GeneratorPVWatts_Impl::resetArrayType() {
    bool result = setString(OS_Generator_PVWattsFields::ArrayType, "");
    OS_ASSERT(result);
  }

  bool GeneratorPVWatts_Impl::setSystemLosses(double systemLosses) {
    bool result = setDouble(OS_Generator_PVWattsFields::SystemLosses, systemLosses);
    OS_ASSERT(result);
    return result;
  }

  void GeneratorPVWatts_Impl::resetSystemLosses() {
    bool result = setString(OS_Generator_PVWattsFields::SystemLosses, "");
    OS_ASSERT(result);
  }

  bool GeneratorPVWatts_Impl::setTiltAngle(double tiltAngle) {
    bool result = true;
    result = result && setString(OS_Generator_PVWattsFields::SurfaceName, "");
    result = result && setDouble(OS_Generator_PVWattsFields::TiltAngle, tiltAngle);
    OS_ASSERT(result);
    return result;
  }

  void GeneratorPVWatts_Impl::resetTiltAngle() {
    bool result = setString(OS_Generator_PVWattsFields::TiltAngle, "");
    OS_ASSERT(result);
  }

  bool GeneratorPVWatts_Impl::setAzimuthAngle(double azimuthAngle) {
    bool result = true;
    result = result && setString(OS_Generator_PVWattsFields::SurfaceName, "");
    result = result && setDouble(OS_Generator_PVWattsFields::AzimuthAngle, azimuthAngle);
    OS_ASSERT(result);
    return result;
  }

  void GeneratorPVWatts_Impl::resetAzimuthAngle() {
    bool result = setString(OS_Generator_PVWattsFields::AzimuthAngle, "");
    OS_ASSERT(result);
  }

  bool GeneratorPVWatts_Impl::setSurface(const PlanarSurface& surface) {
    bool result = true;
    result = result && setString(OS_Generator_PVWattsFields::TiltAngle, "");
    result = result && setString(OS_Generator_PVWattsFields::AzimuthAngle, "");
    if (surface.optionalCast<Surface>()){
      result = result && setPointer(OS_Generator_PVWattsFields::SurfaceName, surface.handle());
    }
    else if (surface.optionalCast<ShadingSurface>()){
      result = result && setPointer(OS_Generator_PVWattsFields::SurfaceName, surface.handle());
    }
    return result;
  }

  void GeneratorPVWatts_Impl::resetSurface() {
    bool result = setString(OS_Generator_PVWattsFields::SurfaceName, "");
    OS_ASSERT(result);
  }

  bool GeneratorPVWatts_Impl::setGroundCoverageRatio(double groundCoverageRatio) {
    bool result = setDouble(OS_Generator_PVWattsFields::GroundCoverageRatio, groundCoverageRatio);
    OS_ASSERT(result);
    return result;
  }

  void GeneratorPVWatts_Impl::resetGroundCoverageRatio() {
    bool result = setString(OS_Generator_PVWattsFields::GroundCoverageRatio, "");
    OS_ASSERT(result);
  }

} // detail

GeneratorPVWatts::GeneratorPVWatts(const Model& model, double dcSystemCapacity)
  : Generator(GeneratorPVWatts::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GeneratorPVWatts_Impl>());

  bool ok = true;

  ok = this->setDCSystemCapacity(dcSystemCapacity);
  if (!ok){
    this->remove();
    LOG_AND_THROW("Cannot create a pvwatts generator with dc system capacity " << dcSystemCapacity);
  }
  //Add ElectricLoadCenterDistribution to get ElectricLoadCenterGenerators
  ElectricLoadCenterDistribution elcd(model);
  elcd.addGenerator(*this);
}

GeneratorPVWatts::GeneratorPVWatts(const Model& model, const PlanarSurface& surface, double dcSystemCapacity)
  : Generator(GeneratorPVWatts::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::GeneratorPVWatts_Impl>());

  bool ok = true;

  ok = this->setSurface(surface);
  if (!ok){
    this->remove();
    LOG_AND_THROW("Cannot create a pvwatts generator without surface");
  }

  ok = this->setDCSystemCapacity(dcSystemCapacity);
  if (!ok){
    this->remove();
    LOG_AND_THROW("Cannot create a pvwatts generator with dc system capacity " << dcSystemCapacity);
  }
  //Add ElectricLoadCenterDistribution to get ElectricLoadCenterGenerators
  ElectricLoadCenterDistribution elcd(model);
  elcd.addGenerator(*this);
}

IddObjectType GeneratorPVWatts::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_PVWatts);
}

std::string GeneratorPVWatts::pvWattsVersion() const {
  return getImpl<detail::GeneratorPVWatts_Impl>()->pvWattsVersion();
}

double GeneratorPVWatts::dcSystemCapacity() const {
  return getImpl<detail::GeneratorPVWatts_Impl>()->dcSystemCapacity();
}

std::string GeneratorPVWatts::moduleType() const {
  return getImpl<detail::GeneratorPVWatts_Impl>()->moduleType();
}

bool GeneratorPVWatts::isModuleTypeDefaulted() const {
  return getImpl<detail::GeneratorPVWatts_Impl>()->isModuleTypeDefaulted();
}

std::string GeneratorPVWatts::arrayType() const {
  return getImpl<detail::GeneratorPVWatts_Impl>()->arrayType();
}

bool GeneratorPVWatts::isArrayTypeDefaulted() const {
  return getImpl<detail::GeneratorPVWatts_Impl>()->isArrayTypeDefaulted();
}

double GeneratorPVWatts::systemLosses() const {
  return getImpl<detail::GeneratorPVWatts_Impl>()->systemLosses();
}

bool GeneratorPVWatts::isSystemLossesDefaulted() const {
  return getImpl<detail::GeneratorPVWatts_Impl>()->isSystemLossesDefaulted();
}

double GeneratorPVWatts::tiltAngle() const {
  return getImpl<detail::GeneratorPVWatts_Impl>()->tiltAngle();
}

bool GeneratorPVWatts::isTiltAngleDefaulted() const {
  return getImpl<detail::GeneratorPVWatts_Impl>()->isTiltAngleDefaulted();
}

double GeneratorPVWatts::azimuthAngle() const {
  return getImpl<detail::GeneratorPVWatts_Impl>()->azimuthAngle();
}

bool GeneratorPVWatts::isAzimuthAngleDefaulted() const {
  return getImpl<detail::GeneratorPVWatts_Impl>()->isAzimuthAngleDefaulted();
}

boost::optional<PlanarSurface> GeneratorPVWatts::surface() const {
  return getImpl<detail::GeneratorPVWatts_Impl>()->surface();
}

double GeneratorPVWatts::groundCoverageRatio() const {
  return getImpl<detail::GeneratorPVWatts_Impl>()->groundCoverageRatio();
}

bool GeneratorPVWatts::isGroundCoverageRatioDefaulted() const {
  return getImpl<detail::GeneratorPVWatts_Impl>()->isGroundCoverageRatioDefaulted();
}

bool GeneratorPVWatts::setDCSystemCapacity(double dcSystemCapacity) {
  return getImpl<detail::GeneratorPVWatts_Impl>()->setDCSystemCapacity(dcSystemCapacity);
}

bool GeneratorPVWatts::setModuleType(std::string moduleType) {
  return getImpl<detail::GeneratorPVWatts_Impl>()->setModuleType(moduleType);
}

void GeneratorPVWatts::resetModuleType() {
  getImpl<detail::GeneratorPVWatts_Impl>()->resetModuleType();
}

bool GeneratorPVWatts::setArrayType(std::string arrayType) {
  return getImpl<detail::GeneratorPVWatts_Impl>()->setArrayType(arrayType);
}

void GeneratorPVWatts::resetArrayType() {
  getImpl<detail::GeneratorPVWatts_Impl>()->resetArrayType();
}

bool GeneratorPVWatts::setSystemLosses(double systemLosses) {
  return getImpl<detail::GeneratorPVWatts_Impl>()->setSystemLosses(systemLosses);
}

void GeneratorPVWatts::resetSystemLosses() {
  getImpl<detail::GeneratorPVWatts_Impl>()->resetSystemLosses();
}

bool GeneratorPVWatts::setTiltAngle(double tiltAngle) {
  return getImpl<detail::GeneratorPVWatts_Impl>()->setTiltAngle(tiltAngle);
}

void GeneratorPVWatts::resetTiltAngle() {
  getImpl<detail::GeneratorPVWatts_Impl>()->resetTiltAngle();
}

bool GeneratorPVWatts::setAzimuthAngle(double azimuthAngle) {
  return getImpl<detail::GeneratorPVWatts_Impl>()->setAzimuthAngle(azimuthAngle);
}

void GeneratorPVWatts::resetAzimuthAngle() {
  getImpl<detail::GeneratorPVWatts_Impl>()->resetAzimuthAngle();
}

bool GeneratorPVWatts::setSurface(const PlanarSurface& surface) {
  return getImpl<detail::GeneratorPVWatts_Impl>()->setSurface(surface);
}

void GeneratorPVWatts::resetSurface() {
  getImpl<detail::GeneratorPVWatts_Impl>()->resetSurface();
}

bool GeneratorPVWatts::setGroundCoverageRatio(double groundCoverageRatio) {
  return getImpl<detail::GeneratorPVWatts_Impl>()->setGroundCoverageRatio(groundCoverageRatio);
}

void GeneratorPVWatts::resetGroundCoverageRatio() {
  getImpl<detail::GeneratorPVWatts_Impl>()->resetGroundCoverageRatio();
}

/// @cond
GeneratorPVWatts::GeneratorPVWatts(std::shared_ptr<detail::GeneratorPVWatts_Impl> impl)
  : Generator(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
