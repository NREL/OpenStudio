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

#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "ModelObject.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/core/UUID.hpp"

#include <utilities/idd/OS_Connection_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

namespace detail {

  Connection_Impl::Connection_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Connection::iddObjectType());
  }

  Connection_Impl::Connection_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                                   Model_Impl* model, 
                                   bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Connection::iddObjectType());
  }

  Connection_Impl::Connection_Impl(const Connection_Impl& other, 
                                   Model_Impl* model, 
                                   bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  // virtual destructor
  Connection_Impl::~Connection_Impl(){}


  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& Connection_Impl::outputVariableNames() const 
  { 
    static StringVector result;
    if (result.empty()) {
    }
    return result;
  }

  IddObjectType Connection_Impl::iddObjectType() const {
    return Connection::iddObjectType();
  }

  boost::optional<ModelObject>  Connection_Impl::sourceObject()
  {
    if ( boost::optional<WorkspaceObject> oCandidate = getTarget(openstudio::OS_ConnectionFields::SourceObject) ) 
      { return oCandidate->optionalCast<ModelObject>(); }
    return boost::none;
  }

  boost::optional<unsigned> Connection_Impl::sourceObjectPort()
  {
    return this->getUnsigned(openstudio::OS_ConnectionFields::OutletPort);
  }

  boost::optional<ModelObject> Connection_Impl::targetObject()
  {
    if ( boost::optional<WorkspaceObject> oCandidate = getTarget(openstudio::OS_ConnectionFields::TargetObject) ) 
      { return oCandidate->optionalCast<ModelObject>(); }
    return boost::none;
  }

  boost::optional<unsigned> Connection_Impl::targetObjectPort()
  {
    return this->getUnsigned(openstudio::OS_ConnectionFields::InletPort).get();
  }

  void Connection_Impl::setSourceObject(ModelObject object)
  {
    setPointer(openstudio::OS_ConnectionFields::SourceObject,object.handle());
  }

  void Connection_Impl::setSourceObjectPort(unsigned port)
  {
    this->setUnsigned(openstudio::OS_ConnectionFields::OutletPort,port);
  }

  void Connection_Impl::setTargetObject(ModelObject object)
  {
    setPointer(openstudio::OS_ConnectionFields::TargetObject,object.handle());
  }

  void Connection_Impl::setTargetObjectPort(unsigned port)
  {
    this->setUnsigned(openstudio::OS_ConnectionFields::InletPort,port);
  }

} // detail

Connection::Connection(const Model& model)
  : ModelObject(Connection::iddObjectType(), model, true)
{
  OS_ASSERT(getImpl<detail::Connection_Impl>());
}

Connection::Connection(std::shared_ptr<detail::Connection_Impl> p)
  : ModelObject(p)
{}

OptionalModelObject Connection::sourceObject()
{
  return getImpl<detail::Connection_Impl>()->sourceObject();
}

OptionalUnsigned Connection::sourceObjectPort()
{
  return getImpl<detail::Connection_Impl>()->sourceObjectPort();
}

OptionalModelObject Connection::targetObject()
{
  return getImpl<detail::Connection_Impl>()->targetObject();
}

OptionalUnsigned Connection::targetObjectPort()
{
  return getImpl<detail::Connection_Impl>()->targetObjectPort();
}

void Connection::setSourceObject(ModelObject object)
{
  return getImpl<detail::Connection_Impl>()->setSourceObject(object);
}

void Connection::setSourceObjectPort(unsigned port)
{
  return getImpl<detail::Connection_Impl>()->setSourceObjectPort(port);
}

void Connection::setTargetObject(ModelObject object)
{
  return getImpl<detail::Connection_Impl>()->setTargetObject(object);
}

void Connection::setTargetObjectPort(unsigned port)
{
  return getImpl<detail::Connection_Impl>()->setTargetObjectPort(port);
}

IddObjectType Connection::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Connection);
  return result;
}

} // model
} // openstudio
