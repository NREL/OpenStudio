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

#ifndef RESULTSVIEWER_CHANGEALIASDIALOG_HPP
#define RESULTSVIEWER_CHANGEALIASDIALOG_HPP

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace resultsviewer{

/**
ChangeAliasDialog is ui dialog for changing file aliases.
*/
class ChangeAliasDialog : public QDialog
{
  Q_OBJECT
public:
  ChangeAliasDialog(const QString& oldAlias, const QString& filename, QWidget* parent=nullptr);
  const QString& newAlias() {return m_newAlias;}

private slots:
  void updateClicked();
  void enableUpdateButton(const QString& alias);

private:
  QString m_newAlias;
  QLineEdit *m_newAliasName;
  QPushButton *m_updateButton;
};

}; // resultsviewer namespace

#endif // RESULTSVIEWER_CHANGEALIASDIALOG_HPP
