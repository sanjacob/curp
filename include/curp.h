#ifndef CURP_H
#define CURP_H

static const char *const entidades[] = {
	"AS", "BC", "BS", "CC", "CL", "CM", "CS", "CH", "DF", "DG", "GT",
	"GR", "HG", "JC", "MC", "MN", "MS", "NT", "NL", "OC", "PL", "QT",
	"QR", "SP", "SL", "SR", "TC", "TS", "TL", "VZ", "YN", "ZS", "NE", NULL
};

static const char *const entidades_nombre[] = {
	"Aguascalientes", "Baja California", "Baja California Sur",
	"Campeche", "Coahuila", "Colima", "Chiapas",
	"Chihuahua", "Ciudad de México", "Durango", "Guanajuato",
	"Guerrero", "Hidalgo", "Jalisco", "México", "Michoacán",
	"Morelos", "Nayarit", "Nuevo León", "Oaxaca", "Puebla",
	"Querétaro", "Quintana Roo", "San Luis Potosí",
	"Sinaloa", "Sonora", "Tabasco", "Tamaulipas", "Tlaxcala",
	"Veracruz", "Yucatán", "Zacatecas", NULL
};

static const char *const entidades_iso[] = {
	"AGU", "BCN", "BCS", "CAM", "COA", "COL", "CHP", "CHH",
	"CMX", "DUR", "GUA", "GRO", "HID", "JAL", "MEX", "MIC",
	"MOR", "NAY", "NLE", "OAX", "PUE", "QUE", "ROO", "SLP",
	"SIN", "SON", "TAB", "TAM", "TLA", "VER", "YUC", "ZAC", NULL
};

static const char *const altisonantes[] = {
	"BACA", "BAKA", "BUEI", "BUEY", "CACA", "CACO", "CAGA", "CAGO",
	"CAKA", "CAKO", "COGE", "COGI", "COJA", "COJE", "COJI", "COJO",
	"COLA", "CULO", "FALO", "FETO", "GETA", "GUEI", "GUEY", "JETA",
	"JOTO", "KACA", "KACO", "KAGA", "KAGO", "KAKA", "KAKO", "KOGE",
	"KOGI", "KOJA", "KOJE", "KOJI", "KOJO", "KOLA", "KULO", "LILO",
	"LOCA", "LOCO", "LOKA", "LOKO", "MAME", "MAMO", "MEAR", "MIAR",
	"MEAS", "MEON", "MION", "MOCO", "MOKO", "MULA", "MULO", "NACA",
	"NACO", "PEDA", "PEDO", "PENE", "PIPI", "PITO", "PUTO", "POPO",
	"PUTA", "QULO", "RATA", "ROBA", "ROBE", "ROBO", "RUIN", "SENO",
	"TETA", "VACA", "VAGA", "VAGO", "VAKA", "VUEI", "VUEY", "WUEI",
	"WUEY", NULL
};

static const char *const preposiciones[] = {
	"DA", "DAS", "DE", "DEL", "DER", "DI", "DIE", "DD",
	"EL", "LA", "LOS", "LAS", "LE", "LES", "MAC", "MC",
	"VAN", "VON", "Y", NULL
};

/* 
	static char *nombres_ignorados = {
		"MARIA", "MA", "MA.", "JOSE", "J", "J."
	}
*/

enum curp_err {
	CURP_VALIDA = 1,
	ERROR_VOCAL = -1,
	ERROR_ALTISONANTE = -3,
	ERROR_FECHA = -4,
	ERROR_SEXO = -10,
	ERROR_ENTIDAD = -11,
	ERROR_CONSONANTE = -13,
	ERROR_VERIFICADOR = -18,
	ERROR_FORMATO = -20,
};

struct curp {
	unsigned int sexo;
	unsigned int dia_nacimiento;
	unsigned int mes_nacimiento;
	unsigned int anio_nacimiento;
	unsigned int entidad_nacimiento;
};

#define CURP_ERROR_POSICION(code) (-code & 0x1F)

/* inicializa la libreria */
int curp_init(void);

/* libera la memoria dinamica */
void curp_exit(void);

/* valida una CURP mexicana, y extrae sus datos */
int curp_validar(const char *s, struct curp *p);

/* extrae datos de una CURP prevalidada */
int curp_cargar(const char *s, struct curp *p);

/* obtener codigo iso de entidad */
const char *curp_entidad_iso(unsigned int entidad);

/* obtener nombre de entidad */
const char *curp_entidad_nombre(unsigned int entidad);

/* obtiene el codigo de error */
enum curp_err curp_error(int code);

#endif
