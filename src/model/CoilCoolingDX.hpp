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

#ifndef MODEL_COILCOOLINGDX_HPP
#define MODEL_COILCOOLINGDX_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ThermalZone;
  class CoilCoolingDXCurveFitPerformance;

  namespace detail {

    class CoilCoolingDX_Impl;

  }  // namespace detail

  /** CoilCoolingDX is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX'. */
  class MODEL_API CoilCoolingDX : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingDX(const Model& model, const CoilCoolingDXCurveFitPerformance& coilCoolingDXCurveFitPerformance);

    virtual ~CoilCoolingDX() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDX(const CoilCoolingDX& other) = default;
    CoilCoolingDX(CoilCoolingDX&& other) = default;
    CoilCoolingDX& operator=(const CoilCoolingDX&) = default;
    CoilCoolingDX& operator=(CoilCoolingDX&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<ThermalZone> condenserZone() const;

    CoilCoolingDXCurveFitPerformance performanceObject() const;

    // boost::optional<HVACComponent> condensateCollectionWaterStorageTank() const;
    // boost::optional<HVACComponent> evaporativeCondenserSupplyWaterStorageTank() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    // TODO: As of E+ 9.3.0, this field appears to have no effect.
    bool setCondenserZone(const ThermalZone& thermalZone);
    void resetCondenserZone();

    // TODO: Eventually provide an API to set these Nodes once E+ is done implementing support for it (as of 9.3.0 it's not the case)
    // boost::optional<std::string> condenserInletNodeName() const;
    // bool setCondenserInletNodeName(const std::string& condenserInletNodeName);
    // void resetCondenserInletNodeName();

    // boost::optional<std::string> condenserOutletNodeName() const;
    // bool setCondenserOutletNodeName(const std::string& condenserOutletNodeName);
    // void resetCondenserOutletNodeName();

    bool setPerformanceObject(const CoilCoolingDXCurveFitPerformance& coilCoolingDXCurveFitPerformance);

    // bool setCondensateCollectionWaterStorageTank(const HVACComponent& condensateCollectionWaterStorageTank);
    // void resetCondensateCollectionWaterStorageTank();

    // bool setEvaporativeCondenserSupplyWaterStorageTank(const HVACComponent& evaporativeCondenserSupplyWaterStorageTank);
    // void resetEvaporativeCondenserSupplyWaterStorageTank();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingDX_Impl;

    explicit CoilCoolingDX(std::shared_ptr<detail::CoilCoolingDX_Impl> impl);

    friend class detail::CoilCoolingDX_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDX");
  };

  /** \relates CoilCoolingDX*/
  using OptionalCoilCoolingDX = boost::optional<CoilCoolingDX>;

  /** \relates CoilCoolingDX*/
  using CoilCoolingDXVector = std::vector<CoilCoolingDX>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDX_HPP
