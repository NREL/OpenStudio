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

#ifndef MODEL_SITEGROUNDTEMPERATURESHALLOW_HPP
#define MODEL_SITEGROUNDTEMPERATURESHALLOW_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include <utilities/time/Date.hpp>

namespace openstudio {
namespace model {

namespace detail {

  class SiteGroundTemperatureShallow_Impl;

} // detail

/** SiteGroundTemperatureShallow is a ModelObject that wraps the OpenStudio IDD object 'OS_Site_GroundTemperature_Shallow'. */
class MODEL_API SiteGroundTemperatureShallow : public ModelObject {
 public:

  /** @name Constructors and Destructors */
  //@{

  explicit SiteGroundTemperatureShallow(Model& model);

  virtual ~SiteGroundTemperatureShallow() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  double januarySurfaceGroundTemperature() const;

  bool isJanuarySurfaceGroundTemperatureDefaulted() const;

  double februarySurfaceGroundTemperature() const;

  bool isFebruarySurfaceGroundTemperatureDefaulted() const;

  double marchSurfaceGroundTemperature() const;

  bool isMarchSurfaceGroundTemperatureDefaulted() const;

  double aprilSurfaceGroundTemperature() const;

  bool isAprilSurfaceGroundTemperatureDefaulted() const;

  double maySurfaceGroundTemperature() const;

  bool isMaySurfaceGroundTemperatureDefaulted() const;

  double juneSurfaceGroundTemperature() const;

  bool isJuneSurfaceGroundTemperatureDefaulted() const;

  double julySurfaceGroundTemperature() const;

  bool isJulySurfaceGroundTemperatureDefaulted() const;

  double augustSurfaceGroundTemperature() const;

  bool isAugustSurfaceGroundTemperatureDefaulted() const;

  double septemberSurfaceGroundTemperature() const;

  bool isSeptemberSurfaceGroundTemperatureDefaulted() const;

  double octoberSurfaceGroundTemperature() const;

  bool isOctoberSurfaceGroundTemperatureDefaulted() const;

  double novemberSurfaceGroundTemperature() const;

  bool isNovemberSurfaceGroundTemperatureDefaulted() const;

  double decemberSurfaceGroundTemperature() const;

  bool isDecemberSurfaceGroundTemperatureDefaulted() const;

  double getTemperatureByMonth(int month) const;

  double getTemperatureByMonth(const openstudio::MonthOfYear & month) const;

  bool isMonthDefaulted(int month) const;

  bool isMonthDefaulted(const openstudio::MonthOfYear & month) const;

  std::vector<double> getAllMonthlyTemperatures() const;

  //@}
  /** @name Setters */
  //@{

  void setJanuarySurfaceGroundTemperature(double januarySurfaceGroundTemperature);

  void resetJanuarySurfaceGroundTemperature();

  void setFebruarySurfaceGroundTemperature(double februarySurfaceGroundTemperature);

  void resetFebruarySurfaceGroundTemperature();

  void setMarchSurfaceGroundTemperature(double marchSurfaceGroundTemperature);

  void resetMarchSurfaceGroundTemperature();

  void setAprilSurfaceGroundTemperature(double aprilSurfaceGroundTemperature);

  void resetAprilSurfaceGroundTemperature();

  void setMaySurfaceGroundTemperature(double maySurfaceGroundTemperature);

  void resetMaySurfaceGroundTemperature();

  void setJuneSurfaceGroundTemperature(double juneSurfaceGroundTemperature);

  void resetJuneSurfaceGroundTemperature();

  void setJulySurfaceGroundTemperature(double julySurfaceGroundTemperature);

  void resetJulySurfaceGroundTemperature();

  void setAugustSurfaceGroundTemperature(double augustSurfaceGroundTemperature);

  void resetAugustSurfaceGroundTemperature();

  void setSeptemberSurfaceGroundTemperature(double septemberSurfaceGroundTemperature);

  void resetSeptemberSurfaceGroundTemperature();

  void setOctoberSurfaceGroundTemperature(double octoberSurfaceGroundTemperature);

  void resetOctoberSurfaceGroundTemperature();

  void setNovemberSurfaceGroundTemperature(double novemberSurfaceGroundTemperature);

  void resetNovemberSurfaceGroundTemperature();

  void setDecemberSurfaceGroundTemperature(double decemberSurfaceGroundTemperature);

  void resetDecemberSurfaceGroundTemperature();

  void setTemperatureByMonth(int month, double temperature);

  void setTemperatureByMonth(const openstudio::MonthOfYear & month, double temperature);

  void resetTemperatureByMonth(int month);

  void resetTemperatureByMonth(const openstudio::MonthOfYear & month);

  void resetAllMonths();

  bool setAllMonthlyTemperatures(const std::vector<double> &monthly_temperatures);

  //@}
 protected:
  /// @cond
  typedef detail::SiteGroundTemperatureShallow_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::SiteGroundTemperatureShallow_Impl;

  explicit SiteGroundTemperatureShallow(std::shared_ptr<detail::SiteGroundTemperatureShallow_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureShallow");
};

/** \relates SiteGroundTemperatureShallow*/
typedef boost::optional<SiteGroundTemperatureShallow> OptionalSiteGroundTemperatureShallow;

/** \relates SiteGroundTemperatureShallow*/
typedef std::vector<SiteGroundTemperatureShallow> SiteGroundTemperatureShallowVector;

} // model
} // openstudio

#endif // MODEL_SITEGROUNDTEMPERATURESHALLOW_HPP
