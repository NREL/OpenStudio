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

#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include <utilities/idd/OS_PortList_FieldEnums.hxx>
#include <utilities/idd/OS_ThermalZone_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

PortList_Impl::PortList_Impl(const IdfObject& idfObject,
                             Model_Impl* model,
                             bool keepHandle)
  : ModelObject_Impl(idfObject,model,keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == PortList::iddObjectType());
}

PortList_Impl::PortList_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                             Model_Impl* model,
                             bool keepHandle)
  : ModelObject_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == PortList::iddObjectType());
}

PortList_Impl::PortList_Impl(const PortList_Impl& other,
                             Model_Impl* model,
                             bool keepHandle)
  : ModelObject_Impl(other,model,keepHandle)
{}

const std::vector<std::string>& PortList_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if (result.empty()){
  }
  return result;
}

IddObjectType PortList_Impl::iddObjectType() const {
  return PortList::iddObjectType();
}

ThermalZone PortList_Impl::thermalZone() const
{
  boost::optional<ThermalZone> result;

  //std::vector<ThermalZone> thermalZones = model().getModelObjects<ThermalZone>();

  //for( std::vector<ThermalZone>::iterator it = thermalZones.begin();
  //     it != thermalZones.end();
  //     it++ )
  //{
  //  if( boost::optional<WorkspaceObject> wo = it->getTarget(OS_ThermalZoneFields::ZoneAirInletPortList) )
  //  {
  //    if( wo->handle() == handle() )
  //    {
  //      result = it->optionalCast<ThermalZone>();

  //      break;
  //    }
  //  }

  //  if( boost::optional<WorkspaceObject> wo = it->getTarget(OS_ThermalZoneFields::ZoneAirExhaustPortList) )
  //  {
  //    if( wo->handle() == handle() )
  //    {
  //      result = it->optionalCast<ThermalZone>();

  //      break;
  //    }
  //  }
  //}

  result = hvacComponent().optionalCast<ThermalZone>();

  OS_ASSERT(result);

  return result.get();
}


std::vector<openstudio::IdfObject> PortList_Impl::remove()
{
  //std::vector<Node> nodes = this->nodes();

  //for( std::vector<Node>::iterator it = nodes.begin();
  //     it < nodes.end();
  //     it++ )
  //{
  //  removeNode(*it);
  //}

  return ModelObject_Impl::remove();
}

unsigned PortList_Impl::port(unsigned portIndex)
{
  unsigned result;
  result = numNonextensibleFields();
  result = result + portIndex;
  return result;
}

unsigned PortList_Impl::portIndex(unsigned port)
{
  unsigned result;
  result = numNonextensibleFields();
  result = port - result;
  return result;
}

unsigned PortList_Impl::nextPort()
{
  return port( nextPortIndex() );
}

boost::optional<ModelObject> PortList_Impl::modelObject(unsigned portIndex)
{
  return connectedObject( port( portIndex ) );
}

boost::optional<ModelObject> PortList_Impl::lastModelObject()
{
  std::vector<ModelObject> objects = modelObjects();
  if( objects.size() > 0 )
  {
    return OptionalModelObject(objects.back());
  }
  else
  {
    return OptionalModelObject();
  }
}

std::vector<ModelObject> PortList_Impl::modelObjects()
{
  std::vector<ModelObject> result;
  for( int i = 0; i <= int(nextPortIndex()) - 1; i++ )
  {
    result.push_back( this->modelObject( i ).get() );
  }
  return result;
}

unsigned PortList_Impl::newPortAfterIndex(unsigned portIndex)
{
  //std::vector<ModelObject> modelObjects = this->modelObjects();
  for(int i = portIndex; i < int(nextPortIndex()) - 1; i++ )
  {
    ModelObject mo = modelObject(i).get();
    unsigned port = connectedObjectPort( this->port(i) ).get();
    model().connect(getObject<ModelObject>(),this->port(i+2),mo,port);
  }

  Model _model = model();
  Node node( _model );

  _model.connect( getObject<ModelObject>(),this->port(portIndex+1),node,node.inletPort() );

  return this->port(portIndex++);
}

unsigned PortList_Impl::portIndexForModelObject( ModelObject & modelObject, bool * ok )
{
  for(unsigned i = 0; i < nextPortIndex(); i++)
  {
    if( this->modelObject(i) == modelObject )
    {
      if( ok ) { *ok = true; }
      return i;
    }
  }
  if( ok ) { *ok = false; }
  return 0;
}

