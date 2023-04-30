/*
 * Class to simulate a page table.
 * 
 * Completion Time: 5 hours
 * @author Edgar Hakobyan
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "../headers/PageTable.h"
//#include "PageTable.h"

//forward declarations for structs
struct page_table_entry {
    int frameNum;
    unsigned int valid_invalid_bit;
    struct page_table_entry* next;
    int fifo;
    int mfu;
};

struct page_table {
    struct page_table_entry* entries;
    enum replacement_algorithm algorithm;
    int numFaults;
    int pageCount;
    int frameCount;
    int frames;
    int fifo;
    int lru;
    int mfu;
    int verbose;
};


/**
 * Creates a new page table object. Returns a pointer to created page table.
 *
 * @param page_count Number of pages.
 * @param frame_count Numbers of frames.
 * @param algorithm Page replacement algorithm
 * @param verbose Enables showing verbose table contents.
 * @return A page table object.
 */
struct page_table* page_table_create(int page_count, int frame_count, enum replacement_algorithm algorithm, int verbose) {
    struct page_table* ptr = (struct page_table*)malloc(sizeof(struct page_table));
    ptr->pageCount = page_count;
    ptr->frameCount = frame_count;
    ptr->algorithm = algorithm;
    ptr->verbose = verbose;
    ptr->numFaults = 0;
    ptr->frames = 0;
    ptr->fifo = 0;

    ptr->entries = (struct page_table_entry*)malloc(sizeof(struct page_table_entry) * ptr->pageCount);
    for (int i = 0; i < ptr->pageCount; i++) {
        ptr->entries[i].valid_invalid_bit = 0;
        ptr->entries[i].fifo = 0;
    }
    return ptr;
}

/**
 * Destroys an existing page table object. Sets outside variable to NULL.
 *
 * @param pt A page table object.
 */
void page_table_destroy(struct page_table** pt) {
    free(*pt);
    *pt = NULL;
}

/**
 * Simulates an instruction accessing a particular page in the page table.
 *
 * @param pt A page table object.
 * @param page The page being accessed.
 */
void page_table_access_page(struct page_table *pt, int page) {

    //First available pages
     if (pt->frames < pt->frameCount
         && pt->entries[page].valid_invalid_bit == 0) {
         pt->entries[page].frameNum = pt->frames;
         pt->entries[page].valid_invalid_bit = 1;
         pt->numFaults = pt->numFaults + 1;
         pt->frames = pt->frames + 1;
         pt->entries[page].mfu = pt->entries[page].mfu + 1;
    }

    //FIFO
    else if (pt->algorithm == 0
             && pt->entries[page].valid_invalid_bit == 0) {
         pt->entries[pt->fifo].valid_invalid_bit = 0;
         pt->entries[page].frameNum = pt->fifo;
         pt->entries[page].valid_invalid_bit = 1;
         pt->fifo = pt->fifo + 1;
         pt->numFaults = pt->numFaults + 1;
    }
    
    //LRU
    else if (pt->algorithm == 1) {
        if (pt->entries[page].valid_invalid_bit == 1) {
            pt->lru = pt->lru + 1;
        }
        else if (pt->entries[page].valid_invalid_bit == 0) {
            pt->entries[pt->lru].valid_invalid_bit = 0;
            pt->entries[page].valid_invalid_bit = 1;
            pt->entries[page].frameNum = pt->lru;
            pt->numFaults = pt->numFaults + 1;
            pt->fifo = pt->fifo + 1;
        }
    }
    
    //MFU
    else if (pt->algorithm == 2) {
        if (pt->entries[page].valid_invalid_bit == 1) {
            pt->entries[page].mfu = pt->entries[page].mfu + 1;
        }
        else if(pt->entries[page].valid_invalid_bit == 0) {

            int temp = 0;
            for (int i = 0; i < pt->pageCount; i++) {
                if (pt->entries[i].valid_invalid_bit == 1) {
                    if (temp < pt->entries[i].mfu) {
                        temp = pt->entries[i].mfu;
                        pt->mfu = i;
                    }
                }
            }
            pt->entries[pt->mfu].mfu = pt->entries[page].mfu + 1;
            pt->numFaults = pt->numFaults + 1;
            pt->entries[page].frameNum = pt->mfu;
            pt->entries[pt->mfu].valid_invalid_bit = 0;
            pt->entries[page].valid_invalid_bit = 1;
            pt->fifo = pt->fifo + 1;
        }
    }
}

/**
 * Displays page table replacement algorithm, number of page faults, and the
 * current contents of the page table.
 *
 * @param pt A page table object.
 */
void page_table_display(struct page_table* pt) {
    printf("====Page Table====\n");
    
    if (pt->algorithm == FIFO) {
        printf("Mode: FIFO\n");
    }
    else if (pt->algorithm == LRU) {
        printf("Mode: LRU\n");
    }
    else if (pt->algorithm == MFU) {
        printf("Mode: MFU\n");
    }
    printf("Page Faults: %d\n", pt->numFaults);
    printf("page frame | dirty valid\n");
    
    for (int i = 0; i < pt->pageCount; i++) {
        printf(" %d\t%d\t%d\t%d\n", i, pt->entries[i].frameNum, pt->verbose, pt->entries[i].valid_invalid_bit);
    }
}

/**
 * Displays the current contents of the page table.
 *
 * @param pt A page table object.
 */
void page_table_display_contents(struct page_table *pt) {
    printf("====Page Table Contents====\n");

    printf("page frame | dirty valid\n");

    for (int i = 0; i < pt->pageCount; i++) {
        printf(" %d     %d      %d      %d\n", i, pt->entries[i].frameNum, pt->verbose, pt->entries[i].valid_invalid_bit);
    }
}

