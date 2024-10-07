/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGELECTRICMULTISTAGESTAGEDATA_HPP
#define MODEL_COILHEATINGELECTRICMULTISTAGESTAGEDATA_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "../model/CoilHeatingElectricMultiStage.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class CoilHeatingElectricMultiStageStageData_Impl;

  }  // namespace detail

  /** CoilHeatingElectricMultiStageStageData is a ModelObject that wraps the OpenStudio IDD object 'OS:Coil:Heating:Electric:MultiStage:StageData'. */
  class MODEL_API CoilHeatingElectricMultiStageStageData : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingElectricMultiStageStageData(const Model& model);

    virtual ~CoilHeatingElectricMultiStageStageData() override = default;
    CoilHeatingElectricMultiStageStageData(const CoilHeatingElectricMultiStageStageData& other) = default;
    CoilHeatingElectricMultiStageStageData(CoilHeatingElectricMultiStageStageData&& other) = default;
    CoilHeatingElectricMultiStageStageData& operator=(const CoilHeatingElectricMultiStageStageData&) = default;
    CoilHeatingElectricMultiStageStageData& operator=(CoilHeatingElectricMultiStageStageData&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double efficiency() const;

    boost::optional<double> nominalCapacity() const;

    bool isNominalCapacityAutosized() const;

    //@}
    /** @name Setters */
    //@{

    bool setEfficiency(double StageEfficiency);

    bool setNominalCapacity(double StageNominalCapacity);

    void autosizeNominalCapacity();

    //@}
    /** @name Other */
    //@{

    // Returns the CoilHeatingElectricMultiStage that references it if any
    boost::optional<CoilHeatingElectricMultiStage> parentCoil() const;

    boost::optional<double> autosizedNominalCapacity() const;

    void autosize();

    void applySizingValues();

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingElectricMultiStageStageData_Impl;

    explicit CoilHeatingElectricMultiStageStageData(std::shared_ptr<detail::CoilHeatingElectricMultiStageStageData_Impl> impl);

    friend class detail::CoilHeatingElectricMultiStageStageData_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingElectricMultiStageStageData");

    // Used to determine the index of this performance data in the
    // list of stages in the parent object.
    boost::optional<std::tuple<int, CoilHeatingElectricMultiStage>> stageIndexAndParentCoil() const;
  };

  /** \relates CoilHeatingElectricMultiStageStageData*/
  using OptionalCoilHeatingElectricMultiStageStageData = boost::optional<CoilHeatingElectricMultiStageStageData>;

  /** \relates CoilHeatingElectricMultiStageStageData*/
  using CoilHeatingElectricMultiStageStageDataVector = std::vector<CoilHeatingElectricMultiStageStageData>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGELECTRICMULTISTAGESTAGEDATA_HPP
