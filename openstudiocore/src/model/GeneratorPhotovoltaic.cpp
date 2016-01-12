/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "GeneratorPhotovoltaic.hpp"
#include "GeneratorPhotovoltaic_Impl.hpp"

#include "Model.hpp"
#include "PlanarSurface.hpp"
#include "PlanarSurface_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"
#include "PhotovoltaicPerformance.hpp"
#include "PhotovoltaicPerformance_Impl.hpp"
#include "PhotovoltaicPerformanceSimple.hpp"
#include "PhotovoltaicPerformanceEquivalentOneDiode.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_Photovoltaic_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  GeneratorPhotovoltaic_Impl::GeneratorPhotovoltaic_Impl(const IdfObject& idfObject,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : Generator_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GeneratorPhotovoltaic::iddObjectType());
  }

  GeneratorPhotovoltaic_Impl::GeneratorPhotovoltaic_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : Generator_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GeneratorPhotovoltaic::iddObjectType());
  }

  GeneratorPhotovoltaic_Impl::GeneratorPhotovoltaic_Impl(const GeneratorPhotovoltaic_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : Generator_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GeneratorPhotovoltaic_Impl::outputVariableNames() const
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

  IddObjectType GeneratorPhotovoltaic_Impl::iddObjectType() const {
    return GeneratorPhotovoltaic::iddObjectType();
  }

  std::vector<ScheduleTypeKey> GeneratorPhotovoltaic_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Generator_PhotovoltaicFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("GeneratorPhotovoltaic","Availability"));
    }
    return result;
  }

 
  std::vector<ModelObject> GeneratorPhotovoltaic_Impl::children() const
  {
    std::vector<ModelObject> result;
    result.push_back(photovoltaicPerformance());
    return result;
  }

  std::vector<openstudio::IdfObject> GeneratorPhotovoltaic_Impl::remove()
  {
    //setString(OS_Generator_PhotovoltaicFields::ModulePerformanceName, "");
    return Generator_Impl::remove();
  }

  std::vector<IddObjectType> GeneratorPhotovoltaic_Impl::allowableChildTypes() const
  {
    return std::vector<IddObjectType>();
  }

  ModelObject GeneratorPhotovoltaic_Impl::clone(Model model) const
  {
    GeneratorPhotovoltaic result = ModelObject_Impl::clone(model).cast<GeneratorPhotovoltaic>();
    PhotovoltaicPerformance newPerformance = this->photovoltaicPerformance().clone(model).cast<PhotovoltaicPerformance>();
    result.setPointer(OS_Generator_PhotovoltaicFields::ModulePerformanceName, newPerformance.handle());

    result.resetSurface();
    return result;
  }
  
  std::string GeneratorPhotovoltaic_Impl::generatorObjectType() const
  {
    return "Generator:Photovoltaic";
  }

  boost::optional<double> GeneratorPhotovoltaic_Impl::ratedThermalToElectricalPowerRatio() const
  {
    return boost::none;
  }

  boost::optional<PlanarSurface> GeneratorPhotovoltaic_Impl::surface() const {
    return getObject<ModelObject>().getModelObjectTarget<PlanarSurface>(OS_Generator_PhotovoltaicFields::SurfaceName);
  }

  PhotovoltaicPerformance GeneratorPhotovoltaic_Impl::photovoltaicPerformance() const {
    boost::optional<PhotovoltaicPerformance> value = getObject<ModelObject>().getModelObjectTarget<PhotovoltaicPerformance>(OS_Generator_PhotovoltaicFields::ModulePerformanceName);
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an PhotovoltaicPerformance attached.");
    }
    return value.get();
  }

  std::string GeneratorPhotovoltaic_Impl::heatTransferIntegrationMode() const {
    boost::optional<std::string> value = getString(OS_Generator_PhotovoltaicFields::HeatTransferIntegrationMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorPhotovoltaic_Impl::isHeatTransferIntegrationModeDefaulted() const {
    return isEmpty(OS_Generator_PhotovoltaicFields::HeatTransferIntegrationMode);
  }

  double GeneratorPhotovoltaic_Impl::numberOfModulesInParallel() const {
    boost::optional<double> value = getDouble(OS_Generator_PhotovoltaicFields::NumberofModulesinParallel,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorPhotovoltaic_Impl::isNumberOfModulesInParallelDefaulted() const {
    return isEmpty(OS_Generator_PhotovoltaicFields::NumberofModulesinParallel);
  }

  double GeneratorPhotovoltaic_Impl::numberOfModulesInSeries() const {
    boost::optional<double> value = getDouble(OS_Generator_PhotovoltaicFields::NumberofModulesinSeries,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorPhotovoltaic_Impl::isNumberOfModulesInSeriesDefaulted() const {
    return isEmpty(OS_Generator_PhotovoltaicFields::NumberofModulesinSeries);
  }

  boost::optional<double> GeneratorPhotovoltaic_Impl::ratedElectricPowerOutput() const {
    return getDouble(OS_Generator_PhotovoltaicFields::RatedElectricPowerOutput,true);
  }

  boost::optional<Schedule> GeneratorPhotovoltaic_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Generator_PhotovoltaicFields::AvailabilityScheduleName);
  }

  bool GeneratorPhotovoltaic_Impl::setSurface(const PlanarSurface& surface) {
    bool result = false;
    if (surface.optionalCast<Surface>()){
      result = setPointer(OS_Generator_PhotovoltaicFields::SurfaceName, surface.handle());
    }else if (surface.optionalCast<ShadingSurface>()){
      result = setPointer(OS_Generator_PhotovoltaicFields::SurfaceName, surface.handle());
    }
    return result;
  }

  void GeneratorPhotovoltaic_Impl::resetSurface() {
    bool result = setString(OS_Generator_PhotovoltaicFields::SurfaceName, "");
    OS_ASSERT(result);
  }

  bool GeneratorPhotovoltaic_Impl::setHeatTransferIntegrationMode(const std::string& heatTransferIntegrationMode) {
    bool result = setString(OS_Generator_PhotovoltaicFields::HeatTransferIntegrationMode, heatTransferIntegrationMode);
    return result;
  }

  void GeneratorPhotovoltaic_Impl::resetHeatTransferIntegrationMode() {
    bool result = setString(OS_Generator_PhotovoltaicFields::HeatTransferIntegrationMode, "");
    OS_ASSERT(result);
  }

  bool GeneratorPhotovoltaic_Impl::setNumberOfModulesInParallel(double numberofModulesinParallel) {
    bool result = setDouble(OS_Generator_PhotovoltaicFields::NumberofModulesinParallel, numberofModulesinParallel);
    return result;
  }

  void GeneratorPhotovoltaic_Impl::resetNumberOfModulesInParallel() {
    bool result = setString(OS_Generator_PhotovoltaicFields::NumberofModulesinParallel, "");
    OS_ASSERT(result);
  }

  bool GeneratorPhotovoltaic_Impl::setNumberOfModulesInSeries(double numberofModulesinSeries) {
    bool result = setDouble(OS_Generator_PhotovoltaicFields::NumberofModulesinSeries, numberofModulesinSeries);
    return result;
  }

  void GeneratorPhotovoltaic_Impl::resetNumberOfModulesInSeries() {
    bool result = setString(OS_Generator_PhotovoltaicFields::NumberofModulesinSeries, "");
    OS_ASSERT(result);
  }

  void GeneratorPhotovoltaic_Impl::setRatedElectricPowerOutput(double ratedElectricPowerOutput) {
    bool result = setDouble(OS_Generator_PhotovoltaicFields::RatedElectricPowerOutput, ratedElectricPowerOutput);
    OS_ASSERT(result);
  }

  void GeneratorPhotovoltaic_Impl::resetRatedElectricPowerOutput() {
    bool result = setString(OS_Generator_PhotovoltaicFields::RatedElectricPowerOutput, "");
    OS_ASSERT(result);
  }

  bool GeneratorPhotovoltaic_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Generator_PhotovoltaicFields::AvailabilityScheduleName,
                              "GeneratorPhotovoltaic",
                              "Availability",
                              schedule);
    return result;
  }

  void GeneratorPhotovoltaic_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_Generator_PhotovoltaicFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

} // detail

GeneratorPhotovoltaic GeneratorPhotovoltaic::simple(const Model& model)
{
  PhotovoltaicPerformanceSimple performance(model);
  return GeneratorPhotovoltaic(model, performance);
}

GeneratorPhotovoltaic GeneratorPhotovoltaic::equivalentOneDiode(const Model& model)
{
  PhotovoltaicPerformanceEquivalentOneDiode performance(model);
  return GeneratorPhotovoltaic(model, performance);
}

GeneratorPhotovoltaic::GeneratorPhotovoltaic(const Model& model, const PhotovoltaicPerformance& performance)
  : Generator(GeneratorPhotovoltaic::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GeneratorPhotovoltaic_Impl>());

  bool ok = setPointer(OS_Generator_PhotovoltaicFields::ModulePerformanceName, performance.handle());
  OS_ASSERT(ok);

}

