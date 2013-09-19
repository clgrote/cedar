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

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------
cedar::aux::gui::PluginManagerDialog::PluginManagerDialog(QWidget *pParent)
:
QDialog(pParent)
{
  this->setupUi(this);

  this->populate();

  QObject::connect(this->mpDeleteButton, SIGNAL(clicked()), this, SLOT(removePlugins()));
  this->mpDeleteButton->setEnabled(false);

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


  QObject::connect(this->mpAddPluginBtn, SIGNAL(clicked()), this, SLOT(addPluginClicked()));

  cedar::aux::SettingsSingleton::getInstance()->connectToPluginAddedSignal
  (
    boost::bind(&cedar::aux::gui::PluginManagerDialog::addPlugin, this, _1)
  );
}


//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

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

void cedar::aux::gui::PluginManagerDialog::addPlugin(const std::string& plugin)
{
  int row = this->mpPluginList->rowCount();
  this->mpPluginList->insertRow(row);

  QCheckBox* p_cb = new QCheckBox();
  p_cb->setChecked(cedar::aux::SettingsSingleton::getInstance()->isPluginLoadedOnStartup(plugin));
  this->mpPluginList->setCellWidget(row, 0, p_cb);

  QLabel* p_name = new QLabel(QString::fromStdString(plugin));
  this->mpPluginList->setCellWidget(row, 1, p_name);

  //!@todo Add path that is found by the plugin system, update if search paths are changed.
  /*
  QLabel* p_path = new QLabel(path.c_str());
  this->mpPluginList->setCellWidget(row, 2, p_path);
  */
}

void cedar::aux::gui::PluginManagerDialog::removePlugins()
{
  unsigned int deleted_items = 0;
  for (int row = 0; row < this->mpPluginList->rowCount(); ++row)
  {
    QCheckBox* p_cb = dynamic_cast<QCheckBox*>(this->mpPluginList->cellWidget(row, 0));
    CEDAR_DEBUG_ASSERT(p_cb != NULL);

    QCheckBox* p_delete = dynamic_cast<QCheckBox*>(this->mpPluginList->cellWidget(row, 1));
    CEDAR_DEBUG_ASSERT(p_delete != NULL);

    QLabel* p_path = dynamic_cast<QLabel*>(this->mpPluginList->cellWidget(row, 3));
    CEDAR_DEBUG_ASSERT(p_path != NULL);

    std::string path = p_path->text().toStdString();

    if (p_delete->isChecked())
    {
      if (p_cb->isChecked())
      {
        cedar::aux::SettingsSingleton::getInstance()->removePluginToLoad(path);
      }
      ++deleted_items;
    }
  }
  if (deleted_items)
  {
    this->mpPluginList->setRowCount(0);
    this->populate();
    this->mpDeleteButton->setEnabled(false);
  }
}

void cedar::aux::gui::PluginManagerDialog::toggleDeleteButton()
{
  for (int row = 0; row < this->mpPluginList->rowCount(); ++row)
  {
    QCheckBox* p_delete = dynamic_cast<QCheckBox*>(this->mpPluginList->cellWidget(row, 1));
    CEDAR_DEBUG_ASSERT(p_delete != NULL);
    if (p_delete->isChecked())
    {
      this->mpDeleteButton->setEnabled(true);
      return;
    }
  }
  this->mpDeleteButton->setEnabled(false);
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
