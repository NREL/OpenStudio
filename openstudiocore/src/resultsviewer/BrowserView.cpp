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

#include "BrowserView.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>


namespace resultsviewer{

BrowserView::BrowserView( QWidget* parent): QTextBrowser(parent)
{
  setAttribute(Qt::WA_DeleteOnClose);
  setReadOnly(true);
  setOpenExternalLinks(true);
  setFrameStyle(QFrame::Plain);
  m_filename = "";
  m_alias = "";
}


void BrowserView::closeEvent(QCloseEvent *evt)
{
  emit(signalClose(this));
}


void BrowserView::setFilename(const QString& filename)
{
  if (!filename.isEmpty())
  {
    m_filename = filename;
  }
}

void BrowserView::setAlias(const QString& alias)
{
  if (!alias.isEmpty())
  {
    m_alias = alias;
    if (!m_alias.isEmpty()) setWindowTitle(tr("(%1) EplusTbl.htm").arg(m_alias));
  }
}

void BrowserView::updateAlias(const QString& alias, const QString& filename)
{
  if ( (!filename.isEmpty()) && (!alias.isEmpty()) )
  {
    if (m_filename.toUpper() == filename.toUpper()) 
    {
      m_alias = alias;
      setWindowTitle(tr("(%1) EplusTbl.htm").arg(m_alias));
    }
  }
}


void BrowserView::mouseDoubleClickEvent(QMouseEvent *evt)
{
  emit(signalFloatOrDockMe(this));
}


void BrowserView::leaveEvent(QEvent *evt)
{
  QCursor::pos();
}

};
