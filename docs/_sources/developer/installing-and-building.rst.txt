Installing and Building
=======================

The vCNC software is built using header files and libraries provided by a
PeerCache customer release bundle, and tested using a lightweight PeerCache
deployment running on the developer's laptop or workstation.

The installation process involves:

* Obtaining the vCNC sources and PeerCache release bundle.
* Installing PeerCache on the developer's machine.
* Configuring the development environment.
* Building the software.
* Running a sanity check.

Obtaining the vCNC sources
--------------------------

Fork the `GitHub project`_ to your GitHub account and then clone it to your
development machine.

.. _GitHub project: https://github.com/nicko7i/vcnc

Installing PeerCache
--------------------

Please refer to the `PeerCache installation document`_ maintained on the
IC Manage Google Drive account.

.. _PeerCache installation document: https://docs.google.com/document/d/1ZiepQCDps2hb8Qi7k9BGE5yPtBrc6hfG7TXoUVFt5Tw/edit?usp=sharing

In the context of building vCNC, the PeerCache software is called the *TOOLROOT*.

Configuring the Development Environment
---------------------------------------

vCNC is a collection of sub-projects knit together by a top-level autotools
build system. Begin by installing autotools, GNU make and GNU C++ v4.8.

Python
''''''

Python is needed for the documentation generator *Sphinx* and for vCNC software
written in Python.  Ensure you are using the Python 3.5 bundled in *TOOLROOT*.

.. code-block:: console

    % sudo pip install --upgrade pip
    % sudo pip install virtualenv
    % virtualenv -p /opt/frqu/TOOLROOT/bin/python3 venv
    % source ./venv/bin/activate
    % (venv) pip install sphinx sphinx-autobuild sphinx_rtd_theme

node.js
'''''''

The build system uses the nvm bundled with *TOOLROOT*. Place the following in
your *.bashrc* file:

.. code-block:: bash

    export NVM_DIR="/opt/frqu/TOOLROOT/nvm"
    [ -s "$NVM_DIR/nvm.sh" ] && . "$NVM_DIR/nvm.sh"  # This loads nvm
    [[ -r $NVM_DIR/bash_completion ]] && . $NVM_DIR/bash_completion

Building the Software
----------------------

The vCNC repository houses a suite of independent projects.  There
is no top-level, "grand unified" build.  Each project is built separately.

Building vnc-rest
'''''''''''''''''

Configure, build and install is done in the familiar way.
The build directory must be called 'Build'
because node_gyp is hardwired to find its dependencies there.

.. code-block:: console

  % autoreconf
  % mkdir Build
  % cd Build
  % ../configure --with-pic --with-toolroot=/opt/frqu/TOOLROOT
  % make 

Building vcnc-web
'''''''''''''''''

Building this documentation
''''''''''''''''''''''''''''
The build must be done under the Python virtual environment.

.. code-block:: console

  % cd *<project-dir>*
  % . ./venv/bin/activate
  (venv) cd documentation
  (venv) make html

Sphinx will generate a static HTML website in the documentation/_build
directory.  It may be viewed at
file:///*<project-dir>*/documentation/_build/html/index.html

Publishing this documentation
'''''''''''''''''''''''''''''

The documentation is published on GitHub Pages by placing the
generated files into the /docs directory of the 'master'
branch of 'vcnc'.

After pushing the latest documentation sources to origin/master, build
the documentation as described above and then publish as follows:

.. code-block:: console

  % cd *<project-dir>*
  % git checkout master
  % git pull
  % cd documentation
  % make publish
  % git add ../docs
  % git commit -m "publish documentation"
  % git push

The updated, generated, documentation is now in master/docs on GitHub.
From there, it automagically appears on GitHub pages.
