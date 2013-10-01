#include <stdio.h>

int sv_garums(char *virkne)
{
    int n = 0;
    while (virkne[n] != '\0') {
        n++;
    }
    return n;
    // ...
    // TODO: PD1.1 kodu rakstīt te!
    // ...
}

void sv_kopet(char *no, char *uz)
{
    int n = 0;
    while (1) {
        uz[n] = no[n];
        if (no[n] == '\0') {
            break;
        }
        n++;
    }
    // ...
    // TODO: PD1.2 kodu rakstīt te!
    // ...
}

int sv_meklet(char *kur, char *ko)
{
    int n = 0, i = 0;
    while (kur[n] != '\0') {
        while (1) {
            if (ko[i] == '\0') return n;
            if (kur[n+i] != ko[i]) {
                break;
            }
            i++;
        }
        i = 0;
        n++;
    }
    return -1;
    // ...
    // TODO: PD1.3 kodu rakstīt te!
    // ...
}

void sv_apgriezt(char *virkne)
{
    char *s = virkne;
    char *e = s + sv_garums(virkne) -1;
    while (s < e) {
        char t = *s;
        *s = *e;
        *e = t;
        s++;
        e--;
    }

    // ...
    // TODO: PD1.4 kodu rakstīt te!
    // ...
}

void sv_vapgriezt(char *virkne)
{
    int g = sv_garums(virkne);
    char r[g+1];
    sv_apgriezt(virkne);
    // talak apgriez katru vardu atseviski
    // ...
    // TODO: PD1.5 kodu rakstīt te!
    // ...
}

int main(void) {
    char buferis[100];

    printf("Tests uzdevumam PD1.1:\n");
    printf("%d\n", sv_garums("hello world"));
    printf("%d\n", sv_garums("123"));
    printf("%d\n", sv_garums(""));

    printf("Tests uzdevumam PD1.2:\n");

    // ...
    // TODO: PD1 testu kodu rakstīt te!
    // ...

    // Uzdevumus 4. un 5. testēt šādā veidā, lai izvairītos no access violation!
    sv_kopet("abcd", buferis);
    // sv_apgriezt(buferis);
    printf("%s\n", buferis);

    printf("Tests uzdevumam PD1.3:\n");
    printf("%d\n", sv_meklet("to be or not to be", "be"));

    printf("Tests uzdevumam PD1.4:\n");
    char virk[4] = "abcd";
    sv_apgriezt(virk);
    printf("%s\n", virk);

    printf("Tests uzdevumam PD1.5:\n");
    char virk2[11] = "hello world";
    sv_vapgriezt(virk2);
    printf("%s\n", virk2);

    return 0;
}
