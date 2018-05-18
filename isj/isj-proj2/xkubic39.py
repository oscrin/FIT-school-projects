#!/usr/bin/env python3
"""
    File name: proj2.py
    Name: ISJ - proj2 (Polynomial)
    Author: Jan Kubica
    Login: xkubic39
    Email: xkubic39@stud.fit.vutbr.cz
    Date created: 10-04-2016
    Date last modified: 15-04-2016
    Python Version: 3.5.1
"""

__author__ = "xkubic39"
__email__ = "xkubic39@stud.fit.vutbr.cz"
__version__ = "1.0"

from operator import add

class Polynomial:
 
    polynom = []
    """
        Class contains one list variable 'polynom' storing polynomial coefficients. 
    """
   
    def __init__(self, *coefs, **kwargs):
        """
            Class can be initialized in three ways, such as:
                pol1 = Polynomial([1,-3,0,2])
                pol2 = Polynomial(1,-3,0,2)
                pol3 = Polynomial(x0=1,x3=2,x1=-3)
        """
        if (len(coefs) > 0):                                                    # if coefficients are given 
            if (type(coefs[0]) is int):                                         # in way of integers
                self.polynom = list(coefs)
            if (type(coefs[0]) is list):                                        # in way of list
                self.polynom = coefs[0]
        if (len(kwargs) > 0):                                                   # searching of the highest exponent "maxkey"
            maxkey = 0
            for key in kwargs:
                newkey = int(key.replace("x", ""))
                if (maxkey < newkey):
                    maxkey = newkey
            self.polynom = [0] * (maxkey+1)
            for key in kwargs:                                                  # inserting coefficients to the list "polynom"
                newkey = int(key.replace("x", ""))
                self.polynom[newkey] = kwargs[key]

    def __str__(self):
        """
            Class is printed out in normal form of a string:
                >>> print(pol2)
                2x^3 - 3x + 1
        """
        polynom = self.polynom
        while (len(polynom) > 1 and polynom[-1] == 0):                          # removing non-important zeros coefficients in the begining
                del polynom[-1]

        absolute = list(map(abs, polynom))                                      # list of absolute coeficients
        len_p = len(polynom)                                                    # lenght of polynom
        poly = ""
        for index in reversed(range(len(polynom))):
            if (polynom[index] < 0 and index != len_p-1):                       # add " - " if coefficient is negative integer
                poly += " - "
            elif (polynom[index] < 0 ):                                         # add "- " if the highest coefficient is negative integer
                poly += "- "
            elif (polynom[index] > 0 and index != len_p-1):                     # add " + " to the non-first coefficient
                poly += " + "
            if (index > 0 and absolute[index] != 0 and absolute[index] == 1):   # add "x" instead of coefficient, if the coefficient is equal to 1
                poly += "x"
            elif (index > 0 and absolute[index] != 0):                          # add coefficient and "x" to the all other exponential parts
                poly += str(absolute[index]) + "x"
            if (index > 1 and absolute[index] != 0):                            # add all exponent higher or equal to 2
                poly += "^" + str(index)
            if (absolute[index] != 0 and index == 0):                           # add non-zero constat in the end of polynomial
                poly += str(absolute[index])
        return poly

    def __add__(poly1, poly2):
        """
            Two polynomials can be summed up:
                >>> print(Polynomial(1,-3,0,2) + Polynomial(0, 2, 1))
                2x^3 + x^2 - x + 1
        """
        if (len(poly1.polynom) > len(poly2.polynom)):                           # setting equal length of both polynomials adding zeros in the end
            while (len(poly1.polynom) > len(poly2.polynom)):
                poly2.polynom.append(0)
        if (len(poly2.polynom) > len(poly1.polynom)):
            while (len(poly2.polynom) > len(poly1.polynom)):
                poly1.polynom.append(0)
        return Polynomial(list(map(add, poly1.polynom, poly2.polynom)))         # counting up exponentially corresponding coefficients

    def __mul__(poly1, poly2):
        """
            Polynomial can be multiplited by another polynomial.
        """
        result = Polynomial([0])
        for com1 in range(len(poly1.polynom)):                                  # multiplying second polynomial coefficient by coefficient
            pom = []
            for com2 in range(len(poly2.polynom)):
                pom.append(poly1.polynom[com1] * poly2.polynom[com2])
            for shift in range(com1):                                           # shift right all the polynomial list if it's exponential coefficient
                pom.insert(0,0)
            result += Polynomial(pom)
        return result

    def __pow__(poly, num):
        """
            Polynomial can be raised to the power of positive integer.
                >>> print(Polynomial(-1, 1) ** 2)
                x^2 - 2x  + 1
        """
        power = poly
        for count in range(num-1):
            power *= poly
        return power

    def derivative(self):
        """
            Method returning a derivation of specificied polynomial.
                >>> print(pol1.derivative())
                6x^2 - 3
        """
        derivate = []
        for i in range(len(self.polynom)):
            derivate.append(self.polynom[i] * i)                                # adding final coefficients in the end of list "derivate" multiplied by it's exponent
        return Polynomial(derivate[1:])

    def at_value(self, *nums):
        """
            Method returning final result of counted polynomial with specifided variable.
            If two variables are given, final result is counted as a difference between 
            two counted polynomials with specifided variables. For example:
            >>> print(Polynomial(1,-3,0,2).at_value(2))
            11
        """
        coef = self.polynom
        result = 0

        if (len(nums) >= 1):                                                    # if just one number is given
            num1 = nums[0]
            for index in reversed(range(len(coef))):
                result += self.polynom[index] * (num1 ** index)
        if (len(nums) >= 2):                                                    # if two numbers are given
            num2 = nums[1]
            result2 = 0
            for index in reversed(range(len(coef))):
                result2 += self.polynom[index] * (num2 ** index)
            result = result2 - result      
        return result

