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
T ParseArg(int &cur_arg, int num_arg, char *argv[])
{
    if constexpr (std::is_same_v<T, bool>)
        return true;

    if (cur_arg >= num_arg)
        throw std::invalid_argument("the required option is not specified");

    if constexpr (std::is_unsigned_v<T>)
    {
        try
        {
            return std::stoull(std::string(argv[cur_arg++]));
        }
        catch (std::invalid_argument &e)
        {
            throw std::invalid_argument(argv[cur_arg]);
        }
    }
    else if constexpr (std::is_same_v<T, char *>)
        return argv[cur_arg++];
    else
        static_assert("Unsupported type");
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
        printf("%s ", opt_name_);
        if (arg_name_)
            printf("[%s] ", arg_name_);
        printf("(value = ");
        if constexpr (std::is_same_v<T, bool>)
            printf("%s", data_ ? "true" : "false");
        else if constexpr (std::is_unsigned_v<T>)
            printf("%llu", static_cast<unsigned long long>(data_));
        else if constexpr (std::is_same_v<T, char *>)
            printf("%s", data_ ? data_ : "<empty>");
        else
            static_assert("Unsupported type");
        printf(") - %s\n", opt_desc_);
    }
};

#define OPTION_DEF(var, type, def, opt_name, arg_name, desc) extern Option<type> var;
#include "options_list.h"
#undef OPTION_DEF
extern FILE *log;

void ParseOptions(int argc, char *argv[]);
void OpenLog();
void CloseLog();
}   // namespace options
#endif
