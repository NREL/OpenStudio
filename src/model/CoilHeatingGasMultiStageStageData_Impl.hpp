/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGGASMULTISTAGESTAGEDATA_IMPL_HPP
#define MODEL_COILHEATINGGASMULTISTAGESTAGEDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class CoilHeatingGasMultiStage;

  namespace detail {

    /** CoilHeatingGasMultiStageStageData_Impl is a ModelObject_Impl that is the implementation class for CoilHeatingGasMultiStageStageData.*/
    class MODEL_API CoilHeatingGasMultiStageStageData_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingGasMultiStageStageData_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingGasMultiStageStageData_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingGasMultiStageStageData_Impl(const CoilHeatingGasMultiStageStageData_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingGasMultiStageStageData_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      // If this object is used by any CoilHeatingGasMultiStage, remove the corresponding extensible group to avoid having 'blanks'
      virtual std::vector<IdfObject> remove() override;

      //@}
      /** @name Getters */
      //@{

      double gasBurnerEfficiency() const;

      boost::optional<double> nominalCapacity() const;

      bool isNominalCapacityAutosized() const;

      double onCycleParasiticElectricLoad() const;

      boost::optional<double> autosizedNominalCapacity() const;

      //@}
      /** @name Setters */
      //@{

      bool setGasBurnerEfficiency(double stageGasBurnerEfficiency);

      bool setNominalCapacity(double stageNominalCapacity);

      void autosizeNominalCapacity();

      bool setOnCycleParasiticElectricLoad(double stageonCycleParasiticElectricLoad);

      //@}
      /** @name Other */
      //@{

      void autosize();

      void applySizingValues();

      // Returns the CoilHeatingGasMultiStage that references it if any
      boost::optional<CoilHeatingGasMultiStage> parentCoil() const;

      // Used to determine the index of this performance data in the
      // list of stages in the parent object.
      boost::optional<std::tuple<int, CoilHeatingGasMultiStage>> stageIndexAndParentCoil() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingGasMultiStageStageData");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGGASMULTISTAGESTAGEDATA_IMPL_HPP
