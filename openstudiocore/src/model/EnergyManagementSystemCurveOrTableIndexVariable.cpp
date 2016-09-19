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

#include "EnergyManagementSystemCurveOrTableIndexVariable.hpp"
#include "EnergyManagementSystemCurveOrTableIndexVariable_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
//#include "AllCurves.hpp"
//#include "AllCurves_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_CurveOrTableIndexVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EnergyManagementSystemCurveOrTableIndexVariable_Impl::EnergyManagementSystemCurveOrTableIndexVariable_Impl(const IdfObject& idfObject,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemCurveOrTableIndexVariable::iddObjectType());
  }

  EnergyManagementSystemCurveOrTableIndexVariable_Impl::EnergyManagementSystemCurveOrTableIndexVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemCurveOrTableIndexVariable::iddObjectType());
  }

  EnergyManagementSystemCurveOrTableIndexVariable_Impl::EnergyManagementSystemCurveOrTableIndexVariable_Impl(const EnergyManagementSystemCurveOrTableIndexVariable_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemCurveOrTableIndexVariable_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EnergyManagementSystemCurveOrTableIndexVariable_Impl::iddObjectType() const {
    return EnergyManagementSystemCurveOrTableIndexVariable::iddObjectType();
  }

  ModelObject EnergyManagementSystemCurveOrTableIndexVariable_Impl::curveorTableObject() const {
    boost::optional<ModelObject> value = optionalCurveorTableObject();
    if (!value) {
      LOG(Info, " does not have an Curveor Table Object attached.");
    }
    return value.get();
  }

  bool EnergyManagementSystemCurveOrTableIndexVariable_Impl::setCurveorTableObject(const ModelObject& allCurves) {
    bool result = setPointer(OS_EnergyManagementSystem_CurveOrTableIndexVariableFields::CurveorTableObjectName, allCurves.handle());
    return result;
  }

  boost::optional<ModelObject> EnergyManagementSystemCurveOrTableIndexVariable_Impl::optionalCurveorTableObject() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_EnergyManagementSystem_CurveOrTableIndexVariableFields::CurveorTableObjectName);
  }

} // detail

EnergyManagementSystemCurveOrTableIndexVariable::EnergyManagementSystemCurveOrTableIndexVariable(const Model& model)
  : ModelObject(EnergyManagementSystemCurveOrTableIndexVariable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemCurveOrTableIndexVariable_Impl>());
}

IddObjectType EnergyManagementSystemCurveOrTableIndexVariable::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_CurveOrTableIndexVariable);
}

ModelObject EnergyManagementSystemCurveOrTableIndexVariable::curveorTableObject() const {
  return getImpl<detail::EnergyManagementSystemCurveOrTableIndexVariable_Impl>()->curveorTableObject();
}

bool EnergyManagementSystemCurveOrTableIndexVariable::setCurveorTableObject(const ModelObject& allCurves) {
  return getImpl<detail::EnergyManagementSystemCurveOrTableIndexVariable_Impl>()->setCurveorTableObject(allCurves);
}

/// @cond
EnergyManagementSystemCurveOrTableIndexVariable::EnergyManagementSystemCurveOrTableIndexVariable(std::shared_ptr<detail::EnergyManagementSystemCurveOrTableIndexVariable_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

