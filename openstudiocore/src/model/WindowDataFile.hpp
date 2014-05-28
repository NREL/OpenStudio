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

#ifndef MODEL_WINDOWDATAFILE_HPP
#define MODEL_WINDOWDATAFILE_HPP

#include "ModelAPI.hpp"
#include "ConstructionBase.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class WindowDataFile_Impl;
}

/** WindowDataFile is a ConstructionBase that wraps EnergyPlus IDD object 
 *  'Construction:WindowDataFile'. WindowDataFile is an alternative to Construction that isFenestration. */
class MODEL_API WindowDataFile : public ConstructionBase {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit WindowDataFile(const Model& model);

  virtual ~WindowDataFile() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
 protected:
  /// @cond
  typedef detail::WindowDataFile_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::WindowDataFile_Impl;

  explicit WindowDataFile(std::shared_ptr<detail::WindowDataFile_Impl> impl);  

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.WindowDataFile");
};

/** \relates WindowDataFile */
typedef boost::optional<WindowDataFile> OptionalWindowDataFile;

/** \relates WindowDataFile */
typedef std::vector<WindowDataFile> WindowDataFileVector;

} // model
} // openstudio

#endif // MODEL_WINDOWDATAFILE_HPP
