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

    File:        FrameworkSettings.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2011 07 26

    Description:

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/processing/FrameworkSettings.h"
#include "cedar/auxiliaries/SetParameter.h"
#include "cedar/auxiliaries/DirectoryParameter.h"
#include "cedar/auxiliaries/systemFunctions.h"
#include "cedar/processing/exceptions.h"

// SYSTEM INCLUDES
#include <boost/property_tree/json_parser.hpp>

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::FrameworkSettings::FrameworkSettings()
:
cedar::aux::Configurable()
{
  cedar::aux::ConfigurablePtr plugin_group(new cedar::aux::Configurable());
  this->addConfigurableChild("plugin", plugin_group);

  std::set<std::string> default_plugin_include_directories;
  this->mPluginIncludeDirectories = cedar::aux::StringSetParameterPtr
                                    (
                                      new cedar::aux::StringSetParameter
                                      (
                                        plugin_group.get(),
                                        "includeDirectories",
                                        default_plugin_include_directories
                                      )
                                    );

  mPluginWorkspace = cedar::aux::DirectoryParameterPtr
                     (
                         new cedar::aux::DirectoryParameter
                         (
                           plugin_group.get(),
                           "workspace",
                            cedar::aux::getUserHomeDirectory() + "/src/"
                         )
                     );

  std::set<std::string> default_known_plugins;
  this->mKnownPlugins = cedar::aux::StringSetParameterPtr
                        (
                          new cedar::aux::StringSetParameter
                          (
                            plugin_group.get(),
                            "knownPlugins",
                            default_known_plugins
                          )
                        );

  try
  {
    this->load();
  }
  catch(cedar::aux::ParseException& exc)
  {
    cedar::aux::LogSingleton::getInstance()->systemInfo
    (
      "Could not load framework settings. A new file will be created.",
      "cedar::proc::FrameworkSettings::FrameworkSettings()"
    );
  }
}

cedar::proc::FrameworkSettings::~FrameworkSettings()
{
  try
  {
    this->save();
  }
  catch(cedar::aux::ParseException& exc)
  {
    cedar::aux::LogSingleton::getInstance()->systemInfo
    (
      "Could not store framework settings. A new file will be created.",
      "cedar::proc::FrameworkSettings::FrameworkSettings()"
    );
  }
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::proc::FrameworkSettings::addKnownPlugin(const std::string& file)
{
  std::string modified_path = file;
  std::string workspace_path = this->mPluginWorkspace->getValue().absolutePath().toStdString();
  if (modified_path.find(workspace_path) == 0)
  {
    modified_path = modified_path.substr(workspace_path.length());

    if (modified_path.empty())
      return;

//do not remove leading slash on apple platform
#ifndef CEDAR_OS_APPLE
    if (modified_path.at(0) == '/')
    {
      modified_path = modified_path.substr(1);
    }
#endif // CEDAR_OS_APPLE
  }

  if (modified_path.empty())
    return;
  this->mKnownPlugins->insert(modified_path);
}

void cedar::proc::FrameworkSettings::removeKnownPlugin(const std::string& file)
{
  CEDAR_ASSERT(this->mKnownPlugins->contains(file))
  this->mKnownPlugins->erase(file);
}

const std::set<std::string>& cedar::proc::FrameworkSettings::getKnownPlugins() const
{
  return this->mKnownPlugins->get();
}

const std::set<std::string>& cedar::proc::FrameworkSettings::getPluginDirectories() const
{
  return this->mPluginIncludeDirectories->get();
}

std::string cedar::proc::FrameworkSettings::getPluginWorkspace() const
{
  return this->mPluginWorkspace->getValue().absolutePath().toStdString();
}

void cedar::proc::FrameworkSettings::load()
{
  std::string path = cedar::aux::getUserApplicationDataDirectory() + "/.cedar/processingFramework";
  try
  {
    this->readJson(path);
  }
  catch (const boost::property_tree::json_parser::json_parser_error& e)
  {
    CEDAR_THROW(cedar::aux::ParseException, "Error reading framework settings: " + std::string(e.what()));
  }
}

void cedar::proc::FrameworkSettings::save()
{
  std::string path = cedar::aux::getUserApplicationDataDirectory() + "/.cedar/processingFramework";
  try
  {
    this->writeJson(path);
  }
  catch (const boost::property_tree::json_parser::json_parser_error& e)
  {
    CEDAR_THROW(cedar::aux::ParseException, "Error saving framework settings: " + std::string(e.what()));
  }
}
