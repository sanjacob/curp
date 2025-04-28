# curp

La primera y única librería de C para la validación y extracción de datos
de la CURP (Clave Única de Registro de Población) Mexicana.

Esta librería esta optimizada para **velocidad** y **eficiencia**, es
perfecta para aplicaciones de datos masivos, o plataformas que cuentan
con recursos de procesamiento y memoria limitados.

- Valida datos de millones de usuarios en segundos
- Minería de datos como la edad y entidad federativa de nacimiento
- Integración en desarrollo móvil, IoT, y wearables
- Ahorra en costos y tiempo de procesamiento en la nube

Algunos casos de uso específicos:

- Lector portátil de credencial de elector.
- Validación de CURP integrada en aplicación móvil.
- Restringir registro a personas por fecha de nacimiento.
- Analizar distribución de usuarios por entidad federativa.
- Sanitización de base de datos con millones de filas.


## Validación

La librería hace una validación completa de la CURP y sus componentes,
cubriendo casos específicos que muchas otras librerías no cubren, sin
sacrificar rendimiento.

- Formato alfabético/numérico de los carácteres
- Vocal del primer apellido y consonantes
- Palabras altisonantes (NXCO, MXCO, etc.)
- Sexo y entidad federativa de nacimiento
- Fecha de nacimiento (meses con 30/31 días, años bisiestos)

<!--Además, si cuentas con el nombre de la persona:

- Comprueba si la CURP es correcta para un nombre o apellido
- Separa un nombre completo en nombre y apellidos usando la CURP-->


| Librería           | Formato  | Altisonantes | Años bisiestos | Extracción |
|--------------------|----------|--------------|----------------|------------|
| **`curp.h`**       | ✅       | ✅           | ✅             | ✅         |
| `CURPSuite.py`     | ✅       | ✅           | ✅             | ✅         |
| `stdnum.py`        | ❌       | ✅           | ✅             | ➖         |
| `mexa.py`          | ❌       | ✅           | ✅             | ✅         |
| `ripper2hl/curp.js`| ✅       | ❌           | ❌             | ❌         |


## Ejemplo

```c
#include <stdio.h>
#include <curp.h>

int main(int argc, char* argv[])
{
    struct curp p;
    int r;

    /* Es necesario inicializar la librería */
    if (!curp_init()) {
        printf("no se pudo inicializar curp.h");
        return 1;
    }

    /* Validación simple de la CURP */
    r = curp_validar("SABC560626MDFLRN09", NULL);

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

La función de validación de la CURP no sólo comprueba si la CURP es correcta
o no, también informa de la razón por la cuál la CURP no es válida.

Es necesario utilizar la función de ayuda `curp_error(int)` para
obtener la causa del error.

```c
int r, motivo;

r = curp_validar("SABC560626MDFLRN09", NULL);
motivo = curp_error(r);
```

Además, es posible obtener la posición en la cuál se encontró el error
por medio del macro `CURP_ERROR_POSICION(int)`.

```c
int r, pos;

r = curp_validar("SABC560626MDFLRN09", NULL);
pos = CURP_ERROR_POSICION(r);
```

Los errores se listan debajo de acuerdo a su prioridad.
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

Si encuentras un error o vulnerabilidad en la librería, por favor
abre un *issue* en GitHub, en donde describas el problema en detalle,
y será examinado lo antes posible.

Si deseas proponer una idea o mejora para la librería, de
igual forma es necesario abrir un *issue*.
*Pull Requests* son bienvenidas para problemas pequeños,
o mejoras en *tests*, pero es recomendable preguntar primero.


## Licencia

Esta librería se proporciona bajo la licencia
[Lesser General Public License v2.1](LICENSE).

También es posible obtener una **licencia comercial** que
te permite integrar la librería directamente de forma estática,
y hacer modificaciones a la misma, sin la obligación de compartir
tu código fuente con el usuario.

Otros beneficios incluyen:

- Soporte técnico para instalación y uso de la librería
- Versiones hechas a la medida para tus requisitos específicos
- Soporte para integrar la librería en Java, C#, C++, PHP, y más

La licencia comercial tiene un costo único de $489 MXN.
Para adquirirla, escribe a jacob@san.contact

<!-- LINKS -->
[mesonbuild]: https://mesonbuild.com
[cmake]: https://cmake.org
[make]: https://linux.die.net/man/1/make
