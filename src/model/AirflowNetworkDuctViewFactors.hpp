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

#ifndef MODEL_AIRFLOWNETWORKDUCTVIEWFACTORS_HPP
#define MODEL_AIRFLOWNETWORKDUCTVIEWFACTORS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include <vector>
#include <unordered_map>
#include "PlanarSurface.hpp"

namespace openstudio {

namespace model {

  class AirflowNetworkLinkage;

  namespace detail {

    class AirflowNetworkDuctViewFactors_Impl;

  }  // namespace detail

  /** This class implements a single point of a viewFactors, meant to replace std::pair<PlanarSurface, double> */
  class MODEL_API ViewFactorData
  {
   public:
    ViewFactorData(const PlanarSurface& s, double y);

    PlanarSurface planarSurface() const;
    double viewFactor() const;

   private:
    openstudio::model::PlanarSurface m_planarSurface;
    double m_viewFactor;
  };

  // Overload operator<<
  MODEL_API std::ostream& operator<<(std::ostream& out, const openstudio::model::ViewFactorData& vf);

  /** AirflowNetworkDuctViewFactors is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkDuctViewFactors'. */
  class MODEL_API AirflowNetworkDuctViewFactors : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirflowNetworkDuctViewFactors(const Model& model);

    virtual ~AirflowNetworkDuctViewFactors() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkDuctViewFactors(const AirflowNetworkDuctViewFactors& other) = default;
    AirflowNetworkDuctViewFactors(AirflowNetworkDuctViewFactors&& other) = default;
    AirflowNetworkDuctViewFactors& operator=(const AirflowNetworkDuctViewFactors&) = default;
    AirflowNetworkDuctViewFactors& operator=(AirflowNetworkDuctViewFactors&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    AirflowNetworkLinkage linkage() const;

    double ductSurfaceExposureFraction() const;

    bool isDuctSurfaceExposureFractionDefaulted() const;

    double ductSurfaceEmittance() const;

    bool isDuctSurfaceEmittanceDefaulted() const;

    boost::optional<double> getViewFactor(const PlanarSurface& surf) const;

    std::vector<ViewFactorData> viewFactors() const;

    //std::unordered_map<PlanarSurface, double> viewFactorMap() const;

    //@}
    /** @name Setters */
    //@{

    bool setLinkage(const AirflowNetworkLinkage& linkage);

    bool setDuctSurfaceExposureFraction(double ductSurfaceExposureFraction);

    void resetDuctSurfaceExposureFraction();

    bool setDuctSurfaceEmittance(double ductSurfaceEmittance);

    void resetDuctSurfaceEmittance();

    bool setViewFactor(const PlanarSurface& surf, double F);
    bool removeViewFactor(const PlanarSurface& surf);
    void resetViewFactors();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkDuctViewFactors_Impl;

    explicit AirflowNetworkDuctViewFactors(std::shared_ptr<detail::AirflowNetworkDuctViewFactors_Impl> impl);

    friend class detail::AirflowNetworkDuctViewFactors_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkDuctViewFactors");
  };

  /** \relates AirflowNetworkDuctViewFactors*/
  using OptionalAirflowNetworkDuctViewFactors = boost::optional<AirflowNetworkDuctViewFactors>;

  /** \relates AirflowNetworkDuctViewFactors*/
  using AirflowNetworkDuctViewFactorsVector = std::vector<AirflowNetworkDuctViewFactors>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKDUCTVIEWFACTORS_HPP
