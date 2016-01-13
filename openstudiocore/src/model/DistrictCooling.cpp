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

  boost::optional<double> DistrictCooling_Impl::nominalCapacity() const {
    return getDouble(OS_DistrictCoolingFields::NominalCapacity,true);
  }

  bool DistrictCooling_Impl::isNominalCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_DistrictCoolingFields::NominalCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  bool DistrictCooling_Impl::setNominalCapacity(boost::optional<double> nominalCapacity) {
    bool result(false);
    if (nominalCapacity) {
      result = setDouble(OS_DistrictCoolingFields::NominalCapacity, nominalCapacity.get());
    }
    return result;
  }

  void DistrictCooling_Impl::autosizeNominalCapacity() {
    bool result = setString(OS_DistrictCoolingFields::NominalCapacity, "autosize");
    OS_ASSERT(result);
  }

  unsigned DistrictCooling_Impl::inletPort()
  {
    return OS_DistrictCoolingFields::ChilledWaterInletNodeName;
  }

  unsigned DistrictCooling_Impl::outletPort()
  {
    return OS_DistrictCoolingFields::ChilledWaterOutletNodeName;
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
  autosizeNominalCapacity();
}

IddObjectType DistrictCooling::iddObjectType() {
  IddObjectType result(IddObjectType::OS_DistrictCooling);
  return result;
}

boost::optional<double> DistrictCooling::nominalCapacity() const {
  return getImpl<detail::DistrictCooling_Impl>()->nominalCapacity();
}

bool DistrictCooling::isNominalCapacityAutosized() const {
  return getImpl<detail::DistrictCooling_Impl>()->isNominalCapacityAutosized();
}

bool DistrictCooling::setNominalCapacity(double nominalCapacity) {
  return getImpl<detail::DistrictCooling_Impl>()->setNominalCapacity(nominalCapacity);
}

void DistrictCooling::autosizeNominalCapacity() {
  getImpl<detail::DistrictCooling_Impl>()->autosizeNominalCapacity();
}



/// @cond

DistrictCooling::DistrictCooling(std::shared_ptr<detail::DistrictCooling_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond


} // model

} // openstudio



