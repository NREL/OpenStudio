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

#include "ConnectorMixer.hpp"
#include "ConnectorMixer_Impl.hpp"
#include "Node.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include <utilities/idd/OS_Connector_Mixer_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail{

  ConnectorMixer_Impl::ConnectorMixer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Mixer_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ConnectorMixer::iddObjectType());
  }

  ConnectorMixer_Impl::ConnectorMixer_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
        : Mixer_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ConnectorMixer::iddObjectType());
  }

  ConnectorMixer_Impl::ConnectorMixer_Impl(
      const ConnectorMixer_Impl& other,
      Model_Impl* model,
      bool keepHandle)
        : Mixer_Impl(other,model,keepHandle)
  {
  }

  ConnectorMixer_Impl::~ConnectorMixer_Impl(){}

  const std::vector<std::string> & ConnectorMixer_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ConnectorMixer_Impl::iddObjectType() const {
    return ConnectorMixer::iddObjectType();
  }

  std::vector<openstudio::IdfObject> ConnectorMixer_Impl::remove()
  {
    OptionalMixer self = model().getModelObject<Mixer>(handle());
    model().disconnect(*self,outletPort());
    for( int i = 0; i < int(nextBranchIndex()) - 1; i++ )
    {
      model().disconnect(*self,inletPort(i));
    }
    return ModelObject_Impl::remove();
  }

  unsigned ConnectorMixer_Impl::outletPort() const
  {
    return OS_Connector_MixerFields::OutletBranchName;
  }

  unsigned ConnectorMixer_Impl::inletPort(unsigned branchIndex) const
  {
    unsigned result;
    result = numNonextensibleFields();
    result = result + branchIndex;
    return result;
  }

  unsigned ConnectorMixer_Impl::nextInletPort() const
  {
    return inletPort( this->nextBranchIndex() );
  }

  bool ConnectorMixer_Impl::addToNode(Node & node)
  {
    return HVACComponent_Impl::addToNode( node );
  }

  ModelObject ConnectorMixer_Impl::clone(Model model) const
  {
    return HVACComponent_Impl::clone( model );
  }

}// detail

// create a new Mixer object in the model's workspace
ConnectorMixer::ConnectorMixer(const Model& model)
  : Mixer(ConnectorMixer::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ConnectorMixer_Impl>());
}

ConnectorMixer::ConnectorMixer(
    std::shared_ptr<detail::ConnectorMixer_Impl> p)
  : Mixer(p)
{}

std::vector<openstudio::IdfObject> ConnectorMixer::remove()
{
  return getImpl<detail::ConnectorMixer_Impl>()->remove();
}

unsigned ConnectorMixer::outletPort() const
{
  return getImpl<detail::ConnectorMixer_Impl>()->outletPort();
}

unsigned ConnectorMixer::inletPort(unsigned branchIndex) const
{
  return getImpl<detail::ConnectorMixer_Impl>()->inletPort(branchIndex);
}

unsigned ConnectorMixer::nextInletPort() const
{
  return getImpl<detail::ConnectorMixer_Impl>()->nextInletPort();
}

bool ConnectorMixer::addToNode(Node & node)
{
  return getImpl<detail::ConnectorMixer_Impl>()->addToNode( node );
}

ModelObject ConnectorMixer::clone(Model model) const
{
  return getImpl<detail::ConnectorMixer_Impl>()->clone( model );
}

IddObjectType ConnectorMixer::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Connector_Mixer);
  return result;
}

} // model
} // openstudio

