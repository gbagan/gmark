/*
 * MIT License
 *
 * Copyright (c) 2020 Thomas Calmant
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <iostream>
#include <list>
#include <map>
#include <string>

/**
 * Namespace containing all that concerns this argument parsing library
 */
namespace argparse
{
/**
     * Defines the mode of the argument: flag or valued
     */
enum ArgumentMode
{
    /**
     * Flag mode: we want to detect the presence of the argument
     */
    FLAG,
    /**
     * Store mode: we associate a value to the argument
     */
    STORE,
};

/**
 * Keeps track of the description of an argument
 */
class ArgumentDescription
{
private:
    /**
     * Identifier of the argument (to get it afterwards)
     */
    const std::string m_id;

    /**
     * List of the aliases used to access this argument
     */
    std::list<std::string> m_aliases;

    /**
     * Mode of the argument (flag, ...)
     */
    const ArgumentMode m_mode;

    /**
     * Description of the argument
     */
    const std::string m_description;

    /**
     * Default value, if any
     */
    const char *m_default;

public:
    /**
     * Sets up the argument description
     *
     * @param id ID of this argument (to get its value after parsing)
     * @param description Description of the argument
     * @param mode Argument mode: flag (no value) or store (single value)
     * @param default_value: Default value if none is given
     */
    ArgumentDescription(
        std::string const &id,
        std::string const &description,
        ArgumentMode const &mode = ArgumentMode::FLAG,
        const char *default_value = nullptr);

    /**
     * Adds an alias to this argument (for the usage message only)
     *
     * @param alias An alias to use this argument
     */
    void add_alias(std::string const &alias)
    {
        m_aliases.push_back(alias);
    }

    /**
     * @returns The ID of this argument
     */
    const std::string id() const
    {
        return m_id;
    }

    /**
     * @returns A copy of the aliases to access this argument
     */
    const std::list<std::string> aliases() const
    {
        return std::list<std::string>(m_aliases);
    }

    /**
     * @returns The mode of this argument (flag, store)
     */
    const ArgumentMode mode() const
    {
        return m_mode;
    }

    /**
     * @returns The description of this argument
     */
    const std::string description() const
    {
        return m_description;
    }

    /**
     * @returns The default value of this argument (can be null)
     */
    const char *default_value() const
    {
        return m_default;
    }
};

/**
 * Stores the result of the parsing of a single argument
 */
class ParsedArgument
{
private:
    /**
     * The decription of the parsed argument
     */
    const ArgumentDescription &m_argument;

    /**
     * The parsed value (empty string if none found)
     */
    std::string m_value;

    /**
     * A flag indicating if the value was given or not
     */
    bool m_set;

public:
    /**
     * @param argument The description of the parsed argument
     * @param parse_value The value given as argument (can be null)
     */
    ParsedArgument(ArgumentDescription const &argument, const char *parsed_value);

    /**
     * Returns the value of this argument.
     *
     * It can be the given value, the default value or an empty string
     *
     * @returns The value of this argument
     */
    const std::string get_value() const
    {
        return m_value;
    }

    /**
     * A flag to indicate if the value was given as argument, or if the flag
     * argument was given.
     *
     * This flag is False if the default value had to be used.
     *
     * @return True if the argument was given by the caller
     */
    bool is_set() const
    {
        return m_set;
    }
};

/**
 * Manages the arguments of the application
 */
class ArgumentParser
{
private:
    /**
     * Name of the program
     */
    const std::string m_name;

    /**
     * List of the arguments in their registration order (for the usage message)
     */
    std::list<ArgumentDescription> m_arguments;

    /**
     * Map of the argument ID to its description
     */
    std::map<const std::string, ArgumentDescription> m_ids;

    /**
     * Map of each arguments aliases to their description
     */
    std::map<const std::string, ArgumentDescription> m_aliases;

    /**
     * Map of the argument ID to its parsed value
     */
    std::map<const std::string, ParsedArgument> m_parsed_arguments;

public:
    /**
     * @param program_name Name of the program (use in usage message)
     */
    ArgumentParser(std::string const &program_name);

    /**
     * Registers an argument to parse
     *
     * @param id ID of the argument, to retrieve its value with get()
     * @param short_name Short alias of the argument, e.g. "-h"
     *      (empty string to ignore)
     * @param long_name Long alias of the argument, e.g. "--help"
     *      (empty string to ignore)
     * @param description Description of the argument
     * @param mode Argument mode (flag, store), flag mode by default
     * @param default_value Value to use if none was given (null by default)
     * @throws std::string Invalid alias
     * @throws char* Invalid alias
     */
    void add_argument(
        std::string const &id,
        std::string const &short_name,
        std::string const &long_name,
        std::string const &description,
        ArgumentMode const &mode = ArgumentMode::FLAG,
        const char *default_value = nullptr);

    /**
     * Parses the given arguments
     *
     * @param args The list of arguments given to the program
     * @param argc The number of arguments
     * @return True on success, False if an unknown argument is encountered
     */
    bool parse(const char **args, const int argc);

    /**
     * Retrieves the value of a parse argument.
     *
     * This method shouldn't be called before parse()
     *
     * @param id ID of the argument
     * @return The parsed argument, with its description and its value
     * @throws std::string Unknown argument ID
     */
    ParsedArgument get(std::string const &id);

    /**
     * Prints the usage of this program on the given output stream
     *
     * @param stream The output stream to use
     */
    void print_usage(std::ostream &stream = std::cout);
};
} // namespace argparse

#endif
