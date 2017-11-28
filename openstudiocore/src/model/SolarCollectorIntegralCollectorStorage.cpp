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

#include "SolarCollectorIntegralCollectorStorage.hpp"
#include "SolarCollectorIntegralCollectorStorage_Impl.hpp"

#include "SolarCollectorPerformanceIntegralCollectorStorage.hpp"
#include "SolarCollectorPerformanceIntegralCollectorStorage_Impl.hpp"
#include "PlanarSurface.hpp"
#include "PlanarSurface_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SolarCollector_IntegralCollectorStorage_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SolarCollectorIntegralCollectorStorage_Impl::SolarCollectorIntegralCollectorStorage_Impl(const IdfObject& idfObject,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SolarCollectorIntegralCollectorStorage::iddObjectType());
  }

  SolarCollectorIntegralCollectorStorage_Impl::SolarCollectorIntegralCollectorStorage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SolarCollectorIntegralCollectorStorage::iddObjectType());
  }

  SolarCollectorIntegralCollectorStorage_Impl::SolarCollectorIntegralCollectorStorage_Impl(const SolarCollectorIntegralCollectorStorage_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  ModelObject SolarCollectorIntegralCollectorStorage_Impl::clone(Model model) const
  {

    SolarCollectorIntegralCollectorStorage result = StraightComponent_Impl::clone(model).cast<SolarCollectorIntegralCollectorStorage>();
    result.setSolarCollectorPerformance(this->solarCollectorPerformance());

    // do not want to point to any surface after cloning
    result.resetSurface();

    return result;
  }

  std::vector<IdfObject> SolarCollectorIntegralCollectorStorage_Impl::remove()
  {
    // DLM: will remove performance object due to parent/child relationship
    return StraightComponent_Impl::remove();
  }

  const std::vector<std::string>& SolarCollectorIntegralCollectorStorage_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
      result.push_back("Solar Collector Storage Water Temperature");
      result.push_back("Solar Collector Absorber Plate Temperature");
      result.push_back("Solar Collector Overall Top Heat Loss Coefficient");
      result.push_back("Solar Collector Thermal Efficiency");
      result.push_back("Solar Collector Storage Heat Transfer Rate");
      result.push_back("Solar Collector Storage Heat Transfer Energy");
      result.push_back("Solar Collector Heat Transfer Rate");
      result.push_back("Solar Collector Heat Transfer Energy");
      result.push_back("Solar Collector Skin Heat Transfer Rate");
      result.push_back("Solar Collector Skin Heat Transfer Energy");
      result.push_back("Solar Collector Transmittance Absorptance Product");
    }
    return result;
  }

  IddObjectType SolarCollectorIntegralCollectorStorage_Impl::iddObjectType() const {
    return SolarCollectorIntegralCollectorStorage::iddObjectType();
  }

  std::vector<ModelObject> SolarCollectorIntegralCollectorStorage_Impl::children() const
  {
    std::vector<ModelObject> result;

    SolarCollectorPerformanceIntegralCollectorStorage solarCollectorPerformance = this->solarCollectorPerformance();
    result.push_back(solarCollectorPerformance);

    return result;
  }

  unsigned SolarCollectorIntegralCollectorStorage_Impl::inletPort()
  {
    return OS_SolarCollector_IntegralCollectorStorageFields::InletNodeName;
  }

  unsigned SolarCollectorIntegralCollectorStorage_Impl::outletPort()
  {
    return OS_SolarCollector_IntegralCollectorStorageFields::OutletNodeName;
  }

  bool SolarCollectorIntegralCollectorStorage_Impl::addToNode(Node & node)
  {
    if (boost::optional<PlantLoop> plantLoop = node.plantLoop())
    {
      if (plantLoop->supplyComponent(node.handle()))
      {
        if (StraightComponent_Impl::addToNode(node))
        {
          return true;
        }
      }
    }

    return false;
  }

  SolarCollectorPerformanceIntegralCollectorStorage SolarCollectorIntegralCollectorStorage_Impl::solarCollectorPerformance() const {
    boost::optional<SolarCollectorPerformanceIntegralCollectorStorage> value = getObject<ModelObject>().getModelObjectTarget<SolarCollectorPerformanceIntegralCollectorStorage>(OS_SolarCollector_IntegralCollectorStorageFields::IntegralCollectorStorageParametersName);
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have a Solar Collector Performance attached.");
    }
    return value.get();
  }

  boost::optional<PlanarSurface> SolarCollectorIntegralCollectorStorage_Impl::surface() const {
    return getObject<ModelObject>().getModelObjectTarget<PlanarSurface>(OS_SolarCollector_IntegralCollectorStorageFields::SurfaceName);
  }

  std::string SolarCollectorIntegralCollectorStorage_Impl::bottomSurfaceBoundaryConditionsType() const {
    boost::optional<std::string> value = getString(OS_SolarCollector_IntegralCollectorStorageFields::BottomSurfaceBoundaryConditionsType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorIntegralCollectorStorage_Impl::isBottomSurfaceBoundaryConditionsTypeDefaulted() const {
    return isEmpty(OS_SolarCollector_IntegralCollectorStorageFields::BottomSurfaceBoundaryConditionsType);
  }

  boost::optional<double> SolarCollectorIntegralCollectorStorage_Impl::maximumFlowRate() const {
    return getDouble(OS_SolarCollector_IntegralCollectorStorageFields::MaximumFlowRate,true);
  }

  bool SolarCollectorIntegralCollectorStorage_Impl::setSolarCollectorPerformance(const SolarCollectorPerformanceIntegralCollectorStorage& performance) {
    ModelObject clone = performance.clone(this->model());
    return setSolarCollectorPerformanceNoClone(clone.cast<SolarCollectorPerformanceIntegralCollectorStorage>());
  }

  void SolarCollectorIntegralCollectorStorage_Impl::resetSolarCollectorPerformance()
  {
    boost::optional<SolarCollectorPerformanceIntegralCollectorStorage> oldPerformance = getObject<ModelObject>().getModelObjectTarget<SolarCollectorPerformanceIntegralCollectorStorage>(OS_SolarCollector_IntegralCollectorStorageFields::IntegralCollectorStorageParametersName);
    if (oldPerformance){
      oldPerformance->remove();
    }

    SolarCollectorPerformanceIntegralCollectorStorage performance(this->model());
    bool ok = setSolarCollectorPerformanceNoClone(performance);
    OS_ASSERT(ok);
  }

  bool SolarCollectorIntegralCollectorStorage_Impl::setSurface(const PlanarSurface& surface) {
    bool result(false);

    // DLM: check for existing solar collectors or photovoltaic generators?

    if (surface.optionalCast<Surface>()){
      result = setPointer(OS_SolarCollector_IntegralCollectorStorageFields::SurfaceName, surface.handle());
    } else if (surface.optionalCast<ShadingSurface>()){
      result = setPointer(OS_SolarCollector_IntegralCollectorStorageFields::SurfaceName, surface.handle());
    }

    return result;
  }

  void SolarCollectorIntegralCollectorStorage_Impl::resetSurface() {
    bool result = setString(OS_SolarCollector_IntegralCollectorStorageFields::SurfaceName, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorIntegralCollectorStorage_Impl::setMaximumFlowRate(double maximumFlowRate) {
    return setDouble(OS_SolarCollector_IntegralCollectorStorageFields::MaximumFlowRate, maximumFlowRate);
  }

  void SolarCollectorIntegralCollectorStorage_Impl::resetMaximumFlowRate() {
    bool result = setString(OS_SolarCollector_IntegralCollectorStorageFields::MaximumFlowRate, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorIntegralCollectorStorage_Impl::setSolarCollectorPerformanceNoClone(const SolarCollectorPerformanceIntegralCollectorStorage& performance) {
    return setPointer(OS_SolarCollector_IntegralCollectorStorageFields::IntegralCollectorStorageParametersName, performance.handle());
  }


} // detail

SolarCollectorIntegralCollectorStorage::SolarCollectorIntegralCollectorStorage(const Model& model)
  : StraightComponent(SolarCollectorIntegralCollectorStorage::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>());

  SolarCollectorPerformanceIntegralCollectorStorage performance(model);

  bool ok = true;
  ok = getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->setSolarCollectorPerformanceNoClone(performance);
  OS_ASSERT(ok);
}

IddObjectType SolarCollectorIntegralCollectorStorage::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SolarCollector_IntegralCollectorStorage);
}

std::vector<std::string> SolarCollectorIntegralCollectorStorage::bottomSurfaceBoundaryConditionsTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SolarCollector_IntegralCollectorStorageFields::BottomSurfaceBoundaryConditionsType);
}

