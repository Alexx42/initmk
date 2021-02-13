#ifndef _FILES_H
# define _FILES_H

#include <string>
#include <vector>

#include "../inc/options.h"

class Files {
public:
	Files() {;}
	~Files() {;}

	const std::vector<std::string> get_sources() const {
		return sources_;
	}

	const std::vector<std::string> get_includes() const {
		return includes_;
	}

	void set_sources(std::vector<std::string>& sources) {
		sources_ = sources;
	}

	void get_all_sources_files(const std::string& path) {
		std::string p;
		for (auto& entry : std::filesystem::directory_iterator(path)) {
			if (entry.is_directory() && is_sources_directory(entry.path().string().substr(path.size() + 1))) {
				for (auto& files : std::filesystem::directory_iterator(entry.path().string())) {
					if (!files.is_regular_file()) {
						continue ;
					}
					p = files.path().string();
					if (type_file(p) == Options::C) {
						sources_.push_back(p);
					} else if (type_file(p) == Options::CXX) {
						sources_.push_back(p);
					}
				}
				get_all_sources_files(entry.path().string());
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

	std::vector<std::string> sources_;
	std::vector<std::string> includes_;
};

#endif