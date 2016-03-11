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

#ifndef MODEL_SOLARCOLLECTORPERFORMANCEFLATPLATE_HPP
#define MODEL_SOLARCOLLECTORPERFORMANCEFLATPLATE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class SolarCollectorPerformanceFlatPlate;

namespace detail {

  class SolarCollectorFlatPlateWater_Impl;
  class SolarCollectorPerformanceFlatPlate_Impl;

} // detail

/** SolarCollectorPerformanceFlatPlate is a ModelObject that wraps the OpenStudio IDD object 'OS:SolarCollectorPerformance:FlatPlate'. */
class MODEL_API SolarCollectorPerformanceFlatPlate : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~SolarCollectorPerformanceFlatPlate() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> testFluidValues();

  static std::vector<std::string> testCorrelationTypeValues();

  /** @name Getters */
  //@{

  double grossArea() const;

  std::string testFluid() const;

  bool isTestFluidDefaulted() const;

  double testFlowRate() const;

  std::string testCorrelationType() const;

  double coefficient1ofEfficiencyEquation() const;

  double coefficient2ofEfficiencyEquation() const;

  boost::optional<double> coefficient3ofEfficiencyEquation() const;

  boost::optional<double> coefficient2ofIncidentAngleModifier() const;

  boost::optional<double> coefficient3ofIncidentAngleModifier() const;

  //@}
  /** @name Setters */
  //@{

  bool setGrossArea(double grossArea);

  bool setTestFluid(std::string testFluid);

  void resetTestFluid();

  bool setTestFlowRate(double testFlowRate);

  bool setTestCorrelationType(std::string testCorrelationType);

  void setCoefficient1ofEfficiencyEquation(double coefficient1ofEfficiencyEquation);

  void setCoefficient2ofEfficiencyEquation(double coefficient2ofEfficiencyEquation);

  void setCoefficient3ofEfficiencyEquation(double coefficient3ofEfficiencyEquation);

  void resetCoefficient3ofEfficiencyEquation();

  void setCoefficient2ofIncidentAngleModifier(double coefficient2ofIncidentAngleModifier);

  void resetCoefficient2ofIncidentAngleModifier();

  void setCoefficient3ofIncidentAngleModifier(double coefficient3ofIncidentAngleModifier);

  void resetCoefficient3ofIncidentAngleModifier();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
 
  explicit SolarCollectorPerformanceFlatPlate(const Model& model);
  
  /// @cond
  typedef detail::SolarCollectorPerformanceFlatPlate_Impl ImplType;

  explicit SolarCollectorPerformanceFlatPlate(std::shared_ptr<detail::SolarCollectorPerformanceFlatPlate_Impl> impl);

  friend class detail::SolarCollectorPerformanceFlatPlate_Impl;
  friend class detail::SolarCollectorFlatPlateWater_Impl;
  friend class SolarCollectorFlatPlateWater;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SolarCollectorPerformanceFlatPlate");
};

/** \relates SolarCollectorPerformanceFlatPlate*/
typedef boost::optional<SolarCollectorPerformanceFlatPlate> OptionalSolarCollectorPerformanceFlatPlate;

/** \relates SolarCollectorPerformanceFlatPlate*/
typedef std::vector<SolarCollectorPerformanceFlatPlate> SolarCollectorPerformanceFlatPlateVector;

} // model
} // openstudio

#endif // MODEL_SOLARCOLLECTORPERFORMANCEFLATPLATE_HPP

