/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LUMINAIREDEFINITION_IMPL_HPP
#define MODEL_LUMINAIREDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition_Impl.hpp"

namespace openstudio {
namespace model {

  class LuminaireDefinition;

  namespace detail {

    /** LuminaireDefinition_Impl is a SpaceLoadDefinition_Impl that is the implementation class for LuminaireDefinition.*/
    class MODEL_API LuminaireDefinition_Impl : public SpaceLoadDefinition_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      LuminaireDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      LuminaireDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      LuminaireDefinition_Impl(const LuminaireDefinition_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~LuminaireDefinition_Impl() override = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      // TODO: Handle Non-Extensible IddField IES File Path.

      double lightingPower() const;
      bool isLightingPowerDefaulted() const;

      double fractionRadiant() const;
      bool isFractionRadiantDefaulted() const;

      double fractionVisible() const;
      bool isFractionVisibleDefaulted() const;

      double returnAirFraction() const;
      bool isReturnAirFractionDefaulted() const;

      bool returnAirFractionCalculatedfromPlenumTemperature() const;
      bool isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted() const;

      double returnAirFractionFunctionofPlenumTemperatureCoefficient1() const;
      bool isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted() const;

      double returnAirFractionFunctionofPlenumTemperatureCoefficient2() const;
      bool isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted() const;

      //@}
      /** @name Setters */
      //@{

      // TODO: Handle Non-Extensible IddField IES File Path.

      bool setLightingPower(double lightingPower);
      void resetLightingPower();

      bool setFractionRadiant(double fractionRadiant);
      void resetFractionRadiant();

      bool setFractionVisible(double fractionVisible);
      void resetFractionVisible();

      bool setReturnAirFraction(double returnAirFraction);
      void resetReturnAirFraction();

      bool setReturnAirFractionCalculatedfromPlenumTemperature(bool returnAirFractionCalculatedfromPlenumTemperature);
      void resetReturnAirFractionCalculatedfromPlenumTemperature();

      bool setReturnAirFractionFunctionofPlenumTemperatureCoefficient1(double returnAirFractionFunctionofPlenumTemperatureCoefficient1);
      void resetReturnAirFractionFunctionofPlenumTemperatureCoefficient1();

      bool setReturnAirFractionFunctionofPlenumTemperatureCoefficient2(double returnAirFractionFunctionofPlenumTemperatureCoefficient2);
      void resetReturnAirFractionFunctionofPlenumTemperatureCoefficient2();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.LuminaireDefinition");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LUMINAIREDEFINITION_IMPL_HPP
