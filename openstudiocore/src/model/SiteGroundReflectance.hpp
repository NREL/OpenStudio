/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

