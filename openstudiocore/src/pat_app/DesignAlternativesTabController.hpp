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

#ifndef PATAPP_DESIGNALTERNATIVESTABCONTROLLER_HPP
#define PATAPP_DESIGNALTERNATIVESTABCONTROLLER_HPP

#include "../shared_gui_components/OSListView.hpp"
#include "../analysis/DataPoint.hpp"
#include "../analysis/Measure.hpp"
#include "../analysis/RubyMeasure.hpp"
#include <QObject>
#include <QPointer>
#include <QSharedPointer>

namespace openstudio{

namespace measuretab {

class VariableGroupListController;
class VariableGroupItemDelegate;

} // measuretab
  
namespace pat {

class DesignAlternativesTabView;


namespace altstab {

class AltsTabVariableGroupItemDelegate;
class DesignAltListController;
class DesignAltItemDelegate;
class PerturbationListController;
class AlternativeModelMeasureListController;
class AlternativeModelMeasureItem;

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
  QSharedPointer<altstab::AltsTabVariableGroupItemDelegate> m_altsTabVariableGroupItemDelegate;
  QSharedPointer<altstab::DesignAltListController> m_designAltListController;
  QSharedPointer<altstab::DesignAltItemDelegate> m_designAltItemDelegate;

  bool m_isButtonStateDirty;
};

namespace altstab {

// Many of these classes are alternatives to the classes in \openstudiocore\src\shared_gui_components\VariableList.hpp
// If possible, we should share the classes in shared_gui_components to these
// These classes are only used on the design alternatives tab

// AltsTabVariableGroupItemDelegate views a VariableGroupItem and returns an AltsTabVariableGroupItemView
class AltsTabVariableGroupItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:
    
  virtual ~AltsTabVariableGroupItemDelegate() {}

  QWidget * view(QSharedPointer<OSListItem> dataSource) override;
};

// AltsTabVariableItemDelegate views a VariableItem and returns an AltsTabVariableItemView
class AltsTabVariableItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  virtual ~AltsTabVariableItemDelegate() {}

  QWidget * view(QSharedPointer<OSListItem> dataSource) override;
};

// AltsTabMeasureItemDelegate views a MeasureItem and returns an AltsTabMeasureItemView
class AltsTabMeasureItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  virtual ~AltsTabMeasureItemDelegate() {}

  QWidget * view(QSharedPointer<OSListItem> dataSource) override;
};

// DesignAltListController controls a list of DesignAltItems
// It can add or remove analysis::Datapoints
class DesignAltListController : public OSListController
{
  Q_OBJECT

  public:

  DesignAltListController(QSharedPointer<OSItemSelectionController> measureSelectionController);

  virtual ~DesignAltListController() {}

  QSharedPointer<OSListItem> itemAt(int i) override;

  int count() override;

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

// DesignAltItem corresponds to an analysis::DataPoint
class DesignAltItem : public OSListItem
{
  Q_OBJECT

  public:

  DesignAltItem(const analysis::DataPoint & dataPoint, bool isBaseline, bool isAlternativeModel, bool alternateModelMeasureNeedsUpdate);

  virtual ~DesignAltItem() {}

  QString name() const;

  QString description() const;

  analysis::DataPoint dataPoint() const;

  bool isBaseline() const;

  bool isAlternativeModel() const;

  bool alternateModelMeasureNeedsUpdate() const;

  QSharedPointer<PerturbationListController> perturbationListController() const;

  QSharedPointer<AlternativeModelMeasureListController> alternativeModelMeasureListController() const;

  public slots:

  void setName(const QString & name);

  void setDescription(const QString & description);

  void remove();

  private:

  QSharedPointer<PerturbationListController> m_perturbationListController;

  QSharedPointer<AlternativeModelMeasureListController> m_alternativeModelMeasureListController;

  analysis::DataPoint m_dataPoint;

  bool m_isBaseline;

  bool m_isAlternativeModel;

  bool m_alternateModelMeasureNeedsUpdate;
};

// DesignAltItemDelegate views a DesignAltItem and returns an DesignAltItemView
class DesignAltItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  QWidget * view(QSharedPointer<OSListItem> dataSource) override;

  public:

  virtual ~DesignAltItemDelegate() {}
};

// These classes are the list controller and related classes for the perturbations associated with a design point.

// PerturbationListController gets the perturbations (e.g. Measures) for a DesignAltItem
class PerturbationListController : public OSListController
{
  Q_OBJECT

  public:

  PerturbationListController(DesignAltItem * designAltItem);
  
  virtual ~PerturbationListController() {}

  QSharedPointer<OSListItem> itemAt(int i) override;

  int count() override;

  private:

  std::vector<analysis::Measure> measures() const;

  QPointer<DesignAltItem> m_designAltItem;
};

// PerturbationItem corresponds to an analysis::Measure
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

// PerturbationItemDelegate views a PerturbationItem and returns a QLabel
class PerturbationItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  QWidget * view(QSharedPointer<OSListItem> dataSource) override;

  public:

  virtual ~PerturbationItemDelegate() {}
};

// These classes are the list controller and related classes for the user defined measures associated with an alternative model,
// i.e. a model swapped into the workflow using the alternativeModelVariable.

// AlternativeModelMeasureListController gets the user defined measures for an AlternativeModel DesignAltItem
class AlternativeModelMeasureListController : public OSListController
{
  Q_OBJECT

public:

  AlternativeModelMeasureListController(DesignAltItem * designAltItem);

  virtual ~AlternativeModelMeasureListController() {}

  QSharedPointer<OSListItem> itemAt(int i);

  int count();

public slots:

  void addAlternativeModelMeasure();

  void alternativeModelMeasureItemChanged();

  void alternativeModelMeasureItemRemoved();

private slots:

  void emitModelReset();

private:

  boost::optional<analysis::RubyMeasure> rubySwapMeasure() const;

  // the current swap measure arguments that represent user defined model measures
  QJsonArray modelMeasures() const;

  void setModelMeasures(const QJsonArray& modelMeasures, bool forceRefresh);

  std::vector<QSharedPointer<AlternativeModelMeasureItem> > alternativeModelMeasureItems() const;

  QPointer<DesignAltItem> m_designAltItem;

  int m_count;
};

// AlternativeModelMeasureItem corresponds to a user defined measure on an alternative model
class AlternativeModelMeasureItem : public OSListItem
{
  Q_OBJECT

public:

  AlternativeModelMeasureItem(const QString& uuid,
                              const QString& displayName,
                              const QString& description,
                              const QString& taxonomyTag,
                              double capitalCost);

  virtual ~AlternativeModelMeasureItem() {}

  QString uuid() const;
  QString displayName() const;
  QString description() const;
  QString taxonomyTag() const;
  double capitalCost() const;

public slots:

  void viewChanged();

signals:

  void changed();

  void removed();

private:

  QString m_uuid;
  QString m_displayName;
  QString m_description;
  QString m_taxonomyTag;
  double m_capitalCost;
};

// AlternativeModelMeasureItemDelegate views an AlternativeModelMeasureItem and returns an AlternativeModelMeasureItemView
class AlternativeModelMeasureItemDelegate : public OSItemDelegate
{
  Q_OBJECT

    QWidget * view(QSharedPointer<OSListItem> dataSource);

public:

  virtual ~AlternativeModelMeasureItemDelegate() {}
};



} // altstab

} // pat

} // openstudio

#endif // PATAPP_DESIGNALTERNATIVESTABCONTROLLER_HPP
