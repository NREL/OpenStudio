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

#ifndef MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_IMPL_HPP
#define MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

class CurveBiquadratic;
class CurveQuadratic;

namespace detail {

  /** CoilWaterHeatingAirToWaterHeatPump_Impl is a WaterToAirComponent_Impl that is the implementation class for CoilWaterHeatingAirToWaterHeatPump.*/
  class MODEL_API CoilWaterHeatingAirToWaterHeatPump_Impl : public WaterToAirComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilWaterHeatingAirToWaterHeatPump_Impl(const IdfObject& idfObject,
                                            Model_Impl* model,
                                            bool keepHandle);

    CoilWaterHeatingAirToWaterHeatPump_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                            Model_Impl* model,
                                            bool keepHandle);

    CoilWaterHeatingAirToWaterHeatPump_Impl(const CoilWaterHeatingAirToWaterHeatPump_Impl& other,
                                            Model_Impl* model,
                                            bool keepHandle);

    virtual ~CoilWaterHeatingAirToWaterHeatPump_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual unsigned airInletPort() override;

    virtual unsigned airOutletPort() override;

    virtual unsigned waterInletPort() override;

    virtual unsigned waterOutletPort() override;

    //@}
    /** @name Getters */
    //@{

    double ratedHeatingCapacity() const;

    double ratedCOP() const;

    double ratedSensibleHeatRatio() const;

    double ratedEvaporatorInletAirDryBulbTemperature() const;

    double ratedEvaporatorInletAirWetBulbTemperature() const;

    double ratedCondenserInletWaterTemperature() const;

    boost::optional<double> ratedEvaporatorAirFlowRate() const;

    bool isRatedEvaporatorAirFlowRateAutocalculated() const;

    boost::optional<double> ratedCondenserWaterFlowRate() const;

    bool isRatedCondenserWaterFlowRateAutocalculated() const;

    bool evaporatorFanPowerIncludedinRatedCOP() const;

    bool condenserPumpPowerIncludedinRatedCOP() const;

    bool condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP() const;

    double condenserWaterPumpPower() const;

    double fractionofCondenserPumpHeattoWater() const;

    double crankcaseHeaterCapacity() const;

    double maximumAmbientTemperatureforCrankcaseHeaterOperation() const;

    std::string evaporatorAirTemperatureTypeforCurveObjects() const;

    CurveBiquadratic heatingCapacityFunctionofTemperatureCurve() const;

    CurveQuadratic heatingCapacityFunctionofAirFlowFractionCurve() const;

    CurveQuadratic heatingCapacityFunctionofWaterFlowFractionCurve() const;

    CurveBiquadratic heatingCOPFunctionofTemperatureCurve() const;

    CurveQuadratic heatingCOPFunctionofAirFlowFractionCurve() const;

    CurveQuadratic heatingCOPFunctionofWaterFlowFractionCurve() const;

    CurveQuadratic partLoadFractionCorrelationCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedHeatingCapacity(double ratedHeatingCapacity);

    bool setRatedCOP(double ratedCOP);

    bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

    bool setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature);

    bool setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletAirWetBulbTemperature);

    bool setRatedCondenserInletWaterTemperature(double ratedCondenserInletWaterTemperature);

    bool setRatedEvaporatorAirFlowRate(boost::optional<double> ratedEvaporatorAirFlowRate);

    void autocalculateRatedEvaporatorAirFlowRate();

    bool setRatedCondenserWaterFlowRate(boost::optional<double> ratedCondenserWaterFlowRate);

    void autocalculateRatedCondenserWaterFlowRate();

    void setEvaporatorFanPowerIncludedinRatedCOP(bool evaporatorFanPowerIncludedinRatedCOP);

    void setCondenserPumpPowerIncludedinRatedCOP(bool condenserPumpPowerIncludedinRatedCOP);

    void setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(bool condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP);

    bool setCondenserWaterPumpPower(double condenserWaterPumpPower);

    bool setFractionofCondenserPumpHeattoWater(double fractionofCondenserPumpHeattoWater);

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation);

    bool setEvaporatorAirTemperatureTypeforCurveObjects(std::string evaporatorAirTemperatureTypeforCurveObjects);

    bool setHeatingCapacityFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic);

    bool setHeatingCapacityFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic);

    bool setHeatingCapacityFunctionofWaterFlowFractionCurve(const CurveQuadratic& curveQuadratic);

    bool setHeatingCOPFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic);

    bool setHeatingCOPFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic);

    bool setHeatingCOPFunctionofWaterFlowFractionCurve(const CurveQuadratic& curveQuadratic);

    bool setPartLoadFractionCorrelationCurve(const CurveQuadratic& curveQuadratic);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilWaterHeatingAirToWaterHeatPump");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<CurveBiquadratic> optionalHeatingCapacityFunctionofTemperatureCurve() const;
    boost::optional<CurveQuadratic> optionalHeatingCapacityFunctionofAirFlowFractionCurve() const;
    boost::optional<CurveQuadratic> optionalHeatingCapacityFunctionofWaterFlowFractionCurve() const;
    boost::optional<CurveBiquadratic> optionalHeatingCOPFunctionofTemperatureCurve() const;
    boost::optional<CurveQuadratic> optionalHeatingCOPFunctionofAirFlowFractionCurve() const;
    boost::optional<CurveQuadratic> optionalHeatingCOPFunctionofWaterFlowFractionCurve() const;
    boost::optional<CurveQuadratic> optionalPartLoadFractionCorrelationCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_IMPL_HPP

