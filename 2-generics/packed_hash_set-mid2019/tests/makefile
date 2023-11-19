include $(GL_LIB_PATH)/makefile_base
LUNIT=$(GL_LIB_PATH)/lunit/lunit.o

tests: tests.c ../packed_hash_set.h ../tuple_hash_set.h
	$(CC) $(CFLAGS) -c tests.c -o $(OUTDIR)/$@.o

run_tests: $(OUTDIR)/packed_hash_set.o $(OUTDIR)/tuple_hash_set.o tests
	OUT_DIR=$(OUTDIR) make -C $(GL_LIB_PATH)/lunit lunit
	$(LD) $(LDFLAGS) $(OUTDIR)/tests.o $(OUTDIR)/packed_hash_set.o $(OUTDIR)/tuple_hash_set.o $(OUTDIR)/lunit.o $(OUTDIR)/cmp.o $(OUTDIR)/list.o -o $(OUTDIR)/$@
