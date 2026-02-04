#pragma once // "Diese Datei nur einmal einbinden"
#include <string>
#include <filesystem>
#include "ThreadPool.h"

namespace fs = std::filesystem;

void scanDirectory(const fs::path& path, ThreadPool& pool);