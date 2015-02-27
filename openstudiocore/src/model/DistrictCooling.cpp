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

#include "DistrictCooling.hpp"
#include "DistrictCooling_Impl.hpp"

#include <utilities/idd/OS_DistrictCooling_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"
#include "../utilities/core/Assert.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  DistrictCooling_Impl::DistrictCooling_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == DistrictCooling::iddObjectType());
  }

  DistrictCooling_Impl::DistrictCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == DistrictCooling::iddObjectType());
  }

  DistrictCooling_Impl::DistrictCooling_Impl(const DistrictCooling_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
  }

  const std::vector<std::string>& DistrictCooling_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType DistrictCooling_Impl::iddObjectType() const {
    return DistrictCooling::iddObjectType();
  }

  double DistrictCooling_Impl::nominalCapacity() const {
    boost::optional<double> value = getDouble(OS_DistrictCoolingFields::NominalCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity DistrictCooling_Impl::getNominalCapacity(bool returnIP) const {
    OptionalDouble value = nominalCapacity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_DistrictCoolingFields::NominalCapacity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  unsigned DistrictCooling_Impl::inletPort()
  {
    return OS_DistrictCoolingFields::ChilledWaterInletNodeName;
  }

  unsigned DistrictCooling_Impl::outletPort()
  {
    return OS_DistrictCoolingFields::ChilledWaterOutletNodeName;
  }
  
  bool DistrictCooling_Impl::setNominalCapacity(double nominalCapacity) {
    bool result = setDouble(OS_DistrictCoolingFields::NominalCapacity, nominalCapacity);
    return result;
  }

  bool DistrictCooling_Impl::setNominalCapacity(const Quantity& nominalCapacity) {
    OptionalDouble value = getDoubleFromQuantity(OS_DistrictCoolingFields::NominalCapacity,nominalCapacity);
    if (!value) {
      return false;
    }
    return setNominalCapacity(value.get());
  }

  openstudio::Quantity DistrictCooling_Impl::nominalCapacity_SI() const {
    return getNominalCapacity(false);
  }

  openstudio::Quantity DistrictCooling_Impl::nominalCapacity_IP() const {
    return getNominalCapacity(true);
  }

  bool DistrictCooling_Impl::addToNode(Node & node)
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

DistrictCooling::DistrictCooling(const Model& model)
  : StraightComponent(DistrictCooling::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::DistrictCooling_Impl>());
}

IddObjectType DistrictCooling::iddObjectType() {
  IddObjectType result(IddObjectType::OS_DistrictCooling);
  return result;
}

double DistrictCooling::nominalCapacity() const {
  return getImpl<detail::DistrictCooling_Impl>()->nominalCapacity();
}

Quantity DistrictCooling::getNominalCapacity(bool returnIP) const {
  return getImpl<detail::DistrictCooling_Impl>()->getNominalCapacity(returnIP);
}

bool DistrictCooling::setNominalCapacity(double nominalCapacity) {
  return getImpl<detail::DistrictCooling_Impl>()->setNominalCapacity(nominalCapacity);
}

bool DistrictCooling::setNominalCapacity(const Quantity& nominalCapacity) {
  return getImpl<detail::DistrictCooling_Impl>()->setNominalCapacity(nominalCapacity);
}


/// @cond

DistrictCooling::DistrictCooling(std::shared_ptr<detail::DistrictCooling_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond


} // model

} // openstudio



