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

#include "PipeOutdoor.hpp"
#include "PipeOutdoor_Impl.hpp"

#include "Construction.hpp"
#include "Construction_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Pipe_Outdoor_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/units/Unit.hpp"

namespace openstudio {
namespace model {

namespace detail {

  PipeOutdoor_Impl::PipeOutdoor_Impl(const IdfObject& idfObject,
                                     Model_Impl* model,
                                     bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PipeOutdoor::iddObjectType());
  }

  PipeOutdoor_Impl::PipeOutdoor_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PipeOutdoor::iddObjectType());
  }

  PipeOutdoor_Impl::PipeOutdoor_Impl(const PipeOutdoor_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PipeOutdoor_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType PipeOutdoor_Impl::iddObjectType() const {
    return PipeOutdoor::iddObjectType();
  }

  unsigned PipeOutdoor_Impl::inletPort()
  {
    return OS_Pipe_OutdoorFields::FluidInletNode;
  }

  unsigned PipeOutdoor_Impl::outletPort()
  {
    return OS_Pipe_OutdoorFields::FluidOutletNode;
  }

  boost::optional<Construction> PipeOutdoor_Impl::construction() const {
    return getObject<ModelObject>().getModelObjectTarget<Construction>(OS_Pipe_OutdoorFields::Construction);
  }

  boost::optional<Node> PipeOutdoor_Impl::ambientTemperatureOutdoorAirNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_Pipe_OutdoorFields::AmbientTemperatureOutdoorAirNode);
  }

  double PipeOutdoor_Impl::pipeInsideDiameter() const {
    boost::optional<double> value = getDouble(OS_Pipe_OutdoorFields::PipeInsideDiameter,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PipeOutdoor_Impl::pipeLength() const {
    boost::optional<double> value = getDouble(OS_Pipe_OutdoorFields::PipeLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PipeOutdoor_Impl::setConstruction(const boost::optional<Construction>& construction) {
    bool result(false);
    if (construction) {
      result = setPointer(OS_Pipe_OutdoorFields::Construction, construction.get().handle());
    }
    else {
      resetConstruction();
      result = true;
    }
    return result;
  }

  void PipeOutdoor_Impl::resetConstruction() {
    bool result = setString(OS_Pipe_OutdoorFields::Construction, "");
    OS_ASSERT(result);
  }

  bool PipeOutdoor_Impl::setAmbientTemperatureOutdoorAirNode(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_Pipe_OutdoorFields::AmbientTemperatureOutdoorAirNode, node.get().handle());
    }
    else {
      resetAmbientTemperatureOutdoorAirNode();
      result = true;
    }
    return result;
  }

  void PipeOutdoor_Impl::resetAmbientTemperatureOutdoorAirNode() {
    bool result = setString(OS_Pipe_OutdoorFields::AmbientTemperatureOutdoorAirNode, "");
    OS_ASSERT(result);
  }

  bool PipeOutdoor_Impl::setPipeInsideDiameter(double pipeInsideDiameter) {
    bool result = setDouble(OS_Pipe_OutdoorFields::PipeInsideDiameter, pipeInsideDiameter);
    return result;
  }

  bool PipeOutdoor_Impl::setPipeLength(double pipeLength) {
    bool result = setDouble(OS_Pipe_OutdoorFields::PipeLength, pipeLength);
    return result;
  }

  bool PipeOutdoor_Impl::addToNode(Node & node)
  {
    if(node.plantLoop()) {
      return StraightComponent_Impl::addToNode(node);
    }

    return false;
  }

} // detail

PipeOutdoor::PipeOutdoor(const Model& model)
  : StraightComponent(PipeOutdoor::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PipeOutdoor_Impl>());

  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  ok = setPipeInsideDiameter(0.05);
  OS_ASSERT(ok);
   ok = setPipeLength(100.0);
  OS_ASSERT(ok);
}

IddObjectType PipeOutdoor::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Pipe_Outdoor);
}

boost::optional<Construction> PipeOutdoor::construction() const {
  return getImpl<detail::PipeOutdoor_Impl>()->construction();
}

boost::optional<Node> PipeOutdoor::ambientTemperatureOutdoorAirNode() const {
  return getImpl<detail::PipeOutdoor_Impl>()->ambientTemperatureOutdoorAirNode();
}

double PipeOutdoor::pipeInsideDiameter() const {
  return getImpl<detail::PipeOutdoor_Impl>()->pipeInsideDiameter();
}

double PipeOutdoor::pipeLength() const {
  return getImpl<detail::PipeOutdoor_Impl>()->pipeLength();
}

bool PipeOutdoor::setConstruction(const Construction& construction) {
  return getImpl<detail::PipeOutdoor_Impl>()->setConstruction(construction);
}

void PipeOutdoor::resetConstruction() {
  getImpl<detail::PipeOutdoor_Impl>()->resetConstruction();
}

bool PipeOutdoor::setAmbientTemperatureOutdoorAirNode(const Node& node) {
  return getImpl<detail::PipeOutdoor_Impl>()->setAmbientTemperatureOutdoorAirNode(node);
}

void PipeOutdoor::resetAmbientTemperatureOutdoorAirNode() {
  getImpl<detail::PipeOutdoor_Impl>()->resetAmbientTemperatureOutdoorAirNode();
}

bool PipeOutdoor::setPipeInsideDiameter(double pipeInsideDiameter) {
  return getImpl<detail::PipeOutdoor_Impl>()->setPipeInsideDiameter(pipeInsideDiameter);
}

bool PipeOutdoor::setPipeLength(double pipeLength) {
  return getImpl<detail::PipeOutdoor_Impl>()->setPipeLength(pipeLength);
}

/// @cond
PipeOutdoor::PipeOutdoor(std::shared_ptr<detail::PipeOutdoor_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

