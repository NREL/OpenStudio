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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMPROGRAM_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMPROGRAM_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class EnergyManagementSystemProgram_Impl;

} // detail

/** EnergyManagementSystemProgram is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:Program'. */
class MODEL_API EnergyManagementSystemProgram : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit EnergyManagementSystemProgram(const Model& model);

  virtual ~EnergyManagementSystemProgram() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  std::string body() const;

  std::vector<std::string> lines() const;

  std::vector<ModelObject> referencedObjects() const;

  std::vector<std::string> invalidReferencedObjects() const;

  //@}
  /** @name Setters */
  //@{

  bool setBody(const std::string& body);

  bool resetBody();

  bool addLine(const std::string& body);

  bool setLines(const std::vector<std::string>& body);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::EnergyManagementSystemProgram_Impl ImplType;

  explicit EnergyManagementSystemProgram(std::shared_ptr<detail::EnergyManagementSystemProgram_Impl> impl);

  friend class detail::EnergyManagementSystemProgram_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.EnergyManagementSystemProgram");
};

/** \relates EnergyManagementSystemProgram*/
typedef boost::optional<EnergyManagementSystemProgram> OptionalEnergyManagementSystemProgram;

/** \relates EnergyManagementSystemProgram*/
typedef std::vector<EnergyManagementSystemProgram> EnergyManagementSystemProgramVector;

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMPROGRAM_HPP

