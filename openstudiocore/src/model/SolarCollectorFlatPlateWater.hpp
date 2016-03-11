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

#ifndef MODEL_SOLARCOLLECTORFLATPLATEWATER_HPP
#define MODEL_SOLARCOLLECTORFLATPLATEWATER_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class SolarCollectorPerformanceFlatPlate;
class PlanarSurface;

namespace detail {

  class SolarCollectorFlatPlateWater_Impl;

} // detail

/** SolarCollectorFlatPlateWater is a StraightComponent that wraps the OpenStudio IDD object 'OS:SolarCollector:FlatPlate:Water'. */
class MODEL_API SolarCollectorFlatPlateWater : public StraightComponent {
  
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SolarCollectorFlatPlateWater(const Model& model);

  virtual ~SolarCollectorFlatPlateWater() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  SolarCollectorPerformanceFlatPlate solarCollectorPerformance() const;

  boost::optional<PlanarSurface> surface() const;

  boost::optional<double> maximumFlowRate() const;

  //@}
  /** @name Setters */
  //@{

  /// Deletes the current parameters and clones the parameters passed in
  bool setSolarCollectorPerformance(const SolarCollectorPerformanceFlatPlate& parameters);
  
  /// Deletes the current parameters and constructs a new default set of parameters
  void resetSolarCollectorPerformance();
  
  bool setSurface(const PlanarSurface& surface);

  void resetSurface();

  //bool setInletNode(const Connection& connection);

  //bool setOutletNode(const Connection& connection);

  bool setMaximumFlowRate(double maximumFlowRate);

  void resetMaximumFlowRate();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SolarCollectorFlatPlateWater_Impl ImplType;

  explicit SolarCollectorFlatPlateWater(std::shared_ptr<detail::SolarCollectorFlatPlateWater_Impl> impl);
  
  friend class detail::SolarCollectorFlatPlateWater_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SolarCollectorFlatPlateWater");
};

/** \relates SolarCollectorFlatPlateWater*/
typedef boost::optional<SolarCollectorFlatPlateWater> OptionalSolarCollectorFlatPlateWater;

/** \relates SolarCollectorFlatPlateWater*/
typedef std::vector<SolarCollectorFlatPlateWater> SolarCollectorFlatPlateWaterVector;

} // model
} // openstudio

#endif // MODEL_SOLARCOLLECTORFLATPLATEWATER_HPP

