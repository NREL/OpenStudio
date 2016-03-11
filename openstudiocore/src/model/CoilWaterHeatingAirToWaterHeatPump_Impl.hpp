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

#ifndef MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_IMPL_HPP
#define MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_IMPL_HPP

#include "ModelAPI.hpp"
#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;

namespace detail {

  /** CoilWaterHeatingAirToWaterHeatPump_Impl is a ModelObject_Impl that is the implementation class for CoilWaterHeatingAirToWaterHeatPump.*/
  class MODEL_API CoilWaterHeatingAirToWaterHeatPump_Impl : public HVACComponent_Impl {
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

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

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

    bool isRatedEvaporatorAirFlowRateAutosized() const;

    boost::optional<double> ratedCondenserWaterFlowRate() const;

    bool isRatedCondenserWaterFlowRateAutosized() const;

    bool evaporatorFanPowerIncludedinRatedCOP() const;

    bool condenserPumpPowerIncludedinRatedCOP() const;

    bool condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP() const;

    double condenserWaterPumpPower() const;

    double fractionofCondenserPumpHeattoWater() const;

    double crankcaseHeaterCapacity() const;

    double maximumAmbientTemperatureforCrankcaseHeaterOperation() const;

    std::string evaporatorAirTemperatureTypeforCurveObjects() const;

    Curve heatingCapacityFunctionofTemperatureCurve() const;

    Curve heatingCapacityFunctionofAirFlowFractionCurve() const;

    Curve heatingCapacityFunctionofWaterFlowFractionCurve() const;

    Curve heatingCOPFunctionofTemperatureCurve() const;

    Curve heatingCOPFunctionofAirFlowFractionCurve() const;

    Curve heatingCOPFunctionofWaterFlowFractionCurve() const;

    Curve partLoadFractionCorrelationCurve() const;

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

    void autosizeRatedEvaporatorAirFlowRate();

    bool setRatedCondenserWaterFlowRate(boost::optional<double> ratedCondenserWaterFlowRate);

    void autosizeRatedCondenserWaterFlowRate();

    void setEvaporatorFanPowerIncludedinRatedCOP(bool evaporatorFanPowerIncludedinRatedCOP);

    void setCondenserPumpPowerIncludedinRatedCOP(bool condenserPumpPowerIncludedinRatedCOP);

    void setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(bool condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP);

    bool setCondenserWaterPumpPower(double condenserWaterPumpPower);

    bool setFractionofCondenserPumpHeattoWater(double fractionofCondenserPumpHeattoWater);

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation);

    bool setEvaporatorAirTemperatureTypeforCurveObjects(std::string evaporatorAirTemperatureTypeforCurveObjects);

    bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve);

    bool setHeatingCOPFunctionofTemperatureCurve(const Curve& curve);

    bool setHeatingCOPFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setHeatingCOPFunctionofWaterFlowFractionCurve(const Curve& curve);

    bool setPartLoadFractionCorrelationCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    std::vector<ModelObject> children() const override;
    ModelObject clone(Model model) const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilWaterHeatingAirToWaterHeatPump");

    boost::optional<Curve> optionalHeatingCapacityFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalHeatingCapacityFunctionofAirFlowFractionCurve() const;
    boost::optional<Curve> optionalHeatingCapacityFunctionofWaterFlowFractionCurve() const;
    boost::optional<Curve> optionalHeatingCOPFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalHeatingCOPFunctionofAirFlowFractionCurve() const;
    boost::optional<Curve> optionalHeatingCOPFunctionofWaterFlowFractionCurve() const;
    boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_IMPL_HPP

