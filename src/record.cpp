#include "record.h"

int record :: init(const char* in, int ip, int ig) {
	phone = ip;
	group = ig;
	if (in) {
		name = new char[std::strlen(in) + 1];
		if (name) { 
			std::strcpy(name, in);
		}
	}
	else {
		name = 0;
	}
	return 0;
}

record :: record(const char* in, int ip, int ig) {
	init(in,ip,ig);
}

record :: record(const record& i) {
	init(i.name,i.phone, i.group);
}

record :: ~record() {
	if (name) {
		delete [] name;
		name = 0;
	}
}

record& record :: operator=(const record& rhs) {
	this -> record :: ~record();
	init (rhs.name, rhs.phone, rhs.group);
	return *this;
}

void record :: print() {
	if (this) {//fflush(stdin);
	   printf("%s %d %d\n", this -> get_name(), this -> get_phone(), this -> get_group());}
	else printf("Empty\n");
}

int record :: read(std::ifstream& fp) {
	char buf[128] ;
	int ph = 0;
	int gr = 0;

	if (fp >> buf >> ph >> gr) {
		this -> init(buf, ph, gr);
		return 1;
	} else {
		return 0;
	}
}


void record :: set_record(record * a)
{
    strcpy(name , a -> get_name());
    phone = a -> get_phone();
    group = a -> get_group();
}
