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

#ifndef RUNMANAGER_LIB_POSTPROCESSREPORTING_HPP
#define RUNMANAGER_LIB_POSTPROCESSREPORTING_HPP

#include <vector>

#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/sql/SqlFile.hpp"

namespace openstudio {
namespace runmanager {
namespace detail {


  class PostProcessReporting
  {
    public:
      static std::vector<Attribute> go(const SqlFile &t_sqlFile, const std::string& jobType);

    private:
      REGISTER_LOGGER("openstudio.runmanager.PostProcessReporting");

  };


} // detail
} // runmanager
} // openstudio

#endif // RUNMANAGER_LIB_POSTPROCESSREPORTING_HPP

