/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "RefrigerationCondenserCascade.hpp"
#include "RefrigerationCondenserCascade_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "RefrigerationSystem.hpp"
#include "RefrigerationSystem_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Refrigeration_Condenser_Cascade_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    RefrigerationCondenserCascade_Impl::RefrigerationCondenserCascade_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == RefrigerationCondenserCascade::iddObjectType());
    }

    RefrigerationCondenserCascade_Impl::RefrigerationCondenserCascade_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == RefrigerationCondenserCascade::iddObjectType());
    }

    RefrigerationCondenserCascade_Impl::RefrigerationCondenserCascade_Impl(const RefrigerationCondenserCascade_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& RefrigerationCondenserCascade_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        // TODO implement checks and make sure there aren't any other variables
        // FOR CONDENSERS COOLING SYSTEMS SERVING CASES AND/OR WALKINS:
        "Refrigeration System Condenser Heat Transfer Rate", "Refrigeration System Condenser Heat Transfer Energy",
        // FOR CONDENSERS COOLING SYSTEMS SERVING AIR CHILLERS:
        "Refrigeration Air Chiller System Condenser Heat Transfer Rate", "Refrigeration Air Chiller System Condenser Heat Transfer Energy"

      };
      return result;
    }

    IddObjectType RefrigerationCondenserCascade_Impl::iddObjectType() const {
      return RefrigerationCondenserCascade::iddObjectType();
    }

    double RefrigerationCondenserCascade_Impl::ratedCondensingTemperature() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_CascadeFields::RatedCondensingTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double RefrigerationCondenserCascade_Impl::ratedApproachTemperatureDifference() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_CascadeFields::RatedApproachTemperatureDifference, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefrigerationCondenserCascade_Impl::isRatedApproachTemperatureDifferenceDefaulted() const {
      return isEmpty(OS_Refrigeration_Condenser_CascadeFields::RatedApproachTemperatureDifference);
    }

    double RefrigerationCondenserCascade_Impl::ratedEffectiveTotalHeatRejectionRate() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_CascadeFields::RatedEffectiveTotalHeatRejectionRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string RefrigerationCondenserCascade_Impl::condensingTemperatureControlType() const {
      boost::optional<std::string> value = getString(OS_Refrigeration_Condenser_CascadeFields::CondensingTemperatureControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefrigerationCondenserCascade_Impl::isCondensingTemperatureControlTypeDefaulted() const {
      return isEmpty(OS_Refrigeration_Condenser_CascadeFields::CondensingTemperatureControlType);
    }

    boost::optional<double> RefrigerationCondenserCascade_Impl::condenserRefrigerantOperatingChargeInventory() const {
      return getDouble(OS_Refrigeration_Condenser_CascadeFields::CondenserRefrigerantOperatingChargeInventory, true);
    }

    boost::optional<double> RefrigerationCondenserCascade_Impl::condensateReceiverRefrigerantInventory() const {
      return getDouble(OS_Refrigeration_Condenser_CascadeFields::CondensateReceiverRefrigerantInventory, true);
    }

    boost::optional<double> RefrigerationCondenserCascade_Impl::condensatePipingRefrigerantInventory() const {
      return getDouble(OS_Refrigeration_Condenser_CascadeFields::CondensatePipingRefrigerantInventory, true);
    }

    bool RefrigerationCondenserCascade_Impl::setRatedCondensingTemperature(double ratedCondensingTemperature) {
      bool result = setDouble(OS_Refrigeration_Condenser_CascadeFields::RatedCondensingTemperature, ratedCondensingTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool RefrigerationCondenserCascade_Impl::setRatedApproachTemperatureDifference(double ratedApproachTemperatureDifference) {
      bool result = setDouble(OS_Refrigeration_Condenser_CascadeFields::RatedApproachTemperatureDifference, ratedApproachTemperatureDifference);
      return result;
    }

    void RefrigerationCondenserCascade_Impl::resetRatedApproachTemperatureDifference() {
      bool result = setString(OS_Refrigeration_Condenser_CascadeFields::RatedApproachTemperatureDifference, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCondenserCascade_Impl::setRatedEffectiveTotalHeatRejectionRate(double ratedEffectiveTotalHeatRejectionRate) {
      bool result = setDouble(OS_Refrigeration_Condenser_CascadeFields::RatedEffectiveTotalHeatRejectionRate, ratedEffectiveTotalHeatRejectionRate);
      return result;
    }

    bool RefrigerationCondenserCascade_Impl::setCondensingTemperatureControlType(const std::string& condensingTemperatureControlType) {
      bool result = setString(OS_Refrigeration_Condenser_CascadeFields::CondensingTemperatureControlType, condensingTemperatureControlType);
      return result;
    }

    void RefrigerationCondenserCascade_Impl::resetCondensingTemperatureControlType() {
      bool result = setString(OS_Refrigeration_Condenser_CascadeFields::CondensingTemperatureControlType, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCondenserCascade_Impl::setCondenserRefrigerantOperatingChargeInventory(
      boost::optional<double> condenserRefrigerantOperatingChargeInventory) {
      bool result(false);
      if (condenserRefrigerantOperatingChargeInventory) {
        result = setDouble(OS_Refrigeration_Condenser_CascadeFields::CondenserRefrigerantOperatingChargeInventory,
                           condenserRefrigerantOperatingChargeInventory.get());
      } else {
        resetCondenserRefrigerantOperatingChargeInventory();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationCondenserCascade_Impl::resetCondenserRefrigerantOperatingChargeInventory() {
      bool result = setString(OS_Refrigeration_Condenser_CascadeFields::CondenserRefrigerantOperatingChargeInventory, "");
      OS_ASSERT(result);
    }

    bool
      RefrigerationCondenserCascade_Impl::setCondensateReceiverRefrigerantInventory(boost::optional<double> condensateReceiverRefrigerantInventory) {
      bool result(false);
      if (condensateReceiverRefrigerantInventory) {
        result =
          setDouble(OS_Refrigeration_Condenser_CascadeFields::CondensateReceiverRefrigerantInventory, condensateReceiverRefrigerantInventory.get());
      } else {
        resetCondensateReceiverRefrigerantInventory();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationCondenserCascade_Impl::resetCondensateReceiverRefrigerantInventory() {
      bool result = setString(OS_Refrigeration_Condenser_CascadeFields::CondensateReceiverRefrigerantInventory, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCondenserCascade_Impl::setCondensatePipingRefrigerantInventory(boost::optional<double> condensatePipingRefrigerantInventory) {
      bool result(false);
      if (condensatePipingRefrigerantInventory) {
        result =
          setDouble(OS_Refrigeration_Condenser_CascadeFields::CondensatePipingRefrigerantInventory, condensatePipingRefrigerantInventory.get());
      } else {
        resetCondensatePipingRefrigerantInventory();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationCondenserCascade_Impl::resetCondensatePipingRefrigerantInventory() {
      bool result = setString(OS_Refrigeration_Condenser_CascadeFields::CondensatePipingRefrigerantInventory, "");
      OS_ASSERT(result);
    }

    boost::optional<RefrigerationSystem> RefrigerationCondenserCascade_Impl::system() const {
      boost::optional<RefrigerationSystem> result;

      auto refrigerationCondenserCascade = this->getObject<RefrigerationCondenserCascade>();
      for (const RefrigerationSystem& refrigerationSystem : this->model().getConcreteModelObjects<RefrigerationSystem>()) {
        RefrigerationCondenserCascadeVector refrigerationCondenserCascades = refrigerationSystem.cascadeCondenserLoads();
        if (!refrigerationCondenserCascades.empty()
            && std::find(refrigerationCondenserCascades.begin(), refrigerationCondenserCascades.end(), refrigerationCondenserCascade)
                 != refrigerationCondenserCascades.end()) {
          result = refrigerationSystem;
          break;
        }
      }

      return result;
    }

    void RefrigerationCondenserCascade_Impl::removeFromSystem() {
      if (boost::optional<RefrigerationSystem> refrigerationSystem = this->system()) {
        auto refrigerationCondenserCascade = this->getObject<RefrigerationCondenserCascade>();
        refrigerationSystem->removeCascadeCondenserLoad(refrigerationCondenserCascade);
      }
    }

    boost::optional<RefrigerationSystem> RefrigerationCondenserCascade_Impl::heatRejectingSystem() const {
      boost::optional<RefrigerationSystem> result;

      auto refrigerationCondenserCascade = this->getObject<RefrigerationCondenserCascade>();
      for (const RefrigerationSystem& refrigerationSystem : this->model().getConcreteModelObjects<RefrigerationSystem>()) {
        if (auto cond = refrigerationSystem.refrigerationCondenser()) {
          if (cond.get() == refrigerationCondenserCascade) {
            result = refrigerationSystem;
            break;
          }
        }
      }
      return result;
    }

    void RefrigerationCondenserCascade_Impl::removeFromHeatRejectingSystem() {
      if (boost::optional<RefrigerationSystem> refrigerationSystem = this->heatRejectingSystem()) {
        refrigerationSystem->resetRefrigerationCondenser();
      }
    }

    std::vector<IdfObject> RefrigerationCondenserCascade_Impl::remove() {
      // Remove from ModelObjectList in RefrigerationSystem(s) if needed
      this->removeFromSystem();

      // It's uncessary to remove it from the heatRejectingSystem since it's a classic object-list field

      return ModelObject_Impl::remove();
    }

  }  // namespace detail

  RefrigerationCondenserCascade::RefrigerationCondenserCascade(const Model& model)
    : ModelObject(RefrigerationCondenserCascade::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::RefrigerationCondenserCascade_Impl>());

    //  -4.0,                    !- Rated Condensing Temperature {C}
    setRatedCondensingTemperature(-4.0);

    //  3.,                      !- Rated Approach Temperature Difference {DeltaC}
    setRatedApproachTemperatureDifference(3.0);

    //  20000.,                  !- Rated Effective Total Heat Rejection Rate {W}
    setRatedEffectiveTotalHeatRejectionRate(20000.0);

    //  Fixed;                   !- Condensing Temperature Control Type
    setCondensingTemperatureControlType("Fixed");
  }

  IddObjectType RefrigerationCondenserCascade::iddObjectType() {
    return {IddObjectType::OS_Refrigeration_Condenser_Cascade};
  }

  std::vector<std::string> RefrigerationCondenserCascade::condensingTemperatureControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Refrigeration_Condenser_CascadeFields::CondensingTemperatureControlType);
  }

  double RefrigerationCondenserCascade::ratedCondensingTemperature() const {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->ratedCondensingTemperature();
  }

  double RefrigerationCondenserCascade::ratedApproachTemperatureDifference() const {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->ratedApproachTemperatureDifference();
  }

  bool RefrigerationCondenserCascade::isRatedApproachTemperatureDifferenceDefaulted() const {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->isRatedApproachTemperatureDifferenceDefaulted();
  }

  double RefrigerationCondenserCascade::ratedEffectiveTotalHeatRejectionRate() const {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->ratedEffectiveTotalHeatRejectionRate();
  }

  std::string RefrigerationCondenserCascade::condensingTemperatureControlType() const {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->condensingTemperatureControlType();
  }

  bool RefrigerationCondenserCascade::isCondensingTemperatureControlTypeDefaulted() const {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->isCondensingTemperatureControlTypeDefaulted();
  }

  boost::optional<double> RefrigerationCondenserCascade::condenserRefrigerantOperatingChargeInventory() const {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->condenserRefrigerantOperatingChargeInventory();
  }

  boost::optional<double> RefrigerationCondenserCascade::condensateReceiverRefrigerantInventory() const {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->condensateReceiverRefrigerantInventory();
  }

  boost::optional<double> RefrigerationCondenserCascade::condensatePipingRefrigerantInventory() const {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->condensatePipingRefrigerantInventory();
  }

  bool RefrigerationCondenserCascade::setRatedCondensingTemperature(double ratedCondensingTemperature) {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->setRatedCondensingTemperature(ratedCondensingTemperature);
  }

  bool RefrigerationCondenserCascade::setRatedApproachTemperatureDifference(double ratedApproachTemperatureDifference) {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->setRatedApproachTemperatureDifference(ratedApproachTemperatureDifference);
  }

  void RefrigerationCondenserCascade::resetRatedApproachTemperatureDifference() {
    getImpl<detail::RefrigerationCondenserCascade_Impl>()->resetRatedApproachTemperatureDifference();
  }

  bool RefrigerationCondenserCascade::setRatedEffectiveTotalHeatRejectionRate(double ratedEffectiveTotalHeatRejectionRate) {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->setRatedEffectiveTotalHeatRejectionRate(ratedEffectiveTotalHeatRejectionRate);
  }

  bool RefrigerationCondenserCascade::setCondensingTemperatureControlType(const std::string& condensingTemperatureControlType) {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->setCondensingTemperatureControlType(condensingTemperatureControlType);
  }

  void RefrigerationCondenserCascade::resetCondensingTemperatureControlType() {
    getImpl<detail::RefrigerationCondenserCascade_Impl>()->resetCondensingTemperatureControlType();
  }

  bool RefrigerationCondenserCascade::setCondenserRefrigerantOperatingChargeInventory(double condenserRefrigerantOperatingChargeInventory) {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->setCondenserRefrigerantOperatingChargeInventory(
      condenserRefrigerantOperatingChargeInventory);
  }

  void RefrigerationCondenserCascade::resetCondenserRefrigerantOperatingChargeInventory() {
    getImpl<detail::RefrigerationCondenserCascade_Impl>()->resetCondenserRefrigerantOperatingChargeInventory();
  }

  bool RefrigerationCondenserCascade::setCondensateReceiverRefrigerantInventory(double condensateReceiverRefrigerantInventory) {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->setCondensateReceiverRefrigerantInventory(condensateReceiverRefrigerantInventory);
  }

  void RefrigerationCondenserCascade::resetCondensateReceiverRefrigerantInventory() {
    getImpl<detail::RefrigerationCondenserCascade_Impl>()->resetCondensateReceiverRefrigerantInventory();
  }

  bool RefrigerationCondenserCascade::setCondensatePipingRefrigerantInventory(double condensatePipingRefrigerantInventory) {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->setCondensatePipingRefrigerantInventory(condensatePipingRefrigerantInventory);
  }

  void RefrigerationCondenserCascade::resetCondensatePipingRefrigerantInventory() {
    getImpl<detail::RefrigerationCondenserCascade_Impl>()->resetCondensatePipingRefrigerantInventory();
  }

  boost::optional<RefrigerationSystem> RefrigerationCondenserCascade::system() const {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->system();
  }

  void RefrigerationCondenserCascade::removeFromSystem() {
    getImpl<detail::RefrigerationCondenserCascade_Impl>()->removeFromSystem();
  }

  boost::optional<RefrigerationSystem> RefrigerationCondenserCascade::heatRejectingSystem() const {
    return getImpl<detail::RefrigerationCondenserCascade_Impl>()->heatRejectingSystem();
  }

  void RefrigerationCondenserCascade::removeFromHeatRejectingSystem() {
    getImpl<detail::RefrigerationCondenserCascade_Impl>()->removeFromHeatRejectingSystem();
  }

  /// @cond
  RefrigerationCondenserCascade::RefrigerationCondenserCascade(std::shared_ptr<detail::RefrigerationCondenserCascade_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
