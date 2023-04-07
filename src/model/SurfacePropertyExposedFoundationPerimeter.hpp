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

#ifndef MODEL_SURFACEPROPERTYEXPOSEDFOUNDATIONPERIMETER_HPP
#define MODEL_SURFACEPROPERTYEXPOSEDFOUNDATIONPERIMETER_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class Surface;

  namespace detail {

    class SurfacePropertyExposedFoundationPerimeter_Impl;

  }  // namespace detail

  /** SurfacePropertyExposedFoundationPerimeter is a ModelObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:ExposedFoundationPerimeter'. */
  class MODEL_API SurfacePropertyExposedFoundationPerimeter : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SurfacePropertyExposedFoundationPerimeter(Surface& surface, std::string exposedPerimeterCalculationMethod, double exposedPerimeter);

    virtual ~SurfacePropertyExposedFoundationPerimeter() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SurfacePropertyExposedFoundationPerimeter(const SurfacePropertyExposedFoundationPerimeter& other) = default;
    SurfacePropertyExposedFoundationPerimeter(SurfacePropertyExposedFoundationPerimeter&& other) = default;
    SurfacePropertyExposedFoundationPerimeter& operator=(const SurfacePropertyExposedFoundationPerimeter&) = default;
    SurfacePropertyExposedFoundationPerimeter& operator=(SurfacePropertyExposedFoundationPerimeter&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    std::string surfaceName() const;

    std::string exposedPerimeterCalculationMethod() const;

    boost::optional<double> totalExposedPerimeter();

    double exposedPerimeterFraction() const;

    bool isExposedPerimeterFractionDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setExposedPerimeterCalculationMethod(const std::string& exposedPerimeterCalculationMethod);

    bool setTotalExposedPerimeter(double totalExposedPerimeter);

    bool setExposedPerimeterFraction(double exposedPerimeterFraction);

    void resetExposedPerimeterFraction();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SurfacePropertyExposedFoundationPerimeter_Impl;

    explicit SurfacePropertyExposedFoundationPerimeter(std::shared_ptr<detail::SurfacePropertyExposedFoundationPerimeter_Impl> impl);

    friend class detail::SurfacePropertyExposedFoundationPerimeter_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SurfacePropertyExposedFoundationPerimeter");
  };

  /** \relates SurfacePropertyExposedFoundationPerimeter*/
  using OptionalSurfacePropertyExposedFoundationPerimeter = boost::optional<SurfacePropertyExposedFoundationPerimeter>;

  /** \relates SurfacePropertyExposedFoundationPerimeter*/
  using SurfacePropertyExposedFoundationPerimeterVector = std::vector<SurfacePropertyExposedFoundationPerimeter>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYEXPOSEDFOUNDATIONPERIMETER_HPP
