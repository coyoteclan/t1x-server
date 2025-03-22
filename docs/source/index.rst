.. t1x-server documentation master file, created by
   sphinx-quickstart on Sat Mar 22 07:00:03 2025.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.


t1x-server Documentation
=========================

.. contents:: Table of Contents
   :local:

Overview
--------

This document provides an overview of the t1x-server setup, including dependencies, installation instructions, usage tips, and build steps.

Dependencies
------------

To build
~~~~~~~~

**Required:**

- g++-multilib
- cmake

To run
~~~~~~

**Required:**

- libstdc++6:i386

Tip
~~~

To install i386 packages, run:

.. code-block:: bash

    dpkg --add-architecture i386
    apt update

Installation
------------

- Place ``t1x.so`` in the CoD root directory.
- Inject ``t1x.so`` into ``coduo_lnxded`` when starting the server (refer to step 5 of the cod.pm server setup guide).

Usage
-----

Accept Denied CD Keys
~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

    set sv_cracked "1"

Show a Message Before Connection
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

    set sv_connectMessage "my message"

Control the display duration with the ``sv_connectMessageChallenges`` cvar. For example,

.. code-block:: bash

    set sv_connectMessageChallenges "2"

Note
----

For additional details, please refer to the source code and the ``t1x.cfg`` file.


Build Steps
-----------

In the build folder, execute the following commands:

.. code-block:: bash

    cmake ..
    make

The file ``t1x.iso`` will be generated in the ``t1x-server/bin`` directory.

CMake Optional Arguments
~~~~~~~~~~~~~~~~~~~~~~~~

- **DEBUG_BUILD:** Include debug information (no optimization).

Example
-------

.. code-block:: bash

    cd build
    cmake .. -DDEBUG_BUILD=ON
    make

