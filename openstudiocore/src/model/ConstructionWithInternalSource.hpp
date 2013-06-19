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

#ifndef MODEL_CONSTRUCTIONWITHINTERNALSOURCE_HPP
#define MODEL_CONSTRUCTIONWITHINTERNALSOURCE_HPP

#include <model/ModelAPI.hpp>
#include <model/LayeredConstruction.hpp>

namespace openstudio {
namespace model {

namespace detail{
  class ConstructionWithInternalSource_Impl;
}

/** ConstructionWithInternalSource is a LayeredConstruction that wraps EnergyPlus IDD object 
 *  'Construction:InternalSource'. ConstructionWithInternalSource is identical to Construction, 
 *  except that it contains an embedded heat source. */
class MODEL_API ConstructionWithInternalSource : public LayeredConstruction {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** TODO default values should be reviewed */
  explicit ConstructionWithInternalSource(const Model& model,
    int sourcePresentAfterLayerNumber = 1,
    int temperatureCalculationRequestedAfterLayerNumber = 1,
    int dimensionsForTheCTFCalculation = 1,
    double tubeSpacing = 0.1);

  virtual ~ConstructionWithInternalSource() {}

  /** Attribute Name: 'sourcePresentAfterLayerNumber' */
  int sourcePresentAfterLayerNumber() const;
  bool setSourcePresentAfterLayerNumber(int sourcePresentAfterLayerNumber);

  /** Attribute Name: 'temperatureCalculationRequestedAfterLayerNumber' */
  int temperatureCalculationRequestedAfterLayerNumber() const;
  bool setTemperatureCalculationRequestedAfterLayerNumber(int temperatureCalculationRequestedAfterLayerNumber);

  /** Attribute Name: 'dimensionsForTheCTFCalculation' */
  int dimensionsForTheCTFCalculation() const;
  bool setDimensionsForTheCTFCalculation(int dimensionsForTheCTFCalculation);

  /** Attribute Name: 'tubeSpacing' */
  double tubeSpacing() const;
  Quantity getTubeSpacing(bool returnIP = false) const;
  bool setTubeSpacing(double tubeSpacing);
  bool setTubeSpacing(const Quantity& tubeSpacing);

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
 protected:
  /// @cond
  typedef detail::ConstructionWithInternalSource_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::ConstructionWithInternalSource_Impl;

  explicit ConstructionWithInternalSource(
      boost::shared_ptr<detail::ConstructionWithInternalSource_Impl> impl);  
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ConstructionWithInternalSource");
};

/** \relates ConstructionWithInternalSource */
typedef boost::optional<ConstructionWithInternalSource> OptionalConstructionWithInternalSource;

/** \relates ConstructionWithInternalSource */
typedef std::vector<ConstructionWithInternalSource> ConstructionWithInternalSourceVector;

} // model
} // openstudio

#endif // MODEL_CONSTRUCTIONWITHINTERNALSOURCE_HPP
