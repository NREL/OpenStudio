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

#ifndef MODEL_ELECTRICLOADCENTERDISTRIBUTION_HPP
#define MODEL_ELECTRICLOADCENTERDISTRIBUTION_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

class Schedule;
class Generator;
class Inverter;
//class ElectricalStorage;
//class Transformer;

namespace detail {

  class ElectricLoadCenterDistribution_Impl;

} // detail

/** ElectricLoadCenterDistribution is a ParentObject that wraps the OpenStudio IDD object 'OS:ElectricLoadCenter:Distribution'. 
 *  There is no ElectricLoadCenter:Generators object in OpenStudio.  Instead, the ElectricLoadCenterDistribution object references
 *  a model object list which references all of the associated generators.  The rated electric power, availability schedule, and thermal
 *  to electric power ratio fields are contained in the generator objects themselves rather than the ElectricLoadCenter:Generators 
 *  as in EnergyPlus. Electric storage and transformers will be added at a later date.
*/
class MODEL_API ElectricLoadCenterDistribution : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ElectricLoadCenterDistribution(const Model& model);

  virtual ~ElectricLoadCenterDistribution() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> generatorOperationSchemeTypeValues();

  static std::vector<std::string> electricalBussTypeValues();

  /** @name Getters */
  //@{

  /// Returns associated generators, note there is no ElectricLoadCenter:Generators object in OpenStudio.
  std::vector<Generator> generators() const;

  std::string generatorOperationSchemeType() const;

  bool isGeneratorOperationSchemeTypeDefaulted() const;

  //boost::optional<double> demandLimitSchemePurchasedElectricDemandLimit() const;

  //boost::optional<Schedule> trackScheduleSchemeSchedule() const;

  //boost::optional<std::string> trackMeterSchemeMeterName() const;

  std::string electricalBussType() const;

  bool isElectricalBussTypeDefaulted() const;

  boost::optional<Inverter> inverter() const;

  //boost::optional<ElectricalStorage> electricalStorage() const;

  //boost::optional<Transformer> transformer() const;

  //@}
  /** @name Setters */
  //@{

  bool addGenerator(const Generator& generator);

  bool removeGenerator(const Generator& generator);

  void resetGenerators();

  bool setGeneratorOperationSchemeType(const std::string& generatorOperationSchemeType);

  void resetGeneratorOperationSchemeType();

  //void setDemandLimitSchemePurchasedElectricDemandLimit(double demandLimitSchemePurchasedElectricDemandLimit);

  //void resetDemandLimitSchemePurchasedElectricDemandLimit();

  //bool setTrackScheduleSchemeSchedule(Schedule& schedule);

  //void resetTrackScheduleSchemeSchedule();

  //bool setTrackMeterSchemeMeterName(const std::string& trackMeterSchemeMeterName);

  //void resetTrackMeterSchemeMeterName();

  //bool setElectricalBussType(const std::string& electricalBussType);

  //void resetElectricalBussType();

  bool setInverter(const Inverter& inverter);

  void resetInverter();

  //bool setElectricalStorage(const ElectricalStorage& electricalStorage);

  //void resetElectricalStorage();

  //bool setTransformer(const Transformer& transformer);

  //void resetTransformer();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:

  /// @cond
  typedef detail::ElectricLoadCenterDistribution_Impl ImplType;

  explicit ElectricLoadCenterDistribution(std::shared_ptr<detail::ElectricLoadCenterDistribution_Impl> impl);

  friend class detail::ElectricLoadCenterDistribution_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ElectricLoadCenterDistribution");
};

/** \relates ElectricLoadCenterDistribution*/
typedef boost::optional<ElectricLoadCenterDistribution> OptionalElectricLoadCenterDistribution;

/** \relates ElectricLoadCenterDistribution*/
typedef std::vector<ElectricLoadCenterDistribution> ElectricLoadCenterDistributionVector;

} // model
} // openstudio

#endif // MODEL_ELECTRICLOADCENTERDISTRIBUTION_HPP

