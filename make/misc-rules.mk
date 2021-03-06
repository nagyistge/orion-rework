$(BUILDDIR)/param/orion3.o: $(BUILDDIR)/container/array.o $(BUILDDIR)/io/util/util.o
$(BUILDDIR)/container/array.o: $(LIBDIR)/container/array_impl.h $(LIBDIR)/container/array_impl.c
$(BUILDDIR)/param/param.o: $(BUILDDIR)/container/array.o
$(BUILDDIR)/io/format/mhd.o: $(BUILDDIR)/io/path/path.o $(BUILDDIR)/util/string.o \
	$(CONFIG_HEADER_FILE)

$(ITK_INTEGRATION_LIB_OBJ): $(BUILDDIR)/integration/itk/libIntegrationITK.a
.NOTPARALLEL: $(BUILDDIR)/integration/itk/libIntegrationITK.a
$(BUILDDIR)/integration/itk/libIntegrationITK.a: \
		$(LIBDIR)/integration/itk/itk.cxx \
		$(FILTER_OBJ) \
		$(BUILDDIR)/ndarray/array_ndarray3.o \
		$(BUILDDIR)/ndarray/ndarray3.o
	@$(MKDIR_BUILD)
	$(CMAKE.generate) -B$(BUILDDIR)/integration/itk -H$(LIBDIR)/integration/itk
	$(MAKE) -C$(BUILDDIR)/integration/itk
