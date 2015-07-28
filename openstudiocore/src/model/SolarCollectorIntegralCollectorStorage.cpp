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

#include <model/SolarCollectorIntegralCollectorStorage.hpp>
#include <model/SolarCollectorIntegralCollectorStorage_Impl.hpp>

#include <model/SolarCollectorPerformanceIntegralCollectorStorage.hpp>
#include <model/SolarCollectorPerformanceIntegralCollectorStorage_Impl.hpp>
#include <model/PlanarSurface.hpp>
#include <model/PlanarSurface_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SolarCollector_IntegralCollectorStorage_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

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

  const std::vector<std::string>& SolarCollectorIntegralCollectorStorage_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SolarCollectorIntegralCollectorStorage_Impl::iddObjectType() const {
    return SolarCollectorIntegralCollectorStorage::iddObjectType();
  }

  CollectorStoragePerformance SolarCollectorIntegralCollectorStorage_Impl::integralCollectorStorageParameters() const {
    boost::optional<CollectorStoragePerformance> value = optionalIntegralCollectorStorageParameters();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Integral Collector Storage Parameters attached.");
    }
    return value.get();
  }

  AllShadingAndHTSurf SolarCollectorIntegralCollectorStorage_Impl::surface() const {
    boost::optional<AllShadingAndHTSurf> value = optionalSurface();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Surface attached.");
    }
    return value.get();
  }

  std::string SolarCollectorIntegralCollectorStorage_Impl::bottomSurfaceBoundaryConditionsType() const {
    boost::optional<std::string> value = getString(OS_SolarCollector_IntegralCollectorStorageFields::BottomSurfaceBoundaryConditionsType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SolarCollectorIntegralCollectorStorage_Impl::isBottomSurfaceBoundaryConditionsTypeDefaulted() const {
    return isEmpty(OS_SolarCollector_IntegralCollectorStorageFields::BottomSurfaceBoundaryConditionsType);
  }

  boost::optional<std::string> SolarCollectorIntegralCollectorStorage_Impl::boundaryConditionModelName() const {
    return getString(OS_SolarCollector_IntegralCollectorStorageFields::BoundaryConditionModelName,true);
  }

  boost::optional<Connection> SolarCollectorIntegralCollectorStorage_Impl::inletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_SolarCollector_IntegralCollectorStorageFields::InletNodeName);
  }

  boost::optional<Connection> SolarCollectorIntegralCollectorStorage_Impl::outletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_SolarCollector_IntegralCollectorStorageFields::OutletNodeName);
  }

  boost::optional<double> SolarCollectorIntegralCollectorStorage_Impl::maximumFlowRate() const {
    return getDouble(OS_SolarCollector_IntegralCollectorStorageFields::MaximumFlowRate,true);
  }

  bool SolarCollectorIntegralCollectorStorage_Impl::setIntegralCollectorStorageParameters(const CollectorStoragePerformance& collectorStoragePerformance) {
    bool result = setPointer(OS_SolarCollector_IntegralCollectorStorageFields::IntegralCollectorStorageParametersName, collectorStoragePerformance.handle());
    return result;
  }

  bool SolarCollectorIntegralCollectorStorage_Impl::setSurface(const AllShadingAndHTSurf& allShadingAndHTSurf) {
    bool result = setPointer(OS_SolarCollector_IntegralCollectorStorageFields::SurfaceName, allShadingAndHTSurf.handle());
    return result;
  }

  bool SolarCollectorIntegralCollectorStorage_Impl::setBottomSurfaceBoundaryConditionsType(std::string bottomSurfaceBoundaryConditionsType) {
    bool result = setString(OS_SolarCollector_IntegralCollectorStorageFields::BottomSurfaceBoundaryConditionsType, bottomSurfaceBoundaryConditionsType);
    return result;
  }

  void SolarCollectorIntegralCollectorStorage_Impl::resetBottomSurfaceBoundaryConditionsType() {
    bool result = setString(OS_SolarCollector_IntegralCollectorStorageFields::BottomSurfaceBoundaryConditionsType, "");
    OS_ASSERT(result);
  }

  void SolarCollectorIntegralCollectorStorage_Impl::setBoundaryConditionModelName(boost::optional<std::string> boundaryConditionModelName) {
    bool result(false);
    if (boundaryConditionModelName) {
      result = setString(OS_SolarCollector_IntegralCollectorStorageFields::BoundaryConditionModelName, boundaryConditionModelName.get());
    }
    else {
      resetBoundaryConditionModelName();
      result = true;
    }
    OS_ASSERT(result);
  }

  void SolarCollectorIntegralCollectorStorage_Impl::resetBoundaryConditionModelName() {
    bool result = setString(OS_SolarCollector_IntegralCollectorStorageFields::BoundaryConditionModelName, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorIntegralCollectorStorage_Impl::setInletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_SolarCollector_IntegralCollectorStorageFields::InletNodeName, connection.get().handle());
    }
    else {
      resetInletNode();
      result = true;
    }
    return result;
  }

  void SolarCollectorIntegralCollectorStorage_Impl::resetInletNode() {
    bool result = setString(OS_SolarCollector_IntegralCollectorStorageFields::InletNodeName, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorIntegralCollectorStorage_Impl::setOutletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_SolarCollector_IntegralCollectorStorageFields::OutletNodeName, connection.get().handle());
    }
    else {
      resetOutletNode();
      result = true;
    }
    return result;
  }

  void SolarCollectorIntegralCollectorStorage_Impl::resetOutletNode() {
    bool result = setString(OS_SolarCollector_IntegralCollectorStorageFields::OutletNodeName, "");
    OS_ASSERT(result);
  }

  bool SolarCollectorIntegralCollectorStorage_Impl::setMaximumFlowRate(boost::optional<double> maximumFlowRate) {
    bool result(false);
    if (maximumFlowRate) {
      result = setDouble(OS_SolarCollector_IntegralCollectorStorageFields::MaximumFlowRate, maximumFlowRate.get());
    }
    else {
      resetMaximumFlowRate();
      result = true;
    }
    return result;
  }

  void SolarCollectorIntegralCollectorStorage_Impl::resetMaximumFlowRate() {
    bool result = setString(OS_SolarCollector_IntegralCollectorStorageFields::MaximumFlowRate, "");
    OS_ASSERT(result);
  }

  boost::optional<CollectorStoragePerformance> SolarCollectorIntegralCollectorStorage_Impl::optionalIntegralCollectorStorageParameters() const {
    return getObject<ModelObject>().getModelObjectTarget<CollectorStoragePerformance>(OS_SolarCollector_IntegralCollectorStorageFields::IntegralCollectorStorageParametersName);
  }

  boost::optional<AllShadingAndHTSurf> SolarCollectorIntegralCollectorStorage_Impl::optionalSurface() const {
    return getObject<ModelObject>().getModelObjectTarget<AllShadingAndHTSurf>(OS_SolarCollector_IntegralCollectorStorageFields::SurfaceName);
  }

} // detail

