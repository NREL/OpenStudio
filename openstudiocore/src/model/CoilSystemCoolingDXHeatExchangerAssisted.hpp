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

#ifndef MODEL_COILSYSTEMCOOLINGDXHEATEXCHANGERASSISTED_HPP
#define MODEL_COILSYSTEMCOOLINGDXHEATEXCHANGERASSISTED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

class AirToAirComponent;

namespace detail {

  class CoilSystemCoolingDXHeatExchangerAssisted_Impl;

} // detail

/** CoilSystemCoolingDXHeatExchangerAssisted is a StraightComponent that wraps the OpenStudio IDD object 'OS:CoilSystem:Cooling:DX:HeatExchangerAssisted'. */
class MODEL_API CoilSystemCoolingDXHeatExchangerAssisted : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilSystemCoolingDXHeatExchangerAssisted(const Model& model);

  virtual ~CoilSystemCoolingDXHeatExchangerAssisted() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  AirToAirComponent heatExchanger() const;

  StraightComponent coolingCoil() const;

  //@}
  /** @name Setters */
  //@{

  bool setHeatExchanger(const AirToAirComponent& heatExchanger);

  bool setCoolingCoil(const StraightComponent& coolingCoil);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilSystemCoolingDXHeatExchangerAssisted_Impl ImplType;

  explicit CoilSystemCoolingDXHeatExchangerAssisted(std::shared_ptr<detail::CoilSystemCoolingDXHeatExchangerAssisted_Impl> impl);

  friend class detail::CoilSystemCoolingDXHeatExchangerAssisted_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilSystemCoolingDXHeatExchangerAssisted");
};

/** \relates CoilSystemCoolingDXHeatExchangerAssisted*/
typedef boost::optional<CoilSystemCoolingDXHeatExchangerAssisted> OptionalCoilSystemCoolingDXHeatExchangerAssisted;

/** \relates CoilSystemCoolingDXHeatExchangerAssisted*/
typedef std::vector<CoilSystemCoolingDXHeatExchangerAssisted> CoilSystemCoolingDXHeatExchangerAssistedVector;

} // model
} // openstudio

#endif // MODEL_COILSYSTEMCOOLINGDXHEATEXCHANGERASSISTED_HPP

