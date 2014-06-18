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

#ifndef MODEL_CONVERGENCELIMITS_IMPL_HPP
#define MODEL_CONVERGENCELIMITS_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** ConvergenceLimits_Impl is a ModelObject_Impl that is the implementation class for ConvergenceLimits.*/
  class MODEL_API ConvergenceLimits_Impl : public ModelObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(boost::optional<int> minimumSystemTimestep READ minimumSystemTimestep WRITE setMinimumSystemTimestep RESET resetMinimumSystemTimestep);
    Q_PROPERTY(int maximumHVACIterations READ maximumHVACIterations WRITE setMaximumHVACIterations RESET resetMaximumHVACIterations);
    Q_PROPERTY(bool isMaximumHVACIterationsDefaulted READ isMaximumHVACIterationsDefaulted);
    Q_PROPERTY(int minimumPlantIterations READ minimumPlantIterations WRITE setMinimumPlantIterations RESET resetMinimumPlantIterations);
    Q_PROPERTY(bool isMinimumPlantIterationsDefaulted READ isMinimumPlantIterationsDefaulted);
    Q_PROPERTY(int maximumPlantIterations READ maximumPlantIterations WRITE setMaximumPlantIterations RESET resetMaximumPlantIterations);
    Q_PROPERTY(bool isMaximumPlantIterationsDefaulted READ isMaximumPlantIterationsDefaulted);
   public:
    /** @name Constructors and Destructors */
    //@{

    ConvergenceLimits_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ConvergenceLimits_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    ConvergenceLimits_Impl(const ConvergenceLimits_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    virtual ~ConvergenceLimits_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    // set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<int> minimumSystemTimestep() const;

    int maximumHVACIterations() const;

    bool isMaximumHVACIterationsDefaulted() const;

    int minimumPlantIterations() const;

    bool isMinimumPlantIterationsDefaulted() const;

    int maximumPlantIterations() const;

    bool isMaximumPlantIterationsDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setMinimumSystemTimestep(boost::optional<int> minimumSystemTimestep);

    void resetMinimumSystemTimestep();

    bool setMaximumHVACIterations(int maximumHVACIterations);

    void resetMaximumHVACIterations();

    bool setMinimumPlantIterations(int minimumPlantIterations);

    void resetMinimumPlantIterations();

    bool setMaximumPlantIterations(int maximumPlantIterations);

    void resetMaximumPlantIterations();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ConvergenceLimits");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CONVERGENCELIMITS_IMPL_HPP

