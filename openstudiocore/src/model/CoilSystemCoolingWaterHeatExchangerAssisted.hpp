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

#ifndef MODEL_COILSYSTEMCOOLINGWATERHEATEXCHANGERASSISTED_HPP
#define MODEL_COILSYSTEMCOOLINGWATERHEATEXCHANGERASSISTED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

class AirToAirComponent;
class WaterToAirComponent;

namespace detail {

  class CoilSystemCoolingWaterHeatExchangerAssisted_Impl;

} // detail

/** CoilSystemCoolingWaterHeatExchangerAssisted is a StraightComponent that wraps the OpenStudio IDD object 'OS:CoilSystem:Cooling:Water:HeatExchangerAssisted'. */
class MODEL_API CoilSystemCoolingWaterHeatExchangerAssisted : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilSystemCoolingWaterHeatExchangerAssisted(const Model& model);

  virtual ~CoilSystemCoolingWaterHeatExchangerAssisted() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  AirToAirComponent heatExchanger() const;

  WaterToAirComponent coolingCoil() const;

  //@}
  /** @name Setters */
  //@{

  bool setHeatExchanger(const AirToAirComponent& heatExchanger);

  bool setCoolingCoil(const WaterToAirComponent& coolingCoil);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl ImplType;

  explicit CoilSystemCoolingWaterHeatExchangerAssisted(std::shared_ptr<detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl> impl);

  friend class detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilSystemCoolingWaterHeatExchangerAssisted");
};

/** \relates CoilSystemCoolingWaterHeatExchangerAssisted*/
typedef boost::optional<CoilSystemCoolingWaterHeatExchangerAssisted> OptionalCoilSystemCoolingWaterHeatExchangerAssisted;

/** \relates CoilSystemCoolingWaterHeatExchangerAssisted*/
typedef std::vector<CoilSystemCoolingWaterHeatExchangerAssisted> CoilSystemCoolingWaterHeatExchangerAssistedVector;

} // model
} // openstudio

#endif // MODEL_COILSYSTEMCOOLINGWATERHEATEXCHANGERASSISTED_HPP

