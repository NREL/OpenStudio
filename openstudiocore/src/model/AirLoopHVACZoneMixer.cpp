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
  : Mixer(p)
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
