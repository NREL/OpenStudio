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

#ifndef MODEL_PROGRAMCONTROL_HPP
#define MODEL_PROGRAMCONTROL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class ProgramControl_Impl;

} // detail

/** ProgramControl is a ModelObject that wraps the OpenStudio IDD object 'OS:ProgramControl'. */
class MODEL_API ProgramControl : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ProgramControl(const Model& model);

  virtual ~ProgramControl() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<int> numberofThreadsAllowed() const;

  //@}
  /** @name Setters */
  //@{

  bool setNumberofThreadsAllowed(int numberofThreadsAllowed);

  void resetNumberofThreadsAllowed();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ProgramControl_Impl ImplType;

  explicit ProgramControl(std::shared_ptr<detail::ProgramControl_Impl> impl);

  friend class detail::ProgramControl_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ProgramControl");
};

/** \relates ProgramControl*/
typedef boost::optional<ProgramControl> OptionalProgramControl;

/** \relates ProgramControl*/
typedef std::vector<ProgramControl> ProgramControlVector;

} // model
} // openstudio

#endif // MODEL_PROGRAMCONTROL_HPP

