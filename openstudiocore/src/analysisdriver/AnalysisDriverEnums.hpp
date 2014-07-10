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

#ifndef ANALYSISDRIVER_ANALYSISDRIVERENUMS_HPP
#define ANALYSISDRIVER_ANALYSISDRIVERENUMS_HPP

#include "AnalysisDriverAPI.hpp"

#include "../utilities/core/Enum.hpp"


namespace openstudio {

namespace analysisdriver {

/** \class AnalysisStatus
 *  \brief Status of an Analysis
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM(AnalysisStatus,
  ((Idle))
  ((Starting))
  ((Running))
  ((Stopping))
  ((Complete))
  ((Error)));
 *  \endcode */
OPENSTUDIO_ENUM(AnalysisStatus,
  ((Idle))
  ((Starting))
  ((Running))
  ((Stopping))
  ((Error)));
  
} // analysisdriver
} // openstudio

#endif // ANALYSISDRIVER_ANALYSISDRIVERENUMS_HPP

