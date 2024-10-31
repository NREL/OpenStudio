/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGELECTRICMULTISTAGESTAGEDATA_IMPL_HPP
#define MODEL_COILHEATINGELECTRICMULTISTAGESTAGEDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class CoilHeatingElectricMultiStage;

  namespace detail {

    /** CoilHeatingElectricMultiStageStageData_Impl is a ModelObject_Impl that is the implementation class for CoilHeatingElectricMultiStageStageData.*/
    class MODEL_API CoilHeatingElectricMultiStageStageData_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingElectricMultiStageStageData_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingElectricMultiStageStageData_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingElectricMultiStageStageData_Impl(const CoilHeatingElectricMultiStageStageData_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingElectricMultiStageStageData_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      // If this object is used by any CoilHeatingElectricMultiStage, remove the corresponding extensible group to avoid having 'blanks'
      virtual std::vector<IdfObject> remove() override;

      //@}
      /** @name Getters */
      //@{

      double efficiency() const;

      boost::optional<double> nominalCapacity() const;

      bool isNominalCapacityAutosized() const;

      //@}
      /** @name Setters */
      //@{

      bool setEfficiency(double StageEfficiency);

      bool setNominalCapacity(boost::optional<double> StageNominalCapacity);

      void autosizeNominalCapacity();

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedNominalCapacity() const;

      void autosize();

      void applySizingValues();

      // Returns the CoilHeatingElectricMultiStage that references it if any
      boost::optional<CoilHeatingElectricMultiStage> parentCoil() const;

      // Used to determine the index of this performance data in the
      // list of stages in the parent object.
      boost::optional<std::tuple<int, CoilHeatingElectricMultiStage>> stageIndexAndParentCoil() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingElectricMultiStageStageData");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGELECTRICMULTISTAGESTAGEDATA_IMPL_HPP
