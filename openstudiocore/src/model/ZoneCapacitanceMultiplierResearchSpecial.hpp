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

#ifndef MODEL_ZONECAPACITANCEMULTIPLIERRESEARCHSPECIAL_HPP
#define MODEL_ZONECAPACITANCEMULTIPLIERRESEARCHSPECIAL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class ZoneCapacitanceMultiplierResearchSpecial_Impl;

} // detail

/** ZoneCapacitanceMultiplierResearchSpecial is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneCapacitanceMultiplier:ResearchSpecial'. */
class MODEL_API ZoneCapacitanceMultiplierResearchSpecial : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ZoneCapacitanceMultiplierResearchSpecial() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double temperatureCapacityMultiplier() const;

  Quantity getTemperatureCapacityMultiplier(bool returnIP=false) const;

  bool isTemperatureCapacityMultiplierDefaulted() const;

  double humidityCapacityMultiplier() const;

  Quantity getHumidityCapacityMultiplier(bool returnIP=false) const;

  bool isHumidityCapacityMultiplierDefaulted() const;

  double carbonDioxideCapacityMultiplier() const;

  Quantity getCarbonDioxideCapacityMultiplier(bool returnIP=false) const;

  bool isCarbonDioxideCapacityMultiplierDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setTemperatureCapacityMultiplier(double temperatureCapacityMultiplier);

  bool setTemperatureCapacityMultiplier(const Quantity& temperatureCapacityMultiplier);

  void resetTemperatureCapacityMultiplier();

  bool setHumidityCapacityMultiplier(double humidityCapacityMultiplier);

  bool setHumidityCapacityMultiplier(const Quantity& humidityCapacityMultiplier);

  void resetHumidityCapacityMultiplier();

  bool setCarbonDioxideCapacityMultiplier(double carbonDioxideCapacityMultiplier);

  bool setCarbonDioxideCapacityMultiplier(const Quantity& carbonDioxideCapacityMultiplier);

  void resetCarbonDioxideCapacityMultiplier();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneCapacitanceMultiplierResearchSpecial_Impl ImplType;

  explicit ZoneCapacitanceMultiplierResearchSpecial(std::shared_ptr<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl> impl);

  friend class detail::ZoneCapacitanceMultiplierResearchSpecial_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  explicit ZoneCapacitanceMultiplierResearchSpecial(Model& model);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneCapacitanceMultiplierResearchSpecial");
};

/** \relates ZoneCapacitanceMultiplierResearchSpecial*/
typedef boost::optional<ZoneCapacitanceMultiplierResearchSpecial> OptionalZoneCapacitanceMultiplierResearchSpecial;

/** \relates ZoneCapacitanceMultiplierResearchSpecial*/
typedef std::vector<ZoneCapacitanceMultiplierResearchSpecial> ZoneCapacitanceMultiplierResearchSpecialVector;

} // model
} // openstudio

#endif // MODEL_ZONECAPACITANCEMULTIPLIERRESEARCHSPECIAL_HPP

