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

#ifndef MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTSMULTIPLESURFACE_IMPL_HPP
#define MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTSMULTIPLESURFACE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** SurfacePropertyConvectionCoefficientsMultipleSurface_Impl is a ModelObject_Impl that is the implementation class for SurfacePropertyConvectionCoefficientsMultipleSurface.*/
  class MODEL_API SurfacePropertyConvectionCoefficientsMultipleSurface_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    SurfacePropertyConvectionCoefficientsMultipleSurface_Impl(const IdfObject& idfObject,
                                                              Model_Impl* model,
                                                              bool keepHandle);

    SurfacePropertyConvectionCoefficientsMultipleSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                              Model_Impl* model,
                                                              bool keepHandle);

    SurfacePropertyConvectionCoefficientsMultipleSurface_Impl(const SurfacePropertyConvectionCoefficientsMultipleSurface_Impl& other,
                                                              Model_Impl* model,
                                                              bool keepHandle);

    virtual ~SurfacePropertyConvectionCoefficientsMultipleSurface_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<std::string> surfaceType() const;

    boost::optional<std::string> convectionCoefficient1Location() const;

    boost::optional<std::string> convectionCoefficient1Type() const;

    boost::optional<double> convectionCoefficient1() const;

    boost::optional<Schedule> convectionCoefficient1Schedule() const;

    //boost::optional<UserConvectionModels> convectionCoefficient1UserCurve() const;

    boost::optional<std::string> convectionCoefficient2Location() const;

    boost::optional<std::string> convectionCoefficient2Type() const;

    boost::optional<double> convectionCoefficient2() const;

    boost::optional<Schedule> convectionCoefficient2Schedule() const;

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

    //bool setConvectionCoefficient1UserCurve(const boost::optional<UserConvectionModels>& userConvectionModels);

    //void resetConvectionCoefficient1UserCurve();

    bool setConvectionCoefficient2Location(const std::string& convectionCoefficient2Location);

    void resetConvectionCoefficient2Location();

    bool setConvectionCoefficient2Type(const std::string& convectionCoefficient2Type);

    void resetConvectionCoefficient2Type();

    bool setConvectionCoefficient2(double convectionCoefficient2);

    void resetConvectionCoefficient2();

    bool setConvectionCoefficient2Schedule(Schedule& schedule);

    void resetConvectionCoefficient2Schedule();

    //bool setConvectionCoefficient2UserCurve(const boost::optional<UserConvectionModels>& userConvectionModels);

    //void resetConvectionCoefficient2UserCurve();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.SurfacePropertyConvectionCoefficientsMultipleSurface");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTSMULTIPLESURFACE_IMPL_HPP

