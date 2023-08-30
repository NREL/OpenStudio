/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == Connection::iddObjectType());
    }

    Connection_Impl::Connection_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == Connection::iddObjectType());
    }

    Connection_Impl::Connection_Impl(const Connection_Impl& other, Model_Impl* model, bool keepHandle) : ModelObject_Impl(other, model, keepHandle) {}

    // virtual destructor

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& Connection_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType Connection_Impl::iddObjectType() const {
      return Connection::iddObjectType();
    }

    boost::optional<ModelObject> Connection_Impl::sourceObject() const {
      if (boost::optional<WorkspaceObject> oCandidate = getTarget(openstudio::OS_ConnectionFields::SourceObject)) {
        return oCandidate->optionalCast<ModelObject>();
      }
      return boost::none;
    }

    boost::optional<unsigned> Connection_Impl::sourceObjectPort() const {
      return this->getUnsigned(openstudio::OS_ConnectionFields::OutletPort);
    }

    boost::optional<ModelObject> Connection_Impl::targetObject() const {
      if (boost::optional<WorkspaceObject> oCandidate = getTarget(openstudio::OS_ConnectionFields::TargetObject)) {
        return oCandidate->optionalCast<ModelObject>();
      }
      return boost::none;
    }

    boost::optional<unsigned> Connection_Impl::targetObjectPort() const {
      return this->getUnsigned(openstudio::OS_ConnectionFields::InletPort).get();
    }

    bool Connection_Impl::setSourceObject(const ModelObject& object) {
      return setPointer(openstudio::OS_ConnectionFields::SourceObject, object.handle());
      ;
    }

    bool Connection_Impl::setSourceObjectPort(unsigned port) {
      return this->setUnsigned(openstudio::OS_ConnectionFields::OutletPort, port);
    }

    bool Connection_Impl::setTargetObject(const ModelObject& object) {
      return setPointer(openstudio::OS_ConnectionFields::TargetObject, object.handle());
      ;
    }

    bool Connection_Impl::setTargetObjectPort(unsigned port) {
      return this->setUnsigned(openstudio::OS_ConnectionFields::InletPort, port);
    }

  }  // namespace detail

  Connection::Connection(const Model& model)
    : ModelObject(Connection::iddObjectType(), model, false)  // No FastName
  {
    OS_ASSERT(getImpl<detail::Connection_Impl>());
  }

  Connection::Connection(std::shared_ptr<detail::Connection_Impl> impl) : ModelObject(std::move(impl)) {}

  OptionalModelObject Connection::sourceObject() const {
    return getImpl<detail::Connection_Impl>()->sourceObject();
  }

  OptionalUnsigned Connection::sourceObjectPort() const {
    return getImpl<detail::Connection_Impl>()->sourceObjectPort();
  }

  OptionalModelObject Connection::targetObject() const {
    return getImpl<detail::Connection_Impl>()->targetObject();
  }

  OptionalUnsigned Connection::targetObjectPort() const {
    return getImpl<detail::Connection_Impl>()->targetObjectPort();
  }

  bool Connection::setSourceObject(const ModelObject& object) {
    return getImpl<detail::Connection_Impl>()->setSourceObject(object);
  }

  bool Connection::setSourceObjectPort(unsigned port) {
    return getImpl<detail::Connection_Impl>()->setSourceObjectPort(port);
  }

  bool Connection::setTargetObject(const ModelObject& object) {
    return getImpl<detail::Connection_Impl>()->setTargetObject(object);
  }

  bool Connection::setTargetObjectPort(unsigned port) {
    return getImpl<detail::Connection_Impl>()->setTargetObjectPort(port);
  }

  IddObjectType Connection::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Connection);
    return result;
  }

}  // namespace model
}  // namespace openstudio
