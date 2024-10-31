/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELLINVERTER_IMPL_HPP
#define MODEL_GENERATORFUELCELLINVERTER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class CurveQuadratic;
  class GeneratorFuelCell;

  namespace detail {

    /** GeneratorFuelCellInverter_Impl is a ModelObject_Impl that is the implementation class for GeneratorFuelCellInverter.*/
    class MODEL_API GeneratorFuelCellInverter_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorFuelCellInverter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellInverter_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellInverter_Impl(const GeneratorFuelCellInverter_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorFuelCellInverter_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const;

      virtual std::vector<ModelObject> children() const;

      //@}
      /** @name Getters */
      //@{

      std::string inverterEfficiencyCalculationMode() const;

      double inverterEfficiency() const;

      boost::optional<CurveQuadratic> efficiencyFunctionofDCPowerCurve() const;

      // Return optional parent generator
      boost::optional<GeneratorFuelCell> fuelCell() const;

      //@}
      /** @name Setters */
      //@{

      bool setInverterEfficiencyCalculationMode(const std::string& inverterEfficiencyCalculationMode);

      void resetInverterEfficiencyCalculationMode();

      bool setInverterEfficiency(double inverterEfficiency);

      void resetInverterEfficiency();

      bool setEfficiencyFunctionofDCPowerCurve(const CurveQuadratic& quadraticCurves);

      void resetEfficiencyFunctionofDCPowerCurve();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorFuelCellInverter");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLINVERTER_IMPL_HPP
