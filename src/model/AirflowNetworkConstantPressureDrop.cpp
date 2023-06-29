/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirflowNetworkConstantPressureDrop.hpp"
#include "AirflowNetworkConstantPressureDrop_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkConstantPressureDrop_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirflowNetworkConstantPressureDrop_Impl::AirflowNetworkConstantPressureDrop_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkConstantPressureDrop::iddObjectType());
    }

    AirflowNetworkConstantPressureDrop_Impl::AirflowNetworkConstantPressureDrop_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkConstantPressureDrop::iddObjectType());
    }

    AirflowNetworkConstantPressureDrop_Impl::AirflowNetworkConstantPressureDrop_Impl(const AirflowNetworkConstantPressureDrop_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkConstantPressureDrop_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkConstantPressureDrop_Impl::iddObjectType() const {
      return AirflowNetworkConstantPressureDrop::iddObjectType();
    }

    double AirflowNetworkConstantPressureDrop_Impl::pressureDrop() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkConstantPressureDropFields::PressureDifferenceAcrosstheComponent, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkConstantPressureDrop_Impl::setPressureDrop(double pressureDrop) {
      bool result = setDouble(OS_AirflowNetworkConstantPressureDropFields::PressureDifferenceAcrosstheComponent, pressureDrop);
      return result;
    }

  }  // namespace detail

  //AirflowNetworkConstantPressureDrop::AirflowNetworkConstantPressureDrop(const Model& model)
  //  : ModelObject(AirflowNetworkConstantPressureDrop::iddObjectType(),model)
  //{
  //  OS_ASSERT(getImpl<detail::AirflowNetworkConstantPressureDrop_Impl>());
  //}

  AirflowNetworkConstantPressureDrop::AirflowNetworkConstantPressureDrop(const Model& model, double pressureDrop)
    : ModelObject(AirflowNetworkConstantPressureDrop::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkConstantPressureDrop_Impl>());

    bool ok = setPressureDrop(pressureDrop);
    OS_ASSERT(ok);
  }

  IddObjectType AirflowNetworkConstantPressureDrop::iddObjectType() {
    return {IddObjectType::OS_AirflowNetworkConstantPressureDrop};
  }

  double AirflowNetworkConstantPressureDrop::pressureDrop() const {
    return getImpl<detail::AirflowNetworkConstantPressureDrop_Impl>()->pressureDrop();
  }

  bool AirflowNetworkConstantPressureDrop::setPressureDrop(double pressureDrop) {
    return getImpl<detail::AirflowNetworkConstantPressureDrop_Impl>()->setPressureDrop(pressureDrop);
  }

  /// @cond
  AirflowNetworkConstantPressureDrop::AirflowNetworkConstantPressureDrop(std::shared_ptr<detail::AirflowNetworkConstantPressureDrop_Impl> impl)
    : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
