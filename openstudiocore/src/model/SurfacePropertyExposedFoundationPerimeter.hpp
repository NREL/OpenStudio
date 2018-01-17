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

} // detail

/** SurfacePropertyExposedFoundationPerimeter is a ModelObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:ExposedFoundationPerimeter'. */
class MODEL_API SurfacePropertyExposedFoundationPerimeter : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{
                                                     
  explicit SurfacePropertyExposedFoundationPerimeter(Surface& surface,
                                                     std::string exposedPerimeterCalculationMethod,
                                                     double exposedPerimeter);

  virtual ~SurfacePropertyExposedFoundationPerimeter() {}

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

  bool setExposedPerimeterCalculationMethod(std::string exposedPerimeterCalculationMethod);
  
  bool setTotalExposedPerimeter(double totalExposedPerimeter);
  
  bool setExposedPerimeterFraction(double exposedPerimeterFraction);
  
  void resetExposedPerimeterFraction();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SurfacePropertyExposedFoundationPerimeter_Impl ImplType;

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
typedef boost::optional<SurfacePropertyExposedFoundationPerimeter> OptionalSurfacePropertyExposedFoundationPerimeter;

/** \relates SurfacePropertyExposedFoundationPerimeter*/
typedef std::vector<SurfacePropertyExposedFoundationPerimeter> SurfacePropertyExposedFoundationPerimeterVector;

} // model
} // openstudio

#endif // MODEL_SURFACEPROPERTYEXPOSEDFOUNDATIONPERIMETER_HPP