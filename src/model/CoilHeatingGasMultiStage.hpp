/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGGASMULTISTAGE_HPP
#define MODEL_COILHEATINGGASMULTISTAGE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class CoilHeatingGasMultiStageStageData;
  class Curve;

  namespace detail {

    class CoilHeatingGasMultiStage_Impl;

  }  // namespace detail

  /** CoilHeatingGasMultiStage is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:Gas:MultiStage'.
 * Performance data for each stage is contained in a separate ModelObject called OS:Coil:Heating:Gas:MultiStage:StageData **/
  class MODEL_API CoilHeatingGasMultiStage : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingGasMultiStage(const Model& model);

    virtual ~CoilHeatingGasMultiStage() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingGasMultiStage(const CoilHeatingGasMultiStage& other) = default;
    CoilHeatingGasMultiStage(CoilHeatingGasMultiStage&& other) = default;
    CoilHeatingGasMultiStage& operator=(const CoilHeatingGasMultiStage&) = default;
    CoilHeatingGasMultiStage& operator=(CoilHeatingGasMultiStage&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<Curve> partLoadFractionCorrelationCurve() const;

    OS_DEPRECATED(3, 7, 0) boost::optional<double> parasiticGasLoad() const;

    boost::optional<double> offCycleParasiticGasLoad() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setPartLoadFractionCorrelationCurve(const Curve& curve);

    void resetPartLoadFractionCorrelationCurve();

    OS_DEPRECATED(3, 7, 0) bool setParasiticGasLoad(double parasiticGasLoad);

    bool setOffCycleParasiticGasLoad(double offCycleParasiticGasLoad);

    OS_DEPRECATED(3, 7, 0) void resetParasiticGasLoad();

    void resetOffCycleParasiticGasLoad();

    //@}
    /** @name Other */
    //@{

    /** Return the performance data for each stage. **/
    std::vector<CoilHeatingGasMultiStageStageData> stages() const;

    unsigned numberOfStages() const;

    /*
   * Get the index of a given CoilHeatingGasMultiStageStageData (1-indexed)
   */
    boost::optional<unsigned> stageIndex(const CoilHeatingGasMultiStageStageData& stage) const;

    /*
   * Add a new stage after all of the existing stages.
   */
    bool addStage(const CoilHeatingGasMultiStageStageData& stage);

    /*
   * Add a new CoilHeatingGasMultiStageStageData to the list which a given index (1 to x).
   * Internally calls addStage then setStageIndex, see remarks there
   */
    bool addStage(const CoilHeatingGasMultiStageStageData& stage, unsigned index);

    /*
   * You can shuffle the priority of a given CoilHeatingGasMultiStageStageData after having added it
   * If index is below 1, it's reset to 1.
   * If index is greater than the number of stages, will reset to last
   */
    bool setStageIndex(const CoilHeatingGasMultiStageStageData& stage, unsigned index);

    /*
   * Set all stages using a list of CoilHeatingGasMultiStageStageDatas
   * Internally calls addStage, and will return the global status, but will continue trying if there are problems
   * (eg: if you make a vector larger than the number of accepted stages, or a vector that has a stage from another model, the valid stages will be
   * added indeed, but it'll eventually return false)
   */
    bool setStages(const std::vector<CoilHeatingGasMultiStageStageData>& stages);

    /*
   * Removes all CoilHeatingGasMultiStageStageDatas in this object
   */
    void removeAllStages();

    /*
   * Remove the given CoilHeatingGasMultiStageStageData from this object's stages
   */
    bool removeStage(const CoilHeatingGasMultiStageStageData& stage);

    /*
   * Remove the CoilHeatingGasMultiStageStageData at the given index (1-indexed)
   */
    bool removeStage(unsigned index);

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingGasMultiStage_Impl;

    explicit CoilHeatingGasMultiStage(std::shared_ptr<detail::CoilHeatingGasMultiStage_Impl> impl);

    friend class detail::CoilHeatingGasMultiStage_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingGasMultiStage");
  };

  /** \relates CoilHeatingGasMultiStage*/
  using OptionalCoilHeatingGasMultiStage = boost::optional<CoilHeatingGasMultiStage>;

  /** \relates CoilHeatingGasMultiStage*/
  using CoilHeatingGasMultiStageVector = std::vector<CoilHeatingGasMultiStage>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGGASMULTISTAGE_HPP
