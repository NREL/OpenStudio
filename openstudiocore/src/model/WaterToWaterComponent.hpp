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

#ifndef MODEL_WATERTOWATERCOMPONENT_HPP
#define MODEL_WATERTOWATERCOMPONENT_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

class AirLoopHVAC;

namespace detail{
  class WaterToWaterComponent_Impl;
}

/** WaterToWaterComponent is the base class for HVACComponent objects which interact with two water loops.  A WaterToWaterComponent
 *  has two inlet nodes and two outlet ports.  One pair of inlet/outlet ports is designated as the supply side and the other pair
 *  is designated as the demand side.
 */
class MODEL_API WaterToWaterComponent : public HVACComponent
{
  public:

  WaterToWaterComponent(IddObjectType type,const Model& model);


  virtual ~WaterToWaterComponent() {}

  /** Returns the supply inlet port. **/
  virtual unsigned supplyInletPort();

  /** Returns the supply outlet port. **/
  virtual unsigned supplyOutletPort();

  /** Returns the demand inlet port. **/
  virtual unsigned demandInletPort();

  /** Returns the demand outlet port. **/
  virtual unsigned demandOutletPort();

  /** Returns the optional ModelObject connected to the supply inlet port. **/
  virtual boost::optional<ModelObject> supplyInletModelObject();

  /** Returns the optional ModelObject connected to the supply outlet port. **/
  virtual boost::optional<ModelObject> supplyOutletModelObject();

  /** Returns the optional ModelObject connected to the demand inlet port. **/
  virtual boost::optional<ModelObject> demandInletModelObject();

  /** Returns the optional ModelObject connected to the demand outlet port. **/
  virtual boost::optional<ModelObject> demandOutletModelObject();

  virtual bool addToNode(Node & node);

  virtual std::vector<openstudio::IdfObject> remove();

  virtual ModelObject clone(Model model) const;

  /** Returns the optional PlantLoop object that the HVAC component is a supply component on.
   */
  boost::optional<PlantLoop> plantLoop() const;

  /** Returns the optional PlantLoop object that the HVAC component is a demand component on.
   */
  boost::optional<PlantLoop> secondaryPlantLoop() const;

  /** Removes the component from the plantLoop if one is attached.
   *  Repairs the plantLoop connections preserving the integrity of the loop.
   */
  bool removeFromPlantLoop();

  /** Removes the component from the secondaryPlantLoop if one is attached.
   *  Repairs the secondaryPlantLoop connections preserving the integrity of the loop.
   */
  bool removeFromSecondaryPlantLoop();

  void disconnect();

  protected:

  friend class Model;

  friend class openstudio::IdfObject;

  /// @cond 

  typedef detail::WaterToWaterComponent_Impl ImplType;

  explicit WaterToWaterComponent(std::shared_ptr<detail::WaterToWaterComponent_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.WaterToWaterComponent");

  /// @endcond 

};

typedef boost::optional<WaterToWaterComponent> OptionalWaterToWaterComponent;

} // model

} // openstudio

#endif // MODEL_WATERTOWATERCOMPONENT_HPP

