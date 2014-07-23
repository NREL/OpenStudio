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

#include "IGPrecisionDialog.hpp"

#include <QtGui>

IGPrecisionDialog::IGPrecisionDialog(QWidget* parent):
  QDialog(parent)
{
  setupUi(this);
  QRegExp regExp("[1-9][0-9]{0,1}");
  lineEdit->setValidator( new QRegExpValidator(regExp, this) );
}

void IGPrecisionDialog::on_buttonBox_accepted()
{
  emit accepted();
  this->hide();
}

void IGPrecisionDialog::on_buttonBox_rejected()
{
  emit rejected();
  this->hide();
}
