
#pragma once

#include "Any.hh"
#include "Parser.hh"

#include <map>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

// TODO: Help Class
// TODO: Generate Format String.
// NOTE: Can't do multiWord strings.....

namespace ExpUtils {

///
/// @brief This is used when a tupple parser isn't enough when parsing command
/// line arguments. The API is developed based on the Python3 ArgParser utility
/// but made to be more deterministic
///
///
class ArgParser {
private:
  struct Arg;

public:
  struct Params;
  struct Help;

  enum ArgType { store, store_true, store_false, append, version };

  ArgParser(std::string _name, std::string _version)
    : name(_name), versionStr(_version) {}
  // ArgParser(std::string _version = "0.0"): versionStr(_version) {}
  ArgParser(std::string _version) : versionStr(_version) {
    std::cerr << "Depricated: Please use ArgParser(name,version)";
  }
  ArgParser() : ArgParser("", "") {}
  ~ArgParser() {}

  /// \brief These are the first several arguments and are all required. These
  /// occur at the begining of the arguments. Ordered Arguments can also only be
  /// single values.
  ///
  /// \param name
  ///
  void addOrderedArgument(const std::string &name) {
    orderedArguments.push_back(Arg(name, "", ArgType::store, false));
  }

  /// \brief True add argument function (all other functions will call this one.
  ///
  /// \param name
  /// \param shortname
  /// \param atype
  /// \param optional
  ///
  void addArgument(const std::string &name,
                   const std::string &shortname,
                   ArgType atype,
                   bool optional);

  /// \name Functions that add arguments via addArgument
  /// @{
  void addArgument(const std::string &name) {
    addArgument(name, "", ArgType::store, true);
  }
  void addArgument(const std::string &name,
                   const std::string &shortname,
                   bool optional = true) {
    addArgument(name, shortname, ArgType::store, optional);
  }
  void addArgument(const std::string &name,
                   const std::string &shortname,
                   ArgType atype) {
    addArgument(name, shortname, atype, true);
  }
  /// @}

  ///
  /// \brief Parser the arguments found in argv. No Through
  ///
  /// \param argv
  /// \param help
  /// \return Params
  ///
  Params parse(const std::vector<std::string> &argv, Help &help);

  /// /name Convenience functions for parse
  /// @{
  Params parse(size_t argc, char *argv[]) {
    // Make into a std::vector
    return parse(std::vector<std::string>(argv, argv + argc));
  }

  Params parse(size_t argc, char *argv[], Help &help) {
    // Make into a std::vector and pass to subsequent function.
    return parse(std::vector<std::string>(argv, argv + argc), help);
  }

  Params parse(size_t argc, const char *argv[]) {
    // Make into a std::vector
    return parse(std::vector<std::string>(argv, argv + argc));
  }

  Params parse(size_t argc, const char *argv[], Help &help) {
    // Make into a std::vector and pass to subsequent function.
    return parse(std::vector<std::string>(argv, argv + argc), help);
  }

  Params parse(const std::vector<std::string> &argv) {
    // bool success = true;
    Help help;

    Params Params = parse(argv, help);

    if (help.isSuccessful()) {
      // Return the resulting arguments from a successful parse.
      return Params;
    } else {
      // Not a successful parse, exit and report the error.
      std::cerr << help.report() << std::endl;
      throw std::runtime_error(help.report());
      // exit(1);
      return Params;
    }
  }
  ///@}

  ///
  /// \brief Convenience structure for resulting parameters
  /// 
  struct Params {
    Params(){};
    ~Params(){};

    bool contains(std::string name) {
      return nameToStringMap.find(name) != nameToStringMap.end();
    }

    void add(std::string name, std::string arguments) {
      nameToStringMap.insert({name, arguments});
    }

    template <typename T>
    T retrieve(std::string name) {
      return retrieve(name, T{});
    }

    template <typename T>
    T retrieve(std::string name, T def) {
      if (contains(name) == false)
        return def;
      T dat;
      if (convert(nameToStringMap[name], dat) == false) {
        throw std::runtime_error("Something went wrong with parsing (probably "
                                 "not the right type): " +
                                 name);
      }
      return dat;
    }

    std::map<std::string, std::string> nameToStringMap;

  }; // struct Params

  struct Help {
    Help(){};
    ~Help(){};

    std::string report() { return reportString; }

