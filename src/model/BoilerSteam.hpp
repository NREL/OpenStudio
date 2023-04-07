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

#ifndef MODEL_BOILERSTEAM_HPP
#define MODEL_BOILERSTEAM_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class BoilerSteam_Impl;

  }  // namespace detail

  /** BoilerSteam is a StraightComponent that wraps the OpenStudio IDD object 'OS:Boiler:Steam'. */
  class MODEL_API BoilerSteam : public StraightComponent
  {

   public:
    explicit BoilerSteam(const Model& model);

    virtual ~BoilerSteam() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    BoilerSteam(const BoilerSteam& other) = default;
    BoilerSteam(BoilerSteam&& other) = default;
    BoilerSteam& operator=(const BoilerSteam&) = default;
    BoilerSteam& operator=(BoilerSteam&&) = default;

    static IddObjectType iddObjectType();

    static std::vector<std::string> fuelTypeValues();

    /** @name Getters and Setters */
    //@{

    std::string fuelType() const;
    bool setFuelType(const std::string& fuelType);

    boost::optional<double> maximumOperatingPressure() const;
    bool setMaximumOperatingPressure(double maximumOperatingPressure);
    void resetMaximumOperatingPressure();

    boost::optional<double> theoreticalEfficiency() const;
    bool setTheoreticalEfficiency(double theoreticalEfficiency);
    void resetTheoreticalEfficiency();

    boost::optional<double> designOutletSteamTemperature() const;
    bool setDesignOutletSteamTemperature(double designOutletSteamTemperature);
    void resetDesignOutletSteamTemperature();

    boost::optional<double> nominalCapacity() const;
    bool isNominalCapacityAutosized() const;
    bool setNominalCapacity(double nominalCapacity);
    void resetNominalCapacity();
    void autosizeNominalCapacity();

    boost::optional<double> minimumPartLoadRatio() const;
    bool setMinimumPartLoadRatio(double minimumPartLoadRatio);
    void resetMinimumPartLoadRatio();

    boost::optional<double> maximumPartLoadRatio() const;
    bool setMaximumPartLoadRatio(double maximumPartLoadRatio);
    void resetMaximumPartLoadRatio();

    boost::optional<double> optimumPartLoadRatio() const;
    bool setOptimumPartLoadRatio(double optimumPartLoadRatio);
    void resetOptimumPartLoadRatio();

    boost::optional<double> coefficient1ofFuelUseFunctionofPartLoadRatioCurve() const;
    bool setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(double coefficient1ofFuelUseFunctionofPartLoadRatioCurve);
    void resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve();

    boost::optional<double> coefficient2ofFuelUseFunctionofPartLoadRatioCurve() const;
    bool setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(double coefficient2ofFuelUseFunctionofPartLoadRatioCurve);
    void resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve();

    boost::optional<double> coefficient3ofFuelUseFunctionofPartLoadRatioCurve() const;
    bool setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(double coefficient3ofFuelUseFunctionofPartLoadRatioCurve);
    void resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve();

    double sizingFactor() const;
    bool isSizingFactorDefaulted() const;
    bool setSizingFactor(double sizingFactor);
    void resetSizingFactor();

    std::string endUseSubcategory() const;
    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    //@}
    /** @name Other */
    //@{

    //@}

    boost::optional<double> autosizedNominalCapacity() const;

   protected:
    /// @cond
    using ImplType = detail::BoilerSteam_Impl;

    explicit BoilerSteam(std::shared_ptr<detail::BoilerSteam_Impl> impl);

    friend class detail::BoilerSteam_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.BoilerSteam");
  };

  /** \relates BoilerSteam*/
  using OptionalBoilerSteam = boost::optional<BoilerSteam>;

  /** \relates BoilerSteam*/
  using BoilerSteamVector = std::vector<BoilerSteam>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_BOILERSTEAM_HPP