SolarCollectorPerformanceIntegralCollectorStorage SolarCollectorIntegralCollectorStorage::solarCollectorPerformance() const {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->solarCollectorPerformance();
}

boost::optional<PlanarSurface> SolarCollectorIntegralCollectorStorage::surface() const {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->surface();
}

std::string SolarCollectorIntegralCollectorStorage::bottomSurfaceBoundaryConditionsType() const {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->bottomSurfaceBoundaryConditionsType();
}

bool SolarCollectorIntegralCollectorStorage::isBottomSurfaceBoundaryConditionsTypeDefaulted() const {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->isBottomSurfaceBoundaryConditionsTypeDefaulted();
}

boost::optional<double> SolarCollectorIntegralCollectorStorage::maximumFlowRate() const {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->maximumFlowRate();
}

bool SolarCollectorIntegralCollectorStorage::setSolarCollectorPerformance(const SolarCollectorPerformanceIntegralCollectorStorage& performance) {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->setSolarCollectorPerformance(performance);
}

void SolarCollectorIntegralCollectorStorage::resetSolarCollectorPerformance() {
  getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->resetSolarCollectorPerformance();
}

bool SolarCollectorIntegralCollectorStorage::setSurface(const PlanarSurface& surface) {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->setSurface(surface);
}

void SolarCollectorIntegralCollectorStorage::resetSurface() {
  getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->resetSurface();
}

bool SolarCollectorIntegralCollectorStorage::setMaximumFlowRate(double maximumFlowRate) {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->setMaximumFlowRate(maximumFlowRate);
}

void SolarCollectorIntegralCollectorStorage::resetMaximumFlowRate() {
  getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->resetMaximumFlowRate();
}

/// @cond
SolarCollectorIntegralCollectorStorage::SolarCollectorIntegralCollectorStorage(std::shared_ptr<detail::SolarCollectorIntegralCollectorStorage_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

