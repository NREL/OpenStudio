/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GROUNDHEATEXCHANGERVERTICAL_IMPL_HPP
#define MODEL_GROUNDHEATEXCHANGERVERTICAL_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {
  namespace detail {

    /** GroundHeatExchangerVertical_Impl is a StraightComponent_Impl that is the implementation class for GroundHeatExchangerVertical.*/
    class MODEL_API GroundHeatExchangerVertical_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      GroundHeatExchangerVertical_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GroundHeatExchangerVertical_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GroundHeatExchangerVertical_Impl(const GroundHeatExchangerVertical_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GroundHeatExchangerVertical_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<int> numberofBoreHoles() const;

      double boreHoleTopDepth() const;

      boost::optional<double> boreHoleLength() const;

      boost::optional<double> boreHoleRadius() const;

      boost::optional<double> groundThermalConductivity() const;

      boost::optional<double> groundThermalHeatCapacity() const;

      boost::optional<double> groundTemperature() const;

      boost::optional<double> designFlowRate() const;

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

      bool setNumberofBoreHoles(boost::optional<int> numberofBoreHoles);

      void resetNumberofBoreHoles();

      bool setBoreHoleTopDepth(double boreHoleTopDepth);

      bool setBoreHoleLength(boost::optional<double> boreHoleLength);

      void resetBoreHoleLength();

      bool setBoreHoleRadius(boost::optional<double> boreHoleRadius);

      void resetBoreHoleRadius();

      bool setGroundThermalConductivity(boost::optional<double> groundThermalConductivity);

      void resetGroundThermalConductivity();

      bool setGroundThermalHeatCapacity(boost::optional<double> groundThermalHeatCapacity);

      void resetGroundThermalHeatCapacity();

      bool setGroundTemperature(boost::optional<double> groundTemperature);

      void resetGroundTemperature();

      bool setDesignFlowRate(double designFlowRate);

      bool setGroutThermalConductivity(boost::optional<double> groutThermalConductivity);

      void resetGroutThermalConductivity();

      bool setPipeThermalConductivity(boost::optional<double> pipeThermalConductivity);

      void resetPipeThermalConductivity();

      bool setPipeOutDiameter(boost::optional<double> pipeOutDiameter);

      void resetPipeOutDiameter();

      bool setUTubeDistance(boost::optional<double> uTubeDistance);

      void resetUTubeDistance();

      bool setPipeThickness(boost::optional<double> pipeThickness);

      void resetPipeThickness();

      bool setMaximumLengthofSimulation(boost::optional<double> maximumLengthofSimulation);

      void resetMaximumLengthofSimulation();

      bool setGFunctionReferenceRatio(double gFunctionReferenceRatio);

      void resetGFunctionReferenceRatio();

      bool setUndisturbedGroundTemperatureModel(const ModelObject& undisturbedGroundTemperatureModel);

      //@}
      /** @name Other */
      //@{

      bool addGFunction(double gFunctionLN, double gFunctionGValue);

      void removeGFunction(unsigned groupIndex);

      void removeAllGFunctions();

      std::vector<std::pair<double, double>> gFunctions();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GroundHeatExchangerVertical");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GROUNDHEATEXCHANGERVERTICAL_IMPL_HPP