IddObjectType GeneratorPhotovoltaic::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_Photovoltaic);
}

std::vector<std::string> GeneratorPhotovoltaic::heatTransferIntegrationModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_PhotovoltaicFields::HeatTransferIntegrationMode);
}

boost::optional<PlanarSurface> GeneratorPhotovoltaic::surface() const {
  return getImpl<detail::GeneratorPhotovoltaic_Impl>()->surface();
}

PhotovoltaicPerformance GeneratorPhotovoltaic::photovoltaicPerformance() const {
  return getImpl<detail::GeneratorPhotovoltaic_Impl>()->photovoltaicPerformance();
}

std::string GeneratorPhotovoltaic::heatTransferIntegrationMode() const {
  return getImpl<detail::GeneratorPhotovoltaic_Impl>()->heatTransferIntegrationMode();
}

bool GeneratorPhotovoltaic::isHeatTransferIntegrationModeDefaulted() const {
  return getImpl<detail::GeneratorPhotovoltaic_Impl>()->isHeatTransferIntegrationModeDefaulted();
}

double GeneratorPhotovoltaic::numberOfModulesInParallel() const {
  return getImpl<detail::GeneratorPhotovoltaic_Impl>()->numberOfModulesInParallel();
}

bool GeneratorPhotovoltaic::isNumberOfModulesInParallelDefaulted() const {
  return getImpl<detail::GeneratorPhotovoltaic_Impl>()->isNumberOfModulesInParallelDefaulted();
}

