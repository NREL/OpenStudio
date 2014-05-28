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

#ifndef MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_HPP
#define MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;

namespace detail {

  class DesignSpecificationZoneAirDistribution_Impl;

} // detail

/** DesignSpecificationZoneAirDistribution is a ModelObject that wraps the OpenStudio IDD object 'OS:DesignSpecification:ZoneAirDistribution'. */
class MODEL_API DesignSpecificationZoneAirDistribution : public ModelObject {
 public:

  explicit DesignSpecificationZoneAirDistribution(const Model& model);

  virtual ~DesignSpecificationZoneAirDistribution() {}

  static IddObjectType iddObjectType();


  double zoneAirDistributionEffectivenessinCoolingMode() const;

  Quantity getZoneAirDistributionEffectivenessinCoolingMode(bool returnIP=false) const;

  bool isZoneAirDistributionEffectivenessinCoolingModeDefaulted() const;

  bool setZoneAirDistributionEffectivenessinCoolingMode(double zoneAirDistributionEffectivenessinCoolingMode);

  bool setZoneAirDistributionEffectivenessinCoolingMode(const Quantity& zoneAirDistributionEffectivenessinCoolingMode);

  void resetZoneAirDistributionEffectivenessinCoolingMode();


  double zoneAirDistributionEffectivenessinHeatingMode() const;

  Quantity getZoneAirDistributionEffectivenessinHeatingMode(bool returnIP=false) const;

  bool isZoneAirDistributionEffectivenessinHeatingModeDefaulted() const;

  bool setZoneAirDistributionEffectivenessinHeatingMode(double zoneAirDistributionEffectivenessinHeatingMode);

  bool setZoneAirDistributionEffectivenessinHeatingMode(const Quantity& zoneAirDistributionEffectivenessinHeatingMode);

  void resetZoneAirDistributionEffectivenessinHeatingMode();


  boost::optional<Schedule> zoneAirDistributionEffectivenessSchedule() const;

  bool setZoneAirDistributionEffectivenessSchedule(Schedule& schedule);

  void resetZoneAirDistributionEffectivenessSchedule();


  double zoneSecondaryRecirculationFraction() const;

  Quantity getZoneSecondaryRecirculationFraction(bool returnIP=false) const;

  bool isZoneSecondaryRecirculationFractionDefaulted() const;

  bool setZoneSecondaryRecirculationFraction(double zoneSecondaryRecirculationFraction);

  bool setZoneSecondaryRecirculationFraction(const Quantity& zoneSecondaryRecirculationFraction);

  void resetZoneSecondaryRecirculationFraction();

 protected:
  /// @cond
  typedef detail::DesignSpecificationZoneAirDistribution_Impl ImplType;

  explicit DesignSpecificationZoneAirDistribution(std::shared_ptr<detail::DesignSpecificationZoneAirDistribution_Impl> impl);

  friend class detail::DesignSpecificationZoneAirDistribution_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.DesignSpecificationZoneAirDistribution");
};

/** \relates DesignSpecificationZoneAirDistribution*/
typedef boost::optional<DesignSpecificationZoneAirDistribution> OptionalDesignSpecificationZoneAirDistribution;

/** \relates DesignSpecificationZoneAirDistribution*/
typedef std::vector<DesignSpecificationZoneAirDistribution> DesignSpecificationZoneAirDistributionVector;

} // model
} // openstudio

#endif // MODEL_DESIGNSPECIFICATIONZONEAIRDISTRIBUTION_HPP

