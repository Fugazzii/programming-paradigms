include $(GL_LIB_PATH)/makefile_base

packed_hash_set: packed_hash_set.h packed_hash_set.c
	$(CC) $(CFLAGS) -c packed_hash_set.c -o $(OUTDIR)/$@.o

tuple_hash_set: tuple_hash_set.h tuple_hash_set.c packed_hash_set.h
	$(CC) $(CFLAGS) -c tuple_hash_set.c -o $(OUTDIR)/$@.o
