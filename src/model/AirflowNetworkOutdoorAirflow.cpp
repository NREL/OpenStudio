/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirflowNetworkOutdoorAirflow.hpp"
#include "AirflowNetworkOutdoorAirflow_Impl.hpp"

#include "ControllerOutdoorAir.hpp"
#include "ControllerOutdoorAir_Impl.hpp"
#include "AirflowNetworkCrack.hpp"
#include "AirflowNetworkCrack_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkOutdoorAirflow_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirflowNetworkOutdoorAirflow_Impl::AirflowNetworkOutdoorAirflow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkOutdoorAirflow::iddObjectType());
    }

    AirflowNetworkOutdoorAirflow_Impl::AirflowNetworkOutdoorAirflow_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                         bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkOutdoorAirflow::iddObjectType());
    }

    AirflowNetworkOutdoorAirflow_Impl::AirflowNetworkOutdoorAirflow_Impl(const AirflowNetworkOutdoorAirflow_Impl& other, Model_Impl* model,
                                                                         bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkOutdoorAirflow_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkOutdoorAirflow_Impl::iddObjectType() const {
      return AirflowNetworkOutdoorAirflow::iddObjectType();
    }

    boost::optional<ControllerOutdoorAir> AirflowNetworkOutdoorAirflow_Impl::controllerOutdoorAir() const {
      boost::optional<ControllerOutdoorAir> value =
        getObject<ModelObject>().getModelObjectTarget<ControllerOutdoorAir>(OS_AirflowNetworkOutdoorAirflowFields::Name);
      return value;
    }

    boost::optional<AirflowNetworkCrack> AirflowNetworkOutdoorAirflow_Impl::crack() const {
      boost::optional<AirflowNetworkCrack> value =
        getObject<ModelObject>().getModelObjectTarget<AirflowNetworkCrack>(OS_AirflowNetworkOutdoorAirflowFields::CrackName);
      return value;
    }

    bool AirflowNetworkOutdoorAirflow_Impl::setCrack(const AirflowNetworkCrack& crack) {
      return setPointer(OS_AirflowNetworkOutdoorAirflowFields::CrackName, crack.handle());
    }

    void AirflowNetworkOutdoorAirflow_Impl::resetCrack() {
      bool result = setString(OS_AirflowNetworkOutdoorAirflowFields::CrackName, "");
      OS_ASSERT(result);
    }

    void AirflowNetworkOutdoorAirflow_Impl::resetControllerOutdoorAir() {
      bool result = setString(OS_AirflowNetworkOutdoorAirflowFields::Name, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  AirflowNetworkOutdoorAirflow::AirflowNetworkOutdoorAirflow(const Model& model, const AirflowNetworkCrack& crack, const Handle& handle)
    : ModelObject(AirflowNetworkOutdoorAirflow::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkOutdoorAirflow_Impl>());
    bool ok = setCrack(crack);
    OS_ASSERT(ok);
    ok = getImpl<detail::AirflowNetworkOutdoorAirflow_Impl>()->setPointer(OS_AirflowNetworkOutdoorAirflowFields::Name, handle);
    OS_ASSERT(ok);
  }

  IddObjectType AirflowNetworkOutdoorAirflow::iddObjectType() {
    return {IddObjectType::OS_AirflowNetworkOutdoorAirflow};
  }

  boost::optional<ControllerOutdoorAir> AirflowNetworkOutdoorAirflow::controllerOutdoorAir() const {
    return getImpl<detail::AirflowNetworkOutdoorAirflow_Impl>()->controllerOutdoorAir();
  }

  boost::optional<AirflowNetworkCrack> AirflowNetworkOutdoorAirflow::crack() const {
    return getImpl<detail::AirflowNetworkOutdoorAirflow_Impl>()->crack();
  }

  bool AirflowNetworkOutdoorAirflow::setCrack(const AirflowNetworkCrack& crack) {
    return getImpl<detail::AirflowNetworkOutdoorAirflow_Impl>()->setCrack(crack);
  }

  void AirflowNetworkOutdoorAirflow::resetCrack() {
    getImpl<detail::AirflowNetworkOutdoorAirflow_Impl>()->resetCrack();
  }

  void AirflowNetworkOutdoorAirflow::resetControllerOutdoorAir() {
    getImpl<detail::AirflowNetworkOutdoorAirflow_Impl>()->resetControllerOutdoorAir();
  }

  /// @cond
  AirflowNetworkOutdoorAirflow::AirflowNetworkOutdoorAirflow(std::shared_ptr<detail::AirflowNetworkOutdoorAirflow_Impl> impl) : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