SolarCollectorIntegralCollectorStorage::SolarCollectorIntegralCollectorStorage(const Model& model)
  : StraightComponent(SolarCollectorIntegralCollectorStorage::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_SolarCollector_IntegralCollectorStorageFields::IntegralCollectorStorageParametersName
  //     OS_SolarCollector_IntegralCollectorStorageFields::SurfaceName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setIntegralCollectorStorageParameters();
  OS_ASSERT(ok);
  // ok = setSurface();
  OS_ASSERT(ok);
}

IddObjectType SolarCollectorIntegralCollectorStorage::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SolarCollector_IntegralCollectorStorage);
}

std::vector<std::string> SolarCollectorIntegralCollectorStorage::bottomSurfaceBoundaryConditionsTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SolarCollector_IntegralCollectorStorageFields::BottomSurfaceBoundaryConditionsType);
}

CollectorStoragePerformance SolarCollectorIntegralCollectorStorage::integralCollectorStorageParameters() const {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->integralCollectorStorageParameters();
}

AllShadingAndHTSurf SolarCollectorIntegralCollectorStorage::surface() const {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->surface();
}

std::string SolarCollectorIntegralCollectorStorage::bottomSurfaceBoundaryConditionsType() const {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->bottomSurfaceBoundaryConditionsType();
}

bool SolarCollectorIntegralCollectorStorage::isBottomSurfaceBoundaryConditionsTypeDefaulted() const {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->isBottomSurfaceBoundaryConditionsTypeDefaulted();
}

boost::optional<std::string> SolarCollectorIntegralCollectorStorage::boundaryConditionModelName() const {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->boundaryConditionModelName();
}

boost::optional<Connection> SolarCollectorIntegralCollectorStorage::inletNode() const {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->inletNode();
}

boost::optional<Connection> SolarCollectorIntegralCollectorStorage::outletNode() const {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->outletNode();
}

boost::optional<double> SolarCollectorIntegralCollectorStorage::maximumFlowRate() const {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->maximumFlowRate();
}

bool SolarCollectorIntegralCollectorStorage::setIntegralCollectorStorageParameters(const CollectorStoragePerformance& collectorStoragePerformance) {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->setIntegralCollectorStorageParameters(collectorStoragePerformance);
}

bool SolarCollectorIntegralCollectorStorage::setSurface(const AllShadingAndHTSurf& allShadingAndHTSurf) {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->setSurface(allShadingAndHTSurf);
}

bool SolarCollectorIntegralCollectorStorage::setBottomSurfaceBoundaryConditionsType(std::string bottomSurfaceBoundaryConditionsType) {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->setBottomSurfaceBoundaryConditionsType(bottomSurfaceBoundaryConditionsType);
}

void SolarCollectorIntegralCollectorStorage::resetBottomSurfaceBoundaryConditionsType() {
  getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->resetBottomSurfaceBoundaryConditionsType();
}

void SolarCollectorIntegralCollectorStorage::setBoundaryConditionModelName(std::string boundaryConditionModelName) {
  getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->setBoundaryConditionModelName(boundaryConditionModelName);
}

void SolarCollectorIntegralCollectorStorage::resetBoundaryConditionModelName() {
  getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->resetBoundaryConditionModelName();
}

bool SolarCollectorIntegralCollectorStorage::setInletNode(const Connection& connection) {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->setInletNode(connection);
}

void SolarCollectorIntegralCollectorStorage::resetInletNode() {
  getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->resetInletNode();
}

bool SolarCollectorIntegralCollectorStorage::setOutletNode(const Connection& connection) {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->setOutletNode(connection);
}

void SolarCollectorIntegralCollectorStorage::resetOutletNode() {
  getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->resetOutletNode();
}

bool SolarCollectorIntegralCollectorStorage::setMaximumFlowRate(double maximumFlowRate) {
  return getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->setMaximumFlowRate(maximumFlowRate);
}

void SolarCollectorIntegralCollectorStorage::resetMaximumFlowRate() {
  getImpl<detail::SolarCollectorIntegralCollectorStorage_Impl>()->resetMaximumFlowRate();
}

/// @cond
SolarCollectorIntegralCollectorStorage::SolarCollectorIntegralCollectorStorage(std::shared_ptr<detail::SolarCollectorIntegralCollectorStorage_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

