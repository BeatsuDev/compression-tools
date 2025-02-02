#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 7) {
        printf("Usage: %s <input_file> <output_file> [--start [[00h]00m]00s] [--end [[00h]00m]00s]\n", argv[0]);
        return 1;
    }

    char *in_file = argv[1];
    char *out_file = argv[2];
    char *start_time = NULL;
    char *end_time = NULL;

    // Parse optional arguments
    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "--start") == 0 && i + 1 < argc) {
            start_time = argv[++i];
        } else if (strcmp(argv[i], "--end") == 0 && i + 1 < argc) {
            end_time = argv[++i];
        } else {
            printf("Invalid argument: %s\n", argv[i]);
            return 1;
        }
    }

    // Construct the ffmpeg command
    char command[512];
    snprintf(command, sizeof(command), "ffmpeg -i \"%s\"", in_file);

    if (start_time) {
        snprintf(command + strlen(command), sizeof(command) - strlen(command), " -ss %s", start_time);
    }
    if (end_time) {
        snprintf(command + strlen(command), sizeof(command) - strlen(command), " -to %s", end_time);
    }

    snprintf(command + strlen(command), sizeof(command) - strlen(command),
             " -c:v libx264 -crf 26 -vf \"scale=-1:720\" -f mp4 \"%s\"", out_file);

    // Execute the command
    int result = system(command);
    if (result != 0) {
        printf("Error executing ffmpeg command\n");
        return 1;
    }

    return 0;
}
