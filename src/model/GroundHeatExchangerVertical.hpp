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

#ifndef MODEL_GROUNDHEATEXCHANGERVERTICAL_HPP
#define MODEL_GROUNDHEATEXCHANGERVERTICAL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {
  namespace detail {

    class GroundHeatExchangerVertical_Impl;

  }  // namespace detail

  class MODEL_API GFunction
  {

   public:
    GFunction(double lnValue, double gValue);
    double lnValue() const;
    double gValue() const;

   private:
    double m_lnValue;
    double m_gValue;
  };

  /** GroundHeatExchangerVertical is a StraightComponent that wraps the OpenStudio IDD object 'OS:GroundHeatExchanger:Vertical'. */
  class MODEL_API GroundHeatExchangerVertical : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GroundHeatExchangerVertical(const Model& model);

    virtual ~GroundHeatExchangerVertical() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GroundHeatExchangerVertical(const GroundHeatExchangerVertical& other) = default;
    GroundHeatExchangerVertical(GroundHeatExchangerVertical&& other) = default;
    GroundHeatExchangerVertical& operator=(const GroundHeatExchangerVertical&) = default;
    GroundHeatExchangerVertical& operator=(GroundHeatExchangerVertical&&) = default;

    //@}

    static IddObjectType iddObjectType();

    bool addGFunction(double gFunctionLN, double gFunctionGValue);

    bool addGFunction(GFunction gFunc);

    void removeGFunction(int groupIndex);

    void removeAllGFunctions();

    std::vector<GFunction> gFunctions();

    /** @name Getters */
    //@{

    boost::optional<double> designFlowRate() const;

    boost::optional<int> numberofBoreHoles() const;

    boost::optional<double> boreHoleLength() const;

    boost::optional<double> boreHoleRadius() const;

    boost::optional<double> groundThermalConductivity() const;

    boost::optional<double> groundThermalHeatCapacity() const;

    boost::optional<double> groundTemperature() const;

    boost::optional<double> groutThermalConductivity() const;

    boost::optional<double> pipeThermalConductivity() const;

    boost::optional<double> pipeOutDiameter() const;

    boost::optional<double> uTubeDistance() const;

    boost::optional<double> pipeThickness() const;

    boost::optional<double> maximumLengthofSimulation() const;

    double gFunctionReferenceRatio() const;

    bool isGFunctionReferenceRatioDefaulted() const;

    //@}
    /** @name Setters */

    //@{

    bool setDesignFlowRate(double designFlowRate);

    bool setNumberofBoreHoles(int numberofBoreHoles);

    void resetNumberofBoreHoles();

    bool setBoreHoleLength(double boreHoleLength);

    void resetBoreHoleLength();

    bool setBoreHoleRadius(double boreHoleRadius);

    void resetBoreHoleRadius();

    bool setGroundThermalConductivity(double groundThermalConductivity);

    void resetGroundThermalConductivity();

    bool setGroundThermalHeatCapacity(double groundThermalHeatCapacity);

    void resetGroundThermalHeatCapacity();

    bool setGroundTemperature(double groundTemperature);

    void resetGroundTemperature();

    bool setGroutThermalConductivity(double groutThermalConductivity);

    void resetGroutThermalConductivity();

    bool setPipeThermalConductivity(double pipeThermalConductivity);

    void resetPipeThermalConductivity();

    bool setPipeOutDiameter(double pipeOutDiameter);

    void resetPipeOutDiameter();

    bool setUTubeDistance(double uTubeDistance);

    void resetUTubeDistance();

    bool setPipeThickness(double pipeThickness);

    void resetPipeThickness();

    bool setMaximumLengthofSimulation(double maximumLengthofSimulation);

    void resetMaximumLengthofSimulation();

    bool setGFunctionReferenceRatio(double gFunctionReferenceRatio);

    void resetGFunctionReferenceRatio();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::GroundHeatExchangerVertical_Impl;

    explicit GroundHeatExchangerVertical(std::shared_ptr<detail::GroundHeatExchangerVertical_Impl> impl);

    friend class detail::GroundHeatExchangerVertical_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GroundHeatExchangerVertical");
  };

  /** \relates GroundHeatExchangerVertical*/
  using OptionalGroundHeatExchangerVertical = boost::optional<GroundHeatExchangerVertical>;

  /** \relates GroundHeatExchangerVertical*/
  using GroundHeatExchangerVerticalVector = std::vector<GroundHeatExchangerVertical>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GROUNDHEATEXCHANGERVERTICAL_HPP
