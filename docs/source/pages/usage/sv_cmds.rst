Server Commands
===============

Ban Client IP Adress
---------------------

``ipban``

ipban Arguments
~~~~~~~~~~~~~~~

- **ip** or **client number**
- **reason**
- **duration** (format: <magnitude><unit> without space)
    **Units:**

    - h for hours
    
    - d for days

ipban Examples
~~~~~~~~~~~~~~

.. code-block:: bash

    ipban -n 2 -r aimbot -d 1d

.. code-block:: bash

    ipban -i 12.345.67.8 -r aimbot -d 3h

Unban Client IP Adress
--------------------------------

``ipunban``

ipunban Arguments
~~~~~~~~~~~~~~~~~

- **ip**

ipunban Examples
~~~~~~~~~~~~~~~~

.. code-block:: bash

    ipunban -i 12.345.67.8
