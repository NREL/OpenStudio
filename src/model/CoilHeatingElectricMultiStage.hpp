/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~CoilHeatingElectricMultiStage() override = default;
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
