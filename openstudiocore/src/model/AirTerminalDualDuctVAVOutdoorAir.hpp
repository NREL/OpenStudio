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

#ifndef MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_HPP
#define MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_HPP

#include "ModelAPI.hpp"
#include "Mixer.hpp"

namespace openstudio {

namespace model {

class Schedule;
class AirLoopHVAC;
class Model;
class Node;
class DesignSpecificationOutdoorAir;

namespace detail {

  class AirTerminalDualDuctVAVOutdoorAir_Impl;

} // detail

/** AirTerminalDualDuctVAVOutdoorAir is a Mixer that wraps the OpenStudio IDD object 'OS:AirTerminal:DualDuct:VAV:OutdoorAir'. */
class MODEL_API AirTerminalDualDuctVAVOutdoorAir : public Mixer {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirTerminalDualDuctVAVOutdoorAir(const Model& model);

  virtual ~AirTerminalDualDuctVAVOutdoorAir() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> perPersonVentilationRateModeValues();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  boost::optional<double> maximumTerminalAirFlowRate() const;

  bool isMaximumTerminalAirFlowRateAutosized() const;

  std::string perPersonVentilationRateMode() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setMaximumTerminalAirFlowRate(double maximumTerminalAirFlowRate);

  void autosizeMaximumTerminalAirFlowRate();

  /** If true, OpenStudio will attach the DesignSpecificationOutdoorAir object associated
    * with the terminal's zone on export to EnergyPlus idf format.
    * This field replaces the functionality of the EnergyPlus field: Design Specification Outdoor Air Object Name.*/
  bool controlForOutdoorAir() const;
  bool setControlForOutdoorAir(bool controlForOutdoorAir);

  bool setPerPersonVentilationRateMode(const std::string& perPersonVentilationRateMode);

  /** This corresponds to Mixer::inletModelObject(0), as well as the system's branch 0
    * (ie. AirLoopHVAC::demandInletNodes()[0] and AirLoopHVAC::supplyOutletNodes()[0])
    * In OpenStudio we avoid using hot / cold nomenclature in the dual duct system api,
    * but here in the terminal interface we use the language for transparency with the idf.
    */
  boost::optional<Node> outdoorAirInletNode() const;

  /** This corresponds to Mixer::inletModelObject(1), as well as the system's branch 1
    * (ie. AirLoopHVAC::demandInletNodes()[1] and AirLoopHVAC::supplyOutletNodes()[1])
    */
  boost::optional<Node> recirculatedAirInletNode() const;


  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirTerminalDualDuctVAVOutdoorAir_Impl ImplType;

  explicit AirTerminalDualDuctVAVOutdoorAir(std::shared_ptr<detail::AirTerminalDualDuctVAVOutdoorAir_Impl> impl);

  friend class detail::AirTerminalDualDuctVAVOutdoorAir_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirTerminalDualDuctVAVOutdoorAir");
};

/** \relates AirTerminalDualDuctVAVOutdoorAir*/
typedef boost::optional<AirTerminalDualDuctVAVOutdoorAir> OptionalAirTerminalDualDuctVAVOutdoorAir;

/** \relates AirTerminalDualDuctVAVOutdoorAir*/
typedef std::vector<AirTerminalDualDuctVAVOutdoorAir> AirTerminalDualDuctVAVOutdoorAirVector;

} // model
} // openstudio

#endif // MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_HPP

