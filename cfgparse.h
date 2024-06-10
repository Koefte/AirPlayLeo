#ifndef cfgparse
#define cfgparse

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// the maximum length of a line.
#define CFG_MAX_LINE_LENGTH 256

// the maximum amount of config items.
#define CFG_MAX_CONFIG_ITEMS 100

// ungrouped items' group name
#define CFG_UNGROUPED ""

// Library to read config files
// Syntax:
//      #comment
//      key=value
// (c) 2024 LeoPlay


// An item consists of: 
//  - the group name
//  - the key
//  - the value
typedef struct {
    char* group;
    char* key;
    char* value;
} ConfigItem;

// Parse a config file.
//
// Includes groups.
// If no group has been defined yet, the item's group will be an empty string.
int cfg_parsefile(const char* filename, ConfigItem* configArray, int maxEntries) {
    FILE* file = fopen(filename, "r");

    if(!file) {
        perror("Failed to open file");
        return -1;
    }

    char line[CFG_MAX_LINE_LENGTH];
    int count = 0;
    char* currentGroup = NULL;

    while (fgets(line, sizeof(line), file) && count < CFG_MAX_CONFIG_ITEMS) {
        if(line[0] == '#' || line[0] == '\n') {
            // Ignore comments and empty lines
            continue;
        }

        if(line[0] == '[') {
            // Group def start
            char* endBracketPos = strchr(line, ']');
            currentGroup = strndup(line + 1, endBracketPos - line - 1);

            continue;
        }

        // Property definition

        char* equalsPos = strchr(line, '=');
        if(!equalsPos) {
            continue;
        }

        configArray[count].key = strndup(line, equalsPos - line);
        configArray[count].value = strndup(equalsPos + 1, strlen(equalsPos + 1));

        if(currentGroup == NULL) {
            configArray[count].group = CFG_UNGROUPED;
        } else {
            configArray[count].group = currentGroup;
        }

        size_t len = strlen(configArray[count].value);
        if(len > 0 && configArray[count].value[len - 1] == '\n') {
            configArray[count].value[len - 1] = '\0';
        }

        count++;
    }

    fclose(file);
    return count;
}

// Gets a value from configArray.
const char* cfg_get(const char* key, const char* group, ConfigItem* configArray, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(configArray[i].key, key) == 0 
                && strcmp(configArray[i].group, group) == 0) {
            return configArray[i].value;
        }
    }
    return NULL;
}

// frees memory used by configArray
void cfg_free(ConfigItem* configArray, int count) {
    for (int i = 0; i < count; i++) {
        free(configArray[i].key);
        free(configArray[i].value);
    }
}

#endif
