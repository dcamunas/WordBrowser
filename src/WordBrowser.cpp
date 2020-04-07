#include <WordBrowser.h>
#include <string>
#include <vector>
#include <iostream>
#include <mutex>

#define RESET "\033[0m"     /* Default color */


/* Class constructor */
WordBrowser::WordBrowser(int thread_id, int start_line, int end_line, std::string color)
{
    thread_id_ = thread_id;
    start_line_ = start_line;
    end_line_ = end_line;
    color_ = color;
}

/* Class destructor */
WordBrowser::~WordBrowser() {}

/* Class methods */

void WordBrowser::add_ocurrence(int line, std::string word, std::string anteccessor, std::string successor, std::mutex &sem)
{
    /* Generate a new struct (WordFinded)*/
    WordFinded *wf = new WordFinded;
    wf->line = line;
    wf->word = word;
    wf->anteccessor = anteccessor;
    wf->successor = successor;

    /*Add struct to the list*/
    std::lock_guard<std::mutex> guard(sem);
    {
        ocurrences_.push_back(*wf);
    }
}

int WordBrowser::get_start_line() { return start_line_; }

int WordBrowser::get_end_line() { return end_line_; }

int WordBrowser::get_thread_id() { return thread_id_; }

int WordBrowser::get_ocurrences_number() { return ocurrences_.size(); }

/* Print information from a Word */
void WordBrowser::print_occurrence(WordFinded wf)
{
    std::cout <<  color_ << "\t[Thread " << thread_id_ + 1 << " Start: " << start_line_ << " - end: " << end_line_ << "]" << RESET;
    std::cout << " :: " << "line " << wf.line << " :: " << "... " << wf.anteccessor << " " 
    << color_ << wf.word << RESET << " " << wf.successor << " ..." << std::endl;
}

/* Print all information from WordBrowser */
void WordBrowser::print()
{
    while(!ocurrences_.empty())
    {
        print_occurrence(ocurrences_.front());
        ocurrences_.pop_front();
    }  
}