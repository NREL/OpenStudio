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

#ifndef MODEL_SITEWATERMAINSTEMPERATURE_HPP
#define MODEL_SITEWATERMAINSTEMPERATURE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class Schedule;

namespace detail {

  class SiteWaterMainsTemperature_Impl;

} // detail

/** SiteWaterMainsTemperature is a ModelObject that wraps the OpenStudio IDD object
 *  'OS:Site:WaterMainsTemperature'. */
class MODEL_API SiteWaterMainsTemperature : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~SiteWaterMainsTemperature() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> calculationMethodValues();

  /** \deprecated */
  static std::vector<std::string> validCalculationMethodValues();

  /** @name Getters */
  //@{

  std::string calculationMethod() const;

  boost::optional<Schedule> temperatureSchedule() const;

  boost::optional<double> annualAverageOutdoorAirTemperature() const;

  OSOptionalQuantity getAnnualAverageOutdoorAirTemperature(bool returnIP=false) const;

  boost::optional<double> maximumDifferenceInMonthlyAverageOutdoorAirTemperatures() const;

  OSOptionalQuantity getMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(bool returnIP=false) const;

  //@}
  /** @name Setters */
  //@{

  bool setCalculationMethod(std::string calculationMethod);

  /** Also sets calculation method to 'Schedule'. */
  bool setTemperatureSchedule(Schedule& schedule);

  void resetTemperatureSchedule();

  /** Also sets calculation method to 'Correlation'. */
  void setAnnualAverageOutdoorAirTemperature(double annualAverageOutdoorAirTemperature);

  /** Also sets calculation method to 'Correlation'. */
  bool setAnnualAverageOutdoorAirTemperature(const Quantity& annualAverageOutdoorAirTemperature);

  void resetAnnualAverageOutdoorAirTemperature();

  /** Also sets calculation method to 'Correlation'. */
  bool setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(double maximumDifferenceInMonthlyAverageOutdoorAirTemperatures);

  /** Also sets calculation method to 'Correlation'. */
  bool setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(const Quantity& maximumDifferenceInMonthlyAverageOutdoorAirTemperatures);

  void resetMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures();

  //@}
 protected:

  /// @cond
  typedef detail::SiteWaterMainsTemperature_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::SiteWaterMainsTemperature_Impl;

  explicit SiteWaterMainsTemperature(std::shared_ptr<detail::SiteWaterMainsTemperature_Impl> impl);

  explicit SiteWaterMainsTemperature(const Model& model);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.SiteWaterMainsTemperature");
};

/** \relates SiteWaterMainsTemperature*/
typedef boost::optional<SiteWaterMainsTemperature> OptionalSiteWaterMainsTemperature;

/** \relates SiteWaterMainsTemperature*/
typedef std::vector<SiteWaterMainsTemperature> SiteWaterMainsTemperatureVector;

} // model
} // openstudio

#endif // MODEL_SITEWATERMAINSTEMPERATURE_HPP

