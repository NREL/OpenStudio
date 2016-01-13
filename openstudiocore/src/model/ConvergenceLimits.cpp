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

#include "ConvergenceLimits.hpp"
#include "ConvergenceLimits_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <utilities/idd/OS_ConvergenceLimits_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

namespace detail {

  ConvergenceLimits_Impl::ConvergenceLimits_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ConvergenceLimits::iddObjectType());
  }

  ConvergenceLimits_Impl::ConvergenceLimits_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ConvergenceLimits::iddObjectType());
  }

  ConvergenceLimits_Impl::ConvergenceLimits_Impl(const ConvergenceLimits_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}
  
  const std::vector<std::string>& ConvergenceLimits_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }
  
  IddObjectType ConvergenceLimits_Impl::iddObjectType() const {
    return ConvergenceLimits::iddObjectType();
  }  

  boost::optional<ParentObject> ConvergenceLimits_Impl::parent() const
  {
    boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
    return result;
  }

  bool ConvergenceLimits_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
      return true;
    }
    return false;
  }

  boost::optional<int> ConvergenceLimits_Impl::minimumSystemTimestep() const {
    return getInt(OS_ConvergenceLimitsFields::MinimumSystemTimestep,true);
  }

  int ConvergenceLimits_Impl::maximumHVACIterations() const {
    boost::optional<int> value = getInt(OS_ConvergenceLimitsFields::MaximumHVACIterations,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ConvergenceLimits_Impl::isMaximumHVACIterationsDefaulted() const {
    return isEmpty(OS_ConvergenceLimitsFields::MaximumHVACIterations);
  }

  int ConvergenceLimits_Impl::minimumPlantIterations() const {
    boost::optional<int> value = getInt(OS_ConvergenceLimitsFields::MinimumPlantIterations,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ConvergenceLimits_Impl::isMinimumPlantIterationsDefaulted() const {
    return isEmpty(OS_ConvergenceLimitsFields::MinimumPlantIterations);
  }

  int ConvergenceLimits_Impl::maximumPlantIterations() const {
    boost::optional<int> value = getInt(OS_ConvergenceLimitsFields::MaximumPlantIterations,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ConvergenceLimits_Impl::isMaximumPlantIterationsDefaulted() const {
    return isEmpty(OS_ConvergenceLimitsFields::MaximumPlantIterations);
  }

  bool ConvergenceLimits_Impl::setMinimumSystemTimestep(boost::optional<int> minimumSystemTimestep) {
    bool result = false;
    if (minimumSystemTimestep) {
      result = setInt(OS_ConvergenceLimitsFields::MinimumSystemTimestep, minimumSystemTimestep.get());
    } else {
      result = setString(OS_ConvergenceLimitsFields::MinimumSystemTimestep, "");
    }
    return result;
  }

  void ConvergenceLimits_Impl::resetMinimumSystemTimestep() {
    bool result = setString(OS_ConvergenceLimitsFields::MinimumSystemTimestep, "");
    OS_ASSERT(result);
  }

  bool ConvergenceLimits_Impl::setMaximumHVACIterations(int maximumHVACIterations) {
    bool result = setInt(OS_ConvergenceLimitsFields::MaximumHVACIterations, maximumHVACIterations);
    return result;
  }

  void ConvergenceLimits_Impl::resetMaximumHVACIterations() {
    bool result = setString(OS_ConvergenceLimitsFields::MaximumHVACIterations, "");
    OS_ASSERT(result);
  }

  bool ConvergenceLimits_Impl::setMinimumPlantIterations(int minimumPlantIterations) {
    bool result = setInt(OS_ConvergenceLimitsFields::MinimumPlantIterations, minimumPlantIterations);
    return result;
  }

  void ConvergenceLimits_Impl::resetMinimumPlantIterations() {
    bool result = setString(OS_ConvergenceLimitsFields::MinimumPlantIterations, "");
    OS_ASSERT(result);
  }

  bool ConvergenceLimits_Impl::setMaximumPlantIterations(int maximumPlantIterations) {
    bool result = setInt(OS_ConvergenceLimitsFields::MaximumPlantIterations, maximumPlantIterations);
    return result;
  }

  void ConvergenceLimits_Impl::resetMaximumPlantIterations() {
    bool result = setString(OS_ConvergenceLimitsFields::MaximumPlantIterations, "");
    OS_ASSERT(result);
  }

} // detail

IddObjectType ConvergenceLimits::iddObjectType() {
  IddObjectType result(IddObjectType::OS_ConvergenceLimits);
  return result;
}

boost::optional<int> ConvergenceLimits::minimumSystemTimestep() const {
  return getImpl<detail::ConvergenceLimits_Impl>()->minimumSystemTimestep();
}

int ConvergenceLimits::maximumHVACIterations() const {
  return getImpl<detail::ConvergenceLimits_Impl>()->maximumHVACIterations();
}

bool ConvergenceLimits::isMaximumHVACIterationsDefaulted() const {
  return getImpl<detail::ConvergenceLimits_Impl>()->isMaximumHVACIterationsDefaulted();
}

int ConvergenceLimits::minimumPlantIterations() const {
  return getImpl<detail::ConvergenceLimits_Impl>()->minimumPlantIterations();
}

bool ConvergenceLimits::isMinimumPlantIterationsDefaulted() const {
  return getImpl<detail::ConvergenceLimits_Impl>()->isMinimumPlantIterationsDefaulted();
}

int ConvergenceLimits::maximumPlantIterations() const {
  return getImpl<detail::ConvergenceLimits_Impl>()->maximumPlantIterations();
}

bool ConvergenceLimits::isMaximumPlantIterationsDefaulted() const {
  return getImpl<detail::ConvergenceLimits_Impl>()->isMaximumPlantIterationsDefaulted();
}

bool ConvergenceLimits::setMinimumSystemTimestep(int minimumSystemTimestep) {
  return getImpl<detail::ConvergenceLimits_Impl>()->setMinimumSystemTimestep(minimumSystemTimestep);
}

void ConvergenceLimits::resetMinimumSystemTimestep() {
  getImpl<detail::ConvergenceLimits_Impl>()->resetMinimumSystemTimestep();
}

bool ConvergenceLimits::setMaximumHVACIterations(int maximumHVACIterations) {
  return getImpl<detail::ConvergenceLimits_Impl>()->setMaximumHVACIterations(maximumHVACIterations);
}

void ConvergenceLimits::resetMaximumHVACIterations() {
  getImpl<detail::ConvergenceLimits_Impl>()->resetMaximumHVACIterations();
}

bool ConvergenceLimits::setMinimumPlantIterations(int minimumPlantIterations) {
  return getImpl<detail::ConvergenceLimits_Impl>()->setMinimumPlantIterations(minimumPlantIterations);
}

void ConvergenceLimits::resetMinimumPlantIterations() {
  getImpl<detail::ConvergenceLimits_Impl>()->resetMinimumPlantIterations();
}

bool ConvergenceLimits::setMaximumPlantIterations(int maximumPlantIterations) {
  return getImpl<detail::ConvergenceLimits_Impl>()->setMaximumPlantIterations(maximumPlantIterations);
}

void ConvergenceLimits::resetMaximumPlantIterations() {
  getImpl<detail::ConvergenceLimits_Impl>()->resetMaximumPlantIterations();
}

/// @cond
ConvergenceLimits::ConvergenceLimits(std::shared_ptr<detail::ConvergenceLimits_Impl> impl)
  : ModelObject(impl)
{}

ConvergenceLimits::ConvergenceLimits(Model& model)
  : ModelObject(ConvergenceLimits::iddObjectType(),model)
{
  setMinimumSystemTimestep(1);
}
/// @endcond

} // model
} // openstudio
