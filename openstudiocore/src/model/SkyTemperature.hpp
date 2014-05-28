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

#ifndef MODEL_SKYTEMPERATURE_HPP
#define MODEL_SKYTEMPERATURE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {
  class SkyTemperature_Impl;
} // detail

class MODEL_API SkyTemperature : public ModelObject {
 public:

  // constructor
  explicit SkyTemperature(const Model& model);

  virtual ~SkyTemperature() {}

  static IddObjectType iddObjectType();

 protected:

  typedef detail::SkyTemperature_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  // constructor
  explicit SkyTemperature(std::shared_ptr<detail::SkyTemperature_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.SkyTemperature");

};

/** \relates SkyTemperature */
typedef boost::optional<SkyTemperature> OptionalSkyTemperature;

/** \relates SkyTemperature */
typedef std::vector<SkyTemperature> SkyTemperatureVector;

} // model
} // openstudio

#endif // MODEL_SKYTEMPERATURE_HPP
