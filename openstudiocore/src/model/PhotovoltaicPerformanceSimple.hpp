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

#ifndef MODEL_PHOTOVOLTAICPERFORMANCESIMPLE_HPP
#define MODEL_PHOTOVOLTAICPERFORMANCESIMPLE_HPP

#include <model/ModelAPI.hpp>
#include "PhotovoltaicPerformance.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;

namespace detail {

  class PhotovoltaicPerformanceSimple_Impl;

} // detail

/** PhotovoltaicPerformanceSimple is a PhotovoltaicPerformance that wraps the OpenStudio IDD object 'OS:PhotovoltaicPerformance:Simple'. */
class MODEL_API PhotovoltaicPerformanceSimple : public PhotovoltaicPerformance {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PhotovoltaicPerformanceSimple(const Model& model);

  virtual ~PhotovoltaicPerformanceSimple() {}

  //@}

  static IddObjectType iddObjectType();

  // DLM: remove
  static std::vector<std::string> conversionEfficiencyInputModeValues();

  /** @name Getters */
  //@{

  // DLM, rename fractionOfSurfaceAreaWithActiveSolarCells
  double fractionofSurfaceAreawithActiveSolarCells() const;

  bool isFractionofSurfaceAreawithActiveSolarCellsDefaulted() const;

  boost::optional<std::string> conversionEfficiencyInputMode() const;

  // DLM: rename, fixedEfficiency
  boost::optional<double> valueforCellEfficiencyifFixed() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> efficiencySchedule() const;

  //@}
  /** @name Setters */
  //@{

  // DLM: rename setFractionOfSurfaceAreaWithActiveSolarCells
  bool setFractionofSurfaceAreawithActiveSolarCells(double fractionofSurfaceAreawithActiveSolarCells);

  // DLM: rename resetFractionOfSurfaceAreaWithActiveSolarCells
  void resetFractionofSurfaceAreawithActiveSolarCells();

  // DLM: I like to remove the setter from the public API, then we just set it when user sets fixed efficiency or scheduled efficiency
  bool setConversionEfficiencyInputMode(std::string conversionEfficiencyInputMode);

  // DLM: remove this too
  void resetConversionEfficiencyInputMode();

  // DLM: I'd rename this, setFixedEfficiency
  bool setValueforCellEfficiencyifFixed(double valueforCellEfficiencyifFixed);

  void resetValueforCellEfficiencyifFixed();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setEfficiencySchedule(Schedule& schedule);

  void resetEfficiencySchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::PhotovoltaicPerformanceSimple_Impl ImplType;

  explicit PhotovoltaicPerformanceSimple(std::shared_ptr<detail::PhotovoltaicPerformanceSimple_Impl> impl);

  friend class detail::PhotovoltaicPerformanceSimple_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PhotovoltaicPerformanceSimple");
};

/** \relates PhotovoltaicPerformanceSimple*/
typedef boost::optional<PhotovoltaicPerformanceSimple> OptionalPhotovoltaicPerformanceSimple;

/** \relates PhotovoltaicPerformanceSimple*/
typedef std::vector<PhotovoltaicPerformanceSimple> PhotovoltaicPerformanceSimpleVector;

} // model
} // openstudio

#endif // MODEL_PHOTOVOLTAICPERFORMANCESIMPLE_HPP

