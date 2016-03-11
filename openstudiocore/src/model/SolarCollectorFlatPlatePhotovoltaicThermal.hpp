/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

