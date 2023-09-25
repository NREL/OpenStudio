/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "BoilerSteam.hpp"
#include "BoilerSteam_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Boiler_Steam_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    BoilerSteam_Impl::BoilerSteam_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == BoilerSteam::iddObjectType());
    }

    BoilerSteam_Impl::BoilerSteam_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == BoilerSteam::iddObjectType());
    }

    BoilerSteam_Impl::BoilerSteam_Impl(const BoilerSteam_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& BoilerSteam_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Boiler Heating Rate", "Boiler Heating Energy", "Boiler Steam Inlet Temperature", "Boiler Steam Outlet Temperature", "Boiler Mass Flow Rate",

        // One of the following blocks will be applicable based on fuel type:
        // TODO: DLM: the return type of this method needs to change to std::vector<std::string> in ModelObject
        // until then, make this include all possible outputVariableNames for class regardless of fuelType
        // std::string fuelType = this->fuelType(,
        // if (fuelType == "Electricity") {
        "Boiler Electricity Rate", "Boiler Electricity Energy",
        // } else if (fuelType == "NaturalGas") {
        "Boiler NaturalGas Rate", "Boiler NaturalGas Energy",
        // } else if (fuelType == "Propane") {
        "Boiler Propane Rate", "Boiler Propane Energy",
        // } else if (fuelType == "FuelOilNo1") {
        "Boiler FuelOilNo1 Rate", "Boiler FuelOilNo1 Energy",
        // } else if (fuelType == "FuelOilNo2") {
        "Boiler FuelOilNo2 Rate", "Boiler FuelOilNo2 Energy",
        // } else if (fuelType == "Coal") {
        "Boiler Coal Rate", "Boiler Coal Energy",
        // } else if (fuelType == "Diesel") {
        "Boiler Diesel Rate", "Boiler Diesel Energy",
        // } else if (fuelType == "Gasoline") {
        "Boiler Gasoline Rate", "Boiler Gasoline Energy",
        // } else if (fuelType == "OtherFuel1") {
        "Boiler OtherFuel1 Rate", "Boiler OtherFuel1 Energy",
        // } else if (fuelType == "OtherFuel2") {
        "Boiler OtherFuel2 Rate", "Boiler OtherFuel2 Energy"
        // }

      };
      return result;
    }

    IddObjectType BoilerSteam_Impl::iddObjectType() const {
      return BoilerSteam::iddObjectType();
    }

    std::string BoilerSteam_Impl::fuelType() const {
      boost::optional<std::string> value = getString(OS_Boiler_SteamFields::FuelType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> BoilerSteam_Impl::maximumOperatingPressure() const {
      return getDouble(OS_Boiler_SteamFields::MaximumOperatingPressure, true);
    }

    boost::optional<double> BoilerSteam_Impl::theoreticalEfficiency() const {
      return getDouble(OS_Boiler_SteamFields::TheoreticalEfficiency, true);
    }

    boost::optional<double> BoilerSteam_Impl::designOutletSteamTemperature() const {
      return getDouble(OS_Boiler_SteamFields::DesignOutletSteamTemperature, true);
    }

    boost::optional<double> BoilerSteam_Impl::nominalCapacity() const {
      return getDouble(OS_Boiler_SteamFields::NominalCapacity, true);
    }

    bool BoilerSteam_Impl::isNominalCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Boiler_SteamFields::NominalCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> BoilerSteam_Impl::minimumPartLoadRatio() const {
      return getDouble(OS_Boiler_SteamFields::MinimumPartLoadRatio, true);
    }

    boost::optional<double> BoilerSteam_Impl::maximumPartLoadRatio() const {
      return getDouble(OS_Boiler_SteamFields::MaximumPartLoadRatio, true);
    }

    boost::optional<double> BoilerSteam_Impl::optimumPartLoadRatio() const {
      return getDouble(OS_Boiler_SteamFields::OptimumPartLoadRatio, true);
    }

    boost::optional<double> BoilerSteam_Impl::coefficient1ofFuelUseFunctionofPartLoadRatioCurve() const {
      return getDouble(OS_Boiler_SteamFields::Coefficient1ofFuelUseFunctionofPartLoadRatioCurve, true);
    }

    boost::optional<double> BoilerSteam_Impl::coefficient2ofFuelUseFunctionofPartLoadRatioCurve() const {
      return getDouble(OS_Boiler_SteamFields::Coefficient2ofFuelUseFunctionofPartLoadRatioCurve, true);
    }

    boost::optional<double> BoilerSteam_Impl::coefficient3ofFuelUseFunctionofPartLoadRatioCurve() const {
      return getDouble(OS_Boiler_SteamFields::Coefficient3ofFuelUseFunctionofPartLoadRatioCurve, true);
    }

    double BoilerSteam_Impl::sizingFactor() const {
      boost::optional<double> value = getDouble(OS_Boiler_SteamFields::SizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool BoilerSteam_Impl::isSizingFactorDefaulted() const {
      return isEmpty(OS_Boiler_SteamFields::SizingFactor);
    }

    bool BoilerSteam_Impl::setFuelType(const std::string& fuelType) {
      bool result = setString(OS_Boiler_SteamFields::FuelType, fuelType);
      return result;
    }

    bool BoilerSteam_Impl::setMaximumOperatingPressure(boost::optional<double> maximumOperatingPressure) {
      bool result(false);
      if (maximumOperatingPressure) {
        result = setDouble(OS_Boiler_SteamFields::MaximumOperatingPressure, maximumOperatingPressure.get());
      } else {
        resetMaximumOperatingPressure();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void BoilerSteam_Impl::resetMaximumOperatingPressure() {
      bool result = setString(OS_Boiler_SteamFields::MaximumOperatingPressure, "");
      OS_ASSERT(result);
    }

    bool BoilerSteam_Impl::setTheoreticalEfficiency(boost::optional<double> theoreticalEfficiency) {
      bool result(false);
      if (theoreticalEfficiency) {
        result = setDouble(OS_Boiler_SteamFields::TheoreticalEfficiency, theoreticalEfficiency.get());
      } else {
        resetTheoreticalEfficiency();
        result = true;
      }
      return result;
    }

    void BoilerSteam_Impl::resetTheoreticalEfficiency() {
      bool result = setString(OS_Boiler_SteamFields::TheoreticalEfficiency, "");
      OS_ASSERT(result);
    }

    bool BoilerSteam_Impl::setDesignOutletSteamTemperature(boost::optional<double> designOutletSteamTemperature) {
      bool result(false);
      if (designOutletSteamTemperature) {
        result = setDouble(OS_Boiler_SteamFields::DesignOutletSteamTemperature, designOutletSteamTemperature.get());
      } else {
        resetDesignOutletSteamTemperature();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void BoilerSteam_Impl::resetDesignOutletSteamTemperature() {
      bool result = setString(OS_Boiler_SteamFields::DesignOutletSteamTemperature, "");
      OS_ASSERT(result);
    }

    bool BoilerSteam_Impl::setNominalCapacity(boost::optional<double> nominalCapacity) {
      bool result(false);
      if (nominalCapacity) {
        result = setDouble(OS_Boiler_SteamFields::NominalCapacity, nominalCapacity.get());
      } else {
        resetNominalCapacity();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void BoilerSteam_Impl::resetNominalCapacity() {
      bool result = setString(OS_Boiler_SteamFields::NominalCapacity, "");
      OS_ASSERT(result);
    }

    void BoilerSteam_Impl::autosizeNominalCapacity() {
      bool result = setString(OS_Boiler_SteamFields::NominalCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool BoilerSteam_Impl::setMinimumPartLoadRatio(boost::optional<double> minimumPartLoadRatio) {
      bool result(false);
      if (minimumPartLoadRatio) {
        result = setDouble(OS_Boiler_SteamFields::MinimumPartLoadRatio, minimumPartLoadRatio.get());
      } else {
        resetMinimumPartLoadRatio();
        result = true;
      }
      return result;
    }

    void BoilerSteam_Impl::resetMinimumPartLoadRatio() {
      bool result = setString(OS_Boiler_SteamFields::MinimumPartLoadRatio, "");
      OS_ASSERT(result);
    }

    bool BoilerSteam_Impl::setMaximumPartLoadRatio(boost::optional<double> maximumPartLoadRatio) {
      bool result(false);
      if (maximumPartLoadRatio) {
        result = setDouble(OS_Boiler_SteamFields::MaximumPartLoadRatio, maximumPartLoadRatio.get());
      } else {
        resetMaximumPartLoadRatio();
        result = true;
      }
      return result;
    }

    void BoilerSteam_Impl::resetMaximumPartLoadRatio() {
      bool result = setString(OS_Boiler_SteamFields::MaximumPartLoadRatio, "");
      OS_ASSERT(result);
    }

    bool BoilerSteam_Impl::setOptimumPartLoadRatio(boost::optional<double> optimumPartLoadRatio) {
      bool result(false);
      if (optimumPartLoadRatio) {
        result = setDouble(OS_Boiler_SteamFields::OptimumPartLoadRatio, optimumPartLoadRatio.get());
      } else {
        resetOptimumPartLoadRatio();
        result = true;
      }
      return result;
    }

    void BoilerSteam_Impl::resetOptimumPartLoadRatio() {
      bool result = setString(OS_Boiler_SteamFields::OptimumPartLoadRatio, "");
      OS_ASSERT(result);
    }

    bool BoilerSteam_Impl::setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(
      boost::optional<double> coefficient1ofFuelUseFunctionofPartLoadRatioCurve) {
      bool result(false);
      if (coefficient1ofFuelUseFunctionofPartLoadRatioCurve) {
        result = setDouble(OS_Boiler_SteamFields::Coefficient1ofFuelUseFunctionofPartLoadRatioCurve,
                           coefficient1ofFuelUseFunctionofPartLoadRatioCurve.get());
      } else {
        resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void BoilerSteam_Impl::resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve() {
      bool result = setString(OS_Boiler_SteamFields::Coefficient1ofFuelUseFunctionofPartLoadRatioCurve, "");
      OS_ASSERT(result);
    }

    bool BoilerSteam_Impl::setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(
      boost::optional<double> coefficient2ofFuelUseFunctionofPartLoadRatioCurve) {
      bool result(false);
      if (coefficient2ofFuelUseFunctionofPartLoadRatioCurve) {
        result = setDouble(OS_Boiler_SteamFields::Coefficient2ofFuelUseFunctionofPartLoadRatioCurve,
                           coefficient2ofFuelUseFunctionofPartLoadRatioCurve.get());
      } else {
        resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void BoilerSteam_Impl::resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve() {
      bool result = setString(OS_Boiler_SteamFields::Coefficient2ofFuelUseFunctionofPartLoadRatioCurve, "");
      OS_ASSERT(result);
    }

    bool BoilerSteam_Impl::setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(
      boost::optional<double> coefficient3ofFuelUseFunctionofPartLoadRatioCurve) {
      bool result(false);
      if (coefficient3ofFuelUseFunctionofPartLoadRatioCurve) {
        result = setDouble(OS_Boiler_SteamFields::Coefficient3ofFuelUseFunctionofPartLoadRatioCurve,
                           coefficient3ofFuelUseFunctionofPartLoadRatioCurve.get());
      } else {
        resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void BoilerSteam_Impl::resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve() {
      bool result = setString(OS_Boiler_SteamFields::Coefficient3ofFuelUseFunctionofPartLoadRatioCurve, "");
      OS_ASSERT(result);
    }

    bool BoilerSteam_Impl::setSizingFactor(double sizingFactor) {
      bool result = setDouble(OS_Boiler_SteamFields::SizingFactor, sizingFactor);
      return result;
    }

    void BoilerSteam_Impl::resetSizingFactor() {
      bool result = setString(OS_Boiler_SteamFields::SizingFactor, "");
      OS_ASSERT(result);
    }

    std::vector<std::string> BoilerSteam_Impl::fuelTypeValues() const {
      return BoilerSteam::fuelTypeValues();
    }

    unsigned BoilerSteam_Impl::inletPort() const {
      return OS_Boiler_SteamFields::WaterInletNodeName;
    }

    unsigned BoilerSteam_Impl::outletPort() const {
      return OS_Boiler_SteamFields::SteamOutletNodeName;
    }

    bool BoilerSteam_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->supplyComponent(node.handle())) {
          if (StraightComponent_Impl::addToNode(node)) {
            plant->setFluidType("Steam");
            return true;
          }
        }
      }

      return false;
    }

    boost::optional<double> BoilerSteam_Impl::autosizedNominalCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    void BoilerSteam_Impl::autosize() {
      autosizeNominalCapacity();
    }

    void BoilerSteam_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedNominalCapacity();
      if (val) {
        setNominalCapacity(val.get());
      }
    }

    std::string BoilerSteam_Impl::endUseSubcategory() const {
      auto value = getString(OS_Boiler_SteamFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool BoilerSteam_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      return setString(OS_Boiler_SteamFields::EndUseSubcategory, endUseSubcategory);
    }

    ComponentType BoilerSteam_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> BoilerSteam_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> BoilerSteam_Impl::heatingFuelTypes() const {
      return {FuelType(fuelType())};
    }

    std::vector<AppGFuelType> BoilerSteam_Impl::appGHeatingFuelTypes() const {
      return {convertFuelTypeToAppG(FuelType(fuelType()))};
    }

  }  // namespace detail

  BoilerSteam::BoilerSteam(const Model& model) : StraightComponent(BoilerSteam::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::BoilerSteam_Impl>());

    setFuelType("NaturalGas");
    setMaximumOperatingPressure(160000);
    setTheoreticalEfficiency(0.8);
    setDesignOutletSteamTemperature(115);
    autosizeNominalCapacity();
    setMinimumPartLoadRatio(0.00001);
    setMaximumPartLoadRatio(1.0);
    setOptimumPartLoadRatio(0.2);
    setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(0.8);
    setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(0.1);
    setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(0.1);
    setSizingFactor(1.0);
    setEndUseSubcategory("General");
  }

  IddObjectType BoilerSteam::iddObjectType() {
    return {IddObjectType::OS_Boiler_Steam};
  }

  std::vector<std::string> BoilerSteam::fuelTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Boiler_SteamFields::FuelType);
  }

  std::string BoilerSteam::fuelType() const {
    return getImpl<detail::BoilerSteam_Impl>()->fuelType();
  }

  boost::optional<double> BoilerSteam::maximumOperatingPressure() const {
    return getImpl<detail::BoilerSteam_Impl>()->maximumOperatingPressure();
  }

  boost::optional<double> BoilerSteam::theoreticalEfficiency() const {
    return getImpl<detail::BoilerSteam_Impl>()->theoreticalEfficiency();
  }

  boost::optional<double> BoilerSteam::designOutletSteamTemperature() const {
    return getImpl<detail::BoilerSteam_Impl>()->designOutletSteamTemperature();
  }

  boost::optional<double> BoilerSteam::nominalCapacity() const {
    return getImpl<detail::BoilerSteam_Impl>()->nominalCapacity();
  }

  bool BoilerSteam::isNominalCapacityAutosized() const {
    return getImpl<detail::BoilerSteam_Impl>()->isNominalCapacityAutosized();
  }

  boost::optional<double> BoilerSteam::minimumPartLoadRatio() const {
    return getImpl<detail::BoilerSteam_Impl>()->minimumPartLoadRatio();
  }

  boost::optional<double> BoilerSteam::maximumPartLoadRatio() const {
    return getImpl<detail::BoilerSteam_Impl>()->maximumPartLoadRatio();
  }

  boost::optional<double> BoilerSteam::optimumPartLoadRatio() const {
    return getImpl<detail::BoilerSteam_Impl>()->optimumPartLoadRatio();
  }

  boost::optional<double> BoilerSteam::coefficient1ofFuelUseFunctionofPartLoadRatioCurve() const {
    return getImpl<detail::BoilerSteam_Impl>()->coefficient1ofFuelUseFunctionofPartLoadRatioCurve();
  }

  boost::optional<double> BoilerSteam::coefficient2ofFuelUseFunctionofPartLoadRatioCurve() const {
    return getImpl<detail::BoilerSteam_Impl>()->coefficient2ofFuelUseFunctionofPartLoadRatioCurve();
  }

  boost::optional<double> BoilerSteam::coefficient3ofFuelUseFunctionofPartLoadRatioCurve() const {
    return getImpl<detail::BoilerSteam_Impl>()->coefficient3ofFuelUseFunctionofPartLoadRatioCurve();
  }

  double BoilerSteam::sizingFactor() const {
    return getImpl<detail::BoilerSteam_Impl>()->sizingFactor();
  }

  bool BoilerSteam::isSizingFactorDefaulted() const {
    return getImpl<detail::BoilerSteam_Impl>()->isSizingFactorDefaulted();
  }

  bool BoilerSteam::setFuelType(const std::string& fuelType) {
    return getImpl<detail::BoilerSteam_Impl>()->setFuelType(fuelType);
  }

  bool BoilerSteam::setMaximumOperatingPressure(double maximumOperatingPressure) {
    return getImpl<detail::BoilerSteam_Impl>()->setMaximumOperatingPressure(maximumOperatingPressure);
  }

  void BoilerSteam::resetMaximumOperatingPressure() {
    getImpl<detail::BoilerSteam_Impl>()->resetMaximumOperatingPressure();
  }

  bool BoilerSteam::setTheoreticalEfficiency(double theoreticalEfficiency) {
    return getImpl<detail::BoilerSteam_Impl>()->setTheoreticalEfficiency(theoreticalEfficiency);
  }

  void BoilerSteam::resetTheoreticalEfficiency() {
    getImpl<detail::BoilerSteam_Impl>()->resetTheoreticalEfficiency();
  }

  bool BoilerSteam::setDesignOutletSteamTemperature(double designOutletSteamTemperature) {
    return getImpl<detail::BoilerSteam_Impl>()->setDesignOutletSteamTemperature(designOutletSteamTemperature);
  }

  void BoilerSteam::resetDesignOutletSteamTemperature() {
    getImpl<detail::BoilerSteam_Impl>()->resetDesignOutletSteamTemperature();
  }

  bool BoilerSteam::setNominalCapacity(double nominalCapacity) {
    return getImpl<detail::BoilerSteam_Impl>()->setNominalCapacity(nominalCapacity);
  }

  void BoilerSteam::resetNominalCapacity() {
    getImpl<detail::BoilerSteam_Impl>()->resetNominalCapacity();
  }

  void BoilerSteam::autosizeNominalCapacity() {
    getImpl<detail::BoilerSteam_Impl>()->autosizeNominalCapacity();
  }

  bool BoilerSteam::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
    return getImpl<detail::BoilerSteam_Impl>()->setMinimumPartLoadRatio(minimumPartLoadRatio);
  }

  void BoilerSteam::resetMinimumPartLoadRatio() {
    getImpl<detail::BoilerSteam_Impl>()->resetMinimumPartLoadRatio();
  }

  bool BoilerSteam::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
    return getImpl<detail::BoilerSteam_Impl>()->setMaximumPartLoadRatio(maximumPartLoadRatio);
  }

  void BoilerSteam::resetMaximumPartLoadRatio() {
    getImpl<detail::BoilerSteam_Impl>()->resetMaximumPartLoadRatio();
  }

  bool BoilerSteam::setOptimumPartLoadRatio(double optimumPartLoadRatio) {
    return getImpl<detail::BoilerSteam_Impl>()->setOptimumPartLoadRatio(optimumPartLoadRatio);
  }

  void BoilerSteam::resetOptimumPartLoadRatio() {
    getImpl<detail::BoilerSteam_Impl>()->resetOptimumPartLoadRatio();
  }

  bool BoilerSteam::setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(double coefficient1ofFuelUseFunctionofPartLoadRatioCurve) {
    return getImpl<detail::BoilerSteam_Impl>()->setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(
      coefficient1ofFuelUseFunctionofPartLoadRatioCurve);
  }

  void BoilerSteam::resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve() {
    getImpl<detail::BoilerSteam_Impl>()->resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve();
  }

  bool BoilerSteam::setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(double coefficient2ofFuelUseFunctionofPartLoadRatioCurve) {
    return getImpl<detail::BoilerSteam_Impl>()->setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(
      coefficient2ofFuelUseFunctionofPartLoadRatioCurve);
  }

  void BoilerSteam::resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve() {
    getImpl<detail::BoilerSteam_Impl>()->resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve();
  }

  bool BoilerSteam::setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(double coefficient3ofFuelUseFunctionofPartLoadRatioCurve) {
    return getImpl<detail::BoilerSteam_Impl>()->setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(
      coefficient3ofFuelUseFunctionofPartLoadRatioCurve);
  }

  void BoilerSteam::resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve() {
    getImpl<detail::BoilerSteam_Impl>()->resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve();
  }

  bool BoilerSteam::setSizingFactor(double sizingFactor) {
    return getImpl<detail::BoilerSteam_Impl>()->setSizingFactor(sizingFactor);
  }

  void BoilerSteam::resetSizingFactor() {
    getImpl<detail::BoilerSteam_Impl>()->resetSizingFactor();
  }

  std::string BoilerSteam::endUseSubcategory() const {
    return getImpl<detail::BoilerSteam_Impl>()->endUseSubcategory();
  }

  bool BoilerSteam::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::BoilerSteam_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  /// @cond
  BoilerSteam::BoilerSteam(std::shared_ptr<detail::BoilerSteam_Impl> impl) : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> BoilerSteam::autosizedNominalCapacity() const {
    return getImpl<detail::BoilerSteam_Impl>()->autosizedNominalCapacity();
  }

}  // namespace model
}  // namespace openstudio
