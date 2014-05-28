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

#ifndef MODEL_SITEGROUNDREFLECTANCE_HPP
#define MODEL_SITEGROUNDREFLECTANCE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class SiteGroundReflectance_Impl;

} // detail

/** SiteGroundReflectance is a ModelObject that wraps the OpenStudio IDD object 'OS:Site:GroundReflectance'. */

class MODEL_API SiteGroundReflectance : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~SiteGroundReflectance() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  double januaryGroundReflectance() const;

  bool isJanuaryGroundReflectanceDefaulted() const;

  double februaryGroundReflectance() const;

  bool isFebruaryGroundReflectanceDefaulted() const;

  double marchGroundReflectance() const;

  bool isMarchGroundReflectanceDefaulted() const;

  double aprilGroundReflectance() const;

  bool isAprilGroundReflectanceDefaulted() const;

  double mayGroundReflectance() const;

  bool isMayGroundReflectanceDefaulted() const;

  double juneGroundReflectance() const;

  bool isJuneGroundReflectanceDefaulted() const;

  double julyGroundReflectance() const;

  bool isJulyGroundReflectanceDefaulted() const;

  double augustGroundReflectance() const;

  bool isAugustGroundReflectanceDefaulted() const;

  double septemberGroundReflectance() const;

  bool isSeptemberGroundReflectanceDefaulted() const;

  double octoberGroundReflectance() const;

  bool isOctoberGroundReflectanceDefaulted() const;

  double novemberGroundReflectance() const;

  bool isNovemberGroundReflectanceDefaulted() const;

  double decemberGroundReflectance() const;

  bool isDecemberGroundReflectanceDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setJanuaryGroundReflectance(double januaryGroundReflectance);

  void resetJanuaryGroundReflectance();

  bool setFebruaryGroundReflectance(double februaryGroundReflectance);

  void resetFebruaryGroundReflectance();

  bool setMarchGroundReflectance(double marchGroundReflectance);

  void resetMarchGroundReflectance();

  bool setAprilGroundReflectance(double aprilGroundReflectance);

  void resetAprilGroundReflectance();

  bool setMayGroundReflectance(double mayGroundReflectance);

  void resetMayGroundReflectance();

  bool setJuneGroundReflectance(double juneGroundReflectance);

  void resetJuneGroundReflectance();

  bool setJulyGroundReflectance(double julyGroundReflectance);

  void resetJulyGroundReflectance();

  bool setAugustGroundReflectance(double augustGroundReflectance);

  void resetAugustGroundReflectance();

  bool setSeptemberGroundReflectance(double septemberGroundReflectance);

  void resetSeptemberGroundReflectance();

  bool setOctoberGroundReflectance(double octoberGroundReflectance);

  void resetOctoberGroundReflectance();

  bool setNovemberGroundReflectance(double novemberGroundReflectance);

  void resetNovemberGroundReflectance();

  bool setDecemberGroundReflectance(double decemberGroundReflectance);

  void resetDecemberGroundReflectance();

  //@}
 protected:
  /// @cond
  typedef detail::SiteGroundReflectance_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::SiteGroundReflectance_Impl;

  explicit SiteGroundReflectance(std::shared_ptr<detail::SiteGroundReflectance_Impl> impl);

  explicit SiteGroundReflectance(Model& model);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SiteGroundReflectance");
};

/** \relates SiteGroundReflectance*/
typedef boost::optional<SiteGroundReflectance> OptionalSiteGroundReflectance;

/** \relates SiteGroundReflectance*/
typedef std::vector<SiteGroundReflectance> SiteGroundReflectanceVector;

} // model
} // openstudio

#endif // MODEL_SITEGROUNDREFLECTANCE_HPP

