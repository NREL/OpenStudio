/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELLAIRSUPPLY_IMPL_HPP
#define MODEL_GENERATORFUELCELLAIRSUPPLY_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;
  class CurveCubic;
  class CurveQuadratic;
  class GeneratorFuelCell;
  class AirSupplyConstituent;

  namespace detail {

    /** GeneratorFuelCellAirSupply_Impl is a ModelObject_Impl that is the implementation class for GeneratorFuelCellAirSupply.*/
    class MODEL_API GeneratorFuelCellAirSupply_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorFuelCellAirSupply_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellAirSupply_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellAirSupply_Impl(const GeneratorFuelCellAirSupply_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorFuelCellAirSupply_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const;

      virtual std::vector<ModelObject> children() const;

      //extensible fields.
      bool addConstituent(const AirSupplyConstituent& constituent);
      bool addConstituent(const std::string& name, double molarFraction);

      bool removeConstituent(unsigned groupIndex);

      void removeAllConstituents();

      std::vector<AirSupplyConstituent> constituents() const;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Node> airInletNode() const;

      boost::optional<CurveCubic> blowerPowerCurve() const;

      double blowerHeatLossFactor() const;

      std::string airSupplyRateCalculationMode() const;

      boost::optional<double> stoichiometricRatio() const;

      boost::optional<CurveQuadratic> airRateFunctionofElectricPowerCurve() const;

      boost::optional<double> airRateAirTemperatureCoefficient() const;

      boost::optional<CurveQuadratic> airRateFunctionofFuelRateCurve() const;

      std::string airIntakeHeatRecoveryMode() const;

      std::string airSupplyConstituentMode() const;

      unsigned int numberofUserDefinedConstituents() const;

      // Convenience function to check that it's equal to 1.0 (If no constituents, returns 0 and warns)
      double sumofConstituentsMolarFractions() const;

      // Return optional parent generator
      boost::optional<GeneratorFuelCell> fuelCell() const;

      //@}
      /** @name Setters */
      //@{

      bool setAirInletNode(const Node& connection);

      void resetAirInletNode();

      bool setBlowerPowerCurve(const CurveCubic& cubicCurves);

      void resetBlowerPowerCurve();

      bool setBlowerHeatLossFactor(double blowerHeatLossFactor);

      void resetBlowerHeatLossFactor();

      bool setAirSupplyRateCalculationMode(const std::string& airSupplyRateCalculationMode);

      bool setStoichiometricRatio(double stoichiometricRatio);

      void resetStoichiometricRatio();

      bool setAirRateFunctionofElectricPowerCurve(const CurveQuadratic& quadraticCurves);

      void resetAirRateFunctionofElectricPowerCurve();

      bool setAirRateAirTemperatureCoefficient(double airRateAirTemperatureCoefficient);

      void resetAirRateAirTemperatureCoefficient();

      bool setAirRateFunctionofFuelRateCurve(const CurveQuadratic& quadraticCurves);

      void resetAirRateFunctionofFuelRateCurve();

      bool setAirIntakeHeatRecoveryMode(const std::string& airIntakeHeatRecoveryMode);

      bool setAirSupplyConstituentMode(const std::string& airSupplyConstituentMode);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      // Not to be exposed publicly, handled automatically by add/removeConstituent
      bool setNumberofUserDefinedConstituents(unsigned int numberofUserDefinedConstituents);
      void resetNumberofUserDefinedConstituents();

      REGISTER_LOGGER("openstudio.model.GeneratorFuelCellAirSupply");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLAIRSUPPLY_IMPL_HPP
