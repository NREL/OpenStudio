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

#ifndef MODEL_SOLARCOLLECTORPERFORMANCEFLATPLATE_HPP
#define MODEL_SOLARCOLLECTORPERFORMANCEFLATPLATE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class SolarCollectorPerformanceFlatPlate;

  namespace detail {

    class SolarCollectorFlatPlateWater_Impl;
    class SolarCollectorPerformanceFlatPlate_Impl;

  }  // namespace detail

  /** SolarCollectorPerformanceFlatPlate is a ModelObject that wraps the OpenStudio IDD object 'OS:SolarCollectorPerformance:FlatPlate'. */
  class MODEL_API SolarCollectorPerformanceFlatPlate : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~SolarCollectorPerformanceFlatPlate() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SolarCollectorPerformanceFlatPlate(const SolarCollectorPerformanceFlatPlate& other) = default;
    SolarCollectorPerformanceFlatPlate(SolarCollectorPerformanceFlatPlate&& other) = default;
    SolarCollectorPerformanceFlatPlate& operator=(const SolarCollectorPerformanceFlatPlate&) = default;
    SolarCollectorPerformanceFlatPlate& operator=(SolarCollectorPerformanceFlatPlate&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> testFluidValues();

    static std::vector<std::string> testCorrelationTypeValues();

    /** @name Getters */
    //@{

    double grossArea() const;

    std::string testFluid() const;

    bool isTestFluidDefaulted() const;

    double testFlowRate() const;

    std::string testCorrelationType() const;

    double coefficient1ofEfficiencyEquation() const;

    double coefficient2ofEfficiencyEquation() const;

    boost::optional<double> coefficient3ofEfficiencyEquation() const;

    boost::optional<double> coefficient2ofIncidentAngleModifier() const;

    boost::optional<double> coefficient3ofIncidentAngleModifier() const;

    //@}
    /** @name Setters */
    //@{

    bool setGrossArea(double grossArea);

    bool setTestFluid(const std::string& testFluid);

    void resetTestFluid();

    bool setTestFlowRate(double testFlowRate);

    bool setTestCorrelationType(const std::string& testCorrelationType);

    bool setCoefficient1ofEfficiencyEquation(double coefficient1ofEfficiencyEquation);

    bool setCoefficient2ofEfficiencyEquation(double coefficient2ofEfficiencyEquation);

    bool setCoefficient3ofEfficiencyEquation(double coefficient3ofEfficiencyEquation);

    void resetCoefficient3ofEfficiencyEquation();

    bool setCoefficient2ofIncidentAngleModifier(double coefficient2ofIncidentAngleModifier);

    void resetCoefficient2ofIncidentAngleModifier();

    bool setCoefficient3ofIncidentAngleModifier(double coefficient3ofIncidentAngleModifier);

    void resetCoefficient3ofIncidentAngleModifier();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    explicit SolarCollectorPerformanceFlatPlate(const Model& model);

    /// @cond
    using ImplType = detail::SolarCollectorPerformanceFlatPlate_Impl;

    explicit SolarCollectorPerformanceFlatPlate(std::shared_ptr<detail::SolarCollectorPerformanceFlatPlate_Impl> impl);

    friend class detail::SolarCollectorPerformanceFlatPlate_Impl;
    friend class detail::SolarCollectorFlatPlateWater_Impl;
    friend class SolarCollectorFlatPlateWater;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SolarCollectorPerformanceFlatPlate");
  };

  /** \relates SolarCollectorPerformanceFlatPlate*/
  using OptionalSolarCollectorPerformanceFlatPlate = boost::optional<SolarCollectorPerformanceFlatPlate>;

  /** \relates SolarCollectorPerformanceFlatPlate*/
  using SolarCollectorPerformanceFlatPlateVector = std::vector<SolarCollectorPerformanceFlatPlate>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORPERFORMANCEFLATPLATE_HPP
