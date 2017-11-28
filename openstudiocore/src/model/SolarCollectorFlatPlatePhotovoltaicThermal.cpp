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

#include "SolarCollectorFlatPlatePhotovoltaicThermal.hpp"
#include "SolarCollectorFlatPlatePhotovoltaicThermal_Impl.hpp"

#include "PlanarSurface.hpp"
#include "PlanarSurface_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"
#include "SolarCollectorPerformancePhotovoltaicThermalSimple.hpp"
#include "SolarCollectorPerformancePhotovoltaicThermalSimple_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "GeneratorPhotovoltaic.hpp"
#include "GeneratorPhotovoltaic_Impl.hpp"
#include "Model.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"

#include <utilities/idd/OS_SolarCollector_FlatPlate_PhotovoltaicThermal_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SolarCollectorFlatPlatePhotovoltaicThermal_Impl::SolarCollectorFlatPlatePhotovoltaicThermal_Impl(const IdfObject& idfObject,
                                                                                                   Model_Impl* model,
                                                                                                   bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType());
  }

  SolarCollectorFlatPlatePhotovoltaicThermal_Impl::SolarCollectorFlatPlatePhotovoltaicThermal_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                   Model_Impl* model,
                                                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType());
  }

  SolarCollectorFlatPlatePhotovoltaicThermal_Impl::SolarCollectorFlatPlatePhotovoltaicThermal_Impl(const SolarCollectorFlatPlatePhotovoltaicThermal_Impl& other,
                                                                                                   Model_Impl* model,
                                                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  ModelObject SolarCollectorFlatPlatePhotovoltaicThermal_Impl::clone(Model model) const
  {

    SolarCollectorFlatPlatePhotovoltaicThermal result = StraightComponent_Impl::clone(model).cast<SolarCollectorFlatPlatePhotovoltaicThermal>();
    SolarCollectorPerformancePhotovoltaicThermalSimple newPerformance = this->solarCollectorPerformance().clone(model).cast<SolarCollectorPerformancePhotovoltaicThermalSimple>();
    result.setPointer(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicThermalModelPerformanceName, newPerformance.handle());

    boost::optional<GeneratorPhotovoltaic> pv = this->generatorPhotovoltaic();
    if (pv){
      GeneratorPhotovoltaic newPV = pv->clone(model).cast<GeneratorPhotovoltaic>();
      result.setGeneratorPhotovoltaic(newPV);
    }

    // do not want to point to any surface after cloning
    result.resetSurface();

    return result;
  }

  std::vector<IdfObject> SolarCollectorFlatPlatePhotovoltaicThermal_Impl::remove()
  {
    // DLM: will remove performance object due to parent/child relationship
    return StraightComponent_Impl::remove();
  }

  const std::vector<std::string>& SolarCollectorFlatPlatePhotovoltaicThermal_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
      result.push_back("Generator Produced Thermal Rate");
      result.push_back("Generator Produced Thermal Energy");
      result.push_back("Generator PVT Fluid Bypass Status");
      result.push_back("Generator PVT Fluid Inlet Temperature");
      result.push_back("Generator PVT Fluid Outlet Temperature");
      result.push_back("Generator PVT Fluid Mass Flow Rate");
    }
    return result;
  }

  IddObjectType SolarCollectorFlatPlatePhotovoltaicThermal_Impl::iddObjectType() const {
    return SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType();
  }

  std::vector<ModelObject> SolarCollectorFlatPlatePhotovoltaicThermal_Impl::children() const
  {
    std::vector<ModelObject> result;

    SolarCollectorPerformancePhotovoltaicThermalSimple solarCollectorPerformance = this->solarCollectorPerformance();
    result.push_back(solarCollectorPerformance);

    return result;
  }

  unsigned SolarCollectorFlatPlatePhotovoltaicThermal_Impl::inletPort()
  {
    return OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::InletNodeName;
  }

  unsigned SolarCollectorFlatPlatePhotovoltaicThermal_Impl::outletPort()
  {
    return OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::OutletNodeName;
  }

  bool SolarCollectorFlatPlatePhotovoltaicThermal_Impl::addToNode(Node & node)
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
    } else if (boost::optional<AirLoopHVAC> airLoopHVAC = node.airLoopHVAC())
    {
      boost::optional<AirLoopHVACOutdoorAirSystem> oa = airLoopHVAC->airLoopHVACOutdoorAirSystem();
      if (oa)
      {
        if (oa->oaComponent(node.handle()))
        {
          if (StraightComponent_Impl::addToNode(node))
          {
            return true;
          }
        }
      }
    }


    return false;
  }

  SolarCollectorPerformancePhotovoltaicThermalSimple SolarCollectorFlatPlatePhotovoltaicThermal_Impl::solarCollectorPerformance() const {
    boost::optional<SolarCollectorPerformancePhotovoltaicThermalSimple> value = getObject<ModelObject>().getModelObjectTarget<SolarCollectorPerformancePhotovoltaicThermalSimple>(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicThermalModelPerformanceName);
    if (!value) {
      // DLM: could default construct one here?
      LOG_AND_THROW(briefDescription() << " does not have an Solar Collector Performance attached.");
    }
    return value.get();

  }

  boost::optional<PlanarSurface> SolarCollectorFlatPlatePhotovoltaicThermal_Impl::surface() const {
    return getObject<ModelObject>().getModelObjectTarget<PlanarSurface>(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::SurfaceName);
  }

  boost::optional<GeneratorPhotovoltaic> SolarCollectorFlatPlatePhotovoltaicThermal_Impl::generatorPhotovoltaic() const
  {
    return getObject<ModelObject>().getModelObjectTarget<GeneratorPhotovoltaic>(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicName);
  }

  boost::optional<double> SolarCollectorFlatPlatePhotovoltaicThermal_Impl::designFlowRate() const {
    return getDouble(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::DesignFlowRate,true);
  }

  bool SolarCollectorFlatPlatePhotovoltaicThermal_Impl::isDesignFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::DesignFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  bool SolarCollectorFlatPlatePhotovoltaicThermal_Impl::setSolarCollectorPerformance(const SolarCollectorPerformancePhotovoltaicThermalSimple& performance) {
    bool result(false);
    SolarCollectorPerformancePhotovoltaicThermalSimple current = this->solarCollectorPerformance();
    if (current.handle() == performance.handle()){
      return true; // no-op
    }
    ModelObject clone = performance.clone(this->model());
    result = setSolarCollectorPerformanceNoClone(clone.cast<SolarCollectorPerformancePhotovoltaicThermalSimple>());
    if (result){
      current.remove();
    } else{
      result = setSolarCollectorPerformanceNoClone(current);
      OS_ASSERT(result);
      return false;
    }
    return result;
  }

  void SolarCollectorFlatPlatePhotovoltaicThermal_Impl::resetSolarCollectorPerformance() {
    boost::optional<SolarCollectorPerformancePhotovoltaicThermalSimple> oldPerformance = getObject<ModelObject>().getModelObjectTarget<SolarCollectorPerformancePhotovoltaicThermalSimple>(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicThermalModelPerformanceName);
    if (oldPerformance){
      oldPerformance->remove();
    }

    SolarCollectorPerformancePhotovoltaicThermalSimple performance(this->model());
    bool ok = setSolarCollectorPerformanceNoClone(performance);
    OS_ASSERT(ok);
  }

  bool SolarCollectorFlatPlatePhotovoltaicThermal_Impl::setSurface(const PlanarSurface& surface) {
    bool result(false);

    // DLM: check for existing solar collectors or photovoltaic generators?

    if (surface.optionalCast<Surface>()){
      result = setPointer(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::SurfaceName, surface.handle());
    } else if (surface.optionalCast<ShadingSurface>()){
      result = setPointer(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::SurfaceName, surface.handle());
    }

    return result;
  }

  void SolarCollectorFlatPlatePhotovoltaicThermal_Impl::resetSurface() {
    bool result = setString(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::SurfaceName, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorFlatPlatePhotovoltaicThermal_Impl::setGeneratorPhotovoltaic(const GeneratorPhotovoltaic& generatorPhotovoltaic)
  {
    // DLM: should we check that these reference the same surface?
    return setPointer(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicName, generatorPhotovoltaic.handle());
  }

  void SolarCollectorFlatPlatePhotovoltaicThermal_Impl::resetGeneratorPhotovoltaic()
  {
    bool result = setString(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicName, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorFlatPlatePhotovoltaicThermal_Impl::setDesignFlowRate(double designFlowRate) {
    return setDouble(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::DesignFlowRate, designFlowRate);
  }

  void SolarCollectorFlatPlatePhotovoltaicThermal_Impl::resetDesignFlowRate() {
    bool result = setString(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::DesignFlowRate, "");
    OS_ASSERT(result);
  }

  void SolarCollectorFlatPlatePhotovoltaicThermal_Impl::autosizeDesignFlowRate() {
    bool result = setString(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::DesignFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool SolarCollectorFlatPlatePhotovoltaicThermal_Impl::setSolarCollectorPerformanceNoClone(const SolarCollectorPerformancePhotovoltaicThermalSimple& performance) {
    return setPointer(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicThermalModelPerformanceName, performance.handle());
  }

} // detail

SolarCollectorFlatPlatePhotovoltaicThermal::SolarCollectorFlatPlatePhotovoltaicThermal(const Model& model)
  : StraightComponent(SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>());

  SolarCollectorPerformancePhotovoltaicThermalSimple performance(model);

  bool ok = true;
  ok = getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->setSolarCollectorPerformanceNoClone(performance);
  OS_ASSERT(ok);
}

IddObjectType SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SolarCollector_FlatPlate_PhotovoltaicThermal);
}

SolarCollectorPerformancePhotovoltaicThermalSimple SolarCollectorFlatPlatePhotovoltaicThermal::solarCollectorPerformance() const {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->solarCollectorPerformance();
}

boost::optional<PlanarSurface> SolarCollectorFlatPlatePhotovoltaicThermal::surface() const {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->surface();
}

boost::optional<GeneratorPhotovoltaic> SolarCollectorFlatPlatePhotovoltaicThermal::generatorPhotovoltaic() const{
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->generatorPhotovoltaic();
}

boost::optional<double> SolarCollectorFlatPlatePhotovoltaicThermal::designFlowRate() const {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->designFlowRate();
}

bool SolarCollectorFlatPlatePhotovoltaicThermal::isDesignFlowRateAutosized() const {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->isDesignFlowRateAutosized();
}

bool SolarCollectorFlatPlatePhotovoltaicThermal::setSurface(const PlanarSurface& surface) {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->setSurface(surface);
}

void SolarCollectorFlatPlatePhotovoltaicThermal::resetSurface() {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->resetSurface();
}

bool SolarCollectorFlatPlatePhotovoltaicThermal::setGeneratorPhotovoltaic(const GeneratorPhotovoltaic& generatorPhotovoltaic){
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->setGeneratorPhotovoltaic(generatorPhotovoltaic);
}

void SolarCollectorFlatPlatePhotovoltaicThermal::resetGeneratorPhotovoltaic(){
  getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->resetGeneratorPhotovoltaic();
}

bool SolarCollectorFlatPlatePhotovoltaicThermal::setSolarCollectorPerformance(const SolarCollectorPerformancePhotovoltaicThermalSimple& performance) {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->setSolarCollectorPerformance(performance);
}

void SolarCollectorFlatPlatePhotovoltaicThermal::resetSolarCollectorPerformance() {
  getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->resetSolarCollectorPerformance();
}

bool SolarCollectorFlatPlatePhotovoltaicThermal::setDesignFlowRate(double designFlowRate) {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->setDesignFlowRate(designFlowRate);
}

void SolarCollectorFlatPlatePhotovoltaicThermal::resetDesignFlowRate() {
  getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->resetDesignFlowRate();
}

void SolarCollectorFlatPlatePhotovoltaicThermal::autosizeDesignFlowRate() {
  getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->autosizeDesignFlowRate();
}

/// @cond
SolarCollectorFlatPlatePhotovoltaicThermal::SolarCollectorFlatPlatePhotovoltaicThermal(std::shared_ptr<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

