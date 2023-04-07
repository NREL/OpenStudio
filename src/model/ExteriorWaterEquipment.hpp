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

#ifndef MODEL_EXTERIORWATEREQUIPMENT_HPP
#define MODEL_EXTERIORWATEREQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "ExteriorLoadInstance.hpp"

namespace openstudio {
namespace model {

  class ExteriorWaterEquipmentDefinition;
  class Schedule;
  class Facility;

  namespace detail {

    class ExteriorWaterEquipment_Impl;

  }  // namespace detail

  /** ExteriorWaterEquipment is a ExteriorLoadInstance that wraps the OpenStudio IDD object
 *  'OS:Exterior:WaterEquipment'. ExteriorWaterEquipment is a child of Facility and instances
 *  ExteriorWaterEquipmentDefinition. */
  class MODEL_API ExteriorWaterEquipment : public ExteriorLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** This constructor sets schedule to always on */
    ExteriorWaterEquipment(const ExteriorWaterEquipmentDefinition& definition);

    /** This constructor requires a user-specified schedule. */
    ExteriorWaterEquipment(const ExteriorWaterEquipmentDefinition& definition, Schedule& schedule);

    virtual ~ExteriorWaterEquipment() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExteriorWaterEquipment(const ExteriorWaterEquipment& other) = default;
    ExteriorWaterEquipment(ExteriorWaterEquipment&& other) = default;
    ExteriorWaterEquipment& operator=(const ExteriorWaterEquipment&) = default;
    ExteriorWaterEquipment& operator=(ExteriorWaterEquipment&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ExteriorWaterEquipmentDefinition exteriorWaterEquipmentDefinition() const;

    Schedule schedule() const;

    double multiplier() const;

    bool isMultiplierDefaulted() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setExteriorWaterEquipmentDefinition(const ExteriorWaterEquipmentDefinition& exteriorWaterEquipmentDefinition);

    bool setSchedule(Schedule& schedule);

    void resetSchedule();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    //@}
    /** @name Other */
    //@{

    Facility facility() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ExteriorWaterEquipment_Impl;

    explicit ExteriorWaterEquipment(std::shared_ptr<detail::ExteriorWaterEquipment_Impl> impl);

    friend class detail::ExteriorWaterEquipment_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExteriorWaterEquipment");
  };

  /** \relates ExteriorWaterEquipment*/
  using OptionalExteriorWaterEquipment = boost::optional<ExteriorWaterEquipment>;

  /** \relates ExteriorWaterEquipment*/
  using ExteriorWaterEquipmentVector = std::vector<ExteriorWaterEquipment>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORWATEREQUIPMENT_HPP
