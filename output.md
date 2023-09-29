
# CS202 Lecture notes -- Vectors of Vectors
<LI><a href=http://web.eecs.utk.edu/~jplank>James S. Plank</a>
<LI>Directory: <b>~jplank/cs202/notes/Vectors-of-Vectors</b>
<LI>Lecture notes:
    <a href=http://web.eecs.utk.edu/~jplank/plank/classes/cs202/Notes/Vectors-of-Vectors>
    <b>http://web.eecs.utk.edu/~jplank/plank/classes/cs202/Notes/Vectors-of-Vectors</b></a>
<LI> Original Notes: 2011 or so.
<LI> Last modification date: * 
Sat Aug 28 16:03:19 EDT 2021
*
---

Interestingly, vectors of strings don't seem to give students problems; however
vectors of vectors do, and the two are really not fundamentally different.  This
lecture gives you some practice with vectors of vectors.
<p>
Let's suppose that I want to represent a matrix of integers in 
C++.  The best way to do that is to use a vector of integer vectors.  
I will illustrate with the program
<b><a href=src/vdm.cpp>src/vdm.cpp</a></b>.
<p>

This program takes three command line arguments: * r*, * c* and * p*.
It then creates a <b>r * c</b> "Vandermonde" matrix over the field defined by the
prime number * p*?  What's a "field?"   In this case, it is the numbers 0 through * p-1*,
where addition, subtraction and multiplication are all modulo * p*.  Division 
is defined to be the inverse of multiplication, but don't worry about it, since it doesn't
really matter in this lecture.
<p>
A * Vandermonde* matrix is one that has the value * (i+1)<sup>j</sup>, mod p* in row * i* and
column * j* (everything is zero-indexed).  It has some very special properties concerning
invertibility of submatrices, but again, we don't care too much -- we just want to create
one and print it.  First, so you understand a Vandermonde matrix, here is one with five rows,
three columns and a prime of 17:

<p><center><table border=3 cellpadding=3><td><pre>
         Col  Col  Col
          0    1    2
         ---  ---  ---  
Row 0 |   1    1    1   
Row 1 |   1    2    4   
Row 2 |   1    3    9   
Row 3 |   1    4   16   
Row 4 |   1    5    8   
</pre></td></table></center><p>

As you can see, the only time we had to do the modulo operator was for the element in row 4,
column 2.  That one is equal to 5<sup>2</sup>, which equals
25, but we take it modulo 17, so it eight.
<p>
Take a look at the code:

<p><center><table border=3 cellpadding=3><td><pre>
<font color=blue>/* This program creates and prints a "Vandermonde" matrix.
   The user will enter a number of rows, a number of columns, and a prime number, p.
   The Vandermonde matrix element in row i, column j is equal to (i+1)^j, mod p.
   Vandermonde matrices have interesting mathematical properties, which I won't
   go into -- if you take CS494 from me in a few semesters, you'll learn about
   some of them! */</font>

#include &lt;vector&gt;
#include &lt;iostream&gt;
#include &lt;cstdio&gt;
#include &lt;sstream&gt;
using namespace std;

int main(int argc, char **argv)
{
  int r;                             <font color=blue>// Number of rows</font>
  int c;                             <font color=blue>// Number of columns</font>
  int p;                             <font color=blue>// The prime number</font>
  istringstream ss;                  <font color=blue>// We use this to read from the command line.</font>
  vector &lt; vector &lt;int&gt; &gt; vdm;       <font color=blue>// The Vandermonde matrix</font>
  int base, val;                     <font color=blue>// We use these to calculate (i+1)^j, mod p</font>
  size_t i, j;

  <font color=blue>/* Error check the command line.  I usually don't like to put multiple statements
     on a single line like this, but with error checking, it's cleaner. */</font>

  if (argc != 4) { cerr &lt;&lt; "usage: vdm rows cols prime\n"; return 1; }
  ss.clear(); ss.str(argv[1]); if (!(ss &gt;&gt; r)) { cerr &lt;&lt; "Bad rows\n"; return 1; }
  ss.clear(); ss.str(argv[2]); if (!(ss &gt;&gt; c)) { cerr &lt;&lt; "Bad cols\n"; return 1; } 
  ss.clear(); ss.str(argv[3]); if (!(ss &gt;&gt; p)) { cerr &lt;&lt; "Bad prime\n"; return 1; }

  <font color=blue>/* First create all of the elements of the matrix. */</font>

  vdm.resize(r);
  for (i = 0; i &lt; vdm.size(); i++) vdm[i].resize(c);

  <font color=blue>/* Next, calculate (i+1)^j mod p and put it into vdm[i][j] */</font>

  for (i = 0; i &lt; vdm.size(); i++) {
    base = i+1;
    val = 1;
    for (j = 0; j &lt; vdm[i].size(); j++) {
      vdm[i][j] = val;
      val = (val * base) % p;
    }
  }

  <font color=blue>/* Finally, print out the Vandermonde matrix. */</font>

  for (i = 0; i &lt; vdm.size(); i++) {
    for (j = 0; j &lt; vdm[i].size(); j++) printf(" %4d", vdm[i][j]);
    cout &lt;&lt; endl;
  }

  return 0;
}
</pre></td></table></center><p>


