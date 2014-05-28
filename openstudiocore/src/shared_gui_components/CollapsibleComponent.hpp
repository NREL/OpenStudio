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

#ifndef SHAREDGUICOMPONENTS_COLLAPSIBLECOMPONENT_HPP
#define SHAREDGUICOMPONENTS_COLLAPSIBLECOMPONENT_HPP

#include <QAbstractButton>

#include "Component.hpp"

class QVBoxLayout;

namespace openstudio {

class CollapsibleComponentHeader;
class ComponentList;

class CollapsibleComponent : public QAbstractButton
{
  Q_OBJECT

public:
  CollapsibleComponent(CollapsibleComponentHeader * collapsibleComponentHeader,
    ComponentList * componentList,
    QWidget * parent = nullptr);
  virtual ~CollapsibleComponent() {}
  bool expanded() const;
  void setExpanded(bool expanded);
  CollapsibleComponentHeader * collapsibleComponentHeader() const;
  ComponentList * componentList() const;
  void setText(const QString& text);
  void setNumResults(int numResults);
  void setNumPages(int numPages);
  void firstPage();

protected:
  void paintEvent(QPaintEvent * event);

private:
  void createLayout();

  CollapsibleComponentHeader * m_collapsibleComponentHeader;
  ComponentList * m_componentList;
  QVBoxLayout * m_mainLayout;

signals:
  void headerClicked(bool);
  void componentClicked(bool);
  void getComponentsByPage(int pageNum);

private slots:
  void on_headerClicked(bool checked);
  void on_componentClicked(bool checked);
  void on_getComponentsByPage(int pageIdx);

};

} // openstudio

#endif // SHAREDGUICOMPONENTS_COLLAPSIBLECOMPONENT_HPP
