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

#include "DistrictHeating.hpp"
#include "DistrictHeating_Impl.hpp"

#include <utilities/idd/OS_DistrictHeating_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"
#include "../utilities/core/Assert.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  DistrictHeating_Impl::DistrictHeating_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == DistrictHeating::iddObjectType());
  }

  DistrictHeating_Impl::DistrictHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == DistrictHeating::iddObjectType());
  }

  DistrictHeating_Impl::DistrictHeating_Impl(const DistrictHeating_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& DistrictHeating_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType DistrictHeating_Impl::iddObjectType() const {
    return DistrictHeating::iddObjectType();
  }

  double DistrictHeating_Impl::nominalCapacity() const {
    boost::optional<double> value = getDouble(OS_DistrictHeatingFields::NominalCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity DistrictHeating_Impl::getNominalCapacity(bool returnIP) const {
    OptionalDouble value = nominalCapacity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_DistrictHeatingFields::NominalCapacity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  unsigned DistrictHeating_Impl::inletPort()
  {
    return OS_DistrictHeatingFields::HotWaterInletNodeName;
  }

  unsigned DistrictHeating_Impl::outletPort()
  {
    return OS_DistrictHeatingFields::HotWaterOutletNodeName;
  }
  
  bool DistrictHeating_Impl::setNominalCapacity(double nominalCapacity) {
    bool result = setDouble(OS_DistrictHeatingFields::NominalCapacity, nominalCapacity);
    return result;
  }

  bool DistrictHeating_Impl::setNominalCapacity(const Quantity& nominalCapacity) {
    OptionalDouble value = getDoubleFromQuantity(OS_DistrictHeatingFields::NominalCapacity,nominalCapacity);
    if (!value) {
      return false;
    }
    return setNominalCapacity(value.get());
  }

  openstudio::Quantity DistrictHeating_Impl::nominalCapacity_SI() const {
    return getNominalCapacity(false);
  }

  openstudio::Quantity DistrictHeating_Impl::nominalCapacity_IP() const {
    return getNominalCapacity(true);
  }

  bool DistrictHeating_Impl::addToNode(Node & node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() )
    {
      if( plant->supplyComponent(node.handle()) )
      {
        return StraightComponent_Impl::addToNode(node);
      }
    }

    return false;
  }

} // detail

DistrictHeating::DistrictHeating(const Model& model)
  : StraightComponent(DistrictHeating::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::DistrictHeating_Impl>());
}

IddObjectType DistrictHeating::iddObjectType() {
  IddObjectType result(IddObjectType::OS_DistrictHeating);
  return result;
}

double DistrictHeating::nominalCapacity() const {
  return getImpl<detail::DistrictHeating_Impl>()->nominalCapacity();
}

Quantity DistrictHeating::getNominalCapacity(bool returnIP) const {
  return getImpl<detail::DistrictHeating_Impl>()->getNominalCapacity(returnIP);
}

bool DistrictHeating::setNominalCapacity(double nominalCapacity) {
  return getImpl<detail::DistrictHeating_Impl>()->setNominalCapacity(nominalCapacity);
}

bool DistrictHeating::setNominalCapacity(const Quantity& nominalCapacity) {
  return getImpl<detail::DistrictHeating_Impl>()->setNominalCapacity(nominalCapacity);
}


/// @cond

DistrictHeating::DistrictHeating(std::shared_ptr<detail::DistrictHeating_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond


} // model

} // openstudio



