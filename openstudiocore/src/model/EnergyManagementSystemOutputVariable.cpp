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

#include "EnergyManagementSystemOutputVariable.hpp"
#include "EnergyManagementSystemOutputVariable_Impl.hpp"
#include "EnergyManagementSystemProgram.hpp"
#include "EnergyManagementSystemProgram_Impl.hpp"
#include "EnergyManagementSystemSubroutine.hpp"
#include "EnergyManagementSystemSubroutine_Impl.hpp"
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

#include "Model.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_OutputVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EnergyManagementSystemOutputVariable_Impl::EnergyManagementSystemOutputVariable_Impl(const IdfObject& idfObject,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemOutputVariable::iddObjectType());
  }

  EnergyManagementSystemOutputVariable_Impl::EnergyManagementSystemOutputVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemOutputVariable::iddObjectType());
  }

  EnergyManagementSystemOutputVariable_Impl::EnergyManagementSystemOutputVariable_Impl(const EnergyManagementSystemOutputVariable_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemOutputVariable_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EnergyManagementSystemOutputVariable_Impl::iddObjectType() const {
    return EnergyManagementSystemOutputVariable::iddObjectType();
  }

  std::string EnergyManagementSystemOutputVariable_Impl::emsVariableName() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName,true);
    return value.get();
  }

  std::string EnergyManagementSystemOutputVariable_Impl::typeOfDataInVariable() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_OutputVariableFields::TypeofDatainVariable,true);
    return value.get();
  }

  std::string EnergyManagementSystemOutputVariable_Impl::updateFrequency() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_OutputVariableFields::UpdateFrequency,true);
    return value.get();
  }

  std::string EnergyManagementSystemOutputVariable_Impl::emsProgramOrSubroutineName() const {
    //return getString(OS_EnergyManagementSystem_OutputVariableFields::EMSProgramorSubroutineName,true);
    boost::optional<WorkspaceObject> object = getTarget(OS_EnergyManagementSystem_OutputVariableFields::EMSProgramorSubroutineName);
    if (object) {
      return object.get().nameString();
    }
    return "";
  }

  boost::optional<EnergyManagementSystemProgram> EnergyManagementSystemOutputVariable_Impl::emsProgram() const {
    boost::optional<EnergyManagementSystemProgram> result;
    OptionalWorkspaceObject object = getTarget(OS_EnergyManagementSystem_OutputVariableFields::EMSProgramorSubroutineName);
    if (object) {
      result = object->optionalCast<EnergyManagementSystemProgram>();
    }
    return result;
  }

  boost::optional<EnergyManagementSystemSubroutine> EnergyManagementSystemOutputVariable_Impl::emsSubroutine() const {
    boost::optional<EnergyManagementSystemSubroutine> result;
    OptionalWorkspaceObject object = getTarget(OS_EnergyManagementSystem_OutputVariableFields::EMSProgramorSubroutineName);
    if (object) {
      result = object->optionalCast<EnergyManagementSystemSubroutine>();
    }
    return result;
  }

  std::string EnergyManagementSystemOutputVariable_Impl::units() const {
    OptionalString results = getString(OS_EnergyManagementSystem_OutputVariableFields::Units,true);
    if (results) {
      return results.get();
    }
    return "";
  }

  bool EnergyManagementSystemOutputVariable_Impl::setEMSVariableName(const std::string& eMSVariableName) {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, eMSVariableName);
    return result;
  }

  bool EnergyManagementSystemOutputVariable_Impl::setEMSVariableName(const EnergyManagementSystemActuator& object) {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, toString(object.handle()));
    return result;
  }

  bool EnergyManagementSystemOutputVariable_Impl::setEMSVariableName(const EnergyManagementSystemSensor& object) {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, toString(object.handle()));
    return result;
  }

  bool EnergyManagementSystemOutputVariable_Impl::setEMSVariableName(const EnergyManagementSystemGlobalVariable& object) {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, toString(object.handle()));
    return result;
  }

  bool EnergyManagementSystemOutputVariable_Impl::setEMSVariableName(const EnergyManagementSystemTrendVariable& object) {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, toString(object.handle()));
    return result;
  }

  bool EnergyManagementSystemOutputVariable_Impl::setEMSVariableName(const EnergyManagementSystemInternalVariable& object) {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, toString(object.handle()));
    return result;
  }

  bool EnergyManagementSystemOutputVariable_Impl::setEMSVariableName(const EnergyManagementSystemCurveOrTableIndexVariable& object) {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, toString(object.handle()));
    return result;
  }

  bool EnergyManagementSystemOutputVariable_Impl::setEMSVariableName(const EnergyManagementSystemConstructionIndexVariable& object) {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, toString(object.handle()));
    return result;
  }

  bool EnergyManagementSystemOutputVariable_Impl::setTypeOfDataInVariable(const std::string& typeofDatainVariable) {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::TypeofDatainVariable, typeofDatainVariable);
    return result;
  }

  bool EnergyManagementSystemOutputVariable_Impl::setUpdateFrequency(const std::string& updateFrequency) {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::UpdateFrequency, updateFrequency);
    return result;
  }

  bool EnergyManagementSystemOutputVariable_Impl::setEMSProgramOrSubroutineName(const EnergyManagementSystemProgram& program) {
    bool result = setPointer(OS_EnergyManagementSystem_OutputVariableFields::EMSProgramorSubroutineName, program.handle());
    return result;
  }

  bool EnergyManagementSystemOutputVariable_Impl::setEMSProgramOrSubroutineName(const EnergyManagementSystemSubroutine& subroutine) {
    bool result = setPointer(OS_EnergyManagementSystem_OutputVariableFields::EMSProgramorSubroutineName, subroutine.handle());
    return result;
  }

  void EnergyManagementSystemOutputVariable_Impl::resetEMSProgramOrSubroutineName() {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::EMSProgramorSubroutineName, "");
    //bool result = setPointer(OS_EnergyManagementSystem_OutputVariableFields::EMSProgramorSubroutineName, NULL);
    OS_ASSERT(result);
  }

  bool EnergyManagementSystemOutputVariable_Impl::setUnits(const std::string& units) {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::Units, units);
    return result;
  }

  void EnergyManagementSystemOutputVariable_Impl::resetUnits() {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::Units, "");
    OS_ASSERT(result);
  }

  boost::optional<ModelObject> EnergyManagementSystemOutputVariable_Impl::emsVariableObject() const {
    boost::optional<ModelObject> value;
    boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, true);
    if (handle) {
      const Model m = this->model();
      value = m.getModelObject<ModelObject>(toUUID(handle.get()));
    }
    return value;
  }

  boost::optional< EnergyManagementSystemActuator> EnergyManagementSystemOutputVariable_Impl::emsActuator() const {
    boost::optional<EnergyManagementSystemActuator> value;
    boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, true);
    if (handle) {
      const Model m = this->model();
      value = m.getModelObject<EnergyManagementSystemActuator>(toUUID(handle.get()));
    }
    return value;
  }

  boost::optional< EnergyManagementSystemSensor> EnergyManagementSystemOutputVariable_Impl::emsSensor() const {
    boost::optional<EnergyManagementSystemSensor> value;
    boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, true);
    if (handle) {
      const Model m = this->model();
      value = m.getModelObject<EnergyManagementSystemSensor>(toUUID(handle.get()));
    }
    return value;
  }

  boost::optional<EnergyManagementSystemGlobalVariable> EnergyManagementSystemOutputVariable_Impl::emsGlobalVariable() const {
    boost::optional<EnergyManagementSystemGlobalVariable> value;
    boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, true);
    if (handle) {
      const Model m = this->model();
      value = m.getModelObject<EnergyManagementSystemGlobalVariable>(toUUID(handle.get()));
    }
    return value;
  }

  boost::optional<EnergyManagementSystemTrendVariable> EnergyManagementSystemOutputVariable_Impl::emsTrendVariable() const {
    boost::optional<EnergyManagementSystemTrendVariable> value;
    boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, true);
    if (handle) {
      const Model m = this->model();
      value = m.getModelObject<EnergyManagementSystemTrendVariable>(toUUID(handle.get()));
    }
    return value;
  }

  boost::optional<EnergyManagementSystemInternalVariable> EnergyManagementSystemOutputVariable_Impl::emsInternalVariable() const {
    boost::optional<EnergyManagementSystemInternalVariable> value;
    boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, true);
    if (handle) {
      const Model m = this->model();
      value = m.getModelObject<EnergyManagementSystemInternalVariable>(toUUID(handle.get()));
    }
    return value;
  }

  boost::optional<EnergyManagementSystemCurveOrTableIndexVariable> EnergyManagementSystemOutputVariable_Impl::emsCurveOrTableIndexVariable() const {
    boost::optional<EnergyManagementSystemCurveOrTableIndexVariable> value;
    boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, true);
    if (handle) {
      const Model m = this->model();
      value = m.getModelObject<EnergyManagementSystemCurveOrTableIndexVariable>(toUUID(handle.get()));
    }
    return value;
  }

  boost::optional<EnergyManagementSystemConstructionIndexVariable> EnergyManagementSystemOutputVariable_Impl::emsConstructionIndexVariable() const {
    boost::optional<EnergyManagementSystemConstructionIndexVariable> value;
    boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, true);
    if (handle) {
      const Model m = this->model();
      value = m.getModelObject<EnergyManagementSystemConstructionIndexVariable>(toUUID(handle.get()));
    }
    return value;
  }

} // detail

