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

#ifndef MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPWRAPPED_IMPL_HPP
#define MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPWRAPPED_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;

namespace detail {

  /** CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl is a ModelObject_Impl that is the implementation class for CoilWaterHeatingAirToWaterHeatPumpWrapped.*/
  class MODEL_API CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl(const IdfObject& idfObject,
                                                   Model_Impl* model,
                                                   bool keepHandle);

    CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle);

    CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl(const CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle);

    virtual ~CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    double ratedHeatingCapacity() const;

    double ratedCOP() const;

    double ratedSensibleHeatRatio() const;

    double ratedEvaporatorInletAirDryBulbTemperature() const;

    double ratedEvaporatorInletAirWetBulbTemperature() const;

    double ratedCondenserWaterTemperature() const;

    boost::optional<double> ratedEvaporatorAirFlowRate() const;

    bool isRatedEvaporatorAirFlowRateAutocalculated() const;

    bool evaporatorFanPowerIncludedinRatedCOP() const;

    double crankcaseHeaterCapacity() const;

    double maximumAmbientTemperatureforCrankcaseHeaterOperation() const;

    std::string evaporatorAirTemperatureTypeforCurveObjects() const;

    Curve heatingCapacityFunctionofTemperatureCurve() const;

    Curve heatingCapacityFunctionofAirFlowFractionCurve() const;

    Curve heatingCOPFunctionofTemperatureCurve() const;

    Curve heatingCOPFunctionofAirFlowFractionCurve() const;

    Curve partLoadFractionCorrelationCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedHeatingCapacity(double ratedHeatingCapacity);

    bool setRatedCOP(double ratedCOP);

    bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

    bool setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature);

    bool setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletAirWetBulbTemperature);

    bool setRatedCondenserWaterTemperature(double ratedCondenserWaterTemperature);

    bool setRatedEvaporatorAirFlowRate(boost::optional<double> ratedEvaporatorAirFlowRate);

    void autocalculateRatedEvaporatorAirFlowRate();

    void setEvaporatorFanPowerIncludedinRatedCOP(bool evaporatorFanPowerIncludedinRatedCOP);

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation);

    bool setEvaporatorAirTemperatureTypeforCurveObjects(std::string evaporatorAirTemperatureTypeforCurveObjects);

    bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& biquadraticCubicCurves);

    bool setHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& quadraticCubicCurves);

    bool setHeatingCOPFunctionofTemperatureCurve(const Curve& biquadraticCubicCurves);

    bool setHeatingCOPFunctionofAirFlowFractionCurve(const Curve& quadraticCubicCurves);

    bool setPartLoadFractionCorrelationCurve(const Curve& quadraticCubicCurves);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilWaterHeatingAirToWaterHeatPumpWrapped");

    boost::optional<Curve> optionalHeatingCapacityFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalHeatingCapacityFunctionofAirFlowFractionCurve() const;
    boost::optional<Curve> optionalHeatingCOPFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalHeatingCOPFunctionofAirFlowFractionCurve() const;
    boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPWRAPPED_IMPL_HPP

