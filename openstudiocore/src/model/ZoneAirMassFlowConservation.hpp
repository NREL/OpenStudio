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

#ifndef MODEL_ZONEAIRMASSFLOWCONSERVATION_HPP
#define MODEL_ZONEAIRMASSFLOWCONSERVATION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

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

  /** This method is deprecated as of OpenStudio 1.9.3. It will return an empty vector.**/
  static std::vector<std::string> sourceZoneInfiltrationTreatmentValues();

  static std::vector<std::string> infiltrationBalancingMethodValues();

  static std::vector<std::string> infiltrationBalancingZonesValues();

  /** @name Getters */
  //@{

  bool adjustZoneMixingForZoneAirMassFlowBalance() const;

  bool isAdjustZoneMixingForZoneAirMassFlowBalanceDefaulted() const;

  /** This method is deprecated as of OpenStudio 1.9.3. It will return an empty string.**/
  std::string sourceZoneInfiltrationTreatment() const;

  /** This method is deprecated as of OpenStudio 1.9.3. It will return false.**/
  bool isSourceZoneInfiltrationTreatmentDefaulted() const;

  std::string infiltrationBalancingMethod() const;

  bool isInfiltrationBalancingMethodDefaulted() const;

  std::string infiltrationBalancingZones() const;

  bool isInfiltrationBalancingZonesDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  void setAdjustZoneMixingForZoneAirMassFlowBalance(bool adjustZoneMixingForZoneAirMassFlowBalance);

  void resetAdjustZoneMixingForZoneAirMassFlowBalance();

  /** This method is deprecated as of OpenStudio 1.9.3.  It will return false.**/
  bool setSourceZoneInfiltrationTreatment(const std::string& sourceZoneInfiltrationTreatment);

  /** This method is deprecated as of OpenStudio 1.9.3**/
  void resetSourceZoneInfiltrationTreatment();

  bool setInfiltrationBalancingMethod(const std::string& infiltrationBalancingMethod);
  
  void resetInfiltrationBalancingMethod();

  bool setInfiltrationBalancingZones(const std::string& infiltrationBalancingZones);

  void resetInfiltrationBalancingZones();

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

