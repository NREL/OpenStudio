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

#ifndef MODEL_WEATHERFILECONDITIONTYPE_HPP
#define MODEL_WEATHERFILECONDITIONTYPE_HPP

#include "ModelAPI.hpp"
#include "SizingPeriod.hpp"

namespace openstudio {
namespace model {

namespace detail {
  class WeatherFileConditionType_Impl;
} // detail

class MODEL_API WeatherFileConditionType : public SizingPeriod {
 public:

  // constructor
  explicit WeatherFileConditionType(const Model& model);

  virtual ~WeatherFileConditionType() {}

  static IddObjectType iddObjectType();

 protected:

  typedef detail::WeatherFileConditionType_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  // constructor
  explicit WeatherFileConditionType(std::shared_ptr<detail::WeatherFileConditionType_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.WeatherFileConditionType");

};

/** \relates WeatherFileConditionType */
typedef boost::optional<WeatherFileConditionType> OptionalWeatherFileConditionType;

/** \relates WeatherFileConditionType */
typedef std::vector<WeatherFileConditionType> WeatherFileConditionTypeVector;

} // model
} // openstudio

#endif // MODEL_WEATHERFILECONDITIONTYPE_HPP
