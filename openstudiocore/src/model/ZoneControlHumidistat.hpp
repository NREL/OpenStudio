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

#ifndef MODEL_ZONECONTROLHUMIDISTAT_HPP
#define MODEL_ZONECONTROLHUMIDISTAT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  class ZoneControlHumidistat_Impl;

} // detail

/** ZoneControlHumidistat is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneControl:Humidistat'. */
class MODEL_API ZoneControlHumidistat : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneControlHumidistat(const Model& model);

  virtual ~ZoneControlHumidistat() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<ThermalZone> controlledZone() const;

  boost::optional<Schedule> humidifyingRelativeHumiditySetpointSchedule() const;

  boost::optional<Schedule> dehumidifyingRelativeHumiditySetpointSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setHumidifyingRelativeHumiditySetpointSchedule(Schedule& schedule);

  bool setDehumidifyingRelativeHumiditySetpointSchedule(Schedule& schedule);

  void resetHumidifyingRelativeHumiditySetpointSchedule();

  void resetDehumidifyingRelativeHumiditySetpointSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneControlHumidistat_Impl ImplType;

  explicit ZoneControlHumidistat(std::shared_ptr<detail::ZoneControlHumidistat_Impl> impl);

  friend class detail::ZoneControlHumidistat_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneControlHumidistat");
};

/** \relates ZoneControlHumidistat*/
typedef boost::optional<ZoneControlHumidistat> OptionalZoneControlHumidistat;

/** \relates ZoneControlHumidistat*/
typedef std::vector<ZoneControlHumidistat> ZoneControlHumidistatVector;

} // model
} // openstudio

#endif // MODEL_ZONECONTROLHUMIDISTAT_HPP

