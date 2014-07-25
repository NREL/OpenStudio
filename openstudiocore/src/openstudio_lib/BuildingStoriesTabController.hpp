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

#ifndef OPENSTUDIO_BUILDINGSTORIESTABCONTROLLER_HPP
#define OPENSTUDIO_BUILDINGSTORIESTABCONTROLLER_HPP

#include "MainTabController.hpp"

namespace openstudio {

class BuildingStoriesController;

class BuildingStoriesTabController : public MainTabController
{
  Q_OBJECT

  public:

    BuildingStoriesTabController(const openstudio::model::Model& model);

    virtual ~BuildingStoriesTabController() {}

  private:

    std::shared_ptr<BuildingStoriesController> m_buildingStoriesController;
 
};

} // openstudio

#endif // OPENSTUDIO_BUILDINGSTORIESTABCONTROLLER_HPP
