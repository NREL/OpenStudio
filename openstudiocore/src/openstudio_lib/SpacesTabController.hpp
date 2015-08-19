/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_SPACESTABCONTROLLER_HPP
#define OPENSTUDIO_SPACESTABCONTROLLER_HPP

#include "MainTabController.hpp"

namespace openstudio {

  class SpacesTabController : public MainTabController
{
  Q_OBJECT

  public:

    SpacesTabController(bool isIP, const model::Model & model);

  virtual ~SpacesTabController() {}

  enum TabID
  {
    SPACES, // "Properties"
    LOADS,
    SURFACES,
    SUBSURFACES,
    INTERIOR_PARTITIONS,
    SHADING,
    DAYLIGHTING
  };

private slots:

void toggleUnits(bool displayIP);

};

} // openstudio

#endif // OPENSTUDIO_SPACESTABCONTROLLER_HPP
