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

#ifndef MODEL_AIRTERMINALDUALDUCTVAV_HPP
#define MODEL_AIRTERMINALDUALDUCTVAV_HPP

#include "ModelAPI.hpp"
#include "Mixer.hpp"

namespace openstudio {

namespace model {

class Schedule;
class DesignSpecificationOutdoorAir;
class Node;

namespace detail {

  class AirTerminalDualDuctVAV_Impl;

} // detail

/** AirTerminalDualDuctVAV is a Mixer that wraps the OpenStudio IDD object 'OS:AirTerminal:DualDuct:VAV'. */
class MODEL_API AirTerminalDualDuctVAV : public Mixer {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirTerminalDualDuctVAV(const Model& model);

  virtual ~AirTerminalDualDuctVAV() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  boost::optional<double> maximumDamperAirFlowRate() const;

  bool isMaximumDamperAirFlowRateAutosized() const;

  double zoneMinimumAirFlowFraction() const;

  boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAirObject() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setMaximumDamperAirFlowRate(double maximumDamperAirFlowRate);

  void autosizeMaximumDamperAirFlowRate();

  bool setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction);

  bool setDesignSpecificationOutdoorAirObject(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir);

  void resetDesignSpecificationOutdoorAirObject();

  //@}
  /** @name Other */
  //@{

  /** This corresponds to Mixer::inletModelObject(0), as well as the system's branch 0
    * (ie. AirLoopHVAC::demandInletNodes()[0] and AirLoopHVAC::supplyOutletNodes()[0])
    * In OpenStudio we avoid using hot / cold nomenclature in the dual duct system api,
    * but here in the terminal interface we use the language for transparency with the idf.
    */
  boost::optional<Node> hotAirInletNode() const;

  /** This corresponds to Mixer::inletModelObject(1), as well as the system's branch 1
    * (ie. AirLoopHVAC::demandInletNodes()[1] and AirLoopHVAC::supplyOutletNodes()[1])
    */
  boost::optional<Node> coldAirInletNode() const;

  //@}
 protected:
  /// @cond
  typedef detail::AirTerminalDualDuctVAV_Impl ImplType;

  explicit AirTerminalDualDuctVAV(std::shared_ptr<detail::AirTerminalDualDuctVAV_Impl> impl);

  friend class detail::AirTerminalDualDuctVAV_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirTerminalDualDuctVAV");
};

/** \relates AirTerminalDualDuctVAV*/
typedef boost::optional<AirTerminalDualDuctVAV> OptionalAirTerminalDualDuctVAV;

/** \relates AirTerminalDualDuctVAV*/
typedef std::vector<AirTerminalDualDuctVAV> AirTerminalDualDuctVAVVector;

} // model
} // openstudio

#endif // MODEL_AIRTERMINALDUALDUCTVAV_HPP

