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

#ifndef MODEL_REFRIGERATIONCOMPRESSOR_HPP
#define MODEL_REFRIGERATIONCOMPRESSOR_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

class CurveBicubic;

namespace detail {

  class RefrigerationCompressor_Impl;

} // detail

/** RefrigerationCompressor is a ParentObject that wraps the OpenStudio IDD object 'OS:Refrigeration:Compressor'. */
class MODEL_API RefrigerationCompressor : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefrigerationCompressor(const Model& model);

  virtual ~RefrigerationCompressor() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> modeofOperationValues();

  /** @name Getters */
  //@{

  CurveBicubic refrigerationCompressorPowerCurve() const;

  CurveBicubic refrigerationCompressorCapacityCurve() const;

  boost::optional<double> ratedSuperheat() const;

  boost::optional<double> ratedReturnGasTemperature() const;

  boost::optional<double> ratedLiquidTemperature() const;

  boost::optional<double> ratedSubcooling() const;

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;

  std::string modeofOperation() const;

  // bool isModeofOperationDefaulted() const;

  boost::optional<CurveBicubic> transcriticalCompressorPowerCurve() const;

  boost::optional<CurveBicubic> transcriticalCompressorCapacityCurve() const;

  //@}
  /** @name Setters */
  //@{

  bool setRefrigerationCompressorPowerCurve(const CurveBicubic& curveBicubic);

  bool setRefrigerationCompressorCapacityCurve(const CurveBicubic& curveBicubic);

  void setRatedSuperheat(double ratedSuperheat);

  void resetRatedSuperheat();

  void setRatedReturnGasTemperature(double ratedReturnGasTemperature);

  void resetRatedReturnGasTemperature();

  void setRatedLiquidTemperature(double ratedLiquidTemperature);

  void resetRatedLiquidTemperature();

  void setRatedSubcooling(double ratedSubcooling);

  void resetRatedSubcooling();

  void setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

  // bool setModeofOperation(std::string modeofOperation);

  // void resetModeofOperation();

  bool setTranscriticalCompressorPowerCurve(const CurveBicubic& curveBicubic);

  void resetTranscriticalCompressorPowerCurve();

  bool setTranscriticalCompressorCapacityCurve(const CurveBicubic& curveBicubic);

  void resetTranscriticalCompressorCapacityCurve();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RefrigerationCompressor_Impl ImplType;

  explicit RefrigerationCompressor(std::shared_ptr<detail::RefrigerationCompressor_Impl> impl);

  friend class detail::RefrigerationCompressor_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RefrigerationCompressor");
};

/** \relates RefrigerationCompressor*/
typedef boost::optional<RefrigerationCompressor> OptionalRefrigerationCompressor;

/** \relates RefrigerationCompressor*/
typedef std::vector<RefrigerationCompressor> RefrigerationCompressorVector;

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONCOMPRESSOR_HPP

