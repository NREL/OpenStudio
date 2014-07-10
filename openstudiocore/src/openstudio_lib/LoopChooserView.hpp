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
#ifndef OPENSTUDIO_LOOPCHOOSERVIEW_HPP
#define OPENSTUDIO_LOOPCHOOSERVIEW_HPP

#include <QWidget>
#include "../model/ModelObject.hpp"
#include "../model/WaterToAirComponent.hpp"
#include "../model/HVACComponent.hpp"
#include "../model/Loop.hpp"
#include "../utilities/core/String.hpp"

class QCheckBox;

class QVBoxLayout;

namespace openstudio {

class LoopChooserItem;

class LoopChooserView : public QWidget
{
  Q_OBJECT;

  public:

  LoopChooserView(QWidget* parent = 0);

  virtual ~LoopChooserView() {}

  LoopChooserItem * loopChooserItemForLoop(std::string loopName);

  void layoutView();

  public slots:

  void layoutModelObject(boost::optional<model::ModelObject> & modelObject);

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  protected:

  void paintEvent ( QPaintEvent * event );

  private slots:

  void onAddToLoopClicked(model::Loop &);

  void onRemoveFromLoopClicked(model::Loop &);

 private:

  boost::optional<model::HVACComponent> m_component;

  QVBoxLayout * m_vLayout;

  std::vector<LoopChooserItem *> m_loopChooserItems;
};

class LoopChooserItem : public QWidget
{
  Q_OBJECT;

  public:

  LoopChooserItem(model::Loop &, LoopChooserView * parent = 0);

  virtual ~LoopChooserItem() {}

  std::string loopName();

  signals:

  void addToLoopClicked(model::Loop &);

  void removeFromLoopClicked(model::Loop &);

  public slots:

  void setChecked( bool checked );

  private slots:

  void sendClickedSignal( bool checked );

  private:

  QCheckBox * m_checkBox;

  LoopChooserView * m_loopChooserView;

  model::Loop m_loop;
};

} // openstudio

#endif // OPENSTUDIO_LOOPCHOOSERVIEW_HPP

