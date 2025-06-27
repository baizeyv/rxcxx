//
// Created by baizeyv on 6/27/2025.
//

#include "file_utility.h"

#include <iostream>
#include <fstream>

void file_utility::check_file_and_create_dir_when_needed(const std::string &p_path) {
    if (p_path.empty())
        return;
    if (const fs::path p(p_path);
        p.has_parent_path()) {
        if (const auto parent_dir = p.parent_path();
            !parent_dir.empty() && !fs::exists(parent_dir)) {
            fs::create_directory(parent_dir);
        }
    }
}

bool file_utility::safe_delete_file(const std::string &p_path) {
    try {
        if (p_path.empty())
            return true;
        const fs::path p(p_path);
        if (!fs::exists(p))
            return true;
        fs::remove(p);
        return true;
    } catch (const std::exception &ex) {
        // TODO: LOG
        std::cout << "safe delete file failed! path = " << p_path << " with error: " << ex.what() << std::endl;
        return false;
    }
}

bool file_utility::safe_copy_file(const std::string &from_file, const std::string &to_file) {
    try {
        if (from_file.empty())
            return false;
        const fs::path source(from_file);
        if (!fs::exists(source))
            return false;
        check_file_and_create_dir_when_needed(to_file);
        if (!safe_delete_file(to_file))
            return false;
        fs::copy_file(source, to_file, fs::copy_options::overwrite_existing);
        return true;
    } catch (const std::exception &ex) {
        // TODO: LOG
        std::cout << "safe copy file failed! from file = " << from_file << ", to file = " << to_file <<
                ", with error = " << ex.what() << std::endl;
        return false;
    }
}

std::string file_utility::safe_read_all_text(const std::string &p_path) {
    try {
        if (p_path.empty() || !fs::exists(p_path))
            return std::string();
        std::ifstream ifs(p_path, std::ios::in | std::ios::binary);
        if (!ifs.is_open()) {
            // TODO: LOG
            std::cerr << "safe read all text failed to open file: " << p_path << std::endl;
            return std::string();
        }
        std::ostringstream oss;
        oss << ifs.rdbuf();
        return oss.str();
    } catch (const std::exception &ex) {
        // TODO: LOG
        std::cerr << "safe read all text failed: " << p_path << ", error: " << ex.what() << std::endl;
        return std::string();
    }
}

bool file_utility::safe_clear_dir(const std::string &folderPath, const std::vector<std::string> &exclude_suffix_names) {
    try {
        if (folderPath.empty())
            return true;

        const fs::path dir(folderPath);

        if (fs::exists(dir) && fs::is_directory(dir)) {
            delete_directory(dir, exclude_suffix_names);
        }

        // 确保目录存在
        if (!fs::exists(dir)) {
            fs::create_directories(dir);
        }

        return true;
    } catch (const std::exception &ex) {
        std::cerr << "safe clear dir failed! path = " << folderPath << " with err = " << ex.what() << std::endl;
        return false;
    }
}

bool file_utility::safe_copy_directory(const std::string &sourceDirName, const std::string &destDirName,
                                       bool copySubDirs, const std::vector<std::string> &exclude_suffix_names) {
    try {
        const fs::path source(sourceDirName);
        const fs::path dest(destDirName);

        if (!fs::exists(source) || !fs::is_directory(source)) {
            throw std::runtime_error("Source directory does not exist: " + sourceDirName);
        }

        // 创建目标目录（如果不存在）
        if (!fs::exists(dest)) {
            fs::create_directories(dest);
        }

        // 遍历当前目录下的文件
        for (const auto &entry: fs::directory_iterator(source)) {
            const fs::path &path = entry.path();
            fs::path targetPath = dest / path.filename();

            if (entry.is_regular_file()) {
                if (!ends_with_any(path.filename().string(), exclude_suffix_names)) {
                    fs::copy_file(path, targetPath, fs::copy_options::overwrite_existing);
                }
            }
        }

        // 如果需要递归复制子目录
        if (copySubDirs) {
            for (const auto &entry: fs::directory_iterator(source)) {
                if (entry.is_directory()) {
                    fs::path subSource = entry.path();
                    if (fs::path subDest = dest / subSource.filename(); !safe_copy_directory(
                        subSource.string(), subDest.string(), true, exclude_suffix_names)) {
                        return false; // 递归中失败也会中止
                    }
                }
            }
        }

        return true;
    } catch (const std::exception &ex) {
        // TODO: LOG
        std::cerr << "safe copy directory failed! source = " << sourceDirName
                << ", dest = " << destDirName
                << ", error = " << ex.what() << std::endl;
        return false;
    }
}