unsigned PortList_Impl::nextPortIndex()
{
  unsigned i = 0;
  OptionalModelObject modelObject;
  modelObject = connectedObject( this->port(i) );
  while( modelObject )
  {
    i++;
    modelObject = connectedObject( this->port(i) );
  }
  return i;
}

void PortList_Impl::removePort(unsigned port)
{
  unsigned t_numFields = numFields();
  for(unsigned i = port + 1; i < t_numFields; i++ )
  {
    if( boost::optional<Connection> connection = getObject<ModelObject>().getModelObjectTarget<Connection>(i) )
    {
      if( boost::optional<ModelObject> targetObject = connection->targetObject() )
      {
        if( targetObject->handle() == handle() )
        {
          connection->setTargetObjectPort(i - 1);
          continue;
        }
      }

      if( boost::optional<ModelObject> sourceObject = connection->sourceObject() )
      {
        if( sourceObject->handle() == handle() )
        {
          connection->setSourceObjectPort(i - 1);
          continue;
        }
      }
    }
  }
  eraseExtensibleGroup(port - numNonextensibleFields());
}

unsigned PortList_Impl::airLoopHVACPort()
{
  std::vector<ModelObject> objects = modelObjects();

  for( const auto & elem : objects )
  {
    if( boost::optional<HVACComponent> hvacComponent = elem.optionalCast<HVACComponent>() )
    {
      if( hvacComponent->airLoopHVAC() )
      {
        return portIndexForModelObject(hvacComponent.get());
      }
    }
  }

  return nextPortIndex();
}

boost::optional<ModelObject> PortList_Impl::airLoopHVACModelObject()
{
  boost::optional<ModelObject> result;

  std::vector<ModelObject> objects = modelObjects();

  for( const auto & elem : objects )
  {
    if( boost::optional<HVACComponent> hvacComponent = elem.optionalCast<HVACComponent>() )
    {
      if( hvacComponent->airLoopHVAC() )
      {
        result = hvacComponent;
      }
    }
  }

  return result;
}

HVACComponent PortList_Impl::hvacComponent() const
{
  boost::optional<HVACComponent> result;

  result = getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_PortListFields::HVACComponent);

  OS_ASSERT(result);

  return result.get();
}

bool PortList_Impl::setHVACComponent(const HVACComponent & hvacComponent)
{
  return setPointer(OS_PortListFields::HVACComponent,hvacComponent.handle());
}

} // detail

PortList::PortList(const HVACComponent& comp)
  : ModelObject(PortList::iddObjectType(),comp.model(),true)
{
  OS_ASSERT(getImpl<detail::PortList_Impl>());

  getImpl<detail::PortList_Impl>()->setHVACComponent(comp);
}

IddObjectType PortList::iddObjectType() {
  return IddObjectType(IddObjectType::OS_PortList);
}

unsigned PortList::port(unsigned portIndex)
{
  return getImpl<detail::PortList_Impl>()->port(portIndex);
}

unsigned PortList::portIndex(unsigned port)
{
  return getImpl<detail::PortList_Impl>()->portIndex(port);
}

unsigned PortList::nextPort()
{
  return getImpl<detail::PortList_Impl>()->nextPort();
}

boost::optional<ModelObject> PortList::modelObject(unsigned portIndex)
{
  return getImpl<detail::PortList_Impl>()->modelObject(portIndex);
}

boost::optional<ModelObject> PortList::lastModelObject()
{
  return getImpl<detail::PortList_Impl>()->lastModelObject();
}

std::vector<ModelObject> PortList::modelObjects()
{
  return getImpl<detail::PortList_Impl>()->modelObjects();
}

unsigned PortList::portIndexForModelObject( ModelObject & modelObject )
{
  return getImpl<detail::PortList_Impl>()->portIndexForModelObject(modelObject);
}

unsigned PortList::nextPortIndex()
{
  return getImpl<detail::PortList_Impl>()->nextPortIndex();
}

ThermalZone PortList::thermalZone() const
{
  return getImpl<detail::PortList_Impl>()->thermalZone();
}

unsigned PortList::airLoopHVACPort()
{
  return getImpl<detail::PortList_Impl>()->airLoopHVACPort();
}

boost::optional<ModelObject> PortList::airLoopHVACModelObject()
{
  return getImpl<detail::PortList_Impl>()->airLoopHVACModelObject();
}

/// @cond
PortList::PortList(std::shared_ptr<detail::PortList_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

