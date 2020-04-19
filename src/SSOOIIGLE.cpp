/* C Libraries */
#include <iostream>
#include <fstream>
#include <mutex>
#include <vector>
#include <thread>
#include <string>
#include <functional>
#include <stdlib.h>
#include <string.h>

/* My libraries */
#include <definitions.h>
#include <WordBrowser.h>

void parse_argv(int argc, char *argv[], std::string &word, std::string &file_path);
void print_title();
std::string open_file(std::string file);
int count_lines(std::string file_path);
void calculate_lines(int lines, int &thread_lines, int &rest_lines);
void assign_generate_threads(int thread_lines, int rest_lines, std::string word, std::string file_path);
void launch_threads(int thread_id, int start_line, int end_line, std::string word, std::string file_path, int &c);
void search(int thread_id, std::string word, std::string file_path);
void search_word(int line, int thread_id, std::string word, std::vector<std::string> line_words);
void add_occurrences_number(int thread_id);
bool equals(std::string word_compare, std::string word);
void parse_word(std::string &word_compare, std::string &word);
void get_word_info(int position, std::vector<std::string> line_st, std::string &anteccessor, std::string &successor);
void wait_threads();
void print_result();

/* Global variables */
int threads_number_;                      // Thread's total number
std::vector<std::thread> threads_vector_; // Threads vector
unsigned int ocurrences_number_ = 0;      // Occurrences total number
std::mutex sem;                    // Declaration of semaphores used
std::vector<WordBrowser> wb_vector_;      // Instances WordBrowser vector

int main(int argc, char *argv[])
{
    /* Variables declaration */
    int lines, thread_lines, rest_lines;
    std::string word, file_path;

    parse_argv(argc, argv, word, file_path);

    print_title();

    /* Assign total lines */
    lines = count_lines(file_path);

    calculate_lines(lines, thread_lines, rest_lines);

    assign_generate_threads(thread_lines, rest_lines, word, file_path);

    wait_threads();

    print_result();
}

/* Parse arguments from command line */
void parse_argv(int argc, char *argv[], std::string &word, std::string &file_path)
{
    if (argc != ARGC_NUMBER)
    {
        fprintf(stderr, "[X] Error in command line argument || ./ssooiigle <file> <word_> <thread_number>\n");
        exit(EXIT_FAILURE);
    }

    /* Asigment arguments to global variables*/
    file_path = open_file(argv[1]);
    word = argv[2];
    threads_number_ = atoi(argv[3]);
}

/*Open file and return path*/
std::string open_file(std::string file)
{
    std::ifstream fs;

    /*Generate the full file_path_*/
    file = FILES_PATH + file;
    fs.open(file, std::ios::binary);

    if (!fs.is_open())
    {
        std::cout << "[X] Error opening file '" << file << "'" << std::endl;
        exit(EXIT_FAILURE);
    }

    fs.close();
    return file;
}

/*Count file's total lines*/
int count_lines(std::string file_path)
{
    int lines;
    std::ifstream file_r;
    std::string line = "";

    file_r.open(file_path, std::ios::binary);

    for (lines = 0; !file_r.eof(); lines++)
    {
        std::getline(file_r, line);
    }

    file_r.close();
    return lines;
}

/*Calculate lines per thread and assigment them*/
void calculate_lines(int lines, int &thread_lines, int &rest_lines)
{
    thread_lines = lines / threads_number_;
    rest_lines = lines % threads_number_;
}

/* Assign lines number to threads and generate them */
void assign_generate_threads(int thread_lines, int rest_lines, std::string word, std::string file_path)
{
    int start_line, end_line;

    for (int i = 0, c = 0; i < threads_number_; i++, c++)
    {
        start_line = (thread_lines * i + 1);
        /*The first thread has rang: [1 -- (thread_lines-1) + rest_lines] */
        (i == threads_number_ - 1) ? end_line = (start_line + thread_lines - 1) + rest_lines : end_line = (start_line + thread_lines - 1);

        launch_threads(i, start_line, end_line, word, file_path, c);
    }
}

/* Generate threads */
void launch_threads(int thread_id, int start_line, int end_line, std::string word, std::string file_path, int &c)
{
    std::string colors[COLORS_NUMBER] = {GREEN, RED, BLUE, YELLOW, MAGENTA, CYAN};

    /* Check if there are more threads than colors*/
    thread_id > COLORS_NUMBER - 1 ? c = 0 : c;

    wb_vector_.push_back(WordBrowser(thread_id, start_line, end_line, colors[c]));
    threads_vector_.push_back(std::thread(search, thread_id, word, file_path));
}

