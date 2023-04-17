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

#ifndef MODEL_ELECTRICEQUIPMENTITEAIRCOOLED_HPP
#define MODEL_ELECTRICEQUIPMENTITEAIRCOOLED_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {

namespace model {

  class ElectricEquipmentITEAirCooledDefinition;
  class Curve;
  class Schedule;

  namespace detail {

    class ElectricEquipmentITEAirCooled_Impl;

  }  // namespace detail

  /** ElectricEquipmentITEAirCooled is a SpaceLoadInstance that wraps the OpenStudio IDD object 'OS:ElectricEquipment:ITE:AirCooled'. */
  /* Its fields are derived from the EnergyPlus IDD object 'ElectricEquipmentITEAirCooled'. \sa ElectricEquipmentITEAirCooledDefinition */
  class MODEL_API ElectricEquipmentITEAirCooled : public SpaceLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ElectricEquipmentITEAirCooled(const ElectricEquipmentITEAirCooledDefinition& electricEquipmentITEAirCooledDefinition);

    virtual ~ElectricEquipmentITEAirCooled() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ElectricEquipmentITEAirCooled(const ElectricEquipmentITEAirCooled& other) = default;
    ElectricEquipmentITEAirCooled(ElectricEquipmentITEAirCooled&& other) = default;
    ElectricEquipmentITEAirCooled& operator=(const ElectricEquipmentITEAirCooled&) = default;
    ElectricEquipmentITEAirCooled& operator=(ElectricEquipmentITEAirCooled&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ElectricEquipmentITEAirCooledDefinition electricEquipmentITEAirCooledDefinition() const;

    // Return the Design Power Input Schedule. If not specified, default to always 1.
    boost::optional<Schedule> designPowerInputSchedule() const;

    bool isDesignPowerInputScheduleDefaulted() const;

    // Return the CPU Loading Schedule. If not specified, default to always 1.
    boost::optional<Schedule> cPULoadingSchedule() const;

    bool isCPULoadingScheduleDefaulted() const;

    std::string cPUEndUseSubcategory() const;

    bool isCPUEndUseSubcategoryDefaulted() const;

    std::string fanEndUseSubcategory() const;

    bool isFanEndUseSubcategoryDefaulted() const;

    std::string electricPowerSupplyEndUseSubcategory() const;

    bool isElectricPowerSupplyEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setElectricEquipmentITEAirCooledDefinition(const ElectricEquipmentITEAirCooledDefinition& electricEquipmentITEAirCooledDefinition);

    // Note Schedules are passed by reference, not const reference.
    bool setDesignPowerInputSchedule(Schedule& schedule);

    void resetDesignPowerInputSchedule();

    // Note Schedules are passed by reference, not const reference.
    bool setCPULoadingSchedule(Schedule& schedule);

    void resetCPULoadingSchedule();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    bool setCPUEndUseSubcategory(const std::string& cPUEndUseSubcategory);

    void resetCPUEndUseSubcategory();

    bool setFanEndUseSubcategory(const std::string& fanEndUseSubcategory);

    void resetFanEndUseSubcategory();

    bool setElectricPowerSupplyEndUseSubcategory(const std::string& electricPowerSupplyEndUseSubcategory);

    void resetElectricPowerSupplyEndUseSubcategory();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> wattsperUnit() const;

    boost::optional<double> wattsperZoneFloorArea() const;

    double getWattsperUnit(double floorArea) const;

    double getWattsperZoneFloorArea(double floorArea) const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ElectricEquipmentITEAirCooled_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit ElectricEquipmentITEAirCooled(std::shared_ptr<detail::ElectricEquipmentITEAirCooled_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ElectricEquipmentITEAirCooled");
  };

  /** \relates ElectricEquipmentITEAirCooled*/
  using OptionalElectricEquipmentITEAirCooled = boost::optional<ElectricEquipmentITEAirCooled>;

  /** \relates ElectricEquipmentITEAirCooled*/
  using ElectricEquipmentITEAirCooledVector = std::vector<ElectricEquipmentITEAirCooled>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICEQUIPMENTITEAIRCOOLED_HPP
