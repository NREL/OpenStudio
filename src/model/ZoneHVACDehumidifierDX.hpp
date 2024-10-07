/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~ZoneHVACDehumidifierDX() override = default;
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
