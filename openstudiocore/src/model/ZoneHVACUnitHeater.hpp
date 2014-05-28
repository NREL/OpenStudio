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

#ifndef MODEL_ZONEHVACUNITHEATER_HPP
#define MODEL_ZONEHVACUNITHEATER_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  class ZoneHVACUnitHeater_Impl;

} // detail

/** ZoneHVACUnitHeater is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:UnitHeater'. */

class MODEL_API ZoneHVACUnitHeater : public ZoneHVACComponent 
{
  public:

  /** @name Constructors and Destructors */

  ZoneHVACUnitHeater(const Model& model,
          Schedule & availabilitySchedule,
          HVACComponent & supplyAirFan,
          HVACComponent & heatingCoil);
  
  virtual ~ZoneHVACUnitHeater() {}


  static IddObjectType iddObjectType();

  static std::vector<std::string> fanControlTypeValues();

  /** @name Getters */

  Schedule availabilitySchedule() const;

  HVACComponent supplyAirFan() const;

  boost::optional<double> maximumSupplyAirFlowRate() const;

  bool isMaximumSupplyAirFlowRateAutosized() const;

  std::string fanControlType() const;

  HVACComponent heatingCoil() const;

  boost::optional<double> maximumHotWaterFlowRate() const;

  bool isMaximumHotWaterFlowRateAutosized() const;

  double minimumHotWaterFlowRate() const;

  bool isMinimumHotWaterFlowRateDefaulted() const;

  double heatingConvergenceTolerance() const;

  bool isHeatingConvergenceToleranceDefaulted() const;

  /** @name Setters */

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setSupplyAirFan(const HVACComponent & fan );

  bool setMaximumSupplyAirFlowRate(double maximumSupplyAirFlowRate);

  void autosizeMaximumSupplyAirFlowRate();

  bool setFanControlType(std::string fanControlType);

  bool setHeatingCoil(const HVACComponent & heatingCoil );

  bool setMaximumHotWaterFlowRate(double maximumHotWaterFlowRate);

  void resetMaximumHotWaterFlowRate();

  void autosizeMaximumHotWaterFlowRate();

  bool setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate);

  void resetMinimumHotWaterFlowRate();

  bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

  void resetHeatingConvergenceTolerance();

  /** @name Other */

 protected:
  /// @cond
  typedef detail::ZoneHVACUnitHeater_Impl ImplType;

  explicit ZoneHVACUnitHeater(std::shared_ptr<detail::ZoneHVACUnitHeater_Impl> impl);

  friend class detail::ZoneHVACUnitHeater_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond

 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACUnitHeater");
};

/** \relates ZoneHVACUnitHeater*/
typedef boost::optional<ZoneHVACUnitHeater> OptionalZoneHVACUnitHeater;

/** \relates ZoneHVACUnitHeater*/
typedef std::vector<ZoneHVACUnitHeater> ZoneHVACUnitHeaterVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACUNITHEATER_HPP

