/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELLELECTRICALSTORAGE_IMPL_HPP
#define MODEL_GENERATORFUELCELLELECTRICALSTORAGE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class GeneratorFuelCell;

  namespace detail {

    /** GeneratorFuelCellElectricalStorage_Impl is a ModelObject_Impl that is the implementation class for GeneratorFuelCellElectricalStorage.*/
    class MODEL_API GeneratorFuelCellElectricalStorage_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorFuelCellElectricalStorage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellElectricalStorage_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellElectricalStorage_Impl(const GeneratorFuelCellElectricalStorage_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorFuelCellElectricalStorage_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string choiceofModel() const;

      double nominalChargingEnergeticEfficiency() const;

      double nominalDischargingEnergeticEfficiency() const;

      double simpleMaximumCapacity() const;

      double simpleMaximumPowerDraw() const;

      double simpleMaximumPowerStore() const;

      double initialChargeState() const;

      // Return optional parent generator
      boost::optional<GeneratorFuelCell> fuelCell() const;

      //@}
      /** @name Setters */
      //@{

      bool setChoiceofModel(const std::string& choiceofModel);

      void resetChoiceofModel();

      bool setNominalChargingEnergeticEfficiency(double nominalChargingEnergeticEfficiency);

      void resetNominalChargingEnergeticEfficiency();

      bool setNominalDischargingEnergeticEfficiency(double nominalDischargingEnergeticEfficiency);

      void resetNominalDischargingEnergeticEfficiency();

      bool setSimpleMaximumCapacity(double simpleMaximumCapacity);

      void resetSimpleMaximumCapacity();

      bool setSimpleMaximumPowerDraw(double simpleMaximumPowerDraw);

      void resetSimpleMaximumPowerDraw();

      bool setSimpleMaximumPowerStore(double simpleMaximumPowerStore);

      void resetSimpleMaximumPowerStore();

      bool setInitialChargeState(double initialChargeState);

      void resetInitialChargeState();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorFuelCellElectricalStorage");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLELECTRICALSTORAGE_IMPL_HPP
