CURP
====

La primera librería de C para la construcción, validación,
y análisis de la Clave Única de Registro de Población.


.. warning::
   :class: alert-warning

   La librería esta en etapa experimental.


.. warning::
   :class: alert-warning

   Esta página esta en construcción.


Validación
----------

Si necesitas validar la CURP de forma eficiente, con la ventaja de
conocer la causa del error y la posición del mismo en la CURP.

.. code-block:: c

        #include <stdio.h>
        #include <curp.h>

        int main() {
            int r;

            /* Se debe inicializar primero */
            curp_init();

            r = curp_validar("SABC560626MDFLRN01", NULL);

            if (r != CURP_VALIDA) {
                int err = curp_error(r);
                int pos = CURP_ERROR_POSICION(r);

                printf("Error %d en caracter %d", err, pos);
            }

            /* Libera memoria al terminar */
            curp_exit();
        }


Análisis
--------

Si deseas extraer los datos de la CURP, como fecha de nacimiento, sexo,
y entidad federativa de nacimiento.

.. code-block:: c

        #include <stdio.h>
        #include <curp.h>

        int main() {
            struct curp p;

            /* Se debe inicializar primero */
            curp_init();

            if (curp_validar("SABC560626MDFLRN01", &p) >= 0) {
                printf("Fecha: %d/%d", p.dia_nacimiento, p.mes_nacimiento);
                printf("Sexo: %s", p.sexo == HOMBRE ? "hombre" : "mujer");
                printf("Entidad: %s", curp_entidad_nombre(p.entidad_nacimiento));
            }

            /* Libera memoria al terminar */
            curp_exit();
        }


Construcción
------------

Para crear una CURP con los datos de una persona.


.. code-block:: c

        #include <stdio.h>
        #include <curp.h>

        int main() {
            struct curp p;
            char *curp;

            curp = malloc(CURP_LENGTH + 1);
            curp[CURP_LENGTH] = '\0';

            p.anio_nacimiento = 1956;
            p.mes_nacimiento = 06;
            p.dia_nacimiento = 26;
            p.sexo = MUJER;
            p.entidad_nacimiento = CIUDAD_DE_MEXICO;

            /* Se debe inicializar primero */
            curp_init();
            
            curp_crear(curp, &p, "CONCEPCION", "SALGADO", "BRISENO");
            printf("%s", curp);

            /* Libera memoria al terminar */
            curp_exit();
        }


----

Librería para C
---------------

.. toctree::
   :maxdepth: 2

   instalacion


Acerca de la CURP
-----------------

.. toctree::
   :maxdepth: 3
   
   instructivo/verificacion
   instructivo/construccion