    void error(const ArgParser &argParser) {
      reportString = generateFormatString(argParser);
      successful = false;
    }

    void error(const std::string &err) {
      reportString = err;
      successful = false;
    }

    bool isSuccessful() { return successful; }
    // void setReportString(const std::string& rs) {reportString = rs;}
    // void setSuccessful(bool s){successful = s;}

    std::string generateFormatString(const ArgParser &argparser) {
      std::ostringstream ss;
      ss <<  "Format String Coming soon: "
         << argparser.name << " " << argparser.versionStr;
      return ss.str();
    }

  private:
    bool successful = true;
    std::string reportString = "";

  }; // struct Help

  // std::string generateFormatString() const{ return "Format String Coming
  // soon...."; }

private:
  // Map containing all information about parsing.
  std::map<std::string, Arg> nameToArgMap;
  // Makes parsing more convenient.
  std::map<std::string, Arg &> shortnameToArgMap;
  // Makes determining if we have all required arguments that don't include the
  // ordered arguments.
  std::vector<std::string> requiredNames;
  // The vector of ordered arguments that occur at the onset of the parsing.
  std::vector<Arg> orderedArguments;

  std::string name;
  std::string versionStr;

  // What does Arg need to do? or is it just convenient?
  struct Arg {
    // Default
    Arg() : Arg("", "", ArgType::store, false) {
      std::cout << "Help!!!" << std::endl;
    }
    // Contructor
    Arg(const std::string &_name,
        const std::string &_shortname,
        ArgType _argtype,
        bool _optional)
        : name(_name), shortname(_shortname), argtype(_argtype),
          optional(_optional) {}
    // Copy constructor

    Arg(const Arg &cp)
        : name(cp.name), shortname(cp.shortname), argtype(cp.argtype),
          optional(cp.optional){};

    ~Arg(){};

