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

#ifndef MODEL_WATERTOAIRCOMPONENT_HPP
#define MODEL_WATERTOAIRCOMPONENT_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

namespace detail{
  class WaterToAirComponent_Impl;
}

class AirLoopHVAC;

class PlantLoop;

/** WaterToAirComponent is the base class for model objects which interact with 
 *  an air stream and a water stream.  
 *
 *  These objects are typically water coils such as the EnergyPlus IDD objects
 *  named Coil:Heating:Water and Coil:Cooling:Water.
 */
class MODEL_API WaterToAirComponent : public HVACComponent
{
  public:
  virtual ~WaterToAirComponent() {}

  /** Returns the air inlet port. **/ 
  virtual unsigned airInletPort();

  /** Returns the air outlet port. **/
  virtual unsigned airOutletPort();

  /** Returns the optional ModelObject connected to the air inlet port. **/
  virtual boost::optional<ModelObject> airInletModelObject();

  /** Returns the optional ModelObject connected to the air outlet port. **/
  virtual boost::optional<ModelObject> airOutletModelObject();

  /** Returns the water inlet port. **/
  virtual unsigned waterInletPort();

  /** Returns the water outlet port. **/
  virtual unsigned waterOutletPort();

  /** Returns the optional ModelObject connected to the water inlet port. **/
  virtual boost::optional<ModelObject> waterInletModelObject();

  /** Returns the optional ModelObject connected to the water outlet port. **/
  virtual boost::optional<ModelObject> waterOutletModelObject();

  virtual boost::optional<AirLoopHVAC> airLoopHVAC() const;

  virtual boost::optional<PlantLoop> plantLoop() const;

  virtual bool addToNode(Node & node);

  virtual bool addToSplitter(Splitter & splitter);

  virtual std::vector<openstudio::IdfObject> remove();

  virtual ModelObject clone(Model model) const;

  void disconnectWaterSide();

  void disconnectAirSide();

  /** Removes the component from the airLoopHVAC if one is attached.
   *  Repairs the airLoopHVAC connections preserving the integrity of the loop.
   *  Does not remove the component from the model
   */
  bool removeFromAirLoopHVAC();

  /** Removes the component from the plantLoop if one is attached.
   *  Repairs the plantLoop connections preserving the integrity of the loop.
   *  Does not remove the component from the model
   */
  bool removeFromPlantLoop();

  protected:

  friend class Model;

  friend class openstudio::IdfObject;

  /// @cond 

  typedef detail::WaterToAirComponent_Impl ImplType;

  explicit WaterToAirComponent(std::shared_ptr<detail::WaterToAirComponent_Impl> impl);

  WaterToAirComponent(IddObjectType type,const Model& model);

  private:

  REGISTER_LOGGER("openstudio.model.WaterToAirComponent");

  /// @endcond

};

typedef boost::optional<WaterToAirComponent> OptionalWaterToAirComponent;

} // model

} // openstudio

#endif // MODEL_WATERTOAIRCOMPONENT_HPP

