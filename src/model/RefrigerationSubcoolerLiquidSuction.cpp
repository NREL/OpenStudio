/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "RefrigerationSubcoolerLiquidSuction.hpp"
#include "RefrigerationSubcoolerLiquidSuction_Impl.hpp"

#include "RefrigerationSystem.hpp"
#include "RefrigerationSystem_Impl.hpp"

#include <utilities/idd/OS_Refrigeration_Subcooler_LiquidSuction_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    RefrigerationSubcoolerLiquidSuction_Impl::RefrigerationSubcoolerLiquidSuction_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == RefrigerationSubcoolerLiquidSuction::iddObjectType());
    }

    RefrigerationSubcoolerLiquidSuction_Impl::RefrigerationSubcoolerLiquidSuction_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == RefrigerationSubcoolerLiquidSuction::iddObjectType());
    }

    RefrigerationSubcoolerLiquidSuction_Impl::RefrigerationSubcoolerLiquidSuction_Impl(const RefrigerationSubcoolerLiquidSuction_Impl& other,
                                                                                       Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& RefrigerationSubcoolerLiquidSuction_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{// TODO: implement checks
                                                   // FOR SUBCOOLERS ON SYSTEMS SERVING CASES AND/OR WALKINS:
                                                   "Refrigeration System Liquid Suction Subcooler Heat Transfer Rate",
                                                   "Refrigeration System Liquid Suction Subcooler Heat Transfer Energy",
                                                   // FOR SUBCOOLERS ON SYSTEMS SERVING AIR CHILLERS:
                                                   "Refrigeration Air Chiller System Liquid Suction Subcooler Heat Transfer Rate",
                                                   "Refrigeration Air Chiller System Liquid Suction Subcooler Heat Transfer Energy"};
      return result;
    }

    IddObjectType RefrigerationSubcoolerLiquidSuction_Impl::iddObjectType() const {
      return RefrigerationSubcoolerLiquidSuction::iddObjectType();
    }

    boost::optional<double> RefrigerationSubcoolerLiquidSuction_Impl::liquidSuctionDesignSubcoolingTemperatureDifference() const {
      return getDouble(OS_Refrigeration_Subcooler_LiquidSuctionFields::LiquidSuctionDesignSubcoolingTemperatureDifference, true);
    }

    boost::optional<double> RefrigerationSubcoolerLiquidSuction_Impl::designLiquidInletTemperature() const {
      return getDouble(OS_Refrigeration_Subcooler_LiquidSuctionFields::DesignLiquidInletTemperature, true);
    }

    boost::optional<double> RefrigerationSubcoolerLiquidSuction_Impl::designVaporInletTemperature() const {
      return getDouble(OS_Refrigeration_Subcooler_LiquidSuctionFields::DesignVaporInletTemperature, true);
    }

    bool RefrigerationSubcoolerLiquidSuction_Impl::setLiquidSuctionDesignSubcoolingTemperatureDifference(
      boost::optional<double> liquidSuctionDesignSubcoolingTemperatureDifference) {
      bool result(false);
      if (liquidSuctionDesignSubcoolingTemperatureDifference) {
        result = setDouble(OS_Refrigeration_Subcooler_LiquidSuctionFields::LiquidSuctionDesignSubcoolingTemperatureDifference,
                           liquidSuctionDesignSubcoolingTemperatureDifference.get());
      } else {
        resetLiquidSuctionDesignSubcoolingTemperatureDifference();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationSubcoolerLiquidSuction_Impl::resetLiquidSuctionDesignSubcoolingTemperatureDifference() {
      bool result = setString(OS_Refrigeration_Subcooler_LiquidSuctionFields::LiquidSuctionDesignSubcoolingTemperatureDifference, "");
      OS_ASSERT(result);
    }

    bool RefrigerationSubcoolerLiquidSuction_Impl::setDesignLiquidInletTemperature(boost::optional<double> designLiquidInletTemperature) {
      bool result(false);
      if (designLiquidInletTemperature) {
        result = setDouble(OS_Refrigeration_Subcooler_LiquidSuctionFields::DesignLiquidInletTemperature, designLiquidInletTemperature.get());
      } else {
        resetDesignLiquidInletTemperature();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationSubcoolerLiquidSuction_Impl::resetDesignLiquidInletTemperature() {
      bool result = setString(OS_Refrigeration_Subcooler_LiquidSuctionFields::DesignLiquidInletTemperature, "");
      OS_ASSERT(result);
    }

    bool RefrigerationSubcoolerLiquidSuction_Impl::setDesignVaporInletTemperature(boost::optional<double> designVaporInletTemperature) {
      bool result(false);
      if (designVaporInletTemperature) {
        result = setDouble(OS_Refrigeration_Subcooler_LiquidSuctionFields::DesignVaporInletTemperature, designVaporInletTemperature.get());
      } else {
        resetDesignVaporInletTemperature();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationSubcoolerLiquidSuction_Impl::resetDesignVaporInletTemperature() {
      bool result = setString(OS_Refrigeration_Subcooler_LiquidSuctionFields::DesignVaporInletTemperature, "");
      OS_ASSERT(result);
    }

    boost::optional<RefrigerationSystem> RefrigerationSubcoolerLiquidSuction_Impl::system() const {

      boost::optional<RefrigerationSystem> system;
      // We use getModelObjectSources to check if more than one
      std::vector<RefrigerationSystem> systems =
        getObject<ModelObject>().getModelObjectSources<RefrigerationSystem>(RefrigerationSystem::iddObjectType());

      if (!systems.empty()) {
        if (systems.size() > 1u) {
          LOG(Error, briefDescription() << " is referenced by more than one RefrigerationSystem, returning the first");
        }
        system = systems[0];
      }
      return system;
    }

  }  // namespace detail

  RefrigerationSubcoolerLiquidSuction::RefrigerationSubcoolerLiquidSuction(const Model& model)
    : ModelObject(RefrigerationSubcoolerLiquidSuction::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>());

    // Brian's values
    setLiquidSuctionDesignSubcoolingTemperatureDifference(6.0);

    // Brian's values
    setDesignLiquidInletTemperature(16.0);

    // Brian's values
    setDesignVaporInletTemperature(0.0);
  }

  IddObjectType RefrigerationSubcoolerLiquidSuction::iddObjectType() {
    return {IddObjectType::OS_Refrigeration_Subcooler_LiquidSuction};
  }

  boost::optional<double> RefrigerationSubcoolerLiquidSuction::liquidSuctionDesignSubcoolingTemperatureDifference() const {
    return getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->liquidSuctionDesignSubcoolingTemperatureDifference();
  }

  boost::optional<double> RefrigerationSubcoolerLiquidSuction::designLiquidInletTemperature() const {
    return getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->designLiquidInletTemperature();
  }

  boost::optional<double> RefrigerationSubcoolerLiquidSuction::designVaporInletTemperature() const {
    return getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->designVaporInletTemperature();
  }

  bool RefrigerationSubcoolerLiquidSuction::setLiquidSuctionDesignSubcoolingTemperatureDifference(
    double liquidSuctionDesignSubcoolingTemperatureDifference) {
    return getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->setLiquidSuctionDesignSubcoolingTemperatureDifference(
      liquidSuctionDesignSubcoolingTemperatureDifference);
  }

  void RefrigerationSubcoolerLiquidSuction::resetLiquidSuctionDesignSubcoolingTemperatureDifference() {
    getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->resetLiquidSuctionDesignSubcoolingTemperatureDifference();
  }

  bool RefrigerationSubcoolerLiquidSuction::setDesignLiquidInletTemperature(double designLiquidInletTemperature) {
    return getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->setDesignLiquidInletTemperature(designLiquidInletTemperature);
  }

  void RefrigerationSubcoolerLiquidSuction::resetDesignLiquidInletTemperature() {
    getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->resetDesignLiquidInletTemperature();
  }

  bool RefrigerationSubcoolerLiquidSuction::setDesignVaporInletTemperature(double designVaporInletTemperature) {
    return getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->setDesignVaporInletTemperature(designVaporInletTemperature);
  }

  void RefrigerationSubcoolerLiquidSuction::resetDesignVaporInletTemperature() {
    getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->resetDesignVaporInletTemperature();
  }

  boost::optional<RefrigerationSystem> RefrigerationSubcoolerLiquidSuction::system() const {
    return getImpl<detail::RefrigerationSubcoolerLiquidSuction_Impl>()->system();
  }

  /// @cond
  RefrigerationSubcoolerLiquidSuction::RefrigerationSubcoolerLiquidSuction(std::shared_ptr<detail::RefrigerationSubcoolerLiquidSuction_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
