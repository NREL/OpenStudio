/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_DAYLIGHTINGDEVICETUBULAR_HPP
#define MODEL_DAYLIGHTINGDEVICETUBULAR_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"

namespace openstudio {
namespace model {

  class SubSurface;
  class ConstructionBase;
  class ThermalZone;

  // Note: TransitionZone is a class defined in ThermalZone.hpp so that the order things are exposed to SWIG makes sense
  class TransitionZone;

  namespace detail {

    class DaylightingDeviceTubular_Impl;

  }  // namespace detail

  /** DaylightingDeviceTubular is a ModelObject that wraps the OpenStudio IDD object 'OS_DaylightingDevice_Tubular'. */
  class MODEL_API DaylightingDeviceTubular : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit DaylightingDeviceTubular(const SubSurface& dome, const SubSurface& diffuser, const ConstructionBase& construction, double diameter,
                                      double totalLength, double effectiveThermalResistance);

    explicit DaylightingDeviceTubular(const SubSurface& dome, const SubSurface& diffuser, const ConstructionBase& construction);

    virtual ~DaylightingDeviceTubular() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DaylightingDeviceTubular(const DaylightingDeviceTubular& other) = default;
    DaylightingDeviceTubular(DaylightingDeviceTubular&& other) = default;
    DaylightingDeviceTubular& operator=(const DaylightingDeviceTubular&) = default;
    DaylightingDeviceTubular& operator=(DaylightingDeviceTubular&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    SubSurface subSurfaceDome() const;

    SubSurface subSurfaceDiffuser() const;

    ConstructionBase construction() const;

    double diameter() const;

    double totalLength() const;

    double effectiveThermalResistance() const;

    std::vector<TransitionZone> transitionZones() const;

    unsigned int numberofTransitionZones() const;

    //@}
    /** @name Setters */
    //@{

    bool setConstruction(const ConstructionBase& construction);

    bool setDiameter(double diameter);

    bool setTotalLength(double totalLength);

    bool setEffectiveThermalResistance(double effectiveThermalResistance);

    bool addTransitionZone(const TransitionZone& transitionZone);

    bool addTransitionZone(const ThermalZone& zone, double length);

    void removeTransitionZone(int groupIndex);

    void removeAllTransitionZones();

    bool addTransitionZones(const std::vector<TransitionZone>& transitionZones);

    //@}

   protected:
    /// @cond
    using ImplType = detail::DaylightingDeviceTubular_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    explicit DaylightingDeviceTubular(std::shared_ptr<detail::DaylightingDeviceTubular_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.DaylightingDeviceTubular");
  };

  /** \relates DaylightingDeviceTubular*/
  using OptionalDaylightingDeviceTubular = boost::optional<DaylightingDeviceTubular>;

  /** \relates DaylightingDeviceTubular*/
  using DaylightingDeviceTubularVector = std::vector<DaylightingDeviceTubular>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DAYLIGHTINGDEVICETUBULAR_HPP
