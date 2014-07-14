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

#include "Configuration.hpp"
#include "AddTool.hpp"
#include "JobFactory.hpp"

#include "../../utilities/core/Application.hpp"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItem>

namespace openstudio {
namespace runmanager {

  SpinBoxDelegate::SpinBoxDelegate(QObject *parent)
    : QItemDelegate(parent)
  {
  }

  QWidget *SpinBoxDelegate::createEditor(QWidget *parent,
      const QStyleOptionViewItem &/* option */,
      const QModelIndex &/* index */) const
  {
    auto editor = new QSpinBox(parent);
    editor->setMinimum(-1);
    editor->setMaximum(100);
    editor->setSpecialValueText("-");

    return editor;
  }

  void SpinBoxDelegate::setEditorData(QWidget *editor,
      const QModelIndex &index) const
  {
    QVariant data = index.model()->data(index, Qt::EditRole);

    int value;
    if (data.toString() == "-")
    {
      value = -1;
    } else {
      value = data.toInt();
    }

    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
  }

  void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
      const QModelIndex &index) const
  {
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();

    if (value == -1)
    {
      model->setData(index, "-", Qt::EditRole);
    } else {
      model->setData(index, value, Qt::EditRole);
    }

  }

  void SpinBoxDelegate::updateEditorGeometry(QWidget *editor,
      const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
  {
    editor->setGeometry(option.rect);
  }


  ToolNameDelegate::ToolNameDelegate(QObject *parent)
    : QItemDelegate(parent)
  {
  }

  QWidget *ToolNameDelegate::createEditor(QWidget *parent,
      const QStyleOptionViewItem &/* option */,
      const QModelIndex &/* index */) const
  {
    auto editor = new QComboBox(parent);
    std::set<int> values = ToolType::getValues();

    for (const auto & value : values)
    {
      editor->addItem(toQString(ToolType::valueDescription(value)));
    }
    return editor;
  }

  void ToolNameDelegate::setEditorData(QWidget *editor,
      const QModelIndex &index) const
  {
    QVariant data = index.model()->data(index, Qt::EditRole);
    QComboBox *cb = static_cast<QComboBox*>(editor);
    cb->setCurrentIndex(cb->findText(data.toString()));
  }

  void ToolNameDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
      const QModelIndex &index) const
  {
    QComboBox *cb = static_cast<QComboBox*>(editor);
    model->setData(index, cb->currentText(), Qt::EditRole);
  }

