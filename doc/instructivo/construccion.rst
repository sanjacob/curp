Construcción de la CURP
=======================

Normas generales
----------------

.. hint::

   El primer y segundo apellido tradicionalmente corresponden
   al apellido paterno y materno respectivamente.
   Sin embargo, el orden es decidido por los padres.


Posición 1-4
^^^^^^^^^^^^

1. La primera letra del primer apellido.
2. La primera vocal interna del primer apellido.
3. La primera letra del segundo apellido.
4. La primera letra del nombre.

.. note::

   La vocal interna debe ser distinta a la primera letra.

**Ejemplo: Concepción Salgado Briseño -> SABC**


Posición 5-10
^^^^^^^^^^^^^

La fecha de nacimiento en formato YYMMdd.


**Ejemplo: 1 de Diciembre de 1995 -> 951201**


Posición 11
^^^^^^^^^^^

Sexo de la persona: M para mujer, H para hombre.

**Ejemplo: Mujer -> M**


Posición 12-13
^^^^^^^^^^^^^^

Clave de la entidad federativa de nacimiento,
de acuerdo a esta tabla.

.. note::
   :class: alert-warning

   A los Mexicanos nacidos en el extranjero se
   les asigna la clave `NE`.


.. table::
   :class: table table-bordered

   +-------------------------+-------+
   | Entidad                 | Clave |
   +=========================+=======+
   | Aguascalientes          |  AS   |
   +-------------------------+-------+
   | Baja California         |  BC   |
   +-------------------------+-------+
   | Baja California Sur     |  BS   |
   +-------------------------+-------+
   | Campeche                |  CC   |
   +-------------------------+-------+
   | Coahuila                |  CL   |
   +-------------------------+-------+
   | Colima                  |  CM   |
   +-------------------------+-------+
   | Chiapas                 |  CS   |
   +-------------------------+-------+
   | Chihuahua               |  CH   |
   +-------------------------+-------+
   | Distrito Federal (CDMX) |  DF   |
   +-------------------------+-------+
   | Durango                 |  DG   |
   +-------------------------+-------+
   | Guanajuato              |  GT   |
   +-------------------------+-------+
   | Guerrero                |  GR   |
   +-------------------------+-------+
   | Hidalgo                 |  HG   |
   +-------------------------+-------+
   | Jalisco                 |  JC   |
   +-------------------------+-------+
   | Estado de México        |  MC   |
   +-------------------------+-------+
   | Michoacán               |  MN   |
   +-------------------------+-------+
   | Morelos                 |  MS   |
   +-------------------------+-------+
   | Nayarit                 |  NT   |
   +-------------------------+-------+
   | Nuevo León              |  NL   |
   +-------------------------+-------+
   | Oaxaca                  |  OC   |
   +-------------------------+-------+
   | Puebla                  |  PL   |
   +-------------------------+-------+
   | Querétaro               |  QT   |
   +-------------------------+-------+
   | Quintana Roo            |  QR   |
   +-------------------------+-------+
   | San Luis Potosí         |  SP   |
   +-------------------------+-------+
   | Sinaloa                 |  SL   |
   +-------------------------+-------+
   | Sonora                  |  SR   |
   +-------------------------+-------+
   | Tabasco                 |  TC   |
   +-------------------------+-------+
   | Tamaulipas              |  TS   |
   +-------------------------+-------+
   | Tlaxcala                |  TL   |
   +-------------------------+-------+
   | Veracruz                |  VZ   |
   +-------------------------+-------+
   | Yucatán                 |  YN   |
   +-------------------------+-------+
   | Zacatecas               |  ZS   |
   +-------------------------+-------+
   | Nacido en el Extranjero |  NE   |
   +-------------------------+-------+


Posición 14-16
^^^^^^^^^^^^^^

14. Primera consonante interna del primer apellido.
15. Primera consonante interna del segundo apellido.
16. Primera consonante interna del nombre.

.. note::
   :class: alert alert-warning

   La consonante interna debe ser distinta a la primera letra.


Posición 17
^^^^^^^^^^^

Diferenciador de homonimia, caracter asignado por la
Secretaría de Gobernación para evitar duplicados.

- 0-9 para fechas de nacimiento antes del 2000
- A-Z para fechas de nacimiento a partir del 2000


Posición 18
^^^^^^^^^^^

Dígito verificador que permite verificar la correcta
conformación y transcripción de la CURP.

.. seealso::
   Algoritmo :ref:`verificacion`
