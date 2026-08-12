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
    char text[100], key[50], cipher[100];
    int i, j = 0, k, keyLen;

    printf("Enter plaintext: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';

    printf("Enter keyword: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';

    trim(text);
    trim(key);

    if (text[0] == '\0')
    {
        printf("Plaintext cannot be empty.\n");
        return 0;
    }

    if (key[0] == '\0')
    {
        printf("Keyword cannot be empty.\n");
        return 0;
    }

    /* Check plaintext */
    for (i = 0; text[i] != '\0'; i++)
    {
        if (text[i] != ' ' && !isalpha((unsigned char)text[i]))
        {
            printf("Digits and special characters are not allowed.\n");
            return 0;
        }
    }

    /* Check key */
    for (i = 0; key[i] != '\0'; i++)
    {
        if (!isalpha((unsigned char)key[i]))
        {
            printf("Digits, special characters and spaces are not allowed in the key.\n");
            return 0;
        }
    }

    keyLen = strlen(key);

    /* Encryption */
    for (i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == ' ')
        {
            cipher[i] = ' ';
        }
        else
        {
            k = (toupper((unsigned char)text[i]) - 'A' +
                 toupper((unsigned char)key[j % keyLen]) - 'A') % 26;

            cipher[i] = k + 'A';
            j++;
        }
    }

    cipher[i] = '\0';

    printf("Ciphertext: %s\n", cipher);

    return 0;
}
