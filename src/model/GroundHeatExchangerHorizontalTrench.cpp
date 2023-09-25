/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "GroundHeatExchangerHorizontalTrench.hpp"
#include "GroundHeatExchangerHorizontalTrench_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "SiteGroundTemperatureUndisturbedKusudaAchenbach.hpp"
#include "SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/DeprecatedHelpers.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_GroundHeatExchanger_HorizontalTrench_FieldEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    GroundHeatExchangerHorizontalTrench_Impl::GroundHeatExchangerHorizontalTrench_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == GroundHeatExchangerHorizontalTrench::iddObjectType());
    }

    GroundHeatExchangerHorizontalTrench_Impl::GroundHeatExchangerHorizontalTrench_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == GroundHeatExchangerHorizontalTrench::iddObjectType());
    }

    GroundHeatExchangerHorizontalTrench_Impl::GroundHeatExchangerHorizontalTrench_Impl(const GroundHeatExchangerHorizontalTrench_Impl& other,
                                                                                       Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& GroundHeatExchangerHorizontalTrench_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Ground Heat Exchanger Mass Flow Rate", "Ground Heat Exchanger Inlet Temperature",
                                                   "Ground Heat Exchanger Outlet Temperature", "Ground Heat Exchanger Fluid Heat Transfer Rate"};
      return result;
    }

    IddObjectType GroundHeatExchangerHorizontalTrench_Impl::iddObjectType() const {
      return GroundHeatExchangerHorizontalTrench::iddObjectType();
    }

    double GroundHeatExchangerHorizontalTrench_Impl::designFlowRate() const {
      boost::optional<double> value = getDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::DesignFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GroundHeatExchangerHorizontalTrench_Impl::trenchLengthinPipeAxialDirection() const {
      boost::optional<double> value = getDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::TrenchLengthinPipeAxialDirection, true);
      OS_ASSERT(value);
      return value.get();
    }

    int GroundHeatExchangerHorizontalTrench_Impl::numberofTrenches() const {
      boost::optional<int> value = getInt(OS_GroundHeatExchanger_HorizontalTrenchFields::NumberofTrenches, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GroundHeatExchangerHorizontalTrench_Impl::horizontalSpacingBetweenPipes() const {
      boost::optional<double> value = getDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::HorizontalSpacingBetweenPipes, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GroundHeatExchangerHorizontalTrench_Impl::pipeInnerDiameter() const {
      boost::optional<double> value = getDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::PipeInnerDiameter, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GroundHeatExchangerHorizontalTrench_Impl::pipeOuterDiameter() const {
      boost::optional<double> value = getDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::PipeOuterDiameter, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GroundHeatExchangerHorizontalTrench_Impl::burialDepth() const {
      boost::optional<double> value = getDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::BurialDepth, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GroundHeatExchangerHorizontalTrench_Impl::soilThermalConductivity() const {
      boost::optional<double> value = getDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::SoilThermalConductivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GroundHeatExchangerHorizontalTrench_Impl::soilDensity() const {
      boost::optional<double> value = getDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::SoilDensity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GroundHeatExchangerHorizontalTrench_Impl::soilSpecificHeat() const {
      boost::optional<double> value = getDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::SoilSpecificHeat, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GroundHeatExchangerHorizontalTrench_Impl::pipeThermalConductivity() const {
      boost::optional<double> value = getDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::PipeThermalConductivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GroundHeatExchangerHorizontalTrench_Impl::pipeDensity() const {
      boost::optional<double> value = getDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::PipeDensity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GroundHeatExchangerHorizontalTrench_Impl::pipeSpecificHeat() const {
      boost::optional<double> value = getDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::PipeSpecificHeat, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GroundHeatExchangerHorizontalTrench_Impl::soilMoistureContentPercent() const {
      boost::optional<double> value = getDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::SoilMoistureContentPercent, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GroundHeatExchangerHorizontalTrench_Impl::soilMoistureContentPercentatSaturation() const {
      boost::optional<double> value = getDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::SoilMoistureContentPercentatSaturation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GroundHeatExchangerHorizontalTrench_Impl::evapotranspirationGroundCoverParameter() const {
      boost::optional<double> value = getDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::EvapotranspirationGroundCoverParameter, true);
      OS_ASSERT(value);
      return value.get();
    }

    ModelObject GroundHeatExchangerHorizontalTrench_Impl::undisturbedGroundTemperatureModel() const {
      boost::optional<ModelObject> modelObject;
      modelObject =
        getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_GroundHeatExchanger_HorizontalTrenchFields::UndisturbedGroundTemperatureModel);
      OS_ASSERT(modelObject);
      return modelObject.get();
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setDesignFlowRate(double designFlowRate) {
      bool result = setDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::DesignFlowRate, designFlowRate);
      return result;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setTrenchLengthinPipeAxialDirection(double trenchLengthinPipeAxialDirection) {
      bool result = setDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::TrenchLengthinPipeAxialDirection, trenchLengthinPipeAxialDirection);
      return result;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setNumberofTrenches(int numberofTrenches) {
      bool result = setInt(OS_GroundHeatExchanger_HorizontalTrenchFields::NumberofTrenches, numberofTrenches);
      return result;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setHorizontalSpacingBetweenPipes(double horizontalSpacingBetweenPipes) {
      bool result = setDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::HorizontalSpacingBetweenPipes, horizontalSpacingBetweenPipes);
      return result;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setPipeInnerDiameter(double pipeInnerDiameter) {
      bool result = setDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::PipeInnerDiameter, pipeInnerDiameter);
      return result;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setPipeOuterDiameter(double pipeOuterDiameter) {
      bool result = setDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::PipeOuterDiameter, pipeOuterDiameter);
      return result;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setBurialDepth(double burialDepth) {
      bool result = setDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::BurialDepth, burialDepth);
      return result;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setSoilThermalConductivity(double soilThermalConductivity) {
      bool result = setDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::SoilThermalConductivity, soilThermalConductivity);
      return result;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setSoilDensity(double soilDensity) {
      bool result = setDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::SoilDensity, soilDensity);
      return result;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setSoilSpecificHeat(double soilSpecificHeat) {
      bool result = setDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::SoilSpecificHeat, soilSpecificHeat);
      return result;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setPipeThermalConductivity(double pipeThermalConductivity) {
      bool result = setDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::PipeThermalConductivity, pipeThermalConductivity);
      return result;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setPipeDensity(double pipeDensity) {
      bool result = setDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::PipeDensity, pipeDensity);
      return result;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setPipeSpecificHeat(double pipeSpecificHeat) {
      bool result = setDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::PipeSpecificHeat, pipeSpecificHeat);
      return result;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setSoilMoistureContentPercent(double soilMoistureContentPercent) {
      bool result = setDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::SoilMoistureContentPercent, soilMoistureContentPercent);
      return result;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setSoilMoistureContentPercentatSaturation(double soilMoistureContentPercentatSaturation) {
      bool result =
        setDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::SoilMoistureContentPercentatSaturation, soilMoistureContentPercentatSaturation);
      return result;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setEvapotranspirationGroundCoverParameter(double evapotranspirationGroundCoverParameter) {
      bool result =
        setDouble(OS_GroundHeatExchanger_HorizontalTrenchFields::EvapotranspirationGroundCoverParameter, evapotranspirationGroundCoverParameter);
      return result;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::setUndisturbedGroundTemperatureModel(const ModelObject& undisturbedGroundTemperatureModel) {
      bool result =
        setPointer(OS_GroundHeatExchanger_HorizontalTrenchFields::UndisturbedGroundTemperatureModel, undisturbedGroundTemperatureModel.handle());
      return result;
    }

    unsigned GroundHeatExchangerHorizontalTrench_Impl::inletPort() const {
      return OS_GroundHeatExchanger_HorizontalTrenchFields::InletNodeName;
    }

    unsigned GroundHeatExchangerHorizontalTrench_Impl::outletPort() const {
      return OS_GroundHeatExchanger_HorizontalTrenchFields::OutletNodeName;
    }

    bool GroundHeatExchangerHorizontalTrench_Impl::addToNode(Node& node) {
      if (auto plant = node.plantLoop()) {
        if (!plant->demandComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    ComponentType GroundHeatExchangerHorizontalTrench_Impl::componentType() const {
      return ComponentType::Both;
    }

    std::vector<FuelType> GroundHeatExchangerHorizontalTrench_Impl::coolingFuelTypes() const {
      return {FuelType::Geothermal};
    }

    std::vector<FuelType> GroundHeatExchangerHorizontalTrench_Impl::heatingFuelTypes() const {
      return {FuelType::Geothermal};
    }

    std::vector<AppGFuelType> GroundHeatExchangerHorizontalTrench_Impl::appGHeatingFuelTypes() const {
      return {AppGFuelType::Geothermal};
    }

  }  // namespace detail

  GroundHeatExchangerHorizontalTrench::GroundHeatExchangerHorizontalTrench(const Model& model)
    : StraightComponent(GroundHeatExchangerHorizontalTrench::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>());

    setDesignFlowRate(0.004);
    setTrenchLengthinPipeAxialDirection(75);
    setNumberofTrenches(2);
    setHorizontalSpacingBetweenPipes(2.0);
    setPipeInnerDiameter(0.016);
    setPipeOuterDiameter(0.02667);
    setBurialDepth(1.25);
    setSoilThermalConductivity(1.08);
    setSoilDensity(962);
    setSoilSpecificHeat(2576);
    setPipeThermalConductivity(0.3895);
    setPipeDensity(641);
    setPipeSpecificHeat(2405);
    setSoilMoistureContentPercent(30);
    setSoilMoistureContentPercentatSaturation(50);
    setEvapotranspirationGroundCoverParameter(0.408);

    SiteGroundTemperatureUndisturbedKusudaAchenbach undisturbedGroundTemperatureModel(model);
    setUndisturbedGroundTemperatureModel(undisturbedGroundTemperatureModel);
  }

  GroundHeatExchangerHorizontalTrench::GroundHeatExchangerHorizontalTrench(const Model& model, const ModelObject& undisturbedGroundTemperatureModel)
    : StraightComponent(GroundHeatExchangerHorizontalTrench::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>());

    bool ok = setUndisturbedGroundTemperatureModel(undisturbedGroundTemperatureModel);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Undisturbed Ground Temperature Model to "
                                     << undisturbedGroundTemperatureModel.briefDescription() << ".");
    }
    setDesignFlowRate(0.004);
    setTrenchLengthinPipeAxialDirection(75);
    setNumberofTrenches(2);
    setHorizontalSpacingBetweenPipes(2.0);
    setPipeInnerDiameter(0.016);
    setPipeOuterDiameter(0.02667);
    setBurialDepth(1.25);
    setSoilThermalConductivity(1.08);
    setSoilDensity(962);
    setSoilSpecificHeat(2576);
    setPipeThermalConductivity(0.3895);
    setPipeDensity(641);
    setPipeSpecificHeat(2405);
    setSoilMoistureContentPercent(30);
    setSoilMoistureContentPercentatSaturation(50);
    setEvapotranspirationGroundCoverParameter(0.408);
  }

  IddObjectType GroundHeatExchangerHorizontalTrench::iddObjectType() {
    return {IddObjectType::OS_GroundHeatExchanger_HorizontalTrench};
  }

  std::vector<std::string> GroundHeatExchangerHorizontalTrench::groundTemperatureModelValues() {
    DEPRECATED_AT_MSG(3, 6, 0, "Use undisturbedGroundTemperatureModel instead.");
    std::vector<std::string> result{"KusudaAchenbach", "SiteGroundTemperature"};
    return result;
  }

  double GroundHeatExchangerHorizontalTrench::designFlowRate() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->designFlowRate();
  }

  double GroundHeatExchangerHorizontalTrench::trenchLengthinPipeAxialDirection() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->trenchLengthinPipeAxialDirection();
  }

  int GroundHeatExchangerHorizontalTrench::numberofTrenches() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->numberofTrenches();
  }

  double GroundHeatExchangerHorizontalTrench::horizontalSpacingBetweenPipes() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->horizontalSpacingBetweenPipes();
  }

  double GroundHeatExchangerHorizontalTrench::pipeInnerDiameter() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->pipeInnerDiameter();
  }

  double GroundHeatExchangerHorizontalTrench::pipeOuterDiameter() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->pipeOuterDiameter();
  }

  double GroundHeatExchangerHorizontalTrench::burialDepth() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->burialDepth();
  }

  double GroundHeatExchangerHorizontalTrench::soilThermalConductivity() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->soilThermalConductivity();
  }

  double GroundHeatExchangerHorizontalTrench::soilDensity() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->soilDensity();
  }

  double GroundHeatExchangerHorizontalTrench::soilSpecificHeat() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->soilSpecificHeat();
  }

  double GroundHeatExchangerHorizontalTrench::pipeThermalConductivity() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->pipeThermalConductivity();
  }

  double GroundHeatExchangerHorizontalTrench::pipeDensity() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->pipeDensity();
  }

  double GroundHeatExchangerHorizontalTrench::pipeSpecificHeat() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->pipeSpecificHeat();
  }

  double GroundHeatExchangerHorizontalTrench::soilMoistureContentPercent() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->soilMoistureContentPercent();
  }

  double GroundHeatExchangerHorizontalTrench::soilMoistureContentPercentatSaturation() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->soilMoistureContentPercentatSaturation();
  }

  std::string GroundHeatExchangerHorizontalTrench::groundTemperatureModel() const {
    LOG(
      Warn,
      "As of 3.6.0, groundTemperatureModel is deprecated. Use undisturbedGroundTemperatureModel instead. It will be removed within three releases.");
    if (undisturbedGroundTemperatureModel().iddObjectType() == IddObjectType::OS_Site_GroundTemperature_Undisturbed_KusudaAchenbach) {
      return "KusudaAchenbach";
    } else {
      return "SiteGroundTemperature";
    }
  }

  bool GroundHeatExchangerHorizontalTrench::isGroundTemperatureModelDefaulted() const {
    DEPRECATED_AT_MSG(3, 6, 0, "Use undisturbedGroundTemperatureModel instead.");
    return false;
  }

  double GroundHeatExchangerHorizontalTrench::kusudaAchenbachAverageSurfaceTemperature() const {
    DEPRECATED_AT_MSG(3, 6, 0, "Use undisturbedGroundTemperatureModel instead.");
    if (undisturbedGroundTemperatureModel().iddObjectType() != IddObjectType::OS_Site_GroundTemperature_Undisturbed_KusudaAchenbach) {
      LOG_AND_THROW("Undisturbed ground temperature model is not KusudaAchenbach.");
    }
    return undisturbedGroundTemperatureModel().cast<SiteGroundTemperatureUndisturbedKusudaAchenbach>().averageSoilSurfaceTemperature().get();
  }

  double GroundHeatExchangerHorizontalTrench::kusudaAchenbachAverageAmplitudeofSurfaceTemperature() const {
    DEPRECATED_AT_MSG(3, 6, 0, "Use undisturbedGroundTemperatureModel instead.");
    if (undisturbedGroundTemperatureModel().iddObjectType() != IddObjectType::OS_Site_GroundTemperature_Undisturbed_KusudaAchenbach) {
      LOG_AND_THROW("Undisturbed ground temperature model is not KusudaAchenbach.");
    }
    return undisturbedGroundTemperatureModel().cast<SiteGroundTemperatureUndisturbedKusudaAchenbach>().averageAmplitudeofSurfaceTemperature().get();
  }

  double GroundHeatExchangerHorizontalTrench::kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature() const {
    DEPRECATED_AT_MSG(3, 6, 0, "Use undisturbedGroundTemperatureModel instead.");
    if (undisturbedGroundTemperatureModel().iddObjectType() != IddObjectType::OS_Site_GroundTemperature_Undisturbed_KusudaAchenbach) {
      LOG_AND_THROW("Undisturbed ground temperature model is not KusudaAchenbach.");
    }
    return undisturbedGroundTemperatureModel().cast<SiteGroundTemperatureUndisturbedKusudaAchenbach>().phaseShiftofMinimumSurfaceTemperature().get();
  }

  double GroundHeatExchangerHorizontalTrench::evapotranspirationGroundCoverParameter() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->evapotranspirationGroundCoverParameter();
  }

  ModelObject GroundHeatExchangerHorizontalTrench::undisturbedGroundTemperatureModel() const {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->undisturbedGroundTemperatureModel();
  }

  bool GroundHeatExchangerHorizontalTrench::setDesignFlowRate(double designFlowRate) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setDesignFlowRate(designFlowRate);
  }

  bool GroundHeatExchangerHorizontalTrench::setTrenchLengthinPipeAxialDirection(double trenchLengthinPipeAxialDirection) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setTrenchLengthinPipeAxialDirection(trenchLengthinPipeAxialDirection);
  }

  bool GroundHeatExchangerHorizontalTrench::setNumberofTrenches(int numberofTrenches) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setNumberofTrenches(numberofTrenches);
  }

  bool GroundHeatExchangerHorizontalTrench::setHorizontalSpacingBetweenPipes(double horizontalSpacingBetweenPipes) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setHorizontalSpacingBetweenPipes(horizontalSpacingBetweenPipes);
  }

  bool GroundHeatExchangerHorizontalTrench::setPipeInnerDiameter(double pipeInnerDiameter) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setPipeInnerDiameter(pipeInnerDiameter);
  }

  bool GroundHeatExchangerHorizontalTrench::setPipeOuterDiameter(double pipeOuterDiameter) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setPipeOuterDiameter(pipeOuterDiameter);
  }

  bool GroundHeatExchangerHorizontalTrench::setBurialDepth(double burialDepth) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setBurialDepth(burialDepth);
  }

  bool GroundHeatExchangerHorizontalTrench::setSoilThermalConductivity(double soilThermalConductivity) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setSoilThermalConductivity(soilThermalConductivity);
  }

  bool GroundHeatExchangerHorizontalTrench::setSoilDensity(double soilDensity) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setSoilDensity(soilDensity);
  }

  bool GroundHeatExchangerHorizontalTrench::setSoilSpecificHeat(double soilSpecificHeat) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setSoilSpecificHeat(soilSpecificHeat);
  }

  bool GroundHeatExchangerHorizontalTrench::setPipeThermalConductivity(double pipeThermalConductivity) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setPipeThermalConductivity(pipeThermalConductivity);
  }

  bool GroundHeatExchangerHorizontalTrench::setPipeDensity(double pipeDensity) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setPipeDensity(pipeDensity);
  }

  bool GroundHeatExchangerHorizontalTrench::setPipeSpecificHeat(double pipeSpecificHeat) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setPipeSpecificHeat(pipeSpecificHeat);
  }

  bool GroundHeatExchangerHorizontalTrench::setSoilMoistureContentPercent(double soilMoistureContentPercent) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setSoilMoistureContentPercent(soilMoistureContentPercent);
  }

  bool GroundHeatExchangerHorizontalTrench::setSoilMoistureContentPercentatSaturation(double soilMoistureContentPercentatSaturation) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setSoilMoistureContentPercentatSaturation(
      soilMoistureContentPercentatSaturation);
  }

  bool GroundHeatExchangerHorizontalTrench::setGroundTemperatureModel(const std::string& groundTemperatureModel) {
    DEPRECATED_AT_MSG(3, 6, 0, "Use undisturbedGroundTemperatureModel instead.");
    return false;
  }

  void GroundHeatExchangerHorizontalTrench::resetGroundTemperatureModel() {
    DEPRECATED_AT_MSG(3, 6, 0, "Use undisturbedGroundTemperatureModel instead.");
  }

  bool GroundHeatExchangerHorizontalTrench::setKusudaAchenbachAverageSurfaceTemperature(double kusudaAchenbachAverageSurfaceTemperature) {
    DEPRECATED_AT_MSG(3, 6, 0, "Use undisturbedGroundTemperatureModel instead.");
    if (undisturbedGroundTemperatureModel().iddObjectType() != IddObjectType::OS_Site_GroundTemperature_Undisturbed_KusudaAchenbach) {
      LOG_AND_THROW("Undisturbed ground temperature model is not KusudaAchenbach.");
    }
    return undisturbedGroundTemperatureModel().cast<SiteGroundTemperatureUndisturbedKusudaAchenbach>().setAverageSoilSurfaceTemperature(
      kusudaAchenbachAverageSurfaceTemperature);
  }

  bool GroundHeatExchangerHorizontalTrench::setKusudaAchenbachAverageAmplitudeofSurfaceTemperature(
    double kusudaAchenbachAverageAmplitudeofSurfaceTemperature) {
    DEPRECATED_AT_MSG(3, 6, 0, "Use undisturbedGroundTemperatureModel instead.");
    if (undisturbedGroundTemperatureModel().iddObjectType() != IddObjectType::OS_Site_GroundTemperature_Undisturbed_KusudaAchenbach) {
      LOG_AND_THROW("Undisturbed ground temperature model is not KusudaAchenbach.");
    }
    return undisturbedGroundTemperatureModel().cast<SiteGroundTemperatureUndisturbedKusudaAchenbach>().setAverageAmplitudeofSurfaceTemperature(
      kusudaAchenbachAverageAmplitudeofSurfaceTemperature);
  }

  bool GroundHeatExchangerHorizontalTrench::setKusudaAchenbachPhaseShiftofMinimumSurfaceTemperature(
    double kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature) {
    DEPRECATED_AT_MSG(3, 6, 0, "Use undisturbedGroundTemperatureModel instead.");
    if (undisturbedGroundTemperatureModel().iddObjectType() != IddObjectType::OS_Site_GroundTemperature_Undisturbed_KusudaAchenbach) {
      LOG_AND_THROW("Undisturbed ground temperature model is not KusudaAchenbach.");
    }
    return undisturbedGroundTemperatureModel().cast<SiteGroundTemperatureUndisturbedKusudaAchenbach>().setPhaseShiftofMinimumSurfaceTemperature(
      kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature);
  }

  bool GroundHeatExchangerHorizontalTrench::setEvapotranspirationGroundCoverParameter(double evapotranspirationGroundCoverParameter) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setEvapotranspirationGroundCoverParameter(
      evapotranspirationGroundCoverParameter);
  }

  bool GroundHeatExchangerHorizontalTrench::setUndisturbedGroundTemperatureModel(const ModelObject& undisturbedGroundTemperatureModel) {
    return getImpl<detail::GroundHeatExchangerHorizontalTrench_Impl>()->setUndisturbedGroundTemperatureModel(undisturbedGroundTemperatureModel);
  }

  /// @cond
  GroundHeatExchangerHorizontalTrench::GroundHeatExchangerHorizontalTrench(std::shared_ptr<detail::GroundHeatExchangerHorizontalTrench_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
