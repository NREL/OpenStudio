/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALSIMPLE_HPP
#define MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALSIMPLE_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject.hpp>

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;

namespace detail {

  class SolarCollectorPerformancePhotovoltaicThermalSimple_Impl;

} // detail

/** SolarCollectorPerformancePhotovoltaicThermalSimple is a ModelObject that wraps the OpenStudio IDD object 'OS:SolarCollectorPerformance:PhotovoltaicThermal:Simple'. */
class MODEL_API SolarCollectorPerformancePhotovoltaicThermalSimple : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SolarCollectorPerformancePhotovoltaicThermalSimple(const Model& model);

  virtual ~SolarCollectorPerformancePhotovoltaicThermalSimple() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> thermalConversionEfficiencyInputModeTypeValues();

  /** @name Getters */
  //@{

  double fractionofSurfaceAreawithActiveThermalCollector() const;

  boost::optional<std::string> thermalConversionEfficiencyInputModeType() const;

  boost::optional<double> valueforThermalConversionEfficiencyifFixed() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> thermalConversionEfficiencySchedule() const;

  double frontSurfaceEmittance() const;

  bool isFrontSurfaceEmittanceDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setFractionofSurfaceAreawithActiveThermalCollector(double fractionofSurfaceAreawithActiveThermalCollector);

  bool setThermalConversionEfficiencyInputModeType(std::string thermalConversionEfficiencyInputModeType);

  void resetThermalConversionEfficiencyInputModeType();

  bool setValueforThermalConversionEfficiencyifFixed(double valueforThermalConversionEfficiencyifFixed);

  void resetValueforThermalConversionEfficiencyifFixed();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setThermalConversionEfficiencySchedule(Schedule& schedule);

  void resetThermalConversionEfficiencySchedule();

  bool setFrontSurfaceEmittance(double frontSurfaceEmittance);

  void resetFrontSurfaceEmittance();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl ImplType;

  explicit SolarCollectorPerformancePhotovoltaicThermalSimple(std::shared_ptr<detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl> impl);

  friend class detail::SolarCollectorPerformancePhotovoltaicThermalSimple_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SolarCollectorPerformancePhotovoltaicThermalSimple");
};

/** \relates SolarCollectorPerformancePhotovoltaicThermalSimple*/
typedef boost::optional<SolarCollectorPerformancePhotovoltaicThermalSimple> OptionalSolarCollectorPerformancePhotovoltaicThermalSimple;

/** \relates SolarCollectorPerformancePhotovoltaicThermalSimple*/
typedef std::vector<SolarCollectorPerformancePhotovoltaicThermalSimple> SolarCollectorPerformancePhotovoltaicThermalSimpleVector;

} // model
} // openstudio

#endif // MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALSIMPLE_HPP

