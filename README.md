# FLibrary
FArray &amp; Collection of Algorithms

# FArray
## Usage
### Include FArray
```c++
#include "FArray.h"
```
### Create FArray
```c++
ff::FArray<int> Array();
ff::FArray<int> Array2(10);
```
### Add Element
```c++
Array.Add(/*Value*/ 50);
Array.Add_Back(/*Value*/ 50);
```
### Insert Element
```c++
Array.Insert(/*Target Position*/ 5, /*Value*/ 50);
```
### Remove Element
```c++
Array.Remove(/*Target Position*/ 7);
```
### Remove Elements with Value
```c++
Array.Remove_Val(/*Value*/ 50);
```
### Remove Elements with Condition
```c++
Array.Remove_Val(/*Condition (here: lambda)*/ [](int x) -> bool { return x <= 50; });
```
### Optimize FArray
After you added or removed a bunch of elements, you should call this function. It will save memory and is also used internel.
```c++
Array.Optimaziation();
```
### Get or Change FArray Element
```c++
const int Value = Array[0];
Arrray[1] = Value;
```
### Sort FArray
This uses UltraQuickSort
```c++
Array.Sort();
```
### Get Array Pointer
This pointer should be deleted if you don't need it anymore.
```c++
Array.GetArray(/*Specify whether you will reuse the Array at a later time. This won't clear the array if it is false. Default: false*/);
```
### Delete FArray
This is also done by the destructor.
```c++
Array.Clean(/*Whether you want to clean the array and fill it with new values. Default: false*/);
// Or
Array.Close();
```
### Reopen FArray
This can be used even if Clean / Close / GetArray was called
```c++
Array.Reopen(/*Default Size*/ 10);
```
## Benchmark
Here is a small [benchmark](Benchmark.cpp).
# FAlgorithms
## Usage
### Include FAlgorithms
```c++
#include "FAlgorithms.h"
```
### UltraQuicksort
#### Complexity:
```c++
O(2n + LargestValue - SmallestValue)
```
#### Additional Meory Usage:
```c++
sizeof(n) * (LargestValue - SmallestValue) * 2 //Bytes
```
#### Example
```c++
const size_t Size = 10;
int* Array = new int[Size];
ff::UltraQuickSort<int>(Array, Size);
```
# License
Shield: [![CC BY 4.0][cc-by-shield]][cc-by]

This work is licensed under a
[Creative Commons Attribution 4.0 International License][cc-by].

[![CC BY 4.0][cc-by-image]][cc-by]

[cc-by]: http://creativecommons.org/licenses/by/4.0/
[cc-by-image]: https://i.creativecommons.org/l/by/4.0/88x31.png
[cc-by-shield]: https://img.shields.io/badge/License-CC%20BY%204.0-lightgrey.svg
