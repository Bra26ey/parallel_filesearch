#include "file_search.h"

namespace bf = boost::filesystem;

std::vector<bf::path> get_files(std::string rootname) {
    bf::path root{rootname};
    std::vector<bf::path> filenames;

    for (auto const &it : bf::recursive_directory_iterator) {
        if (bf::is_regular_file(it.path())) {
            filenames.push_back(it.path());
        }
    }

    return filenames;
}
