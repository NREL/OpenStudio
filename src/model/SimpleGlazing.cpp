/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SimpleGlazing.hpp"
#include "SimpleGlazing_Impl.hpp"

#include <utilities/idd/OS_WindowMaterial_SimpleGlazingSystem_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SimpleGlazing_Impl::SimpleGlazing_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Glazing_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SimpleGlazing::iddObjectType());
    }

    SimpleGlazing_Impl::SimpleGlazing_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Glazing_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SimpleGlazing::iddObjectType());
    }

    SimpleGlazing_Impl::SimpleGlazing_Impl(const SimpleGlazing_Impl& other, Model_Impl* model, bool keepHandle)
      : Glazing_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SimpleGlazing_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SimpleGlazing_Impl::iddObjectType() const {
      return SimpleGlazing::iddObjectType();
    }

    boost::optional<double> SimpleGlazing_Impl::getVisibleTransmittance() const {
      return visibleTransmittance();
    }

    double SimpleGlazing_Impl::uFactor() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_SimpleGlazingSystemFields::UFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SimpleGlazing_Impl::solarHeatGainCoefficient() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_SimpleGlazingSystemFields::SolarHeatGainCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> SimpleGlazing_Impl::visibleTransmittance() const {
      return getDouble(OS_WindowMaterial_SimpleGlazingSystemFields::VisibleTransmittance, true);
    }

    bool SimpleGlazing_Impl::setUFactor(double uFactor) {
      bool result = setDouble(OS_WindowMaterial_SimpleGlazingSystemFields::UFactor, uFactor);
      return result;
    }

    bool SimpleGlazing_Impl::setSolarHeatGainCoefficient(double solarHeatGainCoefficient) {
      bool result = setDouble(OS_WindowMaterial_SimpleGlazingSystemFields::SolarHeatGainCoefficient, solarHeatGainCoefficient);
      return result;
    }

    bool SimpleGlazing_Impl::setVisibleTransmittance(boost::optional<double> visibleTransmittance) {
      bool result(false);
      if (visibleTransmittance) {
        result = setDouble(OS_WindowMaterial_SimpleGlazingSystemFields::VisibleTransmittance, visibleTransmittance.get());
      } else {
        resetVisibleTransmittance();
        result = true;
      }
      return result;
    }

    void SimpleGlazing_Impl::resetVisibleTransmittance() {
      bool result = setString(OS_WindowMaterial_SimpleGlazingSystemFields::VisibleTransmittance, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  SimpleGlazing::SimpleGlazing(const Model& model, double uFactor, double solarHeatGainCoefficient) : Glazing(SimpleGlazing::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SimpleGlazing_Impl>());

    bool ok = true;
    ok = setUFactor(uFactor);
    OS_ASSERT(ok);
    ok = setSolarHeatGainCoefficient(solarHeatGainCoefficient);
    OS_ASSERT(ok);
  }

  IddObjectType SimpleGlazing::iddObjectType() {
    return {IddObjectType::OS_WindowMaterial_SimpleGlazingSystem};
  }

  double SimpleGlazing::uFactor() const {
    return getImpl<detail::SimpleGlazing_Impl>()->uFactor();
  }

  double SimpleGlazing::solarHeatGainCoefficient() const {
    return getImpl<detail::SimpleGlazing_Impl>()->solarHeatGainCoefficient();
  }

  boost::optional<double> SimpleGlazing::visibleTransmittance() const {
    return getImpl<detail::SimpleGlazing_Impl>()->visibleTransmittance();
  }

  bool SimpleGlazing::setUFactor(double uFactor) {
    return getImpl<detail::SimpleGlazing_Impl>()->setUFactor(uFactor);
  }

  bool SimpleGlazing::setSolarHeatGainCoefficient(double solarHeatGainCoefficient) {
    return getImpl<detail::SimpleGlazing_Impl>()->setSolarHeatGainCoefficient(solarHeatGainCoefficient);
  }

  bool SimpleGlazing::setVisibleTransmittance(double visibleTransmittance) {
    return getImpl<detail::SimpleGlazing_Impl>()->setVisibleTransmittance(visibleTransmittance);
  }

  void SimpleGlazing::resetVisibleTransmittance() {
    getImpl<detail::SimpleGlazing_Impl>()->resetVisibleTransmittance();
  }

  /// @cond
  SimpleGlazing::SimpleGlazing(std::shared_ptr<detail::SimpleGlazing_Impl> impl) : Glazing(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
