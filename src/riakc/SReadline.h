/*!
 @file  SReadline.h
 @brief C++ wrapper around libreadline.
 Supported: editing, history, custom completers, keymaps.
 Attention: implementation is not thread safe!
 It is mainly because the readline library provides
 pure C interface and has many calls for an "atomic"
 completion operation
 */
//
// Date:      17 December 2005
//            03 April    2006
//            20 April    2006
//            07 May      2006
//
// Copyright (c) Sergey Satskiy 2005 - 2006
//               <sergesatsky@yahoo.com>
//
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
#ifndef SREADLINE_H
#define SREADLINE_H

#include <stdio.h>

#include <readline/readline.h>
#include <readline/history.h>
#include <readline/keymaps.h>

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <map>

#include <boost/algorithm/string/trim.hpp>
#include <boost/tokenizer.hpp>
#include <boost/function.hpp>

/*! @brief Used to avoid linking errors in case
 of including into many compilation units
 */
namespace
{
//! @brief Tokens in a single variation of a user command
typedef std::vector<std::string> TokensStorage;
//! @brief Set of variations of user commands
typedef std::vector<TokensStorage> CompletionsStorage;
//! @brief Pressed key callback. Must return 0 if OK, != 0 otherwise
typedef boost::function<int(int, int)> KeyCallback;
//! @brief A set of keys binding
typedef std::map<int, KeyCallback> KeysBind;

const size_t DefaultHistoryLimit(64); //!< Default value for the history length
CompletionsStorage Completions; //!< Global storage of custom completions
TokensStorage Tokens; //!< Tokens storage for a single completion session
std::map<Keymap, KeysBind> Keymaps; //!< Global storage for keymaps

bool KeymapWasSetup(false); //!< Has sense if a keymap was
                            //!< setup before the first readline call
Keymap Earlykeymap(0); //!< The keymap which was setup before the first readline call

/*! @brief Custom completion generator
 @param text Pointer to a token to be completed
 @param State 0 for a first call, non 0 for all consequent calls
 */
char * Generator(const char * text, int State);

/*! @brief The function is called before trying to complete a token
 @param text A token to be completed
 @param start Index of the beginning of the token in the readline buffer
 @param end   Index of the end of the token in the readline buffer
 */
char ** UserCompletion(const char * text, int start, int end);

/*! @brief The function selects the set of bindings and
 makes the corresponding call.
 @param Count The parameter is passed by readline
 @param Key   The pressed key
 */
int KeyDispatcher(int Count, int Key);

/*! @brief The readline startup hook. It is required to setup the proper keymap.
 */
int StartupHook(void);

/*! @brief Compares all the Input tokens with starts tokens in the Pattern
 @param Pattern pattern tokens
 @param Input user input tokens
 @return true if first Input.size() tokens are equal to the pattern tokens
 */
template<typename Container>
bool AreTokensEqual(const Container & Pattern, const Container & Input)
{
    if (Input.size() > Pattern.size())
        return false;

    typename Container::const_iterator k(Pattern.begin());
    typename Container::const_iterator j(Input.begin());
    for (; j != Input.end(); ++k, ++j)
    {
        if (*k == "%file")
            continue;
        if (*k != *j)
            return false;
    }
    return true;
}

// See description near the prototype
template<typename ContainerType>
void SplitTokens(const std::string & Source, ContainerType & Container)
{
    typedef boost::tokenizer<boost::char_separator<char> > TokenizerType;

    boost::char_separator<char> Separators(" \t\n"); // Set of token separators
    TokenizerType Tokenizer(Source, Separators); // Tokens provider
    std::string SingleToken; // Temporary storage for a token

    Container.clear();
    for (TokenizerType::const_iterator k(Tokenizer.begin());
            k != Tokenizer.end(); ++k)
            {
        SingleToken = *k;
        boost::algorithm::trim(SingleToken);
        Container.push_back(SingleToken);
    }
}

// See description near the prototype
char ** UserCompletion(const char * text, int start, int end)
{
    // No default completion at all
    rl_attempted_completion_over = 1;

    if (Completions.empty())
        return NULL;

    // Memorize all the previous tokens
    std::string PreInput(rl_line_buffer, start);
    SplitTokens(PreInput, Tokens);

    // Detect whether we should call the standard file name completer or a custom one
    bool FoundPretender(false);

    for (CompletionsStorage::const_iterator k(Completions.begin());
            k != Completions.end(); ++k)
            {
        if (!AreTokensEqual(*k, Tokens))
            continue;

        if ((*k).size() > Tokens.size())
        {
            FoundPretender = true;
            if ((*k)[Tokens.size()] == "%file")
            {
                // Standard file name completer - called for the "%file" keyword
                return rl_completion_matches(text,
                        rl_filename_completion_function);
            }
        }
    }

    if (FoundPretender)
    {
        return rl_completion_matches(text, Generator);
    }
    return NULL;
}

// See description near the prototype
char * Generator(const char * text, int State)
{
    static int Length;
    static CompletionsStorage::const_iterator Iterator;

    if (State == 0)
    {
        Iterator = Completions.begin();
        Length = strlen(text);
    }

    for (; Iterator != Completions.end(); ++Iterator)
    {
        if (!AreTokensEqual(*Iterator, Tokens))
            continue;

        if ((*Iterator).size() > Tokens.size())
        {
            if ((*Iterator)[Tokens.size()] == "%file")
                continue;

            if (strncmp(text, (*Iterator)[Tokens.size()].c_str(), Length) == 0)
            {
                // readline will free the allocated memory
                char * NewString(
                        (char*)malloc(
                                strlen((*Iterator)[Tokens.size()].c_str())
                                        + 1));
                strcpy(NewString, (*Iterator)[Tokens.size()].c_str());
                ++Iterator;
                return NewString;
            }
        }
    }

    return NULL;
}

// See the description near the prototype
int KeyDispatcher(int Count, int Key)
{
    std::map<Keymap, KeysBind>::iterator Set(Keymaps.find(rl_get_keymap()));
    if (Set == Keymaps.end())
    {
        // Most probably it happens bacause the header was
        // included into many compilation units and the
        // keymap setting calls were made in different files.
        // This is the problem of "global" data.
        // The storage of all the registered keymaps is in anonymous
        // namespace.
        throw std::runtime_error("Error selecting a keymap.");
    }

    (Set->second)[Key](Count, Key);
    return 0;
}

// See the description near the prototype
int StartupHook(void)
{
    if (KeymapWasSetup)
    {
        rl_set_keymap(Earlykeymap);
    }
    return 0;
}

} // Anonymous namespace

