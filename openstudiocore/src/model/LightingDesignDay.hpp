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

#ifndef MODEL_LIGHTINGDESIGNDAY_HPP
#define MODEL_LIGHTINGDESIGNDAY_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class DateTime;
class Date;
class Time;

namespace model {

namespace detail {

  class LightingDesignDay_Impl;

} // detail

/** LightingDesignDay is a ModelObject that wraps the OpenStudio IDD object 'OS_LightingDesignDay'. */
class MODEL_API LightingDesignDay : public ModelObject {
 public:

  /** @name Constructors and Destructors */
  //@{

  LightingDesignDay(const std::string& cieSkyModel, 
                    const openstudio::Date& date, 
                    const Model& model);

  virtual ~LightingDesignDay() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  static std::vector<std::string> validCIESkyModelValues();

  //@}
  /** @name Getters */
  //@{

  std::string cieSkyModel() const;

  int snowIndicator() const;

  bool isSnowIndicatorDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setCIESkyModel(std::string cIESkyModel);

  bool setSnowIndicator(int snowIndicator);

  void resetSnowIndicator();

  //@}

  openstudio::Date date() const;

  bool setDate(const openstudio::Date& date);

  std::vector<openstudio::Time> simulationTimes() const;

  std::vector<openstudio::DateTime> simulationDateTimes() const;

  bool addSimulationTime(const openstudio::Time& time);

  void clearSimulationTimes();

  // ensure that this object does not contain the date 2/29
  void ensureNoLeapDays();

 protected:
  /// @cond
  typedef detail::LightingDesignDay_Impl ImplType;

  friend class detail::LightingDesignDay_Impl;
  friend class Model;
  friend class IdfObject;

  explicit LightingDesignDay(std::shared_ptr<detail::LightingDesignDay_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.LightingDesignDay");
};

/** \relates LightingDesignDay*/
typedef boost::optional<LightingDesignDay> OptionalLightingDesignDay;

/** \relates LightingDesignDay*/
typedef std::vector<LightingDesignDay> LightingDesignDayVector;

} // model
} // openstudio

#endif // MODEL_LIGHTINGDESIGNDAY_HPP

