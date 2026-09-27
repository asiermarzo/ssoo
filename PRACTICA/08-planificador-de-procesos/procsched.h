// procsched.h
#define RUTA_CLAVE    "/etc"   //para ftok(RUTA_CLAVE, ID_PROJ)
#define ID_PROJ       22

#define MAX_COMANDO   256 //longitud máxima de un comando

#define TURNO_MS      15   // duración por defecto de un turno (niveles 2 y 3)
#define LATENCIA_MS   33   // espera máxima por defecto de los interactivos (nivel 2)

#define MTYPE_COMANDO 4    // mtype de los comandos especiales

typedef struct {
    long mtype;                 // prioridad (1, 2 o 3) o 4 para comandos especiales
    char comando[MAX_COMANDO];  // "programa arg1 arg2 ...", o el comando: "turno 50"
} peticion_t;
