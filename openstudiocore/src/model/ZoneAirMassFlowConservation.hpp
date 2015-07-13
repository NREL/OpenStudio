/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_ZONEAIRMASSFLOWCONSERVATION_HPP
#define MODEL_ZONEAIRMASSFLOWCONSERVATION_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject.hpp>

namespace openstudio {
namespace model {

namespace detail {

  class ZoneAirMassFlowConservation_Impl;

} // detail

/** ZoneAirMassFlowConservation is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneAirMassFlowConservation'. */
class MODEL_API ZoneAirMassFlowConservation : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ZoneAirMassFlowConservation() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> sourceZoneInfiltrationTreatmentValues();

  /** @name Getters */
  //@{

  bool adjustZoneMixingForZoneAirMassFlowBalance() const;

  bool isAdjustZoneMixingForZoneAirMassFlowBalanceDefaulted() const;

  std::string sourceZoneInfiltrationTreatment() const;

  bool isSourceZoneInfiltrationTreatmentDefaulted() const;

  //@}
  /** @name Setters */
  //@{

   /// If this input field is true, then EnergyPlus attempts to enforce the zone air mass flow conservation.
   /// If it is specified as false, then EnergyPlus calculation defaults to zone air mass flow balance that 
   /// does not include zone mixing objects and that assumes self - balanced simple flow objects per the 
   /// default procedure, which may not necessarily enforce zone mass conservation unless the user specified
   /// a balanced flow to begin with. Mass conservation is enforced both for the receiving
   /// and source zones if there is at least one mixing object defined. 
   /// The default value is false. 
   /// (Note that false input may also results in balanced flow depending on the system specified).
  void setAdjustZoneMixingForZoneAirMassFlowBalance(bool adjustZoneMixingForZoneAirMassFlowBalance);

  void resetAdjustZoneMixingForZoneAirMassFlowBalance();

  /// If this input is "AddInfiltrationFlow", then energy plus adds infiltration air
  /// mass flow on top of the base flow, which is calculated using the infiltration object
  /// user inputs, in order to balance the zone air mass flow. The additional infiltration
  /// air mass flow is not self - balanced. If this input is specified as
  /// "AdjustInfiltrationFlow", then energy plus may adjust the base flow calculated using
  /// the infiltration object user inputs in order to balance the zone air mass flow. If it
  /// is not required to adjust the base infiltration air flow calculated from the user
  /// specified infiltration object inputs, then the base infiltration air mass flow
  /// is assumed self - balanced. If adjustZoneMixingForZoneAirMassFlowBalance is false then this input
  /// field has no impact on the simulation.
  /// The default value is "AddInfiltrationFlow".
  bool setSourceZoneInfiltrationTreatment(const std::string& sourceZoneInfiltrationTreatment);

  void resetSourceZoneInfiltrationTreatment();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneAirMassFlowConservation_Impl ImplType;

  explicit ZoneAirMassFlowConservation(std::shared_ptr<detail::ZoneAirMassFlowConservation_Impl> impl);

  friend class detail::ZoneAirMassFlowConservation_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  explicit ZoneAirMassFlowConservation(Model& model);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneAirMassFlowConservation");
};

/** \relates ZoneAirMassFlowConservation*/
typedef boost::optional<ZoneAirMassFlowConservation> OptionalZoneAirMassFlowConservation;

/** \relates ZoneAirMassFlowConservation*/
typedef std::vector<ZoneAirMassFlowConservation> ZoneAirMassFlowConservationVector;

} // model
} // openstudio

#endif // MODEL_ZONEAIRMASSFLOWCONSERVATION_HPP

