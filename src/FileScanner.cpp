#include FileScanner.h

#include <filesystem> //Für Ordner/Datein
#include <fstream> //Datein lesem
#include <iostream> //Ausgabe

namespace fs = std::filesystem;

std::vector<std::string> FileScanner::search(const std::string& rootPath, const std::string& word) {
    
    std::vector<std::string> results;
    scan(rootPath, word, results); //hier this-> ausprobieren
    return results;
}

void FileScanner::scan(const std::string& path, const std::string& word, std::vector<std::string>& results) {
    //Diese for schleife durchläuft jeden eintrag in path und übergibt diesen dann in entry welcher dann bis zum ende der for schleife bearbeitet wird
    for (const auto& entry : fs::directory_iterator(path)) { //directory_iterator läuft über alle einträge in "path"
        if (entry.is_directory()) {
            //Rekursion wenn entry noch ein Ordner ist (weil wenn scan einen unterordner findet, muss dieser ja ebenfalls gescannt werden -> also nochmal scan() aufrufen)
            scan(entry.path.string(), word, results);

        } else if (entry.is_regular_file()) {
            // ifstream = input file stream -> wird benutzt um Datein zu lesen (Text/Binär)
            // ofstream = output file stream -> Datei schreiben
            // entry.path() gibt Dateipfad von entry zurück (z.B. "/home/christopher22/file-indexer/test.txt")
            // Konstruktor erwartet nämlich std::filesystem::path und entry.path() ist genau das
            std::ifstream file(entry.path()); // "Mach mir einen Lesekanal zu genau diesem Pfad/dieser Datei"
            // wenn Datei nicht geöffnet werden konnte (z.B. keine Rechte, kaputt) dann continue
            if (!file.is_open()) continue;

            std::string line;
            //Datei Zeile für Zeile lesen: Hol dir (getline) eine Zeile von file und speicher die in "line"
            while (std::getline(file, line)) {
                // wenn word nicht gefunden worden ist dann gibt line.find(word) std::string::npos (No position) zurück, deshalb nur wenn es != ist wird der Pfad in dem das Wort gefunden wurde (als String) an Ende des vectors results gepushed
                if (line.find(word) != std::string::npos) {
                    results.push_back(entry.path().string());
                    // Sonst würde Datei weiter durchsucht werden
                    break;
                }
            }
        }
    }
}
