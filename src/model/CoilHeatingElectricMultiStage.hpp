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

#ifndef MODEL_COILHEATINGELECTRICMULTISTAGE_HPP
#define MODEL_COILHEATINGELECTRICMULTISTAGE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class CoilHeatingElectricMultiStageStageData;

  namespace detail {

    class CoilHeatingElectricMultiStage_Impl;

  }  // namespace detail

  /** CoilHeatingElectricMultiStage is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:Electric:MultiStage'.
 * Performance data for each stage is contained in a separate ModelObject called OS:Coil:Heating:Electric:MultiStage:StageData **/
  class MODEL_API CoilHeatingElectricMultiStage : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingElectricMultiStage(const Model& model);

    virtual ~CoilHeatingElectricMultiStage() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingElectricMultiStage(const CoilHeatingElectricMultiStage& other) = default;
    CoilHeatingElectricMultiStage(CoilHeatingElectricMultiStage&& other) = default;
    CoilHeatingElectricMultiStage& operator=(const CoilHeatingElectricMultiStage&) = default;
    CoilHeatingElectricMultiStage& operator=(CoilHeatingElectricMultiStage&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    //@}
    /** @name Other */
    //@{

    /** Return the performance data for each stage. **/
    std::vector<CoilHeatingElectricMultiStageStageData> stages() const;

    unsigned numberOfStages() const;

    /*
   * Get the index of a given CoilHeatingElectricMultiStageStageData (1-indexed)
   */
    boost::optional<unsigned> stageIndex(const CoilHeatingElectricMultiStageStageData& stage) const;

    /*
   * Add a new stage after all of the existing stages.
   */
    bool addStage(const CoilHeatingElectricMultiStageStageData& stage);

    /*
   * Add a new CoilHeatingElectricMultiStageStageData to the list which a given index (1 to x).
   * Internally calls addStage then setStageIndex, see remarks there
   */
    bool addStage(const CoilHeatingElectricMultiStageStageData& stage, unsigned index);

    /*
   * You can shuffle the priority of a given CoilHeatingElectricMultiStageStageData after having added it
   * If index is below 1, it's reset to 1.
   * If index is greater than the number of stages, will reset to last
   */
    bool setStageIndex(const CoilHeatingElectricMultiStageStageData& stage, unsigned index);

    /*
   * Set all stages using a list of CoilHeatingElectricMultiStageStageDatas
   * Internally calls addStage, and will return the global status, but will continue trying if there are problems
   * (eg: if you make a vector larger than the number of accepted stages, or a vector that has a stage from another model, the valid stages will be
   * added indeed, but it'll eventually return false)
   */
    bool setStages(const std::vector<CoilHeatingElectricMultiStageStageData>& stages);

    /*
   * Removes all CoilHeatingElectricMultiStageStageDatas in this object
   */
    void removeAllStages();

    /*
   * Remove the given CoilHeatingElectricMultiStageStageData from this object's stages
   */
    bool removeStage(const CoilHeatingElectricMultiStageStageData& stage);

    /*
   * Remove the CoilHeatingElectricMultiStageStageData at the given index (1-indexed)
   */
    bool removeStage(unsigned index);

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingElectricMultiStage_Impl;

    explicit CoilHeatingElectricMultiStage(std::shared_ptr<detail::CoilHeatingElectricMultiStage_Impl> impl);

    friend class detail::CoilHeatingElectricMultiStage_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingElectricMultiStage");
  };

  /** \relates CoilHeatingElectricMultiStage*/
  using OptionalCoilHeatingElectricMultiStage = boost::optional<CoilHeatingElectricMultiStage>;

  /** \relates CoilHeatingElectricMultiStage*/
  using CoilHeatingElectricMultiStageVector = std::vector<CoilHeatingElectricMultiStage>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGELECTRICMULTISTAGE_HPP
