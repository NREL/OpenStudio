/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "EnergyManagementSystemMeteredOutputVariable.hpp"
#include "EnergyManagementSystemMeteredOutputVariable_Impl.hpp"
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
#include <utilities/idd/OS_EnergyManagementSystem_MeteredOutputVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EnergyManagementSystemMeteredOutputVariable_Impl::EnergyManagementSystemMeteredOutputVariable_Impl(const IdfObject& idfObject,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemMeteredOutputVariable::iddObjectType());
  }

  EnergyManagementSystemMeteredOutputVariable_Impl::EnergyManagementSystemMeteredOutputVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemMeteredOutputVariable::iddObjectType());
  }

  EnergyManagementSystemMeteredOutputVariable_Impl::EnergyManagementSystemMeteredOutputVariable_Impl(const EnergyManagementSystemMeteredOutputVariable_Impl& other,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemMeteredOutputVariable_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EnergyManagementSystemMeteredOutputVariable_Impl::iddObjectType() const {
    return EnergyManagementSystemMeteredOutputVariable::iddObjectType();
  }

  std::string EnergyManagementSystemMeteredOutputVariable_Impl::emsVariableName() const {
    //TODO call emsVAriableObject() return .nameString()
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName,true);
    return value.get();
  }

  std::string EnergyManagementSystemMeteredOutputVariable_Impl::updateFrequency() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::UpdateFrequency,true);
    return value.get();
  }

  std::string EnergyManagementSystemMeteredOutputVariable_Impl::emsProgramOrSubroutineName() const {
    //return getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName,true);
    boost::optional<WorkspaceObject> object = getTarget(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName);
    if (object) {
      return object.get().nameString();
    }
    return "";
  }

  boost::optional<EnergyManagementSystemProgram> EnergyManagementSystemMeteredOutputVariable_Impl::emsProgram() const {
    boost::optional<EnergyManagementSystemProgram> result;
    OptionalWorkspaceObject object = getTarget(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName);
    if (object) {
      result = object->optionalCast<EnergyManagementSystemProgram>();
    }
    return result;
  }

  boost::optional<EnergyManagementSystemSubroutine> EnergyManagementSystemMeteredOutputVariable_Impl::emsSubroutine() const {
    boost::optional<EnergyManagementSystemSubroutine> result;
    OptionalWorkspaceObject object = getTarget(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName);
    if (object) {
      result = object->optionalCast<EnergyManagementSystemSubroutine>();
    }
    return result;
  }

  std::string EnergyManagementSystemMeteredOutputVariable_Impl::resourceType() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::ResourceType,true);
    return value.get();
  }

  std::string EnergyManagementSystemMeteredOutputVariable_Impl::groupType() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::GroupType,true);
    return value.get();
  }

  std::string EnergyManagementSystemMeteredOutputVariable_Impl::endUseCategory() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EndUseCategory,true);
    return value.get();
  }

  std::string EnergyManagementSystemMeteredOutputVariable_Impl::endUseSubcategory() const {
    OptionalString results = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EndUseSubcategory,true);
    if (results) {
      return results.get();
    }
    return "";
  }

  std::string EnergyManagementSystemMeteredOutputVariable_Impl::units() const {
    OptionalString results = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::Units, true);
    if (results) {
      return results.get();
    }
    return "";
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEMSVariableName(const std::string& eMSVariableName) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, eMSVariableName);
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEMSVariableName(const EnergyManagementSystemActuator& object) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, toString(object.handle()));
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEMSVariableName(const EnergyManagementSystemSensor& object) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, toString(object.handle()));
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEMSVariableName(const EnergyManagementSystemGlobalVariable& object) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, toString(object.handle()));
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEMSVariableName(const EnergyManagementSystemTrendVariable& object) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, toString(object.handle()));
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEMSVariableName(const EnergyManagementSystemInternalVariable& object) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, toString(object.handle()));
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEMSVariableName(const EnergyManagementSystemCurveOrTableIndexVariable& object) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, toString(object.handle()));
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEMSVariableName(const EnergyManagementSystemConstructionIndexVariable& object) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, toString(object.handle()));
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setUpdateFrequency(const std::string& updateFrequency) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::UpdateFrequency, updateFrequency);
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEMSProgramOrSubroutineName(const EnergyManagementSystemProgram& program) {
    bool result = setPointer(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName, program.handle());
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEMSProgramOrSubroutineName(const EnergyManagementSystemSubroutine& subroutine) {
    bool result = setPointer(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName, subroutine.handle());
    return result;
  }

  void EnergyManagementSystemMeteredOutputVariable_Impl::resetEMSProgramOrSubroutineName() {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName, "");
    OS_ASSERT(result);
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setResourceType(const std::string& resourceType) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::ResourceType, resourceType);
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setGroupType(const std::string& groupType) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::GroupType, groupType);
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEndUseCategory(const std::string& endUseCategory) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EndUseCategory, endUseCategory);
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EndUseSubcategory, endUseSubcategory);
    return result;
  }

  void EnergyManagementSystemMeteredOutputVariable_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setUnits(const std::string& units) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::Units, units);
    return result;
  }

  void EnergyManagementSystemMeteredOutputVariable_Impl::resetUnits() {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::Units, "");
    OS_ASSERT(result);
  }

  boost::optional<ModelObject> EnergyManagementSystemMeteredOutputVariable_Impl::emsVariableObject() const {
    boost::optional<ModelObject> value;
    boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, true);
    if (handle) {
      const Model m = this->model();
      value = m.getModelObject<ModelObject>(toUUID(handle.get()));
    }
    return value;
  }

  boost::optional< EnergyManagementSystemActuator> EnergyManagementSystemMeteredOutputVariable_Impl::emsActuator() const {
    boost::optional<EnergyManagementSystemActuator> value;
    boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, true);
    if (handle) {
      const Model m = this->model();
      value = m.getModelObject<EnergyManagementSystemActuator>(toUUID(handle.get()));
    }
    return value;
  }

  boost::optional< EnergyManagementSystemSensor> EnergyManagementSystemMeteredOutputVariable_Impl::emsSensor() const {
    boost::optional<EnergyManagementSystemSensor> value;
    boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, true);
    if (handle) {
      const Model m = this->model();
      value = m.getModelObject<EnergyManagementSystemSensor>(toUUID(handle.get()));
    }
    return value;
  }

  boost::optional<EnergyManagementSystemGlobalVariable> EnergyManagementSystemMeteredOutputVariable_Impl::emsGlobalVariable() const {
    boost::optional<EnergyManagementSystemGlobalVariable> value;
    boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, true);
    if (handle) {
      const Model m = this->model();
      value = m.getModelObject<EnergyManagementSystemGlobalVariable>(toUUID(handle.get()));
    }
    return value;
  }

  boost::optional<EnergyManagementSystemTrendVariable> EnergyManagementSystemMeteredOutputVariable_Impl::emsTrendVariable() const {
    boost::optional<EnergyManagementSystemTrendVariable> value;
    boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, true);
    if (handle) {
      const Model m = this->model();
      value = m.getModelObject<EnergyManagementSystemTrendVariable>(toUUID(handle.get()));
    }
    return value;
  }

  boost::optional<EnergyManagementSystemInternalVariable> EnergyManagementSystemMeteredOutputVariable_Impl::emsInternalVariable() const {
    boost::optional<EnergyManagementSystemInternalVariable> value;
    boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, true);
    if (handle) {
      const Model m = this->model();
      value = m.getModelObject<EnergyManagementSystemInternalVariable>(toUUID(handle.get()));
    }
    return value;
  }

  boost::optional<EnergyManagementSystemCurveOrTableIndexVariable> EnergyManagementSystemMeteredOutputVariable_Impl::emsCurveOrTableIndexVariable() const {
    boost::optional<EnergyManagementSystemCurveOrTableIndexVariable> value;
    boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, true);
    if (handle) {
      const Model m = this->model();
      value = m.getModelObject<EnergyManagementSystemCurveOrTableIndexVariable>(toUUID(handle.get()));
    }
    return value;
  }

  boost::optional<EnergyManagementSystemConstructionIndexVariable> EnergyManagementSystemMeteredOutputVariable_Impl::emsConstructionIndexVariable() const {
    boost::optional<EnergyManagementSystemConstructionIndexVariable> value;
    boost::optional<std::string> handle = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, true);
    if (handle) {
      const Model m = this->model();
      value = m.getModelObject<EnergyManagementSystemConstructionIndexVariable>(toUUID(handle.get()));
    }
    return value;
  }

} // detail

