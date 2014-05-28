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

#ifndef MODEL_FANZONEEXHAUST_HPP
#define MODEL_FANZONEEXHAUST_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class FanZoneExhaust_Impl;

} // detail

/** FanZoneExhaust is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:Fan:ZoneExhaust'. */
class MODEL_API FanZoneExhaust : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit FanZoneExhaust(const Model& model);

  virtual ~FanZoneExhaust() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> systemAvailabilityManagerCouplingModeValues();
  
  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  double fanEfficiency() const;

  double pressureRise() const;

  boost::optional<double> maximumFlowRate() const;

  std::string endUseSubcategory() const;

  boost::optional<Schedule> flowFractionSchedule() const;

  std::string systemAvailabilityManagerCouplingMode() const;

  boost::optional<Schedule> minimumZoneTemperatureLimitSchedule() const;

  boost::optional<Schedule> balancedExhaustFractionSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setFanEfficiency(double fanEfficiency);

  void setPressureRise(double pressureRise);

  bool setMaximumFlowRate(double maximumFlowRate);

  void resetMaximumFlowRate();

  void setEndUseSubcategory(std::string endUseSubcategory);

  bool setFlowFractionSchedule(Schedule& schedule);

  void resetFlowFractionSchedule();

  bool setSystemAvailabilityManagerCouplingMode(std::string systemAvailabilityManagerCouplingMode);

  bool setMinimumZoneTemperatureLimitSchedule(Schedule& schedule);

  void resetMinimumZoneTemperatureLimitSchedule();

  bool setBalancedExhaustFractionSchedule(Schedule& schedule);

  void resetBalancedExhaustFractionSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::FanZoneExhaust_Impl ImplType;

  explicit FanZoneExhaust(std::shared_ptr<detail::FanZoneExhaust_Impl> impl);

  friend class detail::FanZoneExhaust_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.FanZoneExhaust");
};

/** \relates FanZoneExhaust*/
typedef boost::optional<FanZoneExhaust> OptionalFanZoneExhaust;

/** \relates FanZoneExhaust*/
typedef std::vector<FanZoneExhaust> FanZoneExhaustVector;

} // model
} // openstudio

#endif // MODEL_FANZONEEXHAUST_HPP

