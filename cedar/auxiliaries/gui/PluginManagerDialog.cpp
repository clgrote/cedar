/*======================================================================================================================

    Copyright 2011, 2012, 2013 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
    This file is part of cedar.

    cedar is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    cedar is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with cedar. If not, see <http://www.gnu.org/licenses/>.

========================================================================================================================

    Institute:   Ruhr-Universitaet Bochum
                 Institut fuer Neuroinformatik

    File:        PluginManagerDialog.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2011 07 26

    Description:

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/auxiliaries/gui/PluginManagerDialog.h"
#include "cedar/auxiliaries/gui/PluginLoadDialog.h"
#include "cedar/auxiliaries/gui/Settings.h"
#include "cedar/auxiliaries/SetParameter.h"
#include "cedar/auxiliaries/Settings.h"
#include "cedar/auxiliaries/assert.h"

// SYSTEM INCLUDES
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <set>

#define PLUGIN_MANAGER_STARTUP_LOAD_ROW 0
#define PLUGIN_MANAGER_PLUGIN_LOADED_ROW 1
#define PLUGIN_MANAGER_PLUGIN_NAME_ROW 2
#define PLUGIN_MANAGER_PLUGIN_PATH_ROW 3

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------
cedar::aux::gui::PluginManagerDialog::PluginManagerDialog(QWidget *pParent)
:
QDialog(pParent)
{
  this->setupUi(this);

  this->populate();

  QObject::connect(this->mpDeleteButton, SIGNAL(clicked()), this, SLOT(removeSelectedPlugins()));
  this->mpDeleteButton->setEnabled(false);

  QObject::connect(this->mpLoadSelectedPluginsBtn, SIGNAL(clicked()), this, SLOT(loadSelectedPlugins()));
  this->mpLoadSelectedPluginsBtn->setEnabled(false);

  QObject::connect(this->mpAddPathBtn, SIGNAL(clicked()), this, SLOT(addSearchPathClicked()));
  QObject::connect(this->mpRemoveSearchPathBtn, SIGNAL(clicked()), this, SLOT(removeSearchPathClicked()));

  cedar::aux::SettingsSingleton::getInstance()->connectToPluginSearchPathAddedSignal
  (
    boost::bind(&cedar::aux::gui::PluginManagerDialog::addPluginSearchPath, this, _1)
  );

  cedar::aux::SettingsSingleton::getInstance()->connectToPluginSearchPathIndexRemovedSignal
  (
    boost::bind(&cedar::aux::gui::PluginManagerDialog::removePluginSearchPathIndex, this, _1)
  );

  cedar::aux::SettingsSingleton::getInstance()->connectToPluginSearchPathsChangedSignal
  (
    boost::bind(&cedar::aux::gui::PluginManagerDialog::updatePluginPaths, this)
  );


  QObject::connect(this->mpAddPluginBtn, SIGNAL(clicked()), this, SLOT(addPluginClicked()));

  cedar::aux::SettingsSingleton::getInstance()->connectToPluginAddedSignal
  (
    boost::bind(&cedar::aux::gui::PluginManagerDialog::addPlugin, this, _1)
  );

  cedar::aux::SettingsSingleton::getInstance()->connectToPluginRemovedSignal
  (
    boost::bind(&cedar::aux::gui::PluginManagerDialog::removePlugin, this, _1)
  );

  QObject::connect(mpPluginList, SIGNAL(itemSelectionChanged()), this, SLOT(toggleSelectedPluginsButtons()));

  cedar::aux::PluginProxy::connectToPluginDeclaredSignal
  (
    boost::bind(&cedar::aux::gui::PluginManagerDialog::pluginDeclared ,this, _1)
  );
}


//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::aux::gui::PluginManagerDialog::pluginDeclared(const std::string& pluginName)
{
  int row = this->getPluginRowFromName(pluginName);

  auto p_loaded = dynamic_cast<QCheckBox*>(this->mpPluginList->cellWidget(row, PLUGIN_MANAGER_PLUGIN_LOADED_ROW));
  CEDAR_DEBUG_ASSERT(p_loaded != NULL);

  p_loaded->setEnabled(true);
  p_loaded->setChecked(true);
  p_loaded->setEnabled(false);
}

void cedar::aux::gui::PluginManagerDialog::populate()
{
  // restore plugins to load from the settings
  auto known_plugins = cedar::aux::SettingsSingleton::getInstance()->getKnownPlugins();
  for (auto iter = known_plugins.begin(); iter != known_plugins.end(); ++iter)
  {
    this->addPlugin(*iter);
  }

  // restore search paths
  auto search_paths = cedar::aux::SettingsSingleton::getInstance()->getPluginSearchPaths();
  for (auto iter = search_paths.begin(); iter != search_paths.end(); ++iter)
  {
    this->addPluginSearchPath(*iter);
  }
}

void cedar::aux::gui::PluginManagerDialog::addPluginSearchPath(const std::string& path)
{
  this->mpSearchPathList->addItem(QString::fromStdString(path));
}

void cedar::aux::gui::PluginManagerDialog::removePluginSearchPathIndex(size_t index)
{
  CEDAR_ASSERT(static_cast<int>(index) < this->mpSearchPathList->count());

  this->mpSearchPathList->takeItem(static_cast<int>(index));
}

void cedar::aux::gui::PluginManagerDialog::removePluginSearchPath(const std::string& path)
{
  for (int row = this->mpSearchPathList->count() - 1; row >= 0; ++row)
  {
    if (this->mpSearchPathList->item(row)->text().toStdString() == path)
    {
      this->mpSearchPathList->takeItem(row);
    }
  }
}

void cedar::aux::gui::PluginManagerDialog::addPluginClicked()
{
  auto p_dialog = new cedar::aux::gui::PluginLoadDialog(this);
  int r = p_dialog->exec();

  if (r == QDialog::Accepted)
  {
    cedar::aux::SettingsSingleton::getInstance()->addPlugin(p_dialog->plugin());
  }

  delete p_dialog;
}

void cedar::aux::gui::PluginManagerDialog::addPlugin(const std::string& pluginName)
{
  int row = this->mpPluginList->rowCount();
  this->mpPluginList->insertRow(row);

  cedar::aux::PluginProxyPtr plugin = cedar::aux::PluginProxy::getPlugin(pluginName);

  QCheckBox* p_cb = new QCheckBox();
  p_cb->setChecked(cedar::aux::SettingsSingleton::getInstance()->isPluginLoadedOnStartup(pluginName));
  this->mpPluginList->setCellWidget(row, PLUGIN_MANAGER_STARTUP_LOAD_ROW, p_cb);
  QObject::connect(p_cb, SIGNAL(toggled(bool)), this, SLOT(loadOnStartupCheckboxToggled(bool)));

  QCheckBox* p_cb2 = new QCheckBox();
  p_cb2->setChecked(plugin->isDeclared());
  p_cb2->setDisabled(true);
  this->mpPluginList->setCellWidget(row, PLUGIN_MANAGER_PLUGIN_LOADED_ROW, p_cb2);

  QLabel* p_name = new QLabel(QString::fromStdString(pluginName));
  this->mpPluginList->setCellWidget(row, PLUGIN_MANAGER_PLUGIN_NAME_ROW, p_name);

  QLabel* p_path = new QLabel();
  this->mpPluginList->setCellWidget(row, PLUGIN_MANAGER_PLUGIN_PATH_ROW, p_path);

  this->updatePluginPath(row);
}

void cedar::aux::gui::PluginManagerDialog::loadOnStartupCheckboxToggled(bool loaded)
{
  QCheckBox* p_sender = dynamic_cast<QCheckBox*>(QObject::sender());
  CEDAR_DEBUG_ASSERT(p_sender != NULL);

  // find out which checkbox sent the signal
  int row;
  for (row = 0; row < this->mpPluginList->rowCount(); ++row)
  {
    if (this->mpPluginList->cellWidget(row, 0) == p_sender)
    {
      break;
    }
  }

  // get name of the corresponding plugin
  std::string plugin_name = this->getPluginNameFromRow(row);

  // set the auto-load state the plugin
  if (loaded)
  {
    cedar::aux::SettingsSingleton::getInstance()->addPluginToLoad(plugin_name);
  }
  else
  {
    cedar::aux::SettingsSingleton::getInstance()->removePluginToLoad(plugin_name);
  }
}


void cedar::aux::gui::PluginManagerDialog::updatePluginPaths()
{
  for (int row = 0; row < this->mpPluginList->rowCount(); ++row)
  {
    this->updatePluginPath(row);
  }
}

void cedar::aux::gui::PluginManagerDialog::updatePluginPath(int row)
{
  QLabel* p_path = dynamic_cast<QLabel*>(this->mpPluginList->cellWidget(row, PLUGIN_MANAGER_PLUGIN_PATH_ROW));
  CEDAR_DEBUG_ASSERT(p_path != NULL);

  std::string plugin_name = this->getPluginNameFromRow(row);

  try
  {
    std::string plugin_path = cedar::aux::PluginProxy::findPlugin(plugin_name);
    p_path->setText(QString::fromStdString(plugin_path));
  }
  catch (const cedar::aux::PluginNotFoundException& e)
  {
    p_path->setText("(not found)");
    p_path->setToolTip(QString::fromStdString(e.exceptionInfo()));
  }
}

void cedar::aux::gui::PluginManagerDialog::removePlugin(const std::string& pluginName)
{
  std::vector<int> rows_to_remove;
  for (int row = 0; row < this->mpPluginList->rowCount(); ++row)
  {
    if (this->getPluginNameFromRow(row) == pluginName)
    {
      rows_to_remove.push_back(row);
    }
  }

  // erase rows, starting from the last ones (so the other ones don't get shifted)
  for (size_t i = 0; i < rows_to_remove.size(); ++i)
  {
    this->mpPluginList->removeRow(rows_to_remove.at(i));
  }
}

std::string cedar::aux::gui::PluginManagerDialog::getPluginNameFromRow(int row) const
{
  CEDAR_DEBUG_ASSERT(this->mpPluginList->rowCount() > row);
  CEDAR_DEBUG_ASSERT(this->mpPluginList->columnCount() > PLUGIN_MANAGER_PLUGIN_NAME_ROW);

  auto p_widget = dynamic_cast<QLabel*>(this->mpPluginList->cellWidget(row, PLUGIN_MANAGER_PLUGIN_NAME_ROW));

  CEDAR_DEBUG_ASSERT(p_widget != NULL);

  return p_widget->text().toStdString();
}

int cedar::aux::gui::PluginManagerDialog::getPluginRowFromName(const std::string& pluginName) const
{
  for (int row = 0; row < this->mpPluginList->rowCount(); ++row)
  {
    if (this->getPluginNameFromRow(row) == pluginName)
    {
      return row;
    }
  }

  CEDAR_THROW(cedar::aux::UnknownNameException, "Cannot find the plugin \"" + pluginName + "\".");
}

void cedar::aux::gui::PluginManagerDialog::loadSelectedPlugins()
{
  QList<QTableWidgetSelectionRange> ranges = this->mpPluginList->selectedRanges();
  for (int i = 0; i < ranges.size(); ++i)
  {
    const QTableWidgetSelectionRange& range = ranges.at(i);

    for (int row = range.topRow(); row <= range.bottomRow(); ++row)
    {
      std::string plugin_name = this->getPluginNameFromRow(row);
      auto plugin = cedar::aux::PluginProxy::getPlugin(plugin_name);
      plugin->declare();
    }
  }
}

void cedar::aux::gui::PluginManagerDialog::removeSelectedPlugins()
{
  std::vector<std::string> to_remove;

  QList<QTableWidgetSelectionRange> ranges = this->mpPluginList->selectedRanges();
  for (int i = 0; i < ranges.size(); ++i)
  {
    const QTableWidgetSelectionRange& range = ranges.at(i);

    for (int row = range.topRow(); row <= range.bottomRow(); ++row)
    {
      to_remove.push_back(this->getPluginNameFromRow(row));
    }
  }

  for (size_t i = 0; i < to_remove.size(); ++i)
  {
    cedar::aux::SettingsSingleton::getInstance()->removePlugin(to_remove.at(i));
  }
}

void cedar::aux::gui::PluginManagerDialog::toggleSelectedPluginsButtons()
{
  bool enable = (this->mpPluginList->selectedRanges().size() > 0);

  this->mpDeleteButton->setEnabled(enable);
  this->mpLoadSelectedPluginsBtn->setEnabled(enable);
}

void cedar::aux::gui::PluginManagerDialog::addSearchPathClicked()
{
  QString dir = QFileDialog::getExistingDirectory(this, "select search path to add", "");
  if (!dir.isEmpty())
  {
    cedar::aux::SettingsSingleton::getInstance()->addPluginSearchPath(dir.toStdString());
  }
}

void cedar::aux::gui::PluginManagerDialog::removeSearchPathClicked()
{
  QList<QListWidgetItem*> selected = this->mpSearchPathList->selectedItems();

  for (int i = 0; i < selected.size(); ++i)
  {
    int index = this->mpSearchPathList->row(selected[i]);
    if (index >= 0)
    {
      cedar::aux::SettingsSingleton::getInstance()->removePluginSearchPath(index);
    }
  }
}