EnergyManagementSystemMeteredOutputVariable::EnergyManagementSystemMeteredOutputVariable(const Model& model, const std::string eMSVariableName)
  : ModelObject(EnergyManagementSystemMeteredOutputVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>());
  bool ok = setEMSVariableName(eMSVariableName);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to " << eMSVariableName << ".");
  }
  setUpdateFrequency("SystemTimestep");
  setResourceType("Electricity");
  setGroupType("Building");
  setEndUseCategory("Fans");
}

EnergyManagementSystemMeteredOutputVariable::EnergyManagementSystemMeteredOutputVariable(const Model& model, const EnergyManagementSystemActuator& object)
  : ModelObject(EnergyManagementSystemMeteredOutputVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setUpdateFrequency("SystemTimestep");
  setResourceType("Electricity");
  setGroupType("Building");
  setEndUseCategory("Fans");
}

EnergyManagementSystemMeteredOutputVariable::EnergyManagementSystemMeteredOutputVariable(const Model& model, const EnergyManagementSystemSensor& object)
  : ModelObject(EnergyManagementSystemMeteredOutputVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setUpdateFrequency("SystemTimestep");
  setResourceType("Electricity");
  setGroupType("Building");
  setEndUseCategory("Fans");
}

EnergyManagementSystemMeteredOutputVariable::EnergyManagementSystemMeteredOutputVariable(const Model& model, const EnergyManagementSystemGlobalVariable& object)
  : ModelObject(EnergyManagementSystemMeteredOutputVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setUpdateFrequency("SystemTimestep");
  setResourceType("Electricity");
  setGroupType("Building");
  setEndUseCategory("Fans");
}

