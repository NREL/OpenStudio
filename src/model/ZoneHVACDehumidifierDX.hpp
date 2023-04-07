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

#ifndef MODEL_ZONEHVACDEHUMIDIFIERDX_HPP
#define MODEL_ZONEHVACDEHUMIDIFIERDX_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;
  // class WaterStorageTank;

  namespace detail {

    class ZoneHVACDehumidifierDX_Impl;

  }  // namespace detail

  /** ZoneHVACDehumidifierDX is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:Dehumidifier:DX'. */
  class MODEL_API ZoneHVACDehumidifierDX : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACDehumidifierDX(const Model& model);

    explicit ZoneHVACDehumidifierDX(const Model& model, const Curve& waterRemovalCurve, const Curve& energyFactorCurve,
                                    const Curve& partLoadFractionCurve);

    virtual ~ZoneHVACDehumidifierDX() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACDehumidifierDX(const ZoneHVACDehumidifierDX& other) = default;
    ZoneHVACDehumidifierDX(ZoneHVACDehumidifierDX&& other) = default;
    ZoneHVACDehumidifierDX& operator=(const ZoneHVACDehumidifierDX&) = default;
    ZoneHVACDehumidifierDX& operator=(ZoneHVACDehumidifierDX&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    double ratedWaterRemoval() const;

    double ratedEnergyFactor() const;

    double ratedAirFlowRate() const;

    Curve waterRemovalCurve() const;

    Curve energyFactorCurve() const;

    Curve partLoadFractionCorrelationCurve() const;

    double minimumDryBulbTemperatureforDehumidifierOperation() const;

    double maximumDryBulbTemperatureforDehumidifierOperation() const;

    double offCycleParasiticElectricLoad() const;

    // boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setRatedWaterRemoval(double ratedWaterRemoval);

    bool setRatedEnergyFactor(double ratedEnergyFactor);

    bool setRatedAirFlowRate(double ratedAirFlowRate);

    bool setWaterRemovalCurve(const Curve& curve);

    bool setEnergyFactorCurve(const Curve& curve);

    bool setPartLoadFractionCorrelationCurve(const Curve& curve);

    bool setMinimumDryBulbTemperatureforDehumidifierOperation(double minimumDryBulbTemperatureforDehumidifierOperation);

    bool setMaximumDryBulbTemperatureforDehumidifierOperation(double maximumDryBulbTemperatureforDehumidifierOperation);

    bool setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad);

    // bool setCondensateCollectionWaterStorageTank(const WaterStorageTank& waterStorageTank);

    // void resetCondensateCollectionWaterStorageTank();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACDehumidifierDX_Impl;

    explicit ZoneHVACDehumidifierDX(std::shared_ptr<detail::ZoneHVACDehumidifierDX_Impl> impl);

    friend class detail::ZoneHVACDehumidifierDX_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACDehumidifierDX");
  };

  /** \relates ZoneHVACDehumidifierDX*/
  using OptionalZoneHVACDehumidifierDX = boost::optional<ZoneHVACDehumidifierDX>;

  /** \relates ZoneHVACDehumidifierDX*/
  using ZoneHVACDehumidifierDXVector = std::vector<ZoneHVACDehumidifierDX>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACDEHUMIDIFIERDX_HPP
