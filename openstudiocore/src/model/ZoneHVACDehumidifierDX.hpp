/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

} // detail

/** ZoneHVACDehumidifierDX is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:Dehumidifier:DX'. */
class MODEL_API ZoneHVACDehumidifierDX : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneHVACDehumidifierDX(const Model& model);

  explicit ZoneHVACDehumidifierDX(
    const Model& model,
    const Curve& waterRemovalCurve,
    const Curve& energyFactorCurve,
    const Curve& partLoadFractionCurve
  );

  virtual ~ZoneHVACDehumidifierDX() {}

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

  void setMinimumDryBulbTemperatureforDehumidifierOperation(double minimumDryBulbTemperatureforDehumidifierOperation);

  void setMaximumDryBulbTemperatureforDehumidifierOperation(double maximumDryBulbTemperatureforDehumidifierOperation);

  bool setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad);

  // bool setCondensateCollectionWaterStorageTank(const WaterStorageTank& waterStorageTank);

  // void resetCondensateCollectionWaterStorageTank();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACDehumidifierDX_Impl ImplType;

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
typedef boost::optional<ZoneHVACDehumidifierDX> OptionalZoneHVACDehumidifierDX;

/** \relates ZoneHVACDehumidifierDX*/
typedef std::vector<ZoneHVACDehumidifierDX> ZoneHVACDehumidifierDXVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACDEHUMIDIFIERDX_HPP

