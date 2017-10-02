
ExpUtils - Experiment Utilities
===============================

Some reasons for existing
-------------------

After working in machine learning and reinforcement learning research for a year now, and in upper level
computer science research/study for a few years, I have found the need to create a library containing various
utilities to make research and experimentation using c++ easier. The need arose from the disparity between the
compiler on my preferred servers and personal computer, and many hours debugging compiler differences. Instead
of each project having to manage this issue for basic functionality this library will be unix specific for c++11 and
up and will hopefully mitigate this issue.

All bugs and comments are welcome, but as I am a current PhD student I will not have much time to fix this library if
it breaks. The library will be put onto the MIT license and most of the code is either my own, replicated from another
source, or my attempt at an ISO implementation for newer features. None of these have been tested completely thoroughly
and bugs are bound to be present in situations outside my purview. I am working on a complete test of the system using 
gtest but this will be slow going.

Why not boost?
---------------

There are a few reasons. First boost would only be useful for a few things in my projects, and the compile time and 
resource need to store the library is to much for the moment. In other words, boost is like killing a cockroach with an
atom bomb: it is too big a solution for my problem and there are consequences for such a solution that could require more
work than necessary. Also this library contains ideas and implementations for useful tools that are not available in Boost
that are specific to computer science experimentation.

Modules
=================


Filesystem
-----------
### Questions:
#### Why am I including an implementation of the c++17 ISO filesystem in this library? 

This is because I find having these ideas encapsulated very useful for file and data management. I also have gotten sick of keeping track of where to save files and data only to have misspelled a directory destroying my progress in an experiment. Having an implementation of a filesystem in c++ standard is useful to mitigate problems I may encounter due to my own typos. 

This should be mostly congruent with the c++17 ISO being implemented in compilers in the future. I like how this works and eventually can be removed and replaced with the std version once it becomes wide-spread. To see the full details of the ISO you can go to https://isocpp.org/files/papers/P0218r1.html and other sites. 

#### What is implemented?

Due to time constraints I have not quite filled out the entire implementation.
Currently I have what I need for it to be useful and will eventually fill out the rest of the implementation. Below is a list of features that are implemented and somewhat tested.


- class path;
- class directory_iterator;
- class recursive_directory_iterator;
- class file_status;
- file/directory creation and removal


Managers
-----------

There are many managers. A manager for saving files, reading files, a manager for keeping track of important values, a error manager to handle error calculations given a truth and prediction, and more. These are useful for many reasons and have been incredibly useful in cleaning up the main experimental code.

Save Manager
------------
