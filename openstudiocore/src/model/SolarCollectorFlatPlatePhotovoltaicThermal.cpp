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

#include <model/SolarCollectorFlatPlatePhotovoltaicThermal.hpp>
#include <model/SolarCollectorFlatPlatePhotovoltaicThermal_Impl.hpp>

#include <model/PlanarSurface.hpp>
#include <model/PlanarSurface_Impl.hpp>
#include <model/SolarCollectorPerformancePhotovoltaicThermalSimple.hpp>
#include <model/SolarCollectorPerformancePhotovoltaicThermalSimple_Impl.hpp>

#include <utilities/idd/OS_SolarCollector_FlatPlate_PhotovoltaicThermal_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

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
    result.setSolarCollectorPerformance(this->solarCollectorPerformance());

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
    }
    return result;
  }

  IddObjectType SolarCollectorFlatPlatePhotovoltaicThermal_Impl::iddObjectType() const {
    return SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType();
  }

  std::vector<ModelObject> SolarCollectorFlatPlateWater_Impl::children() const
  {
    std::vector<ModelObject> result;

    SolarCollectorPerformancePhotovoltaicThermalSimple solarCollectorPerformance = this->solarCollectorPerformance();
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

  boost::optional<PlanarSurface> SolarCollectorFlatPlatePhotovoltaicThermal_Impl::surface() const {
    return getObject<ModelObject>().getModelObjectTarget<PlanarSurface>(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::SurfaceName);
  }

  boost::optional<FlatPlatePVTParameters> SolarCollectorFlatPlatePhotovoltaicThermal_Impl::photovoltaicThermalModelPerformance() const {
    return getObject<ModelObject>().getModelObjectTarget<FlatPlatePVTParameters>(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicThermalModelPerformanceName);
  }

  boost::optional<PVGenerator> SolarCollectorFlatPlatePhotovoltaicThermal_Impl::photovoltaic() const {
    return getObject<ModelObject>().getModelObjectTarget<PVGenerator>(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicName);
  }

  boost::optional<Connection> SolarCollectorFlatPlatePhotovoltaicThermal_Impl::inletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::InletNodeName);
  }

  boost::optional<Connection> SolarCollectorFlatPlatePhotovoltaicThermal_Impl::outletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::OutletNodeName);
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

  bool SolarCollectorFlatPlatePhotovoltaicThermal_Impl::setSurface(const AllShadingAndHTSurf& allShadingAndHTSurf) {
    bool result = setPointer(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::SurfaceName, allShadingAndHTSurf.handle());
    return result;
  }

  bool SolarCollectorFlatPlatePhotovoltaicThermal_Impl::setPhotovoltaicThermalModelPerformance(const boost::optional<FlatPlatePVTParameters>& flatPlatePVTParameters) {
    bool result(false);
    if (flatPlatePVTParameters) {
      result = setPointer(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicThermalModelPerformanceName, flatPlatePVTParameters.get().handle());
    }
    else {
      resetPhotovoltaicThermalModelPerformance();
      result = true;
    }
    return result;
  }

  void SolarCollectorFlatPlatePhotovoltaicThermal_Impl::resetPhotovoltaicThermalModelPerformance() {
    bool result = setString(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicThermalModelPerformanceName, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorFlatPlatePhotovoltaicThermal_Impl::setPhotovoltaic(const boost::optional<PVGenerator>& pVGenerator) {
    bool result(false);
    if (pVGenerator) {
      result = setPointer(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicName, pVGenerator.get().handle());
    }
    else {
      resetPhotovoltaic();
      result = true;
    }
    return result;
  }

  void SolarCollectorFlatPlatePhotovoltaicThermal_Impl::resetPhotovoltaic() {
    bool result = setString(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::PhotovoltaicName, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorFlatPlatePhotovoltaicThermal_Impl::setInletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::InletNodeName, connection.get().handle());
    }
    else {
      resetInletNode();
      result = true;
    }
    return result;
  }

  void SolarCollectorFlatPlatePhotovoltaicThermal_Impl::setDesignFlowRate(boost::optional<double> designFlowRate) {
    bool result(false);
    if (designFlowRate) {
      result = setDouble(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::DesignFlowRate, designFlowRate.get());
    }
    else {
      resetDesignFlowRate();
      result = true;
    }
    OS_ASSERT(result);
  }

  void SolarCollectorFlatPlatePhotovoltaicThermal_Impl::resetDesignFlowRate() {
    bool result = setString(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::DesignFlowRate, "");
    OS_ASSERT(result);
  }

  void SolarCollectorFlatPlatePhotovoltaicThermal_Impl::autosizeDesignFlowRate() {
    bool result = setString(OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::DesignFlowRate, "autosize");
    OS_ASSERT(result);
  }

} // detail

