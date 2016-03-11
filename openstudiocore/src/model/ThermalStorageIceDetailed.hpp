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

#ifndef MODEL_THERMALSTORAGEICEDETAILED_HPP
#define MODEL_THERMALSTORAGEICEDETAILED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class Curve;

namespace detail {

  class ThermalStorageIceDetailed_Impl;

} // detail

/** ThermalStorageIceDetailed is a StraightComponent that wraps the OpenStudio IDD object 'OS:ThermalStorage:Ice:Detailed'. */
class MODEL_API ThermalStorageIceDetailed : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ThermalStorageIceDetailed(const Model& model);

  virtual ~ThermalStorageIceDetailed() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> thawProcessIndicatorValues();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  double capacity() const;

  Curve dischargingCurve() const;

  Curve chargingCurve() const;

  double timestepoftheCurveData() const;

  double parasiticElectricLoadDuringDischarging() const;

  double parasiticElectricLoadDuringCharging() const;

  double tankLossCoefficient() const;

  double freezingTemperatureofStorageMedium() const;

  std::string thawProcessIndicator() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  void setCapacity(double capacity);

  bool setDischargingCurve(const Curve& dischargingCurve);

  bool setChargingCurve(const Curve& chargingCurve);

  void setTimestepoftheCurveData(double timestepoftheCurveData);

  void setParasiticElectricLoadDuringDischarging(double parasiticElectricLoadDuringDischarging);

  void setParasiticElectricLoadDuringCharging(double parasiticElectricLoadDuringCharging);

  void setTankLossCoefficient(double tankLossCoefficient);

  void setFreezingTemperatureofStorageMedium(double freezingTemperatureofStorageMedium);

  bool setThawProcessIndicator(std::string thawProcessIndicator);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ThermalStorageIceDetailed_Impl ImplType;

  explicit ThermalStorageIceDetailed(std::shared_ptr<detail::ThermalStorageIceDetailed_Impl> impl);

  friend class detail::ThermalStorageIceDetailed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ThermalStorageIceDetailed");
};

/** \relates ThermalStorageIceDetailed*/
typedef boost::optional<ThermalStorageIceDetailed> OptionalThermalStorageIceDetailed;

/** \relates ThermalStorageIceDetailed*/
typedef std::vector<ThermalStorageIceDetailed> ThermalStorageIceDetailedVector;

} // model
} // openstudio

#endif // MODEL_THERMALSTORAGEICEDETAILED_HPP

