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

#ifndef OPENSTUDIO_HVACSYSTEMSTABCONTROLLER_HPP
#define OPENSTUDIO_HVACSYSTEMSTABCONTROLLER_HPP

#include "MainTabController.hpp"
#include "../model/Model.hpp"

namespace openstudio {

class HVACSystemsController;

class HVACSystemsTabController : public MainTabController
{
  Q_OBJECT

  public:

  HVACSystemsTabController(bool isIP, const model::Model& model);

  virtual ~HVACSystemsTabController() {}

  void clearSceneSelection();

  private:

  std::shared_ptr<HVACSystemsController> m_hvacSystemsController;

  bool m_isIP;

  signals:
  
  void toggleUnitsClicked(bool displayIP);

private slots:

  void toggleUnits(bool displayIP);

};

} // openstudio

#endif // OPENSTUDIO_HVACSYSTEMSTABCONTROLLER_HPP