EnergyManagementSystemOutputVariable::EnergyManagementSystemOutputVariable(const Model& model, const std::string eMSVariableName)
: ModelObject(EnergyManagementSystemOutputVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemOutputVariable_Impl>());
  bool ok = setEMSVariableName(eMSVariableName);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to " << eMSVariableName << ".");
  }
  setUpdateFrequency("ZoneTimestep");
  setTypeOfDataInVariable("Averaged");
}

EnergyManagementSystemOutputVariable::EnergyManagementSystemOutputVariable(const Model& model, const EnergyManagementSystemActuator& object)
  : ModelObject(EnergyManagementSystemOutputVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemOutputVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to " 
      << object.briefDescription() << ".");
  }
  setUpdateFrequency("ZoneTimestep");
  setTypeOfDataInVariable("Averaged");
}

EnergyManagementSystemOutputVariable::EnergyManagementSystemOutputVariable(const Model& model, const EnergyManagementSystemSensor& object)
  : ModelObject(EnergyManagementSystemOutputVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemOutputVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setUpdateFrequency("ZoneTimestep");
  setTypeOfDataInVariable("Averaged");
}

EnergyManagementSystemOutputVariable::EnergyManagementSystemOutputVariable(const Model& model, const EnergyManagementSystemGlobalVariable& object)
  : ModelObject(EnergyManagementSystemOutputVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemOutputVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setUpdateFrequency("ZoneTimestep");
  setTypeOfDataInVariable("Averaged");
}