    std::string name;
    std::string shortname;
    ArgType argtype;
    bool optional;
  };

}; // Class Argparser

template <>
inline bool ArgParser::Params::retrieve<bool>(std::string name) {
  if (contains(name) == false)
    return false;

  if (nameToStringMap[name] == "0")
    return false;
  else
    return true;
}

inline void ArgParser::addArgument(const std::string &name,
                                   const std::string &shortname,
                                   ArgType atype,
                                   bool optional) {

  if (nameToArgMap.find(name) != nameToArgMap.end() ||
      shortnameToArgMap.find(shortname) != shortnameToArgMap.end()) {
    // Can't have the same name or shortname twice. Undefined behavior. Exit.
    std::cerr << "ArgParser: " + name + " or " + shortname + " already exists."
              << std::endl;
    throw std::runtime_error("ArgParser: " + name + " or " + shortname +
                             " already exists.");
  }

  nameToArgMap.insert({name, ArgParser::Arg(name, shortname, atype, optional)});
  if (shortname != "") {
    shortnameToArgMap.insert({shortname, nameToArgMap.at(name)});
  }

  if (optional == false) {

    requiredNames.push_back(name);

  } else if (nameToArgMap[name].argtype == ArgType::store_true ||
             nameToArgMap[name].argtype == ArgType::store_false) {
    requiredNames.push_back(name);
  }

  // Lets add this to the formater string I think?. Or we should do this at
  // runtime.... in generateFormatString.
}

inline ArgParser::Params ArgParser::parse(const std::vector<std::string> &argv,
                                        Help &help) {

  Params ret{};

  std::vector<std::string> requiredNamescp = requiredNames;

  std::string token;
  std::string app_name = argv[0];
  int j = 1;
  for (size_t i = 1; i < orderedArguments.size() + 1; i++) {
    // Are we still within our limits?
    if (i == argv.size()) {
      help.error(*this);
      return ret;
    }
    // So we want to get all the orderedArguments as single values.
    std::string cur = argv[i];
    if (cur.at(0) == '-') {
      // We haven't gotten to the end of our orderedArguments...
      help.error(*this);
      return ret;
    }

    // Now get single argument for ordered argument.
    ret.nameToStringMap[orderedArguments[i - 1].name] = argv[i];

    j++;
  }

  // Look through the other arguments.
  for (size_t i = j; i < argv.size(); i++) {
    // i should always be within argv.
    std::string cur = argv[i];

    if (cur.at(0) == '-') {
      // New argument.
      if (cur.substr(0, 2) == "--") {
        // No short name

        // Check long names.
        if (nameToArgMap.find(cur.substr(2)) != nameToArgMap.end()) {
          // If exists
          cur = cur.substr(2);
          auto itr =
              std::find(requiredNamescp.begin(), requiredNamescp.end(), cur);
          if (itr != requiredNamescp.end()) {
            // requiredNames.era
            requiredNamescp.erase(itr);
          }

          auto arg = nameToArgMap.at(cur);
          // Check type
          std::string parsedString = "";
          switch (arg.argtype) {
          case store:
            // Want to store all subsequent values.... How should I do this?
            // Probably just here.
            // i;
            //
            while (i < argv.size() - 1) {
              if (argv[i + 1].at(0) == '-') {
                break;
              }
              // After check we know it is safe to incriment;
              i++;
              // Add it to the current string.
              parsedString += (parsedString == "") ? "" : " ";
              parsedString += argv[i];
            }
            break;
          case store_true:
            // Want to store "1"
            parsedString = "1";
            break;
          case store_false:
            // Want to store "0"
            parsedString = "0";
            break;
          case append:
            // Store subsequent Values and the currently stored values.
            if (ret.nameToStringMap.find(cur) != ret.nameToStringMap.end()) {
              parsedString = ret.nameToStringMap[cur];
            }
            // Add the subsequent values...
            while (i < argv.size() - 1) {
              if (argv[i + 1].at(0) == '-') {
                break;
              }
              // After check we know it is safe to incriment;
              i++;
              // Add it to the current string.
              parsedString += (parsedString == "") ? "" : " ";
              parsedString += argv[i];
            }
            break;
          case version:
            // Should we require Version number on construction of argparser??
            help.error(app_name + " " + versionStr);
            break;
          }
          ret.nameToStringMap[cur] = parsedString;
          // That should be it.

        } else {
          // What if it doesnt? Ignore? What about looking through short names?
        }

      } else {
        // Check short name list, if not there check long names?
        if (shortnameToArgMap.find(cur.substr(1)) != shortnameToArgMap.end()) {
          cur = cur.substr(1);

          auto arg = shortnameToArgMap.at(cur);

          auto itr = std::find(
              requiredNamescp.begin(), requiredNamescp.end(), arg.name);
          if (itr != requiredNamescp.end()) {
            // requiredNames.era
            requiredNamescp.erase(itr);
          }

          // Check type
          std::string parsedString = "";
          switch (arg.argtype) {
          case store:
            // Want to store all subsequent values.... How should I do this?
            // Probably just here.
            // i;
            //
            while (i < argv.size() - 1) {
              if (argv[i + 1].at(0) == '-') {
                break;
              }
              // After check we know it is safe to incriment;
              i++;
              // Add it to the current string.
              parsedString += (parsedString == "") ? "" : " ";
              parsedString += argv[i];
            }
            break;
          case store_true:
            // Want to store "1"
            parsedString = "1";
            break;
          case store_false:
            // Want to store "0"
            parsedString = "0";
            break;
          case append:
            // Store subsequent Values and the currently stored values.
            if (ret.nameToStringMap.find(arg.name) !=
                ret.nameToStringMap.end()) {
              parsedString = ret.nameToStringMap[arg.name];
            }
            // Add the subsequent values...
            while (i < argv.size() - 1) {
              if (argv[i + 1].at(0) == '-') {
                break;
              }
              // After check we know it is safe to incriment;
              i++;
              // Add it to the current string.
              parsedString += (parsedString == "") ? "" : " ";
              parsedString += argv[i];
            }
            break;
          case version:
            // Should we require Version number on construction of argparser??
            help.error(app_name + ": " + versionStr);
            break;
          }

          ret.nameToStringMap[arg.name] = parsedString;
        }

      } // if cur.substr(0, 2) == "--"

    } // if cur.at(0) == '-'
    else {
      // I shouldn't get anything here....
    }
  }

  if (requiredNamescp.size() != 0) {

    for (auto arg : requiredNamescp) {
      if (nameToArgMap[arg].argtype == store_true) {
        ret.nameToStringMap[arg] = "0";
      } else if (nameToArgMap[arg].argtype == store_false) {
        ret.nameToStringMap[arg] = "1";
      } else {
        help.error(*this);
      }
    }
  }

  return ret;
  // return Params{};
}

} // namespace ExpUtils

// #endif //_EXPUTILS_ARGPARSER_H
