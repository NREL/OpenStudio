/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_GROUNDHEATEXCHANGERHORIZONTALTRENCH_IMPL_HPP
#define MODEL_GROUNDHEATEXCHANGERHORIZONTALTRENCH_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** GroundHeatExchangerHorizontalTrench_Impl is a StraightComponent_Impl that is the implementation class for GroundHeatExchangerHorizontalTrench.*/
  class MODEL_API GroundHeatExchangerHorizontalTrench_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    GroundHeatExchangerHorizontalTrench_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle);

    GroundHeatExchangerHorizontalTrench_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    GroundHeatExchangerHorizontalTrench_Impl(const GroundHeatExchangerHorizontalTrench_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    virtual ~GroundHeatExchangerHorizontalTrench_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

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

    std::string groundTemperatureModel() const;

    bool isGroundTemperatureModelDefaulted() const;

    double kusudaAchenbachAverageSurfaceTemperature() const;

    double kusudaAchenbachAverageAmplitudeofSurfaceTemperature() const;

    double kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature() const;

    double evapotranspirationGroundCoverParameter() const;

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

    bool setGroundTemperatureModel(std::string groundTemperatureModel);

    void resetGroundTemperatureModel();

    void setKusudaAchenbachAverageSurfaceTemperature(double kusudaAchenbachAverageSurfaceTemperature);

    void setKusudaAchenbachAverageAmplitudeofSurfaceTemperature(double kusudaAchenbachAverageAmplitudeofSurfaceTemperature);

    void setKusudaAchenbachPhaseShiftofMinimumSurfaceTemperature(double kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature);

    bool setEvapotranspirationGroundCoverParameter(double evapotranspirationGroundCoverParameter);

    //@}
    /** @name Other */
    //@{

    unsigned inletPort() override;

    unsigned outletPort() override;

    bool addToNode(Node & node) override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.GroundHeatExchangerHorizontalTrench");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_GROUNDHEATEXCHANGERHORIZONTALTRENCH_IMPL_HPP