EnergyManagementSystemOutputVariable::EnergyManagementSystemOutputVariable(const Model& model, const EnergyManagementSystemTrendVariable& object)
  : ModelObject(EnergyManagementSystemOutputVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemOutputVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setUpdateFrequency("ZoneTimestep");
  setTypeOfDataInVariable("Averaged");
}

EnergyManagementSystemOutputVariable::EnergyManagementSystemOutputVariable(const Model& model, const EnergyManagementSystemInternalVariable& object)
  : ModelObject(EnergyManagementSystemOutputVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemOutputVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setUpdateFrequency("ZoneTimestep");
  setTypeOfDataInVariable("Averaged");
}

EnergyManagementSystemOutputVariable::EnergyManagementSystemOutputVariable(const Model& model, const EnergyManagementSystemCurveOrTableIndexVariable& object)
  : ModelObject(EnergyManagementSystemOutputVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemOutputVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setUpdateFrequency("ZoneTimestep");
  setTypeOfDataInVariable("Averaged");
}

EnergyManagementSystemOutputVariable::EnergyManagementSystemOutputVariable(const Model& model, const EnergyManagementSystemConstructionIndexVariable& object)
  : ModelObject(EnergyManagementSystemOutputVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemOutputVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setUpdateFrequency("ZoneTimestep");
  setTypeOfDataInVariable("Averaged");
}

EnergyManagementSystemOutputVariable::EnergyManagementSystemOutputVariable(const Model& model)
  : ModelObject(EnergyManagementSystemOutputVariable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemOutputVariable_Impl>());
  //TODO setEMSVariableName to a valid default
  setUpdateFrequency("ZoneTimestep");
  setTypeOfDataInVariable("Averaged");
}

IddObjectType EnergyManagementSystemOutputVariable::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_OutputVariable);
}

std::vector<std::string> EnergyManagementSystemOutputVariable::typeOfDataInVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EnergyManagementSystem_OutputVariableFields::TypeofDatainVariable);
}

