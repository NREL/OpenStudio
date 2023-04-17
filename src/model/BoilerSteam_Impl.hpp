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

#ifndef MODEL_BOILERSTEAM_IMPL_HPP
#define MODEL_BOILERSTEAM_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** BoilerSteam_Impl is a StraightComponent_Impl that is the implementation class for BoilerSteam.*/
    class MODEL_API BoilerSteam_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      BoilerSteam_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      BoilerSteam_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      BoilerSteam_Impl(const BoilerSteam_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~BoilerSteam_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      std::string fuelType() const;

      boost::optional<double> maximumOperatingPressure() const;

      boost::optional<double> theoreticalEfficiency() const;

      boost::optional<double> designOutletSteamTemperature() const;

      boost::optional<double> nominalCapacity() const;

      bool isNominalCapacityAutosized() const;

      boost::optional<double> minimumPartLoadRatio() const;

      boost::optional<double> maximumPartLoadRatio() const;

      boost::optional<double> optimumPartLoadRatio() const;

      boost::optional<double> coefficient1ofFuelUseFunctionofPartLoadRatioCurve() const;

      boost::optional<double> coefficient2ofFuelUseFunctionofPartLoadRatioCurve() const;

      boost::optional<double> coefficient3ofFuelUseFunctionofPartLoadRatioCurve() const;

      double sizingFactor() const;

      bool isSizingFactorDefaulted() const;

      boost::optional<double> autosizedNominalCapacity() const;

      std::string endUseSubcategory() const;

      //@}
      /** @name Setters */
      //@{

      bool setFuelType(const std::string& fuelType);

      bool setMaximumOperatingPressure(boost::optional<double> maximumOperatingPressure);

      void resetMaximumOperatingPressure();

      bool setTheoreticalEfficiency(boost::optional<double> theoreticalEfficiency);

      void resetTheoreticalEfficiency();

      bool setDesignOutletSteamTemperature(boost::optional<double> designOutletSteamTemperature);

      void resetDesignOutletSteamTemperature();

      bool setNominalCapacity(boost::optional<double> nominalCapacity);

      void resetNominalCapacity();

      void autosizeNominalCapacity();

      bool setMinimumPartLoadRatio(boost::optional<double> minimumPartLoadRatio);

      void resetMinimumPartLoadRatio();

      bool setMaximumPartLoadRatio(boost::optional<double> maximumPartLoadRatio);

      void resetMaximumPartLoadRatio();

      bool setOptimumPartLoadRatio(boost::optional<double> optimumPartLoadRatio);

      void resetOptimumPartLoadRatio();

      bool setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(boost::optional<double> coefficient1ofFuelUseFunctionofPartLoadRatioCurve);

      void resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve();

      bool setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(boost::optional<double> coefficient2ofFuelUseFunctionofPartLoadRatioCurve);

      void resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve();

      bool setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(boost::optional<double> coefficient3ofFuelUseFunctionofPartLoadRatioCurve);

      void resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve();

      bool setSizingFactor(double sizingFactor);

      void resetSizingFactor();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.BoilerSteam");

      std::vector<std::string> fuelTypeValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_BOILERSTEAM_IMPL_HPP
