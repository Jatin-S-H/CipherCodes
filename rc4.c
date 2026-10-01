#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ================= SWAP ================= */

void swap_bytes(unsigned char *a, unsigned char *b)
{
    unsigned char temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

/* ================= RC4 KSA ================= */

void KSA(unsigned char S[256],
         const unsigned char *key,
         int keylen)
{
    unsigned char T[256];
    int i, j;

    j = 0;

    /*
        Step 1: Initialize S and T

        S[i] = i
        T[i] = K[i mod keylen]
    */

    for (i = 0; i < 256; i++)
    {
        S[i] = (unsigned char)i;
        T[i] = key[i % keylen];
    }

    /*
        Step 2: Permute S
    */

    for (i = 0; i < 256; i++)
    {
        j = (j + (int)S[i] + (int)T[i]) % 256;

        swap_bytes(&S[i], &S[j]);
    }
}

/* ================= RC4 PRGA ================= */

void RC4(unsigned char *data,
         int dataLen,
         const unsigned char *key,
         int keylen)
{
    unsigned char S[256];
    unsigned char streamByte;

    int i, j, t, n;

    i = 0;
    j = 0;

    /* Perform KSA */
    KSA(S, key, keylen);

    /*
        Generate keystream
        and XOR with data
    */

    for (n = 0; n < dataLen; n++)
    {
        i = (i + 1) % 256;

        j = (j + (int)S[i]) % 256;

        swap_bytes(&S[i], &S[j]);

        t = ((int)S[i] + (int)S[j]) % 256;

        streamByte = S[t];

        /*
            Encryption:
            C = M XOR K

            Decryption:
            M = C XOR K
        */

        data[n] = data[n] ^ streamByte;
    }
}

/* ================= HEX OUTPUT ================= */

void print_hex(const unsigned char *data, int length)
{
    int i;

    for (i = 0; i < length; i++)
    {
        printf("%02X", (unsigned int)data[i]);
    }

    printf("\n");
}

/* ================= MAIN ================= */

int main(void)
{
    char plaintext[4096];
    char key[256];

    unsigned char *data;

    int textLen;
    int keyLen;

    /* ================= INPUT ================= */

    printf("Enter Plain Text: ");

    if (fgets(plaintext, sizeof(plaintext), stdin) == NULL)
    {
        printf("Error reading plain text.\n");
        return 1;
    }

    /* Remove newline */
    plaintext[strcspn(plaintext, "\r\n")] = '\0';

    printf("Enter Key: ");

    if (fgets(key, sizeof(key), stdin) == NULL)
    {
        printf("Error reading key.\n");
        return 1;
    }

    /* Remove newline */
    key[strcspn(key, "\r\n")] = '\0';

    /* ================= LENGTH ================= */

    textLen = (int)strlen(plaintext);
    keyLen = (int)strlen(key);

    if (textLen == 0)
    {
        printf("Error: Plain text cannot be empty.\n");
        return 1;
    }

    if (keyLen == 0)
    {
        printf("Error: Key cannot be empty.\n");
        return 1;
    }

    /* ================= MEMORY ================= */

    data = (unsigned char *)malloc((size_t)textLen);

    if (data == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return 1;
    }

    memcpy(data, plaintext, (size_t)textLen);

    /* ================= ENCRYPTION ================= */

    RC4(
        data,
        textLen,
        (const unsigned char *)key,
        keyLen
    );

    printf("\nPlain Text : %s\n", plaintext);
    printf("Key        : %s\n", key);

    printf("Cipher Text: ");
    print_hex(data, textLen);

    /* ================= DECRYPTION ================= */

    /*
        RC4 uses the same operation
        for encryption and decryption.

        C XOR K = M
    */

    RC4(
        data,
        textLen,
        (const unsigned char *)key,
        keyLen
    );

    printf("Decrypted  : %.*s\n", textLen, data);

    /* ================= CLEANUP ================= */

    free(data);

    return 0;
}
