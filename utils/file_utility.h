//
// Created by baizeyv on 6/27/2025.
//

#ifndef FILE_UTILITY_H
#define FILE_UTILITY_H
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class file_utility {
public:
    static void check_file_and_create_dir_when_needed(const std::string &p_path);

    static bool safe_delete_file(const std::string &p_path);

    static bool safe_copy_file(const std::string &from_file, const std::string &to_file);

    static std::string safe_read_all_text(const std::string &p_path);

    static bool safe_clear_dir(const std::string &folderPath,
                               const std::vector<std::string> &exclude_suffix_names = {});

    static bool safe_copy_directory(
        const std::string &sourceDirName,
        const std::string &destDirName,
        bool copySubDirs,
        const std::vector<std::string> &exclude_suffix_names = {});

    static bool safe_delete_dir(const std::string& folderPath, const std::vector<std::string>& exclude_suffix_names = {});

    static bool safe_write_all_text(const std::string& outFile, const std::string& text);

    static std::string truncate_path(const std::string& fullPath, int levels);

    static void check_or_create_dir(const std::string& folderPath);

    static std::uintmax_t get_file_size(const std::string& filePath);

private:
    static bool ends_with_any(const fs::path &file_path, const std::vector<std::string> &exclude_suffix_names);

    static void delete_directory(const fs::path &dir_path, const std::vector<std::string> &exclude_suffix_names = {});
};


#endif //FILE_UTILITY_H
