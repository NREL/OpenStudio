/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_IMPL_HPP
#define MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

class CurveBiquadratic;
class CurveQuadratic;

namespace detail {

  /** CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl is a ParentObject_Impl that is the implementation class for CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.*/
  class MODEL_API CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const IdfObject& idfObject,
                                                                        Model_Impl* model,
                                                                        bool keepHandle);

    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                        Model_Impl* model,
                                                                        bool keepHandle);

    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl& other,
                                                                        Model_Impl* model,
                                                                        bool keepHandle);

    virtual ~CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    double referenceUnitGrossRatedTotalCoolingCapacity() const;

    double referenceUnitGrossRatedSensibleHeatRatio() const;

    double referenceUnitGrossRatedCoolingCOP() const;

    double referenceUnitRatedAirFlowRate() const;

    double referenceUnitRatedWaterFlowRate() const;

    CurveBiquadratic totalCoolingCapacityFunctionofTemperatureCurve() const;

    CurveQuadratic totalCoolingCapacityFunctionofAirFlowFractionCurve() const;

    CurveQuadratic totalCoolingCapacityFunctionofWaterFlowFractionCurve() const;

    CurveBiquadratic energyInputRatioFunctionofTemperatureCurve() const;

    CurveQuadratic energyInputRatioFunctionofAirFlowFractionCurve() const;

    CurveQuadratic energyInputRatioFunctionofWaterFlowFractionCurve() const;

    double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const;

    CurveBiquadratic wasteHeatFunctionofTemperatureCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceUnitGrossRatedTotalCoolingCapacity(double referenceUnitGrossRatedTotalCoolingCapacity);

    bool setReferenceUnitGrossRatedSensibleHeatRatio(double referenceUnitGrossRatedSensibleHeatRatio);

    bool setReferenceUnitGrossRatedCoolingCOP(double referenceUnitGrossRatedCoolingCOP);

    bool setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate);

    bool setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate);

    bool setTotalCoolingCapacityFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic);

    bool setTotalCoolingCapacityFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic);

    bool setTotalCoolingCapacityFunctionofWaterFlowFractionCurve(const CurveQuadratic& curveQuadratic);

    bool setEnergyInputRatioFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic);

    bool setEnergyInputRatioFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic);

    bool setEnergyInputRatioFunctionofWaterFlowFractionCurve(const CurveQuadratic& curveQuadratic);

    bool setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions);

    bool setWasteHeatFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<CurveBiquadratic> optionalTotalCoolingCapacityFunctionofTemperatureCurve() const;
    boost::optional<CurveQuadratic> optionalTotalCoolingCapacityFunctionofAirFlowFractionCurve() const;
    boost::optional<CurveQuadratic> optionalTotalCoolingCapacityFunctionofWaterFlowFractionCurve() const;
    boost::optional<CurveBiquadratic> optionalEnergyInputRatioFunctionofTemperatureCurve() const;
    boost::optional<CurveQuadratic> optionalEnergyInputRatioFunctionofAirFlowFractionCurve() const;
    boost::optional<CurveQuadratic> optionalEnergyInputRatioFunctionofWaterFlowFractionCurve() const;
    boost::optional<CurveBiquadratic> optionalWasteHeatFunctionofTemperatureCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_IMPL_HPP

