/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_INSIDESURFACECONVECTIONALGORITHM_HPP
#define MODEL_INSIDESURFACECONVECTIONALGORITHM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {
  class InsideSurfaceConvectionAlgorithm_Impl;
} // detail

/** InsideSurfaceConvectionAlgorithm derives from ModelObject and is an interface to the OpenStudio IDD object named "SurfaceConvectionAlgorithm:Inside".
 *
 *  InsideSurfaceConvectionAlgorithm is a unique object that specifies the global model used for surface convection at the inside face of all the 
 *  heat transfer surfaces in the model.  This global algorithm may be overridden for specific Surface, SurfaceList, or Zone objects. 
 *  InsideSurfaceConvectionAlgorithm does not have a public constructor because it is a unique ModelObject.  
 *  To get the InsideSurfaceConvectionAlgorithm object for a Model or create one if it does not yet exist use model.getUniqueObject<InsideSurfaceConvectionAlgorithm>().  
 *  To get the InsideSurfaceConvectionAlgorithm object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<InsideSurfaceConvectionAlgorithm>().
 */
class MODEL_API InsideSurfaceConvectionAlgorithm : public ModelObject {
  public:
  virtual ~InsideSurfaceConvectionAlgorithm() {}

  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  static std::vector<std::string> validAlgorithmValues();

  //@}
  /** @name Getters */
  //@{

  std::string algorithm() const;

  bool isAlgorithmDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setAlgorithm(std::string algorithm);

  void resetAlgorithm();

  //@}

  protected:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new InsideSurfaceConvectionAlgorithm object in the model.
  explicit InsideSurfaceConvectionAlgorithm(const Model& model);

  //@}

  /// @cond

  typedef detail::InsideSurfaceConvectionAlgorithm_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  // constructor
  explicit InsideSurfaceConvectionAlgorithm(
      std::shared_ptr<detail::InsideSurfaceConvectionAlgorithm_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.InsideSurfaceConvectionAlgorithm");

  /// @endcond

};

/** \relates InsideSurfaceConvectionAlgorithm */
typedef boost::optional<InsideSurfaceConvectionAlgorithm> OptionalInsideSurfaceConvectionAlgorithm;

} // model
} // openstudio

#endif // MODEL_INSIDESURFACECONVECTIONALGORITHM_HPP
