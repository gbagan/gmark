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

#include <iostream>

#include "argparse.hpp"

namespace argparse
{

ArgumentDescription::ArgumentDescription(
    std::string const &id,
    std::string const &description,
    ArgumentMode const &mode,
    const char *default_value) : m_id(id),
                           m_mode(mode),
                           m_description(description),
                           m_default(default_value)
{
}

ParsedArgument::ParsedArgument(
    ArgumentDescription const &argument,
    const char *parsed_value) : m_argument(argument)
{
    if (parsed_value != nullptr)
    {
        m_set = true;
        m_value = parsed_value;
    }
    else
    {
        m_set = false;
        const char *default_value = argument.default_value();
        if (default_value != nullptr)
        {
            m_value = default_value;
        }
        else
        {
            m_value = "";
        }
    }
}

ArgumentParser::ArgumentParser(
    std::string const &program_name) : m_name(program_name),
                                       m_aliases(),
                                       m_arguments(),
                                       m_parsed_arguments()
{
}

void ArgumentParser::add_argument(
    std::string const &id,
    std::string const &short_name,
    std::string const &long_name,
    std::string const &description,
    ArgumentMode const &mode,
    const char *default_value)
{
    if (!short_name.empty() && m_aliases.find(short_name) != m_aliases.end())
    {
        throw "ERROR: argument short alias " + short_name + " is already used.";
    }
    else if (!long_name.empty() && m_aliases.find(long_name) != m_aliases.end())
    {
        throw "ERROR: argument long alias " + long_name + " is already used.";
    }
    else if (short_name.empty() && long_name.empty())
    {
        throw "ERROR: at least one alias must be given";
    }

    ArgumentDescription descr(id, description, mode, default_value);
    if (!short_name.empty())
    {
        descr.add_alias(short_name);
        m_aliases.insert(std::make_pair(short_name, descr));
    }

    if (!long_name.empty())
    {
        descr.add_alias(long_name);
        m_aliases.insert(std::make_pair(long_name, descr));
    }

    m_arguments.push_back(descr);
    m_ids.insert(std::make_pair(id, descr));
}

bool ArgumentParser::parse(const char **argv, const int argc)
{
    for (int i = 1; i < argc; i++)
    {
        const char *arg_name = argv[i];

        auto it = m_aliases.find(arg_name);
        if (it == m_aliases.end())
        {
            std::cerr << "Unknown argument: " << arg_name << std::endl;
            print_usage(std::cerr);
            return false;
        }

        // Get the argument description
        ArgumentDescription &arg = it->second;

        // Check if we need to parse the rest
        char const *value = nullptr;
        switch (arg.mode())
        {
        case FLAG:
            // Give a valid value for the flag
            value = arg_name;
            break;

        case STORE:
            if (i < argc)
            {
                value = argv[++i];
            }
            break;

        default:
            throw "Invalid argument mode: " + arg.mode();
        }

        ParsedArgument parsed(arg, value);
        m_parsed_arguments.insert(std::make_pair(arg.id(), parsed));
    }

    return true;
}

ParsedArgument ArgumentParser::get(std::string const &id)
{
    auto parsed_it = m_parsed_arguments.find(id);
    if (parsed_it == m_parsed_arguments.end())
    {
        // Argument wasn't parsed, check if it is known
        auto argument_id_it = m_ids.find(id);
        if (argument_id_it == m_ids.end())
        {
            throw "Unknown argument ID " + id;
        }
        else
        {
            // Return the argument with its default value
            return ParsedArgument(argument_id_it->second, nullptr);
        }
    }

    return parsed_it->second;
}

void ArgumentParser::print_usage(std::ostream &stream)
{
    using std::endl;

    stream << "Usage of " << m_name << endl;
    for (auto it = m_arguments.begin(); it != m_arguments.end(); it++)
    {
        ArgumentDescription &arg = *it;
        auto aliases = arg.aliases();
        size_t nb_aliases = aliases.size();
        int i = 0;
        for (auto it = aliases.begin(); it != aliases.end(); ++it)
        {
            // Print the alias
            stream << *it;
            if (++i != nb_aliases)
            {
                // Not the last alias, print the comma
                stream << ", ";
            }
        }
        stream << "\t" << arg.description() << endl;
    }

    stream << endl;
}

} // namespace argparse
