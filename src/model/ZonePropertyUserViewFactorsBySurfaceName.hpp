/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "InternalMass.hpp"
#include "InternalMass_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  /* class Surface;
class SubSurface;
class InternalMass; */

  namespace detail {

    class ZonePropertyUserViewFactorsBySurfaceName_Impl;

  }  // namespace detail

  /** This class implements a view factor */
  class MODEL_API ViewFactor
  {
   public:
    /* Only accepts ModelObjects that are of type Surface, Subsurface or InternalMass, will throw otherwise */
    ViewFactor(const ModelObject& fromSurface, const ModelObject& toSurface, double viewFactor);

    ModelObject fromSurface() const;
    ModelObject toSurface() const;
    double viewFactor() const;

   private:
    // From
    ModelObject m_from_surface;
    ModelObject m_to_surface;
    double m_view_factor;
    REGISTER_LOGGER("openstudio.model.ViewFactor");
  };

  // Overload operator<<
  MODEL_API std::ostream& operator<<(std::ostream& out, const openstudio::model::ViewFactor& viewFactor);

  /** ZonePropertyUserViewFactorsBySurfaceName is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneProperty:UserViewFactors:BySurfaceName'. */
  class MODEL_API ZonePropertyUserViewFactorsBySurfaceName : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /*
   * This constructor shouldn't be used, ThermalZone::getZonePropertyUserViewFactorsBySurfaceName should be prefered
   * as it will either instantiate a new object or return the existing one.
   * A ThermalZone should not have more than one ZonePropertyUserViewFactorsBySurfaceName. If you try to instantiate a second one for a given
   * ThermalZone this constructor will throw
   */
    explicit ZonePropertyUserViewFactorsBySurfaceName(const ThermalZone& thermalZone);

    virtual ~ZonePropertyUserViewFactorsBySurfaceName() {}

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ThermalZone thermalZone() const;

    std::vector<ViewFactor> viewFactors() const;

    unsigned int numberofViewFactors() const;

    /** If a viewFactor group is already present (cf `viewFactorIndex()`), it will Warn and override the viewFactor value */
    boost::optional<unsigned> viewFactorIndex(const ViewFactor& viewFactor) const;

    boost::optional<ViewFactor> getViewFactor(unsigned groupIndex) const;

    //@}
    /** @name Setters */
    //@{

    /** If a viewFactor group is already present (cf `viewFactorIndex()`), it will Warn and override the viewFactor value */
    bool addViewFactor(const ViewFactor& viewFactor);

    // Overloads, they create a ViewFactor wrapper, then call `addViewFactor(const ViewFactor& viewFactor)`
    bool addViewFactor(const Surface& fromSurface, const Surface& toSurface, double viewFactor);

    bool addViewFactor(const Surface& fromSurface, const SubSurface& toSubSurface, double viewFactor);

    bool addViewFactor(const Surface& fromSurface, const InternalMass& toInternalMass, double viewFactor);

    bool addViewFactor(const SubSurface& fromSubSurface, const SubSurface& toSubSurface, double viewFactor);

    bool addViewFactor(const SubSurface& fromSubSurface, const Surface& toSurface, double viewFactor);

    bool addViewFactor(const SubSurface& fromSubSurface, const InternalMass& toInternalMass, double viewFactor);

    bool addViewFactor(const InternalMass& fromInternalMass, const InternalMass& toInernalMass, double viewFactor);

    bool addViewFactor(const InternalMass& fromInternalMass, const Surface& toSurface, double viewFactor);

    bool addViewFactor(const InternalMass& fromInternalMass, const SubSurface& toSubSurface, double viewFactor);

    bool addViewFactors(const std::vector<ViewFactor>& viewFactors);

    void removeViewFactor(int groupIndex);

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

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEPROPERTYUSERVIEWFACTORSBYSURFACENAME_HPP
