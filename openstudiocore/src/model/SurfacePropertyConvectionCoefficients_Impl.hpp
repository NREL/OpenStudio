/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTS_IMPL_HPP
#define MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTS_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class PlanarSurface;
class Surface;
class SubSurface;

namespace detail {

  /** SurfacePropertyConvectionCoefficients_Impl is a ModelObject_Impl that is the implementation class for SurfacePropertyConvectionCoefficients.*/
  class MODEL_API SurfacePropertyConvectionCoefficients_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    SurfacePropertyConvectionCoefficients_Impl(const IdfObject& idfObject,
                                               Model_Impl* model,
                                               bool keepHandle);

    SurfacePropertyConvectionCoefficients_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle);

    SurfacePropertyConvectionCoefficients_Impl(const SurfacePropertyConvectionCoefficients_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle);

    virtual ~SurfacePropertyConvectionCoefficients_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<PlanarSurface> surface() const;

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

    bool setSurface(const PlanarSurface& surface);

    void resetSurface();

    bool setConvectionCoefficient1Location(const std::string& convectionCoefficient1Location);

    void resetConvectionCoefficient1Location();

    bool setConvectionCoefficient1Type(const std::string& convectionCoefficient1Type);

    void resetConvectionCoefficient1Type();

    bool setConvectionCoefficient1(double convectionCoefficient1);

    void resetConvectionCoefficient1();

    bool setConvectionCoefficient1Schedule(Schedule& schedule);

    void resetConvectionCoefficient1Schedule();

    // DLM: not supporting user curves yet
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

    // DLM: not supporting user curves yet
    //bool setConvectionCoefficient2UserCurve(const boost::optional<UserConvectionModels>& userConvectionModels);

    //void resetConvectionCoefficient2UserCurve();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.SurfacePropertyConvectionCoefficients");

  };

} // detail

} // model
} // openstudio

#endif // MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTS_IMPL_HPP

