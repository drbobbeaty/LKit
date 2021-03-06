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
lkit::func::sum		f;
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
lkit::func::sum		f;
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
lkit::func::sum		f;
lkit::expression	y(&f, &b, &a, &c, &d);
std::cout << "The sum is: " << y.evalAsDouble() << std::endl;
```
This will return:
```cpp
The sum is: 24.84
```
Note that the first argument is `b` as opposed to `a`, and so the sum is
a `double` value.

The Language Syntax
-------------------

While it's possible to use these classes in _any_ kind of language, there
exists in LKit, the `lkit::parser` class to convert _source code_ into a
_Langueage Tree_ built up of these class components. The description of the
language is pretty simple, as it's meant to be a simple, transparent, mapping
of these components into a human-readable grammar.

### The Basic Expression

The basic _expression_ is a _function_ followed by a white-space, and then
zero or more _arguments_ to the _function_, each separated by a white-space,
all surrounded by parentheses. Here is an example where the user wants to
arithmetically sum three integers:

	(+ 5 3 2)

The above example is equivalent to the infix-notation:

	5 + 3 + 2

and all evaluate to:

	10

For _functions_ (i.e. operators) where order doesn't matter, the evaluated
value is independent of the order of the _arguments_. However, for _functions_
like subtraction and division, the order matters quite a bit, and the way the
evaluation is done is based on the idea of placing the operator between each
argument and then calculating left to right.

So the following _expression_:

	(- 5 3 2)

is equivalent to the infix notation of:

	5 - 3 - 2

which evaluates to:

	0

The evaluation is similar for division:

	(/ 10.0 2.0 5.0)

is equivalent to the infix notation:

	10.0 / 2.0 / 5.0

and evaluates to:

	1.0

### Multiple Expressions

The language allows for multiple expressions to be in a single source. For
example, the following is a simple way to define the variable `x` and then
use it in the subsequent expression:

	(set x (+ 1 2 3))
	(* x 3 (* x 2))

The parser will make _both_ language trees, and evaluate them in the order
in which they appeared in the code. The resulting value of the evaluation
will be the _last_ expression evaluated.

### Expressions as Values in Expressions

Of course this can all be included as a _value_ (a.k.a. _argument_) in another
_expression_:

	(+ (/ 10.0 2.5) (* (+ 1.5 2 6) 2.0))

in infix notation:

	(10.0 / 2.5) + (1.5 + 2 + 6) * 2.0

which evaluates to:

	23

### Equalities, Inequalities, and Logical Functions

In addition to the obvious arithmetic functions, there are the complete
suite of equality and inequality functions where for `==` _all_ elements
have to be equal to one another, and for `!=` the first argument is the
'test' condition, and _all remaining_ elements have to be not equal to
this 'test' value. Again, it's the same as _distributing_ the operator
in between the arguments and processing left to right.

The following table has several examples of the functions and their
respective results:

| Expression                           | Evaluates to |
|:-------------------------------------|:------------:|
|`(== 1 1.0 (* 2.0 0.5))`              | true         |
|`(!= 1 1.0 (* 2.0 0.5) (/ 2.0 1.0))`  | false        |
|`(< 1 3 5 6 10)`                      | true         |
|`(> 10 9 8 5 5 2)`                    | false        |
|`(or 1 0 0 1)`                        | true         |
|`(and 1 0 0 1)`                       | false        |
|`(not 1)`                             | false        |

### Variable Definition and Assignment

Variables can be referenced in the code anywhere a constant can be used.
The variables are typically the output of an expression, but they can be
simple constants as well. There are a few default variables defined in
the system at this time:

	e = 2.71828183
	pi = 3.14159265

but setting a variable is very simple. The code looks like this:

	(set x 14.5)

And the result is the value of the variable, in this case, the double 14.5.
This can work in many ways, for instance, the following defines the variable
`x` to be 6, and then uses that in another calculation in the code:

	(* (set x (+ 1 2 3)) 3 (* x 2))

Since the processing of the `set` _function_ is done at compile time, and
the lookup of the value of `x` in the last multiplication is done at
evaluation time, you don't have to worry about variables not being defined
_in time_.

Additionally, as long as the `lkit::parser` is active, the variables
defined in one source code can be used in another. For instance, the
following first defines the variable `x` and then uses it in the next
code segment:

```cpp
lkit::parser	p;
p.setSource("(+ (set x 5) 3 (* x 2))");
p.eval();
p.setSource("(* x 7 14)");
value	v = p.eval();
```
i.e. the parser _retains_ all variable and function definitions across
invocations until `reset()` or `clear()` is called.

Additionally, the parser allows for variables to have their values set as
many times as needed. Meaning they will **not** be immutable variables.
But they will be set at compile time, so the user is going to have to be
careful about the order that the variable assignments are being made.

TODO List
---------

### Function Definition

I also need to add in the function definition to the parser. A simple function
that adds two numbers might look like this:

	(defun add [a, b] (+ a b))

I'm not sure about the syntax for the variables, but we'll see.

One thing for sure - the user-defined functions will have to be in the code
_before_ they are referenced in the code itself. I'm not going to mess with
a two-pass compilation process at this point.

### Performance

Past the basics, I need to start looking at speed and efficient updating of
the expressions and therefore the resulting parser tree. I don't want to
recalculate things that haven't changed, and constants don't change, so it's
going to make a partial update scheme possible.

### Time-Series Data

Beyond that, I need to think about the syntax for time-series data.
Specifically, how do I want to short-hand the processing of time-series
data: filters, sliding windows, etc. There needs to be something other
than a simple looping construct. There's a lot to be gained in the
efficiency of the processing that way.

### Quick List of Ideas

*	Put in detection of _change_ in the `lkit::expression` values. This
	will then allow us to skip the calculation of an expression if the
	arguments haven't changed since the last calculation. This is a big
	speed boost.
*	Put in "delta" driven updates for the expressions. Allow that those
	functions _not_ supporting delta-updates will still work, but they
	won't work as efficiently.
*	If we need to have simple arithmetic processing, pull in muParser
	from CKit and retrofit the `lkit::value` for the `CKVariant` in the
	code. This gives us a completely new way to work on the expressions
	without any additional complication in the code.

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