bool file_utility::safe_delete_dir(const std::string &folderPath,
                                   const std::vector<std::string> &exclude_suffix_names) {
    try {
        if (folderPath.empty())
            return true;

        if (const fs::path dir(folderPath); fs::exists(dir) && fs::is_directory(dir)) {
            delete_directory(dir, exclude_suffix_names);
        }

        return true;
    } catch (const std::exception &ex) {
        // TODO: LOG
        std::cerr << "safe delete dir failed! path = " << folderPath
                << " with err: " << ex.what() << std::endl;
        return false;
    }
}

bool file_utility::safe_write_all_text(const std::string &outFile, const std::string &text) {
    try {
        if (outFile.empty())
            return false;

        check_file_and_create_dir_when_needed(outFile);

        std::ofstream ofs(outFile, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!ofs.is_open()) {
            // TODO: LOG
            std::cerr << "safe write all text failed! Cannot open file: " << outFile << std::endl;
            return false;
        }

        ofs << text;
        ofs.close();

        return true;
    } catch (const std::exception &ex) {
        // TODO: LOG
        std::cerr << "safe write all text failed! path = " << outFile << " with err = " << ex.what() << std::endl;
        return false;
    }
}

std::string file_utility::truncate_path(const std::string &fullPath, const int levels) {
    fs::path p(fullPath);

    for (int i = 0; i < levels; ++i) {
        p = p.parent_path();
        if (p.empty()) {
            break;
        }
    }

    return p.u8string();
}

void file_utility::check_or_create_dir(const std::string &folderPath) {
    if (folderPath.empty())
        return;

    if (const fs::path dir(folderPath); !fs::exists(dir)) {
        fs::create_directories(dir);
    }
}

std::uintmax_t file_utility::get_file_size(const std::string &filePath) {
    const fs::path path(filePath);
    if (!fs::exists(path) || !fs::is_regular_file(path)) {
        return 0;
    }

    return fs::file_size(path);
}

bool file_utility::ends_with_any(const fs::path &file_path, const std::vector<std::string> &exclude_suffix_names) {
    const std::string filename = file_path.filename().u8string();
    for (const auto &suffix: exclude_suffix_names) {
        if (filename.size() >= suffix.size() && filename.compare(filename.size() - suffix.size(), suffix.size(), suffix)
            == 0) {
            return true;
        }
    }
    return false;
}

void file_utility::delete_directory(const fs::path &dir_path, const std::vector<std::string> &exclude_suffix_names) {
    if (!fs::exists(dir_path) || !fs::is_directory(dir_path))
        return;

    try {
        // 删除文件
        for (auto &entry: fs::directory_iterator(dir_path)) {
            if (entry.is_regular_file()) {
                const fs::path &file = entry.path();
                bool deleteFile = true;
                if (!exclude_suffix_names.empty() && ends_with_any(file, exclude_suffix_names)) {
                    deleteFile = false;
                }

                if (!deleteFile)
                    continue;

                fs::remove(file);
            }
        }

        // 递归删除子目录
        for (auto &entry: fs::directory_iterator(dir_path)) {
            if (entry.is_directory()) {
                delete_directory(entry.path(), exclude_suffix_names);
            }
        }

        // 删除空目录
        if (fs::is_empty(dir_path)) {
            fs::remove(dir_path);
        }
    } catch (const std::exception &ex) {
        // TODO: LOG
        std::cerr << "delete directory failed on " << dir_path << ": " << ex.what() << std::endl;
    }
}
