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

#ifndef MODEL_HUMIDIFIERSTEAMELECTRIC_HPP
#define MODEL_HUMIDIFIERSTEAMELECTRIC_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent.hpp>

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Connection;
class Connection;
class WaterStorageTank;

namespace detail {

  class HumidifierSteamElectric_Impl;

} // detail

/** HumidifierSteamElectric is a StraightComponent that wraps the OpenStudio IDD object 'OS:Humidifier:Steam:Electric'. */
class MODEL_API HumidifierSteamElectric : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit HumidifierSteamElectric(const Model& model);

  virtual ~HumidifierSteamElectric() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> availabilitySchedule() const;

  boost::optional<double> ratedCapacity() const;

  boost::optional<double> ratedPower() const;

  bool isRatedPowerAutosized() const;

  boost::optional<double> ratedFanPower() const;

  boost::optional<double> standbyPower() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> airInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> airOutletNode() const;

  // TODO: Check return type. From object lists, some candidates are: WaterStorageTank.
  boost::optional<WaterStorageTank> waterStorageTank() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setRatedCapacity(double ratedCapacity);

  void resetRatedCapacity();

  bool setRatedPower(double ratedPower);

  void resetRatedPower();

  void autosizeRatedPower();

  bool setRatedFanPower(double ratedFanPower);

  void resetRatedFanPower();

  bool setStandbyPower(double standbyPower);

  void resetStandbyPower();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setAirInletNode(const Connection& connection);

  void resetAirInletNode();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setAirOutletNode(const Connection& connection);

  void resetAirOutletNode();

  // TODO: Check argument type. From object lists, some candidates are: WaterStorageTank.
  bool setWaterStorageTank(const WaterStorageTank& waterStorageTank);

  void resetWaterStorageTank();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::HumidifierSteamElectric_Impl ImplType;

  explicit HumidifierSteamElectric(boost::shared_ptr<detail::HumidifierSteamElectric_Impl> impl);

  friend class detail::HumidifierSteamElectric_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.HumidifierSteamElectric");
};

/** \relates HumidifierSteamElectric*/
typedef boost::optional<HumidifierSteamElectric> OptionalHumidifierSteamElectric;

/** \relates HumidifierSteamElectric*/
typedef std::vector<HumidifierSteamElectric> HumidifierSteamElectricVector;

} // model
} // openstudio

#endif // MODEL_HUMIDIFIERSTEAMELECTRIC_HPP

