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

#ifndef RUNMANAGER_LIB_JOBTYPE_HPP
#define RUNMANAGER_LIB_JOBTYPE_HPP

#include "../../utilities/core/Enum.hpp"

namespace openstudio {
namespace runmanager {

/** \class JobType
 *  \brief Enumeration of RunManager job types.
 *  \details JobType::ModelObjectPerturbation has been deprecated. See the
 *  OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(JobType,
                ((Workflow))
                ((EnergyPlus))
                ((ModelToIdf))
                ((CalculateEconomics))
                ((ExpandObjects))
                ((ReadVars))
                ((XMLPreprocessor))
                ((PreviewIES))
                ((Ruby))
                ((Null))
                ((ModelObjectPerturbation))
                ((EnergyPlusPostProcess))
                ((ParallelEnergyPlusSplit))
                ((ParallelEnergyPlusJoin))
                ((IdfToModel))
                ((EnergyPlusPreProcess))
                ((Basement))
                ((Slab))
                ((OpenStudioPostProcess))
                ((ModelToRad))
                ((ModelToRadPreProcess))
                ((Dakota))
                ((UserScript))
                );
 *  \endcode */
OPENSTUDIO_ENUM(JobType,
                ((Workflow))
                ((EnergyPlus))
                ((ModelToIdf))
                ((CalculateEconomics))
                ((ExpandObjects))
                ((ReadVars))
                ((XMLPreprocessor))
                ((PreviewIES))
                ((Ruby))
                ((Null))
                ((ModelObjectPerturbation))
                ((EnergyPlusPostProcess))
                ((ParallelEnergyPlusSplit))
                ((ParallelEnergyPlusJoin))
                ((IdfToModel))
                ((EnergyPlusPreProcess))
                ((Basement))
                ((Slab))
                ((OpenStudioPostProcess))
                ((ModelToRad))
                ((ModelToRadPreProcess))
                ((Dakota))
                ((UserScript))
                );

}
}


#endif // RUNMANAGER_LIB_JOBTYPE_HPP
