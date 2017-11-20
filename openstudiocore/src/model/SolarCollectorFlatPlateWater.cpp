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

#include "SolarCollectorFlatPlateWater.hpp"
#include "SolarCollectorFlatPlateWater_Impl.hpp"

#include "SolarCollectorPerformanceFlatPlate.hpp"
#include "SolarCollectorPerformanceFlatPlate_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "Node.hpp"
#include "Model.hpp"

#include <utilities/idd/OS_SolarCollector_FlatPlate_Water_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SolarCollectorFlatPlateWater_Impl::SolarCollectorFlatPlateWater_Impl(const IdfObject& idfObject,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SolarCollectorFlatPlateWater::iddObjectType());
  }

  SolarCollectorFlatPlateWater_Impl::SolarCollectorFlatPlateWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SolarCollectorFlatPlateWater::iddObjectType());
  }

  SolarCollectorFlatPlateWater_Impl::SolarCollectorFlatPlateWater_Impl(const SolarCollectorFlatPlateWater_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  ModelObject SolarCollectorFlatPlateWater_Impl::clone(Model model) const
  {

    SolarCollectorFlatPlateWater result = StraightComponent_Impl::clone(model).cast<SolarCollectorFlatPlateWater>();
    result.setSolarCollectorPerformance(this->solarCollectorPerformance());

    // do not want to point to any surface after cloning
    result.resetSurface();

    return result;
  }

  std::vector<IdfObject> SolarCollectorFlatPlateWater_Impl::remove()
  {
    // DLM: will remove performance object due to parent/child relationship
    return StraightComponent_Impl::remove();
  }

  const std::vector<std::string>& SolarCollectorFlatPlateWater_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
      result.push_back("Solar Collector Incident Angle Modifier");
      result.push_back("Solar Collector Efficiency");
      result.push_back("Solar Collector Heat Transfer Rate");
      result.push_back("Solar Collector Heat Gain Rate");
      result.push_back("Solar Collector Heat Loss Rate");
      result.push_back("Solar Collector Heat Transfer Energy");
    }
    return result;
  }

  IddObjectType SolarCollectorFlatPlateWater_Impl::iddObjectType() const {
    return SolarCollectorFlatPlateWater::iddObjectType();
  }

  std::vector<ModelObject> SolarCollectorFlatPlateWater_Impl::children() const
  {
    std::vector<ModelObject> result;

    SolarCollectorPerformanceFlatPlate solarCollectorPerformance = this->solarCollectorPerformance();
    result.push_back(solarCollectorPerformance);

    return result;
  }

  unsigned SolarCollectorFlatPlateWater_Impl::inletPort()
  {
    return OS_SolarCollector_FlatPlate_WaterFields::InletNodeName;
  }

  unsigned SolarCollectorFlatPlateWater_Impl::outletPort()
  {
    return OS_SolarCollector_FlatPlate_WaterFields::OutletNodeName;
  }

  bool SolarCollectorFlatPlateWater_Impl::addToNode(Node & node)
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

  SolarCollectorPerformanceFlatPlate SolarCollectorFlatPlateWater_Impl::solarCollectorPerformance() const {
    boost::optional<SolarCollectorPerformanceFlatPlate> value = getObject<ModelObject>().getModelObjectTarget<SolarCollectorPerformanceFlatPlate>(OS_SolarCollector_FlatPlate_WaterFields::SolarCollectorPerformanceName);
    if (!value) {
      // DLM: could default construct one here?
      LOG_AND_THROW(briefDescription() << " does not have an Solar Collector Performance attached.");
    }
    return value.get();
  }

  boost::optional<PlanarSurface> SolarCollectorFlatPlateWater_Impl::surface() const {
    return getObject<ModelObject>().getModelObjectTarget<PlanarSurface>(OS_SolarCollector_FlatPlate_WaterFields::SurfaceName);
  }

  boost::optional<double> SolarCollectorFlatPlateWater_Impl::maximumFlowRate() const {
    return getDouble(OS_SolarCollector_FlatPlate_WaterFields::MaximumFlowRate,true);
  }

  bool SolarCollectorFlatPlateWater_Impl::setSolarCollectorPerformance(const SolarCollectorPerformanceFlatPlate& solarCollectorPerformanceFlatPlate) {
    ModelObject clone = solarCollectorPerformanceFlatPlate.clone(this->model());
    return setSolarCollectorPerformanceNoClone(clone.cast<SolarCollectorPerformanceFlatPlate>());
  }

  void SolarCollectorFlatPlateWater_Impl::resetSolarCollectorPerformance()
  {
    boost::optional<SolarCollectorPerformanceFlatPlate> oldPerformance = getObject<ModelObject>().getModelObjectTarget<SolarCollectorPerformanceFlatPlate>(OS_SolarCollector_FlatPlate_WaterFields::SolarCollectorPerformanceName);
    if (oldPerformance){
      oldPerformance->remove();
    }

    SolarCollectorPerformanceFlatPlate performance(this->model());
    bool ok = setSolarCollectorPerformanceNoClone(performance);
    OS_ASSERT(ok);
  }

  bool SolarCollectorFlatPlateWater_Impl::setSurface(const PlanarSurface& surface) {
    bool result(false);

    // DLM: check for existing solar collectors or photovoltaic generators?

    if (surface.optionalCast<Surface>()){
      result = setPointer(OS_SolarCollector_FlatPlate_WaterFields::SurfaceName, surface.handle());
    } else if (surface.optionalCast<ShadingSurface>()){
      result = setPointer(OS_SolarCollector_FlatPlate_WaterFields::SurfaceName, surface.handle());
    }

    return result;
  }

  void SolarCollectorFlatPlateWater_Impl::resetSurface() {
    bool result = setString(OS_SolarCollector_FlatPlate_WaterFields::SurfaceName, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorFlatPlateWater_Impl::setMaximumFlowRate(double maximumFlowRate) {
    return setDouble(OS_SolarCollector_FlatPlate_WaterFields::MaximumFlowRate, maximumFlowRate);
  }

  void SolarCollectorFlatPlateWater_Impl::resetMaximumFlowRate() {
    bool result = setString(OS_SolarCollector_FlatPlate_WaterFields::MaximumFlowRate, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorFlatPlateWater_Impl::setSolarCollectorPerformanceNoClone(const SolarCollectorPerformanceFlatPlate& solarCollectorPerformanceFlatPlate) {
    return setPointer(OS_SolarCollector_FlatPlate_WaterFields::SolarCollectorPerformanceName, solarCollectorPerformanceFlatPlate.handle());
  }

} // detail

SolarCollectorFlatPlateWater::SolarCollectorFlatPlateWater(const Model& model)
  : StraightComponent(SolarCollectorFlatPlateWater::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SolarCollectorFlatPlateWater_Impl>());

  SolarCollectorPerformanceFlatPlate performance(model);

  bool ok = true;
  ok = getImpl<detail::SolarCollectorFlatPlateWater_Impl>()->setSolarCollectorPerformanceNoClone(performance);
  OS_ASSERT(ok);

}

IddObjectType SolarCollectorFlatPlateWater::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SolarCollector_FlatPlate_Water);
}

