#pragma once

#include <vector>
#include <mutex>

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/asio/io_context.hpp>

class PathNode;

class PathTree {
 public:
    PathTree(boost::filesystem::path root);
    PathTree(PathTree &other) = delete;
    ~PathTree();
    void search(const std::string &target, size_t thread_nom);
 
 private:
    void worker_search(const std::string target, const PathNode *const node);
    
 private:
    PathNode *m_root;
    std::string m_dir;

    boost::thread_group m_threads;
    boost::asio::io_context m_context;

    std::mutex m_coutmutex;
};

class PathNode {
 public:
    PathNode() = default;
    PathNode(const std::string &name, const std::string &path) : name(name), path(path) {};
    ~PathNode();
    std::string absolute() const;
 
 public:
    std::vector<PathNode*> childs;
    std::string path;
    std::string name;
};