/******************************************************************/
/*                          IZP Project 2                         */
/*                                                                */
/*                         proj2.c  (v2.0)                        */
/*                                                                */
/*                 Created by Jan Kubica (xkubic39)               */
/*                            Nov 2015                            */
/******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

double ab(double x) // fce pro absolutni hodnotu
{
	return x > 0 ? x : -x;
}

double taylor_log(double x, unsigned int n) // vypocet log pomoci taylorova polynomu
{
	if (x == 0)
	{
		return -INFINITY;
	}

 	if  (x < 0)
 	{
 		return NAN;
 	}

 	if (x == INFINITY)
 	{
 		return INFINITY;
 	}

	if (x >= 0 && x < 1) // pro interval <0;1)
 	{
 		double p = (1-x); // vyjadreni zakladni promenne
 		double t = -p; // prvni iterace polynomu
 		double s = t; // prirazeni prvni iterace do vysledne hodnoty
 		
 		for (unsigned int i = 2; i <= n; i++)
  		{
  			t = (t * p / i) * (i-1); // vzorec pro navyseni clenu o jeden
   	 		s = s + t; // pricteni clenu k vysledku pro aktualni iteraci
   		}

		return s;
	}

	else // pro cisla (x >= 1) - zaporne hodnoty osetreny na vstupu
 	{
 		double t = (x-1) / x; // prvni iterace polynomu
 		double s = t; // prirazeni prvni iterace do vysledne hodnoty
 		
 		for (unsigned int i = 2; i <= n; i++)
  		{
  			t = t * ((x-1) / x) / i * (i-1); // vzorec pro navyseni clenu o jeden
    		s = s + t; // pricteni clenu k vysledku pro aktualni iteraci
    	}

 		return s;
 	}
}


double cfrac_log(double x, unsigned int n) // vypocet log pomoci zrtezeneho zlomku
{
	if (x == 0)
	{
		return -INFINITY;
	}

 	if  (x < 0)
 	{
 		return NAN;
 	}

 	if (x == INFINITY)
 	{
 		return INFINITY;
 	}

	double z = (1-x) / (-1-x); // vyjadreni "z" ze vzorce
	double t = z*z*(n-1)*(n-1) / (2*n-1); // posledni clen cyklu
	double s;
 	
 	for (unsigned int i = --n; i >= 2; i--) // postupne snizovani od celkoveho poctu iteraci
  	{
  		t = z*z*(i-1)*(i-1) / ((2*i - 1) - t); // zpetny vypocet (zabezpeceni preteceni)
    }

	s = 2*z / (1-t); // prvni iterace zlomku

 	return s;
}

unsigned int cfrac_iter(double min, double max, double eps) // vypocet iteraci pro zretezeny zlomek
{
	unsigned int i = 1;

	while (ab(log(min) - cfrac_log(min,i)) >= eps)
	{
		i++;
	}

	unsigned int j = 1;

	while (ab(log(max) - cfrac_log(max,j)) >= eps)
	{
		j++;
	}

	return i > j ? i : j; // navrat vetsi hodnoty
}

unsigned int taylor_iter(double min, double max, double eps) // vypocet iteraci pro tayloruv polynom
{
	unsigned int k = 1;

	while (ab(log(min) - taylor_log(min,k)) > eps)
	{
		k++;
	}

	unsigned int l = 1;

	while (ab(log(max) - taylor_log(max,l)) > eps)
	{
		l++;
	}

	return k > l ? k : l; // navrat vetsi hodnoty
}

int main(int argc, char **argv)
{
	if (argc < 4) // info programu pri nizsim poctu argumentu, nez je pozadovano
	{
		printf("Program works just with these two types of arguments: \n");
		printf("--log X N \n--iter MIN MAX EPS \n");

		return 0;
	}

	if (strcmp(argv[1], "--log") == 0) // postup pro vstup --log
	{
		char *endptr_1;
		char *endptr_2;

		double x = strtod(argv[2], &endptr_1);
		long int n = strtol(argv[3], &endptr_2, 10);

		if (*endptr_1 != '\0' || *endptr_2 != '\0') // kontrola vstupu
		{
			printf("INPUT_ERROR\n");
			return -1;
		}

		if (n <= 0)
		{
			printf("UNSUPPORTED_NUMBER_OF_ITERATIONS\n");
			return -1;
		}

		if (x < 0)
		{
			printf("OUT_OF_DEFINED_LOGARITMUS_INTERVAL\n");
			return -1;
		}

		else // tisk pozadovanych hodnot
		{
			printf("       log(%.5g) = %.12g\n", x, log(x));
			printf("    cf_log(%.5g) = %.12g\n", x, cfrac_log(x,n));
			printf("taylor_log(%.5g) = %.12g\n", x, taylor_log(x,n));
		}
	}

	if (strcmp(argv[1], "--iter") == 0) // postup pro vstup --iter
	{
		char *endptr_1;
		char *endptr_2;
		char *endptr_3;

		double min = strtod(argv[2], &endptr_1);
		double max = strtod(argv[3], &endptr_2);
		double eps = strtod(argv[4], &endptr_3);


		if (*endptr_1 != '\0' || *endptr_2 != '\0' || *endptr_3 != '\0') // kontrola vstupu
		{
			printf("INPUT_ERROR\n");
			return -1;
		}

		if (min<0 || max<0)
		{
			printf("OUT_OF_DEFINED_LOGARITMUS_INTERVAL\n");
			return -1;
		}

		if (min > max)
		{
			printf("UNSUPPORTED_LOGARITMUS_INTERVAL\n");
			return -1;
		}

		if (eps < 1e-12)
		{
			printf("UNSUPPORTED_EPS_LIMIT\n");
			return -1;
		}

		else // tisk pozadovanych hodnot
		{
			printf("       log(%.5g) = %.12g\n", min, log(min));
			printf("       log(%.5g) = %.12g\n", max, log(max));

			
			unsigned int m = cfrac_iter(min, max, eps);

			printf("continued fraction iterations = %d\n", m);
			printf("    cf_log(%.5g) = %.12g\n", min, cfrac_log(min,m));
			printf("    cf_log(%.5g) = %.12g\n", max, cfrac_log(max,m));

			unsigned int n = taylor_iter(min, max, eps);

			printf("taylor polynomial iterations = %d\n", n);
			printf("taylor_log(%.5g) = %.12g\n", min, taylor_log(min,n));
			printf("taylor_log(%.5g) = %.12g\n", max, taylor_log(max,n));
		}
	}

    return 0;
}