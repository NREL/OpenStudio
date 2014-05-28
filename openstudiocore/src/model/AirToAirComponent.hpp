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

#ifndef MODEL_AIRTOAIRCOMPONENT_HPP
#define MODEL_AIRTOAIRCOMPONENT_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class AirToAirComponent_Impl;
}

class AirLoopHVAC;

/** AirToAirComponent is the base class for model objects which interact with 
 *  two air streams.  
 *
 *  These objects are typically heat recovery devices such as the EnergyPlus IDD object
 *  named HeatExchanger:AirToAir:SensibleAndLatent.
 */
class MODEL_API AirToAirComponent : public HVACComponent
{
  public:

  AirToAirComponent(IddObjectType type,const Model& model);

  virtual ~AirToAirComponent() {}

  /** Returns the primary air stream inlet port.
   *
   *  In the conext of air to air heat recovery devices in EnergyPlus,
   *  the primary air stream is often thought of as the supply side air stream
   *  which is the supply of outdoor air into the system.
   */
  unsigned primaryAirInletPort();

  /** Returns the primary air stream outlet port. **/
  unsigned primaryAirOutletPort();

  /** Returns the optional ModelObject connected to the primary air stream inlet port. **/
  boost::optional<ModelObject> primaryAirInletModelObject();

  /** Returns the optional ModelObject connected to the primary air stream outlet port. **/
  boost::optional<ModelObject> primaryAirOutletModelObject();

  /** Returns the secondary air stream inlet port.
   *
   *  In the context of air to air heat recovery devices in EnergyPlus,
   *  the secondary air stream is often the exhaust air stream leaving the system.
   */
  unsigned secondaryAirInletPort();

  /** Returns the secondary air stream outlet port. **/
  unsigned secondaryAirOutletPort();

  /** Returns the optional ModelObject connected to the secondary air stream inlet port. **/
  boost::optional<ModelObject> secondaryAirInletModelObject();

  /** Returns the optional ModelObject connected to the secondary air stream outlet port. **/
  boost::optional<ModelObject> secondaryAirOutletModelObject();

  bool addToNode(Node & node);

  std::vector<openstudio::IdfObject> remove();

  ModelObject clone(Model model) const;

  protected:

  friend class Model;

  friend class openstudio::IdfObject;

  /// @cond 

  typedef detail::AirToAirComponent_Impl ImplType;

  explicit AirToAirComponent(std::shared_ptr<detail::AirToAirComponent_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.AirToAirComponent");

  /// @endcond

};

typedef boost::optional<AirToAirComponent> OptionalAirToAirComponent;

} // model
} // openstudio

#endif // MODEL_AIRTOAIRCOMPONENT_HPP

