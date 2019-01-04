/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACEQUIPMENTLIST_HPP
#define MODEL_ZONEHVACEQUIPMENTLIST_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class ZoneHVACEquipmentList_Impl;

} // detail

class ThermalZone;

/** ZoneHVACEquipmentList is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneHVAC:EquipmentList'. */
class MODEL_API ZoneHVACEquipmentList : public ModelObject {
  public:

  explicit ZoneHVACEquipmentList(const ThermalZone & thermalZone);

  virtual ~ZoneHVACEquipmentList() {}

  static IddObjectType iddObjectType();

  std::string loadDistributionScheme() const;

  bool setLoadDistributionScheme(std::string scheme);

  /** Add new equipment setting the heating and cooling priorities
   *  to the next available priority level.
   *  Air terminals associated with AirLoopHVAC will be moved to first priority.
   *  This method is relatively dumb.  It will add any model object to the list
   *  even if it is not hvac equipment.  That might change in the future.
   */
  bool addEquipment(const ModelObject & equipment);

  /** Remove equipment from the EquipmentList.
    * This will not remove the equipment from the model or
    * disconnect any node connections.  Use only if you know what you are doing.
    */
  bool removeEquipment(const ModelObject & equipment);

  /** Set cooling priority of equipment.
   *  Returns false when equipment is not in the ZoneHVACEquipmentList
   */
  bool setCoolingPriority(const ModelObject & equipment, unsigned priority);

  /** Set heating priority of equipment.
   *  Returns false when equipment is not in the ZoneHVACEquipmentList
   */
  bool setHeatingPriority(const ModelObject & euqipment, unsigned priority);

  /** Return all equipment.  Order is undetermined. */
  std::vector<ModelObject> equipment() const;

  /** Return all equipment.  Order is determined by heating priority */
  std::vector<ModelObject> equipmentInHeatingOrder() const;

  /** Return all equipment.  Order is determined by coooling priority */
  std::vector<ModelObject> equipmentInCoolingOrder() const;

  ThermalZone thermalZone() const;

  /** Return the heating priority for equipment */
  unsigned heatingPriority(const ModelObject & equipment) const;

  /** Return the cooling priority for equipment */
  unsigned coolingPriority(const ModelObject & equipment) const;

  protected:

  /// @cond

  typedef detail::ZoneHVACEquipmentList_Impl ImplType;

  explicit ZoneHVACEquipmentList(std::shared_ptr<detail::ZoneHVACEquipmentList_Impl> impl);

  friend class detail::ZoneHVACEquipmentList_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond

  private:

  REGISTER_LOGGER("openstudio.model.ZoneHVACEquipmentList");
};

/** \relates ZoneHVACEquipmentList*/
typedef boost::optional<ZoneHVACEquipmentList> OptionalZoneHVACEquipmentList;

/** \relates ZoneHVACEquipmentList*/
typedef std::vector<ZoneHVACEquipmentList> ZoneHVACEquipmentListVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACEQUIPMENTLIST_HPP

