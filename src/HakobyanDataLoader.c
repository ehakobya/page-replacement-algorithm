/* 
 * Helper functions to load a reference string.
 * 
 * Completion Time: 2 hours
 * @author Edgar Hakobyan
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "../headers/DataLoader.h"
//#include "DataLoader.h"

/**
 * Loads a test_scenario strut from a textfile.
 *
 * @param filename The name of the file to load.
 * @return A struct containing the loaded file.
 */
struct test_scenario* load_test_data(char* filename) {

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file\n");
        exit(1);
    }

    struct test_scenario* pScenario = malloc(sizeof(struct test_scenario));


    int numPages, numFrames, numEntries;
    
    fscanf(file, "%d", &numPages);
    fscanf(file, "%d", &numFrames);
    fscanf(file, "%d", &numEntries);

    pScenario->page_count = numPages;
    pScenario->frame_count = numFrames;
    pScenario->refstr_len = numEntries;

    int entry;
    for (int i = 0; i < numEntries; i++) {
        fscanf(file, "%d", &entry);
        pScenario->refstr[i] = entry;
    }
    
    fclose(file);
    return pScenario;
}

