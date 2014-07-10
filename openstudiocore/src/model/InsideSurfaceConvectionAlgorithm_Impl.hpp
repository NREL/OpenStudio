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

#ifndef MODEL_INSIDESURFACECONVECTIONALGORITHM_IMPL_HPP
#define MODEL_INSIDESURFACECONVECTIONALGORITHM_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API InsideSurfaceConvectionAlgorithm_Impl : public ModelObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(std::string algorithm READ algorithm WRITE setAlgorithm RESET resetAlgorithm);
    Q_PROPERTY(bool isAlgorithmDefaulted READ isAlgorithmDefaulted);
   public:
    /** @name Constructors and Destructors */
    //@{

    // constructor
    InsideSurfaceConvectionAlgorithm_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    InsideSurfaceConvectionAlgorithm_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                                          Model_Impl* model, 
                                          bool keepHandle);

    // clone copy constructor
    InsideSurfaceConvectionAlgorithm_Impl(const InsideSurfaceConvectionAlgorithm_Impl& other, 
                                          Model_Impl* model, 
                                          bool keepHandle);

    // virtual destructor
    virtual ~InsideSurfaceConvectionAlgorithm_Impl(){}

    //@}
    /** @name Virtual Methods */
    //@{

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    // set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

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
   private:

    REGISTER_LOGGER("openstudio.model.InsideSurfaceConvectionAlgorithm");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_INSIDESURFACECONVECTIONALGORITHM_IMPL_HPP
