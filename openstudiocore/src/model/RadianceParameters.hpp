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

#ifndef MODEL_RADIANCEPARAMETERS_HPP
#define MODEL_RADIANCEPARAMETERS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class RadianceParameters_Impl;

} // detail

/** RadianceParameters is a ModelObject that wraps the OpenStudio IDD object 'OS:RadianceParameters'. */
class MODEL_API RadianceParameters : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~RadianceParameters() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> skyDiscretizationResolutionValues();

  /** @name Getters */
  //@{

  int accumulatedRaysperRecord() const;

  bool isAccumulatedRaysperRecordDefaulted() const;

  double directThreshold() const;

  Quantity getDirectThreshold(bool returnIP=false) const;

  bool isDirectThresholdDefaulted() const;

  double directCertainty() const;

  Quantity getDirectCertainty(bool returnIP=false) const;

  bool isDirectCertaintyDefaulted() const;

  double directJitter() const;

  Quantity getDirectJitter(bool returnIP=false) const;

  bool isDirectJitterDefaulted() const;

  double directPretest() const;

  Quantity getDirectPretest(bool returnIP=false) const;

  bool isDirectPretestDefaulted() const;

  int ambientBouncesVMX() const;

  bool isAmbientBouncesVMXDefaulted() const;

  int ambientBouncesDMX() const;

  bool isAmbientBouncesDMXDefaulted() const;

  int ambientDivisionsVMX() const;

  bool isAmbientDivisionsVMXDefaulted() const;

  int ambientDivisionsDMX() const;

  bool isAmbientDivisionsDMXDefaulted() const;

  int ambientSupersamples() const;

  bool isAmbientSupersamplesDefaulted() const;

  double limitWeightVMX() const;

  Quantity getLimitWeightVMX(bool returnIP=false) const;

  bool isLimitWeightVMXDefaulted() const;

  double limitWeightDMX() const;

  Quantity getLimitWeightDMX(bool returnIP=false) const;

  bool isLimitWeightDMXDefaulted() const;

  int klemsSamplingDensity() const;

  bool isKlemsSamplingDensityDefaulted() const;

  std::string skyDiscretizationResolution() const;

  //@}
  /** @name Setters */
  //@{

  bool setAccumulatedRaysperRecord(int accumulatedRaysperRecord);

  void resetAccumulatedRaysperRecord();

  bool setDirectThreshold(double directThreshold);

  bool setDirectThreshold(const Quantity& directThreshold);

  void resetDirectThreshold();

  bool setDirectCertainty(double directCertainty);

  bool setDirectCertainty(const Quantity& directCertainty);

  void resetDirectCertainty();

  bool setDirectJitter(double directJitter);

  bool setDirectJitter(const Quantity& directJitter);

  void resetDirectJitter();

  bool setDirectPretest(double directPretest);

  bool setDirectPretest(const Quantity& directPretest);

  void resetDirectPretest();

  bool setAmbientBouncesVMX(int ambientBouncesVMX);

  void resetAmbientBouncesVMX();

  bool setAmbientBouncesDMX(int ambientBouncesDMX);

  void resetAmbientBouncesDMX();

  bool setAmbientDivisionsVMX(int ambientDivisionsVMX);

  void resetAmbientDivisionsVMX();

  bool setAmbientDivisionsDMX(int ambientDivisionsDMX);

  void resetAmbientDivisionsDMX();

  bool setAmbientSupersamples(int ambientSupersamples);

  void resetAmbientSupersamples();

  bool setLimitWeightVMX(double limitWeightVMX);

  bool setLimitWeightVMX(const Quantity& limitWeightVMX);

  void resetLimitWeightVMX();

  void setLimitWeightDMX(double limitWeightDMX);

  bool setLimitWeightDMX(const Quantity& limitWeightDMX);

  void resetLimitWeightDMX();

  bool setKlemsSamplingDensity(int klemsSamplingDensity);

  void resetKlemsSamplingDensity();

  bool setSkyDiscretizationResolution(std::string skyDiscretizationResolution);

  void resetSkyDiscretizationResolution();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RadianceParameters_Impl ImplType;

  explicit RadianceParameters(std::shared_ptr<detail::RadianceParameters_Impl> impl);

  friend class detail::RadianceParameters_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  explicit RadianceParameters(Model& model,
                              int accumulatedRaysperRecord = 1,
                              double directThreshold = 0.0,
                              double directCertainty = 1.0,
                              double directJitter = 1.0,
                              double directPretest = 1.0,
                              int ambientBouncesVMX = 10,
                              int ambientBouncesDMX = 2,
                              int ambientDivisionsVMX = 65536,
                              int ambientDivisionsDMX = 1024,
                              int ambientSupersamples = 256,
                              double limitWeightVMX = 0.0000152,
                              double limitWeightDM = 0.01,
                              int klemsSamplingDensity = 1000,
                              std::string skyDiscretizationResolution = "146");

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RadianceParameters");
};

/** \relates RadianceParameters*/
typedef boost::optional<RadianceParameters> OptionalRadianceParameters;

/** \relates RadianceParameters*/
typedef std::vector<RadianceParameters> RadianceParametersVector;

} // model
} // openstudio

#endif // MODEL_RADIANCEPARAMETERS_HPP

