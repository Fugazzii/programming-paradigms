include $(GL_LIB_PATH)/makefile_base

tests: tests.c ../packed_list.h ../ip_list.h
	$(CC) $(CFLAGS) -c tests.c -o $(OUTDIR)/$@.o

run_tests: $(OUTDIR)/packed_list.o $(OUTDIR)/ip_list.o tests
	OUT_DIR=$(OUTDIR) make -C $(GL_LIB_PATH)/lunit lunit
	$(LD) $(LDFLAGS) $(OUTDIR)/tests.o $(OUTDIR)/packed_list.o $(OUTDIR)/ip_list.o $(OUTDIR)/lunit.o $(OUTDIR)/cmp.o $(OUTDIR)/list.o -o $(OUTDIR)/$@
