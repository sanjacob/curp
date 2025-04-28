.. _verificacion:

Dígito Verificador
==================

Dígito verificador, caracter asignado por la Secretaría de
Gobernación a través de la aplicación de un algoritmo que permite
calcular y verificar la correcta conformación y transcripción de la
clave.


Algoritmo
---------

Para validar la integridad de una CURP:

1. Se convierten todos los caracteres alfabéticos a dígitos de acuerdo
   a la siguiente tabla. Los caracteres numéricos se preservan tal cual.

.. table::
   :class: table table-bordered

   === === === === === === === === === === === === === === === === === === === === === === === === === === ===
    A   B   C   D   E   F   G   H   I   J   K   L   M       N   O   P   Q   R   S   T   U   V   W   X   Y   Z 
   === === === === === === === === === === === === === === === === === === === === === === === === === === ===
    0   1   2   3   4   5   6   7   8   9   0   1   2       3   5   6   7   8   9   0   1   2   3   4   5   6
   === === === === === === === === === === === === === === === === === === === === === === === === === === ===


2. Multiplicar cada dígito por su posición (descendiente).
   En otras palabras, el primer dígito se multiplica por 18,
   el segundo por 17, y así sucesivamente.

3. Sumar todos los productos resultantes del paso anterior.

4. La verificación es correcta si el último dígito de la suma es 0.


Ejemplo
-------

Para la CURP `SABC560626MDFLRN0`:

1. Convertimos los caracteres alfabéticos a números de acuerdo a la tabla
   anterior.

   .. table::
      :class: table table-bordered

      === === === === === === === === === === === === === === === === === ===
       S   A   B   C   5   6   0   6   2   6   M   D   F   L   R   N   0   1
      === === === === === === === === === === === === === === === === === ===
       9   0   1   2   5   6   0   6   2   6   2   3   5   1   8   3   0   1
      === === === === === === === === === === === === === === === === === ===


2. Multiplicar cada dígito por su posición (descendiente)

   .. table::
      :class: table table-bordered

      +-----+----+----+----+----+----+----+----+----+----+----+----+----+---+----+---+---+---+
      | 9   | 0  | 1  | 2  | 5  | 6  | 0  | 6  | 2  | 6  | 2  | 3  | 5  | 1 | 8  | 3 | 0 | 1 |
      +=====+====+====+====+====+====+====+====+====+====+====+====+====+===+====+===+===+===+
      |                            .. centered:: :math:`\times`                              |
      +-----+----+----+----+----+----+----+----+----+----+----+----+----+---+----+---+---+---+
      | 18  | 17 | 16 | 15 | 14 | 13 | 12 | 11 | 10 | 9  | 8  | 7  | 6  | 5 | 4  | 3 | 2 | 1 |
      +-----+----+----+----+----+----+----+----+----+----+----+----+----+---+----+---+---+---+
      |                                     .. centered:: =                                  |
      +-----+----+----+----+----+----+----+----+----+----+----+----+----+---+----+---+---+---+
      | 162 | 0  | 16 | 30 | 70 | 78 | 0  | 66 | 20 | 54 | 16 | 21 | 30 | 5 | 32 | 9 | 0 | 1 |
      +-----+----+----+----+----+----+----+----+----+----+----+----+----+---+----+---+---+---+


3. Sumar los productos resultantes

   .. math::
      
      162 + 0 + 16 + 30 + 70 + \dots + 1 = 610
    
4. Como el último dígito es :math:`0`, la verificación es correcta.


Definición Formal
-----------------

1. Definimos una función :math:`f(c)` que convierte números
   en base 36 (:math:`\{0, ..., \text{Z}\}`) a base 10, agregando :math:`1` si es mayor a
   :math:`\text{N}`.


   .. math::

      f(c) = 
      \begin{cases}
       c_{10} & \text{if } c \le \text{N}_{36} \\
       c_{10} + 1 & \text{if } c \gt \text{N}_{36} \\
      \end{cases}


2. Se calcula la suma de verificación


   .. math::

      s = \sum_{i=1}^n f(c_i) \cdot (n-(i-1))


   en donde

   - :math:`c_i` es el caracter en posición :math:`i`.
   - :math:`n` es el número de caracteres.


3. El dígito verificador es correcto si


.. math::


   s\equiv 0 \pmod{10}

