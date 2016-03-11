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

#ifndef MODEL_GENERATORPHOTOVOLTAIC_HPP
#define MODEL_GENERATORPHOTOVOLTAIC_HPP

#include "ModelAPI.hpp"
#include "Generator.hpp"

namespace openstudio {

namespace model {

class PlanarSurface;
class PhotovoltaicPerformance;
class Schedule;

namespace detail {

  class GeneratorPhotovoltaic_Impl;

} // detail

/** GeneratorPhotovoltaic is a Generator that wraps the OpenStudio IDD object 'OS:Generator:Photovoltaic'. */
class MODEL_API GeneratorPhotovoltaic : public Generator {
 public:
  /** @name Constructors and Destructors */
  //@{

  /// Creates a GeneratorPhotovoltaic object with PhotovoltaicPerformanceSimple performance
  static GeneratorPhotovoltaic simple(const Model& model);

  /// Creates a GeneratorPhotovoltaic object with PhotovoltaicPerformanceEquivalentOneDiode performance
  static GeneratorPhotovoltaic equivalentOneDiode(const Model& model);

  virtual ~GeneratorPhotovoltaic() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> heatTransferIntegrationModeValues();

  /** @name Getters */
  //@{

  boost::optional<PlanarSurface> surface() const;

  PhotovoltaicPerformance photovoltaicPerformance() const;

  std::string heatTransferIntegrationMode() const;

  bool isHeatTransferIntegrationModeDefaulted() const;

  double numberOfModulesInParallel() const;

  bool isNumberOfModulesInParallelDefaulted() const;

  double numberOfModulesInSeries() const;

  bool isNumberOfModulesInSeriesDefaulted() const;

  boost::optional<double> ratedElectricPowerOutput() const;

  boost::optional<Schedule> availabilitySchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setSurface(const PlanarSurface& surface);

  void resetSurface();

  bool setHeatTransferIntegrationMode(std::string heatTransferIntegrationMode);

  void resetHeatTransferIntegrationMode();

  bool setNumberOfModulesInParallel(double numberOfModulesInParallel);

  void resetNumberOfModulesInParallel();

  bool setNumberOfModulesInSeries(double numberOfModulesInSeries);

  void resetNumberOfModulesInSeries();

  void setRatedElectricPowerOutput(double ratedElectricPowerOutput);

  void resetRatedElectricPowerOutput();

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::GeneratorPhotovoltaic_Impl ImplType;

  GeneratorPhotovoltaic(const Model& model, const PhotovoltaicPerformance& performance);

  explicit GeneratorPhotovoltaic(std::shared_ptr<detail::GeneratorPhotovoltaic_Impl> impl);

  friend class detail::GeneratorPhotovoltaic_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.GeneratorPhotovoltaic");
};

/** \relates GeneratorPhotovoltaic*/
typedef boost::optional<GeneratorPhotovoltaic> OptionalGeneratorPhotovoltaic;

/** \relates GeneratorPhotovoltaic*/
typedef std::vector<GeneratorPhotovoltaic> GeneratorPhotovoltaicVector;

} // model
} // openstudio

#endif // MODEL_GENERATORPHOTOVOLTAIC_HPP

