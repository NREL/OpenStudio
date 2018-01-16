/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

