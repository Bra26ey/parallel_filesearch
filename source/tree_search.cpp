#include <iostream>
#include <queue>

#include <boost/thread.hpp>
#include <boost/range.hpp>
#include <boost/bind/bind.hpp>

#include "tree_search.h"

namespace bf = boost::filesystem;

PathTree::PathTree(bf::path root) {
    m_root = new PathNode(root.filename().generic_string(), "");
    m_dir = root.parent_path().generic_string();

    std::queue<PathNode*> paths;
    paths.push(m_root);
    while (!paths.empty()) {
        auto current = paths.front();
        auto current_name = current->absolute();
        paths.pop();

        for (auto const &it : boost::make_iterator_range(bf::directory_iterator{m_dir + current_name}, {})) {
            std::string child_name = it.path().filename().generic_string();

            PathNode *node = new PathNode(child_name, current_name);
            current->childs.push_back(node);
            // std::cout << current->absolute() << '\n';

            if (bf::is_directory(it.path())) {
                paths.push(node);
            }
        }
    }
}

PathTree::~PathTree() {
    delete m_root;
}

void PathTree::search(const std::string &target, size_t thread_nom) {
    m_context.post(boost::bind(&PathTree::worker_search, this, target, m_root));

    for (size_t i = 0; i < thread_nom; ++i) {
        m_threads.create_thread(boost::bind(&boost::asio::io_context::run, &m_context));
    }

    m_threads.join_all();
}

void PathTree::worker_search(const std::string target, const PathNode *const node) {
    for (auto const &it : node->childs) {
        if (!it->childs.empty()) {
            m_context.post(boost::bind(&PathTree::worker_search, this, target, it));
            continue;
        }

        if (it->name == target) {
            std::lock_guard lock(m_coutmutex);
            std::cout << "thread " << boost::this_thread::get_id() << " found file:\n"
                      << m_dir << "/" << it->absolute() << "\n";
        }
    }
}

PathNode::~PathNode() {
    for (auto &it : childs) {
        delete it;
    }
};

std::string PathNode::absolute() const {
    return path + "/" + name;
}