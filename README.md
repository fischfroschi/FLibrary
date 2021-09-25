# FLibrary
FArray &amp; Collection of Algorithms

# FArray
## Usage
### Create FArray
```c++
FArray<int> Array();
FArray<int> Array2(10);
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
After you insertet or removed a bunch of elements, you should call this function. It will save memory and is also used internel.
```c++
Array.Optimaziation();
```
### Get Array Pointer
This pointer should be deleted if you don't need it anymore.
```c++
Array.GetArray(/*Specify whether you will reuse the Array at a later time. This won't clear the array if it is false. Default: true*/);
```
### Delete FArray
This is also done by the destructor.
```c++
Array.Clean();
```
# License
Shield: [![CC BY 4.0][cc-by-shield]][cc-by]

This work is licensed under a
[Creative Commons Attribution 4.0 International License][cc-by].

[![CC BY 4.0][cc-by-image]][cc-by]

[cc-by]: http://creativecommons.org/licenses/by/4.0/
[cc-by-image]: https://i.creativecommons.org/l/by/4.0/88x31.png
[cc-by-shield]: https://img.shields.io/badge/License-CC%20BY%204.0-lightgrey.svg
