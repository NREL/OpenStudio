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

#include "EnergyManagementSystemTrendVariable.hpp"
#include "EnergyManagementSystemTrendVariable_Impl.hpp"
#include "EnergyManagementSystemSensor.hpp"
#include "EnergyManagementSystemSensor_Impl.hpp"
#include "EnergyManagementSystemActuator.hpp"
#include "EnergyManagementSystemGlobalVariable.hpp"
#include "EnergyManagementSystemTrendVariable.hpp"
#include "EnergyManagementSystemInternalVariable.hpp"
#include "EnergyManagementSystemCurveOrTableIndexVariable.hpp"
#include "EnergyManagementSystemConstructionIndexVariable.hpp"
#include "EnergyManagementSystemActuator_Impl.hpp"
#include "EnergyManagementSystemGlobalVariable_Impl.hpp"
#include "EnergyManagementSystemTrendVariable_Impl.hpp"
#include "EnergyManagementSystemInternalVariable_Impl.hpp"
#include "EnergyManagementSystemCurveOrTableIndexVariable_Impl.hpp"
#include "EnergyManagementSystemConstructionIndexVariable_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_TrendVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EnergyManagementSystemTrendVariable_Impl::EnergyManagementSystemTrendVariable_Impl(const IdfObject& idfObject,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemTrendVariable::iddObjectType());
  }

  EnergyManagementSystemTrendVariable_Impl::EnergyManagementSystemTrendVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemTrendVariable::iddObjectType());
  }

  EnergyManagementSystemTrendVariable_Impl::EnergyManagementSystemTrendVariable_Impl(const EnergyManagementSystemTrendVariable_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemTrendVariable_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EnergyManagementSystemTrendVariable_Impl::iddObjectType() const {
    return EnergyManagementSystemTrendVariable::iddObjectType();
  }

  std::string EnergyManagementSystemTrendVariable_Impl::emsVariableName() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName,true);
    return value.get();
  }

  boost::optional<ModelObject> EnergyManagementSystemTrendVariable_Impl::emsVariableObject() const {
    boost::optional<ModelObject> value = this->getTarget(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName)->cast<ModelObject>();
    return value;
  }
  boost::optional< EnergyManagementSystemActuator> EnergyManagementSystemTrendVariable_Impl::emsActuator() const {
    boost::optional<EnergyManagementSystemActuator> value = this->getTarget(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName)->cast<EnergyManagementSystemActuator>();
    return value;
  }

  boost::optional< EnergyManagementSystemSensor> EnergyManagementSystemTrendVariable_Impl::emsSensor() const {
    boost::optional<EnergyManagementSystemSensor> value = this->getTarget(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName)->cast<EnergyManagementSystemSensor>();
    return value;
  }

  boost::optional<EnergyManagementSystemGlobalVariable> EnergyManagementSystemTrendVariable_Impl::emsGlobalVariable() const {
    boost::optional<EnergyManagementSystemGlobalVariable> value = this->getTarget(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName)->cast<EnergyManagementSystemGlobalVariable>();
    return value;
  }

  boost::optional<EnergyManagementSystemTrendVariable> EnergyManagementSystemTrendVariable_Impl::emsTrendVariable() const {
    boost::optional<EnergyManagementSystemTrendVariable> value = this->getTarget(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName)->cast<EnergyManagementSystemTrendVariable>();
    return value;
  }

  boost::optional<EnergyManagementSystemInternalVariable> EnergyManagementSystemTrendVariable_Impl::emsInternalVariable() const {
    boost::optional<EnergyManagementSystemInternalVariable> value = this->getTarget(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName)->cast<EnergyManagementSystemInternalVariable>();
    return value;
  }

  boost::optional<EnergyManagementSystemCurveOrTableIndexVariable> EnergyManagementSystemTrendVariable_Impl::emsCurveOrTableIndexVariable() const {
    boost::optional<EnergyManagementSystemCurveOrTableIndexVariable> value = this->getTarget(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName)->cast<EnergyManagementSystemCurveOrTableIndexVariable>();
    return value;
  }

  boost::optional<EnergyManagementSystemConstructionIndexVariable> EnergyManagementSystemTrendVariable_Impl::emsConstructionIndexVariable() const {
    boost::optional<EnergyManagementSystemConstructionIndexVariable> value = this->getTarget(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName)->cast<EnergyManagementSystemConstructionIndexVariable>();
    return value;
  }

  int EnergyManagementSystemTrendVariable_Impl::numberOfTimestepsToBeLogged() const {
    boost::optional<int> value = getInt(OS_EnergyManagementSystem_TrendVariableFields::NumberofTimestepstobeLogged,true);
    return value.get();
  }

  bool EnergyManagementSystemTrendVariable_Impl::setEMSVariableName(const std::string& eMSVariableName) {
    bool result = setString(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName, eMSVariableName);
    return result;
  }


  bool EnergyManagementSystemTrendVariable_Impl::setEMSVariableName(const EnergyManagementSystemActuator& object) {
    bool result = setPointer(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName, object.handle());
    return result;
  }

  bool EnergyManagementSystemTrendVariable_Impl::setEMSVariableName(const EnergyManagementSystemSensor& object) {
    bool result = setPointer(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName, object.handle());
    return result;
  }

  bool EnergyManagementSystemTrendVariable_Impl::setEMSVariableName(const EnergyManagementSystemGlobalVariable& object) {
    bool result = setPointer(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName, object.handle());
    return result;
  }

  bool EnergyManagementSystemTrendVariable_Impl::setEMSVariableName(const EnergyManagementSystemTrendVariable& object) {
    bool result = setPointer(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName, object.handle());
    return result;
  }

  bool EnergyManagementSystemTrendVariable_Impl::setEMSVariableName(const EnergyManagementSystemInternalVariable& object) {
    bool result = setPointer(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName, object.handle());
    return result;
  }

  bool EnergyManagementSystemTrendVariable_Impl::setEMSVariableName(const EnergyManagementSystemCurveOrTableIndexVariable& object) {
    bool result = setPointer(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName, object.handle());
    return result;
  }

  bool EnergyManagementSystemTrendVariable_Impl::setEMSVariableName(const EnergyManagementSystemConstructionIndexVariable& object) {
    bool result = setPointer(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName, object.handle());
    return result;
  }

  bool EnergyManagementSystemTrendVariable_Impl::setNumberOfTimestepsToBeLogged(int numberofTimestepstobeLogged) {
    bool result = setInt(OS_EnergyManagementSystem_TrendVariableFields::NumberofTimestepstobeLogged, numberofTimestepstobeLogged);
    return result;
  }

} // detail

