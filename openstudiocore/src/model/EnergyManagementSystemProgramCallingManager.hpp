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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMPROGRAMCALLINGMANAGER_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMPROGRAMCALLINGMANAGER_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"
#include "EnergyManagementSystemProgram.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class EnergyManagementSystemProgramCallingManager_Impl;

} // detail

/** EnergyManagementSystemProgramCallingManager is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:ProgramCallingManager'. */
class MODEL_API EnergyManagementSystemProgramCallingManager : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit EnergyManagementSystemProgramCallingManager(const Model& model);

  virtual ~EnergyManagementSystemProgramCallingManager() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validCallingPointValues();

  /** @name Getters */
  //@{

  std::string callingPoint() const;

  std::vector<EnergyManagementSystemProgram> programs() const;

  boost::optional<EnergyManagementSystemProgram> getProgram(unsigned index) const;

  //@}
  /** @name Setters */
  //@{

  bool setCallingPoint(const std::string& callingPoint);

  bool eraseProgram(unsigned index);

  void erasePrograms();

  bool addProgram(const EnergyManagementSystemProgram& program);

  bool setProgram(const EnergyManagementSystemProgram& program, unsigned index);

  bool setPrograms(const std::vector<EnergyManagementSystemProgram>& programs);


  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::EnergyManagementSystemProgramCallingManager_Impl ImplType;

  explicit EnergyManagementSystemProgramCallingManager(std::shared_ptr<detail::EnergyManagementSystemProgramCallingManager_Impl> impl);

  friend class detail::EnergyManagementSystemProgramCallingManager_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.EnergyManagementSystemProgramCallingManager");
};

/** \relates EnergyManagementSystemProgramCallingManager*/
typedef boost::optional<EnergyManagementSystemProgramCallingManager> OptionalEnergyManagementSystemProgramCallingManager;

/** \relates EnergyManagementSystemProgramCallingManager*/
typedef std::vector<EnergyManagementSystemProgramCallingManager> EnergyManagementSystemProgramCallingManagerVector;

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMPROGRAMCALLINGMANAGER_HPP

