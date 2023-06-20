/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirflowNetworkZoneExhaustFan.hpp"
#include "AirflowNetworkZoneExhaustFan_Impl.hpp"

#include "FanZoneExhaust.hpp"
#include "FanZoneExhaust_Impl.hpp"
#include "AirflowNetworkCrack.hpp"
#include "AirflowNetworkCrack_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkZoneExhaustFan_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirflowNetworkZoneExhaustFan_Impl::AirflowNetworkZoneExhaustFan_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkZoneExhaustFan::iddObjectType());
    }

    AirflowNetworkZoneExhaustFan_Impl::AirflowNetworkZoneExhaustFan_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                         bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkZoneExhaustFan::iddObjectType());
    }

    AirflowNetworkZoneExhaustFan_Impl::AirflowNetworkZoneExhaustFan_Impl(const AirflowNetworkZoneExhaustFan_Impl& other, Model_Impl* model,
                                                                         bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkZoneExhaustFan_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkZoneExhaustFan_Impl::iddObjectType() const {
      return AirflowNetworkZoneExhaustFan::iddObjectType();
    }

    boost::optional<FanZoneExhaust> AirflowNetworkZoneExhaustFan_Impl::fanZoneExhaust() const {
      boost::optional<FanZoneExhaust> value =
        getObject<ModelObject>().getModelObjectTarget<FanZoneExhaust>(OS_AirflowNetworkZoneExhaustFanFields::Name);
      return value;
    }

    boost::optional<AirflowNetworkCrack> AirflowNetworkZoneExhaustFan_Impl::crack() const {
      boost::optional<AirflowNetworkCrack> value =
        getObject<ModelObject>().getModelObjectTarget<AirflowNetworkCrack>(OS_AirflowNetworkZoneExhaustFanFields::CrackName);
      return value;
    }

    bool AirflowNetworkZoneExhaustFan_Impl::setCrack(const AirflowNetworkCrack& crack) {
      return setPointer(OS_AirflowNetworkZoneExhaustFanFields::CrackName, crack.handle());
    }

    void AirflowNetworkZoneExhaustFan_Impl::resetCrack() {
      bool result = setString(OS_AirflowNetworkZoneExhaustFanFields::CrackName, "");
      OS_ASSERT(result);
    }

    void AirflowNetworkZoneExhaustFan_Impl::resetFanZoneExhaust() {
      bool result = setString(OS_AirflowNetworkZoneExhaustFanFields::Name, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  AirflowNetworkZoneExhaustFan::AirflowNetworkZoneExhaustFan(const Model& model, const AirflowNetworkCrack& crack, const Handle& handle)
    : ModelObject(AirflowNetworkZoneExhaustFan::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>());
    bool ok = setCrack(crack);
    OS_ASSERT(ok);
    ok = getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setPointer(OS_AirflowNetworkZoneExhaustFanFields::Name, handle);
    OS_ASSERT(ok);
  }

  IddObjectType AirflowNetworkZoneExhaustFan::iddObjectType() {
    return {IddObjectType::OS_AirflowNetworkZoneExhaustFan};
  }

  boost::optional<FanZoneExhaust> AirflowNetworkZoneExhaustFan::fanZoneExhaust() const {
    return getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->fanZoneExhaust();
  }

  boost::optional<AirflowNetworkCrack> AirflowNetworkZoneExhaustFan::crack() const {
    return getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->crack();
  }

  bool AirflowNetworkZoneExhaustFan::setCrack(const AirflowNetworkCrack& crack) {
    return getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->setCrack(crack);
  }

  void AirflowNetworkZoneExhaustFan::resetCrack() {
    getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->resetCrack();
  }

  void AirflowNetworkZoneExhaustFan::resetFanZoneExhaust() {
    getImpl<detail::AirflowNetworkZoneExhaustFan_Impl>()->resetFanZoneExhaust();
  }

  /// @cond
  AirflowNetworkZoneExhaustFan::AirflowNetworkZoneExhaustFan(std::shared_ptr<detail::AirflowNetworkZoneExhaustFan_Impl> impl) : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
