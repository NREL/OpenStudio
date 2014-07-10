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

#ifndef MODEL_HEATBALANCEALGORITHM_HPP
#define MODEL_HEATBALANCEALGORITHM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class HeatBalanceAlgorithm_Impl;

} // detail

/** HeatBalanceAlgorithm derives from ModelObject and is an interface to the OpenStudio IDD object named "HeatBalanceAlgorithm".
 *
 *  HeatBalanceAlgorithm is a unique object that specifies the global heat and moisture algorithms
 *  used across building construction calculations. HeatBalanceAlgorithm does not have a public constructor because it is a unique ModelObject.  
 *  To get the HeatBalanceAlgorithm object for a Model or create one if it does not yet exist use model.getUniqueObject<HeatBalanceAlgorithm>().  
 *  To get the HeatBalanceAlgorithm object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<HeatBalanceAlgorithm>().
 */
class MODEL_API HeatBalanceAlgorithm : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~HeatBalanceAlgorithm() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //static std::vector<std::string> validAlgorithmValues(); TODO delete
  static std::vector<std::string> algorithmValues();

  //@}
  /** @name Getters */
  //@{

  std::string algorithm() const;

  bool isAlgorithmDefaulted() const;

  double surfaceTemperatureUpperLimit() const;

  Quantity getSurfaceTemperatureUpperLimit(bool returnIP=false) const;

  bool isSurfaceTemperatureUpperLimitDefaulted() const;

  double minimumSurfaceConvectionHeatTransferCoefficientValue() const;

  Quantity getMinimumSurfaceConvectionHeatTransferCoefficientValue(bool returnIP=false) const;

  bool isMinimumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const;

  double maximumSurfaceConvectionHeatTransferCoefficientValue() const;

  Quantity getMaximumSurfaceConvectionHeatTransferCoefficientValue(bool returnIP=false) const;

  bool isMaximumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setAlgorithm(std::string algorithm);

  void resetAlgorithm();

  bool setSurfaceTemperatureUpperLimit(double surfaceTemperatureUpperLimit);

  bool setSurfaceTemperatureUpperLimit(const Quantity& surfaceTemperatureUpperLimit);

  void resetSurfaceTemperatureUpperLimit();

  bool setMinimumSurfaceConvectionHeatTransferCoefficientValue(double minimumSurfaceConvectionHeatTransferCoefficientValue);

  bool setMinimumSurfaceConvectionHeatTransferCoefficientValue(const Quantity& minimumSurfaceConvectionHeatTransferCoefficientValue);

  void resetMinimumSurfaceConvectionHeatTransferCoefficientValue();

  bool setMaximumSurfaceConvectionHeatTransferCoefficientValue(double maximumSurfaceConvectionHeatTransferCoefficientValue);

  bool setMaximumSurfaceConvectionHeatTransferCoefficientValue(const Quantity& maximumSurfaceConvectionHeatTransferCoefficientValue);

  void resetMaximumSurfaceConvectionHeatTransferCoefficientValue();

  //@}

 protected:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new HeatBalanceAlgorithm object in the model.
  explicit HeatBalanceAlgorithm(const Model& model);

  //@}

  /// @cond

  typedef detail::HeatBalanceAlgorithm_Impl ImplType;

  friend class detail::HeatBalanceAlgorithm_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  explicit HeatBalanceAlgorithm(Model& model);
  // constructor
  explicit HeatBalanceAlgorithm(std::shared_ptr<detail::HeatBalanceAlgorithm_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.HeatBalanceAlgorithm");

  /// @endcond

};

/** \relates HeatBalanceAlgorithm*/
typedef boost::optional<HeatBalanceAlgorithm> OptionalHeatBalanceAlgorithm;

/** \relates HeatBalanceAlgorithm*/
typedef std::vector<HeatBalanceAlgorithm> HeatBalanceAlgorithmVector;

} // model
} // openstudio

#endif // MODEL_HEATBALANCEALGORITHM_HPP

