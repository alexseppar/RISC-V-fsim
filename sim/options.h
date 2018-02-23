#ifndef OPTIONS_H
#define OPTIONS_H
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace options
{
template<typename T>
std::enable_if_t<std::is_same<T, bool>::value, T> ParseArg(int &cur_arg
                                                           __attribute__((unused)),
                                                           int num_arg __attribute__((unused)),
                                                           char *argv[]
                                                           __attribute__((unused)))
{
    return true;
}

template<typename T>
std::enable_if_t<std::is_same<std::remove_const_t<T>, char *>::value, T> ParseArg(int &cur_arg,
                                                                                  int num_arg,
                                                                                  char *argv[])
{
    if (cur_arg < num_arg)
        return argv[cur_arg++];
    else
        throw std::invalid_argument("the required option is not specified");
}

template<typename T>
std::enable_if_t<std::is_unsigned<T>::value && !std::is_same<T, bool>::value, T>
ParseArg(int &cur_arg, int num_arg, char *argv[])
{
    if (cur_arg < num_arg)
        try
        {
            return std::stoull(std::string(argv[cur_arg++]));
        }
        catch (std::invalid_argument &e)
        {
            throw std::invalid_argument(argv[cur_arg - 1]);
        }
    else
        throw std::invalid_argument("the required option is not specified");
}

template<typename T>
class Option
{
private:
    T data_;
    const char *opt_name_;
    const char *arg_name_;
    const char *opt_desc_;

public:
    Option(T default_value, const char *opt_name, const char *arg_name, const char *opt_desc)
        : data_(default_value)
        , opt_name_(opt_name)
        , arg_name_(arg_name)
        , opt_desc_(opt_desc)
    {
    }
    operator T() const
    {
        return data_;
    }
    bool ParseOption(int &cur_arg, int num_arg, char *argv[])
    {
        if (!strcmp(opt_name_, argv[cur_arg]))
        {
            ++cur_arg;
            try
            {
                data_ = ParseArg<T>(cur_arg, num_arg, argv);
            }
            catch (std::exception &e)
            {
                throw std::invalid_argument(std::string("Unrecognized ") + opt_name_ +
                                            " option: " + e.what());
            }
            return true;
        }
        else
            return false;
    }
    void PrintHelp()
    {
        if (arg_name_)
            printf("%s [%s] - %s\n", opt_name_, arg_name_, opt_desc_);
        else
            printf("%s - %s\n", opt_name_, opt_desc_);
    }
};

#define OPTION_DEF(var, type, def, opt_name, arg_name, desc) extern Option<type> var;
#include "options_list.h"
#undef OPTION_DEF
extern FILE *log;

void ParseOptions(int argc, char *argv[]);
void OpenLog();
void CloseLog();
}
#endif
