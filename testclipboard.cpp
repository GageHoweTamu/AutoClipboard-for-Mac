
////


#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

std::string getClipboardContent() {
    FILE* pipe = popen("pbpaste", "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL) {
            result += buffer;
        }
    }
    pclose(pipe);
    return result;
}

void captureSelectionAndCopyToFile(const std::string& content) {
    // Open the file in append mode
    std::ofstream outputFile("clipboard_data.txt", std::ios::app);
    if (outputFile.is_open()) {
        // Append the selection to the end of the file
        outputFile << content << "\n\n\n";
        outputFile.close();
        std::cout << "Selection appended to clipboard_data.txt\n";
    } else {
        std::cerr << "Error opening file\n";
    }
}

int main() {
    std::cout << "Listening for changes in the clipboard...\n";

    std::string lastClipboardContent;

    while (true) {
        std::string clipboardContent = getClipboardContent();

        if (clipboardContent != lastClipboardContent) {
            captureSelectionAndCopyToFile(clipboardContent);
            lastClipboardContent = clipboardContent;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
