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

#ifndef MODEL_OUTPUTVARIABLE_HPP
#define MODEL_OUTPUTVARIABLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class ReportingFrequency;

namespace model {

class Schedule;

namespace detail {
  class OutputVariable_Impl;
} // detail

/** OutputVariable derives from ModelObject and is an interface to the OpenStudio
 *  IDD object named "OS:Output:Variable".
 *
 *  OutputVariable is used to request output data from EnergyPlus simulations.  Each ModelObject defines
 *  a list of available output variable names which can be used to create OutputVariable requests.  An 
 *  OutputVariable request can apply to all objects (e.g. if the 'Key Value' field is set to '*') or only 
 *  a particular object (e.g. if the 'Key Value' field is set to an object's name).  If an OutputVariable object
 *  refers to a particular ModelObject then it is parented by that ModelObject, otherwise it is unparented.
 *  If an OutputVariable object is present when a Model is simulated in EnergyPlus, the OutputVariable can be 
 *  used to access simulation results for its parent ModelObject.
 */
class MODEL_API OutputVariable : public ModelObject {
 public:
  virtual ~OutputVariable() {}

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new OutputVariable object in the model.
  explicit OutputVariable(const std::string& variableName, const Model& model);

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  std::string keyValue() const;

  bool isKeyValueDefaulted() const;

  std::string variableName() const;

  std::string reportingFrequency() const;

  bool isReportingFrequencyDefaulted() const;

  boost::optional<Schedule> schedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setKeyValue(const std::string& keyValue);

  void resetKeyValue();

  bool setVariableName(const std::string& variableName);

  bool setReportingFrequency(const std::string& reportingFrequency);

  void resetReportingFrequency();

  bool setSchedule(Schedule& schedule);

  void resetSchedule();

  //@}
 protected:

  /// @cond 

  typedef detail::OutputVariable_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  // constructor
  explicit OutputVariable(std::shared_ptr<detail::OutputVariable_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.OutputVariable");

  /// @endcond

};

/** \relates OutputVariable */
typedef boost::optional<OutputVariable> OptionalOutputVariable;

/** \relates OutputVariable */
typedef std::vector<OutputVariable> OutputVariableVector;

} // model
} // openstudio

#endif // MODEL_VARIABLE_HPP
