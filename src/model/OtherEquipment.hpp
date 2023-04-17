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

#ifndef MODEL_OTHEREQUIPMENT_HPP
#define MODEL_OTHEREQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {
namespace model {

  class OtherEquipmentDefinition;
  class Schedule;

  namespace detail {

    class OtherEquipment_Impl;

  }  // namespace detail

  /** OtherEquipment is a SpaceLoadInstance that wraps the OpenStudio IDD object
 *  'OS:OtherEquipment'. \sa OtherEquipmentDefinition */
  class MODEL_API OtherEquipment : public SpaceLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit OtherEquipment(const OtherEquipmentDefinition& definition);

    virtual ~OtherEquipment() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OtherEquipment(const OtherEquipment& other) = default;
    OtherEquipment(OtherEquipment&& other) = default;
    OtherEquipment& operator=(const OtherEquipment&) = default;
    OtherEquipment& operator=(OtherEquipment&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Returns the End-Use Subcategory **/
    std::string endUseSubcategory() const;

    /** Returns true if the end-use subcategory is defaulted **/
    bool isEndUseSubcategoryDefaulted() const;

    /** Returns a list of valid fuel types. **/
    static std::vector<std::string> validFuelTypeValues();

    /** Gets the equipment fuel type **/
    std::string fuelType() const;

    /** Returns true if the fuel type is defaulted **/
    bool isFuelTypeDefaulted() const;

    OtherEquipmentDefinition otherEquipmentDefinition() const;

    /** Returns the (fractional) equipment schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
    boost::optional<Schedule> schedule() const;

    /** Returns true if this object does not specify a schedule directly. */
    bool isScheduleDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the End-Use Subcategory **/
    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    /** Resets the End-Use Subcategory **/
    void resetEndUseSubcategory();

    /** Sets the fuel type. **/
    bool setFuelType(const std::string& fuelType);

    /** Resets the fuel type to its default. **/
    void resetFuelType();

    bool setOtherEquipmentDefinition(const OtherEquipmentDefinition& definition);

    /** Sets the (fractional) Schedule. */
    bool setSchedule(Schedule& schedule);

    /** Resets the (fractional) Schedule. */
    void resetSchedule();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

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
    using ImplType = detail::OtherEquipment_Impl;

    friend class detail::OtherEquipment_Impl;
    friend class openstudio::detail::IdfObject_Impl;
    friend class IdfObject;
    friend class Model;

    explicit OtherEquipment(std::shared_ptr<detail::OtherEquipment_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OtherEquipment");
  };

  /** \relates OtherEquipment*/
  using OptionalOtherEquipment = boost::optional<OtherEquipment>;

  /** \relates OtherEquipment*/
  using OtherEquipmentVector = std::vector<OtherEquipment>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OTHEREQUIPMENT_HPP
