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

#include "ChangeAliasDialog.hpp"
#include <QFile>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

namespace resultsviewer{

ChangeAliasDialog::ChangeAliasDialog(const QString& oldAlias, const QString& filename, QWidget* parent):QDialog(parent)
{
  m_newAlias = "";

  int boxWidth = fontMetrics().width(filename) + 20;
  QPalette p(palette());
  p.setColor(QPalette::Base, Qt::lightGray);    

  QLabel *fileLabel = new QLabel(tr("&EnergyPlus file:"));
  auto fileName = new QLineEdit(filename);
  fileLabel->setBuddy(fileName);
  fileName->setReadOnly(true);
  fileName->setPalette(p);
  fileName->setMinimumWidth(boxWidth);

  QLabel *oldAliasLabel = new QLabel(tr("&Old alias:"));
  auto oldAliasName = new QLineEdit(oldAlias);
  oldAliasLabel->setBuddy(oldAliasName);
  oldAliasName->setReadOnly(true);
  oldAliasName->setPalette(p);
  oldAliasName->setMinimumWidth(boxWidth);

  QLabel *newAliasLabel = new QLabel(tr("&New alias:"));
  m_newAliasName = new QLineEdit;
  newAliasLabel->setBuddy(m_newAliasName);
  m_newAliasName->setMinimumWidth(boxWidth);

  m_updateButton = new QPushButton(tr("Update"));
  m_updateButton->setDefault(true);
  m_updateButton->setEnabled(false);

  QPushButton *cancelButton = new QPushButton(tr("Cancel"));

  connect(m_updateButton, SIGNAL(clicked()), this, SLOT(updateClicked()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
  connect(m_newAliasName, SIGNAL(textChanged(const QString&)), this, SLOT(enableUpdateButton(const QString&)));

// layout
  auto gridLayout = new QGridLayout;
  gridLayout->addWidget(fileLabel, 0, 0);
  gridLayout->addWidget(fileName, 0, 1);
  gridLayout->addWidget(oldAliasLabel, 1, 0);
  gridLayout->addWidget(oldAliasName, 1, 1);
  gridLayout->addWidget(newAliasLabel, 2, 0);
  gridLayout->addWidget(m_newAliasName, 2, 1);

  auto buttonLayout = new QHBoxLayout;
  buttonLayout->addStretch();
  buttonLayout->addWidget(m_updateButton);
  buttonLayout->addWidget(cancelButton);

  auto mainLayout = new QVBoxLayout;
  mainLayout->addLayout(gridLayout);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);

  setWindowTitle(tr("Change alias"));
  setFixedHeight(sizeHint().height());
}

void ChangeAliasDialog::enableUpdateButton(const QString& alias)
{
  m_updateButton->setEnabled(!m_newAliasName->text().isEmpty());
}

void ChangeAliasDialog::updateClicked()
{
  m_newAlias = m_newAliasName->text();
  if (!m_newAlias.isEmpty()) accept();
}

};
