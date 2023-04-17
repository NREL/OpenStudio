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

#ifndef MODEL_SOLARCOLLECTORFLATPLATEPHOTOVOLTAICTHERMAL_HPP
#define MODEL_SOLARCOLLECTORFLATPLATEPHOTOVOLTAICTHERMAL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class PlanarSurface;
  class GeneratorPhotovoltaic;
  class SolarCollectorPerformancePhotovoltaicThermalBIPVT;
  class SolarCollectorPerformancePhotovoltaicThermalSimple;

  namespace detail {

    class SolarCollectorFlatPlatePhotovoltaicThermal_Impl;

  }  // namespace detail

  /** SolarCollectorFlatPlatePhotovoltaicThermal is a StraightComponent that wraps the OpenStudio IDD object 'OS:SolarCollector:FlatPlate:PhotovoltaicThermal'. */
  class MODEL_API SolarCollectorFlatPlatePhotovoltaicThermal : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SolarCollectorFlatPlatePhotovoltaicThermal(const Model& model);
    explicit SolarCollectorFlatPlatePhotovoltaicThermal(const SolarCollectorPerformancePhotovoltaicThermalBIPVT& performance);
    explicit SolarCollectorFlatPlatePhotovoltaicThermal(const SolarCollectorPerformancePhotovoltaicThermalSimple& performance);

    virtual ~SolarCollectorFlatPlatePhotovoltaicThermal() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SolarCollectorFlatPlatePhotovoltaicThermal(const SolarCollectorFlatPlatePhotovoltaicThermal& other) = default;
    SolarCollectorFlatPlatePhotovoltaicThermal(SolarCollectorFlatPlatePhotovoltaicThermal&& other) = default;
    SolarCollectorFlatPlatePhotovoltaicThermal& operator=(const SolarCollectorFlatPlatePhotovoltaicThermal&) = default;
    SolarCollectorFlatPlatePhotovoltaicThermal& operator=(SolarCollectorFlatPlatePhotovoltaicThermal&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ModelObject solarCollectorPerformance() const;

    boost::optional<PlanarSurface> surface() const;

    boost::optional<GeneratorPhotovoltaic> generatorPhotovoltaic() const;

    boost::optional<double> designFlowRate() const;

    bool isDesignFlowRateAutosized() const;

    //@}
    /** @name Setters */
    //@{

    /// Deletes the current performance and clones the performance passed in
    bool setSolarCollectorPerformance(const SolarCollectorPerformancePhotovoltaicThermalBIPVT& performance);
    bool setSolarCollectorPerformance(const SolarCollectorPerformancePhotovoltaicThermalSimple& performance);

    /// Deletes the current performance and constructs a new default set of performance
    void resetSolarCollectorPerformance();

    bool setSurface(const PlanarSurface& surface);

    void resetSurface();

    bool setGeneratorPhotovoltaic(const GeneratorPhotovoltaic& generatorPhotovoltaic);

    void resetGeneratorPhotovoltaic();

    bool setDesignFlowRate(double designFlowRate);

    void resetDesignFlowRate();

    void autosizeDesignFlowRate();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedDesignFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl;

    explicit SolarCollectorFlatPlatePhotovoltaicThermal(std::shared_ptr<detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl> impl);

    friend class detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SolarCollectorFlatPlatePhotovoltaicThermal");
  };

  /** \relates SolarCollectorFlatPlatePhotovoltaicThermal*/
  using OptionalSolarCollectorFlatPlatePhotovoltaicThermal = boost::optional<SolarCollectorFlatPlatePhotovoltaicThermal>;

  /** \relates SolarCollectorFlatPlatePhotovoltaicThermal*/
  using SolarCollectorFlatPlatePhotovoltaicThermalVector = std::vector<SolarCollectorFlatPlatePhotovoltaicThermal>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORFLATPLATEPHOTOVOLTAICTHERMAL_HPP
