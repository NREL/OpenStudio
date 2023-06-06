/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExternalInterfaceActuator.hpp"
#include "ExternalInterfaceActuator_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_Actuator_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExternalInterfaceActuator_Impl::ExternalInterfaceActuator_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceActuator::iddObjectType());
    }

    ExternalInterfaceActuator_Impl::ExternalInterfaceActuator_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                   bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExternalInterfaceActuator::iddObjectType());
    }

    ExternalInterfaceActuator_Impl::ExternalInterfaceActuator_Impl(const ExternalInterfaceActuator_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ExternalInterfaceActuator_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ExternalInterfaceActuator_Impl::iddObjectType() const {
      return ExternalInterfaceActuator::iddObjectType();
    }

    ModelObject ExternalInterfaceActuator_Impl::actuatedComponentUnique() const {

      return this->getTarget(OS_ExternalInterface_ActuatorFields::ActuatedComponentUniqueName)->cast<ModelObject>();
    }

    std::string ExternalInterfaceActuator_Impl::actuatedComponentType() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_ActuatorFields::ActuatedComponentType, true);
      if (value) {
        return value.get();
      } else {
        return "";
      }
    }

    std::string ExternalInterfaceActuator_Impl::actuatedComponentControlType() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_ActuatorFields::ActuatedComponentControlType, true);
      if (value) {
        return value.get();
      } else {
        return "";
      }
    }

    boost::optional<double> ExternalInterfaceActuator_Impl::optionalInitialValue() const {
      return getDouble(OS_ExternalInterface_ActuatorFields::OptionalInitialValue, true);
    }

    bool ExternalInterfaceActuator_Impl::setActuatedComponentUnique(const ModelObject& modelObject) {
      bool result = setPointer(OS_ExternalInterface_ActuatorFields::ActuatedComponentUniqueName, modelObject.handle());
      return result;
    }

    bool ExternalInterfaceActuator_Impl::setActuatedComponentType(const std::string& actuatedComponentType) {
      bool result = setString(OS_ExternalInterface_ActuatorFields::ActuatedComponentType, actuatedComponentType);
      return result;
    }

    bool ExternalInterfaceActuator_Impl::setActuatedComponentControlType(const std::string& actuatedComponentControlType) {
      bool result = setString(OS_ExternalInterface_ActuatorFields::ActuatedComponentControlType, actuatedComponentControlType);
      return result;
    }

    bool ExternalInterfaceActuator_Impl::setOptionalInitialValue(double optionalInitialValue) {
      bool result = setDouble(OS_ExternalInterface_ActuatorFields::OptionalInitialValue, optionalInitialValue);
      OS_ASSERT(result);
      return result;
    }

    void ExternalInterfaceActuator_Impl::resetOptionalInitialValue() {
      bool result = setString(OS_ExternalInterface_ActuatorFields::OptionalInitialValue, "");
      OS_ASSERT(result);
    }

    bool ExternalInterfaceActuator_Impl::exportToBCVTB() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_ActuatorFields::ExportToBCVTB, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "True");
    }

    bool ExternalInterfaceActuator_Impl::isExportToBCVTBDefaulted() const {
      return isEmpty(OS_ExternalInterface_ActuatorFields::ExportToBCVTB);
    }

    bool ExternalInterfaceActuator_Impl::setExportToBCVTB(bool exportToBCVTB) {
      bool result = false;
      if (exportToBCVTB) {
        result = setString(OS_ExternalInterface_ActuatorFields::ExportToBCVTB, "True");
      } else {
        result = setString(OS_ExternalInterface_ActuatorFields::ExportToBCVTB, "False");
      }
      OS_ASSERT(result);
      return result;
    }

    void ExternalInterfaceActuator_Impl::resetExportToBCVTB() {
      bool result = setString(OS_ExternalInterface_ActuatorFields::ExportToBCVTB, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  ExternalInterfaceActuator::ExternalInterfaceActuator(const ModelObject& modelObject, const std::string actuatedComponentType,
                                                       const std::string actuatedComponentControlType)
    : ModelObject(ExternalInterfaceActuator::iddObjectType(), modelObject.model()) {
    OS_ASSERT(getImpl<detail::ExternalInterfaceActuator_Impl>());

    bool ok = setActuatedComponentUnique(modelObject);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s setActuatedComponentUnique to " << modelObject.briefDescription() << ".");
    }
    ok = setActuatedComponentType(actuatedComponentType);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s actuatedComponentType to " << actuatedComponentType << ".");
    }
    ok = setActuatedComponentControlType(actuatedComponentControlType);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s actuatedComponentControlType to " << actuatedComponentControlType << ".");
    }
  }

  ExternalInterfaceActuator::ExternalInterfaceActuator(const ModelObject& modelObject)
    : ModelObject(ExternalInterfaceActuator::iddObjectType(), modelObject.model()) {
    OS_ASSERT(getImpl<detail::ExternalInterfaceActuator_Impl>());

    bool ok = setActuatedComponentUnique(modelObject);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s setActuatedComponentUnique to " << modelObject.briefDescription() << ".");
    }
    //TODO set below to some appropriate default that depends on the modelObject::IddType
    // setActuatedComponentType();
    // setActuatedComponentControlType();
  }

  IddObjectType ExternalInterfaceActuator::iddObjectType() {
    return {IddObjectType::OS_ExternalInterface_Actuator};
  }

  ModelObject ExternalInterfaceActuator::actuatedComponentUnique() const {
    return getImpl<detail::ExternalInterfaceActuator_Impl>()->actuatedComponentUnique();
  }

  std::string ExternalInterfaceActuator::actuatedComponentType() const {
    return getImpl<detail::ExternalInterfaceActuator_Impl>()->actuatedComponentType();
  }

  std::string ExternalInterfaceActuator::actuatedComponentControlType() const {
    return getImpl<detail::ExternalInterfaceActuator_Impl>()->actuatedComponentControlType();
  }

  boost::optional<double> ExternalInterfaceActuator::optionalInitialValue() const {
    return getImpl<detail::ExternalInterfaceActuator_Impl>()->optionalInitialValue();
  }

  bool ExternalInterfaceActuator::setActuatedComponentUnique(const ModelObject& modelObject) {
    return getImpl<detail::ExternalInterfaceActuator_Impl>()->setActuatedComponentUnique(modelObject);
  }

  bool ExternalInterfaceActuator::setActuatedComponentType(const std::string& actuatedComponentType) {
    return getImpl<detail::ExternalInterfaceActuator_Impl>()->setActuatedComponentType(actuatedComponentType);
  }

  bool ExternalInterfaceActuator::setActuatedComponentControlType(const std::string& actuatedComponentControlType) {
    return getImpl<detail::ExternalInterfaceActuator_Impl>()->setActuatedComponentControlType(actuatedComponentControlType);
  }

  bool ExternalInterfaceActuator::setOptionalInitialValue(double optionalInitialValue) {
    return getImpl<detail::ExternalInterfaceActuator_Impl>()->setOptionalInitialValue(optionalInitialValue);
  }

  void ExternalInterfaceActuator::resetOptionalInitialValue() {
    getImpl<detail::ExternalInterfaceActuator_Impl>()->resetOptionalInitialValue();
  }

  bool ExternalInterfaceActuator::exportToBCVTB() const {
    return getImpl<detail::ExternalInterfaceActuator_Impl>()->exportToBCVTB();
  }

  bool ExternalInterfaceActuator::isExportToBCVTBDefaulted() const {
    return getImpl<detail::ExternalInterfaceActuator_Impl>()->isExportToBCVTBDefaulted();
  }

  bool ExternalInterfaceActuator::setExportToBCVTB(bool exportToBCVTB) {
    return getImpl<detail::ExternalInterfaceActuator_Impl>()->setExportToBCVTB(exportToBCVTB);
  }

  void ExternalInterfaceActuator::resetExportToBCVTB() {
    getImpl<detail::ExternalInterfaceActuator_Impl>()->resetExportToBCVTB();
  }

  /// @cond
  ExternalInterfaceActuator::ExternalInterfaceActuator(std::shared_ptr<detail::ExternalInterfaceActuator_Impl> impl) : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
