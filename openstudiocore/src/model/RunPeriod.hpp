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

#ifndef MODEL_RUNPERIOD_HPP
#define MODEL_RUNPERIOD_HPP

#include <model/ModelAPI.hpp>
#include <model/ParentObject.hpp>

namespace openstudio {
namespace model {

namespace detail {
  class RunPeriod_Impl;
} // detail


/** RunPeriod derives from ParentObject and is an interface to the unique OpenStudio IDD object named "RunPeriod".
 *
 *  RunPeriod defines a period of time over which to run an EnergyPlus weather file simulation.  Some applications 
 *  require a full annual EnergyPlus simulation, others do not.
 */
class MODEL_API RunPeriod : public ParentObject {
 public:

  /** @name Constructors and Destructors */
  //@{

  virtual ~RunPeriod() {}

  //@}
  /** @name Getters */
  //@{

  // DLM@20110607: these should all have 'get' removed
  // DLM@20110614: I would prefer to not abbreviate names like 'NumTimePeriodRepeats'

  int getBeginMonth() const;
  int getBeginDayOfMonth() const;
  int getEndMonth() const;
  int getEndDayOfMonth() const;
  bool getUseWeatherFileHolidays() const;
  bool getUseWeatherFileDaylightSavings() const;
  bool getApplyWeekendHolidayRule() const;
  bool getUseWeatherFileRainInd() const;
  bool getUseWeatherFileSnowInd() const;
  int getNumTimePeriodRepeats() const;

  //@}
  /** @name Setters */
  //@{

  // DLM@20110607: should all these sets return bool?

  void setBeginMonth(int month);
  void setBeginDayOfMonth(int day);
  void setEndMonth(int month);
  void setEndDayOfMonth(int day);
  void setUseWeatherFileHolidays(bool use);
  void setUseWeatherFileDaylightSavings(bool use);
  void setApplyWeekendHolidayRule(bool apply);
  void setUseWeatherFileRainInd(bool rainInd);
  void setUseWeatherFileSnowInd(bool snowInd);
  void setNumTimePeriodRepeats(int numRepeats);

  //@}

  /// Returns the IddObjectType.
  static IddObjectType iddObjectType();

  /// Returns true if RunPeriod specifies run between 1/1-12/31.
  bool isAnnual() const;

  /// Returns true if not annual simulation.
  bool isPartialYear() const;

  /// Returns true if numTimePeriodRepeats is greater than 1.
  bool isRepeated() const;

 protected:

  /// @cond

  typedef detail::RunPeriod_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  // constructor
  explicit RunPeriod(boost::shared_ptr<detail::RunPeriod_Impl> impl);

  /** Constructor adds a new RunPeriod object to model. */
  explicit RunPeriod(const Model& model);

 private:

  REGISTER_LOGGER("openstudio.model.RunPeriod");

  /// @endcond

};

/** \relates RunPeriod */
typedef boost::optional<RunPeriod> OptionalRunPeriod;

/** \relates RunPeriod */
typedef std::vector<RunPeriod> RunPeriodVector;

} // model
} // openstudio

#endif // MODEL_RUNPERIOD_HPP
