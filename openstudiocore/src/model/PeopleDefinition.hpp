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

#ifndef MODEL_PEOPLEDEFINITION_HPP
#define MODEL_PEOPLEDEFINITION_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class PeopleDefinition_Impl;

} // detail

/** PeopleDefinition is a SpaceLoadDefinition that wraps the OpenStudio IDD object 
 *  'OS_People_Definition'. Its fields are derived from the EnergyPlus IDD object 'People'. 
 *  \sa People
 */
class MODEL_API PeopleDefinition : public SpaceLoadDefinition {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PeopleDefinition(const Model& model);

  virtual ~PeopleDefinition() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> numberofPeopleCalculationMethodValues();

  static std::vector<std::string> meanRadiantTemperatureCalculationTypeValues();

  static std::vector<std::string> thermalComfortModelTypeValues();

  /** @name Getters */
  //@{

  std::string numberofPeopleCalculationMethod() const;

  boost::optional<double> numberofPeople() const;

  boost::optional<double> peopleperSpaceFloorArea() const;

  boost::optional<double> spaceFloorAreaperPerson() const;

  double fractionRadiant() const;

  boost::optional<double> sensibleHeatFraction() const;

  bool isSensibleHeatFractionDefaulted() const;

  bool isSensibleHeatFractionAutocalculated() const;

  double carbonDioxideGenerationRate() const;

  bool isCarbonDioxideGenerationRateDefaulted() const;

  bool enableASHRAE55ComfortWarnings() const;

  bool isEnableASHRAE55ComfortWarningsDefaulted() const;

  std::string meanRadiantTemperatureCalculationType() const;

  bool isMeanRadiantTemperatureCalculationTypeDefaulted() const;

  /** Return the thermal comfort model type in extensible group i. Indexing starts at 0, and
   *  i should be less than numThermalComfortModelTypes. */
  boost::optional<std::string> getThermalComfortModelType(int i) const;

  /** Returns the number of thermal comfort model types registered in this object. */
  int numThermalComfortModelTypes() const;

  //@}
  /** @name Setters */
  //@{

  /** Also sets numberofPeopleCalculationMethod accordingly. */
  bool setNumberofPeople(double numberofPeople);

  /** Also sets numberofPeopleCalculationMethod accordingly. */
  bool setPeopleperSpaceFloorArea(double peopleperSpaceFloorArea);

  /** Also sets numberofPeopleCalculationMethod accordingly. */
  bool setSpaceFloorAreaperPerson(double spaceFloorAreaperPerson);

  bool setFractionRadiant(double fractionRadiant);

  bool setSensibleHeatFraction(double sensibleHeatFraction);

  void resetSensibleHeatFraction();

  void autocalculateSensibleHeatFraction();

  bool setCarbonDioxideGenerationRate(double carbonDioxideGenerationRate);

  void resetCarbonDioxideGenerationRate();

  void setEnableASHRAE55ComfortWarnings(bool enableASHRAE55ComfortWarnings);

  void resetEnableASHRAE55ComfortWarnings();

  bool setMeanRadiantTemperatureCalculationType(std::string meanRadiantTemperatureCalculationType);

  void resetMeanRadiantTemperatureCalculationType();

  bool pushThermalComfortModelType(const std::string& thermalComfortModelType);

  bool setThermalComfortModelType(int i, const std::string& thermalComfortModelType);

  bool eraseThermalComfortModelType(int i);

  //@}
  /** @name Other */
  //@{

  /** Returns the number of people represented by this definition, assuming floorArea (m^2). */
  double getNumberOfPeople(double floorArea) const;

  /** Returns the people/m^2 represented by this definition, assuming floorArea (m^2). */
  double getPeoplePerFloorArea(double floorArea) const;

  /** Returns the m^2/person represented by this definition, assuming floorArea (m^2). */
  double getFloorAreaPerPerson(double floorArea) const;

  /** If method is a \link numberofPeopleCalculationMethodValues valid number of people 
   *  calculation method \endlink, changes this definition to an equivalent number of people, 
   *  under the assumption of floorArea (m^2). */
  bool setNumberOfPeopleCalculationMethod(const std::string& method,double floorArea);

  //@}
 protected:
  /// @cond
  typedef detail::PeopleDefinition_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit PeopleDefinition(std::shared_ptr<detail::PeopleDefinition_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.PeopleDefinition");
};

/** \relates PeopleDefinition*/
typedef boost::optional<PeopleDefinition> OptionalPeopleDefinition;

/** \relates PeopleDefinition*/
typedef std::vector<PeopleDefinition> PeopleDefinitionVector;

} // model
} // openstudio

#endif // MODEL_PEOPLEDEFINITION_HPP

