/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_TIMEDEPENDENTVALUATION_HPP
#define MODEL_TIMEDEPENDENTVALUATION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

#include "../utilities/filetypes/TimeDependentValuationFile.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

class Site;

namespace detail {
  class TimeDependentValuation_Impl;
} // detail

/** TimeDependentValuation is a unique ModelObject that wraps OpenStudio IDD object 
 *  'OS:TimeDependentValuation'. TimeDependentValuation is statically constructed from 
 *  a TimeDependentValuationFile (see the core utilities documentation), and provides custom 
 *  weightings of a building's energy use (total or by fuel type) on an 8760 basis. The file format
 *  is based on the California Energy Commission's time dependent valuation files. */
class MODEL_API TimeDependentValuation : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  static TimeDependentValuation setTimeDependentValuation(
      Model& model,const TimeDependentValuationFile& tdvFile);

  virtual ~TimeDependentValuation() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  /** Load from disk or return cached TDV file. Caches on successful load. */
  boost::optional<TimeDependentValuationFile> file(const openstudio::path& dir=openstudio::path()) const;

  /** Convert file() to SI units, or return cached converted file. Caches on successful
   *  conversion. */
  boost::optional<TimeDependentValuationFile> fileInSIUnits() const;

  /** Get the Site, if it exists. */
  boost::optional<Site> site() const;

  /** Get the url string, if it exists. */
  boost::optional<std::string> url() const;

  /** Get the path, if it exists. */
  boost::optional<openstudio::path> path() const;

  /** Time dependent valuation files specify different weightings for Commercial and Residential 
   *  buildings. Active building sector defines which set of weightings are used to compute
   *  energyTimeDependentValuation, etc. */
  BuildingSector activeBuildingSector() const;

  /** Return the FuelTypes for which time dependent valuations for activeBuildingSector() are 
   *  available. */
  std::vector<FuelType> availableFuelTypes() const;

  //@}
  /** @name Setters */
  //@{

  /** Save the url as relative to basePath, or just keep the file name and extension if 
   *  basePath.empty(). */
  bool makeUrlRelative(const openstudio::path& basePath=openstudio::path());

  /** Search for file and save the absolute url if successful. */
  bool makeUrlAbsolute(const openstudio::path& searchDirectory);

  /** Set the active building sector. Time dependent valuation files specify different weightings 
   *  for Commercial and Residential buildings. Active building sector defines which set of 
   *  weightings are used to compute energyTimeDependentValuation, etc. */
  void setActiveBuildingSector(const BuildingSector& sector);

  //@}
  /** @name Queries */
  //@{

  /** Get the total energy time dependent valuation (J). Requires model().sqlFile() and file(). 
   *  If environmentPeriod.empty(), will only work if 
   *  model().getUniqueObject<SimulationControl>().annualSimulationEnvironmentNames().size() == 1.
   */
  boost::optional<double> energyTimeDependentValuation(
      const std::string& environmentPeriod=std::string()) const;

  /** Get the total cost time dependent valuation (J). Requires model().sqlFile() and file().
   *  If environmentPeriod.empty(), will only work if 
   *  model().getUniqueObject<SimulationControl>().annualSimulationEnvironmentNames().size() == 1.
   */
  boost::optional<double> costTimeDependentValuation(
      const std::string& environmentPeriod=std::string()) const;

  /** Get the energy time dependent valuation for fuelType (J). Requires model().sqlFile() and 
   *  file(). */
  boost::optional<double> getEnergyTimeDependentValuation(
      const FuelType& fuelType,const std::string& environmentPeriod=std::string()) const;

  /** Get the cost time dependent valuation for fuelType ($).  Requires model().sqlFile() and 
   *  file(). */
  boost::optional<double> getCostTimeDependentValuation(
      const FuelType& fuelType,const std::string& environmentPeriod=std::string()) const;

  //@}
 protected:
  /// @cond
  typedef detail::TimeDependentValuation_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  // constructor
  explicit TimeDependentValuation(const Model& model);

  // constructor
  explicit TimeDependentValuation(std::shared_ptr<detail::TimeDependentValuation_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.TimeDependentValuation");
};

/** \relates TimeDependentValuation */
typedef boost::optional<TimeDependentValuation> OptionalTimeDependentValuation;

} // model
} // openstudio

#endif // MODEL_TIMEDEPENDENTVALUATION_HPP
