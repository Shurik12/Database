#include <iostream>
#include "command.h"
#include <time.h>

using namespace std;

#define BUF_LEN 1000
int main() {
    //char* file = "b.txt";
	const char* file = "b.txt";
    //const char* file = "a10000000.txt";
	const char* file2 = "commands2.txt";
	//const char* file3 = "output.txt";
	FILE* fp;
	FILE* fp2;
    //double time = 0;
    //double time_cmd = 0;
	list* l = new list();
	int j;
	//int i = 0;
	char buf[BUF_LEN];
	//command * c = new command();
    int res = 0;

	if (!(fp = fopen(file,"r"))) {
	    // printf("Cannot open file\n");
	    cout << "Cannot open file\n";
	    return -1;
	}
	if (!(fp2 = fopen(file2,"r"))) {
	    fclose(fp);
	    // printf("Cannot open file\n");
	    cout << "Cannot open file\n";
	    return -1;
	}

	if (!(l -> read(fp))) {
		fclose(fp);
	    fclose(fp2);
        // printf("Cannot read\n");
        cout << "Cannot read\n";
        return 1;
	}

	printf("Input command : ");

    while (fgets(buf, BUF_LEN, fp2)) {
        for (j=0; buf[j]; j++) {
          if (buf[j] == '\n') {
             buf[j] = 0;
             break;
          }
        }
		printf("%s\n", buf); 

        command * c = new command();
        if ((c -> init(buf)) < 0) {
            printf("Syntax error!\n");
        }
        //time = clock();
        res = c -> apply( l );
        delete c;

        if (res) break;
        //printf("\n");
        printf("Input command : ");
    }

	delete l;

    fclose(fp);
	// scanf("%d", &j);
    return 0;
}
