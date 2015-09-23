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

#include "TextEditDialog.hpp"

#include <QBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QSizePolicy>
#include <QTextEdit>

namespace openstudio {

TextEditDialog::TextEditDialog(const QString & windowTitle,
  const QString & windowMessage,
  QWidget* parent)
  : OSDialog(false, parent),
  m_windowMessage(windowMessage)
{
  setWindowTitle(windowTitle);
  setWindowModality(Qt::ApplicationModal);
  setSizeGripEnabled(true);
  createWidgets();
}

TextEditDialog::~TextEditDialog()
{
}

QString TextEditDialog::text()
{
  return m_textEdit->toPlainText();
}

void TextEditDialog::setText(const QString & text)
{
  m_textEdit->setText(text);
}

void TextEditDialog::createWidgets()
{
  m_textEdit = new QTextEdit(m_windowMessage);
  m_textEdit->setReadOnly(true);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  upperLayout()->addWidget(m_textEdit);

  // BUTTONS

  this->m_cancelButton->hide();

  // OS SETTINGS

  #ifdef Q_OS_MAC
    setWindowFlags(Qt::FramelessWindowHint);
  #elif defined(Q_OS_WIN)
    setWindowFlags(Qt::WindowCloseButtonHint);
  #endif

}

} // openstudio
