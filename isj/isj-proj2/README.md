## [ISJ] Project 2 - Polynomial

Implementation of class Polymonial in Python, taking polynomials as lists.

For example: `2x^3 - 3x + 1` as `[1,-3,0,2]`

#### Instantiation
```python
pol1 = Polynomial([1,-3,0,2])
pol2 = Polynomial(1,-3,0,2)
pol3 = Polynomial(x0=1,x3=2,x1=-3)
```

#### Print polynomial
Function print() prints polynomial in standard format:
```python
>>> print(pol2)
2x^3 - 3x + 1
```
#### Other methods

1.  **Addition**
```python
>>> print(Polynomial(1,-3,0,2) + Polynomial(0, 2, 1))
2x^3 + x^2 - x + 1
```
2.  **Squaring**
```python
>>> print(Polynomial(-1, 1) ** 2)
x^2 - 2x  + 1
```
3. **Derivation**
```python
>>> print(pol1.deriv())
6x^2 - 3
```
4. **Derivation at value**
```python
>>> print(pol1.at_value(2))
11
```
If two values are given, subsumption of values is counted.
```python
>>> print(pol1.at_value(2,3))
35
```
