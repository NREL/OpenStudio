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

#ifndef MODEL_DAYLIGHTINGDEVICELIGHTWELL_HPP
#define MODEL_DAYLIGHTINGDEVICELIGHTWELL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class SubSurface;
  class InteriorPartitionSurface;
  class ShadingSurface;

  namespace detail {

    class DaylightingDeviceLightWell_Impl;

  }  // namespace detail

  /** DaylightingDeviceLightWell is a ModelObject that wraps the OpenStudio IDD object 'OS_DaylightingDevice_LightWell'. */
  class MODEL_API DaylightingDeviceLightWell : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit DaylightingDeviceLightWell(const SubSurface& subSurface, double heightofWell, double perimeterofBottomofWell, double areaofBottomofWell,
                                        double visibleReflectanceofWellWalls);

    explicit DaylightingDeviceLightWell(const SubSurface& subSurface);

    virtual ~DaylightingDeviceLightWell() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DaylightingDeviceLightWell(const DaylightingDeviceLightWell& other) = default;
    DaylightingDeviceLightWell(DaylightingDeviceLightWell&& other) = default;
    DaylightingDeviceLightWell& operator=(const DaylightingDeviceLightWell&) = default;
    DaylightingDeviceLightWell& operator=(DaylightingDeviceLightWell&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    SubSurface subSurface() const;

    double heightofWell() const;

    double perimeterofBottomofWell() const;

    double areaofBottomofWell() const;

    double visibleReflectanceofWellWalls() const;

    //@}
    /** @name Setters */
    //@{

    bool setHeightofWell(double heightofWell);

    bool setPerimeterofBottomofWell(double perimeterofBottomofWell);

    bool setAreaofBottomofWell(double areaofBottomofWell);

    bool setVisibleReflectanceofWellWalls(double visibleReflectanceofWellWalls);

    //@}

   protected:
    /// @cond
    using ImplType = detail::DaylightingDeviceLightWell_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    explicit DaylightingDeviceLightWell(std::shared_ptr<detail::DaylightingDeviceLightWell_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.DaylightingDeviceLightWell");
  };

  /** \relates DaylightingDeviceLightWell*/
  using OptionalDaylightingDeviceLightWell = boost::optional<DaylightingDeviceLightWell>;

  /** \relates DaylightingDeviceLightWell*/
  using DaylightingDeviceLightWellVector = std::vector<DaylightingDeviceLightWell>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DAYLIGHTINGDEVICELIGHTWELL_HPP