EnergyManagementSystemTrendVariable::EnergyManagementSystemTrendVariable(const Model& model, const std::string eMSVariableName)
  : ModelObject(EnergyManagementSystemTrendVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemTrendVariable_Impl>());
  bool ok = setEMSVariableName(eMSVariableName);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to " << eMSVariableName << ".");
  }
  setNumberOfTimestepsToBeLogged(1);
}

EnergyManagementSystemTrendVariable::EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemActuator& object)
  : ModelObject(EnergyManagementSystemTrendVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemTrendVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to " 
      << object.briefDescription() << ".");
  }
  setNumberOfTimestepsToBeLogged(1);
}

EnergyManagementSystemTrendVariable::EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemSensor& object)
  : ModelObject(EnergyManagementSystemTrendVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemTrendVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setNumberOfTimestepsToBeLogged(1);
}
EnergyManagementSystemTrendVariable::EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemGlobalVariable& object)
  : ModelObject(EnergyManagementSystemTrendVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemTrendVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setNumberOfTimestepsToBeLogged(1);
}
EnergyManagementSystemTrendVariable::EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemTrendVariable& object)
  : ModelObject(EnergyManagementSystemTrendVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemTrendVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setNumberOfTimestepsToBeLogged(1);
}
EnergyManagementSystemTrendVariable::EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemInternalVariable& object)
  : ModelObject(EnergyManagementSystemTrendVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemTrendVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setNumberOfTimestepsToBeLogged(1);
}
EnergyManagementSystemTrendVariable::EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemCurveOrTableIndexVariable& object)
  : ModelObject(EnergyManagementSystemTrendVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemTrendVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setNumberOfTimestepsToBeLogged(1);
}
EnergyManagementSystemTrendVariable::EnergyManagementSystemTrendVariable(const Model& model, const EnergyManagementSystemConstructionIndexVariable& object)
  : ModelObject(EnergyManagementSystemTrendVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemTrendVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setNumberOfTimestepsToBeLogged(1);
}

