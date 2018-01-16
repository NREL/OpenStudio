/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_SOLARCOLLECTORFLATPLATEPHOTOVOLTAICTHERMAL_HPP
#define MODEL_SOLARCOLLECTORFLATPLATEPHOTOVOLTAICTHERMAL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class PlanarSurface;
class GeneratorPhotovoltaic;
class SolarCollectorPerformancePhotovoltaicThermalSimple;

namespace detail {

  class SolarCollectorFlatPlatePhotovoltaicThermal_Impl;

} // detail

/** SolarCollectorFlatPlatePhotovoltaicThermal is a StraightComponent that wraps the OpenStudio IDD object 'OS:SolarCollector:FlatPlate:PhotovoltaicThermal'. */
class MODEL_API SolarCollectorFlatPlatePhotovoltaicThermal : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SolarCollectorFlatPlatePhotovoltaicThermal(const Model& model);

  virtual ~SolarCollectorFlatPlatePhotovoltaicThermal() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  SolarCollectorPerformancePhotovoltaicThermalSimple solarCollectorPerformance() const;

  boost::optional<PlanarSurface> surface() const;

  boost::optional<GeneratorPhotovoltaic> generatorPhotovoltaic() const;

  boost::optional<double> designFlowRate() const;

  bool isDesignFlowRateAutosized() const;

  //@}
  /** @name Setters */
  //@{

  /// Deletes the current parameters and clones the parameters passed in
  bool setSolarCollectorPerformance(const SolarCollectorPerformancePhotovoltaicThermalSimple& parameters);

  /// Deletes the current parameters and constructs a new default set of parameters
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

  boost::optional<double> autosizedDesignFlowRate() const ;



  //@}
 protected:

  /// @cond
  typedef detail::SolarCollectorFlatPlatePhotovoltaicThermal_Impl ImplType;

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
typedef boost::optional<SolarCollectorFlatPlatePhotovoltaicThermal> OptionalSolarCollectorFlatPlatePhotovoltaicThermal;

/** \relates SolarCollectorFlatPlatePhotovoltaicThermal*/
typedef std::vector<SolarCollectorFlatPlatePhotovoltaicThermal> SolarCollectorFlatPlatePhotovoltaicThermalVector;

} // model
} // openstudio

#endif // MODEL_SOLARCOLLECTORFLATPLATEPHOTOVOLTAICTHERMAL_HPP

