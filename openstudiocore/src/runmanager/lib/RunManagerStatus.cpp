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

#include "RunManagerStatus.hpp"

#include "../../utilities/core/Application.hpp"

namespace openstudio {
namespace runmanager {

  RunManagerStatus::RunManagerStatus(QWidget *parent, Qt::WindowFlags flags, const RunManager &t_runmanager)
    : QDialog(parent, flags)
  {
    ui.setupUi(this);

    if (Application::instance().isDefaultInstance())
    {
      QIcon icon = QIcon(":/images/rm_16.png");
      icon.addPixmap(QPixmap(":/images/rm_32.png"));
      icon.addPixmap(QPixmap(":/images/rm_48.png"));
      icon.addPixmap(QPixmap(":/images/rm_64.png"));
      icon.addPixmap(QPixmap(":/images/rm_128.png"));
      icon.addPixmap(QPixmap(":/images/rm_256.png"));
      setWindowIcon(icon);
    }

    m_statuswidget = new JobStatusWidget(t_runmanager, this);

    ui.mainLayout->addWidget(m_statuswidget);
    

    std::vector<std::pair<bool, QAction *> > acts = m_statuswidget->publicActions();

    for (size_t i = 0; i < acts.size(); ++i)
    {
      ui.toolBar->addAction(acts.at(i).second);
    }
   
  }

  RunManagerStatus::~RunManagerStatus()
  {
  }


} // runmanager
} // openstudio


