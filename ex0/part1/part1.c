#include <stdio.h>
#include <stdlib.h>

int isPower(int num); // checks if number is power of 2

int main(void)
{
    int val;
    printf("Enter size of input:\n");
    if (scanf("%d", &val) != 1 || val <= 0) // scanf fail
    {
        printf("Invalid size\n");
        return 0;
    }
    int *arr = (int *)malloc(val * sizeof(int));
    if (arr == NULL) // malloc fail
    {
        free(arr);
        return 0;
    }
    printf("Enter numbers:\n");
    for (int i = 0; i < val; i++)
    {
        if (scanf("%d", &arr[i]) != 1) // scanf fail
        {
            printf("Invalid number\n");
            free(arr);
            return 0;
        }
    }
    int sum = 0;
    for (int i = 0; i < val; i++)
    {
        int j = isPower(arr[i]);
        if (j >= 0) // if the number is power of 2
        {
            printf("The number %d is a power of 2: %d = 2^%d\n", arr[i], arr[i], j);
            sum += j;
        }
    }
    printf("Total exponent sum is %d\n", sum);
    free(arr);
    return 0;
}

int isPower(int num)
{
    // recieves a number and returns -1 if it is not a power of 2
    // returns the log2(num) if it is.
    int j = 0;
    if (num <= 0)
    {
        return -1;
    }
    while (num > 0)
    {
        if (num == 1)
        {
            return j;
        }
        if (num % 2 != 0)
        {
            return -1;
        }
        num = num / 2;
        j++;
    }
    return j;
}
