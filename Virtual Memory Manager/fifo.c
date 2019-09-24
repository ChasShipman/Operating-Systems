#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 256
#define NUM_PAGES 256
#define FRAME_SIZE 256
#define NUM_FRAMES 256
#define TABLE_SIZE 16

typedef struct pageTable{ //struct for page table
    int *pages;
    int size;
    int faults;
} PT;

typedef struct translationLookAsideBuffer{ //struct for tranlationLookAsideBuffer
    int *pages;
    int *frames;
    int size;
    int hits;
} TLB;

TLB *tlb; //translation lookaside buffer
PT *pt; //page table
signed char **frameList; //keep track of frames
int count = 0;//used for keeping track of frames

int getFrames(int pageNumber); //get frame number using page number
void insertTLB(int pageNumber, int frameNumber); //insert into TLB
int getFrameList(); //get frame index in frame list
int convertToPageTable(int pageNumber); //swap from backing store
void newTLB(); //make a new tlb
void newPageTable(); //make a new page table
void displayData(int total); //print the stats
void readFile(char *fileName); //read the address file

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Please provide addresses file.\n");
        exit(1);
    }

    frameList = malloc(sizeof(signed char*) * NUM_FRAMES); //allocate free frame list
    for(int i = 0; i < NUM_FRAMES; i++)
        frameList[i] = 0;
    newTLB();
    newPageTable();
    readFile(argv[1]); //read the file. wraps many of the funtions above
    return 0;
}


int getFrames(int pageNumber) {//mine works
    for (int i = TABLE_SIZE-1; i > -1; i--) {
        if (tlb->pages[i] == pageNumber){//check if page is already in the tlb means tlb hit!!
            tlb->hits++;
            return tlb->frames[i];
        }
    }
    //else not in tlb return invalid
    return -1;
}

void insertTLB(int pageNumber, int frameNumber) {//mine working
    for (int i = 0; i < TABLE_SIZE; i++) {
        tlb->pages[i] = tlb->pages[i + 1];//since fifo replaces "head" of tlb
        tlb->frames[i] = tlb->frames[i + 1]; //set each index to the next index
    }
    
    tlb->pages[TABLE_SIZE - 1] = pageNumber; //set last page number
    tlb->frames[TABLE_SIZE - 1] = frameNumber; //set last frame number
}

int getFrameList() { //helps keep track of free frames
    int frameIndex = count;
    
    if (frameList[frameIndex] != 0) {
        free(frameList[frameIndex]);//free the frame if invalid
    }
 
    count++;
    if (count == NUM_FRAMES) {
        count = 0;//reset count
    }
    return frameIndex;
}

int convertToPageTable(int pageNumber) {
    if (pt->pages[pageNumber] != -1) { //check if in page table aready
        return pt->pages[pageNumber]; //if so return
    }
    //else not found in pageTable = pageFault!
    //need to find in BackingStore!!
    
    signed char *dataStore = malloc(sizeof(signed char) * FRAME_SIZE); //will store data in backingStore
    FILE *BS = fopen("BACKING_STORE.bin", "rb"); //open backingstore
    if (fseek(BS, pageNumber * FRAME_SIZE, SEEK_SET) != 0) {//seek backingstore
        fprintf(stderr, "seeking error.\n");
    }
    
    if (fread(dataStore, sizeof(signed char), FRAME_SIZE, BS) == 0) {//read backingstore
        fprintf(stderr, "reading error.\n");
    }
    
    int frameIndex = getFrameList(); //get index from frame list
    pt->pages[pageNumber] = frameIndex; //set pageNumber
    frameList[frameIndex] = dataStore; //set framelist index from backingstore
    
    fclose(BS);
    pt->faults++;
    return frameIndex;
}

void newTLB() {
    tlb = malloc(sizeof(TLB)); //allocate tlb
    tlb->size = 0;
    tlb->hits = 0;
    tlb->pages = malloc(sizeof(int) * TABLE_SIZE); //allocate pages
    tlb->frames = malloc(sizeof(int) * TABLE_SIZE); //allocate frames
    for (int i = 0; i < TABLE_SIZE; i++) {
        tlb->pages[i] = 0; //pages unused
        tlb->frames[i] = -1; //frame invalidated
    }
}

void newPageTable() {
    pt = malloc(sizeof(pt)); //allocate pagetable
    pt->size = 0;
    pt->faults = 0;
    pt->pages = malloc(sizeof(int) * NUM_PAGES); //allocate pages
    for (int i = 0; i < NUM_PAGES ; i++) {
        pt->pages[i] = -1; //invalidate pages
    }
}

void displayData(int total) {
    printf("Number of Translated Addresses = %d\n", total);
    printf("Page Faults = %d\n", pt->faults);
    printf("Page Fault Rate = %.3f\n", (double) pt->faults / total);
    printf("TLB Hits = %d\n", tlb->hits);
    printf("TLB Hit Rate = %.3f\n", (double) tlb->hits / total);
}

void readFile(char *fileName) {//start reading
    FILE *addrTXT = fopen(fileName, "r"); //open address file
    
    if (addrTXT == NULL) {
        fprintf(stderr, "error with address file.\n");
        exit(1);
    }
    
    int logAddr; // logical address holder
    int total = 0;
    fscanf(addrTXT, "%d", &logAddr); //read in logical address
    while (!feof(addrTXT)) {
        int pageNumber = (logAddr & 0xFFFF) >> 8; //mask pageNumber
        int offset = (logAddr & 0xFF); // mask offset
        
        int frame = getFrames(pageNumber);// get frame number
        
        if (frame < 0) {
            frame = convertToPageTable(pageNumber); //read from backingstore if needed
            //printf("pnum: %d\noffset: %d\n", pageNumber, offset);
            insertTLB(pageNumber, frame);//insert into tlb
        }
        
        printf("Virtual address: %d ", logAddr);
        printf("Physical address: %d ", (frame << 8) | offset); //translate physical address
        printf("Value: %d\n", frameList[frame][offset]);
        
        total++;
        fscanf(addrTXT, "%d", &logAddr);
    }
    
    fclose(addrTXT);
    displayData(total);//print stats
}
