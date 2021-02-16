#ifndef _FILES_H
# define _FILES_H

#include <string>
#include <vector>

#include "../inc/options.h"

class Files {
public:
	Files() {;}
	~Files() {;}

	const std::map<std::string, StringList> get_sources() const {
		return sources_;
	}

	const std::vector<std::string> get_includes() const {
		return includes_;
	}

	void set_sources(std::map<std::string, StringList>& sources) {
		sources_ = sources;
	}

	void get_all_sources_files(const std::string& initial_path, const std::string& path, bool in_src) {
		std::string directory, src;

		for (auto& entry : std::filesystem::directory_iterator(path)) {
			if (entry.is_directory() && ((is_sources_directory(entry.path().string().substr(path.size() + 1))) || in_src)) {
				directory = entry.path().string().substr(initial_path.size() + 1);
				for (auto& files : std::filesystem::directory_iterator(entry.path().string())) {
					if (!files.is_regular_file()) {
						continue ;
					}
					std::size_t pos = files.path().string().rfind('/');
					if (pos == std::string::npos) {
						throw std::runtime_error("invalid path");
					} try {
						src = files.path().string().substr(pos + 1);
					} catch (std::out_of_range& e) {

					}
					if (type_file(src) == Options::C) {
						sources_[directory].push_back(src);
					} else if (type_file(src) == Options::CXX) {
						sources_[directory].push_back(src);
					}
				}
				get_all_sources_files(initial_path, entry.path().string(), true);
			}
		}
	}
	void get_all_include_directories(const std::string& path) {
		for (auto& entry : std::filesystem::directory_iterator(path)) {
			if (entry.is_directory() && is_include_directory(entry.path().string().substr(path.size() + 1))) {
				includes_.push_back(entry.path().string());
				get_all_include_directories(entry.path().string());
			}
		}
	}

	inline static Options::C_CXX type_file(const std::string& file_name) {
		if (file_name.substr(file_name.size() - 3) == ".c") {
			return Options::C;
		} else if (file_name.substr(file_name.size() - 4) == ".cpp") {
			return Options::CXX;
		}
		return Options::Unknown;
	}

	static bool is_sources_directory(const std::string& s) {
		for (auto str: SRC_DIRECTORY) { if (str == s) { return true; } } return false;
	}
	static bool is_include_directory(const std::string& s) {
		for (auto str: INCLUDE_DIRECTORY) { if (str == s) { return true; } } return false;
	}

private:
	inline static const std::string SRC_DIRECTORY[4] = {"src", "srcs", "sources", "source"};
	inline static const std::string INCLUDE_DIRECTORY[3] = {"inc", "include", "includes"};

	std::map<std::string, StringList> sources_;
	std::vector<std::string> includes_;
};

#endif