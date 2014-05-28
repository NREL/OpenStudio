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

#ifndef MODEL_THERMOCHROMICGLAZING_HPP
#define MODEL_THERMOCHROMICGLAZING_HPP

#include "ModelAPI.hpp"
#include "Glazing.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class ThermochromicGlazing_Impl;

} // detail

/** ThermochromicGlazing is a Glazing that wraps the OpenStudio IDD object 'OS:WindowMaterial:GlazingGroup:Thermochromic'. */
class MODEL_API ThermochromicGlazing : public Glazing {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ThermochromicGlazing(const Model& model,
    double opticalDataTemperature = 80.0);

  virtual ~ThermochromicGlazing() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double opticalDataTemperature() const;

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Setters */
  //@{

  void setOpticalDataTemperature(double value);

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ThermochromicGlazing_Impl ImplType;

  explicit ThermochromicGlazing(std::shared_ptr<detail::ThermochromicGlazing_Impl> impl);

  friend class detail::ThermochromicGlazing_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ThermochromicGlazing");
};

/** \relates ThermochromicGlazing*/
typedef boost::optional<ThermochromicGlazing> OptionalThermochromicGlazing;

/** \relates ThermochromicGlazing*/
typedef std::vector<ThermochromicGlazing> ThermochromicGlazingVector;

} // model
} // openstudio

#endif // MODEL_THERMOCHROMICGLAZING_HPP

