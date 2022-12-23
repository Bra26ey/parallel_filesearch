#pragma once

#include <string>
#include <vector>

#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

// int file_search(const boost::filesystem::path &root, const std::string target, size_t thread_nom);

int file_search(const std::vector<boost::filesystem::path> &range, size_t threads_nom, std::string target);

std::vector<boost::filesystem::path> get_files(std::string rootname);