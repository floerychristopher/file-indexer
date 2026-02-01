#include "FileScanner.h"
#include <filesystem> //Für Ordner/Datein
#include <fstream> //Datein lesen
#include <iostream> //Ausgabe
#include <thread>
#include <atomic>

std::atomic<int> threadCounter(0); // globaler Counter

namespace fs = std::filesystem;

std::vector<std::string> FileScanner::search(const std::string& rootPath, const std::string& word) {
    
    std::vector<std::string> results;
    std::mutex mtx;

    ++threadCounter;
    scan(rootPath, word, results, mtx); //hier this-> ausprobieren

    std::cout << "Total threads created: " << threadCounter.load() << "\n";

    return results;
}

void FileScanner::scan(const std::string& path, const std::string& word, std::vector<std::string>& results, std::mutex& mtx) {

    //DEBUG
    std::cout << "Thread (ID:" << std::this_thread::get_id() << ") hat scan() aufgerufen!\n";

    std::vector<std::thread> threads;
    //Diese for schleife durchläuft jeden eintrag in path und übergibt diesen dann in entry welcher dann bis zum ende der for schleife bearbeitet wird
    for (const auto& entry : fs::directory_iterator(path)) { //directory_iterator läuft über alle einträge in "path"

        if (entry.is_directory()) {
            //Rekursion wenn entry noch ein Ordner ist (weil wenn scan einen unterordner findet, muss dieser ja ebenfalls gescannt werden -> also nochmal scan() aufrufen)
            // Neuen thread für subfolder launchen
            if (threads.size() >= MAX_THREADS) {
                // mit .join() auf ersten thread [0] warten bevor neuer gelaunched wird
                threads[0].join();
                // da zu diesem Zeitpunkt threads[0] fertig ist können wir diesen aus dem vector löschen
                threads.erase(threads.begin());
            }
            //scan() wird mit Parametern aufgerufen, welcher in der lambda funktion aufgerufen wurde (scan braucht Funktion um aufgerufen zu werden)
            // neuer Thread wird erstellt:
            threads.emplace_back([this, &word, &results, &mtx, entry]() {
                ++threadCounter;
                scan(entry.path().string(), word, results, mtx);
            });
            //DEBUG
            std::cout << "Thread (ID:" << std::this_thread::get_id() << ") hat neues Dir gefunden und dafür einen neuen Thread erstellt!\n";
        }
        else if (entry.is_regular_file()) {
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
                    std::lock_guard<std::mutex> lock(mtx); //thread-safe write
                    results.push_back(entry.path().string());
                    
                    //DEBUG
                    std::cout << "Thread (ID:" << std::this_thread::get_id() << ") hat Datei " << entry.path().string() << "  gefunden!\n";
                    
                    // Sonst würde Datei weiter durchsucht werden
                    break;
                }
            }
        }
    }
    // Auf alle gelaunchen threads in diesem Folder warten
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}
