#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <cstring>

class record {
   private :
      char * name;
      int phone;
      int group;

   public :
      record(const char* in = 0, int ip = 0 , int ig = 0);
      record(const record &i);
      ~record();
      char * get_name()  const {return name;}
      int get_phone() const {return phone;}
      int get_group() const {return group;}
      record& operator=(const record& rhs);
      int init(const char* in, int ip, int ig);
      void print();
	   int read(std::ifstream& fp);
	   void set_record(record * );

	   friend class command;
};

int read_buf(int , char *);
int write_buf(int , char *);
int read_record(int , char * , int , int);
int write_record(int , char * , int , int);

