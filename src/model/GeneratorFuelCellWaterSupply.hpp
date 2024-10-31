/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELLWATERSUPPLY_HPP
#define MODEL_GENERATORFUELCELLWATERSUPPLY_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class CurveQuadratic;
  class CurveCubic;
  class Node;
  class Schedule;
  class GeneratorFuelCell;

  namespace detail {

    class GeneratorFuelCellWaterSupply_Impl;

  }  // namespace detail

  /** GeneratorFuelCellWaterSupply is a ModelObject that wraps the OpenStudio IDD object 'OS:Generator:FuelCell:WaterSupply'. */
  class MODEL_API GeneratorFuelCellWaterSupply : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GeneratorFuelCellWaterSupply(const Model& model);

    explicit GeneratorFuelCellWaterSupply(const Model& model, const std::string& waterTempMode);

    explicit GeneratorFuelCellWaterSupply(const Model& model, const CurveQuadratic& flowRateCurve, const CurveCubic& pumpPowerCurve,
                                          Schedule tempSchedule);

    explicit GeneratorFuelCellWaterSupply(const Model& model, const CurveQuadratic& flowRateCurve, const CurveCubic& pumpPowerCurve,
                                          const Node& waterTempNode, const std::string& waterTempMode);

    virtual ~GeneratorFuelCellWaterSupply() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GeneratorFuelCellWaterSupply(const GeneratorFuelCellWaterSupply& other) = default;
    GeneratorFuelCellWaterSupply(GeneratorFuelCellWaterSupply&& other) = default;
    GeneratorFuelCellWaterSupply& operator=(const GeneratorFuelCellWaterSupply&) = default;
    GeneratorFuelCellWaterSupply& operator=(GeneratorFuelCellWaterSupply&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> waterTemperatureModelingModeValues();

    /** @name Getters */
    //@{

    CurveQuadratic reformerWaterFlowRateFunctionofFuelRateCurve() const;

    CurveCubic reformerWaterPumpPowerFunctionofFuelRateCurve() const;

    double pumpHeatLossFactor() const;

    std::string waterTemperatureModelingMode() const;

    boost::optional<Node> waterTemperatureReferenceNode() const;

    boost::optional<Schedule> waterTemperatureSchedule() const;

    // Return optional parent generator
    boost::optional<GeneratorFuelCell> fuelCell() const;

    //@}
    /** @name Setters */
    //@{

    bool setReformerWaterFlowRateFunctionofFuelRateCurve(const CurveQuadratic& quadraticCurves);

    bool setReformerWaterPumpPowerFunctionofFuelRateCurve(const CurveCubic& cubicCurves);

    bool setPumpHeatLossFactor(double pumpHeatLossFactor);

    void resetPumpHeatLossFactor();

    bool setWaterTemperatureModelingMode(const std::string& waterTemperatureModelingMode);

    void resetWaterTemperatureModelingMode();

    bool setWaterTemperatureReferenceNode(const Node& connection);

    void resetWaterTemperatureReferenceNode();

    bool setWaterTemperatureSchedule(Schedule& schedule);

    void resetWaterTemperatureSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::GeneratorFuelCellWaterSupply_Impl;

    explicit GeneratorFuelCellWaterSupply(std::shared_ptr<detail::GeneratorFuelCellWaterSupply_Impl> impl);

    friend class detail::GeneratorFuelCellWaterSupply_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorFuelCellWaterSupply");
  };

  /** \relates GeneratorFuelCellWaterSupply*/
  using OptionalGeneratorFuelCellWaterSupply = boost::optional<GeneratorFuelCellWaterSupply>;

  /** \relates GeneratorFuelCellWaterSupply*/
  using GeneratorFuelCellWaterSupplyVector = std::vector<GeneratorFuelCellWaterSupply>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLWATERSUPPLY_HPP
