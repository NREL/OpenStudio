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

#ifndef MODEL_VERSION_HPP
#define MODEL_VERSION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {
namespace detail {
  class Version_Impl;
}

class MODEL_API Version : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //{

  virtual ~Version() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //{

  std::string versionIdentifier() const;

  //@}
 protected:
  /** @name Setters */
  //@{

  void setVersionIdentifier(const std::string& s);

  //@}

  // constructor
  explicit Version(const Model& model);

  typedef detail::Version_Impl ImplType;

  friend class detail::Model_Impl;
  friend class openstudio::IdfObject;
  friend class Model;

  // constructor
  explicit Version(std::shared_ptr<detail::Version_Impl> impl);

 private:
  REGISTER_LOGGER("openstudio.model.RunPeriod");
};

/** \relates Version */
typedef boost::optional<Version> OptionalVersion;

}
}

#endif
