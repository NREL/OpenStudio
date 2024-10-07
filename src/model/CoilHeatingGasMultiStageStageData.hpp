/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGGASMULTISTAGESTAGEDATA_HPP
#define MODEL_COILHEATINGGASMULTISTAGESTAGEDATA_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "../model/CoilHeatingGasMultiStage.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class CoilHeatingGasMultiStageStageData_Impl;

  }  // namespace detail

  /** CoilHeatingGasMultiStageStageData is a ModelObject that wraps the OpenStudio IDD object 'OS:Coil:Heating:Gas:MultiStage:StageData'. */
  class MODEL_API CoilHeatingGasMultiStageStageData : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingGasMultiStageStageData(const Model& model);

    virtual ~CoilHeatingGasMultiStageStageData() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingGasMultiStageStageData(const CoilHeatingGasMultiStageStageData& other) = default;
    CoilHeatingGasMultiStageStageData(CoilHeatingGasMultiStageStageData&& other) = default;
    CoilHeatingGasMultiStageStageData& operator=(const CoilHeatingGasMultiStageStageData&) = default;
    CoilHeatingGasMultiStageStageData& operator=(CoilHeatingGasMultiStageStageData&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double gasBurnerEfficiency() const;

    boost::optional<double> nominalCapacity() const;

    bool isNominalCapacityAutosized() const;

    OS_DEPRECATED(3, 7, 0) double parasiticElectricLoad() const;

    double onCycleParasiticElectricLoad() const;

    //@}
    /** @name Setters */
    //@{

    bool setGasBurnerEfficiency(double gasBurnerEfficiency);

    bool setNominalCapacity(double stageNominalCapacity);

    void autosizeNominalCapacity();

    OS_DEPRECATED(3, 7, 0) bool setParasiticElectricLoad(double stageParasiticElectricLoad);

    bool setOnCycleParasiticElectricLoad(double stageOnCycleParasiticElectricLoad);

    //@}
    /** @name Other */
    //@{

    // Returns the CoilHeatingGasMultiStage that references it if any
    boost::optional<CoilHeatingGasMultiStage> parentCoil() const;

    boost::optional<double> autosizedNominalCapacity() const;

    void autosize();

    void applySizingValues();

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingGasMultiStageStageData_Impl;

    explicit CoilHeatingGasMultiStageStageData(std::shared_ptr<detail::CoilHeatingGasMultiStageStageData_Impl> impl);

    friend class detail::CoilHeatingGasMultiStageStageData_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingGasMultiStageStageData");

    // Used to determine the index of this performance data in the
    // list of stages in the parent object.
    boost::optional<std::tuple<int, CoilHeatingGasMultiStage>> stageIndexAndParentCoil() const;
  };

  /** \relates CoilHeatingGasMultiStageStageData*/
  using OptionalCoilHeatingGasMultiStageStageData = boost::optional<CoilHeatingGasMultiStageStageData>;

  /** \relates CoilHeatingGasMultiStageStageData*/
  using CoilHeatingGasMultiStageStageDataVector = std::vector<CoilHeatingGasMultiStageStageData>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGGASMULTISTAGESTAGEDATA_HPP
