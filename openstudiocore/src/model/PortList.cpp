/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include <model/PortList.hpp>
#include <model/PortList_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/HVACComponent_Impl.hpp>
#include <utilities/idd/OS_PortList_FieldEnums.hxx>
#include <utilities/idd/OS_ThermalZone_FieldEnums.hxx>
#include <utilities/idf/WorkspaceExtensibleGroup.hpp>
#include <utilities/core/Assert.hpp>

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

unsigned PortList_Impl::portIndexForModelObject( ModelObject modelObject )
{
  for(int i = 0; i < (int)nextPortIndex(); i++)
  {
    if( this->modelObject(i) == modelObject )
    {
      return i;
    }
  }
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

void PortList_Impl::removePortForIndex(unsigned portIndex)
{
  int _nextPortIndex = nextPortIndex();
  model().disconnect(getObject<ModelObject>(),this->port(portIndex));
  //std::vector<ModelObject> modelObjects = this->modelObjects();
  for(int i = portIndex + 1; i < _nextPortIndex; i++ )
  {
    ModelObject mo = modelObject(i).get();
    unsigned port = connectedObjectPort( this->port(i) ).get();
    model().disconnect(getObject<ModelObject>(),this->port(i));
    model().connect(getObject<ModelObject>(),this->port(i-1),mo,port);
  }
}

unsigned PortList_Impl::airLoopHVACPort()
{
  std::vector<ModelObject> objects = modelObjects();

  for( std::vector<ModelObject>::iterator it = objects.begin();
       it != objects.end();
       ++it )
  {
    if( boost::optional<HVACComponent> hvacComponent = it->optionalCast<HVACComponent>() )
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

  for( std::vector<ModelObject>::iterator it = objects.begin();
       it != objects.end();
       ++it )
  {
    if( boost::optional<HVACComponent> hvacComponent = it->optionalCast<HVACComponent>() )
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
  : ModelObject(PortList::iddObjectType(),comp.model())
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

unsigned PortList::newPortAfterIndex(unsigned portIndex)
{
  return getImpl<detail::PortList_Impl>()->newPortAfterIndex(portIndex);
}

unsigned PortList::portIndexForModelObject( ModelObject modelObject )
{
  return getImpl<detail::PortList_Impl>()->portIndexForModelObject(modelObject);
}

unsigned PortList::nextPortIndex()
{
  return getImpl<detail::PortList_Impl>()->nextPortIndex();
}

void PortList::removePortForIndex(unsigned portIndex)
{
  return getImpl<detail::PortList_Impl>()->removePortForIndex(portIndex);
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
PortList::PortList(boost::shared_ptr<detail::PortList_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

