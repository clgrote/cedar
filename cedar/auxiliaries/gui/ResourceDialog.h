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

    File:        ResourceDialog.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2013 02 27

    Description:

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_GUI_RESOURCE_DIALOG_H
#define CEDAR_AUX_GUI_RESOURCE_DIALOG_H

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/gui/ui_ResourceDialog.h"
#include "cedar/auxiliaries/Path.h"

// FORWARD DECLARATIONS
#include "cedar/auxiliaries/gui/ResourceDialog.fwd.h"

// SYSTEM INCLUDES
#include <QFileIconProvider>
#include <vector>


/*!@todo describe.
 *
 * @todo describe more.
 */
class cedar::aux::gui::ResourceDialog : public QDialog, public Ui_ResourceDialog
{
  Q_OBJECT

  //--------------------------------------------------------------------------------------------------------------------
  // nested types
  //--------------------------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  ResourceDialog
  (
    QWidget* pParent = NULL,
    const std::vector<std::string>& extensions = std::vector<std::string>()
  );

  //!@brief Destructor
  virtual ~ResourceDialog();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  static cedar::aux::Path openResource
  (
    QWidget* pParent = NULL,
    const std::vector<std::string>& extensions = std::vector<std::string>()
  );

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  void fill();

  void removeEmpty(QTreeWidgetItem* pParent);

  void appendDirectories(const std::string& path, QTreeWidgetItem* pParent);
  void appendFiles(const std::string& path, QTreeWidgetItem* pParent);

  QTreeWidgetItem* findPathNode(const std::string& relativePath, QTreeWidgetItem* pParent = NULL) const;

  void setTextFromItem(QTreeWidgetItem* pItem);

private slots:
  void itemSelected();

  void itemActivated(QTreeWidgetItem* pItem, int);

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet
private:
  std::vector<std::string> mExtensions;

  QFileIconProvider* mIconProvider;

  static const int ITEM_TYPE_FOLDER;
  static const int ITEM_TYPE_FILE;

  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

private:
  // none yet

}; // class cedar::aux::gui::ResourceDialog

#endif // CEDAR_AUX_GUI_RESOURCE_DIALOG_H