/*! @brief The wrapper namespace.
 The namespace is also used for other library elements.
 */
namespace swift
{

/*! @brief The readline keymap wrapper.
 Attention: It is not thread safe!
 Supports: key binding, key unbinding
 */
class SKeymap
{
private:
    Keymap keymap; // Readline keymap

public:
    /*! @brief Creates a new keymap
     @param PrintableBound if true - the printable characters are bound
     if false - the keymap is empty
     */
    explicit SKeymap(bool PrintableBound = false)
            : keymap(NULL)
    {
        if (PrintableBound)
        {
            keymap = rl_make_keymap(); // Printable characters are bound
        }
        else
        {
            keymap = rl_make_bare_keymap(); // Empty keymap
        }

        if (keymap == NULL)
        {
            throw std::runtime_error("Cannot allocate keymap.");
        }

        // Register a new keymap in the global list
        Keymaps[keymap] = KeysBind();
    }

    /*! @brief Creates a new keymap which is a copy of Pattern
     @param Pattern A keymap to be copied
     */
    explicit SKeymap(Keymap Pattern)
            : keymap(rl_copy_keymap(Pattern))
    {
        if (keymap == NULL)
        {
            throw std::runtime_error("Cannot allocate keymap.");
        }

        // Register a new keymap in the global list
        Keymaps[keymap] = KeysBind();
    }

