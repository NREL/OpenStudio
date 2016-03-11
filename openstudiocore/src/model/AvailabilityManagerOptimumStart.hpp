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

#ifndef MODEL_AVAILABILITYMANAGEROPTIMUMSTART_HPP
#define MODEL_AVAILABILITYMANAGEROPTIMUMSTART_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {

namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  class AvailabilityManagerOptimumStart_Impl;

} // detail

/** AvailabilityManagerOptimumStart is a AvailabilityManager that wraps the OpenStudio IDD object 'OS:AvailabilityManager:OptimumStart'. */
class MODEL_API AvailabilityManagerOptimumStart : public AvailabilityManager {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AvailabilityManagerOptimumStart(const Model& model);

  virtual ~AvailabilityManagerOptimumStart() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlTypeValues();

  static std::vector<std::string> controlAlgorithmValues();

  /** @name Getters */
  //@{

  Schedule applicabilitySchedule() const;

  std::string controlType() const;

  boost::optional<ThermalZone> controlZone() const;

  double maximumValueforOptimumStartTime() const;

  std::string controlAlgorithm() const;

  double constantTemperatureGradientduringCooling() const;

  double constantTemperatureGradientduringHeating() const;

  double initialTemperatureGradientduringCooling() const;

  double initialTemperatureGradientduringHeating() const;

  double constantStartTime() const;

  int numberofPreviousDays() const;

  //@}
  /** @name Setters */
  //@{

  bool setApplicabilitySchedule(Schedule& schedule);

  bool setControlType(std::string controlType);

  bool setControlZone(const ThermalZone& thermalZone);

  void resetControlZone();

  void setMaximumValueforOptimumStartTime(double maximumValueforOptimumStartTime);

  bool setControlAlgorithm(std::string controlAlgorithm);

  void setConstantTemperatureGradientduringCooling(double constantTemperatureGradientduringCooling);

  void setConstantTemperatureGradientduringHeating(double constantTemperatureGradientduringHeating);

  void setInitialTemperatureGradientduringCooling(double initialTemperatureGradientduringCooling);

  void setInitialTemperatureGradientduringHeating(double initialTemperatureGradientduringHeating);

  void setConstantStartTime(double constantStartTime);

  bool setNumberofPreviousDays(int numberofPreviousDays);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AvailabilityManagerOptimumStart_Impl ImplType;

  explicit AvailabilityManagerOptimumStart(std::shared_ptr<detail::AvailabilityManagerOptimumStart_Impl> impl);

  friend class detail::AvailabilityManagerOptimumStart_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AvailabilityManagerOptimumStart");
};

/** \relates AvailabilityManagerOptimumStart*/
typedef boost::optional<AvailabilityManagerOptimumStart> OptionalAvailabilityManagerOptimumStart;

/** \relates AvailabilityManagerOptimumStart*/
typedef std::vector<AvailabilityManagerOptimumStart> AvailabilityManagerOptimumStartVector;

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGEROPTIMUMSTART_HPP

