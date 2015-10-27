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

#ifndef OPENSTUDIO_CONSTRUCTIONSTABCONTROLLER_HPP
#define OPENSTUDIO_CONSTRUCTIONSTABCONTROLLER_HPP

#include "MainTabController.hpp"

namespace openstudio {

class DefaultConstructionSetsController;
class ConstructionsController;
class MaterialsController;

class ConstructionsTabController : public MainTabController
{
  Q_OBJECT

  public:

    ConstructionsTabController(bool isIP, const openstudio::model::Model& model);

    virtual ~ConstructionsTabController();

    enum TabID
    {
      DEFAULT_CONSTRUCTIONS,
      CONSTRUCTIONS,
      MATERIALS
    };

  private:

    model::Model m_model;

    bool m_isIP;

    QObject * m_currentController = nullptr;

    int m_currentIndex = -1;

  public slots:

    virtual void setSubTab(int index) override;

};

} // openstudio

#endif // OPENSTUDIO_CONSTRUCTIONSTABCONTROLLER_HPP
