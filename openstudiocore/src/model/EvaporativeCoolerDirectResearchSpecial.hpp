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

#ifndef MODEL_EVAPORATIVECOOLERDIRECTRESEARCHSPECIAL_HPP
#define MODEL_EVAPORATIVECOOLERDIRECTRESEARCHSPECIAL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Curve;

namespace detail {
  class EvaporativeCoolerDirectResearchSpecial_Impl;
} // detail

/** EvaporativeCoolerDirectResearchSpecial is a StraightComponent that wraps the
 *  IDD object named "OS:EvaporativeCooler:Direct:ResearchSpecial". */
class MODEL_API EvaporativeCoolerDirectResearchSpecial : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Constructs a new EvaporativeCoolerDirectResearchSpecial object and places it inside the
   *  model.  The object is fully initialized with all companion objects.
   */
  explicit EvaporativeCoolerDirectResearchSpecial(const Model& model, Schedule & schedule);

  virtual ~EvaporativeCoolerDirectResearchSpecial() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters and Setters */
  //@{

  /** Returns the Schedule referred to by the Availability Schedule field. **/
  Schedule availabilitySchedule() const;

  /** \deprecated */
  Schedule availableSchedule() const;

  /** Sets the Schedule referred to by the AvailabilitySchedule field. **/
  bool setAvailabilitySchedule(Schedule& schedule);

  /** \deprecated */
  bool setAvailableSchedule(Schedule& schedule);

  /** Returns the value of the CoolerEffectiveness field. **/
  /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooler Design Effectiveness" **/
  double coolerEffectiveness() const;

  /** Sets the value of the CoolerEffectiveness field. **/
  void setCoolerEffectiveness( double value );

  /** Returns the value of the RecirculatingWaterPumpPowerConsumption field. **/
  /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Recirculating Water Pump Design Power" **/
  boost::optional<double> recirculatingWaterPumpPowerConsumption() const;

  /** Sets the value of the RecirculatingWaterPumpPowerConsumption field. **/
  void setRecirculatingWaterPumpPowerConsumption( double value );

  void autosizeRecirculatingWaterPumpPowerConsumption();

  bool isRecirculatingWaterPumpPowerConsumptionAutosized() const;

  /** Returns the value of the PrimaryAirDesignFlowRate field. **/
  boost::optional<double> primaryAirDesignFlowRate() const;

  /** Sets the value of the PrimaryAirDesignFlowRate field. **/
  void setPrimaryAirDesignFlowRate( double value );

  void autosizePrimaryAirDesignFlowRate();

  bool isPrimaryAirDesignFlowRateAutosized() const;

  /** Returns the Node referred to by the SensorNodeName field. **/
  boost::optional<Node> sensorNode() const;

  /** Sets the Node referred to by the SensorNodeName field. **/
  void setSensorNode( const Node & node );

  /** Returns the value of the DriftLossFraction field. **/
  double driftLossFraction() const;

  /** Sets the value of the DriftLossFraction field. **/
  void setDriftLossFraction( double value );

  /** Returns the value of the BlowdownConcentrationRatio field. **/
  double blowdownConcentrationRatio() const;

  /** Sets the value of the BlowdownConcentrationRatio field. **/
  void setBlowdownConcentrationRatio( double value );

  boost::optional<Curve> effectivenessFlowRatioModifierCurve() const;

  bool setEffectivenessFlowRatioModifierCurve(const Curve& curve);

  void resetEffectivenessFlowRatioModifierCurve();

  double waterPumpPowerSizingFactor() const;

  void setWaterPumpPowerSizingFactor(double waterPumpPowerSizingFactor);

  boost::optional<Curve> waterPumpPowerModifierCurve() const;

  bool setWaterPumpPowerModifierCurve(const Curve& curve);

  void resetWaterPumpPowerModifierCurve();

  //@}
 protected:
  friend class Model;
  friend class openstudio::IdfObject;

  /// @cond
  typedef detail::EvaporativeCoolerDirectResearchSpecial_Impl ImplType;

  explicit EvaporativeCoolerDirectResearchSpecial(std::shared_ptr<detail::EvaporativeCoolerDirectResearchSpecial_Impl> impl);
 private:
  REGISTER_LOGGER("openstudio.model.EvaporativeCoolerDirectResearchSpecial");
  /// @endcond
};

/** \relates EvaporativeCoolerDirectResearchSpecial */
typedef boost::optional<EvaporativeCoolerDirectResearchSpecial> OptionalEvaporativeCoolerDirectResearchSpecial;

/** \relates EvaporativeCoolerDirectResearchSpecial */
typedef std::vector<EvaporativeCoolerDirectResearchSpecial> EvaporativeCoolerDirectResearchSpecialVector;

} // model
} // openstudio

#endif // MODEL_EVAPORATIVECOOLERDIRECTRESEARCHSPECIAL_HPP