SolarCollectorPerformanceFlatPlate SolarCollectorFlatPlateWater::solarCollectorPerformance() const {
  return getImpl<detail::SolarCollectorFlatPlateWater_Impl>()->solarCollectorPerformance();
}

boost::optional<PlanarSurface> SolarCollectorFlatPlateWater::surface() const {
  return getImpl<detail::SolarCollectorFlatPlateWater_Impl>()->surface();
}

boost::optional<double> SolarCollectorFlatPlateWater::maximumFlowRate() const {
  return getImpl<detail::SolarCollectorFlatPlateWater_Impl>()->maximumFlowRate();
}

bool SolarCollectorFlatPlateWater::setSolarCollectorPerformance(const SolarCollectorPerformanceFlatPlate& solarCollectorPerformanceFlatPlate) {
  return getImpl<detail::SolarCollectorFlatPlateWater_Impl>()->setSolarCollectorPerformance(solarCollectorPerformanceFlatPlate);
}

void SolarCollectorFlatPlateWater::resetSolarCollectorPerformance() {
  getImpl<detail::SolarCollectorFlatPlateWater_Impl>()->resetSolarCollectorPerformance();
}

bool SolarCollectorFlatPlateWater::setSurface(const PlanarSurface& surface) {
  return getImpl<detail::SolarCollectorFlatPlateWater_Impl>()->setSurface(surface);
}

void SolarCollectorFlatPlateWater::resetSurface() {
  return getImpl<detail::SolarCollectorFlatPlateWater_Impl>()->resetSurface();
}

bool SolarCollectorFlatPlateWater::setMaximumFlowRate(double maximumFlowRate) {
  return getImpl<detail::SolarCollectorFlatPlateWater_Impl>()->setMaximumFlowRate(maximumFlowRate);
}

void SolarCollectorFlatPlateWater::resetMaximumFlowRate() {
  getImpl<detail::SolarCollectorFlatPlateWater_Impl>()->resetMaximumFlowRate();
}

/// @cond
SolarCollectorFlatPlateWater::SolarCollectorFlatPlateWater(std::shared_ptr<detail::SolarCollectorFlatPlateWater_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

