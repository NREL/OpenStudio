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

#ifndef RUNMANAGER_LIB_RUNMANAGER_UTIL_HPP
#define RUNMANAGER_LIB_RUNMANAGER_UTIL_HPP

#include "FileInfo.hpp"
#include <QFileInfo>

namespace openstudio {
namespace runmanager {

  /// A small collection of utilities used internally by the RunManager
  class RunManager_Util {

    public: 
      /// \returns a fully populated FileInfo object from a QFileInfo object
      static FileInfo dirFile(QFileInfo fi);

      /// \returns a fully populated FileInfo object from a openstudio::path object
      static FileInfo dirFile(const openstudio::path &p);

  };

}
}

#endif // RUNMANAGER_LIB_RUNMANAGER_UTIL_HPP
