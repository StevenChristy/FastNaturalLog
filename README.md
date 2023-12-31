# Fast Natural Log
Fast, free, and precise algorithm for calculations of natural logarithm.

# Output
|        LN |  FP32 FNL |       STL |        FP64 FNL |             STL |
|----------:|----------:|----------:|----------------:|----------------:|
|       inf |       inf |       inf |             inf |             inf |
|      -inf | -nan(ind) | -nan(ind) |       -nan(ind) |       -nan(ind) |
|       nan |       nan |       nan |             nan |             nan |
| -nan(ind) | -nan(ind) | -nan(ind) |       -nan(ind) |       -nan(ind) |
|        -2 | -nan(ind) | -nan(ind) |       -nan(ind) |       -nan(ind) |
|      -1.5 | -nan(ind) | -nan(ind) |       -nan(ind) |       -nan(ind) |
|        -1 | -nan(ind) | -nan(ind) |       -nan(ind) |       -nan(ind) |
|      -0.5 | -nan(ind) | -nan(ind) |       -nan(ind) |       -nan(ind) |
|         0 |      -inf |      -inf |            -inf |            -inf |
|     1e-07 |  -16.1181 |  -16.1181 |   -16.118095651 |   -16.118095651 |
|      0.01 |  -4.60517 |  -4.60517 |  -4.60517018599 |  -4.60517018599 |
|      0.05 |  -2.99573 |  -2.99573 |  -2.99573227355 |  -2.99573227355 |
|       0.1 |  -2.30259 |  -2.30259 |  -2.30258509299 |  -2.30258509299 |
|       0.2 |  -1.60944 |  -1.60944 |  -1.60943791243 |  -1.60943791243 |
|       0.5 | -0.693147 | -0.693147 |  -0.69314718056 |  -0.69314718056 |
|       0.6 | -0.510826 | -0.510826 | -0.510825623766 | -0.510825623766 |
|      0.85 | -0.162519 | -0.162519 | -0.162518929498 | -0.162518929498 |
|         1 |         0 |         0 |               0 |               0 |
|       1.1 | 0.0953102 | 0.0953102 | 0.0953101798043 | 0.0953101798043 |
|       1.2 |  0.182322 |  0.182322 |  0.182321556794 |  0.182321556794 |
|       1.3 |  0.262364 |  0.262364 |  0.262364264467 |  0.262364264467 |
|       1.4 |  0.336472 |  0.336472 |  0.336472236621 |  0.336472236621 |
|       1.5 |  0.405465 |  0.405465 |  0.405465108108 |  0.405465108108 |
|       1.6 |  0.470004 |  0.470004 |  0.470003629246 |  0.470003629246 |
|       1.7 |  0.530628 |  0.530628 |  0.530628251062 |  0.530628251062 |
|       1.8 |  0.587787 |  0.587787 |  0.587786664902 |  0.587786664902 |
|       1.9 |  0.641854 |  0.641854 |   0.64185388617 |  0.641853886172 |
|         2 |  0.693147 |  0.693147 |   0.69314718056 |   0.69314718056 |
|       2.1 |  0.741937 |  0.741937 |  0.741937344729 |  0.741937344729 |
|       2.2 |  0.788457 |  0.788457 |  0.788457360364 |  0.788457360364 |
|       100 |   4.60517 |   4.60517 |   4.60517018599 |   4.60517018599 |
|      1000 |   6.90776 |   6.90776 |   6.90775527898 |   6.90775527898 |
|     10000 |   9.21034 |   9.21034 |   9.21034037198 |   9.21034037198 |
|       max |   88.7228 |   88.7228 |   709.782712893 |   709.782712893 |
|    lowest | -nan(ind) | -nan(ind) |       -nan(ind) |       -nan(ind) |

# How it works

This implementation of natural log exploits the layout of IEEE 754 floating point and properties of the natural log. We can quickly and accurately calculate the natural log when its in the range of 1 < x < 2. With bit manipulation the exponent can be normalized into that range. We then calculate the natural log of the normalized x. Finally, we calculate the natural log of the original exponent and add that to the result.

$y = \ln x = \ln normalize(x) + logb(x) * \ln 2$

## Steps
1. Obtain a normalized x ($nx$) by changing the exponent of x to $2^0$ using bit manipulation.
2. Obtain the natural log of the normalized x ($\ln nx$) by using any function which is precise within the range of 1 < x < 2.
3. Add $logb(x) * \ln 2$

Additional conditions must checked to ensure that the output matches the behavior of std::log.

# Considerations

This implementation of natural log is accurate over the entire number range and matches the output of the C++ std::log function. Adding additional precision and changing it to support similar data types should be relatively straightforward. Although I only implemented and tested it for float and double it should be easy to add support for long double or any data type that is similar to the IEEE 754 float.

## Smaller Floats

It should fine to decrease the iterations of the polynomial series for smaller data types which will increase the speed.

## Larger Floats

Increase the precision as necessary by adding additional iterations of the polynomial series.

## Precision vs Speed

If speed is more important than precision then iterations can be reduced. If precision is more important then one can simply continue adding iteration as needed.

## Customized Error Checking

The default implementation assumes that one wants the behavior to be identical to the std::log with respect to negative numbers, +inf, -inf, +nan, -nan, and 0. However customizing that behavior should be possible if one needs something different.

## More Speed

If accuracy is only needed where 0 < x < 2 then one could eliminate Step 1 and Step 3.

## Conclusion

This implementation can easily be customized for speed, precision, or other floating point types as needed.
