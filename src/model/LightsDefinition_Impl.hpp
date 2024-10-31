/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LIGHTSDEFINITION_IMPL_HPP
#define MODEL_LIGHTSDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition_Impl.hpp"

namespace openstudio {
namespace model {

  class LightsDefinition;

  namespace detail {

    /** LightsDefinition_Impl is a SpaceLoadDefinition_Impl that is the implementation class for LightsDefinition.*/
    class MODEL_API LightsDefinition_Impl : public SpaceLoadDefinition_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      LightsDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      LightsDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      LightsDefinition_Impl(const LightsDefinition_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~LightsDefinition_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string designLevelCalculationMethod() const;
      boost::optional<double> lightingLevel() const;
      boost::optional<double> wattsperSpaceFloorArea() const;
      boost::optional<double> wattsperPerson() const;

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

      bool setLightingLevel(boost::optional<double> lightingLevel);

      bool setWattsperSpaceFloorArea(boost::optional<double> wattsperSpaceFloorArea);

      bool setWattsperPerson(boost::optional<double> wattsperPerson);

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
      /** @name Other */
      //@{

      /** Returns the lighting level represented by this definition, assuming floorArea (m^2) and
     *  numPeople. */
      double getLightingPower(double floorArea, double numPeople) const;

      /** Returns the watts/m^2 represented by this definition, assuming floorArea (m^2) and
     *  numPeople. */
      double getPowerPerFloorArea(double floorArea, double numPeople) const;

      /** Returns the watts/person represented by this definition, assuming floorArea (m^2) and
     *  numPeople. */
      double getPowerPerPerson(double floorArea, double numPeople) const;

      /** If method is a \link validDesignLevelCalculationMethodValues valid design level
     *  calculation method \endlink, changes this definition to an equivalent lighting level, under
     *  the assumptions of floorArea (m^2) and numPeople. */
      bool setDesignLevelCalculationMethod(const std::string& method, double floorArea, double numPeople);

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.LightsDefinition");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LIGHTSDEFINITION_IMPL_HPP