"""
    Formal testing of program and it's functions.
"""

print ("===== PROGRAM TEST =====\n")

pol1 = Polynomial([1,-3,0,2])
pol2 = Polynomial(1,-3,0,2)
pol3 = Polynomial(x0=1,x3=2,x1=-3)

print ("=== PRINT TEST ===")
print ("   Polynomial([1,-3,0,2])  = %s" % (pol1))
print ("   Polynomial(1,-3,0,2)  = %s" % (pol2))
print ("   Polynomial(x0=1,x3=2,x1=-3)  = %s" % (pol3))

print ("=== SUM TEST ===")
print ("   Polynomial(1,-3,0,2) + Polynomial(0, 2, 1))) = %s" % (Polynomial(1,-3,0,2) + Polynomial(0, 2, 1)))
print ("   Polynomial(5,5,5,5) + Polynomial(-5,-5,-5,-5))) = %s" % (Polynomial(5,5,5,5) + Polynomial(-5,-5,-5,-5)))
print ("   Polynomial(1,2,3,4) + Polynomial(4, 3))) = %s" % (Polynomial(1,2,3,4) + Polynomial(4, 3)))
print ("   Polynomial(9,0,0,2) + Polynomial(2, 2, 1,2,2,2)))= %s" % (Polynomial(9,0,0,2) + Polynomial(2, 2, 1,2,2,2)))
print ("   Polynomial(2, 2, 1,2,2,2) + Polynomial(9,0,0,2))) = %s" % (Polynomial(2, 2, 1,2,2,2) + Polynomial(9,0,0,2)))

print ("=== POW TEST ===")
print ("   (Polynomial(1,1) ** 2) = %s" % (Polynomial(1,1) ** 2))
print ("   (Polynomial(1,1) ** 3) = %s" % (Polynomial(1,1) ** 3))
print ("   (Polynomial(5,1) ** 2) = %s" % (Polynomial(5,1) ** 2))
print ("   (Polynomial(3,2,1) ** 4) = %s" % (Polynomial(3,2,1) ** 4))
print ("   (Polynomial(3,0,1) ** 5) = %s" % (Polynomial(3,0,1) ** 5))

print ("=== AT_VALUE TEST ===")
print ("   Polynomial([1,-3,0,2]).at_value(2)  = %s" % (Polynomial([1,-3,0,2]).at_value(2)))
print ("   Polynomial(1,-3,0,2).at_value(2,3)  = %s" % (Polynomial(1,-3,0,2).at_value(2,3)))

print ("=== DERIVATIVE TEST ===")
print ("   Polynomial([1,-3,0,2]).derivative() = %s" % (Polynomial([1,-3,0,2]).derivative()))
print ("   Polynomial(9,0,0,2).derivative() = %s" % (Polynomial(9,0,0,2).derivative()))
print ("   Polynomial(1,2,3,4).derivative() = %s" % (Polynomial(1,2,3,4).derivative()))
