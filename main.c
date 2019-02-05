/* included sample programs */

/* A program to perform Euclid's
 * Algorithm to compute a gcd. */

void output_function(int x){}
int input(void){ return 0;}
int gcd (int u, int v)
{
    if (v == 0) return u;
    else return gcd(v,u-u/v*v);
    /* u-u/v*v == u mod v */
}

int main(void)
{ int x; int y;
    x = input(); y = input();
    output(gcd(x,y));
    return 0;
}

/* selection sort */
int x[10];
int minloc (int a[], int low, int high)
{
    int i; int x; int k;
    k = low; x = a[low]; i = low + 1;
    while ( i < high)
    {
        if (a[i] < x)
        {
            x = a[i];
            k = i;
        }
        i = i + 1;
    }
    return k;
}

void sort(int a[], int low, int high)
{
    int i; int k;
    i = low;
    while ( i < high-1)
    {
        int t;
        k = minloc(a,i,high);
        t = a[k];
        a[k] = a[i];
        a[i] = t;
        i = i + 1;
    }
}

void main2(void)
{
    int i;
    i = 0;
    while ( i < 10)
    {
        x[i] = input();
        i = i + 1;
    }
    sort(x,0,10);
    i = 0;
    while (i < 10)
    {
        output(x[i]);
        i = i + 1;
    }
}
