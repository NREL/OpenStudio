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

#ifndef MODEL_PHOTOVOLTAICPERFORMANCESIMPLE_HPP
#define MODEL_PHOTOVOLTAICPERFORMANCESIMPLE_HPP

#include "ModelAPI.hpp"
#include "PhotovoltaicPerformance.hpp"

namespace openstudio {

namespace model {

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

  /** @name Getters */
  //@{

  double fractionOfSurfaceAreaWithActiveSolarCells() const;

  bool isfractionOfSurfaceAreaWithActiveSolarCellsDefaulted() const;

  std::string conversionEfficiencyInputMode() const;

  boost::optional<double> fixedEfficiency() const;

  boost::optional<Schedule> efficiencySchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setFractionOfSurfaceAreaWithActiveSolarCells(double fractionOfSurfaceAreaWithActiveSolarCells);

  void resetFractionOfSurfaceAreaWithActiveSolarCells();

  bool setFixedEfficiency(double fixedEfficiency);

  void resetFixedEfficiency();

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

