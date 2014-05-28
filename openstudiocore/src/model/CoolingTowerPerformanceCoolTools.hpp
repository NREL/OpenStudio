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

#ifndef MODEL_COOLINGTOWERPERFORMANCECOOLTOOLS_HPP
#define MODEL_COOLINGTOWERPERFORMANCECOOLTOOLS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class CoolingTowerPerformanceCoolTools_Impl;

} // detail

/** CoolingTowerPerformanceCoolTools is a ModelObject that wraps the OpenStudio IDD object 'OS:CoolingTowerPerformance:CoolTools'. */
class MODEL_API CoolingTowerPerformanceCoolTools : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoolingTowerPerformanceCoolTools(const Model& model);

  virtual ~CoolingTowerPerformanceCoolTools() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double minimumInletAirWetBulbTemperature() const;

  double maximumInletAirWetBulbTemperature() const;

  double minimumRangeTemperature() const;

  double maximumRangeTemperature() const;

  double minimumApproachTemperature() const;

  double maximumApproachTemperature() const;

  double minimumWaterFlowRateRatio() const;

  double maximumWaterFlowRateRatio() const;

  double coefficient1() const;

  double coefficient2() const;

  double coefficient3() const;

  double coefficient4() const;

  double coefficient5() const;

  double coefficient6() const;

  double coefficient7() const;

  double coefficient8() const;

  double coefficient9() const;

  double coefficient10() const;

  double coefficient11() const;

  double coefficient12() const;

  double coefficient13() const;

  double coefficient14() const;

  double coefficient15() const;

  double coefficient16() const;

  double coefficient17() const;

  double coefficient18() const;

  double coefficient19() const;

  double coefficient20() const;

  double coefficient21() const;

  double coefficient22() const;

  double coefficient23() const;

  double coefficient24() const;

  double coefficient25() const;

  double coefficient26() const;

  double coefficient27() const;

  double coefficient28() const;

  double coefficient29() const;

  double coefficient30() const;

  double coefficient31() const;

  double coefficient32() const;

  double coefficient33() const;

  double coefficient34() const;

  double coefficient35() const;

  //@}
  /** @name Setters */
  //@{

  void setMinimumInletAirWetBulbTemperature(double minimumInletAirWetBulbTemperature);

  void setMaximumInletAirWetBulbTemperature(double maximumInletAirWetBulbTemperature);

  void setMinimumRangeTemperature(double minimumRangeTemperature);

  void setMaximumRangeTemperature(double maximumRangeTemperature);

  void setMinimumApproachTemperature(double minimumApproachTemperature);

  void setMaximumApproachTemperature(double maximumApproachTemperature);

  void setMinimumWaterFlowRateRatio(double minimumWaterFlowRateRatio);

  void setMaximumWaterFlowRateRatio(double maximumWaterFlowRateRatio);

  void setCoefficient1(double coefficient1);

  void setCoefficient2(double coefficient2);

  void setCoefficient3(double coefficient3);

  void setCoefficient4(double coefficient4);

  void setCoefficient5(double coefficient5);

  void setCoefficient6(double coefficient6);

  void setCoefficient7(double coefficient7);

  void setCoefficient8(double coefficient8);

  void setCoefficient9(double coefficient9);

  void setCoefficient10(double coefficient10);

  void setCoefficient11(double coefficient11);

  void setCoefficient12(double coefficient12);

  void setCoefficient13(double coefficient13);

  void setCoefficient14(double coefficient14);

  void setCoefficient15(double coefficient15);

  void setCoefficient16(double coefficient16);

  void setCoefficient17(double coefficient17);

  void setCoefficient18(double coefficient18);

  void setCoefficient19(double coefficient19);

  void setCoefficient20(double coefficient20);

  void setCoefficient21(double coefficient21);

  void setCoefficient22(double coefficient22);

  void setCoefficient23(double coefficient23);

  void setCoefficient24(double coefficient24);

  void setCoefficient25(double coefficient25);

  void setCoefficient26(double coefficient26);

  void setCoefficient27(double coefficient27);

  void setCoefficient28(double coefficient28);

  void setCoefficient29(double coefficient29);

  void setCoefficient30(double coefficient30);

  void setCoefficient31(double coefficient31);

  void setCoefficient32(double coefficient32);

  void setCoefficient33(double coefficient33);

  void setCoefficient34(double coefficient34);

  void setCoefficient35(double coefficient35);

 protected:

  /// @cond
  typedef detail::CoolingTowerPerformanceCoolTools_Impl ImplType;

  explicit CoolingTowerPerformanceCoolTools(std::shared_ptr<detail::CoolingTowerPerformanceCoolTools_Impl> impl);

  friend class detail::CoolingTowerPerformanceCoolTools_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond

 private:

  REGISTER_LOGGER("openstudio.model.CoolingTowerPerformanceCoolTools");
};

/** \relates CoolingTowerPerformanceCoolTools*/
typedef boost::optional<CoolingTowerPerformanceCoolTools> OptionalCoolingTowerPerformanceCoolTools;

/** \relates CoolingTowerPerformanceCoolTools*/
typedef std::vector<CoolingTowerPerformanceCoolTools> CoolingTowerPerformanceCoolToolsVector;

} // model
} // openstudio

#endif // MODEL_COOLINGTOWERPERFORMANCECOOLTOOLS_HPP

