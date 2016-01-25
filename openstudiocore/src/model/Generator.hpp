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

#ifndef MODEL_GENERATOR_HPP
#define MODEL_GENERATOR_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {
namespace model {

class ElectricLoadCenterDistribution;

namespace detail{
  class Generator_Impl;
}

/** Generator is the base class for generators.  The ratedElectricPowerOutput, availabilitySchedule, and
 *  ratedThermalToElectricalPowerRatio fields are mapped to fields in the ElectricLoadCenter:Generators object
 *  in EnergyPlus.  The ElectricLoadCenter:Generators object does not exist in OpenStudio.
 */
class MODEL_API Generator : public ParentObject {

  public:

  Generator(IddObjectType type,const Model& model);

  virtual ~Generator() {}

  std::string generatorObjectType() const;
  
  boost::optional<double> ratedElectricPowerOutput() const;

  boost::optional<Schedule> availabilitySchedule() const;

  boost::optional<double> ratedThermalToElectricalPowerRatio() const;

  boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

  protected:

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class detail::Generator_Impl;

  /// @cond 

  typedef detail::Generator_Impl ImplType;

  explicit Generator(std::shared_ptr<detail::Generator_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.Generator");

  /// @endcond 

};

typedef boost::optional<Generator> OptionalGenerator;

typedef std::vector<Generator> GeneratorVector;

} // model
} // openstudio

#endif // MODEL_GENERATOR_HPP

