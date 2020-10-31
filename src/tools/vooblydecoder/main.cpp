#include <cstdio>
#include <filesystem>
#include <vector>
#include <cstdint>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <filename.slp>\n", argv[0]);
        return 1;
    }
    if (!std::filesystem::exists(argv[1])) {
        printf("Usage: %s <filename.slp>\n", argv[0]);
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("Failed to open filename");
        return 1;
    }
    ssize_t size = lseek(fd, 0, SEEK_END) - 4; // 4 bytes header
    if (size <= 0) {
        perror("Failed to get size or too small");
        close(fd);
        return 1;
    }

    static const uint8_t vooblyHeader[4] = { 0xBE, 0xEF, 0x13, 0x37 };
    uint8_t header[4];
    read(fd, header, 4);
    if (memcmp(header, vooblyHeader, 4) != 0) {
        puts("Not a voobly encoded SLP");
        close(fd);
        return 1;
    }

    std::vector<uint8_t> buffer(size);
    if (read(fd, buffer.data(), buffer.size()) != size) {
        perror("Failed to read entire file");
        close(fd);
        return 1;
    }
    close(fd);

    for (uint8_t &c : buffer) {
        c = ' ' * ((c - 0x11) ^ '#') | (uint8_t((c - 0x11) ^ '#') >> 3);
    }
    const std::string outFile = std::filesystem::path(argv[1]).stem().string() + "-decoded.slp";
    fd = open(outFile.c_str(), O_WRONLY);
    if (fd < 0) {
        perror("Failed to open filename");
        return 1;
    }

    if (write(fd, buffer.data(), buffer.size()) != size) {
        perror("Failed to write entire file");
        close(fd);
        return 1;
    }
    close(fd);

    puts("Done.");

    return 0;
}

