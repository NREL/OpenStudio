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

#ifndef MODEL_ZONEHVACLOWTEMPRADIANTVARFLOW_HPP
#define MODEL_ZONEHVACLOWTEMPRADIANTVARFLOW_HPP

#include "ModelAPI.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  class ZoneHVACLowTempRadiantVarFlow_Impl;

} // detail

/** ZoneHVACLowTempRadiantVarFlow is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:LowTemperatureRadiant:VariableFlow'. */
class MODEL_API ZoneHVACLowTempRadiantVarFlow : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneHVACLowTempRadiantVarFlow(const Model& model,
                                                                                  Schedule& availabilitySchedule,
                                                                                  HVACComponent& heatingCoil,
                                                                                 HVACComponent& coolingCoil);

  virtual ~ZoneHVACLowTempRadiantVarFlow() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> radiantSurfaceTypeValues();

  static std::vector<std::string> temperatureControlTypeValues();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  HVACComponent heatingCoil() const;

  HVACComponent coolingCoil() const;

  boost::optional<std::string> radiantSurfaceType() const;

  std::vector<Surface> surfaces() const;

  double hydronicTubingInsideDiameter() const;

  bool isHydronicTubingInsideDiameterDefaulted() const;

  boost::optional<double> hydronicTubingLength() const;

  bool isHydronicTubingLengthDefaulted() const;

  bool isHydronicTubingLengthAutosized() const;

  std::string temperatureControlType() const;

  bool isTemperatureControlTypeDefaulted() const;

  std::string numberofCircuits() const;

  double circuitLength() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setRadiantSurfaceType(std::string radiantSurfaceType);

  void resetRadiantSurfaceType();

  bool setHeatingCoil(HVACComponent& heatingCoil);

  bool setCoolingCoil(HVACComponent& coolingCoil);

  bool setHydronicTubingInsideDiameter(double hydronicTubingInsideDiameter);

  void resetHydronicTubingInsideDiameter();

  bool setHydronicTubingLength(double hydronicTubingLength);

  void resetHydronicTubingLength();

  void autosizeHydronicTubingLength();

  bool setTemperatureControlType(std::string temperatureControlType);

  void resetTemperatureControlType();

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
  typedef detail::ZoneHVACLowTempRadiantVarFlow_Impl ImplType;

  explicit ZoneHVACLowTempRadiantVarFlow(std::shared_ptr<detail::ZoneHVACLowTempRadiantVarFlow_Impl> impl);

  friend class detail::ZoneHVACLowTempRadiantVarFlow_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACLowTempRadiantVarFlow");
};

/** \relates ZoneHVACLowTempRadiantVarFlow*/
typedef boost::optional<ZoneHVACLowTempRadiantVarFlow> OptionalZoneHVACLowTempRadiantVarFlow;

/** \relates ZoneHVACLowTempRadiantVarFlow*/
typedef std::vector<ZoneHVACLowTempRadiantVarFlow> ZoneHVACLowTempRadiantVarFlowVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACLOWTEMPRADIANTVARFLOW_HPP

