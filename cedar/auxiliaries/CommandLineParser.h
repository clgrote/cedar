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

    File:        CommandLineParser.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2013 10 23

    Description:

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_COMMAND_LINE_PARSER_H
#define CEDAR_AUX_COMMAND_LINE_PARSER_H

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/namespace.h"
#include "cedar/auxiliaries/stringFunctions.h"

// SYSTEM INCLUDES
#include <iostream>
#include <set>
#include <map>
#include <vector>

/*!@brief A class for conveniently parsing options from a command line.
 *
 *        Can also read/write parameters to a file.
 *
 * @todo  defineValue: there should be a variant that allows giving values as any type (i.e., a templated version).
 */
class cedar::aux::CommandLineParser
{
  //--------------------------------------------------------------------------------------------------------------------
  // nested types
  //--------------------------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  CommandLineParser();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  /*!@brief Defines a command line flag, i.e., an option that does not expect a value.
   *
   * @param longName    Long name of the parameter. Should not contain spaces.
   * @param description Description to show to the user.
   * @param shortName   Short name of the command. Leave 0 to ignore.
   */
  void defineFlag
  (
    const std::string& longName,
    const std::string& description,
    char shortName = 0,
    const std::string& group = std::string()
  );

  /*!@brief Defines a command line value, i.e., an option that must be given a value. If the option is not present, the
   *        parser will return the default value.
   *
   * @param longName    Long name of the option.
   * @param description Description of the option.
   * @param shortName   Optional short name of the option.
   */
  void defineValue
  (
    const std::string& longName,
    const std::string& description,
    const std::string& defaultValue,
    char shortName = 0,
    const std::string& group = std::string()
  );

  /*!@brief Defines a command line value, i.e., an option that must be given a value. If this option is missing
   *
   * @param longName    Long name of the option.
   * @param description Description of the option.
   * @param shortName   Optional short name of the option.
   */
  void defineValue
  (
    const std::string& longName,
    const std::string& description,
    char shortName = 0,
    const std::string& group = std::string()
  );

  //! Test if a flag is present in the parsed inputs.
  bool hasFlag(const std::string& longName) const;

  /*!@brief   Parse the given command line options
   *
   * @remarks If a command such as help is specified, this may terminate the program unless this is explicitly disabled
   *          with the @em terminationAllowed parameter.
   */
  void parse(int argc, char* argv[], bool terminationAllowed = true);

  /*! Writes the help message to the given stream.
   */
  void writeHelp(std::ostream& stream = std::cout) const;

  /*! Writes a summary of the read values to the given stream.
   */
  void writeSummary(std::ostream& stream = std::cout) const;

  /*!@brief Returns a value with a given type.
   */
  template <typename T>
  T getValue(const std::string& longName) const
  {
    return cedar::aux::fromString<T>(this->getValue(longName));
  }

  void addOptionToGroup(const std::string& groupName, const std::string& longName);

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  void defineOption
       (
         const std::string& longName,
         const std::string& description,
         char shortName = 0,
         const std::string& group = std::string()
       );

  bool isFlag(const std::string& longName) const;

  const std::string& getLongName(char shortName);

  bool shortNameExists(char shortName) const;

  bool hasShortNameFor(const std::string& longName) const;

  char getShortNameFor(const std::string& longName) const;

  bool hasDefaultValue(const std::string& longName) const;

  const std::string& getDefaultValue(const std::string& longName) const;

  bool isDefined(const std::string& longName) const;

  /*!@brief Returns a value with a given type.
   */
  const std::string& getValue(const std::string& longName) const;

  void writeHeading(std::ostream& stream, const std::string& heading, char fill) const;

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet
private:
  //! argc and argv in a more modern structure.
  std::vector<std::string> mArguments;

  //! Contains all defined commands and their associated descriptions.
  std::map<std::string, std::string> mDescriptions;

  //! Contains all defined commands and their associated default values.
  std::map<std::string, std::string> mDefaultValues;

  //! Options stored in here are flags.
  std::set<std::string> mFlags;

  //! List of abbreviations
  std::map<char, std::string> mAbbreviations;

  //! Association of values found for parameters.
  std::map<std::string, std::string> mParsedValues;

  //! Long names of parsed flags.
  std::vector<std::string> mParsedFlags;

  //! Parameters in groups.
  std::map<std::string, std::set<std::string> > mGroups;

}; // class cedar::aux::CommandLineParser

#endif // CEDAR_AUX_COMMAND_LINE_PARSER_H

