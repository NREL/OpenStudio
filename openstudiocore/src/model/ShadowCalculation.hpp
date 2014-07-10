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

#ifndef MODEL_SHADOWCALCULATION_HPP
#define MODEL_SHADOWCALCULATION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {
  class ShadowCalculation_Impl;
} // detail

/** ShadowCalculation derives from ModelObject and is an interface to the OpenStudio IDD object named "ShadowCalculation".
 *
 *  ShadowCalculation defines how often shadowing calculations should be performed in EnergyPlus simulations
 *  as well as how many overlapping figures can be considered in a shadow calculation.
 *  ShadowCalculation does not have a public constructor because it is a unique ModelObject.  
 *  To get the ShadowCalculation object for a Model or create one if it does not yet exist use model.getUniqueObject<ShadowCalculation>().  
 *  To get the ShadowCalculation object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<ShadowCalculation>().
 */
class MODEL_API ShadowCalculation : public ModelObject {
 public:
  virtual ~ShadowCalculation() {}

  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  static std::vector<std::string> validPolygonClippingAlgorithmValues();

  static std::vector<std::string> validSkyDiffuseModelingAlgorithmValues();

  //@}
  /** @name Getters */
  //@{

  int calculationFrequency() const;

  bool isCalculationFrequencyDefaulted() const;

  int maximumFiguresInShadowOverlapCalculations() const;

  bool isMaximumFiguresInShadowOverlapCalculationsDefaulted() const;

  boost::optional<std::string> polygonClippingAlgorithm() const;

  boost::optional<std::string> skyDiffuseModelingAlgorithm() const;

  //@}
  /** @name Setters */
  //@{

  bool setCalculationFrequency(int calculationFrequency);

  void resetCalculationFrequency();

  bool setMaximumFiguresInShadowOverlapCalculations(int maximumFiguresInShadowOverlapCalculations);

  void resetMaximumFiguresInShadowOverlapCalculations();

  bool setPolygonClippingAlgorithm(std::string polygonClippingAlgorithm);

  void resetPolygonClippingAlgorithm();

  bool setSkyDiffuseModelingAlgorithm(std::string skyDiffuseModelingAlgorithm);

  void resetSkyDiffuseModelingAlgorithm();

  //@}

 protected:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new ShadowCalculation object in the model.
  explicit ShadowCalculation(const Model& model);

  //@}

  /// @cond

  typedef detail::ShadowCalculation_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  // constructor
  explicit ShadowCalculation(std::shared_ptr<detail::ShadowCalculation_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.ShadowCalculation");

  /// @endcond

};

/** \relates ShadowCalculation */
typedef boost::optional<ShadowCalculation> OptionalShadowCalculation;

} // model
} // openstudio

#endif // MODEL_SHADOWCALCULATION_HPP
