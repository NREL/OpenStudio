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

#ifndef MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTS_HPP
#define MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "Surface.hpp"
#include "SubSurface.hpp"
#include "InternalMass.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class SurfacePropertyConvectionCoefficients_Impl;

  }  // namespace detail

  /** SurfacePropertyConvectionCoefficients is a ModelObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:ConvectionCoefficients'. */
  class MODEL_API SurfacePropertyConvectionCoefficients : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SurfacePropertyConvectionCoefficients(const Surface& surface);

    explicit SurfacePropertyConvectionCoefficients(const SubSurface& surface);

    explicit SurfacePropertyConvectionCoefficients(const InternalMass& surface);

    virtual ~SurfacePropertyConvectionCoefficients() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SurfacePropertyConvectionCoefficients(const SurfacePropertyConvectionCoefficients& other) = default;
    SurfacePropertyConvectionCoefficients(SurfacePropertyConvectionCoefficients&& other) = default;
    SurfacePropertyConvectionCoefficients& operator=(const SurfacePropertyConvectionCoefficients&) = default;
    SurfacePropertyConvectionCoefficients& operator=(SurfacePropertyConvectionCoefficients&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> convectionCoefficient1LocationValues();

    static std::vector<std::string> convectionCoefficient1TypeValues();

    static std::vector<std::string> convectionCoefficient2LocationValues();

    static std::vector<std::string> convectionCoefficient2TypeValues();

    /** @name Getters */
    //@{

    ModelObject surfaceAsModelObject() const;

    boost::optional<Surface> surfaceAsSurface() const;

    boost::optional<SubSurface> surfaceAsSubSurface() const;

    boost::optional<InternalMass> surfaceAsInternalMass() const;

    boost::optional<std::string> convectionCoefficient1Location() const;

    boost::optional<std::string> convectionCoefficient1Type() const;

    boost::optional<double> convectionCoefficient1() const;

    boost::optional<Schedule> convectionCoefficient1Schedule() const;

    boost::optional<std::string> convectionCoefficient2Location() const;

    boost::optional<std::string> convectionCoefficient2Type() const;

    boost::optional<double> convectionCoefficient2() const;

    boost::optional<Schedule> convectionCoefficient2Schedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setSurface(const ModelObject& surface);

    bool setConvectionCoefficient1Location(const std::string& convectionCoefficient1Location);

    void resetConvectionCoefficient1Location();

    bool setConvectionCoefficient1Type(const std::string& convectionCoefficient1Type);

    void resetConvectionCoefficient1Type();

    bool setConvectionCoefficient1(double convectionCoefficient1);

    void resetConvectionCoefficient1();

    bool setConvectionCoefficient1Schedule(Schedule& schedule);

    void resetConvectionCoefficient1Schedule();

    bool setConvectionCoefficient2Location(const std::string& convectionCoefficient2Location);

    void resetConvectionCoefficient2Location();

    bool setConvectionCoefficient2Type(const std::string& convectionCoefficient2Type);

    void resetConvectionCoefficient2Type();

    bool setConvectionCoefficient2(double convectionCoefficient2);

    void resetConvectionCoefficient2();

    bool setConvectionCoefficient2Schedule(Schedule& schedule);

    void resetConvectionCoefficient2Schedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SurfacePropertyConvectionCoefficients_Impl;

    explicit SurfacePropertyConvectionCoefficients(std::shared_ptr<detail::SurfacePropertyConvectionCoefficients_Impl> impl);

    friend class detail::SurfacePropertyConvectionCoefficients_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SurfacePropertyConvectionCoefficients");
  };

  /** \relates SurfacePropertyConvectionCoefficients*/
  using OptionalSurfacePropertyConvectionCoefficients = boost::optional<SurfacePropertyConvectionCoefficients>;

  /** \relates SurfacePropertyConvectionCoefficients*/
  using SurfacePropertyConvectionCoefficientsVector = std::vector<SurfacePropertyConvectionCoefficients>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTS_HPP
