struct stringentry {
	string item<1024>;
        stringentry *next;
};

typedef stringentry *stringlist;
