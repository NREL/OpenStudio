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
  Q_OBJECT;

  Q_PROPERTY(boost::optional<double> ratedTotalHeatingCapacity READ ratedTotalHeatingCapacity WRITE setRatedTotalHeatingCapacity);
  Q_PROPERTY(bool isRatedTotalHeatingCapacityAutosized READ isRatedTotalHeatingCapacityAutosized);
  Q_PROPERTY(double ratedCOP READ ratedCOP WRITE setRatedCOP);
  Q_PROPERTY(boost::optional<double> ratedAirFlowRate READ ratedAirFlowRate WRITE setRatedAirFlowRate);
  Q_PROPERTY(bool isRatedAirFlowRateAutosized READ isRatedAirFlowRateAutosized);
  Q_PROPERTY(double minimumOutdoorDryBulbTemperatureforCompressorOperation READ minimumOutdoorDryBulbTemperatureforCompressorOperation WRITE setMinimumOutdoorDryBulbTemperatureforCompressorOperation RESET resetMinimumOutdoorDryBulbTemperatureforCompressorOperation);
  Q_PROPERTY(bool isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted READ isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted);
  Q_PROPERTY(double maximumOutdoorDryBulbTemperatureforDefrostOperation READ maximumOutdoorDryBulbTemperatureforDefrostOperation WRITE setMaximumOutdoorDryBulbTemperatureforDefrostOperation RESET resetMaximumOutdoorDryBulbTemperatureforDefrostOperation);
  Q_PROPERTY(bool isMaximumOutdoorDryBulbTemperatureforDefrostOperationDefaulted READ isMaximumOutdoorDryBulbTemperatureforDefrostOperationDefaulted);
  Q_PROPERTY(double crankcaseHeaterCapacity READ crankcaseHeaterCapacity WRITE setCrankcaseHeaterCapacity RESET resetCrankcaseHeaterCapacity);
  Q_PROPERTY(bool isCrankcaseHeaterCapacityDefaulted READ isCrankcaseHeaterCapacityDefaulted);
  Q_PROPERTY(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation READ maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation WRITE setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation RESET resetMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
  Q_PROPERTY(bool isMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperationDefaulted READ isMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperationDefaulted);
  Q_PROPERTY(std::string defrostStrategy READ defrostStrategy WRITE setDefrostStrategy RESET resetDefrostStrategy);
  Q_PROPERTY(bool isDefrostStrategyDefaulted READ isDefrostStrategyDefaulted);
  Q_PROPERTY(std::string defrostControl READ defrostControl WRITE setDefrostControl RESET resetDefrostControl);
  Q_PROPERTY(bool isDefrostControlDefaulted READ isDefrostControlDefaulted);
  Q_PROPERTY(double defrostTimePeriodFraction READ defrostTimePeriodFraction WRITE setDefrostTimePeriodFraction RESET resetDefrostTimePeriodFraction);
  Q_PROPERTY(bool isDefrostTimePeriodFractionDefaulted READ isDefrostTimePeriodFractionDefaulted);
  Q_PROPERTY(boost::optional<double> resistiveDefrostHeaterCapacity READ resistiveDefrostHeaterCapacity WRITE setResistiveDefrostHeaterCapacity RESET resetResistiveDefrostHeaterCapacity);
  Q_PROPERTY(bool isResistiveDefrostHeaterCapacityDefaulted READ isResistiveDefrostHeaterCapacityDefaulted);
  Q_PROPERTY(bool isResistiveDefrostHeaterCapacityAutosized READ isResistiveDefrostHeaterCapacityAutosized);

  Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject);
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

  virtual ModelObject clone(Model model) const;

  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual IddObjectType iddObjectType() const;

  virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

  virtual std::vector<ModelObject> children() const;

  virtual unsigned inletPort();

  virtual unsigned outletPort();

  virtual boost::optional<HVACComponent> containingHVACComponent() const;

  virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;

  //@}
  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  boost::optional<double> ratedTotalHeatingCapacity() const;

  bool isRatedTotalHeatingCapacityAutosized() const;

  double ratedCOP() const;

  boost::optional<double> ratedAirFlowRate() const;

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

  bool addToNode(Node & node);

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