    /*! @brief Frees the allocated keymap
     */
    ~SKeymap()
    {
        // Deregister the keymap
        Keymaps.erase(keymap);
        rl_discard_keymap(keymap);
    }

    /*! @brief Binds the given key to a function
     @param Key A key to be bound
     @param Callback A function to be called when the Key is pressed
     */
    void Bind(int Key, KeyCallback Callback)
    {
        Keymaps[keymap][Key] = Callback;
        if (rl_bind_key_in_map(Key, KeyDispatcher, keymap) != 0)
        {
            // Remove from the map just bound key
            Keymaps[keymap].erase(Key);
            throw std::runtime_error("Invalid key.");
        }
    }

    /*! @brief Unbinds the given key
     @param Key A key to be unbound
     */
    void Unbind(int Key)
    {
        rl_unbind_key_in_map(Key, keymap);
        Keymaps[keymap].erase(Key);
    }

    // void Bind( const std::string &  Sequence, boost::function< int ( int, int ) > );
    // void Unbind( std::string &  Sequence );

public:
    /*! @brief Copy constructor
     @param rhs Right hand side object of SKeymap
     */
    SKeymap(const SKeymap & rhs)
    {
        if (this == &rhs)
        {
            return;
        }
        keymap = rl_copy_keymap(rhs.keymap);
    }

    /*! @brief operator=
     @param rhs Right hand side object of SKeymap
     */
    SKeymap & operator=(const SKeymap & rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }
        keymap = rl_copy_keymap(rhs.keymap);
        return *this;
    }

    friend class SReadline;
};

/*! @brief The readline library wrapper.
 Attention: It is not thread safe!
 Supports: editing, history, custom completers
 */
class SReadline
{
public:
    /*! @brief Constructs the object, sets the completion function
     @param Limit History size
     */
    SReadline(const size_t Limit = DefaultHistoryLimit)
            : HistoryLimit(Limit), HistoryFileName(""), OriginalCompletion(
                    rl_attempted_completion_function)
    {
        rl_startup_hook = StartupHook;
        rl_attempted_completion_function = UserCompletion;
        using_history();
    }

    /*! @brief Constructs the object, sets the completion function, loads history
     @param historyFileName File name to load history from
     @param Limit History size
     */
    SReadline(const std::string & historyFileName, const size_t Limit =
            DefaultHistoryLimit)
            : HistoryLimit(Limit), HistoryFileName(historyFileName), OriginalCompletion(
                    rl_attempted_completion_function)
    {
        rl_startup_hook = StartupHook;
        rl_attempted_completion_function = UserCompletion;
        using_history();
        LoadHistory(HistoryFileName);
    }

    /*! @brief Saves the session history (if the file name was provided)
     and destroys the object
     */
    ~SReadline()
    {
        rl_attempted_completion_function = OriginalCompletion;
        SaveHistory(HistoryFileName);
    }

    /*! @brief Gets a single line from a user
     @param Prompt A printed prompt
     @return A string which was actually inputed
     */
    std::string GetLine(const std::string & Prompt)
    {
        bool Unused;
        return GetLine(Prompt, Unused);
    }

    /*! @brief Gets a single line from a user
     @param Prompt A printed prompt
     @param ReadTokens A user inputed string splitted into tokens.
     The container is cleared first
     @return A string which was actually inputed
     */
    template<typename Container>
    std::string GetLine(const std::string & Prompt, Container & ReadTokens)
    {
        bool Unused;
        return GetLine(Prompt, ReadTokens, Unused);
    }

    /*! @brief Gets a single line from a user
     @param Prompt A printed prompt
     @param BreakOut it is set to true if the EOF found
     @param ReadTokens A user inputed string splitted into tokens.
     The container is cleared first
     @return A string which was actually inputed
     */
    template<typename Container>
    std::string GetLine(const std::string & Prompt, Container & ReadTokens,
            bool & BreakOut)
    {
        std::string Input(GetLine(Prompt, BreakOut));
        SplitTokens(Input, ReadTokens);
        return Input;
    }

