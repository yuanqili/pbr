# Managing Rounding Error

## Floating-Point Arithmetic

### Floating-Point Representation

The IEEE standard specifies that 32-bit floats are represented with a sign bit, 8 bits for the exponent, and 23 bits for
the significand. With 8 bits, the exponent $e_b$ ranges from 0 to 255; the actual exponent used, $e_b$, is computed by
biasing $e$: $$e_b = e - 127.$$

Given a sign $s = \pm 1$, significand $m$, and exponent $e$, the corresponding floating-point value is
$$s \times 1.m \times 2^{e-127}.$$

An implication of this representation is that the spacing between representable floats between two adjacent powers of
two is uniform throughout the range. In a range $[2^e, 2^{e+1})$, the spacing is $$2^{e-23}.$$


```c++
static constexpr Float MaxFloat = std::numeric_limits<Float>::max();
static constexpr Float Infinity = std::numeric_limits<Float>::infinity();
```

With $e_b=255$, if the significand bits are all zero, the value corresponds to positive or negative infinity, according
to the sign bit; non-zero significand bits corresponds to special NaN values, which result from invalid operations. NaNs
propagate through computations. Any comparsion with a NaN value returns false; thus checking for `!(x == x)` serves to
check if a value is a NaN. For clarity, we use the C++ standard library function `std::isnan()` to check.

### Utility Routines

For certain low-level operations, it can be useful to be able to interpret a floating-point value in terms of its
constituent bits and to convert the bits representing a floating-point value to an actual float or double.

```c++
float f = ...;
uint32_t bits = *((uint32_t *)&f);
```

```c++
union FloatBits {
    float f;
    uint32_t ui;
}
FloatBits fb;
fb.f = ...;
uint32_t bits = fb.ui;
```

The two above approaches are ill-defined in modern C++. The following is a good approach:

```c++
inline uint32_t FloatToBits(float f) {
    uint32_t ui;
    memcpy(&ui, &f, sizeof(float));
    return ui;
}

inline float BitsToFloat(uint32_t ui) {
    float f;
    memcpy(&f, &ui, sizeof(uint32_t));
    return f;
}

inline float NextFloatUp(float v) {
    // handle infinity and negative zero for NextFloatUp()
    if (std::isinf(v) && v > 0.f) return v;
    if (v == -0.f) return 0.f;
    // advance v to next higher float
    uint32_t ui = FloatToBits(v);
    v >= 0 ? ++ui : --ui;
    return BitsToFloat(ui);
}
```

Two measurements of error are useful in this effort: absolute and relative. If we perform some floating-point
computation and get a rounded result $\tilde{a}$, we say that the magnitude of the difference between $\tilde{a}$ and
the result of doing that computation in the real numbers is the _absolute error_, $$\delta_a = |\tilde{a} - a|.$$

_Relative error_, $\delta_r$ is the ratio of the absolute error to the precise result:
$$\delta_r = \bigg| {\tilde{a} - a \over a} \bigg| = \bigg| {\delta_a \over a} \bigg|,$$
as long as $a \ne 0$. Therefore we have
$$\tilde{a} = a \pm \delta_a = a(1 \pm \delta_r).$$

