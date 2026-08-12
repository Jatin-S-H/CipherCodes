#include <stdio.h>
#include <string.h>
#include <ctype.h>

char matrix[5][5];

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

void createMatrix(char key[])
{
    int used[26] = {0};
    int r = 0, c = 0, i;
    char ch;

    used['J' - 'A'] = 1;

    for (i = 0; key[i] != '\0'; i++)
    {
        ch = toupper((unsigned char)key[i]);

        if (ch == 'J')
            ch = 'I';

        if (!used[ch - 'A'])
        {
            matrix[r][c++] = ch;
            used[ch - 'A'] = 1;

            if (c == 5)
            {
                c = 0;
                r++;
            }
        }
    }

    for (ch = 'A'; ch <= 'Z'; ch++)
    {
        if (ch != 'J' && !used[ch - 'A'])
        {
            matrix[r][c++] = ch;
            used[ch - 'A'] = 1;

            if (c == 5)
            {
                c = 0;
                r++;
            }
        }
    }
}

void position(char ch, int *r, int *c)
{
    int i, j;

    if (ch == 'J')
        ch = 'I';

    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
            if (matrix[i][j] == ch)
            {
                *r = i;
                *c = j;
                return;
            }
}

int prepare(char input[], char output[])
{
    int i, n = 0, k = 0;
    char temp[100];

    for (i = 0; input[i] != '\0'; i++)
    {
        temp[n] = toupper((unsigned char)input[i]);

        if (temp[n] == 'J')
            temp[n] = 'I';

        n++;
    }

    for (i = 0; i < n;)
    {
        output[k++] = temp[i];

        if (i + 1 < n && temp[i] == temp[i + 1])
        {
            output[k++] = 'X';
            i++;
        }
        else if (i + 1 < n)
        {
            output[k++] = temp[i + 1];
            i += 2;
        }
        else
        {
            output[k++] = 'X';
            i++;
        }
    }

    output[k] = '\0';
    return k;
}

void encrypt(char text[], char cipher[], int n)
{
    int i, r1, c1, r2, c2;

    for (i = 0; i < n; i += 2)
    {
        position(text[i], &r1, &c1);
        position(text[i + 1], &r2, &c2);

        if (r1 == r2)
        {
            cipher[i] = matrix[r1][(c1 + 1) % 5];
            cipher[i + 1] = matrix[r2][(c2 + 1) % 5];
        }
        else if (c1 == c2)
        {
            cipher[i] = matrix[(r1 + 1) % 5][c1];
            cipher[i + 1] = matrix[(r2 + 1) % 5][c2];
        }
        else
        {
            cipher[i] = matrix[r1][c2];
            cipher[i + 1] = matrix[r2][c1];
        }
    }

    cipher[n] = '\0';
}

int main()
{
    char key[50], input[100];
    char text[100], cipher[100];
    int i, n;

    printf("Enter key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';
    trim(key);

    printf("Enter plaintext: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    trim(input);

    /* Check key */
    for (i = 0; key[i] != '\0'; i++)
    {
        if (key[i] == ' ')
        {
            printf("Spaces are not allowed in the key.\n");
            return 0;
        }

        if (!isalpha((unsigned char)key[i]))
        {
            printf("Digits and special characters are not allowed.\n");
            return 0;
        }
    }

    /* Check plaintext */
    for (i = 0; input[i] != '\0'; i++)
    {
        if (input[i] == ' ')
        {
            printf("Spaces are not allowed in Playfair Cipher.\n");
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

    createMatrix(key);

    n = prepare(input, text);

    encrypt(text, cipher, n);

    printf("Ciphertext: %s\n", cipher);

    return 0;
}
