LKit
====

### Introduction

There have been several times that I needed to have a very simple, but
flexible, _embedded_ language for projects I was working on, and didn't
really have what I wanted. Embedding something as complex as PHP is possible,
but it's an _enormous_ overhead for most projects, and adds in a ton of
complexity that you have to factor into the cost of the project. After all,
who is going to maintain this code base going forward? Do we really _need_
the complexity of PHP?

What I arrived at was a very simple lisp-like language that has a very limited
scope of data types for the variables, and consequently, is a pretty clean
and efficient implementation. This is the point of LKit - to make this simple
language easy to include in applications so that I (and others) don't have to
re-write this over again when I need it.

### Library Dependencies

The focus of this library is **not** to be a general threading library. Nor
is it to attempt to write code that's already written. As such, we're going
to be using [boost](www.boost.org) for several components. This will include
things only used in the test applications. Since [boost](www.boost.org) is
available on virtually all platforms, and even included in the next C++
standard, this should not be a significant burden to the user.

There are even instructions to build it on Mac OS X, if you don't wish to use
the [Homebrew](http://mxcl.github.com/homebrew/) version that's freely
available.

The Value
---------

LKit's base variable is `lkit::value` and is a typical _variant_-style value
that can hold, at present, the following data types:

*	`bool` - the classic, and correct, C/C++ boolean value. When this is
	cast into any other values, a `true` is 1, and a `false` is 0.
*	`int` - the simple integer, typically a 32-bit integer, but the size
	is not the greatest factor, it's the lack of a decimal portion that
	is key. It makes math and equality so much easier to test.
*	`double` - the _typical_ value. This is primarily for data manipulation,
	and in that, the `double` is by far the most often used data type.
*	`uint64_t` or `timestamp` - this is for the fact that a lot of the
	data is timeseries, and a lot of the manipulations are based on dealing
	with time, so we need to have something that can represent time. This
	is currently based as microseconds since epoch, in case you needed to
	understand the base and offset.

The `lkit::value` can be assigned values, used in standard mathematical
expressions in C++, has all the equality and inequality operators
defined for it - the works. This appears to C++ as just another data type,
which is the point: make it fit into the standard C++ lexicon, and that will
make dealing with the more advanced classes a lot easier.

The Function
------------

An `lkit::function` is defined to be an operation, or series of operations,
on zero or more arguments, in the form of `value` instances, that produces
a `value` result. The basic API for a function is simple: implement a few
standard constructors and destructor, and then one key method:

```cpp
/*******************************************************************
 *
 *                       Evaluation Methods
 *
 *******************************************************************/
/**
 * This is the main evaluation point for the function. It takes a
 * vector of values and returns a value - simple. How it does this
 * is entirely up to the developer of that function.
 */
virtual value eval( std::vector<value *> & anArg ) = 0;
```
While it's nice to implement the other utility methods like `toString()`
and the equality and inequality operators, it's not strictly necessary.

The `function` can be stateful, if necessary, but then it's the responsibility
of the author to make sure that the state is preserved properly across all
calls to the `eval()` method. Most functions will be _stateless_ because
they won't need to be stateful, and it makes their implementation so much
easier.

The Expression
--------------

The `lkit::expression` is a subclass of `lkit::value`, and as such, can
appear as an argument to an `lkit::function`. The `lkit::expression` is
basically an `lkit::function` and a vector of `lkit::value` instances as
arguments. Since these are all pointers, polymorphism will work for us,
and we can use `value` or `expression` instances interchangeably.

A simple example of using `value`, `function`, and `expression` is
the summing of a few numbers:

```cpp
lkit::value	a, b, c, d;
a = 10.1;
b = 5.5;
c = 3.14;
d = 6.2;
lkit::func::max		f;
lkit::expression	y(&f, &a, &b, &c, &d);
std::cout << "The sum is: " << y.evalAsDouble() << std::endl;
```

### Order of Arguments and Type Coercion

It's important to note that you can make use of the type conercion in the
arguments to a function, and ultimately, an expression. The default for
`lkit::value` operations is to __retain__ the type of the target, and operate
_in_ the argument. The following test will explain:
```cpp
lkit::value	a, b, c, d;
// note that 'a' will be an (int)
a = 10;
// ...and the rest are (double)
b = 5.5;
c = 3.14;
d = 6.2;
lkit::func::max		f;
lkit::expression	y(&f, &a, &b, &c, &d);
std::cout << "The sum is: " << y.evalAsDouble() << std::endl;
```
This will return:
```cpp
The sum is: 24
```
because the first argument is an _integer_ and therefore, the other arguments
will be adding their integer component to the summing integer. This can be
very useful when you want to sum up the number of boolean values - each
`true` will be 1, each `false` a 0 - so long as the first argument is an
integer.

Contrast this to the following:
```cpp
lkit::value	a, b, c, d;
// note that 'a' will be an (int)
a = 10;
// ...and the rest are (double)
b = 5.5;
c = 3.14;
d = 6.2;
lkit::func::max		f;
lkit::expression	y(&f, &b, &a, &c, &d);
std::cout << "The sum is: " << y.evalAsDouble() << std::endl;
```
This will return:
```cpp
The sum is: 24.84
```
Note that the first argument is `b` as opposed to `a`, and so the sum is
a `double` value.

License
-------

	Copyright 2012 by Robert E. Beaty

	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the "Software"),
	to deal in the Software without restriction, including without limitation
	the rights to use, copy, modify, merge, publish, distribute, sublicense,
	and/or sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included
	in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
	OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.

