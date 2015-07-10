/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "Duct.hpp"
#include "Duct_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Duct_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  Duct_Impl::Duct_Impl(const IdfObject& idfObject,
                       Model_Impl* model,
                       bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Duct::iddObjectType());
  }

  Duct_Impl::Duct_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                       Model_Impl* model,
                       bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Duct::iddObjectType());
  }

  Duct_Impl::Duct_Impl(const Duct_Impl& other,
                       Model_Impl* model,
                       bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& Duct_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType Duct_Impl::iddObjectType() const {
    return Duct::iddObjectType();
  }

  boost::optional<Connection> Duct_Impl::inletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_DuctFields::InletNode);
  }

  boost::optional<Connection> Duct_Impl::outletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_DuctFields::OutletNode);
  }

  unsigned Duct_Impl::inletPort()
  {
    return OS_DuctFields::InletNode;
  }

  unsigned Duct_Impl::outletPort()
  {
    return OS_DuctFields::OutletNode;
  }

  bool Duct_Impl::setInletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_DuctFields::InletNode, connection.get().handle());
    }
    else {
      resetInletNode();
      result = true;
    }
    return result;
  }

  void Duct_Impl::resetInletNode() {
    bool result = setString(OS_DuctFields::InletNode, "");
    OS_ASSERT(result);
  }

  bool Duct_Impl::setOutletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_DuctFields::OutletNode, connection.get().handle());
    }
    else {
      resetOutletNode();
      result = true;
    }
    return result;
  }

  void Duct_Impl::resetOutletNode() {
    bool result = setString(OS_DuctFields::OutletNode, "");
    OS_ASSERT(result);
  }

} // detail

Duct::Duct(const Model& model)
  : StraightComponent(Duct::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::Duct_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
}

IddObjectType Duct::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Duct);
}

boost::optional<Connection> Duct::inletNode() const {
  return getImpl<detail::Duct_Impl>()->inletNode();
}

boost::optional<Connection> Duct::outletNode() const {
  return getImpl<detail::Duct_Impl>()->outletNode();
}

bool Duct::setInletNode(const Connection& connection) {
  return getImpl<detail::Duct_Impl>()->setInletNode(connection);
}

void Duct::resetInletNode() {
  getImpl<detail::Duct_Impl>()->resetInletNode();
}

bool Duct::setOutletNode(const Connection& connection) {
  return getImpl<detail::Duct_Impl>()->setOutletNode(connection);
}

void Duct::resetOutletNode() {
  getImpl<detail::Duct_Impl>()->resetOutletNode();
}

/// @cond
Duct::Duct(std::shared_ptr<detail::Duct_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

