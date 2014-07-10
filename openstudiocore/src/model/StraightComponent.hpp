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

#ifndef MODEL_STRAIGHTCOMPONENT_HPP
#define MODEL_STRAIGHTCOMPONENT_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

class AirLoopHVAC;

namespace detail{
  class StraightComponent_Impl;
}

/** StraightComponent is the base class for HVACComponent objects which have precisely one inlet port and one outlet port.
 *
 *  A StraighComponent may appear on either an AirLoopHVAC or a PlantLoop.
 */
class MODEL_API StraightComponent : public HVACComponent {

  public:

  StraightComponent(IddObjectType type,const Model& model);

  virtual ~StraightComponent() {}

  std::vector<openstudio::IdfObject> remove();

  /** Returns the inlet port. **/
   unsigned inletPort();

  /** Returns the outlet port. **/
   unsigned outletPort();

  /** Returns the optional ModelObject connected to the inlet port. **/
   boost::optional<ModelObject> inletModelObject();

  /** Returns the optional ModelObject connected to the outlet port. **/
   boost::optional<ModelObject> outletModelObject();

  /** Returns the optional AirLoopHVAC object that this AirToAirComponent is attached to.  
   *  
   *  Reimplemented from HVACComponent.
   */
  boost::optional<AirLoopHVAC> airLoopHVAC() const;

  bool addToNode(Node & node);

  ModelObject clone(Model model) const;

  void disconnect();

  protected:

  friend class Model;

  friend class openstudio::IdfObject;

  /// @cond 

  typedef detail::StraightComponent_Impl ImplType;

  explicit StraightComponent(std::shared_ptr<detail::StraightComponent_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.StraightComponent");

  /// @endcond 

};

typedef boost::optional<StraightComponent> OptionalStraightComponent;

typedef std::vector<StraightComponent> StraightComponentVector;

} // model
} // openstudio

#endif // MODEL_STRAIGHTCOMPONENT_HPP

