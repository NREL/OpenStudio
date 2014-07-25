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

#include "SummaryTabView.hpp"

#include "../utilities/core/Assert.hpp"

#include <QLabel>

namespace openstudio {

SummaryTabView::SummaryTabView(const model::Model & model,
                               QWidget * parent)
                               : MainTabView("Building Summary",false,parent)
{
  // Hack code to remove when tab active
  QLabel * underConstructionLabel = new QLabel();
  underConstructionLabel->setPixmap(QPixmap(":/images/coming_soon_building_summary.png"));
  underConstructionLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  bool success = this->addTabWidget(underConstructionLabel);
  OS_ASSERT(success);
}

} // openstudio