EnergyManagementSystemTrendVariable::EnergyManagementSystemTrendVariable(const Model& model)
  : ModelObject(EnergyManagementSystemTrendVariable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemTrendVariable_Impl>());
  setNumberOfTimestepsToBeLogged(1);
}

IddObjectType EnergyManagementSystemTrendVariable::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_TrendVariable);
}

std::string EnergyManagementSystemTrendVariable::emsVariableName() const {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->emsVariableName();
}

boost::optional<EnergyManagementSystemActuator> EnergyManagementSystemTrendVariable::emsActuator() const {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->emsActuator();
}

boost::optional<EnergyManagementSystemSensor> EnergyManagementSystemTrendVariable::emsSensor() const {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->emsSensor();
}

boost::optional<EnergyManagementSystemGlobalVariable> EnergyManagementSystemTrendVariable::emsGlobalVariable() const {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->emsGlobalVariable();
}

boost::optional<EnergyManagementSystemTrendVariable> EnergyManagementSystemTrendVariable::emsTrendVariable() const {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->emsTrendVariable();
}

boost::optional<EnergyManagementSystemInternalVariable> EnergyManagementSystemTrendVariable::emsInternalVariable() const {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->emsInternalVariable();
}

boost::optional<EnergyManagementSystemCurveOrTableIndexVariable> EnergyManagementSystemTrendVariable::emsCurveOrTableIndexVariable() const {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->emsCurveOrTableIndexVariable();
}

boost::optional<EnergyManagementSystemConstructionIndexVariable> EnergyManagementSystemTrendVariable::emsConstructionIndexVariable() const {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->emsConstructionIndexVariable();
}

boost::optional<ModelObject> EnergyManagementSystemTrendVariable::emsVariableObject() const {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->emsVariableObject();
}

int EnergyManagementSystemTrendVariable::numberOfTimestepsToBeLogged() const {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->numberOfTimestepsToBeLogged();
}

bool EnergyManagementSystemTrendVariable::setEMSVariableName(const std::string& eMSVariableName) {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->setEMSVariableName(eMSVariableName);
}


bool EnergyManagementSystemTrendVariable::setEMSVariableName(const EnergyManagementSystemActuator& object) {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemTrendVariable::setEMSVariableName(const EnergyManagementSystemSensor& object) {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemTrendVariable::setEMSVariableName(const EnergyManagementSystemGlobalVariable& object) {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemTrendVariable::setEMSVariableName(const EnergyManagementSystemTrendVariable& object) {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemTrendVariable::setEMSVariableName(const EnergyManagementSystemInternalVariable& object) {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemTrendVariable::setEMSVariableName(const EnergyManagementSystemCurveOrTableIndexVariable& object) {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemTrendVariable::setEMSVariableName(const EnergyManagementSystemConstructionIndexVariable& object) {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemTrendVariable::setNumberOfTimestepsToBeLogged(int numberofTimestepstobeLogged) {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->setNumberOfTimestepsToBeLogged(numberofTimestepstobeLogged);
}

/// @cond
EnergyManagementSystemTrendVariable::EnergyManagementSystemTrendVariable(std::shared_ptr<detail::EnergyManagementSystemTrendVariable_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

