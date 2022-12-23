#include <iostream>
#include <algorithm>

#include <boost/thread.hpp>
#include <boost/range.hpp>
#include <boost/bind/bind.hpp>

#include "file_search.h"
#include "tsqueue.h"

namespace bf = boost::filesystem;

static void worker(const std::vector<bf::path> &range, size_t start, size_t end, std::string target);

std::mutex cout_mutex;

std::vector<bf::path> get_files(std::string rootname) {
    bf::path root{rootname};
    std::vector<bf::path> filenames;

    for (auto const &it : boost::make_iterator_range(bf::recursive_directory_iterator{root}, {})) {
        if (bf::is_regular_file(it.path())) {
            filenames.push_back(it.path());
        }
    }

    return filenames;
}

int file_search(const std::vector<bf::path> &range, size_t threads_nom, std::string target) {
    boost::thread_group thread;

    auto count = range.size() / threads_nom;
    if (count == 0) {
        count = 1;
    }

    for (size_t i = 0; i * count < range.size(); ++i) {
        thread.create_thread(boost::bind(&worker, range, i * count,
                                         std::min((i + 1) * count,
                                         range.size()), target));
    }

    thread.join_all();

    return 0;
}

void worker(const std::vector<bf::path> &range, size_t start, size_t end, std::string target) {
    for (size_t i = start; i < end; ++i) {
        if (range[i].filename().generic_string() == target) {
            cout_mutex.lock();
            std::cout << bf::absolute(range[i]) << '\n';
            cout_mutex.unlock();
        }
    }

    cout_mutex.lock();
    std::cout << "thread : " << boost::this_thread::get_id() << " finished\n";
    cout_mutex.unlock();
}