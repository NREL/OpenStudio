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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMMETEREDOUTPUTVARIABLE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMMETEREDOUTPUTVARIABLE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"
#include "EnergyManagementSystemProgram.hpp"
#include "EnergyManagementSystemSubroutine.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class EnergyManagementSystemMeteredOutputVariable_Impl;

} // detail

/** EnergyManagementSystemMeteredOutputVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:MeteredOutputVariable'. */
class MODEL_API EnergyManagementSystemMeteredOutputVariable : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit EnergyManagementSystemMeteredOutputVariable(const Model& model);

  virtual ~EnergyManagementSystemMeteredOutputVariable() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> updateFrequencyValues();

  static std::vector<std::string> resourceTypeValues();

  static std::vector<std::string> groupTypeValues();

  static std::vector<std::string> endUseCategoryValues();

  /** @name Getters */
  //@{

  std::string eMSVariableName() const;

  std::string updateFrequency() const;

  boost::optional<std::string> eMSProgramorSubroutineName() const;

  std::string resourceType() const;

  std::string groupType() const;

  std::string endUseCategory() const;

  boost::optional<std::string> endUseSubcategory() const;

  boost::optional<std::string> units() const;

  //@}
  /** @name Setters */
  //@{

  bool setEMSVariableName(const std::string& eMSVariableName);

  bool setUpdateFrequency(const std::string& updateFrequency);

  bool setEMSProgramorSubroutineName(const EnergyManagementSystemProgram& program);

  bool setEMSProgramorSubroutineName(const EnergyManagementSystemSubroutine& subroutine);

  void resetEMSProgramorSubroutineName();

  bool setResourceType(const std::string& resourceType);

  bool setGroupType(const std::string& groupType);

  bool setEndUseCategory(const std::string& endUseCategory);

  bool setEndUseSubcategory(const std::string& endUseSubcategory);

  void resetEndUseSubcategory();

  bool setUnits(const std::string& units);

  void resetUnits();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::EnergyManagementSystemMeteredOutputVariable_Impl ImplType;

  explicit EnergyManagementSystemMeteredOutputVariable(std::shared_ptr<detail::EnergyManagementSystemMeteredOutputVariable_Impl> impl);

  friend class detail::EnergyManagementSystemMeteredOutputVariable_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.EnergyManagementSystemMeteredOutputVariable");
};

/** \relates EnergyManagementSystemMeteredOutputVariable*/
typedef boost::optional<EnergyManagementSystemMeteredOutputVariable> OptionalEnergyManagementSystemMeteredOutputVariable;

/** \relates EnergyManagementSystemMeteredOutputVariable*/
typedef std::vector<EnergyManagementSystemMeteredOutputVariable> EnergyManagementSystemMeteredOutputVariableVector;

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMMETEREDOUTPUTVARIABLE_HPP

