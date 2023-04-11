char getbyte(const char* stream, int streamlen, int byteindex);

void mainline(double* dataPtr, int datasize);

//py::array Vec2NpArray(std::vector<T> *data, std::vector<size_t> shape)

//PyObject* retarray(double* dataPtr, int datasize, const char* stream, int streamlen) 

void combine(double* dataPtr, int datasize, const char* stream, int streamlen, int byteindex);