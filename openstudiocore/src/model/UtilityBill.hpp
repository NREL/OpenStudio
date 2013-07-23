/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_UTILITYBILL_HPP
#define MODEL_UTILITYBILL_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject.hpp>

namespace openstudio {

class FuelType;
class InstallLocationType;
class EndUseCategoryType;

namespace model {

namespace detail {

  class UtilityBill_Impl;

} // detail

/** UtilityBill is a ModelObject that wraps the OpenStudio IDD object 'OS:UtilityBill'. 
    This object allows the user to enter in actual fuel usage for model calibration.
*/
class MODEL_API UtilityBill : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit UtilityBill(const FuelType& fuelType, const Model& model);

  virtual ~UtilityBill() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> consumptionUnitValues();

  static std::vector<std::string> peakDemandUnitValues();

  /** @name Getters */
  //@{

  FuelType fuelType() const;

  InstallLocationType meterInstallLocation() const;

  bool isMeterInstallLocationDefaulted() const;

  boost::optional<std::string> meterSpecificInstallLocation() const;

  boost::optional<EndUseCategoryType> meterEndUse() const;

  boost::optional<std::string> meterSpecificEndUse() const;

  std::string consumptionUnit() const;

  double consumptionUnitConversionFactor() const;

  boost::optional<std::string> peakDemandUnit() const;

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Setters */
  //@{

  bool setMeterInstallLocation(const InstallLocationType& meterInstallLocation);

  void resetMeterInstallLocation();

  bool setMeterSpecificInstallLocation(const std::string& meterSpecificInstallLocation);

  void resetMeterSpecificInstallLocation();

  bool setMeterEndUse(const EndUseCategoryType& meterEndUse);

  void resetMeterEndUse();

  bool setMeterSpecificEndUse(const std::string& meterSpecificEndUse);

  void resetMeterSpecificEndUse();

  bool setConsumptionUnit(const std::string& consumptionUnit);

  bool setConsumptionUnitConversionFactor(double consumptionUnitConversionFactor);

  bool setPeakDemandUnit(const std::string& peakDemandUnit);

  void resetPeakDemandUnit();

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::UtilityBill_Impl ImplType;

  explicit UtilityBill(boost::shared_ptr<detail::UtilityBill_Impl> impl);

  friend class detail::UtilityBill_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.UtilityBill");
};

/** \relates UtilityBill*/
typedef boost::optional<UtilityBill> OptionalUtilityBill;

/** \relates UtilityBill*/
typedef std::vector<UtilityBill> UtilityBillVector;

} // model
} // openstudio

#endif // MODEL_UTILITYBILL_HPP

