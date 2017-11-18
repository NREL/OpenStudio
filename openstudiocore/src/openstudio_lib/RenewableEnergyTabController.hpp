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

#ifndef OPENSTUDIO_REWABLE_ENERGY_TAB_CONTROLLER_HPP
#define OPENSTUDIO_REWABLE_ENERGY_TAB_CONTROLLER_HPP

#include "MainTabController.hpp"

namespace openstudio {

class RenewableEnergyController;

class RenewableEnergyTabController : public MainTabController
{
  Q_OBJECT

  public:

    RenewableEnergyTabController(bool isIP, const openstudio::model::Model& model);

    virtual ~RenewableEnergyTabController();

  private:

    std::shared_ptr<RenewableEnergyController> m_RenewableEnergyController;

  public slots:

    void toggleUnits(bool displayIP);
};

} // openstudio

#endif // OPENSTUDIO_REWABLE_ENERGY_TAB_CONTROLLER_HPP
