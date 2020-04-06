#include <sstream>
#include <string.h>
#include <algorithm>


/* Path constant */
#define FILES_PATH       "files/"
#define ARGC_NUMBER      4
#define SEPARATOR        ' '
#define FIRST_POSITION   0
#define COLORS_NUMBER    6

/* ------------------------------------------------------------------------------ */

/*My String Tokenizer*/
std::vector <std::string> string_tokenizer(std::string line)
{  
    // Vector of string to save tokens 
    std::vector <std::string> words_vector; 
    std::stringstream check(line);  
    std::string word; 
      
    while(getline(check, word, SEPARATOR)) 
        words_vector.push_back(word); 

    return words_vector;
}

/* Change from uppercase to lowercase */
void upper_to_lower(std::string &upper_word)
{
    std::transform(upper_word.begin(), upper_word.end(), upper_word.begin(), ::tolower);
}

/* ------------------------------------------------------------------------------ */

/* Colors */
#define RESET       "\033[0m"              /* Default */
#define BLACK       "\033[30m"             /* Black */
#define RED         "\033[31m"             /* Red */
#define GREEN       "\033[32m"             /* Green */
#define YELLOW      "\033[33m"             /* Yellow */
#define BLUE        "\033[34m"             /* Blue */
#define MAGENTA     "\033[35m"             /* Magenta */
#define CYAN        "\033[36m"             /* Cyan */
#define WHITE       "\033[37m"             /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

/* Get in: https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c */