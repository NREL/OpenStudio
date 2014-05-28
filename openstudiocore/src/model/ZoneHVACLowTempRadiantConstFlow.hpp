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

#ifndef MODEL_ZONEHVACLOWTEMPRADIANTCONSTFLOW_HPP
#define MODEL_ZONEHVACLOWTEMPRADIANTCONSTFLOW_HPP

#include "ModelAPI.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  class ZoneHVACLowTempRadiantConstFlow_Impl;

} // detail

/** ZoneHVACLowTempRadiantConstFlow is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:LowTemperatureRadiant:ConstantFlow'. */
class MODEL_API ZoneHVACLowTempRadiantConstFlow : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  ZoneHVACLowTempRadiantConstFlow(const Model& model, 
                                  Schedule& availabilitySchedule,
                                  HVACComponent& heatingCoil,
                                  HVACComponent& coolingCoil,
                                  double hydronicTubingLength);

  virtual ~ZoneHVACLowTempRadiantConstFlow() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> radiantSurfaceTypeValues();

  static std::vector<std::string> temperatureControlTypeValues();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  boost::optional<std::string> radiantSurfaceType() const;

  std::vector<Surface> surfaces() const;

  double hydronicTubingInsideDiameter() const;

  bool isHydronicTubingInsideDiameterDefaulted() const;

  double hydronicTubingLength() const;

  std::string temperatureControlType() const;

  bool isTemperatureControlTypeDefaulted() const;

  HVACComponent heatingCoil() const;

  HVACComponent coolingCoil() const;
 
  boost::optional<double> ratedFlowRate() const;

  boost::optional<Schedule> pumpFlowRateSchedule() const;

  double ratedPumpHead() const;

  bool isRatedPumpHeadDefaulted() const;

  boost::optional<double> ratedPowerConsumption() const;

  double motorEfficiency() const;

  bool isMotorEfficiencyDefaulted() const;

  double fractionofMotorInefficienciestoFluidStream() const;

  bool isFractionofMotorInefficienciestoFluidStreamDefaulted() const;

  std::string numberofCircuits() const;

  double circuitLength() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setRadiantSurfaceType(std::string radiantSurfaceType);

  void resetRadiantSurfaceType();

  bool setHydronicTubingInsideDiameter(double hydronicTubingInsideDiameter);

  void resetHydronicTubingInsideDiameter();

  bool setHydronicTubingLength(double hydronicTubingLength);

  void resetHydronicTubingLength();

  bool setTemperatureControlType(std::string temperatureControlType);

  void resetTemperatureControlType();

  bool setHeatingCoil(HVACComponent& heatingCoil);

  bool setCoolingCoil(HVACComponent& coolingCoil);

  void setRatedFlowRate(double ratedFlowRate);

  void resetRatedFlowRate();

  bool setPumpFlowRateSchedule(Schedule& schedule);

  void resetPumpFlowRateSchedule();

  void setRatedPumpHead(double ratedPumpHead);

  void resetRatedPumpHead();

  void setRatedPowerConsumption(double ratedPowerConsumption);

  void resetRatedPowerConsumption();

  bool setMotorEfficiency(double motorEfficiency);

  void resetMotorEfficiency();

  bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

  void resetFractionofMotorInefficienciestoFluidStream();

  bool setNumberofCircuits(std::string numberofCircuits);

  void setCircuitLength(double circuitLength);

  boost::optional<ThermalZone> thermalZone() const;

  bool addToThermalZone(ThermalZone & thermalZone);

  void removeFromThermalZone();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACLowTempRadiantConstFlow_Impl ImplType;

  explicit ZoneHVACLowTempRadiantConstFlow(std::shared_ptr<detail::ZoneHVACLowTempRadiantConstFlow_Impl> impl);

  friend class detail::ZoneHVACLowTempRadiantConstFlow_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACLowTempRadiantConstFlow");
};

/** \relates ZoneHVACLowTempRadiantConstFlow*/
typedef boost::optional<ZoneHVACLowTempRadiantConstFlow> OptionalZoneHVACLowTempRadiantConstFlow;

/** \relates ZoneHVACLowTempRadiantConstFlow*/
typedef std::vector<ZoneHVACLowTempRadiantConstFlow> ZoneHVACLowTempRadiantConstFlowVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACLOWTEMPRADIANTCONSTFLOW_HPP

