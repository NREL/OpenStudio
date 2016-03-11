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

#ifndef MODEL_PIPEINDOOR_HPP
#define MODEL_PIPEINDOOR_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Construction;
class Schedule;
class ThermalZone;

namespace detail {

  class PipeIndoor_Impl;

} // detail

/** PipeIndoor is a  StraightComponent that wraps the OpenStudio IDD object 'OS:Pipe:Indoor'. */
class MODEL_API PipeIndoor : public  StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PipeIndoor(const Model& model);

  virtual ~PipeIndoor() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> environmentTypeValues();

  /** @name Getters */
  //@{

  boost::optional<Construction> construction() const;

  std::string environmentType() const;

  boost::optional<ThermalZone> ambientTemperatureZone() const;

  boost::optional<Schedule> ambientTemperatureSchedule() const;

  boost::optional<Schedule> ambientAirVelocitySchedule() const;

  double pipeInsideDiameter() const;

  double pipeLength() const;

  //@}
  /** @name Setters */
  //@{

  bool setConstruction(const Construction& construction);

  void resetConstruction();

  bool setEnvironmentType(std::string environmentType);

  bool setAmbientTemperatureZone(const ThermalZone& thermalZone);

  void resetAmbientTemperatureZone();

  bool setAmbientTemperatureSchedule(Schedule& schedule);

  void resetAmbientTemperatureSchedule();

  bool setAmbientAirVelocitySchedule(Schedule& schedule);

  void resetAmbientAirVelocitySchedule();

  bool setPipeInsideDiameter(double pipeInsideDiameter);

  bool setPipeLength(double pipeLength);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::PipeIndoor_Impl ImplType;

  explicit PipeIndoor(std::shared_ptr<detail::PipeIndoor_Impl> impl);

  friend class detail::PipeIndoor_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PipeIndoor");
};

/** \relates PipeIndoor*/
typedef boost::optional<PipeIndoor> OptionalPipeIndoor;

/** \relates PipeIndoor*/
typedef std::vector<PipeIndoor> PipeIndoorVector;

} // model
} // openstudio

#endif // MODEL_PIPEINDOOR_HPP

