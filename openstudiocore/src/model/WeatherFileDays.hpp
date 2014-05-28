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

#ifndef MODEL_WEATHERFILEDAYS_HPP
#define MODEL_WEATHERFILEDAYS_HPP

#include "ModelAPI.hpp"
#include "SizingPeriod.hpp"

namespace openstudio {
namespace model {

namespace detail {
  class WeatherFileDays_Impl;
} // detail

class MODEL_API WeatherFileDays : public SizingPeriod {
 public:

  // constructor
  explicit WeatherFileDays(const Model& model);

  virtual ~WeatherFileDays() {}

  static IddObjectType iddObjectType();

 protected:

  typedef detail::WeatherFileDays_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  // constructor
  explicit WeatherFileDays(std::shared_ptr<detail::WeatherFileDays_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.WeatherFileDays");

};

/** \relates WeatherFileDays */
typedef boost::optional<WeatherFileDays> OptionalWeatherFileDays;

/** \relates WeatherFileDays */
typedef std::vector<WeatherFileDays> WeatherFileDaysVector;

} // model
} // openstudio

#endif // MODEL_WEATHERFILEDAYS_HPP
