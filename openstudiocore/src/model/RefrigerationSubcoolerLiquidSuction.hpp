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

#ifndef MODEL_REFRIGERATIONSUBCOOLERLIQUIDSUCTION_HPP
#define MODEL_REFRIGERATIONSUBCOOLERLIQUIDSUCTION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class RefrigerationSubcoolerLiquidSuction_Impl;

} // detail

/** RefrigerationSubcoolerLiquidSuction is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:Subcooler:LiquidSuction'. */
class MODEL_API RefrigerationSubcoolerLiquidSuction : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefrigerationSubcoolerLiquidSuction(const Model& model);

  virtual ~RefrigerationSubcoolerLiquidSuction() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<double> liquidSuctionDesignSubcoolingTemperatureDifference() const;

  boost::optional<double> designLiquidInletTemperature() const;

  boost::optional<double> designVaporInletTemperature() const;

  //@}
  /** @name Setters */
  //@{

  void setLiquidSuctionDesignSubcoolingTemperatureDifference(double liquidSuctionDesignSubcoolingTemperatureDifference);

  void resetLiquidSuctionDesignSubcoolingTemperatureDifference();

  void setDesignLiquidInletTemperature(double designLiquidInletTemperature);

  void resetDesignLiquidInletTemperature();

  void setDesignVaporInletTemperature(double designVaporInletTemperature);

  void resetDesignVaporInletTemperature();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RefrigerationSubcoolerLiquidSuction_Impl ImplType;

  explicit RefrigerationSubcoolerLiquidSuction(std::shared_ptr<detail::RefrigerationSubcoolerLiquidSuction_Impl> impl);

  friend class detail::RefrigerationSubcoolerLiquidSuction_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RefrigerationSubcoolerLiquidSuction");
};

/** \relates RefrigerationSubcoolerLiquidSuction*/
typedef boost::optional<RefrigerationSubcoolerLiquidSuction> OptionalRefrigerationSubcoolerLiquidSuction;

/** \relates RefrigerationSubcoolerLiquidSuction*/
typedef std::vector<RefrigerationSubcoolerLiquidSuction> RefrigerationSubcoolerLiquidSuctionVector;

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONSUBCOOLERLIQUIDSUCTION_HPP