double GeneratorPhotovoltaic::numberOfModulesInSeries() const {
  return getImpl<detail::GeneratorPhotovoltaic_Impl>()->numberOfModulesInSeries();
}

bool GeneratorPhotovoltaic::isNumberOfModulesInSeriesDefaulted() const {
  return getImpl<detail::GeneratorPhotovoltaic_Impl>()->isNumberOfModulesInSeriesDefaulted();
}

boost::optional<double> GeneratorPhotovoltaic::ratedElectricPowerOutput() const {
  return getImpl<detail::GeneratorPhotovoltaic_Impl>()->ratedElectricPowerOutput();
}

boost::optional<Schedule> GeneratorPhotovoltaic::availabilitySchedule() const {
  return getImpl<detail::GeneratorPhotovoltaic_Impl>()->availabilitySchedule();
}

bool GeneratorPhotovoltaic::setSurface(const PlanarSurface& surface) {
  return getImpl<detail::GeneratorPhotovoltaic_Impl>()->setSurface(surface);
}

void GeneratorPhotovoltaic::resetSurface() {
  getImpl<detail::GeneratorPhotovoltaic_Impl>()->resetSurface();
}

//bool GeneratorPhotovoltaic::setModulePerformance(const PVModules& pVModules) {
//  return getImpl<detail::GeneratorPhotovoltaic_Impl>()->setModulePerformance(pVModules);
//}

bool GeneratorPhotovoltaic::setHeatTransferIntegrationMode(std::string heatTransferIntegrationMode) {
  return getImpl<detail::GeneratorPhotovoltaic_Impl>()->setHeatTransferIntegrationMode(heatTransferIntegrationMode);
}

void GeneratorPhotovoltaic::resetHeatTransferIntegrationMode() {
  getImpl<detail::GeneratorPhotovoltaic_Impl>()->resetHeatTransferIntegrationMode();
}

bool GeneratorPhotovoltaic::setNumberOfModulesInParallel(double numberofModulesinParallel) {
  return getImpl<detail::GeneratorPhotovoltaic_Impl>()->setNumberOfModulesInParallel(numberofModulesinParallel);
}

void GeneratorPhotovoltaic::resetNumberOfModulesInParallel() {
  getImpl<detail::GeneratorPhotovoltaic_Impl>()->resetNumberOfModulesInParallel();
}

bool GeneratorPhotovoltaic::setNumberOfModulesInSeries(double numberofModulesinSeries) {
  return getImpl<detail::GeneratorPhotovoltaic_Impl>()->setNumberOfModulesInSeries(numberofModulesinSeries);
}

void GeneratorPhotovoltaic::resetNumberOfModulesInSeries() {
  getImpl<detail::GeneratorPhotovoltaic_Impl>()->resetNumberOfModulesInSeries();
}

void GeneratorPhotovoltaic::setRatedElectricPowerOutput(double ratedElectricPowerOutput) {
  getImpl<detail::GeneratorPhotovoltaic_Impl>()->setRatedElectricPowerOutput(ratedElectricPowerOutput);
}

void GeneratorPhotovoltaic::resetRatedElectricPowerOutput() {
  getImpl<detail::GeneratorPhotovoltaic_Impl>()->resetRatedElectricPowerOutput();
}

bool GeneratorPhotovoltaic::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::GeneratorPhotovoltaic_Impl>()->setAvailabilitySchedule(schedule);
}

void GeneratorPhotovoltaic::resetAvailabilitySchedule() {
  getImpl<detail::GeneratorPhotovoltaic_Impl>()->resetAvailabilitySchedule();
}

/// @cond
GeneratorPhotovoltaic::GeneratorPhotovoltaic(std::shared_ptr<detail::GeneratorPhotovoltaic_Impl> impl)
  : Generator(impl)
{}
/// @endcond

} // model
} // openstudio

