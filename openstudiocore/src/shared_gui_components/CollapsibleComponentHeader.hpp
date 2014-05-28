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

#ifndef SHAREDGUICOMPONENTS_COLLAPSIBLECOMPONENTHEADER_HPP
#define SHAREDGUICOMPONENTS_COLLAPSIBLECOMPONENTHEADER_HPP

#include <QAbstractButton>

#include "Component.hpp"

class QLabel;
class QMouseEvent;
class QPaintEvent;
class QVBoxLayout;

namespace openstudio {

class PageNavigator;

class CollapsibleComponentHeader : public QAbstractButton
{
  Q_OBJECT

public:
  CollapsibleComponentHeader(const std::string& text,
    int numResults,
    int numPages,
    QWidget * parent = nullptr);
  virtual ~CollapsibleComponentHeader() {}
  void setChecked(bool isChecked);
  void setText(const QString& text);
  void setNumResults(int numResults);
  void setNumPages(int numPages);
  void firstPage();

protected:
  void paintEvent(QPaintEvent * event);
  QSize sizeHint() const;

private:
  void createLayout(const std::string& text, int numResults, int numPages);
  void setImage(bool isChecked);

  QLabel * m_text;
  QLabel * m_arrowLabel;
  PageNavigator * m_pageNavigator;
  QLabel * m_numLabel;

signals:
  void getComponentsByPage(int pageNum);

private slots:
  void on_getComponentsByPage(int pageIdx);

};

} // openstudio

#endif // SHAREDGUICOMPONENTS_COLLAPSIBLECOMPONENTHEADER_HPP

