/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_BUILDINGCOMPONENTCONTROLLER_H
#define OPENSTUDIO_BUILDINGCOMPONENTCONTROLLER_H

#include <map>
#include <QObject>
#include <QString>
#include <openstudio_lib/OSDocument.hpp>

namespace openstudio {

class BuildingComponentController : public QObject
{
  Q_OBJECT

  public:

  BuildingComponentController();
  virtual ~BuildingComponentController() {}

  private:

  // Maps main tabs to tids, a tab / tid map.
  std::map<OSDocument::VerticalTabID,std::vector<QString> > m_tabTidbMap;
};

} // openstudio

#endif // OPENSTUDIO_BUILDINGCOMPONENTCONTROLLER_H

