# Minimal makefile for Sphinx documentation
#
# To use this, add the 'python3-sphinx' package in your Linux distro.
# The intiial frmae was created here by sphinq-quickstart.

# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = .
BUILDDIR      = _build

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile
	@which $(SPHINXBUILD) || \
	  (echo "Install XX$(SPHINXBUILD) (package: python3-sphinx)" && exit 1)
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

clean:

distclean: clean
	rm -rf _build

dist: distclean
	dir=`basename $$PWD` && cd .. && tar zcvf $$dir.tar.gz ./$$dir
	dir=`basename $$PWD` && ls -l ../$$dir.tar.gz
