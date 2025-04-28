Instalacion
===========


Clona el repositorio

.. code-block:: bash

    $ git clone https://github.com/sanjacob/curp.git


Compilacion
-----------

Compila usando una de estas opciones:

**make**

.. code-block:: bash

    $ make
    $ make check

**Meson**

.. code-block:: bash

    $ meson setup build && cd build
    $ meson compile
    $ meson test

**CMake**

.. code-block:: bash

    $ cmake -B build
    $ cmake --build build
    $ cd build && ctest


