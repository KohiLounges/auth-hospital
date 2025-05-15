#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <direct.h>   
#include <string.h>

const char *carpeta = "Hospital";
const char *auth_directory = "Hospital/auth.txt";

int carpeta_existe(const char *carpeta);
int program_check(void);
int program_auth(void);

#define CLAVE 'K'  // Clave para encriptar y desencriptar

int main(void) {
    printf("\nPrograma Medico de Salud");
    printf("\nSTATUS: Alpha");
    printf("\n");
    printf("\n");
    int user = program_check();
    
    if (user == 0) {
        printf("\nLog: Program Check OK");
        program_auth();
    } else {
        printf("\nLog: Program Check FAIL");
    }
    

}

int program_check(void) {

if (!carpeta_existe(carpeta)) {
    if (_mkdir(carpeta) == 0) {
        printf("Log: Carpeta creada %s\n", carpeta);
    } else {
        perror("Log: Error al crear la carpeta");
    }
} else {
    printf("Log: La carpeta %s ya existe.\n", carpeta);
}

FILE *auth_verify = fopen(auth_directory, "r");
if (auth_verify == NULL) {
    printf("Log: auth no encontrado.");

    FILE *auth = fopen(auth_directory, "w");

    if (auth != NULL) {
        printf("Log: auth creado.\n");
        fclose(auth); 
        return 0;
    } else {
        perror("Log: No se pudo crear auth.txt");
        return 1;
    }

    } else {
        fclose(auth_verify);
        printf("Log: auth.txt ya existe.\n");
        return 0;
    }
}

int carpeta_existe(const char *carpeta) {
    struct _stat st;
    return (_stat(carpeta, &st) == 0 && (st.st_mode & _S_IFDIR));
}

void encript_use(char *texto, char clave) {
    for (int i = 0; texto[i] != '\0'; i++) {
        texto[i] ^= clave;  
    }
}

int program_auth(void) {

    char usuario[50];
    char password[50];
    char matricula[50];

    // Pedir datos para registro
    printf("\nRegistro de usuario\n");
    printf("Usuario: ");
    fgets(usuario, sizeof(usuario), stdin);
    printf("Contraseña: ");
    fgets(password, sizeof(password), stdin);
    printf("Matrícula: ");
    fgets(matricula, sizeof(matricula), stdin);

    // Quitar salto de línea
    usuario[strcspn(usuario, "\n")] = 0;
    password[strcspn(password, "\n")] = 0;
    matricula[strcspn(matricula, "\n")] = 0;

    // Encriptar datos
    encript_use(usuario, CLAVE);
    encript_use(password, CLAVE);
    encript_use(matricula, CLAVE);

    FILE *f = fopen(auth_directory, "w");
    if (f == NULL) {
        perror("No se pudo abrir auth.txt para escribir");
        return 1;
    }
    fprintf(f, "%s\n%s\n%s\n", usuario, password, matricula);
    fclose(f);
    printf("Datos guardados encriptados correctamente.\n");

    // Pedir datos para login
    char login_usuario[50];
    char login_password[50];
    char login_matricula[50];

    printf("\nLogin\n");
    printf("Usuario: ");
    fgets(login_usuario, sizeof(login_usuario), stdin);
    printf("Contraseña: ");
    fgets(login_password, sizeof(login_password), stdin);
    printf("Matrícula: ");
    fgets(login_matricula, sizeof(login_matricula), stdin);

    // Quitar salto de línea
    login_usuario[strcspn(login_usuario, "\n")] = 0;
    login_password[strcspn(login_password, "\n")] = 0;
    login_matricula[strcspn(login_matricula, "\n")] = 0;

    // Leer datos guardados
    char file_usuario[50];
    char file_password[50];
    char file_matricula[50];

    FILE *auth_file = fopen(auth_directory, "r");
    if (auth_file == NULL) {
        perror("No se pudo abrir auth.txt para leer");
        return 1;
    }
    fgets(file_usuario, sizeof(file_usuario), auth_file);
    fgets(file_password, sizeof(file_password), auth_file);
    fgets(file_matricula, sizeof(file_matricula), auth_file);
    fclose(auth_file);

    // Quitar salto de línea de lo leído
    file_usuario[strcspn(file_usuario, "\n")] = 0;
    file_password[strcspn(file_password, "\n")] = 0;
    file_matricula[strcspn(file_matricula, "\n")] = 0;

    // Desencriptar datos leídos
    encript_use(file_usuario, CLAVE);
    encript_use(file_password, CLAVE);
    encript_use(file_matricula, CLAVE);

    // Comparar
    if (strcmp(login_usuario, file_usuario) == 0 &&
        strcmp(login_password, file_password) == 0 &&
        strcmp(login_matricula, file_matricula) == 0) {
        printf("\n✅ Usuario autenticado correctamente.\n");
        return 0;
    } else {
        printf("\n❌ Datos incorrectos. Acceso denegado.\n");
        return 1;
    }
}