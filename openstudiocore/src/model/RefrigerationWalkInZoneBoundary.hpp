/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONWALKINZONEBOUNDARY_HPP
#define MODEL_REFRIGERATIONWALKINZONEBOUNDARY_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

class ThermalZone;
class Schedule;

namespace detail {

  class RefrigerationWalkInZoneBoundary_Impl;

} // detail

/** RefrigerationWalkInZoneBoundary is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:WalkIn:ZoneBoundary'. */
class MODEL_API RefrigerationWalkInZoneBoundary : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefrigerationWalkInZoneBoundary(const Model& model);

  virtual ~RefrigerationWalkInZoneBoundary() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> stockingDoorOpeningProtectionTypeFacingZoneValues();

  /** @name Getters */
  //@{

  boost::optional<ThermalZone> thermalZone() const;

  double totalInsulatedSurfaceAreaFacingZone() const;

  double insulatedSurfaceUValueFacingZone() const;

  bool isInsulatedSurfaceUValueFacingZoneDefaulted() const;

  double areaofGlassReachInDoorsFacingZone() const;

  bool isAreaofGlassReachInDoorsFacingZoneDefaulted() const;

  double heightofGlassReachInDoorsFacingZone() const;

  bool isHeightofGlassReachInDoorsFacingZoneDefaulted() const;

  double glassReachInDoorUValueFacingZone() const;

  bool isGlassReachInDoorUValueFacingZoneDefaulted() const;

  boost::optional<Schedule> glassReachInDoorOpeningScheduleFacingZone() const;

  double areaofStockingDoorsFacingZone() const;

  bool isAreaofStockingDoorsFacingZoneDefaulted() const;

  double heightofStockingDoorsFacingZone() const;

  bool isHeightofStockingDoorsFacingZoneDefaulted() const;

  double stockingDoorUValueFacingZone() const;

  bool isStockingDoorUValueFacingZoneDefaulted() const;

  boost::optional<Schedule> stockingDoorOpeningScheduleFacingZone() const;

  std::string stockingDoorOpeningProtectionTypeFacingZone() const;

  bool isStockingDoorOpeningProtectionTypeFacingZoneDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setThermalZone(const ThermalZone& thermalZone);

  void resetThermalZone();

  bool setTotalInsulatedSurfaceAreaFacingZone(double totalInsulatedSurfaceAreaFacingZone);

  bool setInsulatedSurfaceUValueFacingZone(double insulatedSurfaceUValueFacingZone);

  void resetInsulatedSurfaceUValueFacingZone();

  bool setAreaofGlassReachInDoorsFacingZone(double areaofGlassReachInDoorsFacingZone);

  void resetAreaofGlassReachInDoorsFacingZone();

  bool setHeightofGlassReachInDoorsFacingZone(double heightofGlassReachInDoorsFacingZone);

  void resetHeightofGlassReachInDoorsFacingZone();

  bool setGlassReachInDoorUValueFacingZone(double glassReachInDoorUValueFacingZone);

  void resetGlassReachInDoorUValueFacingZone();

  bool setGlassReachInDoorOpeningScheduleFacingZone(Schedule& schedule);

  void resetGlassReachInDoorOpeningScheduleFacingZone();

  bool setAreaofStockingDoorsFacingZone(double areaofStockingDoorsFacingZone);

  void resetAreaofStockingDoorsFacingZone();

  bool setHeightofStockingDoorsFacingZone(double heightofStockingDoorsFacingZone);

  void resetHeightofStockingDoorsFacingZone();

  bool setStockingDoorUValueFacingZone(double stockingDoorUValueFacingZone);

  void resetStockingDoorUValueFacingZone();

  bool setStockingDoorOpeningScheduleFacingZone(Schedule& schedule);

  void resetStockingDoorOpeningScheduleFacingZone();

  bool setStockingDoorOpeningProtectionTypeFacingZone(std::string stockingDoorOpeningProtectionTypeFacingZone);

  void resetStockingDoorOpeningProtectionTypeFacingZone();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RefrigerationWalkInZoneBoundary_Impl ImplType;

  explicit RefrigerationWalkInZoneBoundary(std::shared_ptr<detail::RefrigerationWalkInZoneBoundary_Impl> impl);

  friend class detail::RefrigerationWalkInZoneBoundary_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RefrigerationWalkInZoneBoundary");
};

/** \relates RefrigerationWalkInZoneBoundary*/
typedef boost::optional<RefrigerationWalkInZoneBoundary> OptionalRefrigerationWalkInZoneBoundary;

/** \relates RefrigerationWalkInZoneBoundary*/
typedef std::vector<RefrigerationWalkInZoneBoundary> RefrigerationWalkInZoneBoundaryVector;

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONWALKINZONEBOUNDARY_HPP
