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

#include "JobOutputCleanup.hpp"

namespace openstudio {
namespace runmanager {

  JobOutputCleanup::JobOutputCleanup(QWidget *parent, Qt::WindowFlags flags, const std::vector<std::string> &files)
    : QDialog(parent, flags)
  {
    ui.setupUi(this);

    for (size_t i = 0; i < files.size(); ++i)
    {
      QListWidgetItem *item = new QListWidgetItem(toQString(files[i]));

      item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
      item->setCheckState(Qt::Checked);
      
      ui.listWidget->addItem(item);
    }
  }

  JobOutputCleanup::~JobOutputCleanup()
  {
  }

  std::vector<std::string> JobOutputCleanup::getSelectedFiles()
  {
    std::vector<std::string> res;

    for (int i = 0; i < ui.listWidget->count(); ++i)
    {
      QListWidgetItem *item = ui.listWidget->item(i);
      if (item->checkState() == Qt::Checked)
      {
        res.push_back(toString(item->text()));
      }
    }
    return res;
  }


} // runmanager
} // openstudio


