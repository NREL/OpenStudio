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

#include "PipeAdiabatic.hpp"
#include "PipeAdiabatic_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Model.hpp"
#include <utilities/idd/OS_Pipe_Adiabatic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

PipeAdiabatic_Impl::PipeAdiabatic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : StraightComponent_Impl(idfObject, model, keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == PipeAdiabatic::iddObjectType());
}

PipeAdiabatic_Impl::PipeAdiabatic_Impl(
  const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
  : StraightComponent_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == PipeAdiabatic::iddObjectType());
}

PipeAdiabatic_Impl::PipeAdiabatic_Impl(const PipeAdiabatic_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
  : StraightComponent_Impl(other,model,keepHandle)
{}

PipeAdiabatic_Impl::~PipeAdiabatic_Impl(){}

const std::vector<std::string>& PipeAdiabatic_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if (result.empty()){
  }
  return result;
}

IddObjectType PipeAdiabatic_Impl::iddObjectType() const {
  return PipeAdiabatic::iddObjectType();
}

unsigned PipeAdiabatic_Impl::inletPort()
{
  return OS_Pipe_AdiabaticFields::InletNodeName;
}

unsigned PipeAdiabatic_Impl::outletPort()
{
  return OS_Pipe_AdiabaticFields::OutletNodeName;
}

bool PipeAdiabatic_Impl::addToNode(Node & node)
{
  if( boost::optional<PlantLoop> plant = node.plantLoop() )
  {
    return StraightComponent_Impl::addToNode(node);
  }

  return false;
}

} // detail

// create a new PipeAdiabatic object in the model's workspace
PipeAdiabatic::PipeAdiabatic(const Model& model)
  : StraightComponent(PipeAdiabatic::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PipeAdiabatic_Impl>());
}

unsigned PipeAdiabatic::inletPort()
{
  return getImpl<detail::PipeAdiabatic_Impl>()->inletPort();
}

unsigned PipeAdiabatic::outletPort()
{
  return getImpl<detail::PipeAdiabatic_Impl>()->outletPort();
}

PipeAdiabatic::PipeAdiabatic(std::shared_ptr<detail::PipeAdiabatic_Impl> p)
  : StraightComponent(p)
{}

IddObjectType PipeAdiabatic::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Pipe_Adiabatic);
  return result;
}

} // model
} // openstudio