SolarCollectorFlatPlatePhotovoltaicThermal::SolarCollectorFlatPlatePhotovoltaicThermal(const Model& model)
  : StraightComponent(SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_SolarCollector_FlatPlate_PhotovoltaicThermalFields::SurfaceName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setSurface();
  OS_ASSERT(ok);
}

IddObjectType SolarCollectorFlatPlatePhotovoltaicThermal::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SolarCollector_FlatPlate_PhotovoltaicThermal);
}

AllShadingAndHTSurf SolarCollectorFlatPlatePhotovoltaicThermal::surface() const {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->surface();
}

boost::optional<FlatPlatePVTParameters> SolarCollectorFlatPlatePhotovoltaicThermal::photovoltaicThermalModelPerformance() const {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->photovoltaicThermalModelPerformance();
}

boost::optional<PVGenerator> SolarCollectorFlatPlatePhotovoltaicThermal::photovoltaic() const {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->photovoltaic();
}

boost::optional<Connection> SolarCollectorFlatPlatePhotovoltaicThermal::inletNode() const {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->inletNode();
}

boost::optional<Connection> SolarCollectorFlatPlatePhotovoltaicThermal::outletNode() const {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->outletNode();
}

boost::optional<double> SolarCollectorFlatPlatePhotovoltaicThermal::designFlowRate() const {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->designFlowRate();
}

bool SolarCollectorFlatPlatePhotovoltaicThermal::isDesignFlowRateAutosized() const {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->isDesignFlowRateAutosized();
}

bool SolarCollectorFlatPlatePhotovoltaicThermal::setSurface(const AllShadingAndHTSurf& allShadingAndHTSurf) {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->setSurface(allShadingAndHTSurf);
}

bool SolarCollectorFlatPlatePhotovoltaicThermal::setPhotovoltaicThermalModelPerformance(const FlatPlatePVTParameters& flatPlatePVTParameters) {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->setPhotovoltaicThermalModelPerformance(flatPlatePVTParameters);
}

void SolarCollectorFlatPlatePhotovoltaicThermal::resetPhotovoltaicThermalModelPerformance() {
  getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->resetPhotovoltaicThermalModelPerformance();
}

bool SolarCollectorFlatPlatePhotovoltaicThermal::setPhotovoltaic(const PVGenerator& pVGenerator) {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->setPhotovoltaic(pVGenerator);
}

void SolarCollectorFlatPlatePhotovoltaicThermal::resetPhotovoltaic() {
  getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->resetPhotovoltaic();
}

bool SolarCollectorFlatPlatePhotovoltaicThermal::setInletNode(const Connection& connection) {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->setInletNode(connection);
}

void SolarCollectorFlatPlatePhotovoltaicThermal::resetInletNode() {
  getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->resetInletNode();
}

bool SolarCollectorFlatPlatePhotovoltaicThermal::setOutletNode(const Connection& connection) {
  return getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->setOutletNode(connection);
}

void SolarCollectorFlatPlatePhotovoltaicThermal::resetOutletNode() {
  getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->resetOutletNode();
}

void SolarCollectorFlatPlatePhotovoltaicThermal::setDesignFlowRate(double designFlowRate) {
  getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->setDesignFlowRate(designFlowRate);
}

void SolarCollectorFlatPlatePhotovoltaicThermal::resetDesignFlowRate() {
  getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->resetDesignFlowRate();
}

void SolarCollectorFlatPlatePhotovoltaicThermal::autosizeDesignFlowRate() {
  getImpl<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl>()->autosizeDesignFlowRate();
}

/// @cond
SolarCollectorFlatPlatePhotovoltaicThermal::SolarCollectorFlatPlatePhotovoltaicThermal(std::shared_ptr<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

