//#define WIN32_LEAN_AND_MEAN

void check_for_assert(int X, char *File, unsigned int Line);

#ifdef _DEBUG
  #define assert2(X) check_for_assert((int)(X), __FILE__, __LINE__)
#else
  #define assert2(X)
#endif

