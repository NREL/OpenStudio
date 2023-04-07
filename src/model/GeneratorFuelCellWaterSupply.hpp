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

#ifndef MODEL_GENERATORFUELCELLWATERSUPPLY_HPP
#define MODEL_GENERATORFUELCELLWATERSUPPLY_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class CurveQuadratic;
  class CurveCubic;
  class Node;
  class Schedule;
  class GeneratorFuelCell;

  namespace detail {

    class GeneratorFuelCellWaterSupply_Impl;

  }  // namespace detail

  /** GeneratorFuelCellWaterSupply is a ModelObject that wraps the OpenStudio IDD object 'OS:Generator:FuelCell:WaterSupply'. */
  class MODEL_API GeneratorFuelCellWaterSupply : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GeneratorFuelCellWaterSupply(const Model& model);

    explicit GeneratorFuelCellWaterSupply(const Model& model, const std::string& waterTempMode);

    explicit GeneratorFuelCellWaterSupply(const Model& model, const CurveQuadratic& flowRateCurve, const CurveCubic& pumpPowerCurve,
                                          Schedule tempSchedule);

    explicit GeneratorFuelCellWaterSupply(const Model& model, const CurveQuadratic& flowRateCurve, const CurveCubic& pumpPowerCurve,
                                          const Node& waterTempNode, const std::string& waterTempMode);

    virtual ~GeneratorFuelCellWaterSupply() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GeneratorFuelCellWaterSupply(const GeneratorFuelCellWaterSupply& other) = default;
    GeneratorFuelCellWaterSupply(GeneratorFuelCellWaterSupply&& other) = default;
    GeneratorFuelCellWaterSupply& operator=(const GeneratorFuelCellWaterSupply&) = default;
    GeneratorFuelCellWaterSupply& operator=(GeneratorFuelCellWaterSupply&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> waterTemperatureModelingModeValues();

    /** @name Getters */
    //@{

    CurveQuadratic reformerWaterFlowRateFunctionofFuelRateCurve() const;

    CurveCubic reformerWaterPumpPowerFunctionofFuelRateCurve() const;

    double pumpHeatLossFactor() const;

    std::string waterTemperatureModelingMode() const;

    boost::optional<Node> waterTemperatureReferenceNode() const;

    boost::optional<Schedule> waterTemperatureSchedule() const;

    // Return optional parent generator
    boost::optional<GeneratorFuelCell> fuelCell() const;

    //@}
    /** @name Setters */
    //@{

    bool setReformerWaterFlowRateFunctionofFuelRateCurve(const CurveQuadratic& quadraticCurves);

    bool setReformerWaterPumpPowerFunctionofFuelRateCurve(const CurveCubic& cubicCurves);

    bool setPumpHeatLossFactor(double pumpHeatLossFactor);

    void resetPumpHeatLossFactor();

    bool setWaterTemperatureModelingMode(const std::string& waterTemperatureModelingMode);

    void resetWaterTemperatureModelingMode();

    bool setWaterTemperatureReferenceNode(const Node& connection);

    void resetWaterTemperatureReferenceNode();

    bool setWaterTemperatureSchedule(Schedule& schedule);

    void resetWaterTemperatureSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::GeneratorFuelCellWaterSupply_Impl;

    explicit GeneratorFuelCellWaterSupply(std::shared_ptr<detail::GeneratorFuelCellWaterSupply_Impl> impl);

    friend class detail::GeneratorFuelCellWaterSupply_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorFuelCellWaterSupply");
  };

  /** \relates GeneratorFuelCellWaterSupply*/
  using OptionalGeneratorFuelCellWaterSupply = boost::optional<GeneratorFuelCellWaterSupply>;

  /** \relates GeneratorFuelCellWaterSupply*/
  using GeneratorFuelCellWaterSupplyVector = std::vector<GeneratorFuelCellWaterSupply>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLWATERSUPPLY_HPP
