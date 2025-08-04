# curp

La primera librería en C para la validación y extracción de datos de la
CURP (Clave Única de Registro de Población) Mexicana.


## Validación

La librería hace una validación completa de la CURP y sus partes, cubriendo
casos específicos que muchas otras librerías no cubren.

- Formato alfabético/numérico de los carácteres
- Vocal del primer apellido y consonantes
- Palabras altisonantes (NXCO, MXCO, etc.)
- Sexo y entidad federativa de nacimiento
- Fecha de nacimiento (meses con 30/31 días, años bisiestos)

<!--Además, si cuentas con el nombre de la persona:

- Comprueba si la CURP es correcta para un nombre o apellido
- Separa un nombre completo en nombre y apellidos usando la CURP-->


### Ejemplo

```c
#include <stdio.h>
#include <curp.h>

int main(int argc, char* argv[])
{
    /* Es necesario inicializar la librería */
    if (!curp_init()) {
        printf("no se pudo inicializar curp.h");
        return 1;
    }

    /* Validación simple de la CURP */
    if (curp_validar("SABC560626MDFLRN09", NULL) == CURP_VALIDA) {
        printf("la curp es correcta");
    }

    /* Liberar la memoria al terminar */
    curp_exit();
}
```


## Extracción

También es posible extraer las propiedades de la CURP, como:

- Fecha de nacimiento
- Sexo
- Entidad federativa de nacimiento


### Ejemplo

```c
#include <stdio.h>
#include <curp.h>

int main(int argc, char* argv[])
{
    struct curp p;

    /* Es necesario inicializar la librería */
    if (!curp_init()) {
        printf("no se pudo inicializar curp.h");
        return 1;
    }

    /* Valida y extrae propiedades de una CURP */
    if (curp_validar("SABC560626MDFLRN09", &p) == CURP_VALIDA) {
        /* sexo (1: hombre, 2: mujer, 0: x) */
        printf("sexo: %s", p.sexo == HOMBRE ? "Hombre" : "Mujer");

        /* fecha de nacimiento */
        printf("nacimiento: %d-%02d-%02d",
               p.anio_nacimiento,
               p.mes_nacimiento,
               p.dia_nacimiento);

        /* entidad federativa de nacimiento */
        printf("entidad: %s", curp_entidad_nombre(p.entidad));
        printf("entidad iso: %s", curp_entidad_iso(p.entidad));
    }

    /* Liberar la memoria al terminar */
    curp_exit();
}
```


## Errores

Con el propósito de entender los errores de validación, ésta librería hace
posible conocer tanto el motivo como la posición del error en la CURP.

Para obtener la causa del error, se puede utilizar la función de ayuda
`curp_error(int)`.

```c
int r, motivo;

r = curp_validar("SABC560626MDFLRN09", NULL);
motivo = curp_error(r);
```

Los motivos de error se listan debajo de acuerdo a su prioridad.
En general, los errores que ocurran primero en la CURP toman prioridad sobre
errores que ocurran después.

| Nombre            | Descripción                                    |
|-------------------|------------------------------------------------|
| ERROR_FORMATO     | No corresponde a un formato AAAANNNNNNAAAAAAXN |
| ERROR_ALTISONANTE | Contiene una palabra altisonante               |
| ERROR_FECHA       | La fecha de nacimiento no es válida            |
| ERROR_SEXO        | El sexo no es válido                           |
| ERROR_ENTIDAD     | La entidad de nacimiento no es válida          |
| ERROR_VERIFICADOR | El dígito verificador no es correcto           |
| **CURP_VALIDA**   | La CURP tiene un formato válido                |

La posición del error se puede obtener con el macro
`CURP_ERROR_POSICION(int)`.

```c
int r, pos;

r = curp_validar("SABC560626MDFLRN09", NULL);
pos = CURP_ERROR_POSICION(r);
```


## Compilación

Clona el repositorio
```bash
$ git clone https://github.com/sanjacob/curp.git
```
Compila usando una de estas opciones:

[make][make]
```bash
$ make
$ make check
```

[Meson][mesonbuild]
```bash
$ meson setup build && cd build
$ meson compile
$ meson test
```

[CMake][cmake]
```bash
$ cmake -B build
$ cmake --build build
$ cd build && ctest
```


## Contribuciones

Son bienvenidas.


## Licencia

Esta librería se proporciona bajo la licencia
[Lesser General Public License v2.1](LICENSE).


<!-- LINKS -->
[mesonbuild]: https://mesonbuild.com
[cmake]: https://cmake.org
[make]: https://linux.die.net/man/1/make
