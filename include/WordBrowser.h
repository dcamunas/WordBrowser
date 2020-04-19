#include <list>
#include <string>
#include <mutex>

/* Word finded (structure that stores the occurrence data)*/
struct WordFinded
{
    int line;
    std::string word;
    std::string anteccessor;
    std::string successor;
};

/* WordBrowser's class (acts as a search engine, registering the data of the thread that starts the search) */
class WordBrowser
{
private:
    int thread_id_;
    int start_line_;
    int end_line_;
    std::string color_;
    /* Ocurrences list */
    std::list<WordFinded> ocurrences_;

/*Declarations of class methods (in ../src/WordFinded.cpp)*/
public:
    WordBrowser(int thread_id, int start_line, int end_line, std::string color);
    ~WordBrowser();
    void add_ocurrence(int line, std::string word, std::string antecessor, std::string successor);
    int get_start_line();
    int get_end_line();
    int get_thread_id();
    int get_ocurrences_number();
    void print_occurrence(WordFinded wf);
    void print();

};
