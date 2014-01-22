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

    File:        PluginDeclaration.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2013 01 18

    Description:

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_PLUGIN_DECLARATION_H
#define CEDAR_AUX_PLUGIN_DECLARATION_H

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/namespace.h"

// SYSTEM INCLUDES


/*!@todo describe.
 *
 * @todo describe more.
 */
class cedar::aux::PluginDeclaration
{
  //--------------------------------------------------------------------------------------------------------------------
  // nested types
  //--------------------------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  PluginDeclaration();

  //! Constructor that takes a class name.
  PluginDeclaration(const std::string& className, const std::string& category);

  //!@brief Destructor
  virtual ~PluginDeclaration();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  /*!@brief Declares this plugin with the appropriate manager.
   */
  virtual void declare() const = 0;

  //! Returns the class name associated with this declaration.
  virtual std::string getClassName() const = 0;

  //! Returns the category of this declaration.
  inline const std::string& getCategory() const
  {
    return this->mCategory;
  }

  /*!@brief Marks the declared type as deprecated.
   */
  void deprecate(const std::string& description = "")
  {
    this->mIsDeprecated = true;
    this->mDeprecationDescription = description;
  }

  //! Returns whether this step is deprecated.
  bool isDeprecated() const
  {
    return this->mIsDeprecated;
  }

  //! Returns the deprecation description of the declaration.
  const std::string& getDeprecationDescription() const
  {
    return this->mDeprecationDescription;
  }

  //! Adds a deprecated name to the declaration.
  void deprecatedName(const std::string& oldName)
  {
    this->mDeprecatedNames.push_back(oldName);
  }

  //! Returns the deprecated names of this class.
  const std::vector<std::string>& deprecatedNames() const
  {
    return this->mDeprecatedNames;
  }

  //! Returns whether the given name is a deprecated name of this declaration.
  bool isDeprecatedName(const std::string& name) const;

  /*!
   * @brief Returns the class name without the preceding namespace.
   */
  std::string getClassNameWithoutNamespace() const;

  /*!
   * @brief Returns the namespace name without the class name.
   */
  std::string getNamespaceName() const;

  /*! @brief Reads parts of the class from the given configuration node.
   */
  void read(const cedar::aux::ConfigurationNode& node);

  /*! Sets the source of this declaration. Should be empty for build-in plugins.
   *
   * @remarks This is automatically set by the plugin system.
   *
   * @todo This should be done via being friends with PluginProxy, which is currently in proc.
   */
  void setSource(const std::string& source);

  /*! Returns the source of this declaration, i.e., the plugin which declared it. Empty for built-in types.
   */
  inline const std::string& getSource() const
  {
    return this->mSource;
  }

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  //! Implement this to read custom values from plugin description files.
  virtual void customRead(const cedar::aux::ConfigurationNode& node);

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //! Name of the class. Empty if an automatically generated name is to be used.
  std::string mClassName;

private:
  //! Category for this declaration.
  std::string mCategory;

  //! Whether or not the type declared with this object is deprecated.
  bool mIsDeprecated;

  //! Description of the deprecation.
  std::string mDeprecationDescription;

  //! A list of deprecated names for this class.
  std::vector<std::string> mDeprecatedNames;

  //! Source of the declaration. Empty for built-ins.
  std::string mSource;

}; // class cedar::aux::PluginDeclaration


/*!@todo describe.
 *
 * @todo describe more.
 */
template <class BaseTypePtr>
class cedar::aux::PluginDeclarationBaseTemplate : public cedar::aux::PluginDeclaration
{
public:
  //!@brief The standard constructor.
  PluginDeclarationBaseTemplate()
  {
  }

  //! Constructor that takes a class name.
  PluginDeclarationBaseTemplate(const std::string& category, const std::string& className)
  :
  PluginDeclaration(className, category)
  {
  }

  /*!@brief Checks if an Element is of the plugin class's type.
   *
   * @param pointer Instance that is checked
   */
  virtual bool isObjectInstanceOf(BaseTypePtr pointer) const = 0;
};

#endif // CEDAR_AUX_PLUGIN_DECLARATION_H
