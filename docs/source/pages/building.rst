Building From Source
====================

Dependencies
------------

**Required:**

- g++-multilib
- cmake

**Optional:**
none

Build Steps
-----------

In the build folder, execute the following commands:

.. code-block:: bash

    cmake ..
    make

This will create a release build. The file ``t1x.iso`` will be generated in the ``t1x-server/bin`` directory.

CMake Optional Arguments
~~~~~~~~~~~~~~~~~~~~~~~~

- **DEBUG_BUILD:** Include debug information (no optimization).

Example
^^^^^^^

.. code-block:: bash

    cd build
    cmake .. -DDEBUG_BUILD=ON
    make

This will create a debug build.