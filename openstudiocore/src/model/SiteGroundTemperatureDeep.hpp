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

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

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

  virtual ~SiteGroundTemperatureDeep() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  double januaryGroundTemperature() const;

  bool isJanuaryGroundTemperatureDefaulted() const;

  double februaryGroundTemperature() const;

  bool isFebruaryGroundTemperatureDefaulted() const;

  double marchGroundTemperature() const;

  bool isMarchGroundTemperatureDefaulted() const;

  double aprilGroundTemperature() const;

  bool isAprilGroundTemperatureDefaulted() const;

  double mayGroundTemperature() const;

  bool isMayGroundTemperatureDefaulted() const;

  double juneGroundTemperature() const;

  bool isJuneGroundTemperatureDefaulted() const;

  double julyGroundTemperature() const;

  bool isJulyGroundTemperatureDefaulted() const;

  double augustGroundTemperature() const;

  bool isAugustGroundTemperatureDefaulted() const;

  double septemberGroundTemperature() const;

  bool isSeptemberGroundTemperatureDefaulted() const;

  double octoberGroundTemperature() const;

  bool isOctoberGroundTemperatureDefaulted() const;

  double novemberGroundTemperature() const;

  bool isNovemberGroundTemperatureDefaulted() const;

  double decemberGroundTemperature() const;

  bool isDecemberGroundTemperatureDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  void setJanuaryGroundTemperature(double januaryGroundTemperature);

  void resetJanuaryGroundTemperature();

  void setFebruaryGroundTemperature(double februaryGroundTemperature);

  void resetFebruaryGroundTemperature();

  void setMarchGroundTemperature(double marchGroundTemperature);

  void resetMarchGroundTemperature();

  void setAprilGroundTemperature(double aprilGroundTemperature);

  void resetAprilGroundTemperature();

  void setMayGroundTemperature(double mayGroundTemperature);

  void resetMayGroundTemperature();

  void setJuneGroundTemperature(double juneGroundTemperature);

  void resetJuneGroundTemperature();

  void setJulyGroundTemperature(double julyGroundTemperature);

  void resetJulyGroundTemperature();

  void setAugustGroundTemperature(double augustGroundTemperature);

  void resetAugustGroundTemperature();

  void setSeptemberGroundTemperature(double septemberGroundTemperature);

  void resetSeptemberGroundTemperature();

  void setOctoberGroundTemperature(double octoberGroundTemperature);

  void resetOctoberGroundTemperature();

  void setNovemberGroundTemperature(double novemberGroundTemperature);

  void resetNovemberGroundTemperature();

  void setDecemberGroundTemperature(double decemberGroundTemperature);

  void resetDecemberGroundTemperature();

  //@}
 protected:
  /// @cond
  typedef detail::SiteGroundTemperatureDeep_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::SiteGroundTemperatureDeep_Impl;

  explicit SiteGroundTemperatureDeep(std::shared_ptr<detail::SiteGroundTemperatureDeep_Impl> impl);

  explicit SiteGroundTemperatureDeep(Model& model);

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

