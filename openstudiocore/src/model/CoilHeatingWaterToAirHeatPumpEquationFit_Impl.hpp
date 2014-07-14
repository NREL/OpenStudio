/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_COILHEATINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP
#define MODEL_COILHEATINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** CoilHeatingWaterToAirHeatPumpEquationFit_Impl is a WaterToAirComponent_Impl that is the implementation class for CoilHeatingWaterToAirHeatPumpEquationFit.*/
  class MODEL_API CoilHeatingWaterToAirHeatPumpEquationFit_Impl : public WaterToAirComponent_Impl {

   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingWaterToAirHeatPumpEquationFit_Impl(const IdfObject& idfObject,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    CoilHeatingWaterToAirHeatPumpEquationFit_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    CoilHeatingWaterToAirHeatPumpEquationFit_Impl(const CoilHeatingWaterToAirHeatPumpEquationFit_Impl& other,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    virtual ~CoilHeatingWaterToAirHeatPumpEquationFit_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual unsigned airInletPort();

    virtual unsigned airOutletPort();

    virtual unsigned waterInletPort();

    virtual unsigned waterOutletPort();

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateDefaulted() const;

    bool isRatedAirFlowRateAutosized() const;

    boost::optional<double> ratedWaterFlowRate() const;

    bool isRatedWaterFlowRateDefaulted() const;

    bool isRatedWaterFlowRateAutosized() const;

    boost::optional<double> ratedHeatingCapacity() const;

    bool isRatedHeatingCapacityDefaulted() const;

    bool isRatedHeatingCapacityAutosized() const;

    double ratedHeatingCoefficientofPerformance() const;

    bool isRatedHeatingCoefficientofPerformanceDefaulted() const;

    double heatingCapacityCoefficient1() const;

    double heatingCapacityCoefficient2() const;

    double heatingCapacityCoefficient3() const;

    double heatingCapacityCoefficient4() const;

    double heatingCapacityCoefficient5() const;

    double heatingPowerConsumptionCoefficient1() const;

    double heatingPowerConsumptionCoefficient2() const;

    double heatingPowerConsumptionCoefficient3() const;

    double heatingPowerConsumptionCoefficient4() const;

    double heatingPowerConsumptionCoefficient5() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

    void resetRatedAirFlowRate();

    void autosizeRatedAirFlowRate();

    bool setRatedWaterFlowRate(boost::optional<double> ratedWaterFlowRate);

    void resetRatedWaterFlowRate();

    void autosizeRatedWaterFlowRate();

    bool setRatedHeatingCapacity(boost::optional<double> ratedHeatingCapacity);

    void resetRatedHeatingCapacity();

    void autosizeRatedHeatingCapacity();

    bool setRatedHeatingCoefficientofPerformance(double ratedHeatingCoefficientofPerformance);

    void resetRatedHeatingCoefficientofPerformance();

    void setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1);

    void setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2);

    void setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3);

    void setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4);

    void setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5);

    void setHeatingPowerConsumptionCoefficient1(double heatingPowerConsumptionCoefficient1);

    void setHeatingPowerConsumptionCoefficient2(double heatingPowerConsumptionCoefficient2);

    void setHeatingPowerConsumptionCoefficient3(double heatingPowerConsumptionCoefficient3);

    void setHeatingPowerConsumptionCoefficient4(double heatingPowerConsumptionCoefficient4);

    void setHeatingPowerConsumptionCoefficient5(double heatingPowerConsumptionCoefficient5);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingWaterToAirHeatPumpEquationFit");

    virtual boost::optional<HVACComponent> containingHVACComponent() const;

  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP

