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

#ifndef MODEL_COILCOOLINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP
#define MODEL_COILCOOLINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API CoilCoolingWaterToAirHeatPumpEquationFit_Impl : public WaterToAirComponent_Impl
  {

    Q_OBJECT

   public:
    /** @name Constructors and Destructors */

    CoilCoolingWaterToAirHeatPumpEquationFit_Impl(const IdfObject& idfObject,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    CoilCoolingWaterToAirHeatPumpEquationFit_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    CoilCoolingWaterToAirHeatPumpEquationFit_Impl(const CoilCoolingWaterToAirHeatPumpEquationFit_Impl& other,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    virtual ~CoilCoolingWaterToAirHeatPumpEquationFit_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const;

    virtual std::vector<IdfObject> remove();

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //virtual bool addToNode(Node & node);

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

    boost::optional<double> ratedTotalCoolingCapacity() const;

    bool isRatedTotalCoolingCapacityDefaulted() const;

    bool isRatedTotalCoolingCapacityAutosized() const;

    boost::optional<double> ratedSensibleCoolingCapacity() const;

    bool isRatedSensibleCoolingCapacityDefaulted() const;

    bool isRatedSensibleCoolingCapacityAutosized() const;

    double ratedCoolingCoefficientofPerformance() const;

    bool isRatedCoolingCoefficientofPerformanceDefaulted() const;

    double totalCoolingCapacityCoefficient1() const;

    double totalCoolingCapacityCoefficient2() const;

    double totalCoolingCapacityCoefficient3() const;

    double totalCoolingCapacityCoefficient4() const;

    double totalCoolingCapacityCoefficient5() const;

    double sensibleCoolingCapacityCoefficient1() const;

    double sensibleCoolingCapacityCoefficient2() const;

    double sensibleCoolingCapacityCoefficient3() const;

    double sensibleCoolingCapacityCoefficient4() const;

    double sensibleCoolingCapacityCoefficient5() const;

    double sensibleCoolingCapacityCoefficient6() const;

    double coolingPowerConsumptionCoefficient1() const;

    double coolingPowerConsumptionCoefficient2() const;

    double coolingPowerConsumptionCoefficient3() const;

    double coolingPowerConsumptionCoefficient4() const;

    double coolingPowerConsumptionCoefficient5() const;

    double nominalTimeforCondensateRemovaltoBegin() const;

    bool isNominalTimeforCondensateRemovaltoBeginDefaulted() const;

    double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const;

    bool isRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacityDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

    void resetRatedAirFlowRate();

    void autosizeRatedAirFlowRate();

    bool setRatedWaterFlowRate(boost::optional<double> ratedWaterFlowRate);

    void resetRatedWaterFlowRate();

    void autosizeRatedWaterFlowRate();

    bool setRatedTotalCoolingCapacity(boost::optional<double> ratedTotalCoolingCapacity);

    void resetRatedTotalCoolingCapacity();

    void autosizeRatedTotalCoolingCapacity();

    bool setRatedSensibleCoolingCapacity(boost::optional<double> ratedSensibleCoolingCapacity);

    void resetRatedSensibleCoolingCapacity();

    void autosizeRatedSensibleCoolingCapacity();

    bool setRatedCoolingCoefficientofPerformance(double ratedCoolingCoefficientofPerformance);

    void setTotalCoolingCapacityCoefficient1(double totalCoolingCapacityCoefficient1);

    void setTotalCoolingCapacityCoefficient2(double totalCoolingCapacityCoefficient2);

    void setTotalCoolingCapacityCoefficient3(double totalCoolingCapacityCoefficient3);

    void setTotalCoolingCapacityCoefficient4(double totalCoolingCapacityCoefficient4);

    void setTotalCoolingCapacityCoefficient5(double totalCoolingCapacityCoefficient5);

    void setSensibleCoolingCapacityCoefficient1(double sensibleCoolingCapacityCoefficient1);

    void setSensibleCoolingCapacityCoefficient2(double sensibleCoolingCapacityCoefficient2);

    void setSensibleCoolingCapacityCoefficient3(double sensibleCoolingCapacityCoefficient3);

    void setSensibleCoolingCapacityCoefficient4(double sensibleCoolingCapacityCoefficient4);

    void setSensibleCoolingCapacityCoefficient5(double sensibleCoolingCapacityCoefficient5);

    void setSensibleCoolingCapacityCoefficient6(double sensibleCoolingCapacityCoefficient6);

    void setCoolingPowerConsumptionCoefficient1(double coolingPowerConsumptionCoefficient1);

    void setCoolingPowerConsumptionCoefficient2(double coolingPowerConsumptionCoefficient2);

    void setCoolingPowerConsumptionCoefficient3(double coolingPowerConsumptionCoefficient3);

    void setCoolingPowerConsumptionCoefficient4(double coolingPowerConsumptionCoefficient4);

    void setCoolingPowerConsumptionCoefficient5(double coolingPowerConsumptionCoefficient5);

    bool setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin);

    void resetNominalTimeforCondensateRemovaltoBegin();

    bool setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);

    void resetRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingWaterToAirHeatPumpEquationFit");

    virtual boost::optional<HVACComponent> containingHVACComponent() const;
    
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP

