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

#ifndef MODEL_SETPOINTMANAGER_HPP
#define MODEL_SETPOINTMANAGER_HPP

#include <model/ModelAPI.hpp>
#include <model/HVACComponent.hpp>

namespace openstudio {

namespace model {

namespace detail {

class SetpointManager_Impl;

}

 /** SetpointManager is the base class for setpoint managers.
 *  
 */
class MODEL_API SetpointManager : public HVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~SetpointManager() {}

  //@}

  /** @name Getters */
  //@{

  /** Returns the Node referred to by the SetpointNodeName field. **/
  boost::optional<Node> setpointNode() const;

  /** Returns the Control Variable **/
  std::string controlVariable() const;

  //@}
  /** @name Setters */
  //@{

  /** Sets the Control Variable **/
  bool setControlVariable(const std::string& controlVariable);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:

  /** Constructs a new SetpointManager object and places it inside
   *  model. */
  SetpointManager(IddObjectType type, const Model& model);

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class detail::SetpointManager_Impl;

  /// @cond
  typedef detail::SetpointManager_Impl ImplType;

  explicit SetpointManager(std::shared_ptr<detail::SetpointManager_Impl> impl);

 private:
  REGISTER_LOGGER("openstudio.model.SetpointManager");
  /// @endcond
};

typedef boost::optional<SetpointManager> OptionalSetpointManager;
typedef std::vector<SetpointManager> SetpointManagerVector;

} // model

} // openstudio

#endif // MODEL_SETPOINTMANAGER_HPP