EnergyManagementSystemMeteredOutputVariable::EnergyManagementSystemMeteredOutputVariable(const Model& model, const EnergyManagementSystemTrendVariable& object)
  : ModelObject(EnergyManagementSystemMeteredOutputVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setUpdateFrequency("SystemTimestep");
  setResourceType("Electricity");
  setGroupType("Building");
  setEndUseCategory("Fans");
}

EnergyManagementSystemMeteredOutputVariable::EnergyManagementSystemMeteredOutputVariable(const Model& model, const EnergyManagementSystemInternalVariable& object)
  : ModelObject(EnergyManagementSystemMeteredOutputVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setUpdateFrequency("SystemTimestep");
  setResourceType("Electricity");
  setGroupType("Building");
  setEndUseCategory("Fans");
}

EnergyManagementSystemMeteredOutputVariable::EnergyManagementSystemMeteredOutputVariable(const Model& model, const EnergyManagementSystemCurveOrTableIndexVariable& object)
  : ModelObject(EnergyManagementSystemMeteredOutputVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setUpdateFrequency("SystemTimestep");
  setResourceType("Electricity");
  setGroupType("Building");
  setEndUseCategory("Fans");
}

EnergyManagementSystemMeteredOutputVariable::EnergyManagementSystemMeteredOutputVariable(const Model& model, const EnergyManagementSystemConstructionIndexVariable& object)
  : ModelObject(EnergyManagementSystemMeteredOutputVariable::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>());
  bool ok = setEMSVariableName(object);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s eMSVariableName to "
      << object.briefDescription() << ".");
  }
  setUpdateFrequency("SystemTimestep");
  setResourceType("Electricity");
  setGroupType("Building");
  setEndUseCategory("Fans");
}

EnergyManagementSystemMeteredOutputVariable::EnergyManagementSystemMeteredOutputVariable(const Model& model)
  : ModelObject(EnergyManagementSystemMeteredOutputVariable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>());
  //TODO setEMSVariableName to a valid default
  setUpdateFrequency("SystemTimestep");
  setResourceType("Electricity");
  setGroupType("Building");
  setEndUseCategory("Fans");
}

IddObjectType EnergyManagementSystemMeteredOutputVariable::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_MeteredOutputVariable);
}

std::vector<std::string> EnergyManagementSystemMeteredOutputVariable::updateFrequencyValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EnergyManagementSystem_MeteredOutputVariableFields::UpdateFrequency);
}

std::vector<std::string> EnergyManagementSystemMeteredOutputVariable::resourceTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EnergyManagementSystem_MeteredOutputVariableFields::ResourceType);
}

std::vector<std::string> EnergyManagementSystemMeteredOutputVariable::groupTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EnergyManagementSystem_MeteredOutputVariableFields::GroupType);
}

std::vector<std::string> EnergyManagementSystemMeteredOutputVariable::endUseCategoryValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EnergyManagementSystem_MeteredOutputVariableFields::EndUseCategory);
}

std::string EnergyManagementSystemMeteredOutputVariable::emsVariableName() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->emsVariableName();
}

std::string EnergyManagementSystemMeteredOutputVariable::updateFrequency() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->updateFrequency();
}

std::string EnergyManagementSystemMeteredOutputVariable::emsProgramOrSubroutineName() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->emsProgramOrSubroutineName();
}

boost::optional<EnergyManagementSystemProgram> EnergyManagementSystemMeteredOutputVariable::emsProgram() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->emsProgram();
}

boost::optional<EnergyManagementSystemSubroutine> EnergyManagementSystemMeteredOutputVariable::emsSubroutine() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->emsSubroutine();
}

std::string EnergyManagementSystemMeteredOutputVariable::resourceType() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->resourceType();
}

