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

#ifndef MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_HPP
#define MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class PlanarSurface;
  class SolarCollectorPerformanceIntegralCollectorStorage;

  namespace detail {

    class SolarCollectorIntegralCollectorStorage_Impl;

  }  // namespace detail

  /** SolarCollectorIntegralCollectorStorage is a StraightComponent that wraps the OpenStudio IDD object 'OS:SolarCollector:IntegralCollectorStorage'. */
  class MODEL_API SolarCollectorIntegralCollectorStorage : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SolarCollectorIntegralCollectorStorage(const Model& model);

    virtual ~SolarCollectorIntegralCollectorStorage() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SolarCollectorIntegralCollectorStorage(const SolarCollectorIntegralCollectorStorage& other) = default;
    SolarCollectorIntegralCollectorStorage(SolarCollectorIntegralCollectorStorage&& other) = default;
    SolarCollectorIntegralCollectorStorage& operator=(const SolarCollectorIntegralCollectorStorage&) = default;
    SolarCollectorIntegralCollectorStorage& operator=(SolarCollectorIntegralCollectorStorage&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> bottomSurfaceBoundaryConditionsTypeValues();

    /** @name Getters */
    //@{

    SolarCollectorPerformanceIntegralCollectorStorage solarCollectorPerformance() const;

    boost::optional<PlanarSurface> surface() const;

    std::string bottomSurfaceBoundaryConditionsType() const;

    bool isBottomSurfaceBoundaryConditionsTypeDefaulted() const;

    boost::optional<double> maximumFlowRate() const;

    //@}
    /** @name Setters */
    //@{

    /// Deletes the current parameters and clones the parameters passed in
    bool setSolarCollectorPerformance(const SolarCollectorPerformanceIntegralCollectorStorage& solarCollectorPerformance);

    /// Deletes the current parameters and constructs a new default set of parameters
    void resetSolarCollectorPerformance();

    bool setSurface(const PlanarSurface& surface);

    void resetSurface();

    bool setMaximumFlowRate(double maximumFlowRate);

    void resetMaximumFlowRate();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SolarCollectorIntegralCollectorStorage_Impl;

    explicit SolarCollectorIntegralCollectorStorage(std::shared_ptr<detail::SolarCollectorIntegralCollectorStorage_Impl> impl);

    friend class detail::SolarCollectorIntegralCollectorStorage_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SolarCollectorIntegralCollectorStorage");
  };

  /** \relates SolarCollectorIntegralCollectorStorage*/
  using OptionalSolarCollectorIntegralCollectorStorage = boost::optional<SolarCollectorIntegralCollectorStorage>;

  /** \relates SolarCollectorIntegralCollectorStorage*/
  using SolarCollectorIntegralCollectorStorageVector = std::vector<SolarCollectorIntegralCollectorStorage>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_HPP
