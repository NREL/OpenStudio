/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_WATERTOWATERCOMPONENT_HPP
#define MODEL_WATERTOWATERCOMPONENT_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

  class AirLoopHVAC;

  namespace detail {
    class WaterToWaterComponent_Impl;
  }

  /** WaterToWaterComponent is the base class for HVACComponent objects which interact with two water loops.  A WaterToWaterComponent
 *  has two inlet nodes and two outlet ports.  One pair of inlet/outlet ports is designated as the supply side and the other pair
 *  is designated as the demand side.
 */
  class MODEL_API WaterToWaterComponent : public HVACComponent
  {
   public:
    WaterToWaterComponent(IddObjectType type, const Model& model);

    virtual ~WaterToWaterComponent() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    WaterToWaterComponent(const WaterToWaterComponent& other) = default;
    WaterToWaterComponent(WaterToWaterComponent&& other) = default;
    WaterToWaterComponent& operator=(const WaterToWaterComponent&) = default;
    WaterToWaterComponent& operator=(WaterToWaterComponent&&) = default;

    /** Returns the supply inlet port. **/
    virtual unsigned supplyInletPort() const;

    /** Returns the supply outlet port. **/
    virtual unsigned supplyOutletPort() const;

    /** Returns the demand inlet port. **/
    virtual unsigned demandInletPort() const;

    /** Returns the demand outlet port. **/
    virtual unsigned demandOutletPort() const;

    /** Returns the optional ModelObject connected to the supply inlet port. **/
    virtual boost::optional<ModelObject> supplyInletModelObject() const;

    /** Returns the optional ModelObject connected to the supply outlet port. **/
    virtual boost::optional<ModelObject> supplyOutletModelObject() const;

    /** Returns the optional ModelObject connected to the demand inlet port. **/
    virtual boost::optional<ModelObject> demandInletModelObject() const;

    /** Returns the optional ModelObject connected to the demand outlet port. **/
    virtual boost::optional<ModelObject> demandOutletModelObject() const;

    virtual bool addToNode(Node& node);

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

    /** Tertiary plant loop applies to a limited number of WaterToWaterComponent types that
   *  interact with three plant systems.  One example is a water cooled chiller with heat recovery,
   *  which can be connected to a chilled water, condenser, and heat recovery plant simultaneously.
   */
    boost::optional<PlantLoop> tertiaryPlantLoop() const;

    /** Removes the component from the tertiaryPlantLoop if one is attached.
   *  Repairs the tertiaryPlantLoop connections preserving the integrity of the loop.
   */
    bool removeFromTertiaryPlantLoop();

    /** Add this WaterToWaterComponent to a tertiaryPlantLoop by making connections to the
   *  tertiary inlet and outlet.
   */
    virtual bool addToTertiaryNode(Node& node);

    /** Returns the optional ModelObject connected to the tertiary inlet. **/
    boost::optional<ModelObject> tertiaryInletModelObject() const;

    /** Returns the optional ModelObject connected to the tertiary outlet. **/
    boost::optional<ModelObject> tertiaryOutletModelObject() const;

    void disconnect();

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::WaterToWaterComponent_Impl;

    explicit WaterToWaterComponent(std::shared_ptr<detail::WaterToWaterComponent_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.WaterToWaterComponent");

    /// @endcond
  };

  using OptionalWaterToWaterComponent = boost::optional<WaterToWaterComponent>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_WATERTOWATERCOMPONENT_HPP
