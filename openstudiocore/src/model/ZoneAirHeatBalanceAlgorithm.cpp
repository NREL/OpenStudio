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

#include "ZoneAirHeatBalanceAlgorithm.hpp"
#include "ZoneAirHeatBalanceAlgorithm_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_ZoneAirHeatBalanceAlgorithm_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

namespace detail {

  ZoneAirHeatBalanceAlgorithm_Impl::ZoneAirHeatBalanceAlgorithm_Impl(const IdfObject& idfObject,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneAirHeatBalanceAlgorithm::iddObjectType());
  }

  ZoneAirHeatBalanceAlgorithm_Impl::ZoneAirHeatBalanceAlgorithm_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneAirHeatBalanceAlgorithm::iddObjectType());
  }

  ZoneAirHeatBalanceAlgorithm_Impl::ZoneAirHeatBalanceAlgorithm_Impl(const ZoneAirHeatBalanceAlgorithm_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}


  // return the parent object in the hierarchy
  boost::optional<ParentObject> ZoneAirHeatBalanceAlgorithm_Impl::parent() const
  {
    boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
    return result;
  }

  // set the parent, child may have to call methods on the parent
  bool ZoneAirHeatBalanceAlgorithm_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
      return true;
    }
    return false;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& ZoneAirHeatBalanceAlgorithm_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneAirHeatBalanceAlgorithm_Impl::iddObjectType() const {
    return ZoneAirHeatBalanceAlgorithm::iddObjectType();
  }

  std::string ZoneAirHeatBalanceAlgorithm_Impl::algorithm() const {
    boost::optional<std::string> value = getString(OS_ZoneAirHeatBalanceAlgorithmFields::Algorithm,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneAirHeatBalanceAlgorithm_Impl::isAlgorithmDefaulted() const {
    return isEmpty(OS_ZoneAirHeatBalanceAlgorithmFields::Algorithm);
  }

  bool ZoneAirHeatBalanceAlgorithm_Impl::setAlgorithm(std::string algorithm) {
    bool result = setString(OS_ZoneAirHeatBalanceAlgorithmFields::Algorithm, algorithm);
    return result;
  }

  void ZoneAirHeatBalanceAlgorithm_Impl::resetAlgorithm() {
    bool result = setString(OS_ZoneAirHeatBalanceAlgorithmFields::Algorithm, "");
    OS_ASSERT(result);
  }

  std::vector<std::string> ZoneAirHeatBalanceAlgorithm_Impl::validAlgorithmValues() const {
    return ZoneAirHeatBalanceAlgorithm::validAlgorithmValues();
  }

} // detail

IddObjectType ZoneAirHeatBalanceAlgorithm::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneAirHeatBalanceAlgorithm);
}

std::vector<std::string> ZoneAirHeatBalanceAlgorithm::validAlgorithmValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneAirHeatBalanceAlgorithmFields::Algorithm);
}

std::string ZoneAirHeatBalanceAlgorithm::algorithm() const {
  return getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>()->algorithm();
}

bool ZoneAirHeatBalanceAlgorithm::isAlgorithmDefaulted() const {
  return getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>()->isAlgorithmDefaulted();
}

bool ZoneAirHeatBalanceAlgorithm::setAlgorithm(std::string algorithm) {
  return getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>()->setAlgorithm(algorithm);
}

void ZoneAirHeatBalanceAlgorithm::resetAlgorithm() {
  getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>()->resetAlgorithm();
}

/// @cond
ZoneAirHeatBalanceAlgorithm::ZoneAirHeatBalanceAlgorithm(std::shared_ptr<detail::ZoneAirHeatBalanceAlgorithm_Impl> impl)
  : ModelObject(impl)
{
}

ZoneAirHeatBalanceAlgorithm::ZoneAirHeatBalanceAlgorithm(Model& model)
  : ModelObject(ZoneAirHeatBalanceAlgorithm::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneAirHeatBalanceAlgorithm_Impl>());

  setAlgorithm("ThirdOrderBackwardDifference");
}

/// @endcond

} // model
} // openstudio

