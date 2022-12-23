#pragma once

#include <string>
#include <vector>

#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

int file_search(const boost::filesystem::path &root, const std::string target, size_t thread_nom);

std::vector<boost::filesystem::path> get_files(std::string rootname);