std::string EnergyManagementSystemMeteredOutputVariable::groupType() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->groupType();
}

std::string EnergyManagementSystemMeteredOutputVariable::endUseCategory() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->endUseCategory();
}

std::string EnergyManagementSystemMeteredOutputVariable::endUseSubcategory() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->endUseSubcategory();
}

std::string EnergyManagementSystemMeteredOutputVariable::units() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->units();
}


boost::optional<ModelObject> EnergyManagementSystemMeteredOutputVariable::emsVariableObject() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->emsVariableObject();
}

boost::optional<EnergyManagementSystemActuator> EnergyManagementSystemMeteredOutputVariable::emsActuator() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->emsActuator();
}

boost::optional<EnergyManagementSystemSensor> EnergyManagementSystemMeteredOutputVariable::emsSensor() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->emsSensor();
}

boost::optional<EnergyManagementSystemGlobalVariable> EnergyManagementSystemMeteredOutputVariable::emsGlobalVariable() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->emsGlobalVariable();
}

boost::optional<EnergyManagementSystemTrendVariable> EnergyManagementSystemMeteredOutputVariable::emsTrendVariable() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->emsTrendVariable();
}

boost::optional<EnergyManagementSystemInternalVariable> EnergyManagementSystemMeteredOutputVariable::emsInternalVariable() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->emsInternalVariable();
}

boost::optional<EnergyManagementSystemCurveOrTableIndexVariable> EnergyManagementSystemMeteredOutputVariable::emsCurveOrTableIndexVariable() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->emsCurveOrTableIndexVariable();
}

boost::optional<EnergyManagementSystemConstructionIndexVariable> EnergyManagementSystemMeteredOutputVariable::emsConstructionIndexVariable() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->emsConstructionIndexVariable();
}

bool EnergyManagementSystemMeteredOutputVariable::setUpdateFrequency(const std::string& updateFrequency) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setUpdateFrequency(updateFrequency);
}

bool EnergyManagementSystemMeteredOutputVariable::setEMSProgramOrSubroutineName(const EnergyManagementSystemProgram& program) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEMSProgramOrSubroutineName(program);
}

bool EnergyManagementSystemMeteredOutputVariable::setEMSProgramOrSubroutineName(const EnergyManagementSystemSubroutine& subroutine) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEMSProgramOrSubroutineName(subroutine);
}

void EnergyManagementSystemMeteredOutputVariable::resetEMSProgramOrSubroutineName() {
  getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->resetEMSProgramOrSubroutineName();
}

bool EnergyManagementSystemMeteredOutputVariable::setResourceType(const std::string& resourceType) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setResourceType(resourceType);
}

bool EnergyManagementSystemMeteredOutputVariable::setGroupType(const std::string& groupType) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setGroupType(groupType);
}

bool EnergyManagementSystemMeteredOutputVariable::setEndUseCategory(const std::string& endUseCategory) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEndUseCategory(endUseCategory);
}

bool EnergyManagementSystemMeteredOutputVariable::setEndUseSubcategory(const std::string& endUseSubcategory) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void EnergyManagementSystemMeteredOutputVariable::resetEndUseSubcategory() {
  getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->resetEndUseSubcategory();
}

bool EnergyManagementSystemMeteredOutputVariable::setUnits(const std::string& units) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setUnits(units);
}

void EnergyManagementSystemMeteredOutputVariable::resetUnits() {
  getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->resetUnits();
}

bool EnergyManagementSystemMeteredOutputVariable::setEMSVariableName(const std::string& eMSVariableName) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEMSVariableName(eMSVariableName);
}

bool EnergyManagementSystemMeteredOutputVariable::setEMSVariableName(const EnergyManagementSystemActuator& object) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemMeteredOutputVariable::setEMSVariableName(const EnergyManagementSystemSensor& object) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemMeteredOutputVariable::setEMSVariableName(const EnergyManagementSystemGlobalVariable& object) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemMeteredOutputVariable::setEMSVariableName(const EnergyManagementSystemTrendVariable& object) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemMeteredOutputVariable::setEMSVariableName(const EnergyManagementSystemInternalVariable& object) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemMeteredOutputVariable::setEMSVariableName(const EnergyManagementSystemCurveOrTableIndexVariable& object) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEMSVariableName(object);
}

bool EnergyManagementSystemMeteredOutputVariable::setEMSVariableName(const EnergyManagementSystemConstructionIndexVariable& object) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEMSVariableName(object);
}

/// @cond
EnergyManagementSystemMeteredOutputVariable::EnergyManagementSystemMeteredOutputVariable(std::shared_ptr<detail::EnergyManagementSystemMeteredOutputVariable_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

