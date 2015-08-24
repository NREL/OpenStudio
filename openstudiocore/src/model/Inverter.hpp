/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_INVERTER_HPP
#define MODEL_INVERTER_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class Inverter_Impl;
}

/** Inverter is the base class for inverters.
 */
class MODEL_API Inverter : public ParentObject {

  public:

  Inverter(IddObjectType type,const Model& model);

  virtual ~Inverter() {}

  protected:

  friend class Model;

  friend class openstudio::IdfObject;

  /// @cond 

  typedef detail::Inverter_Impl ImplType;

  explicit Inverter(std::shared_ptr<detail::Inverter_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.Inverter");

  /// @endcond 

};

typedef boost::optional<Inverter> OptionalInverter;

typedef std::vector<Inverter> InverterVector;

} // model
} // openstudio

#endif // MODEL_INVERTER_HPP

