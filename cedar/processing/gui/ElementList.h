/*======================================================================================================================

    Copyright 2011, 2012, 2013, 2014, 2015 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        ElementList.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2015 03 30

    Description: Header file for the class cedar::proc::gui::ElementList.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_PROC_GUI_ELEMENT_LIST_H
#define CEDAR_PROC_GUI_ELEMENT_LIST_H

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES

// FORWARD DECLARATIONS
#include "cedar/processing/gui/ElementList.fwd.h"
#include "cedar/processing/ElementDeclaration.fwd.h"
#include "cedar/processing/GroupDeclaration.fwd.h"
#include "cedar/auxiliaries/PluginDeclaration.fwd.h"

// SYSTEM INCLUDES
#include <QTabWidget>
#include <QListWidget>


/*!@brief A widget for displaying elements that can be added to an architecture.
 */
class cedar::proc::gui::ElementList : public QTabWidget
{
  Q_OBJECT

  //--------------------------------------------------------------------------------------------------------------------
  // nested types
  //--------------------------------------------------------------------------------------------------------------------
private:
  class TabBase : public QListWidget
  {
    public:
      TabBase(QWidget* pParent = nullptr);

    protected:
      //! What drop actions are supported by this widget.
      Qt::DropActions supportedDropActions() const;

      //! Opens up the context menu.
      void contextMenuEvent(QContextMenuEvent* pEvent);

      void addEntry(cedar::aux::ConstPluginDeclarationPtr declaration);

      cedar::aux::ConstPluginDeclaration* getDeclarationFromItem(QListWidgetItem* pItem) const;

    private:
      void addElementDeclaration(cedar::proc::ConstElementDeclarationPtr declaration);

      void addGroupDeclaration(cedar::proc::ConstGroupDeclarationPtr declaration);

      void addListEntry
      (
        const std::string& className,
        const std::string& fullClassName,
        const QIcon& icon,
        std::vector<QString> decorations,
        const std::string& description,
        const std::string& deprecation,
        const std::string& source,
        cedar::aux::ConstPluginDeclarationPtr declaration
      );
  };

  class CategoryTab : public TabBase
  {
    public:
      CategoryTab(const std::string& categoryName, QWidget* pParent = nullptr);

      void update();

    private:
      std::string mCategoryName;
  };

  class FavoritesTab : public TabBase
  {
    public:
      FavoritesTab(QWidget* pParent = nullptr);

      void update();
  };

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  ElementList(QWidget* pParent = nullptr);

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:

public slots:
  void reset();

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet
private:
  //! Tab containing the user's favorite elements
  FavoritesTab* mpFavoritesTab;

  //! Widgets for each list of element classes.
  std::map<std::string, CategoryTab*> mCategoryWidgets;
}; // class cedar::proc::gui::ElementList

#endif // CEDAR_PROC_GUI_ELEMENT_LIST_H

