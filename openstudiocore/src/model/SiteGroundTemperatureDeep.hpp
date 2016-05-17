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

#ifndef MODEL_SITEGROUNDTEMPERATUREDEEP_HPP
#define MODEL_SITEGROUNDTEMPERATUREDEEP_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include <utilities/time/Date.hpp>

namespace openstudio {
namespace model {

namespace detail {

  class SiteGroundTemperatureDeep_Impl;

} // detail

/** SiteGroundTemperatureDeep is a ModelObject that wraps the OpenStudio IDD object 'OS_Site_GroundTemperature_Deep'. */
class MODEL_API SiteGroundTemperatureDeep : public ModelObject {
 public:

  /** @name Constructors and Destructors */
  //@{

  explicit SiteGroundTemperatureDeep(Model& model);

  virtual ~SiteGroundTemperatureDeep() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  double januaryDeepGroundTemperature() const;

  bool isJanuaryDeepGroundTemperatureDefaulted() const;

  double februaryDeepGroundTemperature() const;

  bool isFebruaryDeepGroundTemperatureDefaulted() const;

  double marchDeepGroundTemperature() const;

  bool isMarchDeepGroundTemperatureDefaulted() const;

  double aprilDeepGroundTemperature() const;

  bool isAprilDeepGroundTemperatureDefaulted() const;

  double mayDeepGroundTemperature() const;

  bool isMayDeepGroundTemperatureDefaulted() const;

  double juneDeepGroundTemperature() const;

  bool isJuneDeepGroundTemperatureDefaulted() const;

  double julyDeepGroundTemperature() const;

  bool isJulyDeepGroundTemperatureDefaulted() const;

  double augustDeepGroundTemperature() const;

  bool isAugustDeepGroundTemperatureDefaulted() const;

  double septemberDeepGroundTemperature() const;

  bool isSeptemberDeepGroundTemperatureDefaulted() const;

  double octoberDeepGroundTemperature() const;

  bool isOctoberDeepGroundTemperatureDefaulted() const;

  double novemberDeepGroundTemperature() const;

  bool isNovemberDeepGroundTemperatureDefaulted() const;

  double decemberDeepGroundTemperature() const;

  bool isDecemberDeepGroundTemperatureDefaulted() const;

  double getTemperatureByMonth(int month) const;

  double getTemperatureByMonth(const openstudio::MonthOfYear & month) const;

  bool isMonthDefaulted(int month) const;

  bool isMonthDefaulted(const openstudio::MonthOfYear & month) const;

  std::vector<double> getAllMonthlyTemperatures() const;

  //@}
  /** @name Setters */
  //@{

  void setJanuaryDeepGroundTemperature(double januaryDeepGroundTemperature);

  void resetJanuaryDeepGroundTemperature();

  void setFebruaryDeepGroundTemperature(double februaryDeepGroundTemperature);

  void resetFebruaryDeepGroundTemperature();

  void setMarchDeepGroundTemperature(double marchDeepGroundTemperature);

  void resetMarchDeepGroundTemperature();

  void setAprilDeepGroundTemperature(double aprilDeepGroundTemperature);

  void resetAprilDeepGroundTemperature();

  void setMayDeepGroundTemperature(double mayDeepGroundTemperature);

  void resetMayDeepGroundTemperature();

  void setJuneDeepGroundTemperature(double juneDeepGroundTemperature);

  void resetJuneDeepGroundTemperature();

  void setJulyDeepGroundTemperature(double julyDeepGroundTemperature);

  void resetJulyDeepGroundTemperature();

  void setAugustDeepGroundTemperature(double augustDeepGroundTemperature);

  void resetAugustDeepGroundTemperature();

  void setSeptemberDeepGroundTemperature(double septemberDeepGroundTemperature);

  void resetSeptemberDeepGroundTemperature();

  void setOctoberDeepGroundTemperature(double octoberDeepGroundTemperature);

  void resetOctoberDeepGroundTemperature();

  void setNovemberDeepGroundTemperature(double novemberDeepGroundTemperature);

  void resetNovemberDeepGroundTemperature();

  void setDecemberDeepGroundTemperature(double decemberDeepGroundTemperature);

  void resetDecemberDeepGroundTemperature();

  void setTemperatureByMonth(int month, double temperature);

  void setTemperatureByMonth(const openstudio::MonthOfYear & month, double temperature);

  void resetTemperatureByMonth(int month);

  void resetTemperatureByMonth(const openstudio::MonthOfYear & month);

  void resetAllMonths();

  bool setAllMonthlyTemperatures(const std::vector<double> &monthly_temperatures);

  //@}
 protected:
  /// @cond
  typedef detail::SiteGroundTemperatureDeep_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::SiteGroundTemperatureDeep_Impl;

  explicit SiteGroundTemperatureDeep(std::shared_ptr<detail::SiteGroundTemperatureDeep_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.SiteGroundTemperatureDeep");
};

/** \relates SiteGroundTemperatureDeep*/
typedef boost::optional<SiteGroundTemperatureDeep> OptionalSiteGroundTemperatureDeep;

/** \relates SiteGroundTemperatureDeep*/
typedef std::vector<SiteGroundTemperatureDeep> SiteGroundTemperatureDeepVector;

} // model
} // openstudio

#endif // MODEL_SITEGROUNDTEMPERATUREDeep_HPP
