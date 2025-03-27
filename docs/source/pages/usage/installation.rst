Installation
============

Dependencies
------------

**Required:**

- libstdc++6:i386

**Optional:**

none

Tip
~~~

To install i386 packages, run:

.. code-block:: bash

    dpkg --add-architecture i386
    apt update


Installing
------------

- Place ``t1x.so`` in the CoD root directory.
- Inject ``t1x.so`` into ``coduo_lnxded`` when starting the server. Like:

.. code-block:: bash
    
    LD_PRELOAD="./t1x.so" coduo_lnxded +exec myserver.cfg

Refer to step 5 of the `cod.pm <https://cod.pm>`_ server setup `guide <https://cod.pm/guide/a7a40b/call-of-duty-1-server-on-linux-installing-and-configuring>`_ for details.
