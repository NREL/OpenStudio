/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_AIRFLOWNETWORKDUCTVIEWFACTORS_IMPL_HPP
#define MODEL_AIRFLOWNETWORKDUCTVIEWFACTORS_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class AirflowNetworkLinkage;

namespace detail {

  /** AirflowNetworkDuctViewFactors_Impl is a ModelObject_Impl that is the implementation class for AirflowNetworkDuctViewFactors.*/
  class MODEL_API AirflowNetworkDuctViewFactors_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirflowNetworkDuctViewFactors_Impl(const IdfObject& idfObject,
                                       Model_Impl* model,
                                       bool keepHandle);

    AirflowNetworkDuctViewFactors_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle);

    AirflowNetworkDuctViewFactors_Impl(const AirflowNetworkDuctViewFactors_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle);

    virtual ~AirflowNetworkDuctViewFactors_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    AirflowNetworkLinkage linkage() const;

    double ductSurfaceExposureFraction() const;

    bool isDuctSurfaceExposureFractionDefaulted() const;

    double ductSurfaceEmittance() const;

    bool isDuctSurfaceEmittanceDefaulted() const;

    boost::optional<double> getViewFactor(const PlanarSurface &surf) const;

    std::vector<std::pair<PlanarSurface, double>> viewFactors() const;

    //std::unordered_map<PlanarSurface, double> viewFactorMap() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: AirflowNetworkComponent.
    bool setLinkage(const AirflowNetworkLinkage& linkage);

    bool setDuctSurfaceExposureFraction(double ductSurfaceExposureFraction);

    void resetDuctSurfaceExposureFraction();

    bool setDuctSurfaceEmittance(double ductSurfaceEmittance);

    void resetDuctSurfaceEmittance();

    bool setViewFactor(const PlanarSurface &surf, double F);
    bool removeViewFactor(const PlanarSurface &surf);
    void resetViewFactors();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkDuctViewFactors");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<AirflowNetworkLinkage> optionalLinkage() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRFLOWNETWORKDUCTVIEWFACTORS_IMPL_HPP