  void ToolNameDelegate::updateEditorGeometry(QWidget *editor,
      const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
  {
    editor->setGeometry(option.rect);
  }

  Configuration::Configuration(QWidget *parent, Qt::WindowFlags flags, const ConfigOptions &co)
          : QDialog(parent, flags)
  {
    ui.setupUi(this);

    if (Application::instance().isDefaultInstance())
    {
      QIcon icon = QIcon(":/images/rm_16.png");
      icon.addPixmap(QPixmap(":/images/rm_32.png"));
      icon.addPixmap(QPixmap(":/images/rm_48.png"));
      icon.addPixmap(QPixmap(":/images/rm_64.png"));
      icon.addPixmap(QPixmap(":/images/rm_128.png"));
      icon.addPixmap(QPixmap(":/images/rm_256.png"));
      setWindowIcon(icon);
    }

    ui.tableTools->setModel(&m_model);

    // Set defaults
    ui.numLocalProcessesBox->setValue(co.getMaxLocalJobs());
    ui.idfLocationTxt->setText(openstudio::toQString(co.getDefaultIDFLocation().native()));
    ui.epwLocationTxt->setText(openstudio::toQString(co.getDefaultEPWLocation().native()));

    ui.numSlurmProcessesBox->setValue(co.getMaxSLURMJobs());
    ui.slurmUserNameTxt->setText(openstudio::toQString(co.getSLURMUserName()));
    ui.slurmHostTxt->setText(openstudio::toQString(co.getSLURMHost()));

    ui.maxSLURMRuntimeBox->setValue(co.getSLURMMaxTime());
    ui.slurmPartitionTxt->setText(openstudio::toQString(co.getSLURMPartition()));
    ui.slurmAccountTxt->setText(openstudio::toQString(co.getSLURMAccount()));


    openstudio::path outdir = co.getOutputLocation();
    if (!outdir.empty())
    {
      ui.outputLocationTxt->setText(openstudio::toQString(outdir.native()));
      ui.outputLocationTxt->setEnabled(true);
      ui.btnOutputLocation->setEnabled(true);
      ui.cbOutputInPlace->setCheckState(Qt::Unchecked);
    } else {
      ui.outputLocationTxt->setText("");
      ui.outputLocationTxt->setEnabled(false);
      ui.btnOutputLocation->setEnabled(false);
      ui.cbOutputInPlace->setCheckState(Qt::Checked);
    }

    ui.cbSimpleName->setCheckState(co.getSimpleName()?Qt::Checked:Qt::Unchecked);


    QStringList items;
    items.push_back("Tool Name");
    items.push_back("Major");
    items.push_back("Minor");
    items.push_back("Build");
    items.push_back("Binary Directory");
    items.push_back("Remote Binary Archive");
    m_model.setHorizontalHeaderLabels(items);

    std::vector<std::pair<ToolVersion, ToolLocationInfo> > tools = co.getToolLocations();

    for (std::vector<std::pair<ToolVersion, ToolLocationInfo> >::const_iterator itr = tools.begin();
         itr != tools.end();
         ++itr)
    {
      addToolLocation(*itr);
    }

    for (int i = 0; i < ui.tableTools->model()->columnCount(); ++i)
    {
      ui.tableTools->resizeColumnToContents(i);
    }

    connect(ui.btnIDFLocation, SIGNAL(clicked()), this, SLOT(updateIDFLocation()));
    connect(ui.btnEPWLocation, SIGNAL(clicked()), this, SLOT(updateEPWLocation()));
    connect(ui.btnOutputLocation, SIGNAL(clicked()), this, SLOT(updateOutputLocation()));
    connect(ui.btnAddTool, SIGNAL(clicked()), this, SLOT(addTool()));
    connect(ui.btnRemoveTool, SIGNAL(clicked()), this, SLOT(removeSelectedTool()));
    connect(ui.cbOutputInPlace, SIGNAL(stateChanged(int)), this, SLOT(inPlaceStateChanged(int)));

    QItemDelegate *tooldelegate = new ToolNameDelegate(ui.tableTools);
    ui.tableTools->setItemDelegateForColumn(0, tooldelegate);

    QItemDelegate *delegate = new SpinBoxDelegate(ui.tableTools);
    ui.tableTools->setItemDelegateForColumn(1, delegate);
    ui.tableTools->setItemDelegateForColumn(2, delegate);
    ui.tableTools->setItemDelegateForColumn(3, delegate);

  }

  Configuration::~Configuration()
  {
  }

  void Configuration::inPlaceStateChanged(int t_newstate)
  {
    if (t_newstate == Qt::Checked)
    {
      ui.outputLocationTxt->setEnabled(false);
      ui.btnOutputLocation->setEnabled(false);
    } else {
      ui.outputLocationTxt->setEnabled(true);
      ui.btnOutputLocation->setEnabled(true);
    }
  }

  void Configuration::addToolLocation(const std::pair<ToolVersion, ToolLocationInfo> &tool)
  {
    QList<QStandardItem *> row;
    row.push_back(new QStandardItem(toQString(tool.second.toolType.valueDescription())));

    if (tool.first.getMajor())
    {
      row.push_back(new QStandardItem(QString::number(*(tool.first.getMajor()))));
    } else {
      row.push_back(new QStandardItem("-"));
    }
    if (tool.first.getMinor())
    {
      row.push_back(new QStandardItem(QString::number(*(tool.first.getMinor()))));
    } else {
      row.push_back(new QStandardItem("-"));
    }
    if (tool.first.getBuild())
    {
      row.push_back(new QStandardItem(QString::number(*(tool.first.getBuild()))));
    } else {
      row.push_back(new QStandardItem("-"));
    }

    if (!tool.second.binaryDir.empty())
    {
      row.push_back(new QStandardItem(toQString(tool.second.binaryDir.native())));
    } else {
      row.push_back(new QStandardItem(""));
    }

    if (!tool.second.linuxBinaryArchive.empty())
    {
      row.push_back(new QStandardItem(toQString(tool.second.linuxBinaryArchive.native())));
    } else {
      row.push_back(new QStandardItem(""));
    }

    m_model.appendRow(row);
  }

  void Configuration::removeSelectedTool()
  {
    QModelIndexList selected = ui.tableTools->selectionModel()->selectedRows();

    for (QModelIndexList::const_iterator itr = selected.begin();
         itr != selected.end();
         ++itr)
    {
      m_model.removeRow(itr->row());
      break; // Let's just remove the first one in the list for now, multiselect isn't even enabled
    }
  }

  void Configuration::addTool()
  {
    AddTool at(this);
    if (at.exec() == QDialog::Accepted)
    {
      boost::optional<std::pair<ToolVersion, ToolLocationInfo> > tool = at.getTool(); 
      if (tool)
      {
        addToolLocation(*tool);
      }
    }
  }

  void Configuration::updateIDFLocation()
  {
    QString str = QFileDialog::getExistingDirectory(this, "Choose Default IDF Directory", ui.idfLocationTxt->text());

    if (!str.isEmpty() )
    {
      ui.idfLocationTxt->setText(toQString(toPath(str).native()));
    }
  }

  void Configuration::updateEPWLocation()
  {
    QString str = QFileDialog::getExistingDirectory(this, "Choose Default EPW Directory", ui.epwLocationTxt->text());

    if (!str.isEmpty() )
    {
      ui.epwLocationTxt->setText(toQString(toPath(str).native()));
    }
  }

  void Configuration::updateOutputLocation()
  {
    QString str = QFileDialog::getExistingDirectory(this, "Choose Base Output Location For Job Runs", ui.outputLocationTxt->text());

    if (!str.isEmpty() )
    {
      ui.outputLocationTxt->setText(toQString(toPath(str).native()));
    }
  }


  ConfigOptions Configuration::getConfigOptions()
  {
    ConfigOptions co;
    co.setMaxLocalJobs(ui.numLocalProcessesBox->value());
    co.setDefaultIDFLocation(toPath(ui.idfLocationTxt->text()));
    co.setDefaultEPWLocation(toPath(ui.epwLocationTxt->text()));
    co.setSimpleName(ui.cbSimpleName->checkState() == Qt::Checked);

    co.setSLURMMaxTime(ui.maxSLURMRuntimeBox->value());
    co.setSLURMPartition(toString(ui.slurmPartitionTxt->text()));
    co.setSLURMAccount(toString(ui.slurmAccountTxt->text()));

    if (ui.cbOutputInPlace->checkState() != Qt::Checked)
    {
      QString txt = ui.outputLocationTxt->text();
      txt = txt.trimmed();
      if (!txt.isEmpty())
      {
        co.setOutputLocation(toPath(txt));
      }
    }

    //dump the defaults
    co.reset();

    //add the set
    for (int i=0; i < m_model.rowCount(); ++i)
    {
      boost::optional<int> major;
      boost::optional<int> minor;
      boost::optional<int> build;

      if (m_model.item(i, 1)->text() != "-")
      {
        major = m_model.item(i, 1)->text().toInt();
      }

      if (m_model.item(i, 2)->text() != "-")
      {
        minor = m_model.item(i, 2)->text().toInt();
      }

      if (m_model.item(i, 3)->text() != "-")
      {
        build = m_model.item(i, 3)->text().toInt();
      }

      co.setToolLocation(
          ToolVersion(
            major,
            minor,
            build),
          ToolLocationInfo(
            toString(m_model.item(i, 0)->text()),
            toPath(m_model.item(i,4)->text()),
            toPath(m_model.item(i,5)->text()))
          );
    }
    co.setMaxSLURMJobs(ui.numSlurmProcessesBox->value());
    co.setSLURMUserName(toString(ui.slurmUserNameTxt->text()));
    co.setSLURMHost(toString(ui.slurmHostTxt->text()));


    return co;
  }


} // runmanager
} // openstudio


