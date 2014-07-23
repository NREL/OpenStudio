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

#ifndef MODEL_CONNECTION_HPP
#define MODEL_CONNECTION_HPP

#include "ModelAPI.hpp"
#include "../utilities/core/Logger.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {
  class Connection_Impl;
} // detail

class MODEL_API Connection : public ModelObject {
 public:

  // constructor
  explicit Connection(const Model& model);

  virtual ~Connection() {}

  boost::optional<ModelObject> sourceObject();

  boost::optional<unsigned> sourceObjectPort();

  boost::optional<ModelObject> targetObject();

  boost::optional<unsigned> targetObjectPort();

  void setSourceObject(ModelObject object);

  void setSourceObjectPort(unsigned port);

  void setTargetObject(ModelObject object);

  void setTargetObjectPort(unsigned port);

  static IddObjectType iddObjectType();

 protected:

  typedef detail::Connection_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::ModelObject_Impl;

  // constructor
  explicit Connection(std::shared_ptr<detail::Connection_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.Connection");
};

/** \relates Connection */
typedef boost::optional<Connection> OptionalConnection;

/** \relates Connection */
typedef std::vector<Connection> ConnectionVector;

  } // model
} // openstudio

#endif // MODEL_CONNECTION_HPP
