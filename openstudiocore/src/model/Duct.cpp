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

#include "Duct.hpp"
#include "Duct_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"

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

  unsigned Duct_Impl::inletPort()
  {
    return OS_DuctFields::InletNode;
  }

  unsigned Duct_Impl::outletPort()
  {
    return OS_DuctFields::OutletNode;
  }

  bool Duct_Impl::addToNode(Node & node)
  {
    if(node.airLoopHVAC()) {
      return StraightComponent_Impl::addToNode(node);
    }

    return false;
  }

} // detail

Duct::Duct(const Model& model)
  : StraightComponent(Duct::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::Duct_Impl>());

  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
}

IddObjectType Duct::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Duct);
}

/// @cond
Duct::Duct(std::shared_ptr<detail::Duct_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

