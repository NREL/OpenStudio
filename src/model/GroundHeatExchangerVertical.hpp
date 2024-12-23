/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GROUNDHEATEXCHANGERVERTICAL_HPP
#define MODEL_GROUNDHEATEXCHANGERVERTICAL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {
  namespace detail {

    class GroundHeatExchangerVertical_Impl;

  }  // namespace detail

  class MODEL_API GFunction
  {

   public:
    GFunction(double lnValue, double gValue);
    double lnValue() const;
    double gValue() const;

   private:
    double m_lnValue;
    double m_gValue;
  };

  /** GroundHeatExchangerVertical is a StraightComponent that wraps the OpenStudio IDD object 'OS:GroundHeatExchanger:Vertical'. */
  class MODEL_API GroundHeatExchangerVertical : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GroundHeatExchangerVertical(const Model& model);

    explicit GroundHeatExchangerVertical(const Model& model, const ModelObject& undisturbedGroundTemperatureModel);

    virtual ~GroundHeatExchangerVertical() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GroundHeatExchangerVertical(const GroundHeatExchangerVertical& other) = default;
    GroundHeatExchangerVertical(GroundHeatExchangerVertical&& other) = default;
    GroundHeatExchangerVertical& operator=(const GroundHeatExchangerVertical&) = default;
    GroundHeatExchangerVertical& operator=(GroundHeatExchangerVertical&&) = default;

    //@}

    static IddObjectType iddObjectType();

    bool addGFunction(double gFunctionLN, double gFunctionGValue);

    bool addGFunction(GFunction gFunc);

    void removeGFunction(int groupIndex);

    void removeAllGFunctions();

    std::vector<GFunction> gFunctions();

    /** @name Getters */
    //@{

    boost::optional<double> designFlowRate() const;

    boost::optional<int> numberofBoreHoles() const;

    double boreHoleTopDepth() const;

    boost::optional<double> boreHoleLength() const;

    boost::optional<double> boreHoleRadius() const;

    boost::optional<double> groundThermalConductivity() const;

    boost::optional<double> groundThermalHeatCapacity() const;

    boost::optional<double> groundTemperature() const;

    boost::optional<double> groutThermalConductivity() const;

    boost::optional<double> pipeThermalConductivity() const;

    boost::optional<double> pipeOutDiameter() const;

    boost::optional<double> uTubeDistance() const;

    boost::optional<double> pipeThickness() const;

    boost::optional<double> maximumLengthofSimulation() const;

    double gFunctionReferenceRatio() const;

    bool isGFunctionReferenceRatioDefaulted() const;

    ModelObject undisturbedGroundTemperatureModel() const;

    //@}
    /** @name Setters */

    //@{

    bool setDesignFlowRate(double designFlowRate);

    bool setNumberofBoreHoles(int numberofBoreHoles);

    void resetNumberofBoreHoles();

    bool setBoreHoleTopDepth(double boreHoleTopDepth);

    bool setBoreHoleLength(double boreHoleLength);

    void resetBoreHoleLength();

    bool setBoreHoleRadius(double boreHoleRadius);

    void resetBoreHoleRadius();

    bool setGroundThermalConductivity(double groundThermalConductivity);

    void resetGroundThermalConductivity();

    bool setGroundThermalHeatCapacity(double groundThermalHeatCapacity);

    void resetGroundThermalHeatCapacity();

    bool setGroundTemperature(double groundTemperature);

    void resetGroundTemperature();

    bool setGroutThermalConductivity(double groutThermalConductivity);

    void resetGroutThermalConductivity();

    bool setPipeThermalConductivity(double pipeThermalConductivity);

    void resetPipeThermalConductivity();

    bool setPipeOutDiameter(double pipeOutDiameter);

    void resetPipeOutDiameter();

    bool setUTubeDistance(double uTubeDistance);

    void resetUTubeDistance();

    bool setPipeThickness(double pipeThickness);

    void resetPipeThickness();

    bool setMaximumLengthofSimulation(double maximumLengthofSimulation);

    void resetMaximumLengthofSimulation();

    bool setGFunctionReferenceRatio(double gFunctionReferenceRatio);

    void resetGFunctionReferenceRatio();

    bool setUndisturbedGroundTemperatureModel(const ModelObject& undisturbedGroundTemperatureModel);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::GroundHeatExchangerVertical_Impl;

    explicit GroundHeatExchangerVertical(std::shared_ptr<detail::GroundHeatExchangerVertical_Impl> impl);

    friend class detail::GroundHeatExchangerVertical_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GroundHeatExchangerVertical");
  };

  /** \relates GroundHeatExchangerVertical*/
  using OptionalGroundHeatExchangerVertical = boost::optional<GroundHeatExchangerVertical>;

  /** \relates GroundHeatExchangerVertical*/
  using GroundHeatExchangerVerticalVector = std::vector<GroundHeatExchangerVertical>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GROUNDHEATEXCHANGERVERTICAL_HPP
