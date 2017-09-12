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

#include "AirLoopHVACZoneMixer.hpp"
#include "AirLoopHVACZoneMixer_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "AirTerminalSingleDuctUncontrolled.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include <utilities/idd/OS_AirLoopHVAC_ZoneMixer_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirLoopHVACZoneMixer_Impl::AirLoopHVACZoneMixer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Mixer_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACZoneMixer::iddObjectType());
  }

  AirLoopHVACZoneMixer_Impl::AirLoopHVACZoneMixer_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
        : Mixer_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirLoopHVACZoneMixer::iddObjectType());
  }

  AirLoopHVACZoneMixer_Impl::AirLoopHVACZoneMixer_Impl(
      const AirLoopHVACZoneMixer_Impl& other,
      Model_Impl* model,
      bool keepHandle)
        : Mixer_Impl(other,model,keepHandle)
  {
  }

  AirLoopHVACZoneMixer_Impl::~AirLoopHVACZoneMixer_Impl(){}

  const std::vector<std::string>& AirLoopHVACZoneMixer_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirLoopHVACZoneMixer_Impl::iddObjectType() const {
    return AirLoopHVACZoneMixer::iddObjectType();
  }

  std::vector<openstudio::IdfObject> AirLoopHVACZoneMixer_Impl::remove()
  {
    if(this->airLoopHVAC())
    {
      return std::vector<openstudio::IdfObject>();
    }
    else
    {
      this->disconnect();

      return HVACComponent_Impl::remove();
    }
  }

  unsigned AirLoopHVACZoneMixer_Impl::outletPort() const
  {
    return OS_AirLoopHVAC_ZoneMixerFields::OutletNodeName;
  }

  unsigned AirLoopHVACZoneMixer_Impl::inletPort(unsigned branchIndex) const
  {
    unsigned result;
    result = numNonextensibleFields();
    result = result + branchIndex;
    return result;
  }

  unsigned AirLoopHVACZoneMixer_Impl::nextInletPort() const
  {
    return inletPort( this->nextBranchIndex() );
  }

  bool AirLoopHVACZoneMixer_Impl::addToNode(Node & node)
  {
    return HVACComponent_Impl::addToNode( node );
  }

  ModelObject AirLoopHVACZoneMixer_Impl::clone(Model model) const
  {
    return HVACComponent_Impl::clone( model );
  }

  void AirLoopHVACZoneMixer_Impl::disconnect()
  {
    ModelObject mo = this->getObject<ModelObject>();
    model().disconnect(mo,outletPort());
    for( int i = 0; i < int(nextBranchIndex()); i++ )
    {
      model().disconnect(mo,inletPort(i));
    }
  }

} // detail

// create a new AirLoopHVACZoneMixer object in the model's workspace
AirLoopHVACZoneMixer::AirLoopHVACZoneMixer(const Model& model)
  : Mixer(AirLoopHVACZoneMixer::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirLoopHVACZoneMixer_Impl>());
}

AirLoopHVACZoneMixer::AirLoopHVACZoneMixer(
    std::shared_ptr<detail::AirLoopHVACZoneMixer_Impl> p)
  : Mixer(std::move(p))
{}

std::vector<openstudio::IdfObject> AirLoopHVACZoneMixer::remove()
{
  return getImpl<detail::AirLoopHVACZoneMixer_Impl>()->remove();
}

unsigned AirLoopHVACZoneMixer::outletPort() const
{
  return getImpl<detail::AirLoopHVACZoneMixer_Impl>()->outletPort();
}

unsigned AirLoopHVACZoneMixer::inletPort(unsigned branchIndex) const
{
  return getImpl<detail::AirLoopHVACZoneMixer_Impl>()->inletPort(branchIndex);
}

unsigned AirLoopHVACZoneMixer::nextInletPort() const
{
  return getImpl<detail::AirLoopHVACZoneMixer_Impl>()->nextInletPort();
}

bool AirLoopHVACZoneMixer::addToNode(Node & node)
{
  return getImpl<detail::AirLoopHVACZoneMixer_Impl>()->addToNode( node );
}

ModelObject AirLoopHVACZoneMixer::clone(Model model) const
{
  return getImpl<detail::AirLoopHVACZoneMixer_Impl>()->clone( model );
}

void AirLoopHVACZoneMixer::disconnect()
{
  return getImpl<detail::AirLoopHVACZoneMixer_Impl>()->disconnect();
}

IddObjectType AirLoopHVACZoneMixer::iddObjectType() {
  IddObjectType result(IddObjectType::OS_AirLoopHVAC_ZoneMixer);
  return result;
}

} // model
} // openstudio
