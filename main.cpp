#include <iostream>
#include <string>

#include <boost/filesystem.hpp>

#include "file_search.h"

namespace bf = boost::filesystem;

constexpr size_t TARGET = 1;
constexpr size_t PATH = 3;
constexpr size_t N_THREAD = 5;
constexpr size_t ARG_SIZE = 6;


int main(int argc, char *const argv[]) {
    if (static_cast<size_t>(argc) < ARG_SIZE) {
        std::cout << "required " << ARG_SIZE << " args\n"; 
        return -1;
    }

    const std::string target = argv[TARGET];
    const std::string pathname = argv[PATH];
    bf::path path(pathname);

    size_t thread_nom = static_cast<size_t>(atoi(argv[N_THREAD])); 

    const size_t threads_avaliable = boost::thread::hardware_concurrency();

    if (thread_nom > threads_avaliable) {
        thread_nom = threads_avaliable;
        std::cout << "there are " << thread_nom << " threads required\n"
                  << "there are " << threads_avaliable << " threads used\n"; 
    }    

    auto filenames = get_files(pathname);
    file_search(filenames, thread_nom, target);
    
    return 0;
}