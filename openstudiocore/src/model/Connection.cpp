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
  : ModelObject(std::move(p))
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