/* Main function for search words*/
void search(int thread_id, std::string word, std::string file_path)
{
    std::string line;
    std::fstream fs;

    /*Separate string in tokens*/
    std::vector<std::string> line_words;

    int start_line, end_line;
    
    sem.lock();
    start_line = wb_vector_[thread_id].get_start_line();
    end_line = wb_vector_[thread_id].get_end_line();
    sem.unlock();

    fs.open(file_path, std::ios::in);

    for (int i = 1; getline(fs, line); i++)
    {
        /* Thread position */
        if (i >= start_line && i <= end_line)
        {
            line_words = string_tokenizer(line);
            search_word(i, thread_id, word, line_words);
        }
        line_words.clear();
    }

    /* Add ocurrences to 'ocurrences_number_' */
    add_occurrences_number(thread_id);
}

/* Search word in a line */
void search_word(int line, int thread_id, std::string word, std::vector<std::string> line_words)
{
    std::string anteccessor, successor;

    for (unsigned int pos = 0; pos < line_words.size(); pos++)
    {
        if (equals(line_words[pos], word))
        {
            get_word_info(pos, line_words, anteccessor, successor);

            sem.lock();
            wb_vector_[thread_id].add_ocurrence(line, line_words[pos], anteccessor, successor);
            sem.unlock();
        }
    }
}

/* Get word's information (line, anteccessor, successor) */
void get_word_info(int position, std::vector<std::string> line_st, std::string &anteccessor, std::string &successor)
{
    const int LAST_POSITION = line_st.size() - 1;
    /* Initialize default value*/
    anteccessor = line_st[position - 1];
    successor = line_st[position + 1];

    if (position == FIRST_POSITION)
        anteccessor = "";
    else if (position == LAST_POSITION)
        successor = "";
}

/* Comparate word with word found */
bool equals(std::string word_compare, std::string word)
{
    bool equals = true;
    parse_word(word_compare, word);

    if (word.size() == word_compare.size())
    {
        for (unsigned int i = 0; i < word.size() && equals; i++)
        {
            if (word[i] != word_compare[i])
                equals = false;
        }
    }
    else
        equals = false;

    return equals;
}

/* Increment ocurrences number */
void add_occurrences_number(int thread_id)
{
    std::lock_guard<std::mutex> guard(sem);
    {
        ocurrences_number_ += wb_vector_[thread_id].get_ocurrences_number();
    }
}

/*Get alphabetic characters*/
void parse_word(std::string &word_compare, std::string &word)
{
    /* If word to compare is upper, this convert to lower */
    upper_to_lower(word_compare);
    upper_to_lower(word);

    for (unsigned int i = 0; i < word_compare.size(); i++)
    {
        if (word_compare.at(i) < 97 || word_compare.at(i) > 122)
        {
            word_compare.erase(i);
        }
    }
}

/* Wait the threads finalize */
void wait_threads()
{
    std::for_each(threads_vector_.begin(), threads_vector_.end(), std::mem_fn(&std::thread::join));
}

/*Print title (SSOOIIGLE) */
void print_title()
{
    std::cout << "-------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "\t\t\t\t********  " << BOLDBLUE << "SS" << BOLDRED "O" << BOLDYELLOW << "O" << BOLDBLUE "II" << BOLDGREEN << "GL"
              << BOLDRED "E" << RESET "  ********\n"
              << std::endl;
}

/* Print search result */
void print_result()
{
    std::cout << "[X] Number of " << BOLDGREEN << "threads" << RESET << " launched: " << BOLDGREEN << threads_number_ << RESET << std::endl;
    std::cout << "[X] Number of " << BOLDRED << "occurrencess" << RESET << " founds: " << BOLDRED << ocurrences_number_ << RESET << "\n"
              << std::endl;
    for (unsigned int i = 0; i < wb_vector_.size(); i++)
    {
        wb_vector_[i].print();
    }

    std::cout << "-------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "\t\t\t\t********  " << BOLDGREEN "PROGRAM FINALIZED" << RESET << "  ********\n"
              << std::endl;
}
