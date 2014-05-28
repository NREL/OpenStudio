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

#ifndef MODEL_REFRIGERATIONCONDENSERCASCADE_HPP
#define MODEL_REFRIGERATIONCONDENSERCASCADE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class RefrigerationCondenserCascade_Impl;

} // detail

/** RefrigerationCondenserCascade is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:Condenser:Cascade'. */
class MODEL_API RefrigerationCondenserCascade : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefrigerationCondenserCascade(const Model& model);

  virtual ~RefrigerationCondenserCascade() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> condensingTemperatureControlTypeValues();

  /** @name Getters */
  //@{

  double ratedCondensingTemperature() const;

  double ratedApproachTemperatureDifference() const;

  bool isRatedApproachTemperatureDifferenceDefaulted() const;

  double ratedEffectiveTotalHeatRejectionRate() const;

  std::string condensingTemperatureControlType() const;

  bool isCondensingTemperatureControlTypeDefaulted() const;

  boost::optional<double> condenserRefrigerantOperatingChargeInventory() const;

  boost::optional<double> condensateReceiverRefrigerantInventory() const;

  boost::optional<double> condensatePipingRefrigerantInventory() const;

  //@}
  /** @name Setters */
  //@{

  void setRatedCondensingTemperature(double ratedCondensingTemperature);

  bool setRatedApproachTemperatureDifference(double ratedApproachTemperatureDifference);

  void resetRatedApproachTemperatureDifference();

  bool setRatedEffectiveTotalHeatRejectionRate(double ratedEffectiveTotalHeatRejectionRate);

  bool setCondensingTemperatureControlType(std::string condensingTemperatureControlType);

  void resetCondensingTemperatureControlType();

  void setCondenserRefrigerantOperatingChargeInventory(double condenserRefrigerantOperatingChargeInventory);

  void resetCondenserRefrigerantOperatingChargeInventory();

  void setCondensateReceiverRefrigerantInventory(double condensateReceiverRefrigerantInventory);

  void resetCondensateReceiverRefrigerantInventory();

  void setCondensatePipingRefrigerantInventory(double condensatePipingRefrigerantInventory);

  void resetCondensatePipingRefrigerantInventory();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RefrigerationCondenserCascade_Impl ImplType;

  explicit RefrigerationCondenserCascade(std::shared_ptr<detail::RefrigerationCondenserCascade_Impl> impl);

  friend class detail::RefrigerationCondenserCascade_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RefrigerationCondenserCascade");
};

/** \relates RefrigerationCondenserCascade*/
typedef boost::optional<RefrigerationCondenserCascade> OptionalRefrigerationCondenserCascade;

/** \relates RefrigerationCondenserCascade*/
typedef std::vector<RefrigerationCondenserCascade> RefrigerationCondenserCascadeVector;

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONCONDENSERCASCADE_HPP

