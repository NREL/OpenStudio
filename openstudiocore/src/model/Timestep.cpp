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

#include "Timestep.hpp"
#include "Timestep_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <utilities/idd/OS_Timestep_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

namespace model  {

namespace detail {

  Timestep_Impl::Timestep_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Timestep::iddObjectType());
  }

  Timestep_Impl::Timestep_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Timestep::iddObjectType());
  }

  Timestep_Impl::Timestep_Impl(const Timestep_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
  }

  IddObjectType Timestep_Impl::iddObjectType() const {
    return Timestep::iddObjectType();
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> Timestep_Impl::parent() const
  {
    boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
    return result;
  }

  // set the parent, child may have to call methods on the parent
  bool Timestep_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
      return true;
    }
    return false;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& Timestep_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  int Timestep_Impl::numberOfTimestepsPerHour() const {
    boost::optional<int> value = getInt(OS_TimestepFields::NumberofTimestepsperHour,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Timestep_Impl::isNumberOfTimestepsPerHourDefaulted() const {
    return isEmpty(OS_TimestepFields::NumberofTimestepsperHour);
  }

  bool Timestep_Impl::setNumberOfTimestepsPerHour(int numberOfTimestepsPerHour) {
    bool result = setInt(OS_TimestepFields::NumberofTimestepsperHour, numberOfTimestepsPerHour);
    return result;
  }

  void Timestep_Impl::resetNumberOfTimestepsPerHour() {
    bool result = setString(OS_TimestepFields::NumberofTimestepsperHour, "");
    OS_ASSERT(result);
  }

} // detail

/// constructor
Timestep::Timestep(const Model& model)
  : ModelObject(Timestep::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::Timestep_Impl>());

  setNumberOfTimestepsPerHour(6);
}

// constructor
Timestep::Timestep(std::shared_ptr<detail::Timestep_Impl> impl)
  : ModelObject(impl)
{}

IddObjectType Timestep::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Timestep);
  return result;
}

int Timestep::numberOfTimestepsPerHour() const {
  return getImpl<detail::Timestep_Impl>()->numberOfTimestepsPerHour();
}

bool Timestep::isNumberOfTimestepsPerHourDefaulted() const {
  return getImpl<detail::Timestep_Impl>()->isNumberOfTimestepsPerHourDefaulted();
}

bool Timestep::setNumberOfTimestepsPerHour(int numberOfTimestepsPerHour) {
  return getImpl<detail::Timestep_Impl>()->setNumberOfTimestepsPerHour(numberOfTimestepsPerHour);
}

void Timestep::resetNumberOfTimestepsPerHour() {
  getImpl<detail::Timestep_Impl>()->resetNumberOfTimestepsPerHour();
}

} // model
} // openstudio
