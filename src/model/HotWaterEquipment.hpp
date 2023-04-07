/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_HOTWATEREQUIPMENT_HPP
#define MODEL_HOTWATEREQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class HotWaterEquipmentDefinition;

  namespace detail {

    class HotWaterEquipment_Impl;

  }  // namespace detail

  /** HotWaterEquipment is a SpaceLoadInstance that wraps the OpenStudio IDD object
 *  'OS:HotWaterEquipment'. Its fields are derived from the EnergyPlus IDD object
 *  'HotWaterEquipment'. \sa HotWaterEquipmentDefinition
 */
  class MODEL_API HotWaterEquipment : public SpaceLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit HotWaterEquipment(const HotWaterEquipmentDefinition& hotWaterEquipmentDefinition);

    virtual ~HotWaterEquipment() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HotWaterEquipment(const HotWaterEquipment& other) = default;
    HotWaterEquipment(HotWaterEquipment&& other) = default;
    HotWaterEquipment& operator=(const HotWaterEquipment&) = default;
    HotWaterEquipment& operator=(HotWaterEquipment&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Gets the HotWaterEquipmentDefinition object. */
    HotWaterEquipmentDefinition hotWaterEquipmentDefinition() const;

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

    /** Sets the HotWaterEquipmentDefinition object. */
    bool setHotWaterEquipmentDefinition(const HotWaterEquipmentDefinition& definition);

    /** Sets the (fractional) Schedule. */
    bool setSchedule(Schedule& schedule);

    /** Resets the (fractional) Schedule. */
    void resetSchedule();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

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
    using ImplType = detail::HotWaterEquipment_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit HotWaterEquipment(std::shared_ptr<detail::HotWaterEquipment_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.HotWaterEquipment");
  };

  /** \relates HotWaterEquipment*/
  using OptionalHotWaterEquipment = boost::optional<HotWaterEquipment>;

  /** \relates HotWaterEquipment*/
  using HotWaterEquipmentVector = std::vector<HotWaterEquipment>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HOTWATEREQUIPMENT_HPP
