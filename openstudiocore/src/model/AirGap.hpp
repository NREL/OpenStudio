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

#ifndef MODEL_AIRGAP_HPP
#define MODEL_AIRGAP_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class AirGap_Impl;

} // detail

/** AirGap is a OpaqueMaterial that wraps the OpenStudio IDD object 'OS:Material:AirGap'. */
class MODEL_API AirGap : public OpaqueMaterial {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirGap(const Model& model,
    double thermalResistance = 0.1);

  virtual ~AirGap() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double thermalResistance() const;

  OSOptionalQuantity getThermalResistance(bool returnIP=false) const;

  //@}
  /** @name Setters */
  //@{

  bool setThermalResistance(double thermalResistance);

  bool setThermalResistance(const Quantity& thermalResistance);

  void resetThermalResistance();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirGap_Impl ImplType;

  explicit AirGap(std::shared_ptr<detail::AirGap_Impl> impl);

  friend class detail::AirGap_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirGap");
};

/** \relates AirGap*/
typedef boost::optional<AirGap> OptionalAirGap;

/** \relates AirGap*/
typedef std::vector<AirGap> AirGapVector;

} // model
} // openstudio

#endif // MODEL_AIRGAP_HPP

