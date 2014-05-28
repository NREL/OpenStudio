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

#include "OpenStudioAlgorithm.hpp"
#include "OpenStudioAlgorithm_Impl.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  OpenStudioAlgorithm_Impl::OpenStudioAlgorithm_Impl(const std::string& name,
                                                     const AlgorithmOptions& options)
    : Algorithm_Impl(name,options)
  {}

  OpenStudioAlgorithm_Impl::OpenStudioAlgorithm_Impl(const UUID& uuid,
                                                     const UUID& versionUUID,
                                                     const std::string& name,
                                                     const std::string& displayName,
                                                     const std::string& description,
                                                     bool complete,
                                                     bool failed,
                                                     int iter,
                                                     const AlgorithmOptions& options)
    : Algorithm_Impl(uuid,versionUUID,name,displayName,description,complete,failed,iter,options)
  {}

  OpenStudioAlgorithm_Impl::OpenStudioAlgorithm_Impl(const OpenStudioAlgorithm_Impl &other)
    : Algorithm_Impl(other)
  {}

} // detail

int OpenStudioAlgorithm::createNextIteration(Analysis& analysis) {
  return getImpl<detail::OpenStudioAlgorithm_Impl>()->createNextIteration(analysis);
}

/// @cond
OpenStudioAlgorithm::OpenStudioAlgorithm(std::shared_ptr<detail::OpenStudioAlgorithm_Impl> impl)
  : Algorithm(impl)
{}
/// @endcond

} // analysis
} // openstudio

