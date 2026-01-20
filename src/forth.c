#include <forth.h>
#include <string.h>

static int64_t stack[STACK_SIZE];
static int64_t sp = 0;

/* e.g. % 65 $ 1 - !*/

void push(int64_t val)
{
        if (sp < STACK_SIZE)
                stack[sp++] = val;
}

int64_t pop(void)
{
        if (sp > 0)
                return stack[--sp];
#ifdef FORTH_DEBUG
        SerialPrint("WARNING: Stack underflow\r\n");
#endif
        return 0;
}

void executeToken(char **buf)
{
        if (!(*buf) || !(**buf))
                return;
        char *tok = *buf;
        if (isdigit(*tok))
        {
                size_t l;
                push(atoi(tok, &l));
                *buf += l - 1;
        }
        else if (*tok == 'l')
        {
#ifdef FORTH_DEBUG
                SerialPrint("INFO: Grabbing\r\n");
#endif
                uint64_t index = pop();
                push(stack[index % STACK_SIZE]);
        }
        else if (*tok == 's')
        {
                int64_t index = pop();
                int64_t value = pop();
                stack[index % STACK_SIZE] = value;
        }
        else if (*tok == '+')
        {
#ifdef FORTH_DEBUG
                SerialPrint("INFO: Adding\r\n");
#endif
                int64_t b = pop();
                int64_t a = pop();
                push(a + b);
        }
        else if (*tok == '@')
        {
#ifdef FORTH_DEBUG
                SerialPrint("INFO: Grabbing PC\r\n");
#endif
                push((int64_t)tok + 1);
        }
        else if (*tok == '!')
        {
#ifdef FORTH_DEBUG
                SerialPrint("INFO: Jumping\r\n");
#endif
                *buf = pop();
                return;
        }
        else if (*tok == '?')
        {
#ifdef FORTH_DEBUG
                SerialPrint("INFO: Jumping?\r\n");
#endif
                if ((int64_t)pop() > 0)
                        *buf = (char *)pop();
                return;
        }
        else if (*tok == '-')
        {
#ifdef FORTH_DEBUG
                SerialPrint("INFO: Subtracting\r\n");
#endif
                int64_t b = pop();
                int64_t a = pop();
                push(a - b);
        }
        else if (*tok == '*')
        {
#ifdef FORTH_DEBUG
                SerialPrint("INFO: Multiplying\r\n");
#endif
                int64_t b = pop();
                int64_t a = pop();
                push(a * b);
        }
        else if (*tok == '/')
        {
#ifdef FORTH_DEBUG
                SerialPrint("INFO: Dividing\r\n");
#endif
                int64_t b = pop();
                int64_t a = pop();
                push(a / b);
        }
        else if (*tok == '.')
        {
#ifdef FORTH_DEBUG
                SerialPrint("INFO: Printing\r\n");
#endif
                char str[16];
                itos(pop(), str, 10);
                SerialPrint(str);
                SerialPrint("\r\n");
        }
        else if (*tok == ',')
        {
#ifdef FORTH_DEBUG
                SerialPrint("INFO: Reading Char\r\n");
#endif
                push(SerialRead());
        }
        else if (*tok == '$')
        {
#ifdef FORTH_DEBUG
                SerialPrint("INFO: Printing Char\r\n");
#endif
                SerialPut(pop());
        }
        else if (*tok == 'd')
        {
#ifdef FORTH_DEBUG
                SerialPrint("INFO: Duplicating\r\n");
#endif
                int64_t val = pop();
                push(val);
                push(val);
        }
        else if (*tok == '^')
        {
#ifdef FORTH_DEBUG
                SerialPrint("INFO: Swapping\r\n");
#endif
                int64_t a = pop();
                int64_t b = pop();
                push(b);
                push(a);
        }
        else if (*tok == ' ')
        {
                /* do nothing */
        }
        else
        {
                SerialPrint("Unknown token: ");
                SerialPrint(tok);
                SerialPrint("\r\n");
        }

        (*buf) += 1;
}

void forth(char *buf)
{
        while (*buf)
        {
                executeToken(&buf);
        }
}
