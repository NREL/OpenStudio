/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_FOUNDATIONKIVA_HPP
#define MODEL_FOUNDATIONKIVA_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"

#include "Material.hpp"
#include "Material_Impl.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"

namespace openstudio {
namespace model {

class Material;
class ConstructionBase;
class Surface;

namespace detail {

  class FoundationKiva_Impl;

} // detail

/** FoundationKiva is a ModelObject that wraps the OpenStudio IDD object 'OS:Foundation:Kiva'. */
class MODEL_API FoundationKiva : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit FoundationKiva(Model& model);

  virtual ~FoundationKiva() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Material> interiorHorizontalInsulationMaterial() const;

  double interiorHorizontalInsulationDepth() const;

  bool isInteriorHorizontalInsulationDepthDefaulted() const;

  boost::optional<double> interiorHorizontalInsulationWidth();

  boost::optional<Material> interiorVerticalInsulationMaterial() const;

  boost::optional<double> interiorVerticalInsulationDepth();

  boost::optional<Material> exteriorHorizontalInsulationMaterial() const;

  boost::optional<double> exteriorHorizontalInsulationDepth();

  double exteriorHorizontalInsulationWidth() const;

  bool isExteriorHorizontalInsulationWidthDefaulted() const;

  boost::optional<Material> exteriorVerticalInsulationMaterial() const;

  boost::optional<double> exteriorVerticalInsulationDepth();

  double wallHeightAboveGrade() const;

  bool isWallHeightAboveGradeDefaulted() const;

  double wallDepthBelowSlab() const;

  bool isWallDepthBelowSlabDefaulted() const;

  boost::optional<ConstructionBase> footingWallConstruction() const;

  boost::optional<Material> footingMaterial() const;

  double footingDepth() const;

  bool isFootingDepthDefaulted() const;

  std::vector<Surface> surfaces() const;

  //@}
  /** @name Setters */
  //@{

  bool setInteriorHorizontalInsulationMaterial(const Material& material);

  void resetInteriorHorizontalInsulationMaterial();

  bool setInteriorHorizontalInsulationDepth(double interiorHorizontalInsulationDepth);

  void resetInteriorHorizontalInsulationDepth();

  bool setInteriorHorizontalInsulationWidth(double interiorHorizontalInsulationWidth);

  bool setInteriorVerticalInsulationMaterial(const Material& material);

  void resetInteriorVerticalInsulationMaterial();

  bool setInteriorVerticalInsulationDepth(double interiorVerticalInsulationDepth);

  bool setExteriorHorizontalInsulationMaterial(const Material& material);

  void resetExteriorHorizontalInsulationMaterial();

  bool setExteriorHorizontalInsulationDepth(double exteriorHorizontalInsulationDepth);

  bool setExteriorHorizontalInsulationWidth(double exteriorHorizontalInsulationWidth);

  void resetExteriorHorizontalInsulationWidth();

  bool setExteriorVerticalInsulationMaterial(const Material& material);

  void resetExteriorVerticalInsulationMaterial();

  bool setExteriorVerticalInsulationDepth(double exteriorVerticalInsulationDepth);

  bool setWallHeightAboveGrade(double wallHeightAboveGrade);

  void resetWallHeightAboveGrade();

  bool setWallDepthBelowSlab(double wallDepthBelowSlab);

  void resetWallDepthBelowSlab();

  bool setFootingWallConstruction(const ConstructionBase& construction);

  void resetFootingWallConstruction();

  bool setFootingMaterial(const Material& material);

  void resetFootingMaterial();

  bool setFootingDepth(double footingDepth);

  void resetFootingDepth();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::FoundationKiva_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::FoundationKiva_Impl;

  explicit FoundationKiva(std::shared_ptr<detail::FoundationKiva_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.FoundationKiva");
};

/** \relates FoundationKiva*/
typedef boost::optional<FoundationKiva> OptionalFoundationKiva;

/** \relates FoundationKiva*/
typedef std::vector<FoundationKiva> FoundationKivaVector;

} // model
} // openstudio

#endif // MODEL_FOUNDATIONKIVA_HPP