#include <WordBrowser.h>
#include <string>
#include <vector>
#include <iostream>
#include <mutex>

#define RESET "\033[0m"     /* Default color */


/* Class constructor */
WordBrowser::WordBrowser(int thread_id, int start_line, int end_line, std::string color)
{
    this->thread_id_ = thread_id;
    this->start_line_ = start_line;
    this->end_line_ = end_line;
    this->color_ = color;
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
        this->ocurrences_.push_back(*wf);
    }
}

int WordBrowser::get_start_line() { return this->start_line_; }

int WordBrowser::get_end_line() { return this->end_line_; }

int WordBrowser::get_thread_id() { return this->thread_id_; }

int WordBrowser::get_ocurrences_number() { return this->ocurrences_.size(); }

/* Print information from a Word */
void WordBrowser::print_occurrence(WordFinded wf)
{
    std::cout << this-> color_ << "\t[Thread " << this->thread_id_ + 1 << " Start: " << this->start_line_ << " - end: " << this->end_line_ << "]" << RESET;
    std::cout << " :: " << "line " << wf.line << " :: " << "... " << wf.anteccessor << " " 
    << this->color_ << wf.word << RESET << " " << wf.successor << " ..." << std::endl;
}

/* Print all information from WordBrowser */
void WordBrowser::print()
{
    while(!this->ocurrences_.empty())
    {
        print_occurrence(this->ocurrences_.front());
        this->ocurrences_.pop_front();
    }  
}