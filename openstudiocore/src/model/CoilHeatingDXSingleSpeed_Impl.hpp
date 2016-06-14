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

#ifndef MODEL_COILHEATINGDXSINGLESPEED_IMPL_HPP
#define MODEL_COILHEATINGDXSINGLESPEED_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Curve;
class ZoneHVACComponent;

namespace detail {

/** CoilHeatingDXSingleSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingDXSingleSpeed.*/
class MODEL_API CoilHeatingDXSingleSpeed_Impl : public StraightComponent_Impl {
  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
 public:
  /** @name Constructors and Destructors */
  //@{

  CoilHeatingDXSingleSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  CoilHeatingDXSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

  CoilHeatingDXSingleSpeed_Impl(const CoilHeatingDXSingleSpeed_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

  virtual ~CoilHeatingDXSingleSpeed_Impl() {}

  //@}
  /** @name Virtual Methods */
  //@{

  virtual ModelObject clone(Model model) const override;

  virtual const std::vector<std::string>& outputVariableNames() const override;

  virtual IddObjectType iddObjectType() const override;

  virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

  virtual std::vector<ModelObject> children() const override;

  virtual unsigned inletPort() override;

  virtual unsigned outletPort() override;

  virtual boost::optional<HVACComponent> containingHVACComponent() const override;

  virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

  //@}
  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  boost::optional<double> ratedTotalHeatingCapacity() const;

  bool isRatedTotalHeatingCapacityAutosized() const;

  double ratedCOP() const;

  boost::optional<double> ratedAirFlowRate() const;

  double ratedSupplyFanPowerPerVolumeFlowRate() const;

  bool isRatedAirFlowRateAutosized() const;

  double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

  bool isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted() const;

  double maximumOutdoorDryBulbTemperatureforDefrostOperation() const;

  bool isMaximumOutdoorDryBulbTemperatureforDefrostOperationDefaulted() const;

  double crankcaseHeaterCapacity() const;

  bool isCrankcaseHeaterCapacityDefaulted() const;

  double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

  bool isMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperationDefaulted() const;

  std::string defrostStrategy() const;

  bool isDefrostStrategyDefaulted() const;

  std::string defrostControl() const;

  bool isDefrostControlDefaulted() const;

  double defrostTimePeriodFraction() const;

  bool isDefrostTimePeriodFractionDefaulted() const;

  boost::optional<double> resistiveDefrostHeaterCapacity() const;

  bool isResistiveDefrostHeaterCapacityDefaulted() const;

  bool isResistiveDefrostHeaterCapacityAutosized() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setRatedTotalHeatingCapacity(boost::optional<double> ratedTotalHeatingCapacity);

  void autosizeRatedTotalHeatingCapacity();

  bool setRatedCOP(double ratedCOP);

  bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

  void autosizeRatedAirFlowRate();

  bool setRatedSupplyFanPowerPerVolumeFlowRate(double ratedSupplyFanPowerPerVolumeFlowRate);

  bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

  void resetMinimumOutdoorDryBulbTemperatureforCompressorOperation();

  bool setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation);

  void resetMaximumOutdoorDryBulbTemperatureforDefrostOperation();

  bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

  void resetCrankcaseHeaterCapacity();

  bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

  void resetMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();

  bool setDefrostStrategy(std::string defrostStrategy);

  void resetDefrostStrategy();

  bool setDefrostControl(std::string defrostControl);

  void resetDefrostControl();

  bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

  void resetDefrostTimePeriodFraction();

  bool setResistiveDefrostHeaterCapacity(boost::optional<double> resistiveDefrostHeaterCapacity);

  void resetResistiveDefrostHeaterCapacity();

  void autosizeResistiveDefrostHeaterCapacity();

  //@}
  /** @name Curves */
  //@{

  // A5 , \field Total Heating Capacity Function of Temperature Curve Name
  // \object-list BiquadraticQuadraticCubicCurves
  Curve totalHeatingCapacityFunctionofTemperatureCurve() const;
  bool setTotalHeatingCapacityFunctionofTemperatureCurve( const Curve& curve );

  // A6 , \field Total Heating Capacity Function of Flow Fraction Curve Name
  // \object-list QuadraticCubicCurves
  Curve totalHeatingCapacityFunctionofFlowFractionCurve() const;
  bool setTotalHeatingCapacityFunctionofFlowFractionCurve( const Curve& curve );  

  // A7 ,  \field Energy Input Ratio Function of Temperature Curve Name
  // \object-list BiquadraticQuadraticCubicCurves
  Curve energyInputRatioFunctionofTemperatureCurve() const;
  bool setEnergyInputRatioFunctionofTemperatureCurve( const Curve& curve );

  // A8 , \field Energy Input Ratio Function of Flow Fraction Curve Name
  // \object-list QuadraticCubicCurves
  Curve energyInputRatioFunctionofFlowFractionCurve() const;
  bool setEnergyInputRatioFunctionofFlowFractionCurve( const Curve& curve );

  // A9 , \field Part Load Fraction Correlation Curve Name
  // \object-list QuadraticCubicCurves
  Curve partLoadFractionCorrelationCurve() const;
  bool setPartLoadFractionCorrelationCurve( const Curve& curve );  

  // A10, \field Defrost Energy Input Ratio Function of Temperature Curve Name
  // \object-list BiquadraticCurves
  // not a required curve so it needs a reset 
  boost::optional<Curve> defrostEnergyInputRatioFunctionofTemperatureCurve() const;
  bool setDefrostEnergyInputRatioFunctionofTemperatureCurve( const boost::optional<Curve> curve );

  bool addToNode(Node & node) override;

  //@}
 protected:
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingDXSingleSpeed");

  boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

  bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
};

} // detail

} // model

} // openstudio

#endif // MODEL_COILHEATINGDXSINGLESPEED_IMPL_HPP

