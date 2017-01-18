# Minimal makefile for Sphinx documentation
#

# You can set these variables from the command line.
SPHINXOPTS    =
SPHINXBUILD   = sphinx-build
SPHINXPROJ    = vcnc
SOURCEDIR     = .
BUILDDIR      = _build

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

#
#  The documentation is published on GitHub Pages by placing the
#  generated files into the /docs directory of the 'master'
#  branch of 'vcnc'.
#
#  This target places the files in the directory.  The git operations
#  are done manually.  See the documentation.
#
publish:
	make html
	mkdir -p ../docs
	rsync -a --delete _build/html/ ../docs

.PHONY: help Makefile


# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
