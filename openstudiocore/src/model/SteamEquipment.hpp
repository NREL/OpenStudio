/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_STEAMEQUIPMENT_HPP
#define MODEL_STEAMEQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {
namespace model {

class SteamEquipmentDefinition;
class Schedule;

namespace detail {

  class SteamEquipment_Impl;

} // detail

/** SteamEquipment is a SpaceLoadInstance that wraps the OpenStudio IDD object
 *  'OS:SteamEquipment'. \sa SteamEquipmentDefinition
 */
class MODEL_API SteamEquipment : public SpaceLoadInstance {
 public:

  /** @name Constructors and Destructors */
  //@{

  explicit SteamEquipment(const SteamEquipmentDefinition& definition);

  virtual ~SteamEquipment() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  SteamEquipmentDefinition steamEquipmentDefinition() const;

  /** Returns the (fractional) equipment schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
  boost::optional<Schedule> schedule() const;

  /** Returns true if this object does not specify a schedule directly. */
  bool isScheduleDefaulted() const;

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setSteamEquipmentDefinition(const SteamEquipmentDefinition& definition);

  /** Sets the (fractional) Schedule. */
  bool setSchedule(Schedule& schedule);

  /** Resets the (fractional) Schedule. */
  void resetSchedule();

  bool setMultiplier(double multiplier);

  void resetMultiplier();

  bool setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

  //@}
  /** @name Other */
  //@{

  /** Returns the design level represented by this instance, assuming floorArea (m^2) and
   *  numPeople. */
  double getDesignLevel(double floorArea, double numPeople) const;

  /** Returns the watts/m^2 represented by this instance, assuming floorArea (m^2) and
   *  numPeople. */
  double getPowerPerFloorArea(double floorArea, double numPeople) const;

  /** Returns the watts/person represented by this instance, assuming floorArea (m^2) and
   *  numPeople. */
  double getPowerPerPerson(double floorArea, double numPeople) const;

  //@}
 protected:
  /// @cond
  typedef detail::SteamEquipment_Impl ImplType;

  friend class detail::SteamEquipment_Impl;
  friend class openstudio::detail::IdfObject_Impl;
  friend class IdfObject;
  friend class Model;

  explicit SteamEquipment(std::shared_ptr<detail::SteamEquipment_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.SteamEquipment");
};

/** \relates SteamEquipment*/
typedef boost::optional<SteamEquipment> OptionalSteamEquipment;

/** \relates SteamEquipment*/
typedef std::vector<SteamEquipment> SteamEquipmentVector;

} // model
} // openstudio

#endif // MODEL_STEAMEQUIPMENT_HPP

