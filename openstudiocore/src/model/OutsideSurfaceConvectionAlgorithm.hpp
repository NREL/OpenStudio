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

#ifndef MODEL_OUTSIDESURFACECONVECTIONALGORITHM_HPP
#define MODEL_OUTSIDESURFACECONVECTIONALGORITHM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {
  class OutsideSurfaceConvectionAlgorithm_Impl;
} // detail

/** OutsideSurfaceConvectionAlgorithm derives from ModelObject and is an interface to the OpenStudio IDD object named "SurfaceConvectionAlgorithm:Outside".
 *
 *  OutsideSurfaceConvectionAlgorithm is a unique object that specifies the global model used for surface convection at the outside face of all the 
 *  heat transfer surfaces in the model.  This global algorithm may be overridden for specific Surface, SurfaceList, or Zone objects. 
 *  OutsideSurfaceConvectionAlgorithm does not have a public constructor because it is a unique ModelObject.  
 *  To get the OutsideSurfaceConvectionAlgorithm object for a Model or create one if it does not yet exist use model.getUniqueObject<OutsideSurfaceConvectionAlgorithm>().  
 *  To get the OutsideSurfaceConvectionAlgorithm object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<OutsideSurfaceConvectionAlgorithm>().
 */
class MODEL_API OutsideSurfaceConvectionAlgorithm : public ModelObject {
 public:
  virtual ~OutsideSurfaceConvectionAlgorithm() {}

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

  /// Constructs a new OutsideSurfaceConvectionAlgorithm object in the model.
  explicit OutsideSurfaceConvectionAlgorithm(const Model& model);

  //@}

  /// @cond

  typedef detail::OutsideSurfaceConvectionAlgorithm_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  // constructor
  explicit OutsideSurfaceConvectionAlgorithm(std::shared_ptr<detail::OutsideSurfaceConvectionAlgorithm_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.OutsideSurfaceConvectionAlgorithm");

  /// @endcond

};

/** \relates OutsideSurfaceConvectionAlgorithm */
typedef boost::optional<OutsideSurfaceConvectionAlgorithm> OptionalOutsideSurfaceConvectionAlgorithm;

} // model
} // openstudio

#endif // MODEL_OUTSIDESURFACECONVECTIONALGORITHM_HPP
