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

#ifndef MODEL_SIMPLEGLAZING_HPP
#define MODEL_SIMPLEGLAZING_HPP

#include "ModelAPI.hpp"
#include "Glazing.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class SimpleGlazing_Impl;

} // detail

/** SimpleGlazing is a Glazing that wraps the OpenStudio IDD object 'OS:WindowMaterial:SimpleGlazingSystem'. */
class MODEL_API SimpleGlazing : public Glazing {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SimpleGlazing(const Model& model,
    double uFactor = 0.1,
    double solarHeatGainCoefficient = 0.1);

  virtual ~SimpleGlazing() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double uFactor() const;

  Quantity getUFactor(bool returnIP=false) const;

  double solarHeatGainCoefficient() const;

  Quantity getSolarHeatGainCoefficient(bool returnIP=false) const;

  boost::optional<double> visibleTransmittance() const;

  OSOptionalQuantity getVisibleTransmittance(bool returnIP=false) const;

  //@}
  /** @name Setters */
  //@{

  bool setUFactor(double uFactor);

  bool setUFactor(const Quantity& uFactor);

  bool setSolarHeatGainCoefficient(double solarHeatGainCoefficient);

  bool setSolarHeatGainCoefficient(const Quantity& solarHeatGainCoefficient);

  bool setVisibleTransmittance(double visibleTransmittance);

  bool setVisibleTransmittance(const Quantity& visibleTransmittance);

  void resetVisibleTransmittance();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SimpleGlazing_Impl ImplType;

  explicit SimpleGlazing(std::shared_ptr<detail::SimpleGlazing_Impl> impl);

  friend class detail::SimpleGlazing_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SimpleGlazing");
};

/** \relates SimpleGlazing*/
typedef boost::optional<SimpleGlazing> OptionalSimpleGlazing;

/** \relates SimpleGlazing*/
typedef std::vector<SimpleGlazing> SimpleGlazingVector;

} // model
} // openstudio

#endif // MODEL_SIMPLEGLAZING_HPP

