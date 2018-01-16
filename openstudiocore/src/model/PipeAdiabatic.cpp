/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
  if (result.empty())
  {
    // Not appropriate: no specific output
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
  : StraightComponent(std::move(p))
{}

IddObjectType PipeAdiabatic::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Pipe_Adiabatic);
  return result;
}

} // model
} // openstudio