    /*! @brief Gets a single line from a user
     @param Prompt A printed prompt
     @param BreakOut it is set to true if the EOF found
     @return A string which was actually inputed
     */
    std::string GetLine(const std::string & Prompt, bool & BreakOut)
    {
        BreakOut = true;

        char * ReadLine(readline(Prompt.c_str()));
        if (ReadLine == NULL)
            return std::string();

        // It's OK
        BreakOut = false;
        std::string Input(ReadLine);
        free(ReadLine);

        boost::algorithm::trim(Input);
        if (!Input.empty())
        {
            if ((history_length == 0)
                    || (Input != history_list()[history_length - 1]->line))
            {
                add_history(Input.c_str());
                if (history_length >= static_cast<int>(HistoryLimit))
                {
                    stifle_history(HistoryLimit);
                }
            }
        }

        return Input;
    }

    /*! @brief Fills the given container with the current history list
     Does not clear the given container
     */
    template<typename ContainerType>
    void GetHistory(ContainerType & Container)
    {
        for (int k(0); k < history_length; ++k)
        {
            Container.push_back(history_list()[k]->line);
        }
    }

    /*! @brief Saves the history to the given file stream
     @param OS output file stream
     @return true if success
     */
    bool SaveHistory(std::ostream & OS)
    {
        if (!OS)
            return false;
        for (int k(0); k < history_length; ++k)
        {
            OS << history_list()[k]->line << std::endl;
        }
        return true;
    }

    /*! @brief Saves the history to the given file
     @param FileName File name to save the history to
     @return true if success
     */
    bool SaveHistory(const std::string & FileName)
    {
        if (FileName.empty())
            return false;

        std::ofstream OS(FileName.c_str());
        return SaveHistory(OS);
    }

    /*! @brief Clears the history. Does not affect the file where
     the previous session history is saved.
     */
    void ClearHistory()
    {
        clear_history();
    }

    /*! @brief Loads a history from a file stream
     @param IS Input file stream
     @return true if success
     */
    bool LoadHistory(std::istream & IS)
    {
        if (!IS)
            return false;

        ClearHistory();
        std::string OneLine;

        while (!getline(IS, OneLine).eof())
        {
            boost::algorithm::trim(OneLine);
            if ((history_length == 0)
                    || (OneLine != history_list()[history_length - 1]->line))
            {
                add_history(OneLine.c_str());
            }
        }
        stifle_history(HistoryLimit);
        return true;
    }

    /*! @brief Loads a history from the given file
     @param FileName File name to be load from
     @return true if success
     */
    bool LoadHistory(const std::string & FileName)
    {
        if (FileName.empty())
            return false;

        std::ifstream IS(FileName.c_str());
        return LoadHistory(IS);
    }

    /*! @brief Allows to register custom completers.
     Supports a special keyword: %file. It means to use the standard file name completer
     For example the given container elements could be as follows:
     command1 opt1
     command1 opt2 %file
     command2
     command2 opt1
     Each container element must describe a single possible command line
     The container element must have a conversion to std::string operator
     @param Container A container which has all the user possible commands
     */
    template<typename ContainerType>
    void RegisterCompletions(const ContainerType & Container)
    {
        Completions.clear();
        for (typename ContainerType::const_iterator k(Container.begin());
                k != Container.end(); ++k)
                {
            std::vector<std::string> OneLine;
            SplitTokens(static_cast<std::string>(*k), OneLine);
            Completions.push_back(OneLine);
        }
    }

    /*! @brief Sets the given keymap
     @param NewKeymap The keymap that should be used from now
     */
    void SetKeymap(SKeymap & NewKeymap)
    {
        rl_set_keymap(NewKeymap.keymap);
        KeymapWasSetup = true;
        Earlykeymap = NewKeymap.keymap;
    }

private:
    const size_t HistoryLimit; //!< Constructor accepted or default
    const std::string HistoryFileName; //!< Original constructor accepted
    rl_completion_func_t * OriginalCompletion; //!< Original state will be restored
};

}
;
// namespace swift

#endif

