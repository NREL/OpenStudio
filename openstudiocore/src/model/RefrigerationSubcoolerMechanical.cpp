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

#include "RefrigerationSubcoolerMechanical.hpp"
#include "RefrigerationSubcoolerMechanical_Impl.hpp"

#include "RefrigerationSystem.hpp"
#include "RefrigerationSystem_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/OS_Refrigeration_Subcooler_Mechanical_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationSubcoolerMechanical_Impl::RefrigerationSubcoolerMechanical_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationSubcoolerMechanical::iddObjectType());
  }

  RefrigerationSubcoolerMechanical_Impl::RefrigerationSubcoolerMechanical_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationSubcoolerMechanical::iddObjectType());
  }

  RefrigerationSubcoolerMechanical_Impl::RefrigerationSubcoolerMechanical_Impl(const RefrigerationSubcoolerMechanical_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefrigerationSubcoolerMechanical_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationSubcoolerMechanical_Impl::iddObjectType() const {
    return RefrigerationSubcoolerMechanical::iddObjectType();
  }

  ModelObject RefrigerationSubcoolerMechanical_Impl::clone(Model model) const
  {
    RefrigerationSubcoolerMechanical modelObjectClone = ModelObject_Impl::clone(model).cast<RefrigerationSubcoolerMechanical>();

    modelObjectClone.resetCapacityProvidingSystem();

    return modelObjectClone;
  }

  boost::optional<RefrigerationSystem> RefrigerationSubcoolerMechanical_Impl::capacityProvidingSystem() const {
    return getObject<ModelObject>().getModelObjectTarget<RefrigerationSystem>(OS_Refrigeration_Subcooler_MechanicalFields::CapacityProvidingSystem);
  }

  boost::optional<double> RefrigerationSubcoolerMechanical_Impl::outletControlTemperature() const {
    return getDouble(OS_Refrigeration_Subcooler_MechanicalFields::OutletControlTemperature,true);
  }

  bool RefrigerationSubcoolerMechanical_Impl::setCapacityProvidingSystem(const boost::optional<RefrigerationSystem>& refrigerationSystem) {
    bool result(false);
    if (refrigerationSystem) {
      result = setPointer(OS_Refrigeration_Subcooler_MechanicalFields::CapacityProvidingSystem, refrigerationSystem.get().handle());
    }
    else {
      resetCapacityProvidingSystem();
      result = true;
    }
    return result;
  }

  void RefrigerationSubcoolerMechanical_Impl::resetCapacityProvidingSystem() {
    bool result = setString(OS_Refrigeration_Subcooler_MechanicalFields::CapacityProvidingSystem, "");
    OS_ASSERT(result);
  }

  void RefrigerationSubcoolerMechanical_Impl::setOutletControlTemperature(boost::optional<double> outletControlTemperature) {
    bool result(false);
    if (outletControlTemperature) {
      result = setDouble(OS_Refrigeration_Subcooler_MechanicalFields::OutletControlTemperature, outletControlTemperature.get());
    }
    else {
      resetOutletControlTemperature();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationSubcoolerMechanical_Impl::resetOutletControlTemperature() {
    bool result = setString(OS_Refrigeration_Subcooler_MechanicalFields::OutletControlTemperature, "");
    OS_ASSERT(result);
  }

} // detail

RefrigerationSubcoolerMechanical::RefrigerationSubcoolerMechanical(const Model& model)
  : ModelObject(RefrigerationSubcoolerMechanical::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationSubcoolerMechanical_Impl>());

  setOutletControlTemperature(10.0);
}

IddObjectType RefrigerationSubcoolerMechanical::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_Subcooler_Mechanical);
}

boost::optional<RefrigerationSystem> RefrigerationSubcoolerMechanical::capacityProvidingSystem() const {
  return getImpl<detail::RefrigerationSubcoolerMechanical_Impl>()->capacityProvidingSystem();
}

boost::optional<double> RefrigerationSubcoolerMechanical::outletControlTemperature() const {
  return getImpl<detail::RefrigerationSubcoolerMechanical_Impl>()->outletControlTemperature();
}

bool RefrigerationSubcoolerMechanical::setCapacityProvidingSystem(const RefrigerationSystem& refrigerationSystem) {
  return getImpl<detail::RefrigerationSubcoolerMechanical_Impl>()->setCapacityProvidingSystem(refrigerationSystem);
}

void RefrigerationSubcoolerMechanical::resetCapacityProvidingSystem() {
  getImpl<detail::RefrigerationSubcoolerMechanical_Impl>()->resetCapacityProvidingSystem();
}

void RefrigerationSubcoolerMechanical::setOutletControlTemperature(double outletControlTemperature) {
  getImpl<detail::RefrigerationSubcoolerMechanical_Impl>()->setOutletControlTemperature(outletControlTemperature);
}

void RefrigerationSubcoolerMechanical::resetOutletControlTemperature() {
  getImpl<detail::RefrigerationSubcoolerMechanical_Impl>()->resetOutletControlTemperature();
}

/// @cond
RefrigerationSubcoolerMechanical::RefrigerationSubcoolerMechanical(std::shared_ptr<detail::RefrigerationSubcoolerMechanical_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

