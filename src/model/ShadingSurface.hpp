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

#ifndef MODEL_SHADINGSURFACE_HPP
#define MODEL_SHADINGSURFACE_HPP

#include "ModelAPI.hpp"
#include "PlanarSurface.hpp"

namespace openstudio {
namespace model {

  class ShadingSurfaceGroup;
  class Schedule;
  class DaylightingDeviceShelf;

  namespace detail {

    class ShadingSurface_Impl;

  }  // namespace detail

  /** ShadingSurface is a PlanarSurface that wraps the OpenStudio IDD object
 *  'OS:ShadingSurface'. */
  class MODEL_API ShadingSurface : public PlanarSurface
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ShadingSurface(const std::vector<Point3d>& vertices, const Model& model);

    virtual ~ShadingSurface() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ShadingSurface(const ShadingSurface& other) = default;
    ShadingSurface(ShadingSurface&& other) = default;
    ShadingSurface& operator=(const ShadingSurface&) = default;
    ShadingSurface& operator=(ShadingSurface&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    /// get the shading group
    boost::optional<ShadingSurfaceGroup> shadingSurfaceGroup() const;

    // get the transmittance schedule
    boost::optional<Schedule> transmittanceSchedule() const;

    boost::optional<double> numberofVertices() const;

    bool isNumberofVerticesDefaulted() const;

    bool isNumberofVerticesAutocalculated() const;

    //@}
    /** @name Setters */
    //@{

    /// set the exterior shading group
    bool setShadingSurfaceGroup(const ShadingSurfaceGroup& shadingSurfaceGroup);

    void resetShadingSurfaceGroup();

    /// set the transmittance schedule
    bool setTransmittanceSchedule(Schedule& transmittanceSchedule);

    /// reset the transmittance schedule
    void resetTransmittanceSchedule();

    bool setNumberofVertices(boost::optional<double> numberofVertices);

    bool setNumberofVertices(double numberofVertices);

    void resetNumberofVertices();

    void autocalculateNumberofVertices();

    //@}

    boost::optional<DaylightingDeviceShelf> daylightingDeviceShelf() const;

   protected:
    /// @cond
    using ImplType = detail::ShadingSurface_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    explicit ShadingSurface(std::shared_ptr<detail::ShadingSurface_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ShadingSurface");
  };

  /** \relates ShadingSurface*/
  using OptionalShadingSurface = boost::optional<ShadingSurface>;

  /** \relates ShadingSurface*/
  using ShadingSurfaceVector = std::vector<ShadingSurface>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SHADINGSURFACE_HPP
