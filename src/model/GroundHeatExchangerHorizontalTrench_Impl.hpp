/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GROUNDHEATEXCHANGERHORIZONTALTRENCH_IMPL_HPP
#define MODEL_GROUNDHEATEXCHANGERHORIZONTALTRENCH_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    /** GroundHeatExchangerHorizontalTrench_Impl is a StraightComponent_Impl that is the implementation class for GroundHeatExchangerHorizontalTrench.*/
    class MODEL_API GroundHeatExchangerHorizontalTrench_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      GroundHeatExchangerHorizontalTrench_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GroundHeatExchangerHorizontalTrench_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GroundHeatExchangerHorizontalTrench_Impl(const GroundHeatExchangerHorizontalTrench_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GroundHeatExchangerHorizontalTrench_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

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

      double designFlowRate() const;

      double trenchLengthinPipeAxialDirection() const;

      int numberofTrenches() const;

      double horizontalSpacingBetweenPipes() const;

      double pipeInnerDiameter() const;

      double pipeOuterDiameter() const;

      double burialDepth() const;

      double soilThermalConductivity() const;

      double soilDensity() const;

      double soilSpecificHeat() const;

      double pipeThermalConductivity() const;

      double pipeDensity() const;

      double pipeSpecificHeat() const;

      double soilMoistureContentPercent() const;

      double soilMoistureContentPercentatSaturation() const;

      double evapotranspirationGroundCoverParameter() const;

      ModelObject undisturbedGroundTemperatureModel() const;

      //@}
      /** @name Setters */
      //@{

      bool setDesignFlowRate(double designFlowRate);

      bool setTrenchLengthinPipeAxialDirection(double trenchLengthinPipeAxialDirection);

      bool setNumberofTrenches(int numberofTrenches);

      bool setHorizontalSpacingBetweenPipes(double horizontalSpacingBetweenPipes);

      bool setPipeInnerDiameter(double pipeInnerDiameter);

      bool setPipeOuterDiameter(double pipeOuterDiameter);

      bool setBurialDepth(double burialDepth);

      bool setSoilThermalConductivity(double soilThermalConductivity);

      bool setSoilDensity(double soilDensity);

      bool setSoilSpecificHeat(double soilSpecificHeat);

      bool setPipeThermalConductivity(double pipeThermalConductivity);

      bool setPipeDensity(double pipeDensity);

      bool setPipeSpecificHeat(double pipeSpecificHeat);

      bool setSoilMoistureContentPercent(double soilMoistureContentPercent);

      bool setSoilMoistureContentPercentatSaturation(double soilMoistureContentPercentatSaturation);

      bool setEvapotranspirationGroundCoverParameter(double evapotranspirationGroundCoverParameter);

      bool setUndisturbedGroundTemperatureModel(const ModelObject& undisturbedGroundTemperatureModel);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GroundHeatExchangerHorizontalTrench");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GROUNDHEATEXCHANGERHORIZONTALTRENCH_IMPL_HPP