To start with, take a look at the way I declare the vector of vectors:

<p><center><table border=3 cellpadding=3><td><pre>
  vector &lt; vector &lt;int&gt; &gt; vdm;       <font color=blue>// The Vandermonde matrix</font>
</pre></td></table></center><p>

It's a good idea to separate the &gt;'s and &lt;'s with a space.  On some copilers, "&gt;&gt;"
is interpreted as a keyword (like with <b>cin</b>) and you'll get a compiler error if you omit
the space.  Other compilers are ok with no space -- since you never know, it's best to be safe
and keep the space.  I will always have the space.

<p>
We start by resizing <b>vdm</b> to be the number of rows.  When we do that, each vector element 
is an empty vector.  We go through each of these and resize it to be the number of columns.  Now
our matrix has <b>r*c</b> element.

<p>
Next, we set the elements by running through each row, and setting
<b>base</b> to * (i+1)* and <b>val</b> to one.  Now we calculate
* (i+1)<sup>j</sup>%p* by multiplying the previous element, which
is * (i+1)<sup>(j-1)</sup>%p* by * (i+1)*.  
When we're done, we have an <b>r * c</b>
Vandermonde matrix.  The second set of <b>for</b> loops prints it out. 

<pre>
UNIX> <font color=darkred><b>bin/vdm 1 1 101</b></font>
    1
UNIX> <font color=darkred><b>bin/vdm 3 3 101</b></font>
    1    1    1
    1    2    4
    1    3    9
UNIX> <font color=darkred><b>bin/vdm 3 5 101</b></font>
    1    1    1    1    1
    1    2    4    8   16
    1    3    9   27   81
UNIX> <font color=darkred><b>bin/vdm 3 5 7</b></font>
    1    1    1    1    1
    1    2    4    1    2
    1    3    2    6    4
UNIX> <font color=darkred><b></b></font>
</pre>

You should be able to verify to yourselves that all of the above matrices are Vandermonde matrices 
in their given fields.
<p>
---
<h3>Vectors of Vectors don't have to represent rectangular matrices</h3>

There's nothing that says we have to make each of the sub-vectors the same size.  
For example, consider Pascal's triangle.  This is a triangle of numbers
as pictured below:

<p><center><table border=3><td><img src=img/Pascals-Triangle-1.jpg width=400></td></table></center><p>

The numbers are arranged in rows, where row * i* has * i+1* elements (as always, our
lives are zero-indexed).  The first and last element in each row is equal to one.  
Each other element is the sum of the two elements above it.  Suppose we want to write
a program to generate Pascal's triangle in a data structure.  One easy way to do 
this is to generate it as a vector of integer vectors, where element * i* of the
vector is a vector containing the elements of row * i*.  We can visualize it below:

<p><center><table border=3><td><img src=img/Pascals-Triangle-2.jpg width=420></td></table></center><p>

Scanning for a pattern, let's consider the * j*-th element in row * i*.  
If it is the first or last element in the row, it will equal one.  Otherwise, you can 
see from the picture that it is equal to the sum of elements * j-1* and * j* in row * i-1*. 
That gives us a nice way to construct the triangle.  The code is in 
<b><a href=src/pascal.cpp>src/pascal.cpp</a></b>:

<p><center><table border=3 cellpadding=3><td><pre>
<font color=blue>/* This program creates Pascal's triangle and prints it out.
   It stores Pascal's triangle as a vector of vectors. */</font>

