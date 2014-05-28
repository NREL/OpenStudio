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

#ifndef PATAPP_DESIGNALTERNATIVESTABCONTROLLER_HPP
#define PATAPP_DESIGNALTERNATIVESTABCONTROLLER_HPP

#include "../shared_gui_components/OSListView.hpp"
#include "../analysis/DataPoint.hpp"
#include "../analysis/Measure.hpp"
#include <QObject>
#include <QPointer>
#include <QSharedPointer>

namespace openstudio{

namespace measuretab {

class VariableGroupListController;

} // measuretab
  
namespace pat {

class DesignAlternativesTabView;


namespace altstab {

class VariableGroupItemDelegate;
class DesignAltListController;
class DesignAltItemDelegate;
class PerturbationListController;

} // altstab

class DesignAlternativesTabController : public QObject
{
  Q_OBJECT

  public:

  QPointer<DesignAlternativesTabView> designAlternativesTabView;

  DesignAlternativesTabController();

  virtual ~DesignAlternativesTabController();

  private slots:

  // Schedule an update for the buttons that are active or inactive based on selections.
  // This method uses a single shot timer in a way that the buttons only get updated once per event loop.
  // Calls updateButtonStatusBasedOnSelectionNow() and the expiration of the timer.
  void updateButtonStatusBasedOnSelection();

  // Update the buttons that are active or inactive based on selections.
  void updateButtonStatusBasedOnSelectionNow();

  private:

  QSharedPointer<measuretab::VariableGroupListController> m_variableGroupListController;
  QSharedPointer<altstab::VariableGroupItemDelegate> m_variableGroupItemDelegate;
  QSharedPointer<altstab::DesignAltListController> m_designAltListController;
  QSharedPointer<altstab::DesignAltItemDelegate> m_designAltItemDelegate;

  bool m_isButtonStateDirty;
};

namespace altstab {

class VariableGroupItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:
  virtual ~VariableGroupItemDelegate() {}

  QWidget * view(QSharedPointer<OSListItem> dataSource);
};

class VariableItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:
  virtual ~VariableItemDelegate() {}

  QWidget * view(QSharedPointer<OSListItem> dataSource);
};

class MeasureItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:
  virtual ~MeasureItemDelegate() {}

  QWidget * view(QSharedPointer<OSListItem> dataSource);
};

class DesignAltListController : public OSListController
{
  Q_OBJECT

  public:

  DesignAltListController(QSharedPointer<OSItemSelectionController> measureSelectionController);
  virtual ~DesignAltListController() {}

  QSharedPointer<OSListItem> itemAt(int i);

  int count();

  void removeItemForDataPoint(analysis::DataPoint datapoint);

  public slots:

  void addOneItemForEachSelectedMeasure();

  void addOneItemWithAllSelectedMeasures();

  void addItemFromExternalFile();

  private:

  std::vector<analysis::DataPoint> dataPoints() const;

  std::string suggestDesignAltName(const boost::optional<analysis::Measure>& measure) const;

  QSharedPointer<OSItemSelectionController> m_measureSelectionController;
};

class DesignAltItem : public OSListItem
{
  Q_OBJECT

  public:

  DesignAltItem(const analysis::DataPoint & dataPoint, bool isBaseline);
  virtual ~DesignAltItem() {}

  QString name() const;

  QString description() const;

  analysis::DataPoint dataPoint() const;

  bool isBaseline() const;

  QSharedPointer<PerturbationListController> perturbationListController() const;

  public slots:

  void setName(const QString & name);

  void setDescription(const QString & description);

  void remove();

  private:

  QSharedPointer<PerturbationListController> m_perturbationListController;

  analysis::DataPoint m_dataPoint;

  bool m_isBaseline;
};

class DesignAltItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  QWidget * view(QSharedPointer<OSListItem> dataSource);

  public:
  virtual ~DesignAltItemDelegate() {}
};

// These classes are the list controller and related classes for the perturbations associated with a design point.
class PerturbationListController : public OSListController
{
  Q_OBJECT

  public:

  PerturbationListController(DesignAltItem * designAltItem);
  virtual ~PerturbationListController() {}

  QSharedPointer<OSListItem> itemAt(int i);

  int count();

  private:

  std::vector<analysis::Measure> measures() const;

  QPointer<DesignAltItem> m_designAltItem;
};

class PerturbationItem : public OSListItem
{
  Q_OBJECT

  public:

  PerturbationItem(const analysis::Measure & measure);
  virtual ~PerturbationItem() {}

  QString name() const;
  bool isFixedMeasureItem() const;

  private:
  boost::optional<analysis::MeasureGroup> measureGroupParent() const;

  analysis::Measure m_measure;
};

class PerturbationItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  QWidget * view(QSharedPointer<OSListItem> dataSource);

  public:

  virtual ~PerturbationItemDelegate() {}
};

} // altstab

} // pat

} // openstudio

#endif // PATAPP_DESIGNALTERNATIVESTABCONTROLLER_HPP
