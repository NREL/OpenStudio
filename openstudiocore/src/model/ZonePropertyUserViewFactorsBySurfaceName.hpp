/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_ZONEPROPERTYUSERVIEWFACTORSBYSURFACENAME_HPP
#define MODEL_ZONEPROPERTYUSERVIEWFACTORSBYSURFACENAME_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"

#include "PlanarSurface.hpp"
/* #include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "InternalMass.hpp"
#include "InternalMass_Impl.hpp" */

namespace openstudio {
namespace model {

class ThermalZone;
/* class Surface;
class SubSurface;
class InternalMass; */

namespace detail {

  class ZonePropertyUserViewFactorsBySurfaceName_Impl;

} // detail


/** This class implements a view factor */
class ViewFactorData {
  public:
    ViewFactorData(const PlanarSurface& fromSurface, const PlanarSurface& toSurface, double viewFactor);  

    PlanarSurface fromSurface() const;
    PlanarSurface toSurface() const;
    double viewFactor() const;

  private:
    PlanarSurface m_from_surface;
    PlanarSurface m_to_surface;
    double m_view_factor;
    REGISTER_LOGGER("openstudio.model.ViewFactor");
};

// Overload operator<<
std::ostream& operator<< (std::ostream& out, const openstudio::model::ViewFactorData& viewFactor);

/** ZonePropertyUserViewFactorsBySurfaceName is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneProperty:UserViewFactors:bySurfaceName'. */
class MODEL_API ZonePropertyUserViewFactorsBySurfaceName : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZonePropertyUserViewFactorsBySurfaceName(ThermalZone& thermalZone);

  virtual ~ZonePropertyUserViewFactorsBySurfaceName() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  ThermalZone thermalZone() const;

  std::vector<ViewFactorData> viewFactors() const;

  unsigned int numberofViewFactors() const;  

  //@}
  /** @name Setters */
  //@{

  bool addViewFactor(const ViewFactorData& viewFactor);

  bool addViewFactor(const PlanarSurface& fromSurface, const PlanarSurface& toSurface, double viewFactor);

  bool addViewFactors(const std::vector<ViewFactorData> &viewFactors);

  bool removeViewFactor(unsigned groupIndex);

  void removeAllViewFactors();

  //@}
  /** @name Other */
  //@{

  //@}
 protected: 
  /// @cond
  typedef detail::ZonePropertyUserViewFactorsBySurfaceName_Impl ImplType;

  explicit ZonePropertyUserViewFactorsBySurfaceName(std::shared_ptr<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl> impl);

  friend class detail::ZonePropertyUserViewFactorsBySurfaceName_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZonePropertyUserViewFactorsBySurfaceName");
};

/** \relates ZonePropertyUserViewFactorsBySurfaceName*/
typedef boost::optional<ZonePropertyUserViewFactorsBySurfaceName> OptionalZonePropertyUserViewFactorsBySurfaceName;

/** \relates ZonePropertyUserViewFactorsBySurfaceName*/
typedef std::vector<ZonePropertyUserViewFactorsBySurfaceName> ZonePropertyUserViewFactorsBySurfaceNameVector;

} // model
} // openstudio

#endif // MODEL_ZONEPROPERTYUSERVIEWFACTORSBYSURFACENAME_HPP