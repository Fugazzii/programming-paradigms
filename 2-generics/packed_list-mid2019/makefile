include $(GL_LIB_PATH)/makefile_base

packed_list: packed_list.h packed_list.c
	$(CC) $(CFLAGS) -c packed_list.c -o $(OUTDIR)/$@.o

ip_list: ip_list.h ip_list.c packed_list.h
	$(CC) $(CFLAGS) -c ip_list.c -o $(OUTDIR)/$@.o
