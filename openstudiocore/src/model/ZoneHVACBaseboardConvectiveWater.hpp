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

#ifndef MODEL_ZONEHVACBASEBOARDCONVECTIVEWATER_HPP
#define MODEL_ZONEHVACBASEBOARDCONVECTIVEWATER_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"
#include "ThermalZone.hpp"
#include "Model.hpp"

namespace openstudio {
namespace model {

class Schedule;
class StraightComponent;

namespace detail {

  class ZoneHVACBaseboardConvectiveWater_Impl;

} // detail

/** ZoneHVACBaseboardConvectiveWater is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:Baseboard:Convective:Water'. */
class MODEL_API ZoneHVACBaseboardConvectiveWater : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  ZoneHVACBaseboardConvectiveWater(const Model& model,
                                            Schedule & availabilitySchedule,
                                            StraightComponent & heatingCoilBaseboard  );

  virtual ~ZoneHVACBaseboardConvectiveWater() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  StraightComponent heatingCoil() const;


  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setHeatingCoil(const StraightComponent& heatingCoilBaseboard);

   /** Returns the optional ThermalZone that this ZoneHVACBaseboardConvectiveWater is attached to
   **/
  boost::optional<ThermalZone> thermalZone();

  /** Adds this ZoneHVACBaseboardConvectiveWater to the thermal zone.  Returns true if the operation was
   *  successful.
   **/
  bool addToThermalZone(ThermalZone & thermalZone);

  /** Detaches this ZoneHVACBaseboardConvectiveWater from the associated ThermalZone.
   *  If there is no attached ThermalZone there is no effect.
   **/
  void removeFromThermalZone();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACBaseboardConvectiveWater_Impl ImplType;

  explicit ZoneHVACBaseboardConvectiveWater(std::shared_ptr<detail::ZoneHVACBaseboardConvectiveWater_Impl> impl);

  friend class detail::ZoneHVACBaseboardConvectiveWater_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardConvectiveWater");
};

/** \relates ZoneHVACBaseboardConvectiveWater*/
typedef boost::optional<ZoneHVACBaseboardConvectiveWater> OptionalZoneHVACBaseboardConvectiveWater;

/** \relates ZoneHVACBaseboardConvectiveWater*/
typedef std::vector<ZoneHVACBaseboardConvectiveWater> ZoneHVACBaseboardConvectiveWaterVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACBASEBOARDCONVECTIVEWATER_HPP

