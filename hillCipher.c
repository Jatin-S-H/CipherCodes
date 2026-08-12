#include <stdio.h>
#include <string.h>
#include <ctype.h>

void trim(char s[])
{
    int start = 0, end = strlen(s) - 1;

    while (s[start] == ' ')
        start++;

    while (end >= start && s[end] == ' ')
        end--;

    memmove(s, s + start, end - start + 1);
    s[end - start + 1] = '\0';
}

int main()
{
    char input[100], text[100], cipher[100];
    int key[2][2] = {{27, 4}, {20, 7}};
    int i, n = 0, a, b;

    printf("Enter plaintext: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    trim(input);

    /* Check spaces and invalid characters */
    for (i = 0; input[i] != '\0'; i++)
    {
        if (input[i] == ' ')
        {
            printf("Spaces are not allowed in Hill Cipher.\n");
            return 0;
        }

        if (!isalpha((unsigned char)input[i]))
        {
            printf("Digits and special characters are not allowed.\n");
            return 0;
        }
    }

    if (input[0] == '\0')
    {
        printf("Plaintext cannot be empty.\n");
        return 0;
    }

    /* Convert to uppercase */
    for (i = 0; input[i] != '\0'; i++)
        text[n++] = toupper((unsigned char)input[i]);

    /* Padding */
    if (n % 2 != 0)
        text[n++] = 'X';

    /* Encryption */
    for (i = 0; i < n; i += 2)
    {
        a = text[i] - 'A';
        b = text[i + 1] - 'A';

        cipher[i] =
            (key[0][0] * a + key[0][1] * b) % 26 + 'A';

        cipher[i + 1] =
            (key[1][0] * a + key[1][1] * b) % 26 + 'A';
    }

    cipher[n] = '\0';

    printf("Ciphertext: %s\n", cipher);

    return 0;
}
