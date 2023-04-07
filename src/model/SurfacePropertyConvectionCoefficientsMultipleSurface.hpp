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

#ifndef MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTSMULTIPLESURFACE_HPP
#define MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTSMULTIPLESURFACE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class SurfacePropertyConvectionCoefficientsMultipleSurface_Impl;

  }  // namespace detail

  /** SurfacePropertyConvectionCoefficientsMultipleSurface is a ModelObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:ConvectionCoefficients:MultipleSurface'. */
  class MODEL_API SurfacePropertyConvectionCoefficientsMultipleSurface : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SurfacePropertyConvectionCoefficientsMultipleSurface(const Model& model);

    virtual ~SurfacePropertyConvectionCoefficientsMultipleSurface() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SurfacePropertyConvectionCoefficientsMultipleSurface(const SurfacePropertyConvectionCoefficientsMultipleSurface& other) = default;
    SurfacePropertyConvectionCoefficientsMultipleSurface(SurfacePropertyConvectionCoefficientsMultipleSurface&& other) = default;
    SurfacePropertyConvectionCoefficientsMultipleSurface& operator=(const SurfacePropertyConvectionCoefficientsMultipleSurface&) = default;
    SurfacePropertyConvectionCoefficientsMultipleSurface& operator=(SurfacePropertyConvectionCoefficientsMultipleSurface&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> surfaceTypeValues();

    static std::vector<std::string> convectionCoefficient1LocationValues();

    static std::vector<std::string> convectionCoefficient1TypeValues();

    static std::vector<std::string> convectionCoefficient2LocationValues();

    static std::vector<std::string> convectionCoefficient2TypeValues();

    /** @name Getters */
    //@{

    boost::optional<std::string> surfaceType() const;

    boost::optional<std::string> convectionCoefficient1Location() const;

    boost::optional<std::string> convectionCoefficient1Type() const;

    boost::optional<double> convectionCoefficient1() const;

    boost::optional<Schedule> convectionCoefficient1Schedule() const;

    // DLM: not supporting user curves yet
    //boost::optional<UserConvectionModels> convectionCoefficient1UserCurve() const;

    boost::optional<std::string> convectionCoefficient2Location() const;

    boost::optional<std::string> convectionCoefficient2Type() const;

    boost::optional<double> convectionCoefficient2() const;

    boost::optional<Schedule> convectionCoefficient2Schedule() const;

    // DLM: not supporting user curves yet
    //boost::optional<UserConvectionModels> convectionCoefficient2UserCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setSurfaceType(const std::string& surfaceType);

    void resetSurfaceType();

    bool setConvectionCoefficient1Location(const std::string& convectionCoefficient1Location);

    void resetConvectionCoefficient1Location();

    bool setConvectionCoefficient1Type(const std::string& convectionCoefficient1Type);

    void resetConvectionCoefficient1Type();

    bool setConvectionCoefficient1(double convectionCoefficient1);

    void resetConvectionCoefficient1();

    bool setConvectionCoefficient1Schedule(Schedule& schedule);

    void resetConvectionCoefficient1Schedule();

    // DLM: not supporting user curves yet
    //bool setConvectionCoefficient1UserCurve(const UserConvectionModels& userConvectionModels);

    //void resetConvectionCoefficient1UserCurve();

    bool setConvectionCoefficient2Location(const std::string& convectionCoefficient2Location);

    void resetConvectionCoefficient2Location();

    bool setConvectionCoefficient2Type(const std::string& convectionCoefficient2Type);

    void resetConvectionCoefficient2Type();

    bool setConvectionCoefficient2(double convectionCoefficient2);

    void resetConvectionCoefficient2();

    bool setConvectionCoefficient2Schedule(Schedule& schedule);

    void resetConvectionCoefficient2Schedule();

    // DLM: not supporting user curves yet
    //bool setConvectionCoefficient2UserCurve(const UserConvectionModels& userConvectionModels);

    //void resetConvectionCoefficient2UserCurve();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl;

    explicit SurfacePropertyConvectionCoefficientsMultipleSurface(
      std::shared_ptr<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl> impl);

    friend class detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SurfacePropertyConvectionCoefficientsMultipleSurface");
  };

  /** \relates SurfacePropertyConvectionCoefficientsMultipleSurface*/
  using OptionalSurfacePropertyConvectionCoefficientsMultipleSurface = boost::optional<SurfacePropertyConvectionCoefficientsMultipleSurface>;

  /** \relates SurfacePropertyConvectionCoefficientsMultipleSurface*/
  using SurfacePropertyConvectionCoefficientsMultipleSurfaceVector = std::vector<SurfacePropertyConvectionCoefficientsMultipleSurface>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTSMULTIPLESURFACE_HPP
