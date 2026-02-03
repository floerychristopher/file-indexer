#pragma once // "Diese Datei nur einmal einbinden"
#include <string>
#include "ThreadPool.h"

void scanDirectory(const std::string& path, ThreadPool& pool);