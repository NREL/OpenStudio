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

#ifndef MODEL_PLANTCOMPONENTTEMPERATURESOURCE_HPP
#define MODEL_PLANTCOMPONENTTEMPERATURESOURCE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class PlantComponentTemperatureSource_Impl;

} // detail

/** PlantComponentTemperatureSource is a StraightComponent that wraps the OpenStudio IDD object 'OS:PlantComponent:TemperatureSource'. */
class MODEL_API PlantComponentTemperatureSource : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PlantComponentTemperatureSource(const Model& model);

  virtual ~PlantComponentTemperatureSource() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> temperatureSpecificationTypeValues();

  /** @name Getters */
  //@{

  boost::optional<double> designVolumeFlowRate() const;

  bool isDesignVolumeFlowRateAutosized() const;

  std::string temperatureSpecificationType() const;

  boost::optional<double> sourceTemperature() const;

  boost::optional<Schedule> sourceTemperatureSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setDesignVolumeFlowRate(double designVolumeFlowRate);

  void autosizeDesignVolumeFlowRate();

  bool setTemperatureSpecificationType(std::string temperatureSpecificationType);

  void setSourceTemperature(double sourceTemperature);

  void resetSourceTemperature();

  bool setSourceTemperatureSchedule(Schedule& schedule);

  void resetSourceTemperatureSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::PlantComponentTemperatureSource_Impl ImplType;

  explicit PlantComponentTemperatureSource(std::shared_ptr<detail::PlantComponentTemperatureSource_Impl> impl);

  friend class detail::PlantComponentTemperatureSource_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PlantComponentTemperatureSource");
};

/** \relates PlantComponentTemperatureSource*/
typedef boost::optional<PlantComponentTemperatureSource> OptionalPlantComponentTemperatureSource;

/** \relates PlantComponentTemperatureSource*/
typedef std::vector<PlantComponentTemperatureSource> PlantComponentTemperatureSourceVector;

} // model
} // openstudio

#endif // MODEL_PLANTCOMPONENTTEMPERATURESOURCE_HPP

