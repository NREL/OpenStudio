/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_SPACEINFILTRATIONDESIGNFLOWRATE_HPP
#define MODEL_SPACEINFILTRATIONDESIGNFLOWRATE_HPP

#include <model/ModelAPI.hpp>
#include <model/SpaceLoad.hpp>

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  class SpaceInfiltrationDesignFlowRate_Impl;

} // detail

/** SpaceInfiltrationDesignFlowRate is a SpaceLoad that wraps the OpenStudio IDD
 *  object 'OS:SpaceInfiltration:DesignFlowRate'. */
class MODEL_API SpaceInfiltrationDesignFlowRate : public SpaceLoad {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SpaceInfiltrationDesignFlowRate(const Model& model);

  virtual ~SpaceInfiltrationDesignFlowRate() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  static std::vector<std::string> validDesignFlowRateCalculationMethodValues();

  /** \deprecated */
  static std::vector<std::string> designFlowRateCalculationMethodValues();

  //@}
  /** @name Getters */
  //@{

  /** Returns the (fractional) infiltration schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
  boost::optional<Schedule> schedule() const;

  /** Returns true if this object does not specify a schedule directly. */
  bool isScheduleDefaulted() const;

  std::string designFlowRateCalculationMethod() const;

  boost::optional<double> designFlowRate() const;

  boost::optional<double> flowperSpaceFloorArea() const;

  boost::optional<double> flowperExteriorSurfaceArea() const;

  boost::optional<double> flowperExteriorWallArea() const;

  boost::optional<double> airChangesperHour() const;

  double constantTermCoefficient() const;

  bool isConstantTermCoefficientDefaulted() const;

  double temperatureTermCoefficient() const;

  bool isTemperatureTermCoefficientDefaulted() const;

  double velocityTermCoefficient() const;

  bool isVelocityTermCoefficientDefaulted() const;

  double velocitySquaredTermCoefficient() const;

  bool isVelocitySquaredTermCoefficientDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  /** Sets the (fractional) Schedule. */
  bool setSchedule(Schedule& schedule);

  /** Resets the (fractional) Schedule. */
  void resetSchedule();

  bool setDesignFlowRate(double designFlowRate);

  bool setFlowperSpaceFloorArea(double flowperSpaceFloorArea);

  bool setFlowperExteriorSurfaceArea(double flowperExteriorSurfaceArea);

  bool setFlowperExteriorWallArea(double flowperExteriorWallArea);

  bool setAirChangesperHour(double airChangesperHour);

  bool setConstantTermCoefficient(double constantTermCoefficient);

  void resetConstantTermCoefficient();

  bool setTemperatureTermCoefficient(double temperatureTermCoefficient);

  void resetTemperatureTermCoefficient();

  bool setVelocityTermCoefficient(double velocityTermCoefficient);

  void resetVelocityTermCoefficient();

  bool setVelocitySquaredTermCoefficient(double velocitySquaredTermCoefficient);

  void resetVelocitySquaredTermCoefficient();

  //@}
 protected:
  /// @cond
  typedef detail::SpaceInfiltrationDesignFlowRate_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit SpaceInfiltrationDesignFlowRate(boost::shared_ptr<detail::SpaceInfiltrationDesignFlowRate_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.SpaceInfiltrationDesignFlowRate");
};

/** \relates SpaceInfiltrationDesignFlowRate*/
typedef boost::optional<SpaceInfiltrationDesignFlowRate> OptionalSpaceInfiltrationDesignFlowRate;

/** \relates SpaceInfiltrationDesignFlowRate*/
typedef std::vector<SpaceInfiltrationDesignFlowRate> SpaceInfiltrationDesignFlowRateVector;

} // model
} // openstudio

#endif // MODEL_SPACEINFILTRATIONDESIGNFLOWRATE_HPP