#include &lt;vector&gt;
#include &lt;iostream&gt;
#include &lt;sstream&gt;
#include &lt;cstdio&gt;
using namespace std;

int main(int argc, char **argv)
{
  int r;                               <font color=blue>// The number of rows</font>
  vector &lt; vector &lt;int&gt; &gt; pascal;      <font color=blue>// The vector of vectors that holds Pascal's triangle</font>

  size_t i, j;
  istringstream ss;

  <font color=blue>/* Error check the command line. */</font>

  if (argc != 2) { cerr &lt;&lt; "usage: pascal rows\n"; return 1; }
  ss.clear();  ss.str(argv[1]); if (!(ss &gt;&gt; r)) { cerr &lt;&lt; "Bad rows\n"; return 1; }

  <font color=blue>/* Create an entry in the vector for each row.  Then add values to each row
     by using push_back() with either the value one, or the sum of two values
     on the previous row. */</font>

  pascal.resize(r);
  for (i = 0; i &lt; pascal.size(); i++) {
    for (j = 0; j &lt;= i; j++) {
      if (j == 0 || j == i) {
        pascal[i].push_back(1);
      } else {
        pascal[i].push_back(pascal[i-1][j-1] + pascal[i-1][j]);
      }
    }
  }

  <font color=blue>/* Print out the vector of vectors. */</font>

  for (i = 0; i &lt; pascal.size(); i++) {
    for (j = 0; j &lt; pascal[i].size(); j++) printf(" %4d", pascal[i][j]);
    cout &lt;&lt; endl;
  }

  return 0;
}
</pre></td></table></center><p>

When we run it, it's pictured a little differently than above, but you should
see that it is clearly the same triangle:

<pre>
UNIX> <font color=darkred><b>bin/pascal 10</b></font>
    1
    1    1
    1    2    1
    1    3    3    1
    1    4    6    4    1
    1    5   10   10    5    1
    1    6   15   20   15    6    1
    1    7   21   35   35   21    7    1
    1    8   28   56   70   56   28    8    1
    1    9   36   84  126  126   84   36    9    1
UNIX> <font color=darkred><b></b></font>
</pre>

---
<h3>Making Copies of Things</h3>

C++ has default methods for making copies of data structures.  In
<b><a href=src/vcopy.cpp>src/vcopy.cpp</a></b>, we add a second vector of vectors
to the Pascal program.  

<p><center><table border=3 cellpadding=3><td><pre>
  vector &lt; vector &lt;int&gt; &gt; negative_pascal; <font color=blue>// This will be a copy, and we'll negate the elements.</font>
</pre></td></table></center><p>

At the end of the program, before we print anything out, we make a copy of <b>pascal</b>, 
and then we run through it and negate all of the elements:

<p><center><table border=3 cellpadding=3><td><pre>
  <font color=blue>/* Make a copy of pascal. */</font>

  negative_pascal = pascal;

  <font color=blue>/* Set each element of this to its negation. */</font>

  for (i = 0; i &lt; negative_pascal.size(); i++) {
    for (j = 0; j &lt; negative_pascal[i].size(); j++) {
      negative_pascal[i][j] = -negative_pascal[i][j];
    }
  }
</pre></td></table></center><p>
  
And we print both of them:

<pre>
UNIX> <font color=darkred><b>bin/vcopy 5</b></font>
</pre>
Pascal's Triangle:

    1
    1    1
    1    2    1
    1    3    3    1
    1    4    6    4    1

Pascal's Negative Triangle:

   -1
   -1   -1
   -1   -2   -1
   -1   -3   -3   -1
   -1   -4   -6   -4   -1
UNIX> 

This is nothing exciting, really, but I want to highlight how easy it was to copy
that vector of vectors.  One line:

<p><center><table border=3 cellpadding=3><td><pre>
negative_pascal = pascal;
</pre></td></table></center><p>
  
This is blessing and a curse.  It's a blessing, because it's a lot easier than
creating <b>negative_pascal</b> with loops and <b>push_back</b> commands.  It's a curse,
because you can burn megabytes of memory with a single line of code, and poor management
of memory is a speed and resource killer of computers.
<p>
So I want to you pay attention to when you make copies of things, because it is in fact
so easy!
 