std::vector<std::string> EnergyManagementSystemOutputVariable::updateFrequencyValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EnergyManagementSystem_OutputVariableFields::UpdateFrequency);
}

std::string EnergyManagementSystemOutputVariable::emsVariableName() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->emsVariableName();
}

std::string EnergyManagementSystemOutputVariable::typeOfDataInVariable() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->typeOfDataInVariable();
}

std::string EnergyManagementSystemOutputVariable::updateFrequency() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->updateFrequency();
}

std::string EnergyManagementSystemOutputVariable::emsProgramOrSubroutineName() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->emsProgramOrSubroutineName();
}

boost::optional<EnergyManagementSystemProgram> EnergyManagementSystemOutputVariable::emsProgram() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->emsProgram();
}

boost::optional<EnergyManagementSystemSubroutine> EnergyManagementSystemOutputVariable::emsSubroutine() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->emsSubroutine();
}

std::string EnergyManagementSystemOutputVariable::units() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->units();
}

boost::optional<ModelObject> EnergyManagementSystemOutputVariable::emsVariableObject() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->emsVariableObject();
}

boost::optional<EnergyManagementSystemActuator> EnergyManagementSystemOutputVariable::emsActuator() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->emsActuator();
}

boost::optional<EnergyManagementSystemSensor> EnergyManagementSystemOutputVariable::emsSensor() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->emsSensor();
}

boost::optional<EnergyManagementSystemGlobalVariable> EnergyManagementSystemOutputVariable::emsGlobalVariable() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->emsGlobalVariable();
}

boost::optional<EnergyManagementSystemTrendVariable> EnergyManagementSystemOutputVariable::emsTrendVariable() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->emsTrendVariable();
}

boost::optional<EnergyManagementSystemInternalVariable> EnergyManagementSystemOutputVariable::emsInternalVariable() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->emsInternalVariable();
}

boost::optional<EnergyManagementSystemCurveOrTableIndexVariable> EnergyManagementSystemOutputVariable::emsCurveOrTableIndexVariable() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->emsCurveOrTableIndexVariable();
}

boost::optional<EnergyManagementSystemConstructionIndexVariable> EnergyManagementSystemOutputVariable::emsConstructionIndexVariable() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->emsConstructionIndexVariable();
}

bool EnergyManagementSystemOutputVariable::setTypeOfDataInVariable(const std::string& typeofDatainVariable) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setTypeOfDataInVariable(typeofDatainVariable);
}

bool EnergyManagementSystemOutputVariable::setUpdateFrequency(const std::string& updateFrequency) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setUpdateFrequency(updateFrequency);
}

bool EnergyManagementSystemOutputVariable::setEMSProgramOrSubroutineName(const EnergyManagementSystemProgram& program) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setEMSProgramOrSubroutineName(program);
}

bool EnergyManagementSystemOutputVariable::setEMSProgramOrSubroutineName(const EnergyManagementSystemSubroutine& subroutine) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setEMSProgramOrSubroutineName(subroutine);
}

void EnergyManagementSystemOutputVariable::resetEMSProgramOrSubroutineName() {
  getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->resetEMSProgramOrSubroutineName();
}

bool EnergyManagementSystemOutputVariable::setUnits(const std::string& units) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setUnits(units);
}

void EnergyManagementSystemOutputVariable::resetUnits() {
  getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->resetUnits();
}

bool EnergyManagementSystemOutputVariable::setEMSVariableName(const std::string& eMSVariableName) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setEMSVariableName(eMSVariableName);
}

bool EnergyManagementSystemOutputVariable::setEMSVariableName(const EnergyManagementSystemActuator& object) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemOutputVariable::setEMSVariableName(const EnergyManagementSystemSensor& object) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemOutputVariable::setEMSVariableName(const EnergyManagementSystemGlobalVariable& object) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemOutputVariable::setEMSVariableName(const EnergyManagementSystemTrendVariable& object) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemOutputVariable::setEMSVariableName(const EnergyManagementSystemInternalVariable& object) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemOutputVariable::setEMSVariableName(const EnergyManagementSystemCurveOrTableIndexVariable& object) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemOutputVariable::setEMSVariableName(const EnergyManagementSystemConstructionIndexVariable& object) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setEMSVariableName(object);
}

/// @cond
EnergyManagementSystemOutputVariable::EnergyManagementSystemOutputVariable(std::shared_ptr<detail::EnergyManagementSystemOutputVariable_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

