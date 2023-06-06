/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExteriorLightsDefinition.hpp"
#include "ExteriorLightsDefinition_Impl.hpp"

#include <utilities/idd/OS_Exterior_Lights_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExteriorLightsDefinition_Impl::ExteriorLightsDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ExteriorLoadDefinition_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExteriorLightsDefinition::iddObjectType());
    }

    ExteriorLightsDefinition_Impl::ExteriorLightsDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : ExteriorLoadDefinition_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExteriorLightsDefinition::iddObjectType());
    }

    ExteriorLightsDefinition_Impl::ExteriorLightsDefinition_Impl(const ExteriorLightsDefinition_Impl& other, Model_Impl* model, bool keepHandle)
      : ExteriorLoadDefinition_Impl(other, model, keepHandle) {}

    // TODO: remove
    const std::vector<std::string>& ExteriorLightsDefinition_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ExteriorLightsDefinition_Impl::iddObjectType() const {
      return ExteriorLightsDefinition::iddObjectType();
    }

    double ExteriorLightsDefinition_Impl::designLevel() const {
      boost::optional<double> value = getDouble(OS_Exterior_Lights_DefinitionFields::DesignLevel, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ExteriorLightsDefinition_Impl::setDesignLevel(double designLevel) {
      bool result = setDouble(OS_Exterior_Lights_DefinitionFields::DesignLevel, designLevel);
      return result;
    }

  }  // namespace detail

  ExteriorLightsDefinition::ExteriorLightsDefinition(const Model& model) : ExteriorLoadDefinition(ExteriorLightsDefinition::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ExteriorLightsDefinition_Impl>());

    bool ok = setDesignLevel(0.0);
    OS_ASSERT(ok);
  }

  IddObjectType ExteriorLightsDefinition::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Exterior_Lights_Definition);
    return result;
  }

  double ExteriorLightsDefinition::designLevel() const {
    return getImpl<detail::ExteriorLightsDefinition_Impl>()->designLevel();
  }

  bool ExteriorLightsDefinition::setDesignLevel(double designLevel) {
    return getImpl<detail::ExteriorLightsDefinition_Impl>()->setDesignLevel(designLevel);
  }

  /// @cond
  ExteriorLightsDefinition::ExteriorLightsDefinition(std::shared_ptr<detail::ExteriorLightsDefinition_Impl> impl)
    : ExteriorLoadDefinition(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
